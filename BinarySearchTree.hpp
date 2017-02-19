#ifndef BINARY_SEARCH_TREE_HPP_
# define BINARY_SEARCH_TREE_HPP_

# include <cstddef>
# include <memory>
# include <utility>
# include <functional>
# include <queue>
# include <stack>
# include <deque>
# include <algorithm>
# include <initializer_list>
# include <list>
# include <vector>

# include <iostream>

namespace Tree
{
  template <typename T>
  class BinarySearchTree
  {
  public :
    class Node
    {
    private :
      T _value;
      Node *_left;
      Node *_right;
      
    public :
      Node(const T&, Node * = nullptr, Node * = nullptr);
      Node(T&&, Node * = nullptr, Node * = nullptr);
      Node(const Node&) = default;
      ~Node() = default;
      Node& operator=(const Node&) = default;
      const T& getValue() const;
      T& getValue();
      void setValue(const T&);
      void setValue(T&&);
      const Node *getLeft() const;
      Node *getLeft();
      void setLeft(Node *);
      const Node *getRight() const;
      Node *getRight();
      void setRight(Node *);
    };

  public :
    enum class TraversalType : unsigned short
    {
      DFS_PREFIX, DFS_SUFFIX, DFS_INFIX, BFS
	};
    
  public :
    using FunctorNode = std::function<void (const Node *)>;
  
  private :
    Node *_root;
    std::size_t _size;
    
  public :
    BinarySearchTree();
    BinarySearchTree(const BinarySearchTree&);
    BinarySearchTree(const std::initializer_list<T>&);
    ~BinarySearchTree();
    BinarySearchTree& operator=(const BinarySearchTree&);
    bool operator==(const BinarySearchTree&) const;
    std::size_t size() const;
    std::size_t height() const;
    std::size_t width() const;
    void insert(const T&);
    void insert(T&&);
    Node *find(const T&) const;
    bool erase(const T&);
    void clear();
    void apply(const FunctorNode&,
	       TraversalType = TraversalType::DFS_PREFIX) const;
    
    template <template <class, class = std::allocator<T>> class U>
      U<T> toSortedSeqContainer() const;
    
  private :
    void copyTree(const Node *);
    void copyTree(Node *, const Node *);
    bool compareTree(const Node *, const Node *) const;
    std::size_t height(const Node *) const;
    void findPredecessor(Node *, Node*&, Node*&);
    void replaceByPredecessor(Node *);
    bool erase(Node *, Node *, const T&);
    bool eraseNode(Node *, Node *);
    bool eraseLeaf(Node *, Node *);    
    void eraseAll(Node *);
    void applyDFSPrefix(const FunctorNode&, const Node *) const;
    void applyDFSSuffix(const FunctorNode&, const Node *) const;
    void applyDFSInfix(const FunctorNode&, const Node *) const;
    void applyBFS(const FunctorNode&) const;

    template <template <class, class = std::allocator<T>> class U>
      void toSortedSeqContainer(U<T>&, const Node *) const;
    
    void initSortedSeqContainer(std::vector<T>&) const;
    void initSortedSeqContainer(std::list<T>&) const;
  };

  template <typename T>
  BinarySearchTree<T>::Node::Node(const T& value,
				  Node *left,
				  Node *right) :
    _value(value),
    _left(left),
    _right(right)
  { }

  template <typename T>
  BinarySearchTree<T>::Node::Node(T&& value,
				  Node *left,
				  Node *right) :
    _value(std::move(value)),
    _left(left),
    _right(right)
  { }
  
  template <typename T>
  const T& BinarySearchTree<T>::Node::getValue() const { return _value; }

  template <typename T>
  T& BinarySearchTree<T>::Node::getValue() { return _value; }

  template <typename T>
  void
  BinarySearchTree<T>::Node::setValue(const T& value) { this->_value = value; }

