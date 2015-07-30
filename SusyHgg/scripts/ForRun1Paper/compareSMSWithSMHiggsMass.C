

float computeSigEff(TH1F *hist, float mean){
  std::cout << "MakeSpinFits::computeSigEff" << std::endl;
  // compute the sigma_eff for the pdf
  //only valid for symmetric pdfs

  double sigmaEff = 0;

  //find the bin closest to the mean
  int meanBin = -1;
  double distanceToMean = 9999;
  double histIntegral = 0;
  for (int i=0; i<hist->GetXaxis()->GetNbins()+1;i++) {
    if ( fabs(hist->GetBinCenter(i)-mean) < distanceToMean) {
      meanBin = i;
      distanceToMean = fabs(hist->GetBinCenter(i)-mean);
    }
    if (hist->GetBinCenter(i) > 115 && hist->GetBinCenter(i) < 135) {
    //if (hist->GetBinCenter(i) > 100 && hist->GetBinCenter(i) < 150) {
      histIntegral += hist->GetBinContent(i);
    }
  }

  int nbins = 0;
  while(true){
    nbins += 1;

    //integrate from meanBin for nbins in both directions
    double tmpInt = 0;
    for(int i=meanBin-nbins; i<=meanBin+nbins; i++) {
      tmpInt += hist->GetBinContent(i);
    }

    double cov = tmpInt / histIntegral;
    if(cov > 0.683){  
      sigmaEff = (hist->GetBinCenter(meanBin + nbins) -  hist->GetBinCenter(meanBin - nbins))/2;
      break;
    }
  }

  return sigmaEff;

}



