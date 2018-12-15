#include <atomic>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


namespace dsalgo {

enum ShmQueueStatus {
  // deque was fine
  OK,

  // deque didn't have as many bytes as requested
  TRUNCATED,

  // the handle has fallen behind too much that it got evicted from the queue
  EVICTED
};

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
    hdr_ = reinterpret_cast<ShmHeader*>(shared_mem_);
    hdr_->capacity_ = capacity;
    hdr_->tail_idx_ = 0;
    hdr_->epoch_ = 0;
    hdr_->write_lock_.clear();

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
    char* hdr_mem = reinterpret_cast<char*>(mmap(NULL, sizeof(ShmHeader),
          PROT_READ | PROT_WRITE, MAP_SHARED, shm_file_fd_, 0));
    hdr_ = reinterpret_cast<ShmHeader*>(hdr_mem);
    int queue_size = sizeof(ShmHeader) + (sizeof(char) * hdr_->capacity_);
    munmap(hdr_mem, sizeof(ShmHeader));

    // once we've read in the header, we know how big the queue is and can
    // mmap the circular buffer that is part of the shared memory queue
    shared_mem_ = reinterpret_cast<char*>(mmap(NULL, queue_size,
          PROT_READ | PROT_WRITE, MAP_SHARED, shm_file_fd_, 0));

