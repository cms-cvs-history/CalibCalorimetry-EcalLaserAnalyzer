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
#include "TStyle.h"

#include <TGWindow.h>
#include <TGDockableFrame.h>

// simple test program
#include "MusEcal.hh"
#include "MERunManager.hh"
#include "../../interface/ME.h"
#include "../../interface/TPNCor.h"
#include "../../interface/MEGeom.h"
#include "MEVarVector.hh"
#include "MEEBDisplay.hh"
#include "MEPlotWindow.hh"
#include "../../interface/MEChannel.h" 
#include "MERunManager.hh"

void setTDRStyle(int isIt1D) {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);

  tdrStyle->SetEndErrorSize(2);
  tdrStyle->SetErrorX(0.);

  tdrStyle->SetMarkerStyle(20);

//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);

// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);

// Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.13);
  tdrStyle->SetPadRightMargin(0.05);

// For the Global title:
  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);

// For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.05);

// For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

// Postscript options:
  tdrStyle->SetPaperSize(20.,20.);

  // set the paper & margin sizes
 if( isIt1D==0 ){
   tdrStyle->SetPaperSize(20,26);
   tdrStyle->SetPadTopMargin(0.05);
   tdrStyle->SetPadTopMargin(0.10);
   tdrStyle->SetPadRightMargin(0.16);
   tdrStyle->SetPadBottomMargin(0.16);
   tdrStyle->SetPadLeftMargin(0.16);
 }

 tdrStyle->SetMarkerStyle(2);
 tdrStyle->SetHistLineWidth(2);
 tdrStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

 tdrStyle->SetOptStat(0);
 tdrStyle->SetOptFit(0);

 tdrStyle->SetPadTickX(1);
 tdrStyle->SetPadTickY(1);
 tdrStyle->SetPalette(1);

  tdrStyle->cd();
}

