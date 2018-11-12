#pragma once

#include "Utils.h"
#include <assert.h>
#include <vector>
#include <unordered_map>


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
  LruQueue(const std::vector<T>& elements) {
    assert(elements.size() >= 1);
    for (const T& e : elements) {
      // You cannot have a well-defined LRU if there are duplicate elements.
      // If an element is marked as used and there are two of them, you wouldn't
      // know which one to indicate as most-recently used.
      assert(elem_to_entry_.find(e) == elem_to_entry_.end());
      LruEntry* new_entry = CreateNewLruEntry(e);
      AppendLruEntry(new_entry);
    }
  }

  ~LruQueue() {
    FreeMemory();
  }

  LruQueue(const LruQueue<T>& other) {
    CopyFrom(other);
  }

  LruQueue& operator=(const LruQueue& other) {
    FreeMemory();
    CopyFrom(other);
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
    assert(elem_to_entry_.find(elem) != elem_to_entry_.end());
    LruEntry* entry_to_mark_used = elem_to_entry_.at(elem);
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

    LruEntry(const T& e) : e_(e) {}
  };

  /**
   * Releases all heap memory allocated by this LruQueue.
   */
  void FreeMemory() {
    LruEntry* curr_entry = lru_;
    while (curr_entry != nullptr) {
      LruEntry* next_entry = curr_entry->next_;
      delete curr_entry;
      curr_entry = next_entry;
    }
    lru_ = nullptr;
    mru_ = nullptr;
  }

  /**
   * Deep Copies all elements from another LruQueue (LRU order is also preserved
   * by the copy).
   */
  void CopyFrom(const LruQueue<T>& other) {
    LruEntry* curr_entry = other.lru_;
    while (curr_entry != nullptr) {
      LruEntry* next_entry = curr_entry->next_;
      LruEntry* new_entry = CreateNewLruEntry(curr_entry->e_);
      AppendLruEntry(new_entry);
      curr_entry = next_entry;
    }
  }

  /**
   * Creates a new entry for the LruQueue's linked list.
   *
   * The element is populated, but the next and previous markers are not.
   */
  LruEntry* CreateNewLruEntry(const T& e) {
    LruEntry* new_entry = new LruEntry(e);
    return new_entry;
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
    elem_to_entry_[entry->e_] = entry;
  }

  /**
   * Removes an LruEntry from the linked list, but DOES NOT delete it.
   *
   * @param entry The LruEntry to remove from the linked list
   */
  void RemoveLruEntry(LruEntry* entry) {
    LruEntry* prev_entry = entry->prev_;
    LruEntry* next_entry = entry->next_;
    if (prev_entry != nullptr) {
      prev_entry->next_ = next_entry;
    }
    if (next_entry != nullptr) {
      next_entry->prev_ = prev_entry;
    }
    entry->next_ = nullptr;
    entry->prev_ = nullptr;
    if (entry == lru_) {
      lru_ = next_entry;
    }
    if (entry == mru_) {
      mru_ = prev_entry;
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
   * Hashes elements to their LruEntry.
   */
  std::unordered_map<T, LruEntry*> elem_to_entry_;

};

} // namspace dsalgo

