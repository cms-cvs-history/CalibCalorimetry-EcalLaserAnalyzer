#include <sstream>
#include <cassert>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl; 
using std::ifstream;
using namespace std;

#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TProfile.h> 
#include <TPaletteAxis.h> 

#include "MEEBDisplay.hh"
#include "MEEEDisplay.hh"
#include "MEPlotWindow.hh"

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
  //tdrStyle->SetOptTitle(1);
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
  int type=0;
  int ecalPart=0; //EB

  while ( (c = getopt( argc, argv, "f:e:" ) ) != EOF ) 
    {
      switch (c) 
	{
	case 'f': 
	  type       =  atoi ( optarg ) ;    
	  cout <<" type "<<type << endl;
	  break;
	case 'e': 
	  ecalPart      =  atoi ( optarg ) ;    
	  cout <<" ecalPart "<<ecalPart << endl;
	  break;

	}
    }


  int lmmin=1;
  int lmmax=72;
 
  stringstream names2D[4][2];
  stringstream names1D[4][2];


  names2D[0][0]<<"stabA2D";
  names2D[1][0]<<"stabB2D";
  names2D[2][0]<<"stab2D";
  names2D[3][0]<<"stabBest2D";

  names2D[0][1]<<"stabA2DABFix";
  names2D[1][1]<<"stabB2DABFix";
  names2D[2][1]<<"stab2DABFix";
  names2D[3][1]<<"stabBest2DABFix";

  names1D[0][0]<<"stabA1D";
  names1D[1][0]<<"stabB1D";
  names1D[2][0]<<"stab1D";
  names1D[3][0]<<"stabBest1D";

  names1D[0][1]<<"stabA1DABFix";
  names1D[1][1]<<"stabB1DABFix";
  names1D[2][1]<<"stab1DABFix";
  names1D[3][1]<<"stabBest1DABFix";


  TH2D *histTot2D[4][2];
  TH1D *histTot1D[4][2];
  
  double xmin=-95.5;
  double xmax=94.5;
  double ymin=-9.5;
  double ymax=370.5;

  int nBinsX=190;
  int nBinsY=380;
  

  for(int i=0;i<4;i++){
    for(int j=0;j<2;j++){
      stringstream name2D;
      name2D<<"histTot2D_"<<i<<"_"<<j;
      stringstream name1D;
      name1D<<"histTot1D_"<<i<<"_"<<j;
      
      histTot2D[i][j]=new TH2D(name2D.str().c_str(),name2D.str().c_str(), 
			    nBinsX,xmin, xmax, nBinsY, ymin, ymax );
      histTot1D[i][j]=new TH1D(name1D.str().c_str(),name1D.str().c_str(),1000, 0.0, 2.0);
    }
  }

  cout<<" histos booked "<<endl;

  for (int lmr=lmmin; lmr<=lmmax;lmr++){  

    cout<< "lmr= "<< lmr << endl;

    stringstream name;
    name<<"STAB/StabilityEB"<<lmr<<".root";
    
    TH2D *hist2D[4][2];
    TH1D *hist1D[4][2];
    
    TFile f(name.str().c_str());
    
    for(int i=0;i<4;i++){
      for(int j=0;j<2;j++){
	
	hist2D[i][j]=(TH2D*) f.Get(names2D[i][j].str().c_str());
	hist1D[i][j]=(TH1D*) f.Get(names1D[i][j].str().c_str());
	
	
	for(int iX=1;iX<=nBinsX;iX++){
	  for(int iY=1;iY<=nBinsY;iY++){
	    if(hist2D[i][j]->GetBinContent(iX,iY)!=0.){
	      histTot2D[i][j]->SetBinContent(iX,iY,hist2D[i][j]->GetBinContent(iX,iY));
	    }  	 	    
	  }
	}
	for( int ix=1;ix<1001;ix++){
	  if(hist1D[i][j]->GetBinContent(ix)!=0.){
	    histTot1D[i][j]->Fill(hist1D[i][j]->GetBinCenter(ix),hist1D[i][j]->GetBinContent(ix));
	  }
	}	
      }    
    }
  }

  
  
  TCanvas *can2D[4][2];
  TCanvas *can1D[4][2];

  for(int i=0;i<4;i++){
    for(int j=0;j<2;j++){

      setTDRStyle(0);
      stringstream namecan2D;
      namecan2D<<"can2D_"<<i<<"_"<<j;
      can2D[i][j]= new TCanvas(namecan2D.str().c_str(),namecan2D.str().c_str(), 450, 700 );
      
      if(ecalPart==0) histTot2D[i][j]->GetZaxis()->SetRangeUser(0.0,0.2);
      else  histTot2D[i][j]->GetZaxis()->SetRangeUser(0.0,1.0);
      
      histTot2D[i][j]->Draw("COLZ");

      if( ecalPart==0) MEEBDisplay::drawEBGlobal();
      if( ecalPart==1) MEEEDisplay::drawEEGlobal();

      stringstream namegif2D;
      namegif2D<<"STAB/Stab2D_"<<i<<"_"<<j<<".gif";
      can2D[i][j]->Print(namegif2D.str().c_str(),"gif");
      
      setTDRStyle(1);

      stringstream namecan1D;
      namecan1D<<"can1D_"<<i<<"_"<<j;
      can1D[i][j]= new TCanvas(namecan1D.str().c_str(),namecan1D.str().c_str(), 500, 500 );
      can1D[i][j]->SetLogy();
      if(ecalPart==0) histTot1D[i][j]->GetZaxis()->SetRangeUser(0.0,0.2);
      else  histTot1D[i][j]->GetZaxis()->SetRangeUser(0.0,1.0);
      
      histTot1D[i][j]->SetXTitle("RMS (\%)");
      histTot1D[i][j]->Draw();

      stringstream namegif1D;
      namegif1D<<"STAB/Stab1D_"<<i<<"_"<<j<<".gif";
      can1D[i][j]->Print(namegif1D.str().c_str(),"gif");
      
    }
  }


}


