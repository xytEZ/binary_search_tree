#include <iostream>
#include <list>

#include "BinarySearchTree.hpp"

template <typename T>
struct NodeKeyDisplayer
{
  ~NodeKeyDisplayer() { std::cout << std::endl; }

  void operator()(const typename Tree::BinarySearchTree<T>::Node *node)
  {
    std::cout << node->getValue() << ' ';
  }
};

int main()
{
  Tree::BinarySearchTree<int> tree;
  int value1 = 16;
  int value2 = 93;

  std::cout << "( ( ( ( Insert 16, 93, 93, -35, 62 keys ) ) ) )" << std::endl;
  tree.insert(value1);
  tree.insert(value2);
  tree.insert(93);
  tree.insert(-35);
  tree.insert(62);
  std::cout << std::endl << std::endl;


  
  // Tree state n°1

  std::cout << "[Tree state n°1]" << std::endl << std::endl;
    
  std::cout << "( ( ( ( Depth-first search PREFIX ) ) ) )" << std::endl;
  
  tree.apply(NodeKeyDisplayer<int>());

  std::cout << "( ( ( ( tree.operator==(tree) ) ) ) )" << std::endl;
  if (tree == tree)
    std::cout << "tree is equal to tree" << std::endl;
  else
    std::cout << "tree is not equal to tree" << std::endl;
  std::cout << std::endl;
  
  Tree::BinarySearchTree<int> tree2 = tree;
  
  tree2.insert(-66);
  tree2.insert(-40);
  
  std::cout << "( ( ( ( tree.operator==(tree2) ) ) ) )" << std::endl;
  if (tree == tree2)
    std::cout << "tree is equal to tree2" << std::endl;
  else
    std::cout << "tree is not equal to tree2" << std::endl;
  std::cout << std::endl << std::endl;
  
  tree = tree2;

  

  // Tree state n°2

  std::cout << "[Tree state n°2]" << std::endl << std::endl;

  std::cout << "( ( ( ( Depth-first search PREFIX ) ) ) )" << std::endl;
  
  tree.apply(NodeKeyDisplayer<int>());
  
  Tree::BinarySearchTree<int> tree3 = { 42, 17, 69, 95, 62, 10, 99 };
  
  tree = tree3;

  std::cout << std::endl;

  
  
  // Tree state n°3
  
  std::cout << "[Tree state n°3]" << std::endl << std::endl;

  std::cout << "Size : " << tree.size() << std::endl;
  std::cout << "Height : " << tree.height() << std::endl;
  std::cout << "Width : " << tree.width() << std::endl;
  
  std::cout << std::endl;
  
  std::cout << "( ( ( ( Depth-first search PREFIX ) ) ) )" << std::endl;
  
  tree.apply(NodeKeyDisplayer<int>());
  
  std::cout << "( ( ( ( Depth-first search SUFFIX ) ) ) )" << std::endl;
  
  tree.apply(NodeKeyDisplayer<int>(),
	     Tree::BinarySearchTree<int>::TraversalType::DFS_SUFFIX);
  
  std::cout << "( ( ( ( Depth-first search INFIX ) ) ) )" << std::endl;
  
  tree.apply(NodeKeyDisplayer<int>(),
	     Tree::BinarySearchTree<int>::TraversalType::DFS_INFIX);

  std::cout << "( ( ( ( Breadth-first search ) ) ) )" << std::endl;
  
  tree.apply(NodeKeyDisplayer<int>(),
	     Tree::BinarySearchTree<int>::TraversalType::BFS);

  std::cout << "( ( ( ( Find key = 62 ) ) ) )" << std::endl;
  if (tree.find(62))
    std::cout << "key = 62 is found" << std::endl;
  else
    std::cout << "key = 62 is not found" << std::endl;
  std::cout << std::endl;

  std::cout << "( ( ( ( Find key = -42 ) ) ) )" << std::endl;
  if (tree.find(-42))
    std::cout << "key = -42 is found" << std::endl;
  else
    std::cout << "key = -42 is not found" << std::endl;
  std::cout << std::endl;

  std::cout << "( ( ( ( Sorted sequence container ) ) ) )" << std::endl;

  std::list<int> sortedList = tree.toSortedSeqContainer<std::list>();

  std::for_each(sortedList.cbegin(),
		sortedList.cend(),
		[](int value) -> void
		{
		  std::cout << value << ' ';
		});
  std::cout << std::endl << std::endl;
		  
  std::cout << "( ( ( ( Erase 42, 95, 95, 69, -6, 62 keys ) ) ) )" 
    << std::endl;
  tree.erase(42);
  tree.erase(95);
  tree.erase(95);
  tree.erase(69);
  tree.erase(-6);
  tree.erase(62);
  std::cout << std::endl << std::endl;
  
  

  // Tree state n°4

  std::cout << "[Tree state n°4]" << std::endl << std::endl;

  std::cout << "Size : " << tree.size() << std::endl;
  std::cout << "Height : " << tree.height() << std::endl;
  std::cout << "Width : " << tree.width() << std::endl;

  std::cout << std::endl;

  std::cout << "( ( ( ( Depth-first search PREFIX ) ) ) )" << std::endl;

  tree.apply(NodeKeyDisplayer<int>());

  std::cout << "( ( ( ( Find key = 42 ) ) ) )" << std::endl;
  if (tree.find(42))
    std::cout << "key = 42 is found" << std::endl;
  else
    std::cout << "key = 42 is not found" << std::endl;
  std::cout << std::endl;

  std::cout << "( ( ( ( Find key = 99 ) ) ) )" << std::endl;
  if (tree.find(99))
    std::cout << "key = 99 is found" << std::endl;
  else
    std::cout << "key = 99 is not found" << std::endl;
    std::cout << std::endl;

  std::cout << "( ( ( ( Erase 99, 17, 10 ) ) ) )" << std::endl;
  tree.erase(99);
  tree.erase(17);
  tree.erase(10);

  std::cout << "Size : " << tree.size() << std::endl;
  std::cout << "Height : " << tree.height() << std::endl;
  std::cout << "Width : " << tree.width() << std::endl;

  std::cout << std::endl;
  
    
  std::cout << "( ( ( ( Clear tree2 ) ) ) )" << std::endl;

  tree2.clear();

  std::cout << "tree2 size, height, width : "
	    << tree2.size()
	    << ", "
	    << tree2.height()
	    << ", "
	    << tree2.width()
	    << std::endl;
  return 1;
}

/*

 Tree state n°1 :

                   16

                 /    \

              -35      93

                      /
                   
                     62



 Tree state n°2 :

                   16

                 /    \

              -35      93

             /        /
                   
          -66       62                   

             \

              -40




 Tree state n°3 : 

                   42
                
                 /    \
                
	       17      69
               
             /       /    \

           10      62      95

                              \

                               99


 
 Tree state n°4 :

                   17
                
                 /    \
                
	       10      99
               

*/
