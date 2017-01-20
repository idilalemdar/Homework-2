#ifndef MUSICLIST__
#define MUSICLIST__

#include "tree.hpp"
#include "song.hpp"
#include "name.hpp"
#include "year.hpp"

class MusicList
{
public:
  MusicList( ); //empty constructor
  MusicList( const list<Song> & ); //sort list whenever necessary and construct tree
  ~MusicList( ); //destroy list
  void insert( const string &, const string &,  //insert a new song with these parameters
               int, size_t =0 );                //push_back into list, insert into indices
  void remove( const string &, const string &,  //remove those items
               int ); 
  int getNumberOfSongs( ) const;  //how many songs are there in the indices?
  void printAllNameSorted( ) const; //print all sorted via name-based index
  void printAllYearSorted( ) const; //print all sorted via year-based index
  void printNameRange( const string &, const string & ) const; //print all songs sorted with names in range e.g. "AA" to "GK"
  void printYearRange( int, int ) const; //print all songs sorted with years in range e.g. 2005 to 2011
  void printNameTree( ) const; //print the underlying tree via its own print method, implemented, do not change
  void printYearTree( ) const; //the same.
private:
  list<Song> playlist; //actual song list
  Tree<Song,NameComparator> nameBasedIndex; //use this tree index for name queries
  Tree<Song,YearComparator> yearBasedIndex; //use this tree index for year queries
 
  NameComparator nameLessThan; //comparator function object for name attribute
  YearComparator yearLessThan; //comparator function object for year attribute

  void printAllNameSorted(Tree<Song, NameComparator>::TreeNode* ) const;
  void printAllYearSorted(Tree<Song, YearComparator>::TreeNode* ) const;
  void performActualDeletion( ); //delete nodes from the linked list if their count exceeds a certain threshold
  void printNameRange( const string &, const string &, Tree<Song, NameComparator>::TreeNode*) const;
  void printYearRange( int , int, Tree<Song, YearComparator>::TreeNode*) const;
};

#endif
