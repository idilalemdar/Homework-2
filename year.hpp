#ifndef YEAR_COMPARATOR__
#define YEAR_COMPARATOR__
#include "song.hpp"

class YearComparator
{
public:
  bool operator( )( const Song & s1, const Song & s2 ) const
  {
  	unsigned short year1 = s1.getYear();
  	unsigned short year2 = s2.getYear();
  	if(year1 < year2)
  	{
  		return true;
  	}
  	else if(year1 > year2)
  	{
  		return false;
  	}
  	else
  	{
  		string band1 = s1.getBand();
    	string band2 = s2.getBand();
    	if(band1 < band2)
    	{
    		return true;
    	}
    	else if(band1 > band2)
    	{
    		return false;
    	}
    	else
    	{
    		string name1 = s1.getName();
		  	string name2 = s2.getName();
		    if(name1 < name2)
		    {
		    	return true;
		    }
		    else if(name1 > name2)
		    {
		    	return false;
		    }
		    else
		    {
		    	return false;
		    }	
    	}
  	}
  }
};

#endif
