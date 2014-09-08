#include "FitterNew.hpp"

#include "TObjArray.h"
#include "TMath.h"

#include <iostream>

#define NUM_CPU 1

const std::vector<TString> Fitter::catNames = { "HighPt","Hbb","Zbb","HighRes","LowRes"};
const std::vector<TString> Fitter::systematicNames = {"jec","phoE","btag","sigE"}; 
//const std::vector<TString> Fitter::catNames = { "Hbb"};
//const std::vector<TString> Fitter::systematicNames = {}; 


Fitter::Fitter(TString inputFileName,TString outputFileName,bool useHT):SusyHggTree(inputFileName) {
  outputFile = new TFile(outputFileName,"RECREATE");
  setUseHT(useHT);
  buildHistograms();  

  for(auto cat: catNames)   setSigEff(cat,3);

}

Fitter::~Fitter() {
  outputFile->Close();
}

void Fitter::buildHistograms() {
  for(auto cat: catNames) {
    if(useHT) {
      SignalRegionHistograms[cat] = new TH2F("data_"+cat+"_SignalRegion","",50,0,2500,12,0,300);
      SignalRegionHistogramsFineBin[cat] = new TH2F("data_"+cat+"_SignalRegion_FineBin","",500,0,2500,100,0,300);
      for(auto dir: systematicDir) {
	for(auto sys: systematicNames) {
	  SignalRegionHistograms[cat+"_"+sys+"_"+dir] = new TH2F("data_"+cat+"_SignalRegion_"+sys+"_"+dir,"",50,0,2500,12,0,300);
	}
      }
    } else {
      SignalRegionHistograms[cat] = new TH2F("data_"+cat+"_SignalRegion","",nXbins-1,xBins,nYbins-1,yBins);
      SignalRegionHistogramsFineBin[cat] = new TH2F("data_"+cat+"_SignalRegion_FineBin","",250,0,2500,100,0,1);
      for(auto dir: systematicDir) {
	for(auto sys: systematicNames) {
	  SignalRegionHistograms[cat+"_"+sys+"_"+dir] = new TH2F("data_"+cat+"_SignalRegion_"+sys+"_"+dir,"",nXbins-1,xBins,nYbins-1,yBins);
	}
      }
    }
    mgg_dists[cat] = new TH1D(cat+"_mgg_dist","",3000,minMgg,maxMgg);
  }
}

bool Fitter::passBasicSelection() {
  if(!pho1_pass_iso || !pho2_pass_iso) return false;

  //if(ele1_pt>15 || mu1_pt>15) return false;

  switch(basicSelection) {
  case kAN239:
    //AN13/239-like selection
    if(pho1_pt<40 || pho2_pt < 25) return false;
    if( fabs(pho1_eta)>1.442 || fabs(pho2_eta)>1.442) return false;
    break;

  case kHighPt:
    if(pho1_pt<40 || pho2_pt < 25) return false;
    break;

  case kLoose:
    if(pho1_pt<32 || pho2_pt < 24) return false;
    break;

  default:
    std::cout << "invalid basic selection (or selection not implemented)" << std::endl;
    assert(false);

  }
  return true;
}

TString Fitter::getCategory(const TLorentzVector& pho1, const TLorentzVector&pho2,float se1, float se2,float btag,float mbbH,float mbbZ,float r9_1,float r9_2) {
  float ptgg = (pho1+pho2).Pt();

  //return catNames[0];

  if(ptgg > 110) return catNames[0];

  if( fabs(mbbH-125.) <15. ) return catNames[1];
  if( fabs(mbbZ-91.) <15. ) return catNames[2];

  //if(btag > 0.244) return catNames[1];

  //if(r9_1<0.94 || r9_2<0.94) return catNames[4];

  
  if(  se1 > ( fabs(pho1.Eta())>1.48 ? 0.024 : 0.015 ) ) return catNames[4]; //pho1 fails se/e cut
  if(  se2 > ( fabs(pho2.Eta())>1.48 ? 0.024 : 0.015 ) ) return catNames[4]; //pho1 fails se/e cut
  

  return catNames[3]; //high res category

}

