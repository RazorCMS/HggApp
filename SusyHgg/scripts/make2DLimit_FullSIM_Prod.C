#include "TString.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TAxis.h"

#include "getTheoXSec.C"


void make2DLimitWH(TString combine_dir,TString type="WH",bool blind=true) {

  //vecbosStyle->SetPadRightMargin(0.17);

  const int n=5;
  float x[n] = {120,140,162.5,187.5,212.5};
  float y[n] = {0,12.5,37.5,62.5,87.5};

  //TH2F limit("limit","",4,112.5,212.5,4,-12.5,87.5);
  //TH2F explim("exp","",4,112.5,212.5,4,-12.5,87.5);
  TH2F limit("limit","",n-1,x,n-1,y);
  TH2F explim("exp","",n-1,x,n-1,y);

  TGraphErrors* theo = getTheoXSec("/home/amott/HggApp/SusyHgg/xsecs/CharginoNeutralino.txt");

  for(int m1=125;m1<201;m1+=25) {
    for(int m2=0;m2<m1-124;m2+=25) {
      //int i=(m-125)/25;
      TFile limit_file(Form("%s/higgsCombineChi%s_%d_%d.Asymptotic.mH120.root",combine_dir.Data(),type.Data(),m2,m1) );
      TTree *limit_tree = (TTree*)limit_file.Get("limit");
      TTreeFormula limit_form("get_limit","limit",limit_tree);
      
      limit_tree->GetEntry(1);
      float down = limit_form.EvalInstance();
      limit_tree->GetEntry(2);
      float exp = limit_form.EvalInstance();
      limit_tree->GetEntry(3);
      float up = limit_form.EvalInstance();
      limit_tree->GetEntry(5);
      float obs = limit_form.EvalInstance();
      
      limit.Fill(m1,m2,exp);
      explim.Fill(m1,m2,(exp/theo->Eval(m1)<1 ? 1:0)  );
    }
  }
  TCanvas cv;
  cv.SetLogz();
    
  limit.SetZTitle("95% CL Upper Limit (pb)");
  limit.SetXTitle("m_{chargino} [GeV]");
  limit.SetYTitle("m_{LSP} [GeV]");
  
  limit.SetAxisRange(1,10,"Z");

  limit.Draw("COLZ");
  
  //explim.Draw("CONT3SAME");

  TLatex prelim(0.12,0.96,"CMS Preliminary");
  prelim.SetNDC();
  prelim.SetTextSize(0.045);
  prelim.Draw();
    
  TLatex lbl(0.5,0.96,"#sqrt{s} = 8 TeV  #int L dt = 19.78 fb^{-1}");
  lbl.SetNDC();
  lbl.SetTextSize(0.035);
  lbl.Draw();

  TString infix = (blind ? "" : "_OBS");
  cv.SaveAs(Form("%s/expected_exclusion_%s_2D%s.png",combine_dir.Data(),type.Data(),infix.Data()));
  cv.SaveAs(Form("%s/expected_exclusion_%s_2D%s.pdf",combine_dir.Data(),type.Data(),infix.Data()));

}
