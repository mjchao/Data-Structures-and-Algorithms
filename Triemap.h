#include <algorithm>
#include <functional>
#include <list>
#include <stack>
#include <type_traits>


//TODO can optimize if we allow the user to specify a function mapping KeyElem_t
//to unique integers and move to an array-based nodes.
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
 * Eq = comparator for determining if two 
 */
template<
  class Key,
  class Val,
  class Eq=std::equal_to<decltype(+*std::declval<const Key&>().begin())>
  >
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

    // after we've traversed to the end node for the key, we can insert or
    // overwrite the value
    if (curr_node->v == nullptr) {
      // if inserting, update size
      ++size_;
    } else {
      // if overwritign, delete the previous value
      delete curr_node->v;
    }
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

  /**
   * Removes the given key from the map.
   *
   * @param k the key to remove.
   * @param return if the key was found and removed.
   */
  bool Remove(const Key& k) {

    // if the node we end up removing is the last child of its parent, we'll
    // want to delete the parent as well. And if the parent is the last child of
    // it's parent, then we'll want to delete the parent as well, and so on...
    // So we keep a traceback of nodes that may be potentially empty. The memory
    // is allocated on the heap as a std::stack so that long keys don't cause
    // stackoverflow.
    using NodeIt = typename std::list<Node>::iterator;
    std::stack<NodeIt> potentially_empty_node_its;

    Node* curr_node = &root_;
    for (KeyIt_t it = k.begin(); it != k.end(); ++it) {
      NodeIt next_child_it = curr_node->FindChildItWithKeyElem(*it);
      if (next_child_it == curr_node->children.end()) {
        return false;
      }
      curr_node = &(*next_child_it);

      // push onto traceback here because we don't want the root to be
      // processed. The root should never be deleted.
      potentially_empty_node_its.push(next_child_it);
    }

    if (curr_node->v == nullptr) {
      return false;
    }

    // remove the entry by clearing the value
    delete curr_node->v;
    curr_node->v = nullptr;
    --size_;

    // check if parent/ancestors should be deleted as well, after removing
    // this value
    if (curr_node->children.empty() && !potentially_empty_node_its.empty()) {
      NodeIt empty_child_it = potentially_empty_node_its.top();
      potentially_empty_node_its.pop();
      while (!potentially_empty_node_its.empty()) {
      
        // delete the empty node from the parent
        NodeIt parent_node = potentially_empty_node_its.top();
        potentially_empty_node_its.pop();
        parent_node->children.erase(empty_child_it);

        // repeat the process if the parent becomes empty
        if (parent_node->children.empty()) {
          empty_child_it = parent_node;
        } else {
          break;
        }
      }
    }
    return true;
  }

  int Size() const {
    return size_;
  }

  void Clear() {
    root_ = Node();
    size_ = 0;
  }

private:

  static bool Equal(const KeyElem_t& e1, const KeyElem_t& e2) {
    static Eq eq_fn_;
    return eq_fn_(e1, e2); 
  }

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

    Node(Node&& other) noexcept {
      MoveFrom(other);
    }

    Node& operator=(const Node& other) {
      FreeMem();
      CopyFrom(other);
      return *this;
    }

    Node& operator=(Node&& other) noexcept {
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
      auto it = FindChildItWithKeyElem(key_elem);
      return (it != children.end()) ? &(*it) : nullptr;
    }
    
    /**
     * @return iterator to the child node that holds the given key element,
     * or end() if no child holds that key element.
     */
    typename std::list<Node>::iterator FindChildItWithKeyElem(
        const KeyElem_t& key_elem) {
      for (auto it = children.begin(); it != children.end(); ++it) {
        if (Equal(it->e, key_elem)) {
          return it;
        }
      }
      return children.end();
    }
  };

  /**
   * Root of the trie, corresponding to a Key with no elements.
   */
  Node root_;

  /**
   * Number of elements in the trie
   */
  int size_ = 0;
 
};

} // namespace dsalgo

