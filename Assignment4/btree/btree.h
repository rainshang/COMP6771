/**
 * The btree is a linked structure which operates much like
 * a binary search tree, save the fact that multiple client
 * elements are stored in a single node.  Whereas a single element
 * would partition the tree into two ordered subtrees, a node 
 * that stores m client elements partition the tree 
 * into m + 1 sorted subtrees.
 */

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <cstddef>
#include <utility>
#include <map>
#include <queue>

// we better include the iterator
#include "btree_iterator.h"

// we do this to avoid compiler errors about non-template friends
// what do we do, remember? :)

//Add declarations for non-template friends
template <typename T>
class btree;
template <typename T>
std::ostream &operator<<(std::ostream &os, const btree<T> &tree);

template <typename T>
class btree
{
public:
  /** Hmm, need some iterator typedefs here... friends? **/

  /**
   * Constructs an empty btree.  Note that
   * the elements stored in your btree must
   * have a well-defined copy constructor and destructor.
   * The elements must also know how to order themselves
   * relative to each other by implementing operator<
   * and operator==. (These are already implemented on
   * behalf of all built-ins: ints, doubles, strings, etc.)
   * 
   * @param maxNodeElems the maximum number of elements
   *        that can be stored in each B-Tree node
   */
  btree(size_t maxNodeElems = 40) : maxNodeElems(maxNodeElems){};

  /**
   * The copy constructor and  assignment operator.
   * They allow us to pass around B-Trees by value.
   * Although these operations are likely to be expensive
   * they make for an interesting programming exercise.
   * Implement these operations using value semantics and 
   * make sure they do not leak memory.
   */

  /** 
   * Copy constructor
   * Creates a new B-Tree as a copy of original.
   *
   * @param original a const lvalue reference to a B-Tree object
   */
  btree(const btree<T> &original);

  /** 
   * Move constructor
   * Creates a new B-Tree by "stealing" from original.
   *
   * @param original an rvalue reference to a B-Tree object
   */
  btree(btree<T> &&original);

  /** 
   * Copy assignment
   * Replaces the contents of this object with a copy of rhs.
   *
   * @param rhs a const lvalue reference to a B-Tree object
   */
  btree<T> &operator=(const btree<T> &rhs);

  /** 
   * Move assignment
   * Replaces the contents of this object with the "stolen"
   * contents of original.
   *
   * @param rhs a const reference to a B-Tree object
   */
  btree<T> &operator=(btree<T> &&rhs);

  /**
   * Puts a breadth-first traversal of the B-Tree onto the output
   * stream os. Elements must, in turn, support the output operator.
   * Elements are separated by space. Should not output any newlines.
   *
   * @param os a reference to a C++ output stream
   * @param tree a const reference to a B-Tree object
   * @return a reference to os
   */
  friend std::ostream &operator<<<T>(std::ostream &os, const btree<T> &tree);

  /**
   * The following can go here
   * -- begin() 
   * -- end() 
   * -- rbegin() 
   * -- rend() 
   * -- cbegin() 
   * -- cend() 
   * -- crbegin() 
   * -- crend() 
   */

  friend class btree_iterator<T>;
  friend class const_btree_iterator<T>;

  typedef btree_iterator<T> iterator;
  typedef const_btree_iterator<T> const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  //non-const and const iterators for begin() and end()
  iterator begin();
  iterator end();

  const_iterator begin() const;                      //const begin()
  const_iterator end() const;                        //const end()
  const_iterator cbegin() const { return begin(); }; //cbegin()
  const_iterator cend() const { return end(); };     //cend()

