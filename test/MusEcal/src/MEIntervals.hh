#ifndef MEIntervals_hh
#define MEIntervals_hh

//
// Authors  : Gautier Hamel de Monchenault and Julie Malcles, Saclay 
//

#include <vector>
using namespace std;

#include "MERunManager.hh"
#include "METimeInterval.hh"
class MEVarVector;

#define CUT 0.05
#define NBINSDIFF 9
#define TMAX 24
#define NRUNSMIN 5

class MEIntervals
{
public:

  enum level{ level1, level2, level3, level4, level5, nlevel };
  static double  threshold[nlevel];
  static int     lineColor[nlevel];
  static int     lineStyle[nlevel];
  static int     lineWidth[nlevel];

  MEIntervals( MEVarVector* varvec0,MEVarVector* varvec1, ME::Time firstkey, ME::Time lastkey,
	       int var0, int var1=-1);
  virtual  ~MEIntervals();

  // methods to set parameters
  void setCut  ( double cut     ){ _cut  = cut;  }
  void setNbinsDiff ( int nbinsdiff  ){ _nbinsdiff  = nbinsdiff;}
  void setTmax ( double tmax    ){ _tmax = tmax; }
  void setNRunsMin ( double nrunsmin    ){ _nrunsmin = nrunsmin; }
 
  // accessors
  bool useTwoVar() const { return _twoVar; }
  METimeInterval* topInterval() { return _topInterval; }
  vector< ME::Time >& key();
  vector< float >& diff();
  vector< float >& val( unsigned ii=0 );
  int var( unsigned ii=0 ) { return _var[ii]; }
  double norm( unsigned ii=0 ) { return _norm[ii]; }
  double min( unsigned ii=0 )  { return _min[ii]/_norm[ii]; }
  double max( unsigned ii=0 )  { return _max[ii]/_norm[ii]; }

  // method to get validity intervals
  void splitIntervals( double cut );

  // get intervals 
  list<ME::Time> getIntervals(){ return _intervals; }


  // debug
  bool debug;

private:

  //MEKeyInterval* _topInterval;
  METimeInterval* _topInterval;

  bool _cleaned;
  bool _diffCreated;

  //  vector<unsigned int> _run;
  //  vector<unsigned int> _timestamp;
  vector<ME::Time> _key;
  
  bool _twoVar;

  bool _findMax;

  vector<float> _valtake;  
  vector<float> _val[2];  
  vector< bool > _val_ok[2];
  vector<bool> _valtakeok;  

  float _min[2];
  float _max[2];
  double _norm[2];
  
  vector< ME::Time > _diffkeys;
  vector< float > _diff;

  unsigned int _nruns;
  unsigned int _nrunsel;

  MEVarVector* _varvec[2];
  int _var[2];
  ME::Time _firstkey;
  ME::Time _lastkey;

  list<ME::Time> _intervals;

  double _cut;
  unsigned int _nbinsdiff;
  double _tmax;
  double _nrunsmin;

  // produce the diff vector 
  void createDiff();

  int getNInter( ME::Time key );

  // method to clean data from bad runs
  void init( );
  void cleanUp( );

  //ClassDef(MEIntervals,0) // MEIntervals -- Monitoring utility for survey of Ecal
};

#endif

