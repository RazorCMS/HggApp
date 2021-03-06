#ifndef SigRegionBinning_h
#define SigRegionBinning_h
#include <vector>
#include <algorithm>
#include <iostream>
#include "TRandom3.h"

#include "TH1D.h"
#include "TH2F.h"
#include "TAxis.h"
#include "TMath.h"

#include "TCanvas.h"
#include "TLine.h"

#include "assert.h"

namespace SigRegionBinning {
  enum BinningRegion {kHighPt, kHbb, kZbb, kHighRes, kLowRes};

  const int nBoxes=5;

  BinningRegion br(unsigned int reg) { assert(reg<5); return (BinningRegion)reg; }

  TString getRegionName(BinningRegion reg) {
    switch(reg) {
    case kHighPt:
      return "HighPt";
      
    case kHbb:
      return "Hbb";

    case kZbb:
      return "Zbb";

    case kHighRes:
      return "HighRes";

    case kLowRes:
      return "LowRes";
    }
    return "";
  }

  struct region {
    float MR_min;
    float MR_max;
    float Rsq_min;
    float Rsq_max;
    region(float minMr,float maxMr, float minRsq, float maxRsq) {
      MR_min=minMr; MR_max=maxMr; Rsq_min=minRsq; Rsq_max=maxRsq;
    }
  };
  
  //build the full list of regions from the MR edge list
  void buildRegionsFromEdges(std::vector<float>& MRedges, std::vector<region>& regions, float RsqStep=0.05) {
    std::sort(MRedges.begin(),MRedges.end()); //sort low to high
    for(int iMRbin=0;iMRbin<MRedges.size()-1;iMRbin++) {
      for(int iRsqBin=0;iRsqBin<MRedges.size()-1-iMRbin; iRsqBin++) {
	if(iRsqBin<MRedges.size()-iMRbin-2) { //not at the end in Rsq
	  regions.push_back(region(MRedges.at(iMRbin),MRedges.at(iMRbin+1),iRsqBin*RsqStep,(iRsqBin+1)*RsqStep));
	}
	else {
	  regions.push_back(region(MRedges.at(iMRbin),MRedges.at(iMRbin+1),iRsqBin*RsqStep,1.));	
	}
      }
    }
  }
  
  void printRegions(const std::vector<region>& regions) {
    std::vector<region>::const_iterator it = regions.begin();
    for(; it!=regions.end(); it++) {
      std::cout << it->MR_min << " - " << it->MR_max << "    " << it->Rsq_min << " - " << it->Rsq_max << std::endl;
    }
  }

  std::vector<float> getMREdges(BinningRegion r) {
    std::vector<float> edges;
    switch(r) {
    case kHighPt:
      edges.push_back(150);
      edges.push_back(200);
      edges.push_back(300);
      edges.push_back(500);
      edges.push_back(1600);
      edges.push_back(3000);
      break;
      
    case kHbb:
      edges.push_back(150);
      edges.push_back(300);
      edges.push_back(3000);
      
      break;
      
    case kZbb:
      edges.push_back(150);
      edges.push_back(450);
      edges.push_back(3000);
      break;
      
    case kHighRes:
      edges.push_back(150);
      edges.push_back(250);
      edges.push_back(400);
      edges.push_back(1400);
      edges.push_back(3000);
      break;
      
    case kLowRes:
      edges.push_back(150);
      edges.push_back(200);
      edges.push_back(250);
      edges.push_back(400);
      edges.push_back(1200);
      edges.push_back(3000);
      break;
    }
    return edges;
  }
  
  std::vector<region> getSigRegions(BinningRegion r) {
    std::vector<region> reg;
    std::vector<float> mrEdges = getMREdges(r);
    buildRegionsFromEdges( mrEdges , reg);
    
    //printRegions(reg);
    
    return reg;
  }