// int main(int argc, char **argv)
// {
//   int c;
//   int type=0;
//   int ecalPart=0; //EB

//   while ( (c = getopt( argc, argv, "f:e:" ) ) != EOF ) 
//     {
//       switch (c) 
// 	{
// 	case 'f': 
// 	  type       =  atoi ( optarg ) ;    
// 	  cout <<" type "<<type << endl;
// 	  break;
// 	case 'e': 
// 	  ecalPart      =  atoi ( optarg ) ;    
// 	  cout <<" ecalPart "<<ecalPart << endl;
// 	  break;

// 	}
//     }

//   // type  =   0,  1,    2.
//   // magnet= off, on, both. 
  
//   TFile *f;
//   stringstream fname;

//   string typeName;
 
//   if(type==0) typeName="_MagnetOff";
//   if(type==1) typeName="_MagnetOn";


//   string partName;
//   if(ecalPart==0) partName="EB";
//   if(ecalPart==1) partName="EE";

//   string outName;
 
//   if(ecalPart==0) outName="EBStab";
//   if(ecalPart==1) outName="EEStab";


//   setTDRStyle(0);

//   TH2D *stabCor;
//   TH2D *stab;
//   TH2D *stabBestCor;
//   TH2D *stabBest;

//   TH1D *stabCor1D;
//   TH1D *stab1D;
//   TH1D *stabBest1DCor;
//   TH1D *stabBest1D;

//   if(ecalPart==0) fname<<"/nfshome0/ecallaser/cmssw/CMSSW_3_2_0_dev/src/StabMO/Stability_APDoPN_EB_CRAFT09"<<typeName<<".root";
//   else fname<<"/nfshome0/ecallaser/cmssw/CMSSW_3_2_0_dev/src/StabMO/Stability_APDoPN_EE_CRAFT09"<<typeName<<".root";
//   cout<<fname.str()<< endl;
  
//   f=new TFile(fname.str().c_str());
  
//   stabBest=(TH2D*) f->Get("stabBest2D");
//   stab=(TH2D*) f->Get("stab2D");
//   stabBestCor=(TH2D*) f->Get("stabBest2DCor");
//   stabCor=(TH2D*) f->Get("stab2DCor");
  
