#include <iostream>
#include <assert.h>

#include "MusEcal.hh"
#include "MEIntervals.hh"
#include "MERun.hh"
#include "../../interface/MEChannel.h"
#include "MEVarVector.hh"

#include <TMath.h>


double MEIntervals::threshold[ MEIntervals::nlevel ] = { 0.03, 0.02, 0.01, 0.005, 0.0005 };
int MEIntervals::lineColor[ MEIntervals::nlevel ]    = { kMagenta, kBlue, kRed, kGreen, kYellow };
int MEIntervals::lineStyle[ MEIntervals::nlevel ]    = { kSolid, kSolid, kSolid, kSolid, kSolid };
int MEIntervals::lineWidth[ MEIntervals::nlevel ]    = { 1, 1, 1, 1, 1 };

ClassImp(MEIntervals)

MEIntervals::MEIntervals( MEVarVector* varvec0, MEVarVector* varvec1,  ME::Time firstkey, ME::Time lastkey, int var0, int var1 )
  :   _firstkey( firstkey ), _lastkey( lastkey )
{
  debug=false;  
  
  if( debug ) cout << "Entering MEIntervals constructor " << endl;
  
  // Initialize variables 
  _twoVar = ( var1!=-1 );
  assert( var0>0 );
  if( _twoVar ) { assert ( var1>0 && var1!=var0 ); }
  _var[0] = var0;
  _var[1] = var1; 

  _varvec[0] = varvec0;
  _varvec[1] = varvec1; 

  _norm[0]=1.0;
  _norm[1]=1.0;

  _topInterval=0;

  _findMax = true;

  _cut    = CUT; 
  _nbinsdiff = NBINSDIFF; 
  _tmax   = TMAX;
  _nrunsmin   = NRUNSMIN;
  if( !_findMax ) _nrunsmin = 0;
  
  _cleaned = false;
  _diffCreated = false;
  
  if(debug) cout << "Inside Init firstkey: "<< _firstkey<<" lastkey:"<<_lastkey<< endl;

  init();
  cleanUp();
  
  
}

MEIntervals::~MEIntervals() 
{
  delete _topInterval;
}

void MEIntervals::init()
{
  // get the run manager

  // create the top key interval
  _topInterval = new METimeInterval( _firstkey, _lastkey );
  
  if(debug) cout<< "CHECK first:"<< _firstkey<<" last:"<<_lastkey<<" "<<_twoVar<< endl;

  // first get the keys in interval
  _varvec[0]->getTime(_key, _topInterval);
  _nruns = _key.size();
  _nrunsel = _nruns;

  vector<ME::Time> key;
  double normtake=0;
  _varvec[0]->getTimeValFlagAndNorm(_var[0],key,_valtake, _valtakeok, normtake, _topInterval);
  
  _val[0]=_valtake;
  _val_ok[0]=_valtakeok;
  _norm[0]=normtake;

  assert( _val[0].size()==_nruns );
  if( _twoVar )
    {
      double normtake=0;
      _varvec[0]->getTimeValFlagAndNorm(_var[1],key,_valtake, _valtakeok,normtake,_topInterval);

      _val[1]=_valtake;
      _val_ok[1]=_valtakeok;
      _norm[1]=normtake;
      
      assert( _val[1].size()==_nruns );
    }
  
  _intervals.clear();
  _intervals.push_back(  _key[0] );
  _intervals.push_front( _key[_nruns-1] );

}

void
MEIntervals::cleanUp( ){
  
  for (unsigned int jrun=0; jrun<_nruns;jrun++) 
    { 
      bool ok =  ( jrun==0 || (_val[0][jrun]>0.0 && _val_ok[0][jrun] ) );
      
      if( !ok )
	{
	  if(debug) cout<< " in Cleanup not OK!!! " << _val[0][jrun]<<"  " <<_norm[0]<< endl; 
	  _val_ok[0][jrun] = false;
	  if( _twoVar ) _val_ok[1][jrun] = false;
	}
      
      // normalize
      _val[0][jrun] /= _norm[0];
      if( _twoVar ) _val[1][jrun] /= _norm[1];
      
    }
  
  _nrunsel=_nruns;
  
  int bad=0;
  for(unsigned int i=0;i<_nrunsel;i++ ) {
    
    bool erase = false;
    if( _twoVar && ( !_val_ok[0][i] || !_val_ok[1][i] ) ) erase = true;
    else if(  !_val_ok[0][i] ) erase = true;
    
    
    if( erase ) {
      if(debug) cout<< " Cleaning "<< i-bad << " "<<_key[i-bad]<<" "<<_val[0][i-bad] <<endl;
     
      _key.erase(_key.begin()+i-bad);
      _val[0].erase(_val[0].begin()+i-bad); 
      if( _twoVar ) _val[1].erase(_val[1].begin()+i-bad); 
      
      bad++;
    }else{
      if(debug) cout<< " Keeping "<< i-bad << " "<<_key[i-bad]<<" "<<_val[0][i-bad] <<endl;
    }
  }
  
  _nrunsel = _key.size();
  
  if(debug) {
    cout <<" +===================================+ " << endl; 
    cout <<" + Selected : "<< _nrunsel << " good runs over "<< _nruns<< endl;
    cout <<" +===================================+ " << endl; 
  }
  
  _cleaned = true;
  
}


