#ifndef MEKeyInterval_hh
#define MEKeyInterval_hh

#include <list>
#include <TString.h>

#include "MERunManager.hh"

class MEKeyInterval 
{
public:

  static int instances;

  // public constructor
  MEKeyInterval( ME::Time firstKey, ME::Time lastKey );

  // public splitting
  void split( const std::list<ME::Time>& keys );

  // recursive destructor
  virtual ~MEKeyInterval();
  
  // find an interval at a given level
  MEKeyInterval* get( ME::Time key, unsigned level=1000 );

  // first and last intervals at required level connected to this 
  MEKeyInterval* first( unsigned level );
  MEKeyInterval* last( unsigned level );

  // accessors
  unsigned level() { return _level; }
  ME::Time firstKey() const { return _firstKey; }
  ME::Time lastKey()  const { return _lastKey; }
  bool isGood() const { return _good; }

  // interval navigation at same level
  MEKeyInterval* above() { return _above; }
  MEKeyInterval* next();
  MEKeyInterval* previous();
  MEKeyInterval* firstIn();
  MEKeyInterval* lastIn();

  // recursive printing
  void print( unsigned level );

  // one-line printing
  void oneLine();

  // convenient printing [key0;key1[
  TString inBrackets();

private :

  // private constructor
  MEKeyInterval( ME::Time firstKey, ME::Time lastKey, MEKeyInterval* previous, MEKeyInterval* above, bool OK=true ); 

  // private splitting
  MEKeyInterval* cut( ME::Time key );

  // key interval
  ME::Time _firstKey;
  ME::Time _lastKey;

  // navigation pointers
  MEKeyInterval* _previous;
  MEKeyInterval* _next;
  MEKeyInterval* _above;
  MEKeyInterval* _below;

  // level of the interval, 0 being the top interval
  unsigned _level;

  bool _good;

};

#endif
