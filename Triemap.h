#include <algorithm>
#include <list>
#include <type_traits>


namespace dsalgo {

/**
 * Trie-based map.
 *
 * (Key, Value) pairs are inserted into the Trie. Keys should be iterable.
 * The key only needs to provide begin() and end() functions that return
 * forward iterators. Dereferencing an iterator should return the value_type of
 * the elements within the key.
 * 
 * Examples of valid Keys: strings, vectors, linked lists. Tries are especially
 * useful for mapping strings to values.
 *
 * Key = key used to look up values in the triemap
 * Val = type that gets mapped to in the triemap
 */
template<class Key, class Val>
class Triemap {

public:

  using KeyIt_t = decltype(std::declval<const Key&>().begin());
  using KeyElem_t = decltype(+*std::declval<KeyIt_t>());

  Triemap() {}

  /**
   * Maps the given key to the given value in the trie.
   *
   * @param k the key to map
   * @param v the value to map the key to
   */
  void Put(const Key& k, const Val& v) {

    // keep track of the current node we're at in the traversal.
    // traversal starts at the root
    Node* curr_node = &root_;

    // for each element in the the key we're inserting 
    for (KeyIt_t it = k.begin(); it != k.end(); ++it) {
      const KeyElem_t& curr_key_elem = *it;

      // check if the current node has that element as a child already
      Node* next_child = curr_node->FindChildWithKeyElem(curr_key_elem);
      
      // if the child doesn't exist
      if (next_child == nullptr) {

        // create the child
        Node new_node;
        new_node.e = curr_key_elem;

        // and add it to the end of the child list
        curr_node->children.push_back(new_node);

        // and the next child to traverse to will be this newly created child
        next_child = &curr_node->children.back();
      }

      curr_node = next_child;
    }

    // after we've traversed to the end node for the key, we can insert the
    // value
    curr_node->v = new Val(v);
  }

  /**
   * Gets the value to which the given key is mapped to. Or returns nullptr if
   * the given key was not inserted into the triemap.
   *
   * @param k the key to search for
   * @return the value to which the given key is mapped, or nullptr if the key
   * was never mapped to anything
   */
  Val* Get(const Key& k) {
    Node* curr_node = &root_;
    for (KeyIt_t it = k.begin(); it != k.end(); ++it) {
      Node* next_child = curr_node->FindChildWithKeyElem(*it);
      if (next_child == nullptr) {
        return nullptr;
      }
      curr_node = next_child;
    }
    return curr_node->v;
  }

private:

  /**
   * Represents a node in the trie
   */
  struct Node {
    std::list<Node> children;
    KeyElem_t e;

    // if this is nullptr, it means no key ends at this node
    Val* v = nullptr;

    Node() {}

    ~Node() {
      FreeMem();
    }

    Node(const Node& other) {
      CopyFrom(other);
    }

    Node(const Node&& other) noexcept {
      MoveFrom(other);
    }

    Node& operator=(const Node& other) {
      FreeMem();
      CopyFrom(other);
      return *this;
    }

    Node& operator=(const Node&& other) noexcept {
      FreeMem();
      MoveFrom(other);
      return *this;
    }

    void FreeMem() {
      if (v != nullptr) {
        delete v;
      }
    }

    void CopyFrom(const Node& other) {
      children = other.children;
      e = other.e;
      if (other.v != nullptr) {
        v = new Val(*other.v);
      }
    }

    void MoveFrom(Node& other) {
      children = std::move(other.children);
      v = other.v;
      other.v = nullptr;
    }

    /**
     * @return the child node that holds the given key element, or nullptr
     * if no child holds that key element
     */
    Node* FindChildWithKeyElem(const KeyElem_t& key_elem) {
      for (Node& n : children) {
        if (n.e == key_elem) {
          return &n;
        }
      }
      return nullptr;
    }
  };

  /**
   * Root of the trie, corresponding to a Key with no elements.
   */
  Node root_;
  
};

} // namespace dsalgo

