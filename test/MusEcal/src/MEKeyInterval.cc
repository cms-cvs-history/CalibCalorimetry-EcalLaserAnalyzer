#define MEKeyInterval_cxx
#include <assert.h>
#include <fstream>
#include <list>
#include <iostream>
#include <algorithm>
using namespace std;
#include <TString.h>

#include "MEKeyInterval.hh"

int MEKeyInterval::instances=0;
 
// public constructor
MEKeyInterval::MEKeyInterval( ME::Time firstKey, ME::Time lastKey )
  : _firstKey( firstKey ), _lastKey (lastKey ),
    _previous(0), _next(0), _above(0), _below(0), _level(0), _good(true)
{
  assert( _lastKey>_firstKey );
  instances++;
}

// private constructor
MEKeyInterval::MEKeyInterval( ME::Time firstKey, ME::Time lastKey, MEKeyInterval* previous, MEKeyInterval* above, bool good )
  : _firstKey(firstKey), _lastKey(lastKey),
    _previous(previous), _next(0), _above(above), _below(0), _good(good)
{
  instances++;
  if( _previous==0 ) 
    {
      assert( _above!=0 );
      assert( _firstKey == _above->_firstKey );
      assert( _lastKey == _above->_lastKey );
      _level = _above->_level + 1;
    }
  else
    {
      _level  = _previous->_level;
    }
  assert( _lastKey>_firstKey );
}

MEKeyInterval::~MEKeyInterval()
{
  if( _next )  delete _next;  _next=0;
  if( _below ) delete _below; _below=0;
  if( _previous ) _previous->_next=0;
  else if( _above ) _above->_below=0;
  instances--;
}

void
MEKeyInterval::split( const list<ME::Time>& keys )
{  
  if( _below!=0 ) _below->split( keys );
  else
    {
      _below = new MEKeyInterval( _firstKey, _lastKey, 0, this );
      MEKeyInterval* current = _below;
      list<ME::Time> theList( keys );
      theList.unique();
      theList.sort();
      list<ME::Time>::iterator it;
      for( it=theList.begin(); it!=theList.end(); it++ )
	{
	  ME::Time key = *it;
	  if( key<=_firstKey ) continue;
	  if( key>=_lastKey  ) break;
	  current = current->cut( key );
	}  
    }
  if( _next ) _next->split( keys );
}

MEKeyInterval*
MEKeyInterval::cut( ME::Time key )
{
  if( key<=_firstKey || key>=_lastKey ) return this;
  _next = new MEKeyInterval( key, _lastKey, this, _above );
  _lastKey = key;  
  return _next;
}

void 
MEKeyInterval::print( unsigned level )
{
  if( _level==level )
    {
      oneLine();
    }
  if( _below ) _below->print( level );
  if( _next  ) _next->print( level );
}

void 
MEKeyInterval::oneLine()
{
  cout << "firstKey/lastKey/level ";
  cout << _firstKey << "/" << _lastKey << "/" << _level << endl;
}

TString
MEKeyInterval::inBrackets()
{
  TString str;
  str += "[";
  str += _firstKey;
  str += ";";
  str += _lastKey;
  str += "[";
  return str;
}

MEKeyInterval*
MEKeyInterval::get( ME::Time key, unsigned level )
{
   if( _level>level )
     {
       assert( _above!=0 );
       return _above->get( key, level );
     }
   if( key<_firstKey )
    {
      if( _previous ) return _previous->get( key, level );
      return 0;
    }
  if( key>=_lastKey ) 
    {
      if( _next ) return _next->get( key, level );
      if( key==_lastKey )
	{
	  //	  if( _below ) return _below->get( key, level );
	  return 0;  // last run is excluded !
	}
      return 0;
    }
  if( _level==level ) return this;
  if( _below ) return _below->get( key, level );
  return this; // this is as far as it gets
}

MEKeyInterval*
MEKeyInterval::first( unsigned level )
{
  if( _level<level ) 
    {
      if( _below ) return _below->first( level ); 
      return 0;
    }
  if( _level>level ) 
    {
      if( _above ) return _above->first( level ); 
      return 0;
    }
  return this->firstIn();
}

