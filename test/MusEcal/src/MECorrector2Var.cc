#include <assert.h>
#include <sstream>
#include "TF1.h"
#include "TGraphErrors.h"
#include "MECorrector2Var.hh"
#include "MEIntervals.hh"
#include "MERun.hh"
#include "../../interface/MEChannel.h"
#include "MEVarVector.hh"
#include "MusEcal.hh"

#include <TMath.h>

unsigned int MECorrector2Var::nrunsmin = 100;
MECorMap MECorrector2Var::corMap;

ClassImp(MECorrector2Var)
  
  // FIXME: type == color now

MECorrector2Var::MECorrector2Var( MEVarVector* varvec0, MEVarVector* varvec1,  
				  METimeInterval* interval, 
				  int type , 
				  int var0, int zoom0,
				  int var1, int zoom1, unsigned fitDegree,
				  double x0, double y0 ) 
  :  _varvec0( varvec0 ), _varvec1( varvec1 ), _interval( interval ),
  _type( type ), _fitDegree( fitDegree ) 
{
  _debug=false;  

  _varvec[0]   = varvec0;
  _varvec[1]   = varvec1;
  _var[0]   = var0;
  _var[1]   = var1;
  _zoom[0]  = zoom0;
  _zoom[1]  = zoom1;
  _val0[0]  = x0;
  _val0[1]  = y0;

  if( _debug ) cout << "Entering MECorrector2Var constructor " << endl;

  _nruns = 0;
  _nfit  = 0;

  for( int jj=0; jj<2; jj++ )
    {
      _var_norm[jj] = 0;
      _var_min[jj]  = 0;
      _var_max[jj]  = 0;
    }
  
  _fitParams.clear();
  _fitParams.resize( _fitDegree+1, 0. );
  
  _fitDone = false;
  _fitOK   = false;
  _f = 0;

  assert( _interval!=0 );

  corMap[ _interval ] = this;

  // OK, perform the fit
  fit();
  if( _debug ) print();

}

MECorrector2Var::MECorrector2Var( MEVarVector* varvec0, MEVarVector* varvec1,  
				  METimeInterval* interval, 
				  int type , 
				  int var0, int zoom0,
				  int var1, int zoom1, unsigned fitDegree,
				  double x0, double y0, vector<double> beta ) 
  :  _varvec0( varvec0 ), _varvec1( varvec1 ), _interval( interval ),
     _type( type ), _fitDegree( fitDegree ) 
{
  _debug=false;  
  
  _varvec[0]   = varvec0;
  _varvec[1]   = varvec1;
  _var[0]   = var0;
  _var[1]   = var1;
  _zoom[0]  = zoom0;
  _zoom[1]  = zoom1;
  _val0[0]  = x0;
  _val0[1]  = y0;

  if( _debug ) cout << "Entering MECorrector2Var constructor2 " << endl;

  _nruns = 0;
  _nfit  = 0;

  for( int jj=0; jj<2; jj++ )
    {
      _var_norm[jj] = 0;
      _var_min[jj]  = 0;
      _var_max[jj]  = 0;
    }
  
  _fitParams.clear();
  _fitParams.resize( _fitDegree+1, 0. );
  
  _fitDone = true;
  _fitOK   = true;
  
  if(_debug){
    cout << " betaParams " <<beta[0]<<"  " <<beta[1]<< endl;
    
  }
  // OK, perform the fit
  _f = 0;
  fit();
  setBetaParams(beta);
  

  assert( _interval!=0 );
  corMap[ _interval ] = this;
  
  if( _debug ) print();
}


MECorrector2Var::~MECorrector2Var() 
{  
  corMap.erase( _interval );

}

double
MECorrector2Var::toReduced( int jj, double val_ )
{
  double reduced_val_(0);
  double zr_ = MusEcal::zoomRange[ _zoom[jj] ];
  double norm_ = _var_norm[jj];
  assert( zr_!=0 );
  if( norm_==0 ) return -100;
  reduced_val_ =  ( val_-norm_ )/norm_/zr_;
  return reduced_val_;
}

double
MECorrector2Var::fromReduced( int jj, double reduced_val )
{
  if( !_fitDone ) fit();
  assert( _fitDone );
  double val(0);
  double zr = MusEcal::zoomRange[ _zoom[jj] ];
  double norm = _var_norm[jj];
  assert( zr!=0 );
  if( norm==0 ) return -101;
  val = norm*( 1 + reduced_val*zr );
  return val;
}

double 
MECorrector2Var::f( double reduced_val0 )
{
  if( !_fitDone ) fit();
  assert( _fitDone );
  if( !_fitOK ) return 0;
  assert(  _f!=0 );
  return _f->Eval( reduced_val0 );
}

