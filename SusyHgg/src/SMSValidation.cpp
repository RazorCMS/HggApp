#include "SMSValidation.hh"

#include "TLorentzVector.h"
#include "Utils.cpp"


SMSValidation::SMSValidation(TString inputFileName, TString outputFileName):
  SusyHggTree(inputFileName)
{
  outputFile = new TFile(outputFileName,"RECREATE");
}

SMSValidation::~SMSValidation() {
  outputFile->cd();
  for(auto plot: plots) plot.second->Write();
  outputFile->Close();
}

void SMSValidation::Run() {
  setupPlots();

  Long64_t iEntry=-1;
  while(fChain->GetEntry(++iEntry)) {
    processEntry();
  }
}

void SMSValidation::setupPlots(){

  for(auto cat: *catNames) {
    plots[cat+"_mgg_noscale"] = new TH1D(cat+"_mgg_noscale","",3000,110,140);
    plots[cat+"_mgg_scale"] = new TH1D(cat+"_mgg_scale","",3000,110,140);
  }
}

void SMSValidation::processEntry() {
  if(!pho1_pass_iso || !pho2_pass_iso) return;

  TLorentzVector p1,p2;
  p1.SetPtEtaPhiM(pho1_pt,pho1_eta,pho1_phi,0);
  p2.SetPtEtaPhiM(pho2_pt,pho2_eta,pho2_phi,0);

  TString cat = Fitter::getCategory(p1,p2,pho1_sigEoE,pho2_sigEoE,highest_csv);

  float sf1 = getFastSIMScaleFactor(p1.Eta(),p1.Eta()).first;
  float sf2 = getFastSIMScaleFactor(p2.Eta(),p2.Eta()).first;

  plots[cat+"_mgg_noscale"]->Fill(mgg,pileupWeight);
  plots[cat+"_mgg_scale"]->Fill(mgg,pileupWeight*sf1*sf2);
}