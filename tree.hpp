#ifndef TREE_H__
#define TREE_H__

#include <list>
#include <iostream>
#include <cmath>
#include <functional> 

using std::list;
using std::cout;
using std::endl;
using std::less;

template <typename Comparable, typename Comparator=less<Comparable> >
class Tree
{
public:
  Tree( );  //construct an empty tree
  Tree( const list<Comparable> & ); //construct complete BST with min height based on list vals
  ~Tree( ); //release all dynamically allocated memory
  
  void construct( const list<Comparable> & ); //release own sources and get values from list

  const Comparable * findMin( ) const; //pointer to element that is minimal wrt Comparator, implemented
  const Comparable * findMax( ) const; //pointer to element that is maximal wrt Comparator, implemented

  const Comparable * getAddress( const Comparable & ) const; //find pointer to element with given values

  bool contains( const Comparable * ) const; //does this element with data exists in tree, implemented
  bool isEmpty( ) const; //is the tree empty, implemented
    
  void insert( const Comparable * ); //insert if dereferenced argument's data is not present, update heights of those affected
  void remove( const Comparable * ); //delete if dereferenced argument's data is present, update heights of those affected

  size_t getSize( ) const; //accessor to size, implemented
  int getHeight( ) const; //accessor  to height, implemented
  bool isBalanced( ) const; //accessor to balanced, implemented

  void print( ) const; //inorder print of dereferenced pointers held in nodes, implemented

  friend class MusicList; //MusicList class can access private parts of Tree

private:
  struct TreeNode  //scope Tree::TreeNode, thus is only public to Tree structure, no non-friend outsiders can see
  {
    const Comparable * element; //adress of the item
    TreeNode * left; //pointer to the root of left subtree
    TreeNode * right; //pointer to the root of right subtree
    TreeNode * parent; //pointer to parent node, pay attention to this!
    size_t height; //height >= 0

    TreeNode( const Comparable * el, TreeNode * l, TreeNode * r, TreeNode * p )
      : element( el ), left( l ), right( r ), parent( p ) {}
  };

  TreeNode* root; //designated root
  Comparator isLessThan;
  list<Comparable> l;
  
  size_t size; //number of nodes
  bool balanced;

  Tree( const Tree & ); //disabled, provide no implementation
  Tree & operator=( const Tree & ); //disabled, provide no implementation

  void makeEmpty(TreeNode*&);
  void inOrder(TreeNode*) const;
  void construct(typename list<Comparable>::const_iterator&, TreeNode*&, size_t, TreeNode*);
  const Comparable* getAddress(const Comparable&, TreeNode* ) const;
  void setHeight(TreeNode*&);
  void insert( const Comparable * , TreeNode*&, TreeNode*); //utility insert
  void remove( const Comparable * , TreeNode*&); //utility remove
  TreeNode * findMin( TreeNode * ) const; //recursively find min in (leftward) subtrees
  TreeNode * findMax( TreeNode * ) const; //recursively find max in (rightward) subtrees
  bool contains( const Comparable *, TreeNode * ) const; //recursively search for an item
  
  bool isLeaf( TreeNode * ) const; //isLeaf
  bool isFull( TreeNode * ) const; //isFull
  void print( TreeNode * ) const; //recursive print

  template <typename T>  //templated max
  T max( T x, T y ) { return x > y ? x : y; }

  template <typename T>  //templated min
  T min( T x, T y ) { return x < y ? x : y; }
  
}; //Do not modify method prototypes and data member names

template <typename Comparable, typename Comparator>
Tree<Comparable,Comparator>::Tree( ) : root(NULL), size(0), balanced(true) {}

template <typename Comparable, typename Comparator>
Tree<Comparable,Comparator>::Tree( const list<Comparable> & data )
{
  root = NULL;
  size = data.size();
  balanced = true;
  typename list<Comparable>::const_iterator itr = data.begin();
  construct(itr, root, size, NULL);
}

template <typename Comparable, typename Comparator>
void
Tree<Comparable,Comparator>::construct( const list<Comparable> & data )
{
  makeEmpty(root);
  size = data.size();
  balanced = true;
  typename list<Comparable>::const_iterator itr = data.begin();
  construct(itr, root, size, NULL);
}

