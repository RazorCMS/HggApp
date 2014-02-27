#include "DataFitterNew.hh"

#include "RooRealVar.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooAbsReal.h"
#include "RooFormulaVar.h"

RealVar DataFitter::doFitGetScale(TTree* data,float sigEff,RooWorkspace* ws) {

  assert(data->GetEntries() != 0);

  RooRealVar mgg("mgg","",110,140);

  RooRealVar a1("a1","",-0.3,-1.,0.);
  RooRealVar a2("a2","",-0.1,-1.,0.);
  RooRealVar f("f","",0.5,0.005,0.995);

  RooRealVar NBkg("NBkg","",1000,0,1e9);

  RooExponential e1("e1","",mgg,a1);
  RooExponential e2("e2","",mgg,a2);

  RooAddPdf add("add","",e1,e2,f);
  
  RooExtendPdf pdf("pdf","",add,NBkg);

  RooDataSet rdata("data","",data,mgg);
    

  pdf.fitTo(rdata,RooFit::Strategy(0),RooFit::Extended(kTRUE));
  RooFitResult* res = pdf.fitTo(rdata,RooFit::Strategy(2),RooFit::Extended(kTRUE),RooFit::Save(kTRUE));
  
  mgg.setRange("sig",125-2*sigEff,125+2*sigEff);

  RooAbsReal *sig_int = pdf.createIntegral(mgg,RooFit::NormSet(mgg),RooFit::Range("sig"));
  sig_int->SetName("signal_integral");


  float N_sideband = rdata.sumEntries("(mgg>110 && mgg <120) || (mgg>130 && mgg<140)");

  RealVar scale;
  scale.val = NBkg.getVal()*sig_int->getVal()/N_sideband;
  scale.error = scale.val*sqrt( 1/N_sideband + pow(sig_int->getPropagatedError(*res)/sig_int->getVal(),2) + 1/NBkg.getVal() );

  if(ws) {
    ws->import(pdf);
    ws->import(rdata);
    ws->import(*res);
    ws->import(*sig_int);
  }else{
    delete res;
    delete sig_int;
  }
  return scale;
}

void DataFitter::buildSidebandHistograms() {
  for(auto cat: catNames) {
    SidebandRegionHistograms[cat] = new TH2F("data_"+cat+"_SidebandRegion","",nXbins-1,xBins,nYbins-1,yBins);
    SidebandRegionHistogramsFineBin[cat] = new TH2F("data_"+cat+"_SidebandRegion_FineBin","",250,0,2500,100,0,1);

    SidebandRegionHistograms[cat+"_fit_Up"] = new TH2F("data_"+cat+"_SidebandRegion_fitUp","",nXbins-1,xBins,nYbins-1,yBins);

    SidebandRegionHistograms[cat+"_fit_Down"] = new TH2F("data_"+cat+"_SidebandRegion_fitDown","",nXbins-1,xBins,nYbins-1,yBins);

    SidebandRegionHistograms[cat+"_bkgShape_Up"] = new TH2F("data_"+cat+"_SidebandRegion_bkgShapeUp","",nXbins-1,xBins,nYbins-1,yBins);

    SidebandRegionHistograms[cat+"_bkgShape_Down"] = new TH2F("data_"+cat+"_SidebandRegion_bkgShapeDown","",nXbins-1,xBins,nYbins-1,yBins);


    TTree massTree("massTree","");
    float m;
    massTree.Branch("mgg",&m);
    Long64_t iEntry=-1;
    while( fChain->GetEntry(++iEntry) ) {
      TLorentzVector pho1;
      TLorentzVector pho2;
  
      pho1.SetPtEtaPhiM(pho1_pt,pho1_eta,pho1_phi,0);
      pho2.SetPtEtaPhiM(pho2_pt,pho2_eta,pho2_phi,0);
      
      float se1=pho1_sigEoE;
      float se2=pho2_sigEoE;
      
      float btag = highest_csv;

      if( getCategory(pho1,pho2,se1,se2,btag) == cat ) {
	m=mgg;
	massTree.Fill();
      }
    }
    
    RooWorkspace *ws = new RooWorkspace(cat+"_mgg_workspace","");
    scales[cat] = doFitGetScale(&massTree,sigmaEffectives[cat],ws);
    mggFitWorkspaces.push_back(ws);
  }
}

void DataFitter::Run() {
  buildSidebandHistograms();
  Long64_t iEntry=-1;
  while(fChain->GetEntry(++iEntry)) {
    weight=1; //for data
    processEntry();
    processEntrySidebands();
  }

  for(auto cat: catNames) {
    SidebandRegionHistograms[cat+"_bkgShape_Up"]->Scale( SidebandRegionHistograms[cat]->Integral()/SidebandRegionHistograms[cat+"_bkgShape_Up"]->Integral() );
    SidebandRegionHistograms[cat+"_bkgShape_Down"]->Scale( SidebandRegionHistograms[cat]->Integral()/SidebandRegionHistograms[cat+"_bkgShape_Down"]->Integral() );
  }
  outputFile->cd();
  for(auto hist: SignalRegionHistograms) hist.second->Write();
  for(auto hist: SignalRegionHistogramsFineBin) hist.second->Write();

  for(auto hist: SidebandRegionHistograms) hist.second->Write();
  for(auto hist: SidebandRegionHistogramsFineBin) hist.second->Write();

  for(auto ws: mggFitWorkspaces) ws->Write();

  outputFile->Close();

  for(auto scale: scales) std::cout << scale.first << "   " << scale.second.val << " +- " << scale.second.error << std::endl;
}

void DataFitter::processEntrySidebands() {
    TLorentzVector pho1;
    TLorentzVector pho2;
  
    pho1.SetPtEtaPhiM(pho1_pt,pho1_eta,pho1_phi,0);
    pho2.SetPtEtaPhiM(pho2_pt,pho2_eta,pho2_phi,0);

    float se1=pho1_sigEoE;
    float se2=pho2_sigEoE;
    
    float btag = highest_csv;
    TString cat = getCategory(pho1,pho2,se1,se2,btag);
    float sigRegWidth = 2*sigmaEffectives[cat];

    if((mgg>110 && mgg<120) || (mgg>130 && mgg<140)){
      SidebandRegionHistograms[cat]->Fill(MR,Rsq,scales[cat].val);
      SidebandRegionHistograms[cat+"_fit_Up"]->Fill(MR,Rsq,scales[cat].val+scales[cat].error);
      SidebandRegionHistograms[cat+"_fit_Down"]->Fill(MR,Rsq,scales[cat].val-scales[cat].error);
      SidebandRegionHistogramsFineBin[cat]->Fill(MR,Rsq,scales[cat].val);
    }
    if(mgg > 130 && mgg < 140) {
      SidebandRegionHistograms[cat+"_bkgShape_Up"]->Fill(MR,Rsq,scales[cat].val);
    }
    if(mgg > 110 && mgg < 120) {
      SidebandRegionHistograms[cat+"_bkgShape_Down"]->Fill(MR,Rsq,scales[cat].val);
    }
}

float DataFitter::getSysErrPho(float eta, float r9) {
  TString region = "EBLow";
  TString r9s = (r9 > 0.94 ? "highR9" : "lowR9");
  
  if(fabs(eta) > 1.0)  region = "EBHigh";
  if(fabs(eta) > 1.48) region = "EELow";
  if(fabs(eta) > 2.0)  region = "EEHigh";

  return scaleSys[ Form("%s_%s",region.Data(),r9s.Data()) ]; 
}


