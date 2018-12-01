#include "Utils.h"
#include <assert.h>
#include <functional>


namespace dsalgo {

/**
 * Hashmap with linear probing.
 *
 * When entries are deleted, they leave behind a "ghost" element that still
 * counts towards the load factor. Once the hashmap resizes, "ghost" entries
 * will be removed.
 *
 * Key = type used for lookup
 * Val = type that gets mapped to in the hashmap
 * Hash = hash function for the key
 * Eq = equality function for the key
 */
template<
  class Key,
  class Val,
  class Hash=std::hash<Key>,
  class Eq=std::equal_to<Key>
  >
class Hashmap {

public:

  /**
   * Creates a hashamp with the specified initial maximum capacity and load
   * factor.
   *
   * @param init_capacity initial maximum capacity of the hashmap if the load
   * factor were 1. If this is not a power of 2, it will be rounded up to the
   * next power of 2. Using a table size that is a power of 2 allows optimizing
   * the modulo operation used to compute a key's index in the hash table.
   * @param load_factor how soon to resize the hashmap. When the hashmap
   * contains capacity * load_factor entries or ghost entries, it will double
   * in size.
   */
  Hashmap(int init_capacity, float load_factor=0.7)
      : load_factor_(load_factor) {

    if (init_capacity < 8) {
      init_capacity = 8;
    }

    // round capacity up if not power of 2.
    table_size_ = IsPowerOf2(init_capacity) ?
      init_capacity :
      NextPowerOf2(init_capacity);

    table_ = new Entry[table_size_]; 
  }

  Hashmap() : Hashmap(8) {}

  ~Hashmap() {
    if (table_ != nullptr) {
      delete[] table_;
    }
  }

  /**
   * Maps the given key to the given value.
   *
   * If the key is already mapped in the hashmap, it's value will be
   * overwritten.
   *
   * If the hashmap resizes after Put() is called, then all references and
   * pointers to values in the hashmap are invalidated.
   *
   * @param k key to insert
   * @param v value to which to map the key
   */
  void Put(const Key& k, const Val& v) {
    if (num_entries_ >= load_factor_ * table_size_) {
      Resize();
    }

    int insert_idx = LocateEntryIdx(k);
    Entry& insert_entry = table_[insert_idx];

    // updating entry - only have to update the value
    if (insert_entry.is_valid) {
      insert_entry.v = v;

    // inserting new entry - have to update both key and value
    } else {
      insert_entry.k = k;
      insert_entry.v = v;
      insert_entry.is_valid = true;
      ++num_entries_;
      ++size_;
    }
  }

  /**
   * Gets the value to which the given key is mapped.
   *
   * If the provided key is not already in the hashmap, then it will be inserted
   * into the hashmap and mapped to the default value (result of calling the
   * value's default constructor).
   *
   * Use Get() if you just want to check if a key is in the hashmap but
   * not create a new entry if the key doesn't exist.
   *
   * @param key to look up
   * @return the value to which the given key is mapped.
   */
  Val& operator[](const Key& k) const {
    int entry_idx = LocateEntryIdx(k);
    Entry& entry = table_[entry_idx];
    if (!entry.is_valid) {
      entry.is_valid = true;
    }
    return entry.v;
  }

  /**
   * @return the value associated with the given key, or nullptr if the key
   * is not in the hashmap.
   */
  Val* Get(const Key& k) const {
    int entry_idx = LocateEntryIdx(k);
    Entry& entry = table_[entry_idx];
    if (entry.is_valid) {
      return &entry.v;
    } else {
      return nullptr;
    }
  }

  /**
   * Removes the given key from the hashmap if present. Otherwise, does nothing.
   *
   * @param k the key to remove
   * @return if the key was removed
   */
  bool Remove(const Key& k) {
    int entry_idx = LocateEntryIdx(k);
    Entry& entry = table_[entry_idx];
    if (entry.is_valid) {
      entry.is_valid = false;
      entry.is_deleted = true;
      --size_;
      return true;
    }
    return false;
  }