  //rever_iterator iterators
  reverse_iterator rbegin() { return reverse_iterator(end()); }                   //rbegin
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); } //const rbegin
  const_reverse_iterator crbegin() const { return rbegin(); }                     //crbegin()

  reverse_iterator rend() { return reverse_iterator(begin()); }                   //rend
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); } //const rend
  const_reverse_iterator crend() const { return rend(); }                         //crend()

  /**
    * Returns an iterator to the matching element, or whatever 
    * the non-const end() returns if the element could 
    * not be found.  
    *
    * @param elem the client element we are trying to match.  The elem,
    *        if an instance of a true class, relies on the operator< and
    *        and operator== methods to compare elem to elements already 
    *        in the btree.  You must ensure that your class implements
    *        these things, else code making use of btree<T>::find will
    *        not compile.
    * @return an iterator to the matching element, or whatever the
    *         non-const end() returns if no such match was ever found.
    */
  iterator find(const T &elem);

  /**
    * Identical in functionality to the non-const version of find, 
    * save the fact that what's pointed to by the returned iterator
    * is deemed as const and immutable.
    *
    * @param elem the client element we are trying to match.
    * @return an iterator to the matching element, or whatever the
    *         const end() returns if no such match was ever found.
    */
  const_iterator find(const T &elem) const;

  /**
    * Operation which inserts the specified element
    * into the btree if a matching element isn't already
    * present.  In the event where the element truly needs
    * to be inserted, the size of the btree is effectively
    * increases by one, and the pair that gets returned contains
    * an iterator to the inserted element and true in its first and
    * second fields.  
    *
    * If a matching element already exists in the btree, nothing
    * is added at all, and the size of the btree stays the same.  The 
    * returned pair still returns an iterator to the matching element, but
    * the second field of the returned pair will store false.  This
    * second value can be checked to after an insertion to decide whether
    * or not the btree got bigger.
    *
    * The insert method makes use of T's copy constructor,
    * and if these things aren't available, 
    * then the call to btree<T>::insert will not compile.  The implementation
    * also makes use of the class's operator== and operator< as well.
    *
    * @param elem the element to be inserted.
    * @return a pair whose first field is an iterator positioned at
    *         the matching element in the btree, and whose second field 
    *         stores true if and only if the element needed to be added 
    *         because no matching element was there prior to the insert call.
    */
  std::pair<iterator, bool> insert(const T &elem);

  /**
    * Disposes of all internal resources, which includes
    * the disposal of any client objects previously
    * inserted using the insert operation. 
    * Check that your implementation does not leak memory!
    */
  ~btree();

private:
  // The details of your implementation go here
  struct Element;

  /*
    * A node consists of a mapping of values to Elements.
    * A map is ideal due to its weak ordering on inserted values of type T.
    * Each node is aware of its parent node to simplify later traversal algorithms.
    */
  struct Node
  {
    //Node constructor
    Node(Node *p = nullptr) : parent(p), elements(std::map<T, Element>()) {}

    //Structures
    Node *parent;
    std::map<T, Element> elements;
  };

  /*
    * Each node has up to 'maxNodeElems' elements. Each element has a value T.
    * Each element also has a left and right child node which have value 'nullptr' to indicate no child.
    */
  struct Element
  {
    //Element constructor
    Element(T value = T(), Node *left = nullptr, Node *right = nullptr) : value(value), leftChild(left), rightChild(right) {}

    //Structures
    T value;
    Node *leftChild;
    Node *rightChild;
  };

private:
  size_t maxNodeElems; //stores the max number of elements each node may contain
  Node root;           //store the root node as all other nodes will be linked to it

private:
  //Recursive node copy function to copy a btree
  void copyBTree(const Node *source, Node *parent, Node *dest);

  //Recursive print node function to print an entire tree in breadth first order
  static void printBTree(std::ostream &os, const Node *node, std::queue<Node *> &childs, const T &lastValue); //declare static so nonmember << operator may use it

  //Recursive insertion function to find and insert an elem (if it is unique)
  std::pair<typename btree<T>::iterator, bool> recursiveInsert(Node *node, const T &elem);

  //Recursive find functions, non-const and const versions provided to cater for non-const and const BTree's
  iterator recursiveFind(Node *node, const T &elem);
  const_iterator recursiveFind(const Node *node, const T &elem) const;

  //Recursively element delete function that deletes an element and all of its linked childs
  void deleteElement(Element &e);
};