double
MECorrector2Var::correctionFactor( double val0 )
{
  if( !_fitDone ) fit();
  assert( _fitDone );
  double reduced_val0 = toReduced( 0, val0 );
  double reduced_val1 = f( reduced_val0 );
  //  return fromReduced( 1, reduced_val1 );
  double norm = _var_norm[1];
  if( norm==0 ) return 1;
  double zr = MusEcal::zoomRange[ _zoom[1] ];
  double cor =  norm*( 1 + reduced_val1*zr );
  return cor;
}

MECorrector2Var*
MECorrector2Var::getCorrector( ME::Time key, unsigned level )
{
  if( !_fitDone ) fit();
  assert( _fitDone );

  METimeInterval* ki = _interval->get( key, level );
  if( ki==0 ) return 0;
  
  MECorrector2Var* cor = corMap[ki];
  assert( cor!=0 );
  while( !cor->_fitOK )
    {
      ki = ki->above();
      if( ki==0 ) return cor;
      cor = corMap[ki];
    }
  return cor;
}

void
MECorrector2Var::fit()
{
  // Variables vectors   
  if(_debug) cout << "Entering MECorrector2Var::fit " << endl;

  _fitDone = true;

  // get vectors in interval

  // first get keys in interval (not really needed)
  vector< ME::Time > _key;
  _varvec[0]->getTime(_key, _interval);

  _nruns = _key.size();
  if( _debug )
    {
      cout << "Number of runs in range: " << _nruns << endl;
    }
  
  // then values
  vector< float >   _val[2];
  vector< bool >     _ok[2];
  for( unsigned jj=0; jj<2; jj++ )
    {

      _varvec[jj]->getTimeValFlagAndNorm(_var[jj],_key,_val[jj],_ok[jj],_var_norm[jj],_interval);
      
      if( _debug )
	{
	  cout << "var/nruns " << _var[jj] << "/" << _val[jj].size() << endl; 
	}
      assert( _val[jj].size()==_nruns );
    }

  // GHM -- if the value of x0 is given, use it as opposed to use the average in the interval
  if( _val0[0]>0 ) 
    {
      _var_norm[0] = _val0[0];
    }
  if( _val0[1]>0 ) 
    {
      _var_norm[1] = _val0[1];
    }

  if( _type>=ME::iSizeC )
    {
      cout << "Only for LASER variables " << endl;
      return;
    } 
  
  Double_t xx[2][_nruns];
  Double_t keep[_nruns];
  
  for( unsigned ii=0; ii<_nruns; ii++ )
    {
      keep[ii]  = true;
      
      for( unsigned jj=0; jj<2; jj++ )
	{
	  xx[jj][ii] = 0;

	  bool   ok  = _ok[jj][ii];
	  double val = _val[jj][ii];
	  double reduced_val = toReduced( jj, val ); 
	  if( !ok ) keep[ii] = false;
	  if( reduced_val<-1 || reduced_val>1 ) keep[ii] = false;
	  xx[jj][ii] = reduced_val;
	  if(_debug) cout<< "ii=" << ii<< ", jj=" << jj << " val=" << val << " reduced_val=" << reduced_val << endl;
	}
    }
  
  vector<double> xgood;
  vector<double> ygood;
  
  int jj=0;
  for( unsigned ii=0; ii<_nruns; ii++ )
    {
      if( keep[ii] )
	{
	  if( _debug ) cout << ii << "/" << jj << " " << xx[0][ii] << " " << xx[1][ii] << endl;
	  xgood.push_back( xx[0][ii] );
	  ygood.push_back( xx[1][ii] );
	  jj++;
	}
    }
  
  double minx(-1), maxx(+1);
  _nfit = xgood.size();
  if( _debug ) cout << "nfit/jj " << _nfit << "/" << jj << endl;
  
  // perform the fit only if enough runs 
  if( _nfit<nrunsmin )
    {
      if( _debug ) cout << "Not enough runs in interval to perform the fit -- no correction computed " << endl;
    }
  else
    {
      // fit
      if(_fitOK==false){
	_fitOK = true;
	
	double XX[_nfit]; double YY[_nfit]; 
	
	for( unsigned ii=0; ii<_nfit; ii++ ) { XX[ii]=xgood[ii]; YY[ii]=ygood[ii]; }
	TGraph* gr = new TGraph( _nfit, XX, YY );
	TString dumstr = "pol"; dumstr += _fitDegree;
	if( _debug ) cout << dumstr << endl;
	//      _f = new TF1( "f1", dumstr, minx, maxx );
	_f = new TF1( "f1", dumstr.Data(), minx, maxx );
	if( _debug ) _f->Print();
	gr->Fit("f1","N0Q");      
	if(_debug)  cout << " TF1 parameters check: "<<  _f->GetNpar() << " " << _fitDegree << endl;
	_fitParams.clear();
	_fitParams.resize( _fitDegree+1, 0 );
	for( int iparam=0; iparam<_f->GetNpar(); iparam++ )
	  {
	    double alpha_ = _f->GetParameter(iparam);
	    _fitParams[iparam] =  alpha_;
	  }
	delete gr;
      }
    }
}