void compareSMSWithSMHiggsMass() {

TFile *fileSM = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/AlexRaidDisk/GluGluToHToGG_M-125_8TeV-powheg-pythia6__Summer12_DR53X-PU_RD1_START53_V7N-v1_v9.root", "READ");
TFile *fileSMS = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/AlexRaidDisk/SMS-TChiWH_WincHgg_2J_mChargino-130to200_mLSP-1to70_TuneZ2star_8TeV-madgraph-tauola__Summer12_DR53X-PU_S10_START53_V19-v1_v9.root","READ");


// TFile *fileSM = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/AlexRaidDisk/TTH_HToGG_M-125_8TeV-pythia6__Summer12_DR53X-PU_RD1_START53_V7N-v1_v9.root", "READ");
// TFile *fileSMS = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/AlexRaidDisk/SMS-TChiHH_2b2g_2J_mChargino-130to200_mLSP-1to70_TuneZ2star_8TeV-madgraph-tauola__Summer12_DR53X-PU_S10_START53_V19-v1_v9.root","READ");

TTree *treeSM = fileSM->Get("SusyHggTree");
TTree *treeSMS = fileSMS->Get("SusyHggTree");

TH1F *MGG_HighPt_SM = new TH1F("MGG_HighPt_SM",";m_{#gamma#gamma} [GeV/c^{2}];Fraction Of Events",12000,100,160);
TH1F *MGG_HighPt_SMS = new TH1F("MGG_HighPt_SMS",";m_{#gamma#gamma} [GeV/c^{2}];Fraction Of Events",12000,100,160);
TH1F *MGG_HighRes_SM = new TH1F("MGG_HighRes_SM",";m_{#gamma#gamma} [GeV/c^{2}];Fraction Of Events",12000,100,160);
TH1F *MGG_HighRes_SMS = new TH1F("MGG_HighRes_SMS",";m_{#gamma#gamma} [GeV/c^{2}];Fraction Of Events",12000,100,160);
TH1F *MGG_LowRes_SM = new TH1F("MGG_LowRes_SM",";m_{#gamma#gamma} [GeV/c^{2}];Fraction Of Events",12000,100,160);
TH1F *MGG_LowRes_SMS = new TH1F("MGG_LowRes_SMS",";m_{#gamma#gamma} [GeV/c^{2}];Fraction Of Events",12000,100,160);
 treeSM->Draw("mgg>>MGG_HighPt_SM","ptgg>110&&abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho1_pass_iso && pho2_pass_id && pho2_pass_iso && mgg>100&&mgg<160","hist");
 treeSMS->Draw("mgg>>MGG_HighPt_SMS","ptgg>110&&abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho1_pass_iso && pho2_pass_id && pho2_pass_iso && mgg>100&&mgg<160","hist");
 treeSM->Draw("mgg>>MGG_HighRes_SM","abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && ptgg > 20 && ptgg<110&&pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho1_pass_iso && pho2_pass_id && pho2_pass_iso && mgg>100&&mgg<160","hist");
 treeSMS->Draw("mgg>>MGG_HighRes_SMS","abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && ptgg > 20 && ptgg<110&&pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015 &&abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho1_pass_iso && pho2_pass_id && pho2_pass_iso && mgg>100&&mgg<160","hist");
 treeSM->Draw("mgg>>MGG_LowRes_SM","abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && ptgg > 20 && ptgg<110&&!(pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015)&& abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho1_pass_iso && pho2_pass_id && pho2_pass_iso && mgg>100&&mgg<160","hist");
 treeSMS->Draw("mgg>>MGG_LowRes_SMS","abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && ptgg > 20 && ptgg<110&&!(pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015)&&abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho1_pass_iso && pho2_pass_id && pho2_pass_iso && mgg>100&&mgg<160","hist");

MGG_HighPt_SM->Scale( 1/MGG_HighPt_SM->Integral());
MGG_HighPt_SMS->Scale( 1/MGG_HighPt_SMS->Integral());
MGG_HighRes_SM->Scale( 1/MGG_HighRes_SM->Integral());
MGG_HighRes_SMS->Scale( 1/MGG_HighRes_SMS->Integral());
MGG_LowRes_SM->Scale( 1/MGG_LowRes_SM->Integral());
MGG_LowRes_SMS->Scale( 1/MGG_LowRes_SMS->Integral());


//Compute sigmaEff;
 double sigmaEff_SM_HighPt = computeSigEff(MGG_HighPt_SM,MGG_HighPt_SM->GetMean());
 double sigmaEff_SMS_HighPt = computeSigEff(MGG_HighPt_SMS,MGG_HighPt_SMS->GetMean());
 double sigmaEff_SM_HighRes = computeSigEff(MGG_HighRes_SM,MGG_HighRes_SM->GetMean());
 double sigmaEff_SMS_HighRes = computeSigEff(MGG_HighRes_SMS,MGG_HighRes_SMS->GetMean());
 double sigmaEff_SM_LowRes = computeSigEff(MGG_LowRes_SM,MGG_LowRes_SM->GetMean());
 double sigmaEff_SMS_LowRes = computeSigEff(MGG_LowRes_SMS,MGG_LowRes_SMS->GetMean());


 //Draw Plots
 TCanvas *cv = 0;
 TLegend *legend= 0;

 cv = new TCanvas("cv","cv", 800,600); 
 MGG_HighPt_SM->Rebin(100);
 MGG_HighPt_SMS->Rebin(100);
 MGG_HighPt_SM->SetStats(false);
 MGG_HighPt_SM->Draw();
 MGG_HighPt_SMS->Draw("samehist");
 MGG_HighPt_SM->SetLineColor(kBlue);
 MGG_HighPt_SMS->SetLineColor(kRed);
 
 legend = new TLegend(0.60,0.50,0.90,0.84);
 legend->SetTextSize(0.04);
 legend->SetBorderSize(0);
 legend->SetFillStyle(0);
 legend->AddEntry(MGG_HighPt_SM,"SM Higgs","L");
 legend->AddEntry(MGG_HighPt_SMS,"SMS Model","L");
 legend->Draw();
 cv->SaveAs("SMVsSMSHiggsMass_HighPt.gif");

 cv = new TCanvas("cv","cv", 800,600); 
 MGG_HighRes_SM->Rebin(100);
 MGG_HighRes_SMS->Rebin(100);
 MGG_HighRes_SM->SetStats(false);
 MGG_HighRes_SM->Draw();
 MGG_HighRes_SMS->Draw("samehist");
 MGG_HighRes_SM->SetLineColor(kBlue);
 MGG_HighRes_SMS->SetLineColor(kRed);
 
 legend = new TLegend(0.60,0.50,0.90,0.84);
 legend->SetTextSize(0.04);
 legend->SetBorderSize(0);
 legend->SetFillStyle(0);
 legend->AddEntry(MGG_HighRes_SM,"SM Higgs","L");
 legend->AddEntry(MGG_HighRes_SMS,"SMS Model","L");
 legend->Draw();
 cv->SaveAs("SMVsSMSHiggsMass_HighRes.gif");

 cv = new TCanvas("cv","cv", 800,600); 
 MGG_LowRes_SM->Rebin(100);
 MGG_LowRes_SMS->Rebin(100);
 MGG_LowRes_SM->SetStats(false);
 MGG_LowRes_SM->Draw();
 MGG_LowRes_SMS->Draw("samehist");
 MGG_LowRes_SM->SetLineColor(kBlue);
 MGG_LowRes_SMS->SetLineColor(kRed);
 
 legend = new TLegend(0.60,0.50,0.90,0.84);
 legend->SetTextSize(0.04);
 legend->SetBorderSize(0);
 legend->SetFillStyle(0);
 legend->AddEntry(MGG_LowRes_SM,"SM Higgs","L");
 legend->AddEntry(MGG_LowRes_SMS,"SMS Model","L");
 legend->Draw();
 cv->SaveAs("SMVsSMSHiggsMass_LowRes.gif");


 //Print out
 cout << "High Pt Box:\n";
 cout << "sigmaEff ggH : " << sigmaEff_SM_HighPt << "\n";
 cout << "sigmaEff SMS : " << sigmaEff_SMS_HighPt << "\n";
 cout << "\n\n";

 cout << "High Res Box:\n";
 cout << "sigmaEff ggH : " << sigmaEff_SM_HighRes << "\n";
 cout << "sigmaEff SMS : " << sigmaEff_SMS_HighRes << "\n";
 cout << "\n\n";

 cout << "Low Res Box:\n";
 cout << "sigmaEff ggH : " << sigmaEff_SM_LowRes << "\n";
 cout << "sigmaEff SMS : " << sigmaEff_SMS_LowRes << "\n";
 cout << "\n\n";


}