//Implement

template <typename T>
btree<T>::btree(const btree<T> &original)
{
  maxNodeElems = original.maxNodeElems;
  root = original.root;

  //Recursively copy binary tree using helper function
  copyBTree(&original.root, nullptr, &root);
}

/*
 * Helper Function : Copy nodes in btree recursively.
*/
template <typename T>
void btree<T>::copyBTree(const Node *source, Node *parent, Node *dest)
{
  std::queue<Node *> sourceChilds;
  std::queue<Node *> destChilds;

  //Copy map from source to dest
  dest->elements = source->elements;

  //Copy parent
  dest->parent = parent;

  //For each node
  auto dit = dest->elements.begin();

  for (auto sit = source->elements.begin(); sit != source->elements.end(); ++sit, ++dit)
  {
    //Check left child for a valid node to copy
    if (sit->second.leftChild != nullptr)
    {
      //Make new node with this node as its parent
      Node *newNode = new Node(dest);

      sourceChilds.push(sit->second.leftChild);
      destChilds.push(newNode);
      dit->second.leftChild = newNode;
    }

    //Check right child for a valid node to copy
    if (sit->second.rightChild != nullptr)
    {
      //Make new node with this node as its parent
      Node *newNode = new Node(dest);

      sourceChilds.push(sit->second.rightChild);
      destChilds.push(newNode);
      dit->second.rightChild = newNode;
    }
  }

  //For each children
  while (!sourceChilds.empty())
  {
    //Get front node
    Node *nextSource = sourceChilds.front();
    Node *nextDest = destChilds.front();

    //Pop it off queue
    sourceChilds.pop();
    destChilds.pop();

    //Recursively copy child nodes with this node as the parent
    copyBTree(nextSource, dest, nextDest);
  }
}

/*
* Move constructor
*
* Move over root node and update all child nodes parents to point to new moved root.
* Leave moved from object in valid state by nullifing all left and right child links.
*/
template <typename T>
btree<T>::btree(btree<T> &&original)
{
  maxNodeElems = original.maxNodeElems;
  root = std::move(original.root);

  //Iterate through root elements
  for (auto it = root.elements.begin(); it != root.elements.end(); ++it)
  {
    //For each valid child, update the parent node to newely moved node
    if (it->second.leftChild != nullptr)
    {
      it->second.leftChild->parent = &root;
    }

    if (it->second.rightChild != nullptr)
    {
      it->second.rightChild->parent = &root;
    }
  }

  //We have to leave original in a valid state
  //We must nullify its child links so that it can destruct correctly
  for (auto it = original.root.elements.begin(); it != original.root.elements.end(); ++it)
  {
    it->second.leftChild = it->second.rightChild = nullptr;
  }
}

/*
 * Operater= Copy Semantics (assignment operator)
*/
template <typename T>
btree<T> &btree<T>::operator=(const btree<T> &rhs)
{
  maxNodeElems = rhs.maxNodeElems;
  root = rhs.root;

  //Recursively copy binary tree using helper function
  copyBTree(&rhs.root, nullptr, &root);

  //Return this pointer
  return *this;
}

/*
* Operater= Move Semantics (assignment operator)
*/
template <typename T>
btree<T> &btree<T>::operator=(btree<T> &&rhs)
{
  maxNodeElems = rhs.maxNodeElems;
  root = std::move(rhs.root);

  //Iterate through root elements
  for (auto it = root.elements.begin(); it != root.elements.end(); ++it)
  {
    //For each valid child, update the parent node to newely moved node
    if (it->second.leftChild != nullptr)
    {
      it->second.leftChild->parent = &root;
    }

    if (it->second.rightChild != nullptr)
    {
      it->second.rightChild->parent = &root;
    }
  }

  //We have to leave original in a valid state
  //We must nullify its child links so that it can destruct correctly
  for (auto it = rhs.root.elements.begin(); it != rhs.root.elements.end(); ++it)
  {
    it->second.leftChild = it->second.rightChild = nullptr;
  }

  return *this;
}