template <typename Comparable, typename Comparator>
Tree<Comparable,Comparator>::~Tree( )
{
  makeEmpty(root);
}

template <typename Comparable, typename Comparator>
void
Tree<Comparable,Comparator>::insert( const Comparable * data )
{
  insert(data, root, NULL);
  if(size > 1  && root->height >= 2*floor(log2(size)))
  {
    l.clear();
    inOrder(root);
    construct(l);
  }
}

template <typename Comparable, typename Comparator>
void
Tree<Comparable,Comparator>::remove( const Comparable * data )
{
  remove(data, root);
  if(size > 1  && root->height >= 2*floor(log2(size)))
  {
    z
    inOrder(root);
    construct(l);
  }
}

template <typename Comparable, typename Comparator>
const Comparable *
Tree<Comparable,Comparator>::findMin( ) const
{
  if( isEmpty( ) )
    return NULL;
  
  return findMin( root )->element; //call in to recursive utility function
}

template <typename Comparable, typename Comparator>
const Comparable *
Tree<Comparable,Comparator>::findMax( ) const
{
  if( isEmpty( ) )
    return NULL;
  
  return findMax( root )->element;
}

template <typename Comparable, typename Comparator>
const Comparable *
Tree<Comparable,Comparator>::getAddress( const Comparable & item ) const
{ 
  return getAddress(item, root);
}

template <typename Comparable, typename Comparator>
bool
Tree<Comparable,Comparator>::contains( const Comparable * item ) const
{  
  return contains( item, root );
}

template <typename Comparable, typename Comparator>
size_t
Tree<Comparable,Comparator>::getSize( ) const
{
  return size;
}

template <typename Comparable, typename Comparator>
int
Tree<Comparable,Comparator>::getHeight( ) const
{
  if( isEmpty( ) )
    return -1;  //convention: empty tree has height -1
  else
    return root->height; //>= 0
}

template <typename Comparable, typename Comparator>
bool
Tree<Comparable,Comparator>::isEmpty( ) const
{
  return ( root == NULL );
}

template <typename Comparable, typename Comparator>
void
Tree<Comparable,Comparator>::print( ) const
{
  print( root );
  cout << endl;  
}

template <typename Comparable, typename Comparator>
bool
Tree<Comparable,Comparator>::isBalanced( ) const
{
  return balanced;
}

//private utility functions

template <typename Comparable, typename Comparator>
void
Tree<Comparable,Comparator>::construct(typename list<Comparable>::const_iterator& itr, TreeNode*& t, size_t n, TreeNode* par)
{
  typename list<Comparable>::const_iterator it = itr;   
  size_t h = floor(log2(n));
  size_t pivotIndex;
  if(h)
  {
    pivotIndex = min(n - pow(2, h-1), pow(2,h) - 1);
  }
  else
  {
    pivotIndex = 0;
  }
  for(size_t i = 0; i < pivotIndex; i++)
  {
    itr++;
  }
  t = new TreeNode(&(*itr), NULL, NULL, par);
  t->height = h;
  if(pivotIndex != 0 )
  {
    construct(it, t->left, pivotIndex, t);
  }
  if(n - pivotIndex - 1 != 0)
  {
    construct(++itr, t->right, n - pivotIndex - 1, t);
  }
}

template <typename Comparable, typename Comparator>
void 
Tree<Comparable,Comparator>::setHeight(TreeNode*& t)
{
  if(t->left && t->right)
  {
    t->height = 1 + max(t->left->height, t->right->height);
  }
  else if(t->left)
  {
    t->height = 1 + t->left->height;
  }
  else if(t->right)
  {
    t->height = 1 + t->right->height;
  }
  else
  {
    t->height = 0;
  }
}

template <typename Comparable, typename Comparator>
void
Tree<Comparable,Comparator>::makeEmpty(TreeNode*& t)
{
    if(t)
    {
      makeEmpty(t->left);
      makeEmpty(t->right);
      delete t;
    }
    t = NULL;
}

template <typename Comparable, typename Comparator>
void
Tree<Comparable,Comparator>::inOrder(TreeNode* t) const
{
  if(t)
  {
    inOrder(t->left);
    l.push_back(*(t->element));
    inOrder(t->right);
  }
}