  //make an empty histogram named <name> for the given region
  TH2F* makeHistogram(BinningRegion r, TString name) {
    std::vector<float> vecMRedges = getMREdges(r);    
    float MRedges[10];
    float Rsqedges[10];

    std::copy(vecMRedges.begin(),vecMRedges.end(),MRedges);
    for(int i=0; i<=vecMRedges.size()-2; i++) {
      Rsqedges[i] = 0.05*i;
    }
    Rsqedges[vecMRedges.size()-1]=1.0;

    for(int i=0; i<vecMRedges.size(); i++) {
      std::cout << Rsqedges[i] << std::endl;
    }
    TH2F* hist = new TH2F(name,"",vecMRedges.size()-1,MRedges,vecMRedges.size()-1,Rsqedges);
    hist->SetXTitle("M_{R} (GeV)");
    hist->SetYTitle("R^{2}");

    return hist;
  }

  //add other (which can have finer binning) to sigRegHist
  //note, the binning of other should be consistent (callers responsibility, no checking done)
  void addToSignalRegionHistogram(TH2F* sigRegHist, TH2F* other) {
    for(int iXbin=1; iXbin<=other->GetNbinsX(); iXbin++) {
      float x = other->GetXaxis()->GetBinCenter(iXbin);
      for(int iYbin=1; iYbin<=other->GetNbinsY(); iYbin++) {
	float y = other->GetYaxis()->GetBinCenter(iYbin);
	sigRegHist->Fill(x,y,other->GetBinContent(iXbin,iYbin));
      }
    }
  }

  //make a signal region histogram and copy
  TH2F* makeHistogram(BinningRegion r, TString name, TH2F* origHist) {
    TH2F* sigRegHist = makeHistogram(r,name);
    addToSignalRegionHistogram(sigRegHist,origHist);
    return sigRegHist;
  }

  //integrates R2 bins to make the signal region plots correct
  void formatSigRegionPlot(TH2F* hist) {
    const int nX = hist->GetNbinsX();
    const int nY = hist->GetNbinsY();

    for(int iXbin=1; iXbin<=nX; iXbin++) {
      float topContents=0;
      for(int iYbin=nY-(iXbin-1); iYbin<=nY; iYbin++) {
	topContents += hist->GetBinContent(iXbin,iYbin);
      }
      for(int iYbin=nY-(iXbin-1); iYbin<=nY; iYbin++) {
	hist->SetBinContent(iXbin,iYbin,topContents);
      }
    }
  }

  //make a 1D projection of the 2D histogram (useful for comparing things)
  TH1D* make1DProj(TH2F* hist,bool addError=false, float sf=1) {
    const int nX = hist->GetNbinsX();
    const int nY = hist->GetNbinsY();

    const int n1D = nX*(nX+1)/2;

    TH1D* h1D = new TH1D(TString(hist->GetName())+"_1D","",n1D,-0.5,n1D-0.5);
    TAxis* x = h1D->GetXaxis();
    h1D->SetYTitle("Events / Bin");

    int iBin=0;
    for(int iXbin=1; iXbin<=nX; iXbin++) {
      for(int iYbin=1; iYbin<nY-(iXbin-1)+1; iYbin++) {
	float content = hist->GetBinContent(iXbin,iYbin);
	if(iYbin==nY-(iXbin-1)) {
	  for(int iYIntBin = nY-(iXbin-1)+1; iYIntBin<=nY;iYIntBin++) {
	    content+=hist->GetBinContent(iXbin,iYIntBin);
	    std::cout << content << std::endl;
	  }
	}
	h1D->SetBinContent(++iBin,content);
	if(addError) {
	  h1D->SetBinError(iBin,TMath::Sqrt(content*(1+sf)));
	}
	x->SetBinLabel(iBin,Form("%0.2f #leq R^{2} < %0.2f  %0.0f #leq M_{R} < %0.0f",
				 hist->GetYaxis()->GetBinLowEdge(iYbin),
				 (iYbin==nY-(iXbin-1)?1.00: hist->GetYaxis()->GetBinLowEdge(iYbin+1)),
				 hist->GetXaxis()->GetBinLowEdge(iXbin),
				 hist->GetXaxis()->GetBinLowEdge(iXbin+1)
				 ));	

				 
      }
    }
    x->SetLabelSize(0.03);
    return h1D;
  }