int main(int argc, char **argv)
{

  int c;
  int first=132306;
  int last=133000;
  int lmregion=1; 

  
  string fname_(getenv("MUSECAL"));
  cout<< fname_<< endl;
  stringstream dirss;
  dirss<<fname_<<"/STAB";

  string savedir=dirss.str();

  int color = 0; 

  while ( (c = getopt( argc, argv, "f:l:r:d:c:" ) ) != EOF ) 
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
	case 'r': 
	  lmregion       =  atoi ( optarg ) ;    
	  cout <<" lmr: "<< lmregion << endl;
	  break;
	case 'd': 
	  savedir       =   optarg  ;    
	  cout <<" savedir "<< savedir << endl;
	  break;
	case 'c': 
	  color       =   atoi ( optarg ) ;   
	  cout <<" color "<< color << endl;
	  break;
	}
    }

  
  MusEcal::firstRun=first;
  MusEcal::lastRun =last;

  int lmmin=lmregion; //  1
  int lmmax=lmregion+1; // 72

  // Barrel:
  //========
  int reg=ME::ecalRegion( lmmin );
  bool isBarrel=true;
  double xmin=-95.5;
  double xmax=94.5;
  double ymin=-9.5;
  double ymax=370.5;
  int nBinsX=190;
  int nBinsY=380;
  int iz=1;

  if(reg==ME::iEEM || reg==ME::iEEP){
    isBarrel=false;
  }
  
  if(!isBarrel){
    xmin=-4.5;
    xmax=105.5;
    ymin=-105.5;
    ymax=105.5;
    nBinsX=110;
    nBinsY=211;
    if( lmregion>82 ) iz=-1;
    
    TFile *_file = TFile::Open("/nfshome0/ecallaser/MusEcalJulie/geom/eegeom.root" );
    assert( _file!=0 );
    TH2F *_ee_h     = (TH2F*) _file->Get("ee_lmr");
    
    TAxis *xaxe=_ee_h->GetXaxis();
    TAxis *yaxe=_ee_h->GetYaxis();
    
    xmin=xaxe->GetXmin();
    xmax=xaxe->GetXmax();
    ymin=yaxe->GetXmin();
    ymax=yaxe->GetXmax();
    nBinsX=_ee_h->GetNbinsX();
    nBinsY=_ee_h->GetNbinsY();
    
  }
  
  stringstream name;
  if(color==0){
    if(isBarrel) name<<savedir<<"/StabilityEB"<<lmregion<<"_"<< first<<"_"<<last<<".root";
    else name<<savedir<<"/StabilityEE"<<lmregion<<"_"<< first<<"_"<<last<<".root";
  }else{

    if(isBarrel) name<<savedir<<"/StabilityEBRed"<<lmregion<<"_"<< first<<"_"<<last<<".root";
    else name<<savedir<<"/StabilityEERed"<<lmregion<<"_"<< first<<"_"<<last<<".root"; 
  }

  int type  = ME::iLaser; 
  int debug = 0;
  
  // Declare output file and histograms
  //====================================

  TFile* fileSave = new TFile(name.str().c_str(), "RECREATE");

  TH2F* eb2D[3][2]; TH2F * nevt[2];
  TH2F* eb2Dbest[2];
  eb2Dbest[0]= new TH2F( "stabBest2D" ,      "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );
  eb2Dbest[1]= new TH2F( "stabBest2DABFix" , "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );
 
  TH2F* pn2Dbest[2];
  pn2Dbest[0]= new TH2F( "BestNorm2D" ,      "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );
  pn2Dbest[1]= new TH2F( "BestNorm2DABFix" , "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );


  eb2D[0][0]       = new TH2F( "stabA2D" , "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );
  eb2D[1][0]       = new TH2F( "stabB2D" , "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );
  eb2D[2][0]       = new TH2F( "stab2D",   "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );
  nevt[0]        = new TH2F( "nevt2D",   "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );

  eb2D[0][1]       = new TH2F( "stabA2DABFix" , "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );
  eb2D[1][1]       = new TH2F( "stabB2DABFix" , "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );
  eb2D[2][1]       = new TH2F( "stab2DABFix",   "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );
  nevt[1]        = new TH2F( "nevt2DABFix",   "", nBinsX, xmin, xmax, nBinsY, ymin, ymax );

  for (int ii=0;ii<3;ii++){
    for (int jj=0;jj<2;jj++){
      eb2D[ii][jj]->SetStats( kFALSE );
      eb2D[ii][jj]->GetXaxis()->SetTitle("i #eta");
      eb2D[ii][jj]->GetXaxis()->CenterTitle();
      eb2D[ii][jj]->GetYaxis()->SetTitle("i #phi");
      eb2D[ii][jj]->GetYaxis()->CenterTitle();
    }
  }
  

  TH1F* eb1D[3][2];
  eb1D[0][0]  = new TH1F("stabA1D" , "", 1000, 0.0, 2.0);
  eb1D[1][0]  = new TH1F("stabB1D" , "", 1000, 0.0, 2.0);
  eb1D[2][0]  = new TH1F("stab1D", "", 1000, 0.0, 2.0);

  eb1D[0][1]  = new TH1F("stabA1DABFix" , "", 1000, 0.0, 2.0);
  eb1D[1][1]  = new TH1F("stabB1DABFix" , "", 1000, 0.0, 2.0);
  eb1D[2][1]  = new TH1F("stab1DABFix", "", 1000, 0.0, 2.0);

  TH1F* eb1Dbest[2];
  eb1Dbest[0]=new TH1F("stabBest1D" , "", 1000, 0.0, 2.0);
  eb1Dbest[1]=new TH1F("stabBest1DABFix" , "", 1000, 0.0, 2.0);

  // Set histograms titles
  //======================
  for (int jj=0;jj<2;jj++){
    
    eb1D[0][jj]->SetTitle("APD/PNA");
    eb1D[1][jj]->SetTitle("APD/PNB");
    eb1D[2][jj]->SetTitle("APD/PN");
    
    eb1D[0][jj]->SetXTitle("RMS (%)");
    eb1D[1][jj]->SetXTitle("RMS (%)");
    eb1D[2][jj]->SetXTitle("RMS (%)");

    eb1Dbest[jj]->SetTitle("APD/PN");
    eb1Dbest[jj]->SetXTitle("RMS (%)");
    
  }
  if (debug==1) cout << " Creating MusEcal Instance ..."<< endl;

  cout<< " Creating MusEcal "<< endl ;
  MusEcal *me = new MusEcal(ME::iLaser, color);
  cout<< " ... done"<< endl ;


  double mean[900][3][2];
  double rms[900][3][2];
  double sum[900][3][2];
  double sum2[900][3][2];
  double nn[900][3][2];


  double meannocor[900][3][2];
  double rmsnocor[900][3][2];
  double sumnocor[900][3][2];
  double sum2nocor[900][3][2];

  
  int nVarAnal1=3;
  int nVarAnal2=2;
  int iVarAnal[3][2];

  iVarAnal[0][0]=ME::iAPD_OVER_PNACOR_MEAN;
  iVarAnal[1][0]=ME::iAPD_OVER_PNBCOR_MEAN; 
  iVarAnal[2][0]=ME::iAPD_OVER_PNCOR_MEAN;

  iVarAnal[0][1]=ME::iAPDABFIX_OVER_PNACOR_MEAN;
  iVarAnal[1][1]=ME::iAPDABFIX_OVER_PNBCOR_MEAN; 
  iVarAnal[2][1]=ME::iAPDABFIX_OVER_PNCOR_MEAN;
  
  stringstream nameVarAnal[3][2];
  nameVarAnal[0][0]<< "APD_OVER_PNA_MEAN";
  nameVarAnal[1][0]<< "APD_OVER_PNB_MEAN";
  nameVarAnal[2][0]<< "APD_OVER_PN_MEAN";

  nameVarAnal[0][1]<< "APDABFIX_OVER_PNA_MEAN";
  nameVarAnal[1][1]<< "APDABFIX_OVER_PNB_MEAN";
  nameVarAnal[2][1]<< "APDABFIX_OVER_PN_MEAN";
   
  double nrunmean=0;
  int nrunmeanC=0;
 
  // Loop on LMR :
  //==============

  vector< MEChannel* > vec;
  std::vector<int> iapdopn;
  
  for (int lmr=lmmin; lmr<lmmax;lmr++){  
    
    MERunManager* testMgr = new MERunManager( lmr, type, color );
    
    if( testMgr->size()==0 )
      {
	delete testMgr;
	continue;
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
	
	vector< float > valtake;
	vector< bool  > flagtake;
	
	vector< float > valinit[3][2];
	vector< bool  > flaginit[3][2];
	
	float  valcor[3][2];
	bool   flagcor[3][2];
	
	vector< float > shapeCorVal;
	vector< bool >   shapeCorFlag;

	vector< float > shapeCorPNAVal;
	vector< bool >   shapeCorPNAFlag;
	
	vector< float > shapeCorPNBVal;
	vector< bool >   shapeCorPNBFlag;
	
	for(int j1=0;j1<nVarAnal1;j1++){
	  for(int j2=0;j2<nVarAnal2;j2++){
	    
	    mean[ii][j1][j2]=0.0;
	    rms[ii][j1][j2]=0.0;
	    sum[ii][j1][j2]=0.0;
	    sum2[ii][j1][j2]=0.0;
	    nn[ii][j1][j2]=0.0;

	    meannocor[ii][j1][j2]=0.0;
	    rmsnocor[ii][j1][j2]=0.0;
	    sumnocor[ii][j1][j2]=0.0;
	    sum2nocor[ii][j1][j2]=0.0;
	  }	
	}
	
	MEChannel* _leaf = vec[ii];
	MEChannel* leaf_ = _leaf;
	
	int iX=leaf_->ix();
	int iY=leaf_->iy();
	
	// Get back variables:
	//====================
		
	MEChannel* pnleaf_=leaf_;
	if(pnleaf_->ig()>ME::iLMModule){
	  while( pnleaf_->ig() != ME::iLMModule){
	    pnleaf_=pnleaf_->m();
	  }
	}

	MEVarVector* apdVector_;
	MEVarVector* pnaVector_;
	MEVarVector* pnbVector_;

	apdVector_ =  me->curMgr()->apdVector(leaf_);
	pnaVector_ =  me->curMgr()->pnVector(pnleaf_,0);
	pnbVector_ =  me->curMgr()->pnVector(pnleaf_,1);
	

	apdVector_->getTime( timeGlobal );
	
	for( int k1=0;k1<nVarAnal1;k1++){ 
	  for( int k2=0;k2<nVarAnal2;k2++){ 
	    
	    apdVector_->getValAndFlag( iVarAnal[k1][k2], timeGlobal, valtake ,flagtake );
	    
	    valinit[k1][k2]  = valtake; 
	    flaginit[k1][k2] = flagtake;
	    
	  }
	}
	
	apdVector_->getValAndFlag(ME::iAPD_SHAPE_COR, timeGlobal, shapeCorVal, shapeCorFlag );

	pnaVector_->getValAndFlag(ME::iPN_SHAPE_COR, timeGlobal, shapeCorPNAVal, shapeCorPNAFlag );
	pnbVector_->getValAndFlag(ME::iPN_SHAPE_COR, timeGlobal, shapeCorPNBVal, shapeCorPNBFlag );
      
	
	unsigned int nrun = timeGlobal.size();
	nrunmean+=nrun;
	nrunmeanC++;
	
	if(nrun<=1){
	  delete apdVector_;
	  continue;
	}
      
	// Loop on runs and apply selection:
	//==================================
      
	double shapeCorMin=0.7;
	double shapeCorPNMin=0.9;
	
	if(!isBarrel){
	  shapeCorPNMin=0.7;
	  shapeCorMin=0.6;
	  
	}
	
	
	for (unsigned int irun=0;irun<nrun;irun++){
	  
	  
	  // Calculate stabilities for selected runs:
	  //=========================================
	  
	  double corr[3];
	  bool myflag[3];
	  for(int kk=0;kk<nVarAnal1;kk++){
	    corr[kk]=1.0;
	  }


	  if(shapeCorVal[irun]>shapeCorMin && shapeCorFlag[irun]){
	    
	    corr[0] = 1.0/shapeCorVal[irun];
	    corr[1] = 1.0/shapeCorVal[irun];
	    corr[2] = 1.0/shapeCorVal[irun];
	    myflag[0]=true;
	    myflag[1]=true;
	    myflag[2]=true;

	    // fixme: fix for endcap + problem with shapecorrectionpn...

	    if(lmr<73 && lmr >82){
	      
	      if( shapeCorPNAVal[irun]> shapeCorPNMin && shapeCorPNBVal[irun]>shapeCorPNMin && shapeCorPNAFlag[irun] && shapeCorPNBFlag[irun]){
		corr[2]*=0.5*(shapeCorPNAVal[irun]+shapeCorPNBVal[irun]);
		myflag[2]=true;
	      }else  myflag[2]=false;
	      
	      if( shapeCorPNAVal[irun]>shapeCorPNMin && shapeCorPNAFlag[irun]){
		corr[0]*=shapeCorPNAVal[irun];  
		myflag[0]=true;
	      }else  myflag[0]=false;
	      
	      if( shapeCorPNBVal[irun]>shapeCorPNMin && shapeCorPNBFlag[irun]){
		corr[1]*=shapeCorPNBVal[irun];  
		myflag[1]=true;
	      }else  myflag[1]=false;
	    }
	    
	  }else{
	    myflag[0]=false;
	    myflag[1]=false;
	    myflag[2]=false;
	  }
	  
	  for(int k1=0;k1<nVarAnal1;k1++){
	    for(int k2=0;k2<nVarAnal2;k2++){
	   
	      flagcor[k1][k2]=flaginit[k1][k2][irun]*myflag[k1];
	      valcor[k1][k2]=valinit[k1][k2][irun]*corr[k1];
	      
	      if( flagcor[k1][k2] ){
		
		sum2[ii][k1][k2]+=valcor[k1][k2]*valcor[k1][k2];
		sum[ii][k1][k2]+=valcor[k1][k2];
		
		sum2nocor[ii][k1][k2]+=valinit[k1][k2][irun]*valinit[k1][k2][irun];
		sumnocor[ii][k1][k2]+=valinit[k1][k2][irun];
		
		nn[ii][k1][k2]+=1.0;
		
	      }
	      //else{
	      //cout<< "flag not ok "<<irun<<" "<<lmr<<" "<<valcor[k1][k2]<<endl;
	      //}
	    }
	  }

	}
		

	for(int j1=0;j1<3;j1++){
	  for(int j2=0;j2<2;j2++){
	    
	    if(nn[ii][j1][j2]>10){
	      mean[ii][j1][j2]=sum[ii][j1][j2]/nn[ii][j1][j2];
	      rms[ii][j1][j2]=sqrt((sum2[ii][j1][j2]/nn[ii][j1][j2])-(mean[ii][j1][j2]*mean[ii][j1][j2]));
	      meannocor[ii][j1][j2]=sumnocor[ii][j1][j2]/nn[ii][j1][j2];
	      rmsnocor[ii][j1][j2]=sqrt((sum2nocor[ii][j1][j2]/nn[ii][j1][j2])-(meannocor[ii][j1][j2]*meannocor[ii][j1][j2]));
	    }else{
	      mean[ii][j1][j2]=-10;
	      rms[ii][j1][j2]=-10;
	      meannocor[ii][j1][j2]=-10;
	      rmsnocor[ii][j1][j2]=-10;
	    }
	    
	    //cout<<" Test1 var="<<j1<<" type="<<j2<<" sum="<< sum[ii][j1][j2]<<" sum2="<< sum2[ii][j1][j2]<<" nn="<<nn[ii][j1][j2]<<" mean="<<mean[ii][j1][j2]<<endl;

	    if(mean[ii][j1][j2]>0.0 && nn[ii][j1][j2]>10){
	      
	      // Fill stability histograms
	      //==========================
	      eb2D[j1][j2]->Fill(iX,iz*iY,100.0*rms[ii][j1][j2]/mean[ii][j1][j2]);
	      eb1D[j1][j2]->Fill(100.0*rms[ii][j1][j2]/mean[ii][j1][j2]);
	      
	      //cout<<" Test2 var="<<j1<<" type="<<j2<<" iX="<<iX<<" iY="<<iY<<" stab="<<100.0*rms[ii][j1][j2]/mean[ii][j1][j2]<< endl;
	      if (j1==0) nevt[j2]->Fill(iX,iz*iY,nn[ii][j1][j2]);
	      
	    }
	  }
	}
      	
	
	for(int j2=0;j2<2;j2++){
	  double take;
	  double a=99999.0;
	  if(mean[ii][0][j2]!=0.0) a= 100.0*rms[ii][0][j2]/mean[ii][0][j2];
	  double b=99999.0;
	  if(mean[ii][1][j2]!=0.0) b=100.0*rms[ii][1][j2]/mean[ii][1][j2];
	  double both=99999.0;
	  if(mean[ii][2][j2]!=0.0) both=100.0*rms[ii][2][j2]/mean[ii][2][j2];

	  int ichoice=2;
	  take=both;
	  
	  if( a==0.0 && b==0.0 && both==0.0 ){
	    take=0.0;
	  }else{
	    if (a==0.0) a=99999.0;
	    if (b==0.0) b=99999.0;
	    if (both==0.0) both=99999.0;

	    double minab=b;
	    int iab=1;	  
	    if(a<b){
	      minab=a;
	      iab=0;
	    }
	    if(minab<both){
	      ichoice=iab;
	      take=minab;
	    }
	    if (take==99999.0) take=0.0;
	  }
	  
	  if( take!=0.0 && (nn[ii][0][j2]>10 || nn[ii][1][j2]>10) ){
	    eb2Dbest[j2]->Fill(iX,iz*iY,take);
	    eb1Dbest[j2]->Fill(take);
	    pn2Dbest[j2]->Fill(iX,iz*iY,double(ichoice));
	    
	  }
	}
	
	delete apdVector_;
	
      }
    
    delete testMgr;
  }

  fileSave->Write();
  fileSave->Close();

  return 0; 
}