void
MECorrector2Var::print()
{
  if(_debug) cout<< " Entering Print " << endl;

  _interval->oneLine();
  cout << "nruns=" << _nruns ;
  if( _fitOK )
    {
      cout << " nfit=" << _nfit;
      //      _f->Print();
      assert( _f!=0 );
      cout << "... Fit parameters --> "; 
      for(  int iparam=0; iparam<_f->GetNpar(); iparam++ )
 	{
 	  double alpha_fit = fitParams( iparam );
	  double alpha_    = _f->GetParameter(iparam);
	  //	  double alpha_ = 0;
 	  cout << " " << iparam << ":" << alpha_ << ":" << betaParams( iparam, alpha_fit ) 
	       << ":" << betaParams( iparam, alpha_ );
 	}
      cout << endl;
    }
  cout << endl;
}

void
MECorrector2Var::cleanup()
{
  MECorMap::iterator it;
  for( it=corMap.begin(); it!=corMap.end(); it++ )
    {
      METimeInterval* ki = it->first;
      MECorrector2Var* cor =  it->second;
      corMap.erase( ki );
      delete cor;
    }
}

MECorrector2Var*
MECorrector2Var::corrector(  METimeInterval* ki )
{
  if( ki==0 ) return 0;
  MECorrector2Var* cor_(0); 
  if( MECorrector2Var::corMap.count( ki )!=1 ) return 0;
  cor_ =  MECorrector2Var::corMap[ ki ];
  assert( cor_!=0 );
  if( cor_->fitOK() ) return cor_;
  return corrector( ki->above() );
}

double 
MECorrector2Var::fitParams( unsigned ii )
{
  assert( _fitDone );
  if( !_fitOK ) return 0;
  if( _fitParams.size()!=_fitDegree+1 )
    {
      cout << "size/fitDegree+1 :  " << _fitParams.size() << "/" << _fitDegree+1 << endl;
    }
  if( ii>_fitDegree ) return 0;
  return _fitParams[ii];
}

double 
MECorrector2Var::betaParams( unsigned ii, double alpha_ )
{
  assert( _fitDone );
  // assert( _fitParams.size()==_fitDegree+1 ); 
  if( ii>_fitDegree ) return 0;
  //double alpha_ = _fitParams[ii]; //
  double zx_    = MusEcal::zoomRange[ _zoom[0] ];
  double zy_    = MusEcal::zoomRange[ _zoom[1] ];
  double beta_ = 0;
  if( ii==0 )
    {
      beta_ = 1 + zy_*alpha_;
    }
  else
    {
      double x0_    = _var_norm[0];
      assert( zx_>0 && x0_>0 );
      beta_  = zy_*alpha_/pow( zx_*x0_, ii );
    }
  return beta_;
}

void
MECorrector2Var::setBetaParams( vector<double> betaParams )
{
  if(_debug) cout<<" Entering setBetaParams " << betaParams.size()<<" " <<_fitDegree+1<<endl;
  
  assert(betaParams.size()==_fitDegree+1 ); // consistency
  // if( _f!=0 ){
  //     cout << " delete f" << endl;
  //     delete _f;
  
  //   }
  //if( _debug ) cout<<" setBetaParams after delete " <<endl;
  // _fitParams.clear();
  //_fitParams.resize( _fitDegree+1, 0. );

  double minx(-1), maxx(+1);
  
  stringstream dumstrstr;
  dumstrstr<<"pol"<< _fitDegree<< endl;
  
  if( _debug ) cout << dumstrstr.str() << endl;
  _f = new TF1( "f1", dumstrstr.str().c_str(), minx, maxx );
  if( _debug ) _f->Print();

  double zx_    = MusEcal::zoomRange[ _zoom[0] ];
  double zy_    = MusEcal::zoomRange[ _zoom[1] ];
  double x0_    = _var_norm[0];
  assert( zx_>0 && x0_>0 );

  for( int iparam=0; iparam<_f->GetNpar(); iparam++ )
    {
      double beta_ = betaParams[iparam];
      double alpha_ = 0;
      if( iparam==0 )
	{
	  alpha_ = beta_/zy_;
	}
      else
	{
	  alpha_ = pow( zx_*x0_, iparam )*beta_/zy_;
	}

      _f->SetParameter( iparam, alpha_ );
      //_fitParams[iparam] = alpha_;
    }
  _fitOK = true;
}
