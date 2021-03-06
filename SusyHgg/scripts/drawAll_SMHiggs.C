#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TList.h"
#include "TLatex.h"

#include <map>
#include <vector>
#include <iostream>

void drawAll_SMHiggs(TString folder) {
  const int nProc = 4;
  TString processes[nProc] = {"ggH","vbfH","wzH","ttH"};

  std::map<TString,TH1*> hists;
  std::vector<TFile*> files;

  

  for(int i=0;i<nProc;i++) {
    files.push_back(new TFile(folder+"/"+processes[i]+".root") );

    TList *l = files.back()->GetListOfKeys();

    for(int iHist=0; iHist < l->GetEntries(); iHist++) {
      if(i==0) {
	hists[l->At(iHist)->GetName()] = (TH1*)files.back()->Get(l->At(iHist)->GetName());
      }else{
	hists[l->At(iHist)->GetName()]->Add((TH1*)files.back()->Get(l->At(iHist)->GetName()));
      }
    }
  }

  TLatex prelim(0.12,0.96,"CMS Preliminary Simulation");
  prelim.SetNDC();
  prelim.SetTextSize(0.045);
  prelim.SetTextColor(kBlack);

  TLatex lumi(0.6,0.96,"#sqrt{s} = 8 TeV  L = 19.8 fb^{-1}");
  lumi.SetNDC();
  lumi.SetTextSize(0.045);
  lumi.SetTextColor(kBlack);

  

  TCanvas cv;
  cv.SetLogz();
  for(auto h: hists) {
    if( TString(h.second->GetName()).Contains("Up")
	|| TString(h.second->GetName()).Contains("Down") ) continue;
    if(TString(h.second->GetName()).Contains("mgg_dist")){
      std::cout << "TH1D" << std::endl;
      h.second->Rebin(50);

      h.second->SetXTitle("m_{#gamma#gamma} [GeV]");
      h.second->SetYTitle(Form("Events / %0.2f",h.second->GetBinWidth(2)));
      h.second->SetMarkerStyle(11);
      h.second->SetMarkerSize(1.2);
      h.second->Draw("P");
    }else{
      h.second->SetXTitle("M_{R} [GeV]");
      h.second->SetYTitle("R^{2}");
      h.second->Draw("COLZ");
    }

    prelim.Draw("SAME");
    lumi.Draw("SAME");
    cv.SaveAs(folder+"/figs/"+h.first+"_SMTot.png");
    cv.SaveAs(folder+"/figs/"+h.first+"_SMTot.pdf");
  }

}

