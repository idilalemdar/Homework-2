#include "musiclist.hpp"

size_t inactiveCount = 0;

void //final, do not change
MusicList::printNameTree( ) const
{
  nameBasedIndex.print( );
}

void //final, do not change
MusicList::printYearTree( ) const
{
  yearBasedIndex.print( );
}

MusicList::MusicList( ) {}

MusicList::MusicList( const list<Song> & s ) : playlist(s)
{
  NameComparator nc;
  playlist.sort(nc);
  nameBasedIndex.construct(playlist);
  YearComparator yc;
  playlist.sort(yc);
  yearBasedIndex.construct(playlist); 
}

MusicList::~MusicList( )
{
    performActualDeletion();
}

void
MusicList::insert( const string & nm, const string & bd,
                   int year, size_t duration )
{
  Song* s = new Song(nm, bd, year, duration);
  if(!nameBasedIndex.contains(s))
  {
    playlist.push_back(*s);
    nameBasedIndex.insert(s);
    yearBasedIndex.insert(s);
  }
  else
  {
    delete s;
  }
}

void
MusicList::remove( const string & nm, const string & bd,
                   int year )
{
  Song s(nm, bd, year);
  const Song* sp = nameBasedIndex.getAddress(s);
  if(sp)
  {
    (const_cast<Song*>(sp))->deactivate();
    nameBasedIndex.remove(sp);
    yearBasedIndex.remove(sp);
    inactiveCount++;
  }
  if(2*inactiveCount >= playlist.size())
  {
    performActualDeletion(); 
    inactiveCount = 0;
  }
}

int
MusicList::getNumberOfSongs( ) const
{
  return nameBasedIndex.getSize();
}

void
MusicList::printAllNameSorted( ) const
{
    printAllNameSorted(nameBasedIndex.root);
}

void
MusicList::printAllYearSorted( ) const
{
    printAllYearSorted(yearBasedIndex.root);
}

void
MusicList::printNameRange( const string & lower,
                           const string & upper ) const
{
  printNameRange(lower, upper, nameBasedIndex.root);
}


void
MusicList::printYearRange( int lower, int upper ) const
{
  printYearRange(lower, upper, yearBasedIndex.root);
}

void 
MusicList::performActualDeletion( )
{
  typename list<Song>::iterator itr = playlist.begin();
  while(itr != playlist.end())
  {
    if((*itr).isActive() == false)
    {
      itr = playlist.erase(itr);
    }
    else
    {
      itr++;
    }
  }
}

void
MusicList::printAllYearSorted(Tree<Song, YearComparator>::TreeNode* t ) const
{
    if(t)
    {
        printAllYearSorted(t->left);
        cout << *(t->element) << endl;
        printAllYearSorted(t->right);
    }
}

void
MusicList::printAllNameSorted(Tree<Song, NameComparator>::TreeNode* t ) const
{
    if(t)
    {
        printAllNameSorted(t->left);
        cout << *(t->element) << endl;
        printAllNameSorted(t->right);
    }
}

void
MusicList::printNameRange( const string & lower, const string &upper, Tree<Song, NameComparator>::TreeNode* t) const
{
    if(t)
    {
        printNameRange(lower, upper, t->left);
        if(lower <= t->element->getName() && t->element->getName() <= upper)
        {
            cout << *(t->element) << endl;
        }
        printNameRange(lower, upper, t->right);
    }
}

void
MusicList::printYearRange( int lower, int upper, Tree<Song, YearComparator>::TreeNode* t) const
{
    if(t)
    {
        printYearRange(lower, upper, t->left);
        if(lower <= t->element->getYear() && t->element->getYear() <= upper)
        {
            cout << *(t->element) << endl;
        }
        printYearRange(lower, upper, t->right);
    }
}