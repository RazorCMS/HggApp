 // 2859  outfileAlternate_HighPt.txt
 // 2857  outfileStandard_HighPt.txt
 // 2852  outfileVectSumPt_HighPt.txt

 // 7878   outfileAlternate_HighRes.txt
 // 13968  outfileStandard_HighRes.txt
 // 13848  outfileVectSumPt_HighRes.txt

 // 27713  outfileAlternate_LowRes.txt
 // 17754  outfileStandard_LowRes.txt
 // 17734  outfileVectSumPt_LowRes.txt


 // Standard Vs Alternate Overlap: 
 // Alternate HighRes Efficiency is much lower than Standard analysis: 22% vs 44%
 // HighPt 99.93% overlap
 // HighRes 44%
 // LowRes 60%

 // Standard Vs VectSumPt
 // HighPt  97.4%
 // HighRes 98.3%
 // LowRes  98.8%






void checkOverlap() {

  ofstream outfileStandard_HighPt; outfileStandard_HighPt.open ("outfileStandard_HighPt.txt");
  ofstream outfileAlternate_HighPt; outfileAlternate_HighPt.open ("outfileAlternate_HighPt.txt");
  ofstream outfileVectSumPt_HighPt; outfileVectSumPt_HighPt.open ("outfileVectSumPt_HighPt.txt");
  ofstream outfileStandard_HighRes; outfileStandard_HighRes.open ("outfileStandard_HighRes.txt");
  ofstream outfileAlternate_HighRes; outfileAlternate_HighRes.open ("outfileAlternate_HighRes.txt");
  ofstream outfileVectSumPt_HighRes; outfileVectSumPt_HighRes.open ("outfileVectSumPt_HighRes.txt");
  ofstream outfileStandard_LowRes; outfileStandard_LowRes.open ("outfileStandard_LowRes.txt");
  ofstream outfileAlternate_LowRes; outfileAlternate_LowRes.open ("outfileAlternate_LowRes.txt");
  ofstream outfileVectSumPt_LowRes; outfileVectSumPt_LowRes.open ("outfileVectSumPt_LowRes.txt");


  TFile *fileStandard = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/HggRazorNtuple/DoublePhoton_Standard_ABCD_Total_GoodLumi.root");
  TFile *fileAlternate = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/HggRazorNtuple/DoublePhotonAlternate_ABCD_Total_GoodLumi.root");
  TFile *fileVectSumPt = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/HggRazorNtuple/DoublePhoton_vectSumPt_ABCD_Total_GoodLumi.root");
  TTree *treeStandard_HighPt = (TTree*)fileStandard->Get("HighPt");
  TTree *treeAlternate_HighPt = (TTree*)fileAlternate->Get("HighPt");
  TTree *treeVectSumPt_HighPt = (TTree*)fileVectSumPt->Get("HighPt");
  TTree *treeStandard_HighRes = (TTree*)fileStandard->Get("HighRes");
  TTree *treeAlternate_HighRes = (TTree*)fileAlternate->Get("HighRes");
  TTree *treeVectSumPt_HighRes = (TTree*)fileVectSumPt->Get("HighRes");
  TTree *treeStandard_LowRes = (TTree*)fileStandard->Get("LowRes");
  TTree *treeAlternate_LowRes = (TTree*)fileAlternate->Get("LowRes");
  TTree *treeVectSumPt_LowRes = (TTree*)fileVectSumPt->Get("LowRes");

  float MR = 0;
  float Rsq = 0;
  float mgg = 0;
  float pho1Pt = 0;
  float pho1Eta = 0;
  float pho1SigmaEOverE = 0;
  float pho2Pt = 0;
  float pho2Eta = 0;
  float pho2SigmaEOverE = 0;
  uint run = 0;
  uint event = 0;
  treeStandard_HighPt->SetBranchAddress("run",&run);
  treeStandard_HighPt->SetBranchAddress("event",&event);
  treeStandard_HighPt->SetBranchAddress("MR",&MR);
  treeStandard_HighPt->SetBranchAddress("t1Rsq",&Rsq);
  treeStandard_HighPt->SetBranchAddress("mGammaGamma",&mgg);
  treeStandard_HighPt->SetBranchAddress("pho1Pt",&pho1Pt);
  treeStandard_HighPt->SetBranchAddress("pho1Eta",&pho1Eta);
  treeStandard_HighPt->SetBranchAddress("pho2Pt",&pho2Pt);
  treeStandard_HighPt->SetBranchAddress("pho2Eta",&pho2Eta);
  treeAlternate_HighPt->SetBranchAddress("run",&run);
  treeAlternate_HighPt->SetBranchAddress("event",&event);
  treeAlternate_HighPt->SetBranchAddress("MR",&MR);
  treeAlternate_HighPt->SetBranchAddress("t1Rsq",&Rsq);
  treeAlternate_HighPt->SetBranchAddress("mGammaGamma",&mgg);
  treeAlternate_HighPt->SetBranchAddress("pho1Pt",&pho1Pt);
  treeAlternate_HighPt->SetBranchAddress("pho1Eta",&pho1Eta);
  treeAlternate_HighPt->SetBranchAddress("pho2Pt",&pho2Pt);
  treeAlternate_HighPt->SetBranchAddress("pho2Eta",&pho2Eta);
  treeVectSumPt_HighPt->SetBranchAddress("run",&run);
  treeVectSumPt_HighPt->SetBranchAddress("event",&event);
  treeVectSumPt_HighPt->SetBranchAddress("MR",&MR);
  treeVectSumPt_HighPt->SetBranchAddress("t1Rsq",&Rsq);
  treeVectSumPt_HighPt->SetBranchAddress("mGammaGamma",&mgg);
  treeVectSumPt_HighPt->SetBranchAddress("pho1Pt",&pho1Pt);
  treeVectSumPt_HighPt->SetBranchAddress("pho1Eta",&pho1Eta);
  treeVectSumPt_HighPt->SetBranchAddress("pho2Pt",&pho2Pt);
  treeVectSumPt_HighPt->SetBranchAddress("pho2Eta",&pho2Eta);
  treeStandard_HighRes->SetBranchAddress("run",&run);
  treeStandard_HighRes->SetBranchAddress("event",&event);
  treeStandard_HighRes->SetBranchAddress("MR",&MR);
  treeStandard_HighRes->SetBranchAddress("t1Rsq",&Rsq);
  treeStandard_HighRes->SetBranchAddress("mGammaGamma",&mgg);
  treeStandard_HighRes->SetBranchAddress("pho1Pt",&pho1Pt);
  treeStandard_HighRes->SetBranchAddress("pho1Eta",&pho1Eta);
  treeStandard_HighRes->SetBranchAddress("pho2Pt",&pho2Pt);
  treeStandard_HighRes->SetBranchAddress("pho2Eta",&pho2Eta);
  treeAlternate_HighRes->SetBranchAddress("run",&run);
  treeAlternate_HighRes->SetBranchAddress("event",&event);
  treeAlternate_HighRes->SetBranchAddress("MR",&MR);
  treeAlternate_HighRes->SetBranchAddress("t1Rsq",&Rsq);
  treeAlternate_HighRes->SetBranchAddress("mGammaGamma",&mgg);
  treeAlternate_HighRes->SetBranchAddress("pho1Pt",&pho1Pt);
  treeAlternate_HighRes->SetBranchAddress("pho1Eta",&pho1Eta);
  treeAlternate_HighRes->SetBranchAddress("pho2Pt",&pho2Pt);
  treeAlternate_HighRes->SetBranchAddress("pho2Eta",&pho2Eta);
  treeVectSumPt_HighRes->SetBranchAddress("run",&run);
  treeVectSumPt_HighRes->SetBranchAddress("event",&event);
  treeVectSumPt_HighRes->SetBranchAddress("MR",&MR);
  treeVectSumPt_HighRes->SetBranchAddress("t1Rsq",&Rsq);
  treeVectSumPt_HighRes->SetBranchAddress("mGammaGamma",&mgg);
  treeVectSumPt_HighRes->SetBranchAddress("pho1Pt",&pho1Pt);
  treeVectSumPt_HighRes->SetBranchAddress("pho1Eta",&pho1Eta);
  treeVectSumPt_HighRes->SetBranchAddress("pho2Pt",&pho2Pt);
  treeVectSumPt_HighRes->SetBranchAddress("pho2Eta",&pho2Eta);
  treeStandard_LowRes->SetBranchAddress("run",&run);
  treeStandard_LowRes->SetBranchAddress("event",&event);
  treeStandard_LowRes->SetBranchAddress("MR",&MR);
  treeStandard_LowRes->SetBranchAddress("t1Rsq",&Rsq);
  treeStandard_LowRes->SetBranchAddress("mGammaGamma",&mgg);
  treeStandard_LowRes->SetBranchAddress("pho1Pt",&pho1Pt);
  treeStandard_LowRes->SetBranchAddress("pho1Eta",&pho1Eta);
  treeStandard_LowRes->SetBranchAddress("pho2Pt",&pho2Pt);
  treeStandard_LowRes->SetBranchAddress("pho2Eta",&pho2Eta);
  treeAlternate_LowRes->SetBranchAddress("run",&run);
  treeAlternate_LowRes->SetBranchAddress("event",&event);
  treeAlternate_LowRes->SetBranchAddress("MR",&MR);
  treeAlternate_LowRes->SetBranchAddress("t1Rsq",&Rsq);
  treeAlternate_LowRes->SetBranchAddress("mGammaGamma",&mgg);
  treeAlternate_LowRes->SetBranchAddress("pho1Pt",&pho1Pt);
  treeAlternate_LowRes->SetBranchAddress("pho1Eta",&pho1Eta);
  treeAlternate_LowRes->SetBranchAddress("pho2Pt",&pho2Pt);
  treeAlternate_LowRes->SetBranchAddress("pho2Eta",&pho2Eta);
  treeVectSumPt_LowRes->SetBranchAddress("run",&run);
  treeVectSumPt_LowRes->SetBranchAddress("event",&event);
  treeVectSumPt_LowRes->SetBranchAddress("MR",&MR);
  treeVectSumPt_LowRes->SetBranchAddress("t1Rsq",&Rsq);
  treeVectSumPt_LowRes->SetBranchAddress("mGammaGamma",&mgg);
  treeVectSumPt_LowRes->SetBranchAddress("pho1Pt",&pho1Pt);
  treeVectSumPt_LowRes->SetBranchAddress("pho1Eta",&pho1Eta);
  treeVectSumPt_LowRes->SetBranchAddress("pho2Pt",&pho2Pt);
  treeVectSumPt_LowRes->SetBranchAddress("pho2Eta",&pho2Eta);


  for (int n=0;n<treeStandard_HighPt->GetEntries();n++) { 
    
    treeStandard_HighPt->GetEntry(n);
    
    if ( pho1Pt > 25 && pho2Pt > 25 && ( pho1Pt>40 || pho2Pt>40) 
	 && fabs(pho1Eta) < 1.44 && fabs(pho2Eta) < 1.44
	 && mgg > 103 && mgg < 160
	 //&& MR > 200 && Rsq > 0.05
	 ) {
      outfileStandard_HighPt << run << " " << event << "\n";
    }
    
  }
 
  for (int n=0;n<treeAlternate_HighPt->GetEntries();n++) { 
    
    treeAlternate_HighPt->GetEntry(n);
    
    if ( pho1Pt > 25 && pho2Pt > 25 && ( pho1Pt>40 || pho2Pt>40) 
	 && fabs(pho1Eta) < 1.44 && fabs(pho2Eta) < 1.44
	 && mgg > 103 && mgg < 160
	 //&& MR > 200 && Rsq > 0.05
	 ) {
      outfileAlternate_HighPt << run << " " << event << "\n";
    }
    
  }
 

  for (int n=0;n<treeVectSumPt_HighPt->GetEntries();n++) { 
    
    treeVectSumPt_HighPt->GetEntry(n);
    
    if ( pho1Pt > 25 && pho2Pt > 25 && ( pho1Pt>40 || pho2Pt>40) 
	 && fabs(pho1Eta) < 1.44 && fabs(pho2Eta) < 1.44
	 && mgg > 103 && mgg < 160
	 //&& MR > 200 && Rsq > 0.05
	 ) {
      outfileVectSumPt_HighPt << run << " " << event << "\n";
    }
    
  }
 

  for (int n=0;n<treeStandard_HighRes->GetEntries();n++) { 
    
    treeStandard_HighRes->GetEntry(n);
    
    if ( pho1Pt > 25 && pho2Pt > 25 && ( pho1Pt>40 || pho2Pt>40) 
	 && fabs(pho1Eta) < 1.44 && fabs(pho2Eta) < 1.44
	 && mgg > 103 && mgg < 160
	 //&& MR > 200 && Rsq > 0.05
	 ) {
      outfileStandard_HighRes << run << " " << event << "\n";
    }
    
  }
 
  for (int n=0;n<treeAlternate_HighRes->GetEntries();n++) { 
    
    treeAlternate_HighRes->GetEntry(n);
    
    if ( pho1Pt > 25 && pho2Pt > 25 && ( pho1Pt>40 || pho2Pt>40) 
	 && fabs(pho1Eta) < 1.44 && fabs(pho2Eta) < 1.44
	 && mgg > 103 && mgg < 160
	 //&& MR > 200 && Rsq > 0.05
	 ) {
      outfileAlternate_HighRes << run << " " << event << "\n";
    }
    
  }
 

  for (int n=0;n<treeVectSumPt_HighRes->GetEntries();n++) { 
    
    treeVectSumPt_HighRes->GetEntry(n);
    
    if ( pho1Pt > 25 && pho2Pt > 25 && ( pho1Pt>40 || pho2Pt>40) 
	 && fabs(pho1Eta) < 1.44 && fabs(pho2Eta) < 1.44
	 && mgg > 103 && mgg < 160
	 //&& MR > 200 && Rsq > 0.05
	 ) {
      outfileVectSumPt_HighRes << run << " " << event << "\n";
    }
    
  }
 


  for (int n=0;n<treeStandard_LowRes->GetEntries();n++) { 
    
    treeStandard_LowRes->GetEntry(n);
    
    if ( pho1Pt > 25 && pho2Pt > 25 && ( pho1Pt>40 || pho2Pt>40) 
	 && fabs(pho1Eta) < 1.44 && fabs(pho2Eta) < 1.44
	 && mgg > 103 && mgg < 160
	 //&& MR > 200 && Rsq > 0.05
	 ) {
      outfileStandard_LowRes << run << " " << event << "\n";
    }
    
  }
 
  for (int n=0;n<treeAlternate_LowRes->GetEntries();n++) { 
    
    treeAlternate_LowRes->GetEntry(n);
    
    if ( pho1Pt > 25 && pho2Pt > 25 && ( pho1Pt>40 || pho2Pt>40) 
	 && fabs(pho1Eta) < 1.44 && fabs(pho2Eta) < 1.44
	 && mgg > 103 && mgg < 160
	 //&& MR > 200 && Rsq > 0.05
	 ) {
      outfileAlternate_LowRes << run << " " << event << "\n";
    }
    
  }
 

  for (int n=0;n<treeVectSumPt_LowRes->GetEntries();n++) { 
    
    treeVectSumPt_LowRes->GetEntry(n);
    
    if ( pho1Pt > 25 && pho2Pt > 25 && ( pho1Pt>40 || pho2Pt>40) 
	 && fabs(pho1Eta) < 1.44 && fabs(pho2Eta) < 1.44
	 && mgg > 103 && mgg < 160
	 //&& MR > 200 && Rsq > 0.05
	 ) {
      outfileVectSumPt_LowRes << run << " " << event << "\n";
    }
    
  }
 


 

}