  /**
   * @return number of entries in this hashmap.
   */
  int Size() const {
    return size_;
  }

private:

  /**
   * Represents an entry in the hashmap.
   */
  struct Entry {
    Key k;
    Val v;

    // if this entry represents something that has been inserted into the
    // hashmap, as opposed to being unitialized space in the underlying table.
    bool is_valid = false;

    // if the entry is deleted, then it is a "ghost" element. An entry should
    // never have (is_valid == true) and (is_deleted == true). But an entry
    // can have (is_valid == false) and (is_deleted == true/false)
    bool is_deleted = false;
  };

  /**
   * @return hash code for the given key
   */
  inline int HashCode(const Key& k) const {
    return hash_fn_(k);
  }

  /**
   * @return the index in the underlying table at which the key belongs. Does
   * not account for collisions in which lineary probing is required.
   */
  inline int IndexFor(const Key& k) const {
    return HashCode(k) & (table_size_ - 1);
  }

  /**
   * @return if the two keys are equivalent
   */
  inline bool Equals(const Key& k1, const Key& k2) const {
    return eq_fn_(k1, k2);
  }

  /**
   * @return index in the underlying table at which the key is located if the
   * key is in the hashmap. Otherwise, returns the index at which the key should
   * be inserted.
   */
  int LocateEntryIdx(const Key& k) const {
    int expected_idx = IndexFor(k); 
    int idx_to_check = expected_idx;

    for (int i = 0; i < table_size_; ++i) {
      const Entry& entry_to_check = table_[idx_to_check];

      // we've made an internal error if the entry is both valid and deleted
      assert(!(entry_to_check.is_valid && entry_to_check.is_deleted));

      // is the entry the one we're looking for?
      bool is_correct_entry = (entry_to_check.is_valid &&
          Equals(k, entry_to_check.k));

      // is the current spot occupied? (i.e. could we insert the element here?)
      bool is_unallocated_entry = (!entry_to_check.is_valid &&
          !entry_to_check.is_deleted);

      // if we found the correct entry, then we're done.
      // if we reached an unoccupied spot, that means the key isn't in the
      // hashtable and we're also done.
      if (is_correct_entry || is_unallocated_entry) {
        return idx_to_check; 
      }

      // if we reach here, then the entry we're checking is either deleted or
      // valid but not the one we're looking for

      // apply linear probing and advance forward to the next index,
      // rolling over to index 0 if we reach the end of the hashtable 
      idx_to_check = (idx_to_check + 1) & (table_size_ - 1);
    }

    // should never reach here
    assert(false);
    return 0;
  }

  void Resize() {

    // back up old table data
    Entry* old_table = table_;
    int old_table_size = table_size_;

    // compute new size required to maintain load factor
    int new_table_size = table_size_;
    while (size_ >= new_table_size * load_factor_) {
      new_table_size *= 2;
    }

    // create new table
    table_size_ = new_table_size;
    table_ = new Entry[table_size_];

    // re-insert all the valid elements in the old table
    for (int i = 0; i < old_table_size; ++i) {
      if (old_table[i].is_valid) {
        int insert_idx = LocateEntryIdx(old_table[i].k);
        table_[insert_idx] = std::move(old_table[i]);
      }
    }

    // all ghost-entries have been discarded during the resize, so num_entries_
    // is now the same as size_
    num_entries_ = size_;

    // free old table memory
    delete[] old_table;
  }

private:

  // underlying table for the hashmap
  Entry* table_ = nullptr;
  
  // size of the underlying table, must be a power of 2.
  int table_size_ = 0;

  // number of entries in the hashmap, including ghost entries.
  int num_entries_ = 0;

  // number of entries in the hashmap excluding ghost entries. This is
  // the number of entries the user of the hashmap should be aware of.
  int size_ = 0;

  // hashmap will resize when number of entires or ghost entries exceeds
  // table_size_ * load_factor_
  float load_factor_ = 0;

  Hash hash_fn_;

  Eq eq_fn_;
};

} // namespace dsalgo