  float getExpPer(TRandom3& rng, std::vector<float> expErr) {
    float expPer=0;
    for(std::vector<float>::const_iterator expIt=expErr.begin();
	expIt != expErr.end(); expIt++) {
      expPer+=rng.Gaus(0,*expIt);
    }
    return expPer;
  }

  //expErr should be a list of % errors for the expected background
  float pValue(float obs, float exp, std::vector<float> expErr) {
    //if(obs==0 && exp<0.50) return 1;
    TRandom3 rng;

    size_t count=0;
    float diff = fabs(obs-exp);

    size_t nToy=100;
    while(count<100) {
      nToy*=10;
      count=0;
      for(int i=0;i<nToy;i++) {
	float thisExpMean = exp*getExpPer(rng,expErr);
	float thisExp = rng.Poisson(thisExpMean);
	if( fabs(thisExp-obs) >= diff ) count++;
      }
    }
    return float(count)/nToy;
  }

  float pValue(float obs, float exp, float expErr) {
    std::vector<float> vecExpErr(1,expErr);
    return pValue(obs,exp,vecExpErr);
  }

  //combine uses a different order of bins, so we convert here.
  std::pair<BinningRegion,int> ConvertCombineBin(int cbin) {
    int combineOrder15[] = {15,13,14,10,11,12,6,7,8,9,1,2,3,4,5};
    int combineOrder3[] = {3,1,2};
    int combineOrder10[] = {10,8,9,5,6,7,1,2,3,4};
    if(cbin < 16) { //HighPt
      return std::make_pair(kHighPt,combineOrder15[cbin]);
    }
    if(cbin<19) {
      return std::make_pair(kHbb,combineOrder3[cbin-16]);
    }
    if(cbin<22) {
      return std::make_pair(kZbb,combineOrder3[cbin-19]);
    }
    if(cbin<32) {
      return std::make_pair(kHighRes,combineOrder10[cbin-22]);
    }
    return std::make_pair(kLowRes,combineOrder15[cbin-32]);

  }

  //convert a region and bin index into combine binning
  int ConvertToCombineBin(BinningRegion reg, int bin) {
    int combineOrder15[] = {10,11,12,13,14,6,7,8,9,3,4,5,1,2,0};
    int combineOrder3[] = {1,2,0};
    int combineOrder10[] = {6,7,8,9,3,4,5,1,2,0};
    switch(reg) {
    case kHighPt:
      return combineOrder15[bin];
    case kHbb:
      return combineOrder3[bin]+15;
    case kZbb:
      return combineOrder3[bin]+18;
    case kHighRes:
      return combineOrder10[bin]+21;
    case kLowRes:
      return combineOrder15[bin]+31;
    }
    return -1;
  }

  //draw the lines for the signal regions on a plot
  std::vector<TLine*> drawSigRegionLines(TCanvas* cv, BinningRegion reg) {
    std::vector<float> MR_edges = getMREdges(reg);
    std::vector<TLine*> lines;

    float MR_min = MR_edges.at(0);
    //make horizontal lines
    for(int i=0; i<MR_edges.size()-1; i++) {
      lines.push_back(new TLine(MR_min,0.05*(i+1),MR_edges.at(MR_edges.size()-i-2),0.05*(i+1)));
    }
    
    //make vertical lines
    for(int i=0;i<MR_edges.size(); i++) {
      lines.push_back(new TLine(MR_edges.at(i),0,MR_edges.at(i),1));
    }
    cv->cd();
    for(std::vector<TLine*>::iterator lIt=lines.begin(); lIt!=lines.end(); lIt++) {
      (*lIt)->SetLineWidth(2);
      (*lIt)->Draw();
    }
    return lines;
  }

};

#endif
