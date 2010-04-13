#ifndef MECorrector2Var_hh
#define MECorrector2Var_hh

//
// Authors  : Gautier Hamel de Monchenault and Julie Malcles, Saclay 
//

#include <vector>
using namespace std;
class TF1;

//#include "MusEcalHist.hh"
#include "METimeInterval.hh"

class MEVarVector;
class MECorrector2Var;
typedef map< METimeInterval*, MECorrector2Var* > MECorMap;

class MECorrector2Var
{
public:

  static unsigned int nrunsmin;
  static MECorMap corMap;
  static void cleanup();
  static MECorrector2Var* corrector( METimeInterval* ki );

  MECorrector2Var( MEVarVector* varvec0, MEVarVector* varvec1,
		   METimeInterval* interval, 
		   int type, 
		   int var0, int zoom0, 
		   int var1, int zoom1, 
		   unsigned fitDegree=1,
		   double x0 = -1,
		   double y0 = -1);
  MECorrector2Var( MEVarVector* varvec0, MEVarVector* varvec1,
		   METimeInterval* interval, 
		   int type, 
		   int var0, int zoom0, 
		   int var1, int zoom1, 
		   unsigned fitDegree,
		   double x0 ,
		   double y0 ,
		   vector<double> beta);
  virtual  ~MECorrector2Var();

  // perform the fit
  void fit();
  bool fitOK() { return _fitOK; }

  // get the correction factor for a given value
  MECorrector2Var* getCorrector( ME::Time key, unsigned level );
  double correctionFactor( double val0 );

  // accessors
  METimeInterval* interval() { return _interval; }
  int      var( int jj=1 )  { return _var[jj]; }
  int      zoom( int jj=1 ) { return _zoom[jj]; }
  double   norm( int jj=1 ) { return _var_norm[jj]; }
  unsigned level() { return _interval->level(); }
  

  // variable transform
  double toReduced( int jj, double val ); 
  double fromReduced( int jj, double reduced_val ); 

  // the fit function
  TF1* fitFunction() { return _f; }
  double f( double reduced_val0 );

  // polynomial coefficients
  double fitParams( unsigned ii );
  double betaParams( unsigned ii, double alpha );
  void setBetaParams( vector<double> betaParams );

  // printing
  void print();

private:
  MEVarVector* _varvec[2];
  MEVarVector* _varvec0;
  MEVarVector* _varvec1;
  METimeInterval* _interval;

  // map of corrections for sub intervals at level+1
  //  MECorMap _corMap; 
  
  int _type;

  int _var[2];
  int _zoom[2];
  double _val0[2];
  double _var_norm[2];
  double _var_min[2];
  double _var_max[2];
  unsigned _fitDegree;
  vector< double > _fitParams;

  unsigned _nruns;
  unsigned _nfit;

  bool _fitOK;
  bool _fitDone;
  TF1* _f;

  // debug
  bool _debug;

  ClassDef(MECorrector2Var,0) // MECorrector2Var -- Monitoring utility for survey of Ecal

};

#endif

