#include <iostream>
using namespace std;

#include "METwoVarPanel.hh"

#include "MusEcalGUI.hh"
#include <TSystem.h>

ClassImp(METwoVarPanel)

METwoVarPanel::METwoVarPanel( const TGWindow *p, MusEcalGUI* main, 
			  UInt_t w, UInt_t h)
  :  _gui( main )
{

  _type = _gui->_type;
  _color = _gui->_color;
  
  fMain=0;
  fVframe=0;
  fHframe[0]=0;
  fHframe[1]=0;
  fHframe1=0;
  fVarBox[0]=0;
  fVarBox[1]=0;
  fFitBox=0;
  fPlotButton=0;

  fHint1 = new TGLayoutHints( kLHintsTop | kLHintsCenterX, 5, 5, 5, 5 );
  fHint3 = new TGLayoutHints( kLHintsTop | kLHintsLeft , 3 ,3 ,3 ,3 );
  fHint2 = new TGLayoutHints( kLHintsTop | kLHintsLeft | kLHintsExpandX, 0, 0, 3, 0);
  fHint4 = new TGLayoutHints( kLHintsTop | kLHintsLeft | kLHintsExpandX,
			      2, 2, 0, 0);
  fHint5 =  new TGLayoutHints( kLHintsCenterX, 0, 0, 0, 0 );

  fMain = new TGTransientFrame(p, main, w, h);
  fMain->Connect( "CloseWindow()", "METwoVarPanel", this, "DoClose()" );
  fMain->DontCallClose();
  fMain->SetCleanup(kDeepCleanup);

  fMain->ChangeOptions( (fMain->GetOptions() & ~kVerticalFrame) | kHorizontalFrame );

  fVframe = new TGVerticalFrame( fMain, 0, 0, 0 );
  for( UInt_t kk=0;  kk<2; kk++ )
    {
      fHframe[kk] = new TGHorizontalFrame( fVframe, 0, 0, 0 );
      fVarBox[kk] = new TGListBox( fHframe[kk], 150, 100 );
      TString str;
      str = "SetVar"; str+=kk; str+="(UInt_t)";
      fVarBox[kk]->Connect( "Selected(Int_t)", "METwoVarPanel", this, str );
      fZoomBox[kk] = new TGListBox( fHframe[kk], 150, 100 );
      str = "SetZoom"; str+=kk; str+="(UInt_t)";
      fZoomBox[kk]->Connect( "Selected(Int_t)", "METwoVarPanel", this, str );
      if( ( _type==ME::iLaser ) && ( _color==ME::iBlue || _color==ME::iRed ) )
	{
	  if( kk==0 )
	    _var[kk] =  MusEcal::iShapeCorAPD;
	  else
	    _var[kk] = MusEcal::iAPDoPNA;
	  for( int jj=0; jj<MusEcal::iSizeLV; jj++ )
	    {
	      fVarBox[kk]->AddEntry( MusEcal::historyVarName[jj], jj );
	    }  
	  fVarBox[kk]->Resize(150,100);
	  fVarBox[kk]->Select( _var[kk] );
	  fVarBox[kk]->Layout();

	  _zoom[kk] = MusEcal::historyVarZoom[_color][ _var[kk] ];
	  if( _zoom[kk]==MusEcal::iZero ) _zoom[kk] = MusEcal::iThirtyPercent;
	  for( int iZoom=0; iZoom<MusEcal::iZero; iZoom++ )
	    {
	      fZoomBox[kk]->AddEntry( MusEcal::zoomName[iZoom], iZoom );
	    }
	  fZoomBox[kk]->Resize(150,100);
	  fZoomBox[kk]->Select( _zoom[kk] );
	  fZoomBox[kk]->Layout();
	}
      fHframe[kk]->AddFrame( fVarBox[kk],  fHint3 );
      fHframe[kk]->AddFrame( fZoomBox[kk], fHint3 );
      fHframe[kk]->Resize(310,110);
      fHframe[kk]->MapSubwindows();  
      fVframe->AddFrame( fHframe[kk], fHint5 );
    }

  fHframe1 = new TGHorizontalFrame( fVframe, 0, 0, 0 );

//   fFitGroup = new TGHorizontalFrame( fHframe1, 10, 5 );
//   fHframe1->AddFrame( fFitGroup, fHint3 );
  fFitBox      = new TGNumberEntry( fHframe1, 1, 2, -1,
 				    TGNumberFormat::kNESInteger, 
 				    TGNumberFormat::kNEANonNegative,
 				    TGNumberFormat::kNELLimitMinMax , 0, 5 );
//   fFitGroup->AddFrame(fFitBox, fHint3 );
//   fFitLabel = new TGLabel( fFitGroup, "Fit Degree" );
//   fFitGroup->AddFrame( fFitLabel, fHint3 );
   fHframe1->AddFrame( fFitBox,  fHint3 );

   fPlotButton = new TGTextButton( fHframe1      , "   Plot      " );
   fPlotButton->Connect("Clicked()","METwoVarPanel",this,"DoPlot()");
   fHframe1->AddFrame( fPlotButton,  fHint3 );

//   fHframe1->Resize(310,50);
   fHframe1->MapSubwindows();  
   fVframe->AddFrame( fHframe1, fHint5 );

   fVframe->MapSubwindows();  

  fMain->AddFrame( fVframe,    fHint5 );

  TString str;
  str += ME::type[_type];
  if(_type==ME::iLaser) str += ME::color[_color];
  str += "  Two-Variable Panel";
  fMain->SetWindowName(str);
  TGDimension size = fMain->GetDefaultSize();
  fMain->Resize(size);

  fMain->MapSubwindows();
  fMain->MapWindow();

}

METwoVarPanel::~METwoVarPanel()
{
  _gui->_fTwoVarPanel=0;

  fMain->DeleteWindow();   // deletes fMain
}

void 
METwoVarPanel::CloseWindow()
{
  // Called when window is closed via the window manager.

  delete this;
}

void 
METwoVarPanel::DoClose()
{
  CloseWindow();
}

void
METwoVarPanel::DoPlot()
{
  //  cout << "METwoVarPanel -- Plot " << endl;
  //   for( UInt_t kk=0; kk<2; kk++ )
  //     {
  //       cout << "for Var["  << kk << "]=" << _var[kk] 
  // 	   << " and zoom " << _zoom[kk] << endl;
  //     }
  _fitDegree = (int)fFitBox->GetNumber();
  //cout << " ... and fit with a polynome of degree " << _fitDegree << endl;
  _gui->correlation2Var( _var[0], _var[1], _zoom[0], _zoom[1], _fitDegree , 1);
}

void
METwoVarPanel::DoFit( int ifit )
{
  _fitDegree = ifit;
}

void
METwoVarPanel::SetVar0( UInt_t ii )
{
  _var[0] = ii;
  //  cout << "Set Var[0]=" << ii << endl;
}

void
METwoVarPanel::SetVar1( UInt_t ii )
{
  _var[1] = ii;
  //  cout << "Set Var[1]=" << ii << endl;
}

void
METwoVarPanel::SetZoom0( UInt_t ii )
{
  _zoom[0] = ii;
  //  cout << "Set Zoom[0]=" << ii << endl;
}

void
METwoVarPanel::SetZoom1( UInt_t ii )
{
  _zoom[1] = ii;
  //  cout << "Set Zoom[1]=" << ii << endl;
}