template <typename Comparable, typename Comparator>
const Comparable *
Tree<Comparable,Comparator>::getAddress(const Comparable & item, TreeNode* t ) const
{ 
  if(!t)
  {
    return NULL;
  }
  else if(isLessThan(item, *(t->element)))
  {
    return getAddress(item, t->left);
  }
  else if(isLessThan(*(t->element), item))
  {
    return getAddress(item, t->right);
  }
  else
  {
    return t->element;
  }
}

template <typename Comparable, typename Comparator>
void 
Tree<Comparable,Comparator>::insert(const Comparable* item , TreeNode*& t, TreeNode* par)
{
  if(!t)
  {
    t = new TreeNode(item, NULL, NULL, par);
    t->height = 0;
    while(par)
    {
      setHeight(par);
      par = par->parent;
    }
    size++;
  }
  else if(isLessThan(*item, *(t->element)))
  {
    insert(item, t->left, t);
  }
  else if(isLessThan(*(t->element), *item))
  {
    insert(item, t->right, t);
  }
  else
  {
    ;
  }
}

template <typename Comparable, typename Comparator>
void 
Tree<Comparable,Comparator>::remove(const Comparable*  item, TreeNode*& t)
{
  if(!t)
  {
    return;
  }
  if(isLessThan(*item, *(t->element)))
  {
    remove(item, t->left);
  }
  else if(isLessThan(*(t->element), *item))
  {
    remove(item, t->right);
  }
  else if(t->left && t->right)
  {
    t->element = findMin(t->right)->element;
    remove(t->element, t->right);
  }
  else
  {
    TreeNode* oldNode = t;
    TreeNode* pr = t->parent;
    if(t->left)
    {
      t = t->left;
      if(t)
      {
        t->parent = pr;
      }
    }
    else
    {
      t = t->right;
      if(t)
      {
        t->parent = pr;
      }
    }
    size--;
    delete oldNode;
    while(pr)
    {
      setHeight(pr);
      pr = pr->parent;
    }
    delete pr;
  }
}

template <typename Comparable, typename Comparator>
typename Tree<Comparable,Comparator>::TreeNode *
Tree<Comparable,Comparator>::findMin( TreeNode * t ) const
{
  if( t == NULL )
    return NULL;

  if( t->left == NULL )
    return t;

  return findMin( t->left );
}

template <typename Comparable, typename Comparator>
typename Tree<Comparable,Comparator>::TreeNode *
Tree<Comparable,Comparator>::findMax( TreeNode * t ) const
{
  if( t != NULL )
    while( t->right != NULL )
      t = t->right;
  return t;
}

template <typename Comparable, typename Comparator>
bool
Tree<Comparable,Comparator>::contains( const Comparable * item, TreeNode * t ) const
{
  if( t == NULL )
    return false;
  else if( isLessThan( *item, *(t->element) ) )
    return contains( item, t->left );
  else if( isLessThan( *(t->element), *item ) )
    return contains( item, t->right );
  else
    return true;
}

template <typename Comparable, typename Comparator>
bool
Tree<Comparable,Comparator>::isLeaf( TreeNode * t ) const
{
  return ( ( t->left == NULL ) && ( t->right == NULL ) );
}

template <typename Comparable, typename Comparator>
bool
Tree<Comparable,Comparator>::isFull( TreeNode * t ) const
{
  return ( ( t->left != NULL ) && ( t->right != NULL ) );
}

template <typename Comparable, typename Comparator>
void
Tree<Comparable,Comparator>::print( TreeNode * t ) const
{
  if( t && t->left != NULL )
  {
    cout << '[';
    print( t->left );
  }
  else if( t && t->left == NULL && t->right != NULL )
  {
    cout << "[";
  }

  if( t != NULL )
  {
    if( isLeaf( t ) )
    {
      cout << '(' << *( t->element ) << ')';
    }
    else if( !isLeaf( t ) )
    {
      cout << *( t->element );
    }
  }

  if( t && t->right != NULL )
  {
    print( t->right );
    cout << ']';
  }
  else if( t && t->left != NULL && t->right == NULL )
  {
    cout << "]";
  }
}

#endif