void Fitter::processEntry() {
  { //nominal 
    TLorentzVector pho1;
    TLorentzVector pho2;
  
    pho1.SetPtEtaPhiM(pho1_pt,pho1_eta,pho1_phi,0);
    pho2.SetPtEtaPhiM(pho2_pt,pho2_eta,pho2_phi,0);

    float se1=pho1_sigEoE;
    float se2=pho2_sigEoE;
    
    float btag = highest_csv;
    float mbbH = mbb_NearH;
    float mbbZ = mbb_NearZ;
    TString cat = getCategory(pho1,pho2,se1,se2,btag,mbbH,mbbZ,pho1_r9,pho2_r9);    
    float sigRegWidth = nSigEffSignalRegion*sigmaEffectives[cat];

    float thisMR = MR;
    float thisRsq = Rsq;
    if(useHT) {
      thisMR = HT;
      thisRsq = MET;
    }

    if(mgg > 125- sigRegWidth && mgg < 126 + sigRegWidth) {
      SignalRegionHistograms[cat]->Fill(thisMR,thisRsq,weight);
      SignalRegionHistogramsFineBin[cat]->Fill(thisMR,thisRsq,weight);
    }

    
    mgg_dists[cat]->Fill(mgg,weight);
  }

  for(auto dir: systematicDir) {
    for(auto sys: systematicNames) {
      TLorentzVector pho1;
      TLorentzVector pho2;
      
      pho1.SetPtEtaPhiM(pho1_pt,pho1_eta,pho1_phi,0);
      pho2.SetPtEtaPhiM(pho2_pt,pho2_eta,pho2_phi,0);
      
      float se1=pho1_sigEoE;
      float se2=pho2_sigEoE;
      
      float btag = highest_csv;
      float mbbH = mbb_NearH;
      float mbbZ = mbb_NearZ;
 
      float thisMR = MR;
      float thisRsq = Rsq;
      if(useHT) {
	thisMR = HT;
	thisRsq = MET;
      }
     
      float mass = mgg;

      if(sys == "phoE") { //photon energy systematic
	float err1 = getSysErrPho(pho1_eta,pho1_r9);
	float err2 = getSysErrPho(pho2_eta,pho2_r9);
	assert(err1 > 0. && err1 < 0.1);
	assert(err2 > 0. && err2 < 0.1);
	if(dir=="Up") {
	  pho1.SetPtEtaPhiM( pho1.Pt()*(1+err1), pho1.Eta(),pho1.Phi(),0);
	  pho2.SetPtEtaPhiM( pho2.Pt()*(1+err2), pho2.Eta(),pho2.Phi(),0);
	  se1*=1/(1+err1);
	  se2*=1/(1+err2);
	}else{
	  pho1.SetPtEtaPhiM( pho1.Pt()*(1-err1), pho1.Eta(),pho1.Phi(),0);
	  pho2.SetPtEtaPhiM( pho2.Pt()*(1-err2), pho2.Eta(),pho2.Phi(),0);	  
	  se1*=1/(1-err1);
	  se2*=1/(1-err2);
	}

     
	mass = (pho1+pho2).M();
      }

      if(sys == "jec") { //jet energy scale changes Rsq and MR
	if(dir=="Up") {
	  thisMR = MR_up;
	  thisRsq = Rsq_up;
	  if(useHT) {
	    thisMR = HT_up;
	    thisRsq = MET;
	  }
	  btag = highest_csv_up; //on the off chance this changes the jet 
	  mbbH = mbb_NearH_up;
	  mbbZ = mbb_NearZ_up;
	}else{
	  thisMR = MR_down;
	  thisRsq = Rsq_down;
	  if(useHT) {
	    thisMR = HT_down;
	    thisRsq = MET;
	  }

	  btag = highest_csv_down; //on the off chance this changes the jet 
	  mbbH = mbb_NearH_down;
	  mbbZ = mbb_NearZ_down;
	}
      }

      if(sys=="btag") {
	float err =0;
	if(highest_csv_pt > SFb_error.rbegin()->first) err = SFb_error.rbegin()->second;
	else{
	  auto SFBe = SFb_error.begin();
	  for(;SFBe != SFb_error.end(); SFBe++) {
	    if(SFBe->first > highest_csv_pt) {
	      SFBe--;
	      err = SFBe->second;
	      break;
	    }
	  }
	} 
	if(dir=="Up") {
	  btag+=err;
	}else{
	  btag-=err;
	}
      }


      TString cat = getCategory(pho1,pho2,se1,se2,btag,mbbH,mbbZ,pho1_r9,pho2_r9);
      float sigRegWidth = nSigEffSignalRegion*sigmaEffectives[cat];

      if(mass > 125- sigRegWidth && mass < 126 + sigRegWidth) {
	SignalRegionHistograms[ cat+"_"+sys+"_"+dir ]->Fill(thisMR,thisRsq,weight);	
      }
    }
  }
}

float Fitter::getSysErrPho(float eta,float r9) {
     TString region = "EBLow";
     TString r9s = (r9 > 0.94 ? "highR9" : "lowR9");

     if(fabs(eta) > 1.0)  region = "EBHigh";
     if(fabs(eta) > 1.48) region = "EELow";
     if(fabs(eta) > 2.0)  region = "EEHigh";

     return smearSys[ Form("%s_%s",region.Data(),r9s.Data()) ];
}


void Fitter::Run() {
  Long64_t iEntry=-1;
  while(fChain->GetEntry(++iEntry)) {
    weight = pileupWeight * hggSigStrength*target_xsec*lumi*HggBR/N_total;
    if(!passBasicSelection()) continue;
    processEntry();
  }

  outputFile->cd();
  for(auto hist: SignalRegionHistograms) hist.second->Write();
  for(auto hist: SignalRegionHistogramsFineBin) hist.second->Write();
  for(auto hist: mgg_dists) hist.second->Write();
  outputFile->Close();
}