    hdr_ = reinterpret_cast<ShmHeader*>(shared_mem_);
    buf_ = shared_mem_ + sizeof(ShmHeader);
  }

  ~ShmQueue() {
    if (hdr_ != nullptr) {
      munmap(shared_mem_, sizeof(ShmHeader) +
          (sizeof(char) * hdr_->capacity_));
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

    // wait for all writes to finish so that we can set the tail and epoch.
    // We don't want to get the tail and epoch out of sync if a write operation
    // is currently updating both variables.
    while (hdr_->write_lock_.test_and_set());
    rtn.idx = hdr_->tail_idx_;
    rtn.epoch = hdr_->epoch_;

    // allow writing again
    hdr_->write_lock_.clear();

    return rtn;
  }

  /**
   * @return if the given handle is still valid for this shared memory queue.
   * Handles are evicted if they take too long to read from the queue and
   * the buffer has already wrapped around and caught up to the handle's
   * head.
   */
  inline bool IsHandleEvicted(ShmQueueHandle* handle) {
    // if the tail is greater on or after the handle index, then the epoch must
    // be the same. If the epoch of the queue is greater than the epoch of the
    // handle, then the queue has already wrapped-around and passed the
    // handle.
    if (hdr_->tail_extension_idx_ >= handle->idx) {
      return hdr_->epoch_ != handle->epoch; 
    }

    // if the tail if before the handle's index, then the epoch must be one
    // greater than the handle.
    else {
      return hdr_->epoch_ != (handle->epoch + 1);
    }
  }

  /**
   * Attempts to deque up to size bytes from the queue.
   *
   * If there are size bytes available, they will all be dequed. If there are
   * not size bytes available, all the remaining bytes will be dequed.
   * 
   * @param buf Buffer in which to store dequeued bytes. Must be enough to hold
   * size bytes.
   * @param size maximum number of bytes to deque
   * @param status stores the status of the dequeing (e.g. EVICTED if the handle
   * has been evicted, or TRUNCATED if there weren't enough bytes to read.
   * @return Number of bytes dequeued.
   */
  int Dequeue(ShmQueueHandle* handle, char* buf, int size, int& status) {
    // check if evicted at the beginning so that if we are evicted, we can
    // stop without doing extra work.
    if (IsHandleEvicted(handle)) {
      status = ShmQueueStatus::EVICTED;
      return 0;
    }
    
    // have to copy the header's tail index atomically here in case it's being
    // changed by a enqueue operation while we're performing this dequeue.
    int tail_idx = hdr_->tail_idx_;

    bool no_wraparound = tail_idx >= handle->idx;
    int bytes_available = no_wraparound ?
        (tail_idx - handle->idx) : (hdr_->capacity_ - handle->idx + tail_idx);
    int bytes_to_deque = std::min(bytes_available, size);

    // dequeing size bytes does not require any wraparound iff the queue
    // itself isn't wrapped around or the queue has wrapped around but reading
    // the requested number of  bytes from the head won't require wrapping
    // around.
    bool no_deque_wraparound = (no_wraparound ||
        bytes_to_deque < (hdr_->capacity_ - tail_idx));

    // no wraparound needed for this deque size
    // no-wraparound is more common than wraparound, so the no-wraparound case
    // comes first
    if (no_deque_wraparound) {
      std::memcpy(buf, buf_ + handle->idx, bytes_to_deque);

    // wraparound needed for this deque size
    } else {
      int bytes_to_end = (hdr_->capacity_ - tail_idx);
      std::memcpy(buf, buf_ + handle->idx, bytes_to_end);

      int bytes_from_beginning = bytes_to_deque - bytes_to_end;
      std::memcpy(buf, buf_, bytes_from_beginning);
    }
    
    // have to also check that we weren't evicted while reading.
    if (IsHandleEvicted(handle)) {
      status = ShmQueueStatus::EVICTED;
      return 0;
    }

    // update the handle's epoch and index and return the number of bytes read
    if (no_deque_wraparound) {
      handle->idx += bytes_to_deque;
    } else {
      handle->idx += bytes_to_deque - hdr_->capacity_;
      ++handle->epoch;
    }
    return (bytes_to_deque == size) ?
        ShmQueueStatus::OK : ShmQueueStatus::TRUNCATED;
  }

  /**
   * Enqueue some bytes into the queue.
   * 
   * The capacity of the queue should be much greater than the typical
   * enqueue size. Otherwise, any dequeuing process will constantly be
   * evicted when the circular buffer wraps around and catches up to the
   * reader.
   *
   * You cannot enqueue anything larger than the queue's capacity.
   *
   * @param bytes buffer of bytes to enqueue
   * @param size number of bytes to enqueue
   * @throw logic_error if the number of bytes to enqueue is greater than the
   * capacity of the queue
   */
  void Enqueue(char* bytes, int size) {
    if (size > hdr_->capacity_) {
      throw std::logic_error(
          "Cannot enqueue more bytes than the capacity of the queue.");
    }

    // wait until no other process is writing
    while (hdr_->write_lock_.test_and_set());

    int end_idx = hdr_->tail_idx_ + size;

    // no wraparound
    if (end_idx < hdr_->capacity_) {

      // notify all consumers that we're overwritting everything from
      // [tail_idx, tail_extension_idx). This will be an atomic operation.
      hdr_->tail_extension_idx_ = end_idx;

      // now, any consumer who's head index is in the range
      // [tail_idx, tail_extension_idx) is evicted.
      //
      // We can safely copy the bytes over now.
      std::memcpy(buf_ + hdr_->tail_idx_, bytes, size);

      // notify all consumers that the writing is finished by moving the
      // tail_idx up to tail_extension_idx. This will be an atomic operation
      // as well.
      hdr_->tail_idx_ = hdr_->tail_extension_idx_;

    // wraparound
    } else {

      end_idx -= hdr_->capacity_;

      // notify all consumers we're overwriting everything from 
      // [tail_idx, end) and from [begin, tail_extension_idx). This will
      // be an atomic operation.
      hdr_->tail_extension_idx_ = end_idx;

      // now, any consumer who's head index is in the range
      // [tail_idx, end) or [begin, tail_extension_idx) is evicted.
      //
      // We can safely copy the bytes over now.

      // copy to end
      int bytes_to_end = (hdr_->capacity_ - hdr_->tail_idx_);
      std::memcpy(buf_ + hdr_->tail_idx_, bytes, bytes_to_end);

      // wraparound to beginning and copy
      std::memcpy(buf_, bytes + bytes_to_end, (size - bytes_to_end));

      // increment epoch since we wrapped around
      hdr_->epoch_ += 1;

      // notify all consumers that the writing is finished by moving the
      // tail_idx up the tail_extension_idx. This will be an atomic operation
      // as well.
      hdr_->tail_idx_ = (size - bytes_to_end);
    }

    // indicate that we're done writing
    hdr_->write_lock_.clear();
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
     * Indicates how far an ongoing enqueue will extend the tail. If a process
     * is enqueuing, it will need to get exclusive access to the space
     * on the buffer where it is going to write the new data. The handles for
     * consumers that are so far behind that the tail extension has wrapped
     * around and caught up will be evicted.
     */
    int tail_extension_idx_;

    /**
     * Epoch on the shared memory queue (number of cycles we've gone around the
     * shared memory circular buffer).
     */
    int epoch_;

    /**
     * Only one process can be enqueuing at a single time. This flag prevents
     * multiple processes from enqueuing.
     */
    std::atomic_flag write_lock_;
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
  ShmHeader* hdr_ = nullptr;

  /**
   * Circular buffer of shared memory (points to right after the ShmHeader)
   */
  char* buf_ = nullptr;

};

} // namespace dsalgo