void 
MEIntervals::splitIntervals( double cut )
{
  if( !_diffCreated ) createDiff();

   if(debug) cout << "Search for intervals with cut=" << cut << endl;
  _cut = cut;
 
  // ! faire le split quand meme...
  if(_nrunsel==0){
    //cout<< "No Run Selected: return"<< endl;
    _topInterval->split( _intervals );
    return;
  }
  
  _intervals.clear();
 
  // vectors of timestamps 
  vector<ME::Time>& _timestamp = _key;
 

  // Find _intervals for a given cut 
  
  double max=0.0;
  ME::Time curkey=_key[0];
  int rundiff=0;
  int k=0;
  map< ME::Time, unsigned int > nrunsintervals;

  if(debug) cout<<"FIXINGBUG: pushing back first key "<<  curkey << endl;

  _intervals.push_back( curkey ); // push_back first run
  nrunsintervals[ curkey ] = 0;
  
  if( _findMax )
    {

      for(unsigned int i=0;i<_nrunsel;i++ ) {
    
	if(TMath::Abs(_diff.at(i)) >= _cut) { 
      
	  if(debug) cout << "Cut passed for Run "<< _key.at(i) << " and diff with value "<<TMath::Abs(_diff.at(i))<< endl;
	
	  // find minimum
      
	  max=TMath::Abs(_diff.at(i));
	  ME::Time maxkey=_key.at(i);
	  rundiff=0;
	  k=1;
      
	  while (TMath::Abs(_diff.at(i+k))>=_cut && i+k<unsigned(_diff.size()))
	    {
	      if(TMath::Abs(_diff.at(i+k))>max) 
		{
		  max = TMath::Abs(_diff.at(i+k));
		  maxkey=_key.at(i+k);
		  rundiff=k;
		}	
	      k++;
	    }
	
	  if(debug) cout << "Maximum found to be "<< max<< " for run "<<rundiff<<" "<< curkey << " "<< _key.at(i+rundiff)<< endl;	
	  
	  // increment runs in previous interval
	  nrunsintervals[curkey]+=rundiff;

	  // create next interval
	  curkey = maxkey;
	  
	  if(debug) cout<<"FIXINGBUG: create next interval pushback nextkey "<<   _key[i]<< endl;
	  _intervals.push_back(curkey);  
	  nrunsintervals[curkey]=k-rundiff;

	  i+=k;
	
	}else if(i>0  && (_timestamp.at(i)-_timestamp.at(i-1)) >= _tmax*3600. ){
      
	  curkey = _key[i];
	  _intervals.push_back( curkey );
	  nrunsintervals[curkey]=0;
	}

	nrunsintervals[curkey]++;
      }
    }
  else
    {
      bool aboveCut = false;
      for( unsigned int i=0; i<_nrunsel; i++ ) 
	{
    
	  bool crossTheLine = false;
	  if(TMath::Abs(_diff.at(i)) >= _cut) 
	    { 
	      if( !aboveCut )
		{
		  crossTheLine = true;
		  aboveCut = true;
		}
	    }
	  else
	    {
	      if( aboveCut )
		{
		  crossTheLine = true;
		  aboveCut = false;
		}
	    }
	  
	  if( crossTheLine && debug) cout << "Crossing the cut line for Run "<< _key.at(i) << " and diff with value " << TMath::Abs(_diff.at(i)) << endl;
	  
	  if( crossTheLine || ( i>0 &&  (_timestamp[i]-_timestamp[i-1]) >= _tmax*3600. ) )
	    { 
	      // create next interval

	      if(debug) cout<<"FIXINGBUG: create next interval pushback nextkey "<<   _key[i]<< endl;
	      curkey = _key[i];
	      _intervals.push_back(curkey);  
	      nrunsintervals[curkey]=0;
	    }
	  nrunsintervals[curkey]++;
	}
    }

  if(debug) cout<<"FIXINGBUG:  pushback lastrun "<<   _key.at(_key.size()-1)<< endl;
  _intervals.push_back( _key.at(_key.size()-1) );  // push_back last run 


  list<ME::Time>::iterator it2=_intervals.begin();
  map< ME::Time, ME::Time> bounds;
  unsigned int lasttime=_key[0];
  unsigned int curtime=_key[0];
  unsigned int cinter=0;
  for( it2=_intervals.begin(); it2!=_intervals.end(); it2++ ){
    curtime=*it2;
    if (cinter>=1 || cinter==_intervals.size()-1 ){ // FIXME to be tested
      bounds[lasttime]=curtime;
    }
    lasttime=*it2;
    cinter++;    
  }

  // Cleanup intervals too small   
  ME::Time firstkey_ = _key[0];
  ME::Time lastkey_  = _key[0];
  unsigned int nruns_ = 0;
  unsigned int ncur_  = 0;

  map< ME::Time, unsigned int >::iterator it;
  for ( it=nrunsintervals.begin(); it!=nrunsintervals.end(); it++ ){
    
   
    firstkey_ = it->first;
    lastkey_  = bounds[firstkey_];
    nruns_    = it->second;
    nruns_   += ncur_;
    if(debug) cout << "NRuns[" << firstkey_ << "-->" << lastkey_<< "]=" << nruns_ <<endl;
    
    if( nruns_<_nrunsmin) 
      {
	if(debug) cout << "Remove interval " << lastkey_ << endl;
	_intervals.remove( lastkey_ );
	ncur_ += nruns_;
	lastkey_ = firstkey_;
      }
    else
      {
	ncur_ = 0;
      }
  }

  _intervals.unique();
  _intervals.sort();
  if(debug ) cout <<"Intervals vector size 1 " << _intervals.size() << endl;


  // if first sequence or last sequence have been cleaned, put them back eventually
  
  if(_intervals.front()!=_firstkey){
    if(_intervals.size()>=1 ) _intervals.pop_front();
    _intervals.push_front(_firstkey);
  } 

  if(_intervals.back()!=_lastkey){
    if(_intervals.size()>=2)_intervals.pop_back();
    _intervals.push_back(_lastkey);
  } 

  if(debug ) cout <<"Intervals vector size 2 " << _intervals.size() << endl;

  _topInterval->split( _intervals );
  
}

