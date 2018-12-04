#pragma once

#include "Hashmap.h"
#include "Utils.h"
#include <assert.h>
#include <new>
#include <vector>


namespace dsalgo {

/**
 * Queue for selecting the next least-recently used element. All elements must
 * be provided at construction time. Any element can be marked as "used" at any
 * time. After construction, GetLru() and MarkUsed() are both O(1).
 *
 * Every element in the LruQueue should have a unique hash.
 */
template <typename T>
class LruQueue {

public:

  /**
   * Constructs an LruQueue for the given elements.
   *
   * @param elements The elements that the LruQueue should manage. The first
   * element in the list is assumed to be least-recently used, the second
   * element is assumed to be second leas-recently used, and so on. The last
   * element in the list is assumed to be most-recently used.
   */
  LruQueue(const std::vector<T>& elements) : size_(elements.size()) {
    assert(elements.size() >= 1);

    // operations will benefit from cache locality if we allocate a block of
    // memory up front rather than allocating one node of the linked list
    // at a time.
    mem_ = new LruEntry[elements.size()];

    for (int i = 0; i < static_cast<int>(elements.size()); ++i) {
      // You cannot have a well-defined LRU if there are duplicate elements.
      // If an element is marked as used and there are two of them, you wouldn't
      // know which one to indicate as most-recently used.
      assert(elem_to_entry_.Get(elements[i]) == nullptr);
      PlaceNewLruEntry(mem_ + i, elements[i]);
      AppendLruEntry(mem_ + i);
      elem_to_entry_.Put(mem_[i].e_, (mem_ + i));
    }
  }

  ~LruQueue() {
    FreeMemory();
  }

  LruQueue(const LruQueue<T>& other) {
    CopyFrom(other);
  }

  LruQueue(LruQueue<T>&& other) noexcept {
    MoveFrom(other);
  }

  LruQueue& operator=(const LruQueue& other) {
    FreeMemory();
    CopyFrom(other);
    return *this;
  }
  
  LruQueue& operator=(LruQueue&& other) {
    FreeMemory();
    MoveFrom(other);
    return *this;
  }

  /**
   * @return The least-recently used element.
   */
  const T& GetLru() const {
    return lru_->e_;
  }

  /**
   * Marks the given element as used and moves it to the end of the queue.
   *
   * @param elem The element to mark as used.
   */
  void MarkUsed(const T& elem) {
    assert(elem_to_entry_.Get(elem) != nullptr);
    LruEntry* entry_to_mark_used = *elem_to_entry_.Get(elem);
    if (entry_to_mark_used != mru_) {
      RemoveLruEntry(entry_to_mark_used);
      AppendLruEntry(entry_to_mark_used);
    }
  }

private:

  /**
   * An entry in the linked list of elements.
   */
  struct LruEntry {
    T e_;
    LruEntry* next_ = nullptr;
    LruEntry* prev_ = nullptr;

    LruEntry() {}
    LruEntry(const T& e) : e_(e) {}
  };

  /**
   * Releases all heap memory allocated by this LruQueue.
   */
  void FreeMemory() {
    if (mem_ != nullptr) {
      delete[] mem_;
    }
    lru_ = nullptr;
    mru_ = nullptr;
    mem_ = nullptr;
  }

  /**
   * Moves another LruQueue's contents into this queue. The other queue is
   * emptied out.
   */
  void MoveFrom(LruQueue<T>& other) {
    lru_ = other.lru_;
    mru_ = other.mru_;
    mem_ = other.mem_;
    size_ = other.size_;
    elem_to_entry_ = std::move(other.elem_to_entry_);
    other.lru_ = nullptr;
    other.mru_ = nullptr;
    other.mem_ = nullptr;
    other.size_ = 0;
  }

  /**
   * Deep copies all elements from another LruQueue (LRU order is also preserved
   * by the copy).
   *
   * Note that no memory clean up is performed before copying. Call FreeMemory()
   * before calling CopyFrom() if you need to clean up the current object.
   */
  void CopyFrom(const LruQueue<T>& other) {
    size_ = other.size_;
    mem_ = new LruEntry[size_];
    std::copy(other.mem_, other.mem_ + other.size_, mem_);
    for (int i = 0; i < size_; ++i) {
      elem_to_entry_.Put(mem_[i].e_, (mem_ + i));
    }

    LruEntry* curr_entry = other.lru_;
    while (curr_entry != nullptr) {
      LruEntry* next_entry = curr_entry->next_;
      AppendLruEntry(*elem_to_entry_.Get(curr_entry->e_));
      curr_entry = next_entry;
    }
  }

  /**
   * Creates a new entry for the LruQueue's linked list via placement_new.
   *
   * @param alloced_mem Memory that has already been allocated on the heap.
   * @param e new element to add to the LRU.
   */
  void PlaceNewLruEntry(LruEntry* alloced_mem, const T& e) {
    new (alloced_mem) LruEntry(e); 
  }

  /**
   * Adds the given entry to the end of the LruQueue's linked list. The entry
   * is then the most-recently used element.
   */
  void AppendLruEntry(LruEntry* entry) {

    // if list is non-empty
    if (lru_ != nullptr) {

      // append new entry to the end
      mru_->next_ = entry;
      entry->prev_ = mru_;
      mru_ = entry;

    // if list is empty
    } else {
      lru_ = entry;
      mru_ = entry;
    }
  }

  /**
   * Removes an LruEntry from the linked list, but DOES NOT delete it.
   *
   * @param entry The LruEntry to remove from the linked list
   */
  void RemoveLruEntry(LruEntry* entry) {

    // optimize for removing the LRU because that's the point of using an
    // LRU queue.
    if (LIKELY(entry == lru_)) {
      lru_ = entry->next_;
      lru_->prev_ = nullptr;
      entry->next_ = nullptr;
    } else {
      LruEntry* prev_entry = entry->prev_;
      LruEntry* next_entry = entry->next_;
      prev_entry->next_ = next_entry;
      if (next_entry != nullptr) {
        next_entry->prev_ = prev_entry;
      }
      entry->next_ = nullptr;
      entry->prev_ = nullptr;
      if (entry == mru_) {
        mru_ = prev_entry;
      }
    }
  }

private:

  /**
   * The head of the linked list - the least recently used element.
   */
  LruEntry* lru_ = nullptr;

  /**
   * The tail of the linked list - the most recently used element.
   */
  LruEntry* mru_ = nullptr;

  /**
   * Memory allocated for the linked list.
   */
  LruEntry* mem_ = nullptr;

  int size_ = 0;

  /**
   * Hashes elements to their LruEntry.
   *
   * TODO this is very slow and we should use a custom linear-probing
   * hashmap instead.
   */
  Hashmap<T, LruEntry*> elem_to_entry_;

};

} // namspace dsalgo