/*
* Print out tree values in breadth-first (level) order.
* Assumes << operator is implemented on type T
*
* Complexity: O(log n) to find last element in BTree and O(n) to print out each value. Remember end() is O(1)
* This complexity is identical to having an O(log n) end() function and O(n) print function.
*/
template <typename T>
std::ostream &operator<<(std::ostream &os, const btree<T> &tree)
{

  //Determine value of last element in this BTree
  typename std::map<T, typename btree<T>::Element>::const_iterator it = tree.root.elements.end();

  //Get last element in root
  --it;

  //Get lowest rightmost value, this will be last element in BTree
  while (it->second.rightChild != nullptr)
  {
    it = it->second.rightChild->elements.end();
    --it;
  }

  //Create childs queue which we will use for the BF traversal
  //This will keep track of which node is next to expand and will be passed by reference
  std::queue<typename btree<T>::Node *> childs;

  //Delegate printing to recursive helper function
  btree<T>::printBTree(os, &tree.root, childs, it->first);

  return os;
}

/*
 * Helper Function: Print out node contents recursively with a single space between each element
 * A final space is not printed for the final value. This required the passing of the value by reference and works as this value will be unique.
 *
 * Complexity: See notes for operator<<
*/
template <typename T>
void btree<T>::printBTree(std::ostream &os, const Node *node, std::queue<Node *> &childs, const T &lastValue)
{

  //Print out elements in this node
  for (auto it = node->elements.begin(); it != node->elements.end(); ++it)
  {

    //Print out value
    os << it->first;

    //Print space for all but last element
    if (it->first != lastValue)
      os << " ";

    if (it->second.leftChild != nullptr)
    {
      childs.push(it->second.leftChild);
    }

    if (it->second.rightChild != nullptr)
    {
      childs.push(it->second.rightChild);
    }
  }

  //For each children
  while (!childs.empty())
  {
    //Get front node
    Node *n = childs.front();

    //Pop it off queue
    childs.pop();

    //Recursively print its contents
    printBTree(os, n, childs, lastValue);
  }
}

/*
 * Returns: an iterator positioned at the element found in the B-Tree. If the element being searched is not 
 * found in the B-Tree, an iterator that is equal to the return value of end() is returned.
*/
template <typename T>
typename btree<T>::iterator btree<T>::find(const T &elem)
{
  //Delegate work to recursive helper function
  return recursiveFind(&root, elem);
}

template <typename T>
typename btree<T>::const_iterator btree<T>::find(const T &elem) const
{
  //Delegate work to recursive helper function
  return recursiveFind(&root, elem);
}

/*
 * Helper function: Recursively search for an element in a node.
 *
 * Complexity: O(log n) to find location of element using left and right child links
*/
template <typename T>
typename btree<T>::iterator btree<T>::recursiveFind(Node *node, const T &elem)
{
  //Make iterator and set it to lower bound for this node
  typename std::map<T, typename btree<T>::Element>::iterator it = node->elements.lower_bound(elem);

  //Check if elem is bigger than all values in this node
  if (it == node->elements.end())
  {

    //Get last element
    --it;

    //If last element has a right child, search for elem there
    if (it->second.rightChild != nullptr)
      return recursiveFind(it->second.rightChild, elem);
    //Otherwise, element is not in Btree
    else
      return end();
  }
  //Otherwise see if this is value we are searching for
  else if (it->first == elem)
  {
    //If so return iterator to this element
    return btree_iterator<T>(node, it);
  }
  //Otherwise value must be located in left child
  else
  {
    //If last element has a left child, search for elem there
    if (it->second.leftChild != nullptr)
      return recursiveFind(it->second.leftChild, elem);
    //Otherwise, element is not in Btree
    else
      return end();
  }
}