void 
MEIntervals::createDiff()
{
  _diffCreated = true;

  if(debug) cout << "Create diff vector ";
  if( _twoVar )
    {
     if(debug) cout << " for couple of variables (" <<  _var[0] << ", " <<  _var[1] << ")";
    }
  else
    {
     if(debug) cout << " for single variable " << _var[0]  ;
    }
  if(debug)cout << endl;
  
  //
  // Calculate diffs
  //
  
  unsigned int nn=_nbinsdiff;
  double summinus;
  double sumplus;
  
  if(debug) cout << "In CreateDiff: _nrunsel = "<<_nrunsel<< " nn "<< nn<<endl;

  _diff.resize( _nrunsel, 0. );


  for( unsigned int i=0; i<_nrunsel; i++ ) 
    {
      _diff[i] = 0.;
      //if(debug) cout << "In CreateDiff: entering loop " <<i<<"  " <<_twoVar<<endl;

      if( i<nn || i>=_nrunsel-nn ) continue;
    
      if( _twoVar )
	{
	  double rho_up=0;
	  double rho_down=0;
	  double eta_up=0;
	  double eta_down=0;
	  for( unsigned jj=0; jj<nn; jj++ )
	    {
	      rho_up   += _val[0][ i+1+jj ];
	      rho_down += _val[0][ i-1-jj ];
	      eta_up   += _val[1][ i+1+jj ];
	      eta_down += _val[1][ i-1-jj ];
	    }
	  rho_up   /= nn; 
	  rho_down /= nn; 
	  eta_up   /= nn; 
	  eta_down /= nn;
	  double rho   = rho_up - rho_down;
	  double eta   = eta_up - eta_down;
	  _diff[i] = sqrt( pow( rho, 2 ) + pow( eta, 2 ) );
	  
	  // fixme !
	  _diff[i] /= 2;
	  //if(debug) cout << "In CreateDiff: twoVar diff=" <<  _diff[i]<<endl;
	}
      else
	{
	  summinus=0.0;
	  sumplus=0.0;
	  
	  for (unsigned int k=1;k<nn+1;k++){
	    summinus+=_val[0].at(i-k);
	    sumplus+=_val[0].at(i+k);  
	  }
	  if(summinus!=0) 
	    {
	      _diff[i] = (sumplus-summinus)/summinus;  
	    }
	  //if(debug) cout << "In CreateDiff: oneVar diff=" <<  _diff[i]<<"  " <<sumplus<<"  " << summinus<< endl;
	}
      if(debug && i%50==0) cout << "diff[" <<i<< "]=" << _diff.at(i) <<  endl; 
 
    }
    
  assert( _diff.size()==_key.size() );

}

int
MEIntervals::getNInter( ME::Time key )
{
 
  int nInt=-1;

  unsigned int j=0;
  list<ME::Time>::iterator it;
  list<ME::Time>::iterator itnext;
  for( it=_intervals.begin(); it!=_intervals.end(); it++ ){
    itnext = it;
    if( ++itnext==_intervals.end() ) 
      {
	nInt = j;
	continue;
      }
    if( key>=*it && key<*itnext ) nInt=j;
    j++;
  }

//   if(key==_intervals.at(_intervals.size()-1)) nInt=_intervals.size()-2;
//   else{
//     for(unsigned int j=0;j<_intervals.size()-1;j++){
//       if(key >= _intervals.at(j) && key < _intervals.at(j+1)) nInt=j;
//     }
//   }
  
  return nInt;

}

vector< ME::Time >& 
MEIntervals::key() 
{ 
  if( !_diffCreated ) createDiff();
  return _key; 
}

vector< float >& 
MEIntervals::diff() 
{ 
  if( !_diffCreated ) createDiff();
  return _diff; 
}

vector< float >& 
MEIntervals::val( unsigned ii ) 
{ 
  if( !_diffCreated ) createDiff();
  return _val[ii]; 
}
