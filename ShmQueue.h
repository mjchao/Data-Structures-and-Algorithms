#include <atomic>
#include <iostream>
#include <stdexcept>
#include <string>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


namespace dsalgo {

/**
 * Handle that is required for accessing the shared memory queue.
 */
struct ShmQueueHandle {
  int idx = 0;
  int epoch = 0;
};

/**
 * A circular-buffer queue stored in shared memory.
 */
class ShmQueue {

public:

  /**
   * Creates a ShmQueue by mapping to a new shared memory region.
   *
   * @param file in which shared memory region can be swapped to disk
   * @param capacity the capacity of the shared memory in bytes
   * @param overwrite overwrite the current shared memory file if it exists
   * @throws runtime_error if we could not create the shared memory file
   */
  ShmQueue(const std::string& filename, int capacity, bool overwrite)
      : shm_file_(filename)
  {

    if (capacity <= 0) {
      throw std::logic_error("Capacity of shared memory should be positive.");
    }

    // if the file already exists, we should not overwrite it in case another
    // shmqueue is using it.
    struct stat buffer;   
    if (!overwrite && stat(shm_file_.c_str(), &buffer) == 0) {
      throw std::runtime_error("Cannot create new file " + shm_file_ +
          " for swapping out shared memory because it already exists."
          " Specify overwrite=true to overwrite it.");
    }

    // open file for shmqueue to use
    shm_file_fd_ = open(shm_file_.c_str(), O_CREAT|O_RDWR, 00666); 
    if (shm_file_fd_ < 0) {
      throw std::runtime_error("Could not open " + shm_file_ +
          " for swapping out shared memory.");
    }

    // calculate how big (in bytes) the ShmQueue will be.
    int queue_size = sizeof(ShmHeader) + (sizeof(char) * capacity);

    // allocate enough disk space for swapping out the entire mmap'ed region
    ftruncate(shm_file_fd_, queue_size);
    shared_mem_ = reinterpret_cast<char*>(mmap(NULL, queue_size,
          PROT_READ | PROT_WRITE, MAP_SHARED, shm_file_fd_, 0));

    // set the header
    queue_hdr_ = reinterpret_cast<ShmHeader*>(shared_mem_);
    queue_hdr_->capacity_ = capacity;
    queue_hdr_->tail_idx_ = 0;
    queue_hdr_->epoch_ = 0;

    // set the buffer, don't have to waste time initializing everything to 0
    // though
    buf_ = shared_mem_ + sizeof(ShmHeader);
  }

  /**
   * Creates a shared memory queue from an existing shared memory region.
   *
   * @param filename file in which a shared memory region is currently being
   * swapped out to
   * @throws runtime_error if we could not load the shared memory file
   */
  ShmQueue(const std::string& filename)
      : shm_file_(filename)
  {

    struct stat buffer;   
    if (stat(shm_file_.c_str(), &buffer) != 0) {
      throw std::runtime_error("Cannot load from shm file " + shm_file_ +
          " because it does not exist. Use Create() to create a new shm file.");
    }

    shm_file_fd_ = open(shm_file_.c_str(), O_RDWR, 00666);
    if (shm_file_fd_ < 0) {
      throw std::runtime_error("Could not open " + shm_file_ +
          " to load the shared memory region.");
    }

    // load queue header first
    char* queue_hdr_mem = reinterpret_cast<char*>(mmap(NULL, sizeof(ShmHeader),
          PROT_READ | PROT_WRITE, MAP_SHARED, shm_file_fd_, 0));
    queue_hdr_ = reinterpret_cast<ShmHeader*>(queue_hdr_mem);
    int queue_size = sizeof(ShmHeader) + (sizeof(char) * queue_hdr_->capacity_);
    munmap(queue_hdr_mem, sizeof(ShmHeader));

    // once we've read in the header, we know how big the queue is and can
    // mmap the circular buffer that is part of the shared memory queue
    shared_mem_ = reinterpret_cast<char*>(mmap(NULL, queue_size,
          PROT_READ | PROT_WRITE, MAP_SHARED, shm_file_fd_, 0));

    queue_hdr_ = reinterpret_cast<ShmHeader*>(shared_mem_);
    buf_ = shared_mem_ + sizeof(ShmHeader);
  }

  ~ShmQueue() {
    if (queue_hdr_ != nullptr) {
      munmap(shared_mem_, sizeof(ShmHeader) +
          (sizeof(char) * queue_hdr_->capacity_));
    }
  }

  /**
   * Creates a new handle that can be provided for reading the shared memory
   * queue.
   *
   * The new handle provides access to anything that is enqueued after its
   * creation.
   *
   * @return a new handle for reading in the shared memory queue.
   */
  ShmQueueHandle NewHandle() {
    ShmQueueHandle rtn;
    rtn.idx = queue_hdr_->tail_idx_;
    rtn.epoch = queue_hdr_->epoch_;
    return rtn;
  }

private:

  /**
   * Stores variables required to synchronize access
   */
  struct ShmHeader {
    /**
     * Capacity of the shared memory. Constant, should never change except
     * during creation of the ShmQueue.
     */
    int capacity_;

    /**
     * Index of the tail of the queue. mmap will page-align the memory region
     * it gives us, so tail_idx_ will bye 4-byte aligned and can be
     * updated atomically by an x86 processor.
     */
    int tail_idx_;

    /**
     * Epoch on the shared memory queue (number of cycles we've gone around the
     * shared memory circular buffer).
     */
    int epoch_;
    
  };

private:

  /**
   * Name of disk file on which shared memory can be swapped out onto.
   */
  const std::string shm_file_ = "no-file-specified";

  /**
   * File descriptor for the shared memory file.
   */
  int shm_file_fd_ = -1;

  /**
   * Shared memory (points to the ShmHeader)
   */
  char* shared_mem_ = nullptr;

  /**
   * Header containing details about this queue (such as its capacity)
   */
  ShmHeader* queue_hdr_ = nullptr;

  /**
   * Circular buffer of shared memory (points to right after the ShmHeader)
   */
  char* buf_ = nullptr;

};

} // namespace dsalgo