//Const equivalent to above recursiveFind function. Only difference is node is taken with const qualifier.
template <typename T>
typename btree<T>::const_iterator btree<T>::recursiveFind(const Node *node, const T &elem) const
{
  //Make iterator and set it to lower bound for this node
  typename std::map<T, typename btree<T>::Element>::const_iterator it = node->elements.lower_bound(elem);

  //Check if elem is bigger than all values in this node
  if (it == node->elements.end())
  {

    //Get last element
    --it;

    //If last element has a right child, search for elem there
    if (it->second.rightChild != nullptr)
      return recursiveFind(it->second.rightChild, elem);
    //Otherwise, element is not in Btree
    else
      return end();
  }
  //Otherwise see if this is value we are searching for
  else if (it->first == elem)
  {
    //If so return iterator to this element
    return const_btree_iterator<T>(node, it);
  }
  //Otherwise value must be located in left child
  else
  {
    //If last element has a left child, search for elem there
    if (it->second.leftChild != nullptr)
      return recursiveFind(it->second.leftChild, elem);
    //Otherwise, element is not in Btree
    else
      return end();
  }
}

/*
* Insert elements into the BTree
*
* Returns: A pair consisting of an iterator positioned at the element inserted and a boolean indicating the 
* success of insertion.
*
*/
template <typename T>
std::pair<typename btree<T>::iterator, bool> btree<T>::insert(const T &elem)
{
  //Delegate work to recursive helper function
  return recursiveInsert(&root, elem);
}

/*
* Helper function: Recursive insertion function to find and insert an elem (if it is unique)
*
* As a map is used for the underlying containter, we can simply use the return pair from map::insert which contains an iterator to the inserted element.
* Our btree_iterator can then construct a btree_iterator from this iterator.
*
* Complexity: O(log n) to find location of element using left and right child links and insert at that location or detect duplicate
*/

template <typename T>
std::pair<typename btree<T>::iterator, bool> btree<T>::recursiveInsert(Node *node, const T &elem)
{

  //Iterate over node elements
  for (auto it = node->elements.begin(); it != node->elements.end(); ++it)
  {
    //Create boolean for final iteration special case
    bool finalIteration = false;

    if (it == --node->elements.end())
      finalIteration = true;

    //Keep traversing while elem < it's element and not equal to it
    if (elem == it->first)
    {
      //Exact match found, return pair
      typename std::map<T, typename btree<T>::Element>::iterator itt(it); //create typenamed iterator from it
      return std::pair<typename btree<T>::iterator, bool>(btree_iterator<T>(node, itt), false);
    }
    //If elem is less than element or this is the last element, we must insert or expand here
    else if (elem < it->first || finalIteration)
    {

      //We can insert the element at this location in this node if there is space
      if (node->elements.size() < maxNodeElems)
      {
        //Create new element
        Element e(elem);
        auto itt = node->elements.insert(std::pair<T, Element>(elem, e));                              //insert and get insert pair from map::insert
        return std::pair<typename btree<T>::iterator, bool>(btree_iterator<T>(node, itt.first), true); //itt.first will be a iterator to the map element
      }
      //Otherwise, recursively analyse the left or right child
      else
      {
        Node *child = it->second.leftChild;

        //Select right child for the last element if elem is bigger
        if (finalIteration && elem > it->first)
          child = it->second.rightChild;

        //If child is empty node
        if (child == nullptr)
        {
          //Create new node
          child = new Node(node);

          //Set pointers to child node
          if (finalIteration && elem > it->first)
            it->second.rightChild = child;
          else
            it->second.leftChild = child;
        }

        //We recursively insert searching the child and assigning the parent child to any newely created nodes
        return recursiveInsert(child, elem);
      }
    }
  }

  //No elements in the node (for loop did not iterate at all), we must add the new element
  //Create new element
  Element e(elem);
  auto itt = node->elements.insert(std::pair<T, Element>(elem, e)); //insert and get insert pair from map::insert

  //Return this new pair
  return std::pair<typename btree<T>::iterator, bool>(btree<T>::iterator(node, itt.first), true);
}