  template <typename T>
  void  BinarySearchTree<T>::Node::setValue(T&& value)
  {
    this->_value = std::move(value);
  }
  
  template <typename T>
  const typename BinarySearchTree<T>::Node *
  BinarySearchTree<T>::Node::getLeft() const { return _left; }

  template <typename T>
  typename BinarySearchTree<T>::Node *
  BinarySearchTree<T>::Node::getLeft() { return _left; }

  template <typename T>
  void BinarySearchTree<T>::Node::setLeft(Node *left) { _left = left; }

  template <typename T>
  const typename BinarySearchTree<T>::Node *
  BinarySearchTree<T>::Node::getRight() const { return _right; }

  template <typename T>
  typename BinarySearchTree<T>::Node *
  BinarySearchTree<T>::Node::getRight() { return _right; }

  template <typename T>
  void BinarySearchTree<T>::Node::setRight(Node *right) { _right = right; }

  
  template <typename T>
  BinarySearchTree<T>::BinarySearchTree() :
    _root(nullptr),
    _size(0)
  { }

  template <typename T>
  BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T>& other)
  {
    copyTree(other._root);
    _size = other._size;
  }

  template <typename T>
  BinarySearchTree<T>::BinarySearchTree(const std::initializer_list<T>& list) :
    BinarySearchTree()
  {
    typename std::initializer_list<T>::iterator begin = list.begin();
    typename std::initializer_list<T>::iterator end = list.end();

    while (begin != end)
      {
	insert(*begin);
	++begin;
      }
  }
  
  template <typename T>
  BinarySearchTree<T>::~BinarySearchTree() { eraseAll(_root); }

  template <typename T>
  BinarySearchTree<T>&
  BinarySearchTree<T>::operator=(const BinarySearchTree<T>& other)
  {
    clear();
    copyTree(other._root);
    _size = other._size;
    return *this;
  }

  template <typename T>
  bool BinarySearchTree<T>::operator==(const BinarySearchTree<T>& other) const
  {
    return compareTree(_root, other._root);
  }
  
  template <typename T>
  std::size_t BinarySearchTree<T>::size() const { return _size; }

  template <typename T>
  std::size_t BinarySearchTree<T>::height() const
  {
    return height(_root);
  }

  template <typename T>
  std::size_t BinarySearchTree<T>::width() const
  {
    std::size_t width = 0;

    if (_root)
      {
	std::queue<Node *> nodes;
		
	nodes.push(_root);
	while (!nodes.empty())
	  {
	    std::size_t nodesSize = nodes.size();
	    std::queue<Node *> widthNodes = std::move(nodes);
	    
	    if (width < nodesSize)
	      width = nodesSize;
	    while (!widthNodes.empty())
	      {
		Node *node = widthNodes.front();
	        
		widthNodes.pop();
		if (node->getLeft())
		  nodes.push(node->getLeft());
		if (node->getRight())
		  nodes.push(node->getRight());
	      }
	  }
      }
    return width;
  }
  
  template <typename T>
  void BinarySearchTree<T>::insert(const T& value)
  {
    if (_root)
      {
	Node *current = _root;

	while (current)
	  {
	    Node *parent = current;

	    if (current->getValue() == value)
	      return;
	    else if (current->getValue() > value)
	      {
		current = current->getLeft();
		if (!current)
		  parent->setLeft(new Node(value));
	      }
	    else
	      {
		current = current->getRight();
		if (!current)
		  parent->setRight(new Node(value));
	      }
	  }
      }
    else
      _root = new Node(value);
    ++_size;
  }

  template <typename T>
  void BinarySearchTree<T>::insert(T&& value)
  {
    if (_root)
      {
	Node *current = _root;
    
	while (current)
	  {
	    Node *parent = current;

	    if (current->getValue() == value)
	      return;
	    else if (current->getValue() > value)
	      {
		current = current->getLeft();
		if (!current)
		  parent->setLeft(new Node(std::forward<T>(value)));
	      }
	    else
	      {
		current = current->getRight();
		if (!current)
		  parent->setRight(new Node(std::forward<T>(value)));
	      }
	  }
      }
    else
      _root = new Node(std::forward<T>(value));
    ++_size;
  }
  
  template <typename T>
  bool BinarySearchTree<T>::erase(const T& value)
  {
    if (erase(_root, nullptr, value))
      {
	--_size;
	if (_size == 0)
	  _root = nullptr;
	return true;
      }
    else
      return false;
  }
  
  template <typename T>
  typename BinarySearchTree<T>::Node *
  BinarySearchTree<T>::find(const T& value) const
  {
    Node *current = _root;

    while (current)
      {
	if (current->getValue() == value)
	  return current;
	current = (current->getValue() > value) ?
	  current->getLeft() : current->getRight();
      }
    return nullptr;
  }
  
  template <typename T>
  void BinarySearchTree<T>::apply(const FunctorNode& functor,
				  TraversalType flag) const
  {
    if (_root)
      {
	if (flag == TraversalType::DFS_PREFIX)
	  applyDFSPrefix(functor, _root);
	else if (flag == TraversalType::DFS_SUFFIX)
	  applyDFSSuffix(functor, _root);
	else if (flag == TraversalType::DFS_INFIX)
	  applyDFSInfix(functor, _root);
	else
	  applyBFS(functor);
      }
  }

  template <typename T>
  template <template <class, class = std::allocator<T>> class U>
    U<T> BinarySearchTree<T>::toSortedSeqContainer() const
  {
    U<T> sortedSeqContainer;

    initSortedSeqContainer(sortedSeqContainer);
    toSortedSeqContainer(sortedSeqContainer, _root);
    return sortedSeqContainer;
  }

  template <typename T>
  void BinarySearchTree<T>::clear()
  {
    eraseAll(_root);
    _root = nullptr;
    _size = 0;
  }
  
  template <typename T>
  void BinarySearchTree<T>::copyTree(const Node *src)
  {
    if (src)
      {
	_root = new Node(src->getValue());
	copyTree(_root, src);
      }
  }
  
  template <typename T>
  void BinarySearchTree<T>::copyTree(Node *dest, const Node *src)
  {
    if (src->getLeft())
      {
	dest->setLeft(new Node(src->getLeft()->getValue()));
	copyTree(dest->getLeft(), src->getLeft());
      }
    if (src->getRight())
      {
	dest->setRight(new Node(src->getRight()->getValue()));
	copyTree(dest->getRight(), src->getRight());
      }
  }

  template <typename T>
  bool BinarySearchTree<T>::compareTree(const Node *lhs, const Node *rhs) const
  {
    if (lhs && rhs && lhs->getValue() == rhs->getValue())
      return !(compareTree(lhs->getLeft(), rhs->getLeft())
	       ^ compareTree(lhs->getRight(), rhs->getRight()));
    else if (!lhs && !rhs)
      return true;
    else
      return false;
  }
  
  template <typename T>
  std::size_t BinarySearchTree<T>::height(const Node *node) const
  {
    return node ?
      1 + std::max(height(node->getLeft()), height(node->getRight())) : 0;
  }

  template <typename T>
  void BinarySearchTree<T>::findPredecessor(Node *current,
					    Node*& predecessor,
					    Node*& parent)
  {
    parent = current;
    current = current->getLeft();    
    while (current && current->getRight())
      {
	parent = current;
	current = (current->getRight()) ?
	  current->getRight() : current->getLeft();
      }
    predecessor = current;
  }

  template <typename T>
  void BinarySearchTree<T>::replaceByPredecessor(Node *current)
  {
    Node *parent = nullptr;
    Node *predecessor = nullptr;

    findPredecessor(current, predecessor, parent);
    std::swap(current->getValue(), predecessor->getValue());
    eraseLeaf(predecessor, parent);
  }

  template <typename T>
  bool BinarySearchTree<T>::erase(Node *current,
				  Node *parent,
				  const T& value)
  {
    if (!current)
      return false;
    else if (current
	&& current->getValue() == value
	&& (current->getLeft() || current->getRight()))
      return eraseNode(current, parent);
    else if (current
	     && current->getValue() == value
	     && !current->getLeft()
	     && !current->getRight())
      return eraseLeaf(current, parent);
    else
      return erase(current->getLeft(), current, value)
	|| erase(current->getRight(), current, value);
  }

  template <typename T>
  bool BinarySearchTree<T>::eraseNode(Node *current, Node *parent)
  {
    if (current->getLeft() && current->getRight())
      replaceByPredecessor(current);
    else
      {
	if (!parent && current->getLeft())
	  _root = current->getLeft();
	else if (!parent && current->getRight())
	  _root = current->getRight();
	else if (parent && parent->getLeft() == current)
	  parent->getLeft()->setLeft(current->getLeft());
	else if (parent && parent->getRight() == current)
	  parent->getRight()->setRight(current->getRight());
	delete current;
      }
    return true;
  }
  
  template <typename T>
  bool BinarySearchTree<T>::eraseLeaf(Node *current, Node *parent)
  {
    if (parent && parent->getLeft() == current)
      parent->getLeft()->setLeft(nullptr);
    else if (parent && parent->getRight() == current)
      parent->getRight()->setRight(nullptr);
    delete current;
    return true;
  }
  
  template <typename T>
  void BinarySearchTree<T>::eraseAll(Node *node)
  {
    if (node)
      {
	eraseAll(node->getLeft());
	eraseAll(node->getRight());
	delete node;
      }
  }
  
  template <typename T>
  void BinarySearchTree<T>::applyDFSPrefix(const FunctorNode& functor,
					   const Node *node) const
  {
    if (node)
      {
	functor(node);
	applyDFSPrefix(functor, node->getLeft());
	applyDFSPrefix(functor, node->getRight());
      }
  }
  
  template <typename T>
  void BinarySearchTree<T>::applyDFSSuffix(const FunctorNode& functor,
					   const Node *node) const
  {
    if (node)
      {
	applyDFSSuffix(functor, node->getLeft());
	functor(node);
	applyDFSSuffix(functor, node->getRight());
      }
  }

  template <typename T>
  void BinarySearchTree<T>::applyDFSInfix(const FunctorNode& functor,
					  const Node *node) const
  {
    if (node)
      {
	applyDFSInfix(functor, node->getLeft());
	applyDFSInfix(functor, node->getRight());
	functor(node);
      }
  }

  template <typename T>
  void BinarySearchTree<T>::applyBFS(const FunctorNode& functor) const
  {
    std::queue<Node *> nodes;

    nodes.push(_root);
    while (!nodes.empty())
      {
	Node *node = nodes.front();
        
	functor(node);
	nodes.pop();
	if (node->getLeft())
	  nodes.push(node->getLeft());
	if (node->getRight())
	  nodes.push(node->getRight());
      }
  }
  
  template <typename T>
  template <template <class, class = std::allocator<T>> class U>
    void BinarySearchTree<T>::toSortedSeqContainer(U<T>& sortedSeqContainer,
						   const Node *node) const
  {
    if (node)
      {
	toSortedSeqContainer(sortedSeqContainer, node->getLeft());
	sortedSeqContainer.push_back(node->getValue());
	toSortedSeqContainer(sortedSeqContainer, node->getRight());
      }
  }

  template <typename T>
  inline void
  BinarySearchTree<T>::initSortedSeqContainer(std::vector<T>& vector) const
  {
    vector.resize(_size);
  }

  template <typename T>
  inline void
  BinarySearchTree<T>::initSortedSeqContainer(std::list<T>&) const { }
}

#endif /* !BINARY_SEARCH_TREE_HPP_ */
