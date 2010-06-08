#include <cassert>
#include <iostream>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

#include "TMath.h"
#include "TRint.h"
#include "TGClient.h"
#include "TGraphErrors.h"
#include "TGraph.h"

#include <TGWindow.h>
#include <TGDockableFrame.h>

// simple test program

#include "MusEcal.hh"
#include "MusEcalGUI.hh"
#include "../../interface/MEGeom.h"
#include "MEVarVector.hh"
#include "MEEBDisplay.hh"
#include "MEPlotWindow.hh"
#include "../../interface/MEChannel.h" 
#include "MERunManager.hh"

int main(int argc, char **argv)
{
  
  int c;
  int first=0;
  int last=999999;

  while ( (c = getopt( argc, argv, "f:l:m:d:" ) ) != EOF ) 
    {
      switch (c) 
	{
	case 'f': 
	  first       =  atoi ( optarg ) ;    
	  cout <<" first run "<<first << endl;
	  break;
	case 'l': 
	  last       =  atoi ( optarg ) ;    
	  cout <<" last run "<<last << endl;
	  break;
	}
    }

  string period(getenv("MEPERIOD"));
  
  int type  = ME::iLaser; 
  int color = ME::iBlue; 
  int debug = 0;

  MusEcal* me;
  me = new MusEcal( type, color ); 
  vector< MEChannel* > vec;
  int lmr=1;
    
  MERunManager* Mgr = new MERunManager( lmr, type, color );
    
  if( Mgr->size()==0 )
    {
      delete Mgr;
      cout<< "empty lmr "<<lmr<< endl;
      return 0;
    }
  
    cout << " Setting LMR to "<<lmr<< endl;
    me->setLMRegion(lmr); 
    
    cout << " ... done"<< endl;
    
    vec.clear();
    me->curMgr()->tree()->getListOfChannels( vec );
    
    int nChan=vec.size();
    
    cout<< "nChannels: "<<nChan<< endl;
    
    // Loop on channels inside LMR:
    //=============================
  
    for( int ii=0; ii<nChan; ii++ ) 
      {
  
	vector< ME::Time > timeGlobal;
	
	vector< float > APDPNCOR;
	vector< bool  > flagAPDPNCOR;

	vector< float > APDPNABFIXCOR;
	vector< bool  > flagAPDPNABFIXCOR;

	vector< float > APDPNABFITCOR;
	vector< bool  > flagAPDPNABFITCOR;

	vector< float > MID;
	vector< bool  > flagMID;

	vector< float > CLS;
	vector< bool  > flagCLS;
		
	vector< float > FWHM;
	vector< bool  > flagFWHM;

	vector< float > SHAPECOR_APD;
	vector< bool  > flagSHAPECOR_APD;

	vector< float > SHAPECOR_PNA;
	vector< bool  > flagSHAPECOR_PNA;

	vector< float > SHAPECOR_PNB;
	vector< bool  > flagSHAPECOR_PNB;

	MEChannel* _leaf = vec[ii];
	MEChannel* leaf_ = _leaf;

	int iX=leaf_->ix();
	int iY=leaf_->iy();
	
	// Get back variables:
	//====================
	
	MEChannel* pnleaf_=leaf_;
	MEChannel* mtqleaf_=leaf_;
	
	if(_leaf->ig()>ME::iLMModule){
	  while( pnleaf_->ig() != ME::iLMModule){
	    pnleaf_=pnleaf_->m();
	  }
	}
	if(_leaf->ig()>ME::iLMRegion){
	  while( mtqleaf_->ig() != ME::iLMRegion){
	    mtqleaf_=mtqleaf_->m();
	  }
	}
      
	MEVarVector* apdVector_;
	MEVarVector* pnaVector_;
	MEVarVector* pnbVector_;
	MEVarVector* mtqVector_; 
	MEVarVector* midvec ;
	

	apdVector_ =  me->curMgr()->apdVector(leaf_);
	midvec = me->midVector(leaf_);
	

	apdVector_->getTime( timeGlobal );
	pnaVector_ =  me->curMgr()->pnVector(pnleaf_,0);
	pnbVector_ =  me->curMgr()->pnVector(pnleaf_,1);
	mtqVector_ =  me->curMgr()->mtqVector(mtqleaf_);
	
	pnaVector_->getValAndFlag( ME::iPN_SHAPE_COR, timeGlobal, SHAPECOR_PNA, flagSHAPECOR_PNA );
	pnbVector_->getValAndFlag( ME::iPN_SHAPE_COR, timeGlobal, SHAPECOR_PNB, flagSHAPECOR_PNB );

	apdVector_->getValAndFlag( ME::iAPD_SHAPE_COR, timeGlobal, SHAPECOR_APD, flagSHAPECOR_APD );
	apdVector_->getValAndFlag( ME::iAPD_OVER_PNCOR_MEAN, timeGlobal,APDPNCOR,flagAPDPNCOR );
	apdVector_->getValAndFlag( ME::iAPDABFIT_OVER_PNCOR_MEAN, timeGlobal, APDPNABFITCOR, flagAPDPNABFITCOR );
	apdVector_->getValAndFlag( ME::iAPDABFIX_OVER_PNCOR_MEAN, timeGlobal,APDPNABFIXCOR, flagAPDPNABFIXCOR  );

	midvec->getValAndFlag(ME::iMID_MEAN, timeGlobal,MID,flagMID);

 
	mtqVector_->getValAndFlag( ME::iMTQ_FWHM, timeGlobal, FWHM, flagFWHM ); 
	

	for (int it=0;it<timeGlobal.size();it++){
	  double mycor=1.0;
	  if(SHAPECOR_PNA[it]>0 && flagSHAPECOR_PNA[it]) mycor=SHAPECOR_APD[it]/SHAPECOR_PNA[it];
	  double myval= APDPNCOR[it];
	  if(mycor!=0) myval/=mycor;

	  cout<< " my val:" <<myval<< endl;
	}

      }
}