//   stabBest1D=(TH1D*) f->Get("stabBest1D");
//   stab1D=(TH1D*) f->Get("stab1D");
//   stabBest1DCor=(TH1D*) f->Get("stabBest1DCor");
//   stabCor1D=(TH1D*) f->Get("stab1DCor");
  
  
//   TCanvas *can2d = new TCanvas("can2d","can2d", 450, 700 );
//   if(ecalPart==0) stabBestCor->GetZaxis()->SetRangeUser(0.0,0.5);
//   else stabBestCor->GetZaxis()->SetRangeUser(0.0,1.0);
//   stabBestCor->Draw("COLZ");
//   if( ecalPart==0) MEEBDisplay::drawEBGlobal();
//   if( ecalPart==1) MEEEDisplay::drawEEGlobal();
//   stringstream namePlot;
//   stringstream namePlot2;
//   namePlot<<"/nfshome0/ecallaser/cmssw/CMSSW_3_2_0_dev/src/" <<outName<<"/StabBestCor"<<partName<<typeName<<".gif";
//   namePlot2<<"/nfshome0/ecallaser/cmssw/CMSSW_3_2_0_dev/src/" <<outName<<"/StabBestCor"<<partName<<typeName<<".eps";
//   can2d->Print(namePlot.str().c_str(),"gif");
//   can2d->Print(namePlot2.str().c_str(),"eps");


  
//   TCanvas *can2d2 = new TCanvas("can2d2","can2d2", 450, 700 );
//   if(ecalPart==0) stabBest->GetZaxis()->SetRangeUser(0.0,0.5);
//   else stabBest->GetZaxis()->SetRangeUser(0.0,1.0);
//   //stabBest->GetZaxis()->SetRangeUser(0.0,1.0);
//   stabBest->Draw("COLZ");
//   if( ecalPart==0) MEEBDisplay::drawEBGlobal();
//   if( ecalPart==1) MEEEDisplay::drawEEGlobal();
//   stringstream namePlot3;
//   stringstream namePlot4;
//   namePlot3<<"/nfshome0/ecallaser/cmssw/CMSSW_3_2_0_dev/src/" <<outName<<"/StabBest"<<partName<<typeName<<".gif";
//   namePlot4<<"/nfshome0/ecallaser/cmssw/CMSSW_3_2_0_dev/src/" <<outName<<"/StabBest"<<partName<<typeName<<".eps";
//   can2d2->Print(namePlot3.str().c_str(),"gif");
//   can2d2->Print(namePlot4.str().c_str(),"eps");

//   setTDRStyle(1);

//   TCanvas *can = new TCanvas("can","can");
//   //can->SetLogy();
//   stabBest1DCor->SetLineColor(2);
//   stabBest1DCor->SetLineWidth(2);
//   stabBest1D->SetLineWidth(1);
  
//   stringstream nameTit;
//   if(type==0) nameTit<<"EB stability: Magnet OFF";
//   if(type==1) nameTit<<"EB stability: Magnet ON";
//   if(type==2) nameTit<<"EB stability CRAFT09";
//   stabBest1D->SetTitle(nameTit.str().c_str());
//   stabBest1D->SetXTitle("RMS (\%)");
//   stabBest1DCor->Draw();
//   stabBest1D->Draw("same");
  
//   stringstream namePlot5;
//   stringstream namePlot6;
//   stringstream namePlot7;
//   stringstream namePlot8;
//   namePlot5<<"/nfshome0/ecallaser/cmssw/CMSSW_3_2_0_dev/src/" <<outName<<"/StabBest1D"<<partName<<typeName<<".gif";
//   namePlot6<<"/nfshome0/ecallaser/cmssw/CMSSW_3_2_0_dev/src/" <<outName<<"/StabBest1D"<<partName<<typeName<<".eps";
//   namePlot7<<"/nfshome0/ecallaser/cmssw/CMSSW_3_2_0_dev/src/" <<outName<<"/StabBest1DLog"<<partName<<typeName<<".gif";
//   namePlot8<<"/nfshome0/ecallaser/cmssw/CMSSW_3_2_0_dev/src/" <<outName<<"/StabBest1DLog"<<partName<<typeName<<".eps";

//   can->Print(namePlot5.str().c_str(),"gif");
//   can->Print(namePlot6.str().c_str(),"eps");
//   can->SetLogy();
//   can->Print(namePlot7.str().c_str(),"gif");
//   can->Print(namePlot8.str().c_str(),"eps");

// }