MEKeyInterval*
MEKeyInterval::last( unsigned level )
{
  if( _level<level ) 
    {
      if( _below->lastIn() ) return _below->lastIn()->last( level ); 
      return 0;
    }
  if( _level>level ) 
    {
      if( _above->lastIn() ) return _above->lastIn()->last( level ); 
      return 0;
    }
  return this->lastIn();
}

MEKeyInterval*
MEKeyInterval::firstIn()
{
  if( _previous ) return _previous->firstIn(); 
  return this;
}
MEKeyInterval*
MEKeyInterval::lastIn()
{
  if( _next ) return _next->lastIn(); 
  return this;
}

MEKeyInterval*
MEKeyInterval::next()
{
  if( _next ) return _next; 
  if( _above==0 ) return 0;
  MEKeyInterval* _above_next = _above->next();
  if( _above_next==0 ) return 0;
  MEKeyInterval* _above_next_below = _above_next->_below;
  if( _above_next_below==0 ) return 0;
  return _above_next_below->firstIn();
}

MEKeyInterval*
MEKeyInterval::previous()
{
  if( _previous ) return _previous; 
  if( _above!=0 && _above->_previous!=0 && _above->_previous->_below!=0 ) 
    return _above->_previous->_below->lastIn();
  return 0;
}

//
// Test code 
//


//   MEKeyInterval* topInterval = new MEKeyInterval( 1, 100 );


//   ME::Time key;
//   bool store(false);

//   ifstream fin1;
//   fin1.open("keyList1");
//   std::list< ME::Time > keyList1;
//   do
//     {
//       if( store ) keyList1.push_back( key );
//       fin1 >> key;
//       cout << "key=" << key << endl;
//       store = true;
//     }
//   while( fin1.peek() != EOF );
//   cout << "size1= " << keyList1.size() << endl;
//   fin1.close();


//   ifstream fin2;
//   fin2.open("keyList2");
//   std::list< ME::Time > keyList2;
//   do
//     {
//       if( store ) keyList2.push_back( key );
//       fin2 >> key;
//       cout << "key=" << key << endl;
//       store = true;
//     }
//   while( fin2.peek() != EOF );
//   cout << "size2= " << keyList2.size() << endl;
//   fin2.close();

//   cout << "First splitting " << endl;
//   topInterval->split( keyList1 );

//   cout << "Second splitting " << endl;
//   topInterval->split( keyList2 );

//   for( int ii=0; ii<=2; ii++ )
//   {
//     cout << "\n --> Level " << ii << endl;
//     topInterval->print(ii);
//   }

//   unsigned testKey[5] = { 4, 19 , 20, 73, 102 };
//   MEKeyInterval* ki = topInterval;
//   for( int ii=0; ii<5; ii++ )
//     {

//       cout << "\n find " << testKey[ii] << endl;
//       ki = topInterval->get( testKey[ii], 1 );
//       if( ki!=0 ) ki->oneLine(); else cout << "not found " << endl;
//       ki = topInterval->get( testKey[ii], 2 );
//       if( ki!=0 ) ki->oneLine(); else cout << "not found " << endl;
//     }

//   MEKeyInterval* startKi = topInterval->first(1)->next();

//   for( int ii=0; ii<3; ii++ )
//     {
//       cout << "Iterate at level " << ii << endl;
//       for( ki=startKi->first(ii); ki!=0; ki=ki->next() )
// 	{
// 	  ki->oneLine();
// 	}
//     }

//   for( int ii=0; ii<3; ii++ )
//     {
//       cout << "Iterate in reverse direction at level " << ii << endl;
//       for( ki=startKi->last(ii); ki!=startKi->first(ii)->previous(); ki=ki->previous() )
// 	{
// 	  ki->oneLine();
// 	}
//     }
  
//   cout << "Number of MEKeyInterval instances " << MEKeyInterval::instances << endl; 
//   delete topInterval;
//   cout << "Number of MEKeyInterval instances " << MEKeyInterval::instances << endl; 
//
