#ifndef METwoVarPanel_hh
#define METwoVarPanel_hh

#include <TGFrame.h>
#include <TGButton.h>
#include <TGListBox.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>

#include <vector>
using namespace std;

class MusEcalGUI;

class METwoVarPanel {

private:

  TGTransientFrame  *fMain;

  TGVerticalFrame   *fVframe;
  TGHorizontalFrame *fHframe[2];
  TGHorizontalFrame *fHframe1;

  TGListBox*        fVarBox[2];
  TGListBox*        fZoomBox[2];

  TGHorizontalFrame*   fFitGroup;
  TGNumberEntry*        fFitBox;
  TGLabel*              fFitLabel;

  TGTextButton*     fPlotButton;
  
  TGLayoutHints* fHint1;
  TGLayoutHints* fHint2;
  TGLayoutHints* fHint3;
  TGLayoutHints* fHint4;
  TGLayoutHints* fHint5;

  MusEcalGUI* _gui;

  int _type;
  int _color;
  int _var[2];
  int _zoom[2];
  int _fitDegree;

public:

  METwoVarPanel( const TGWindow *p, MusEcalGUI* main, UInt_t w, UInt_t h );
  virtual ~METwoVarPanel();

  // slots
  void CloseWindow();
  void DoClose();
  void DoPlot();
  void DoFit(int);
  void SetVar0(UInt_t);
  void SetZoom0(UInt_t);
  void SetVar1(UInt_t);
  void SetZoom1(UInt_t);

ClassDef(METwoVarPanel,0) // METwoVarPanel -- 

};

#endif