/*
 * begin() 
 *
 * Complexity: O(log n) on average. O(n) for degenerate tree (worst case as tree isn't auto balanced as per spec)
*/
template <typename T>
typename btree<T>::iterator btree<T>::begin()
{

  //Set iterator to roots first element
  typename std::map<T, typename btree<T>::Element>::iterator it = root.elements.begin();

  //If empty root node or first element has no left child
  if (it == root.elements.end() || it->second.leftChild == nullptr)
  {

    //Return iterator pointing to root node
    return btree_iterator<T>(&root, it);
  }
  //Otherwise, a left child exists
  else
  {
    Node *p = it->second.leftChild;

    //Find the left most whild
    while (it->second.leftChild != nullptr)
    {
      p = it->second.leftChild;
      it = p->elements.begin();
    }

    //Return iterator to lowest value element
    return btree_iterator<T>(p, it);
  }
}

/*
* cbegin()
*/
template <typename T>
typename btree<T>::const_iterator btree<T>::begin() const
{

  //Set iterator to roots first element
  typename std::map<T, typename btree<T>::Element>::const_iterator it = root.elements.begin();

  //If empty root node or first element has no left child
  if (it == root.elements.end() || it->second.leftChild == nullptr)
  {

    //Return iterator pointing to root node
    return const_btree_iterator<T>(&root, it);
  }
  //Otherwise, a left child exists
  else
  {
    Node *p = it->second.leftChild;

    //Find the left most whild
    while (it->second.leftChild != nullptr)
    {
      p = it->second.leftChild;
      it = p->elements.begin();
    }

    //Return iterator to lowest value element
    return const_btree_iterator<T>(p, it);
    ;
  }
}

/*
* end()
*
* Complexity: O(1), returns first element in root node. Iterators utilise this for performance gains.
*/

template <typename T>
typename btree<T>::iterator btree<T>::end()
{
  typename std::map<T, typename btree<T>::Element>::iterator it = root.elements.end();
  return btree_iterator<T>(&root, it);
}

/*
* cend()
*/
template <typename T>
typename btree<T>::const_iterator btree<T>::end() const
{
  typename std::map<T, typename btree<T>::Element>::const_iterator it = root.elements.end();
  return const_btree_iterator<T>(&root, it);
}

/*
 * Destructor 
 *
 * Delegates work to helper function deleteElements to delete all elements that are linked to root node.
*/
template <typename T>
btree<T>::~btree()
{
  //For each element in the root node
  for (auto it = root.elements.begin(); it != root.elements.end(); ++it)
  {
    //Delete its elements
    deleteElement(it->second);
  }
}

/*
 * Helper function: Expands elements left and right childs recursively, calling deleteElement on them.
 * Deletes all elements from the bottom of tree to the top (that is final elements to be deleted will belong to the root node)
*/
template <typename T>
void btree<T>::deleteElement(Element &e)
{
  //If left child exists
  if (e.leftChild != nullptr)
  {
    //Expand its elements
    for (auto it = e.leftChild->elements.begin(); it != e.leftChild->elements.end(); ++it)
    {
      deleteElement(it->second);
    }

    //Delete the left child
    delete e.leftChild;
    e.leftChild = nullptr;
  }

  //If right child exists
  if (e.rightChild != nullptr)
  {
    //Expand its elements
    for (auto it = e.rightChild->elements.begin(); it != e.rightChild->elements.end(); ++it)
    {
      deleteElement(it->second);
    }

    //Delete the right child
    delete e.rightChild;
    e.rightChild = nullptr;
  }
}

#endif
