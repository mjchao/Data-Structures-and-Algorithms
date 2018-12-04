#include "Utils.h"
#include <assert.h>
#include <functional>
#include <iostream>


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
    FreeMem();
  }

  Hashmap(const Hashmap<Key, Val, Hash, Eq>& other) {
    CopyFrom(other);
  }

  Hashmap(Hashmap<Key, Val, Hash, Eq>&& other) noexcept {
    MoveFrom(other);
  }

  Hashmap<Key, Val, Hash, Eq>& operator=(
      const Hashmap<Key, Val, Hash, Eq>& other) {
    FreeMem();
    CopyFrom(other);
    return *this;
  }

  Hashmap<Key, Val, Hash, Eq>& operator=(
      Hashmap<Key, Val, Hash, Eq> && other) noexcept {
    FreeMem();
    MoveFrom(other);
    return *this;
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
    if (size_ >= load_factor_ * table_size_) {
      Resize();
    }

    int hashcode = HashCode(k);
    int insert_idx = LocateEntryIdx(k);
    assert(insert_idx != -1);
    Entry& insert_entry = table_[insert_idx];

    // updating entry - only have to update the value
    if (insert_entry.IsValid()) {
      insert_entry.v = v;

    // inserting new entry - have to update both key and value
    } else {
      insert_entry.k = k;
      insert_entry.v = v;
      insert_entry.hashcode = hashcode;
      ++size_;
    }
  }

  /**
   * Gets the value to which the given key is mapped.
   *
   * If the provided key is not already in the hashmap, then it will be inserted
   * into the hashmap and mapped to the default value (calls the default
   * constructor).
   *
   * This is slower than using Put/Get() if you already know whether or not
   * the element is in the hashmap.
   *
   * Use Get() if you just want to check if a key is in the hashmap but
   * not create a new entry if the key doesn't exist.
   *
   * @param key to look up
   * @return the value to which the given key is mapped.
   */
  Val& operator[](const Key& k) {
    Val* v = Get(k);
    if (v != nullptr) {
      return *v;
    }
    Put(k, Val());
    return *Get(k);
  }

  /**
   * @return the value associated with the given key, or nullptr if the key
   * is not in the hashmap.
   */
  Val* Get(const Key& k) const {
    int entry_idx = LocateEntryIdx(k);
    if (entry_idx == -1) {
      return nullptr;
    }

    Entry& entry = table_[entry_idx];
    if (entry.IsValid()) {
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
    int remove_idx = LocateEntryIdx(k);
    if (remove_idx == -1) {
      return false;
    }

    Entry& entry_to_remove = table_[remove_idx];
    if (entry_to_remove.IsValid()) {
      int unoccupied_idx = remove_idx;
      int idx_to_check = unoccupied_idx;
      while (true) {
        idx_to_check = (idx_to_check + 1) & (table_size_ - 1);

        // if we reach an unoccupied spot, we're done
        if (!table_[idx_to_check].IsValid()) {
          break;
        }

        // if table is completely full and we've gone one full cycle around
        // then we're done
        if (idx_to_check == remove_idx) {
          break;
        }

        // this is where the entry we're checking should belong if there
        // were no collisions, a.k.a the canonical index
        int idx_for_entry_to_check = IndexFor(table_[idx_to_check].hashcode);

        // But if there were collisions and the entry was shifted via linear
        // probing, we *may* be able to move it into the unoccupied spot
        // under two conditions.
        //
        // At this point, every element from [remove_idx, idx_to_check] is
        // occupied except for the element at unoccupied_idx.

        // If there is no wraparound yet, then we can move the entry into the
        // unoccupied spot if...
        //    1. the candidate entry's canonical index in the table is earlier
        //       than the unoccupied spot. This means we're just shifting the
        //       candidate entry earlier in the chain of collisions, which will
        //       speed up finding it in future searches.
        //                              OR
        //    2. the candidate entry's canonical index in the table is after the
        //       current index that we're checking. If the candidate was
        //       properly placed at the current index we're checking, then
        //       everything from [0, idx_to_check] and from
        //       [idx_for_entry_to_check, table_end] must be occupied. This
        //       means that the candidate entry belongs at
        //       idx_for_entry_to_check and wrapped all the way around due
        //       to collisions with the linear probing. So by moving the
        //       candidate entry to an earlier spot, we're just shifting the
        //       candidate entry to earlier in the chain of collisions.
        //
        // In either case, we're just shifting the candidate entry to earlier
        // in the chain of collisions.
        bool not_wrapped_around_and_can_move =
          (idx_to_check > unoccupied_idx) &&
          (idx_for_entry_to_check <= unoccupied_idx ||
           idx_for_entry_to_check > idx_to_check);

        // If there is wrapround, then we can move the entry into the unoccupied
        // spot if...
        //    1. the candidate entry's canonical index in the table is earlier
        //       than the unoccupied spot.
        //                          AND
        //    2. the candidate entry's canonical index in the table is greater
        //       than the current index we're checking.
        //
        // The first condition guarantees that we're shifting the element
        // earlier in the chain of collisions. The second condition guarantees
        // we're not moving an element farther away from where it should be and
        // thus introducing a "gap" in it's linear probing chain.
        //
        // As an example of why not remove when
        // idx_for_entry_to_check <= idx_to_check, consider this table:
        //                         [a, b, c, d]
        // where b, c and d hash to index 3 and a hashes to index 0. (So 
        // we inserted d, then a, then b, and then c).
        //
        // If we delete d, then we get an unoccupied space at index 3.
        //                        [a, b, c, -]
        // The remove algorithm then considers moving a down to replace d.
        // But we shouldn't move a because it's already in the correct spot!
        // It's only when idx_for_entry_to_check > idx_to_check (indicating
        // that the candidate entry wrapped around due to collisions) that we're
        // actually moving the element closer to it's canonical index.
        bool wrapped_around_and_can_move = (idx_to_check < unoccupied_idx) &&
          (idx_for_entry_to_check <= unoccupied_idx &&
           idx_for_entry_to_check > idx_to_check);

        if (not_wrapped_around_and_can_move || wrapped_around_and_can_move) {
          table_[unoccupied_idx] = std::move(table_[idx_to_check]); 
          unoccupied_idx = idx_to_check;
        }
      }
      table_[unoccupied_idx].Invalidate();
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

  /**
   * Removes all elements from this hashmap.
   */
  void Clear() {
    for (int i = 0; i < table_size_; ++i) {
      Entry& to_clear = table_[i];
      to_clear.Invalidate();
    }
    size_ = 0;
  }

private:

  /**
   * Represents an entry in the hashmap.
   */
  struct Entry {
    Key k;
    Val v;
    // if hashcode is -1, that means the element is not valid.
    int hashcode = -1;

    /**
     * @return if this is a valid hashmap entry (as opposed to just unused
     * memory in the table)
     */
    inline bool IsValid() const {
      return hashcode != -1;
    }

    /**
     * Marks this entry as unallocated
     */
    inline void Invalidate() {
      hashcode = -1;
    }
  };

  /**
   * Frees any memory that has been allocated for this hashtable.
   */
  void FreeMem() {
    if (table_ != nullptr) {
      delete[] table_;
    }
  }

  /**
   * Copies another hashtable into this hashtable. Does not free any currently
   * allocated memory though.
   */
  void CopyFrom(const Hashmap<Key, Val, Hash, Eq>& other) {
    table_ = new Entry[other.table_size_];  
    std::copy(other.table_, other.table_ + other.table_size_, table_);
    table_size_ = other.table_size_;
    size_ = other.size_;
    load_factor_ = other.load_factor_;
    hash_fn_ = other.hash_fn_;
    eq_fn_ = other.eq_fn_;
  }

  /**
   * Moves another hastable into this hashtable. The other hashtable is
   * emptied and invalidated.
   */
  void MoveFrom(Hashmap<Key, Val, Hash, Eq>& other) {
    table_ = other.table_;
    table_size_ = other.table_size_;
    size_ = other.size_;
    load_factor_ = other.load_factor_;
    hash_fn_ = std::move(other.hash_fn_);
    eq_fn_ = std::move(other.eq_fn_);

    other.table_ = nullptr;
    // don't have to reset any of the other fields in other
  }

  /**
   * @return hash code for the given key
   */
  inline int HashCode(const Key& k) const {
    // std::hash returns a size_t, which is an unsigned int. Only keep bottom 31
    // bits to prevent the hash from going negative when we coerce size_t into
    // a signed int.
    return hash_fn_(k) & ((2 << 31) - 1);
  }

  /**
   * @return the index in the underlying table at which the hashcode belongs.
   * Does not account for collisions in which lineary probing is required.
   */
  inline int IndexFor(int hashcode) const {
    return hashcode & (table_size_ - 1);
  }

  /**
   * @return if the two keys are equivalent
   */
  inline bool Equals(const Key& k1, const Key& k2) const {
    return eq_fn_(k1, k2);
  }

  /**
   * @param k key for which to locate the index at which it should be inserted
   * @param hashcode hashcode for the key, or -1, if you want this function to
   * compute it.
   * @return index in the underlying table at which the key is located if the
   * key is in the hashmap. Otherwise, returns the index at which the key should
   * be inserted. Returns -1 if the table is completely full and the element
   * is not there.
   */
  int LocateEntryIdx(const Key& k, int hashcode=-1) const {
    if (hashcode == -1) {
      hashcode = HashCode(k);
    }
    int expected_idx = IndexFor(hashcode); 
    int idx_to_check = expected_idx;

    for (int i = 0; i < table_size_; ++i) {
      const Entry& entry_to_check = table_[idx_to_check];

      // is the entry the one we're looking for?
      bool is_correct_entry = (entry_to_check.IsValid() &&
          Equals(k, entry_to_check.k));

      bool is_unallocated_entry = (!entry_to_check.IsValid());

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

    // table is completely full and the element was not found
    return -1;
  }

  void Resize() {

    // back up old table data
    Entry* old_table = table_;
    int old_table_size = table_size_;

    // create new table
    while (size_ >= table_size_ * load_factor_) {
      table_size_ *= 2;
    }
    table_ = new Entry[table_size_];

    // re-insert all the valid elements in the old table
    for (int i = 0; i < old_table_size; ++i) {
      if (old_table[i].IsValid()) {
        int insert_idx = LocateEntryIdx(old_table[i].k);
        assert(insert_idx != -1);
        table_[insert_idx] = std::move(old_table[i]);
      }
    }

    // free old table memory
    delete[] old_table;
  }

private:

  // underlying table for the hashmap
  Entry* table_ = nullptr;
  
  // size of the underlying table, must be a power of 2.
  int table_size_ = 0;

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

