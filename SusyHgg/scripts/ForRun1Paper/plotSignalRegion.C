

void plotSignalRegion() {

  TFile *fileData = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/AlexRaidDisk/DoublePhoton_22Jan2013_Run2012ABCD_v9.root","READ");
  TTree *tree = (TTree*)fileData->Get("SusyHggTree");
  

  TH1F *MGG_HighRes = new TH1F("MGG_HighRes",";m_{#gamma#gamma} (GeV); Events / (1.5 GeV)", 38,103,160);
  MGG_HighRes->SetBinErrorOption(TH1::kPoisson);
 tree->Draw("mgg>>MGG_HighRes","abs(mbb-125)>=25 && abs(mbb-91.2)>=25 &&  ptgg > 20 && ptgg<110&&pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho1_pass_iso && pho2_pass_id && pho2_pass_iso && mgg>103&&mgg<160 && MR > 400 && MR < 1400 && t1Rsq>0.05 ","e1");


 //Draw Plots
 TCanvas *cv = 0;
 TLegend *legend= 0;
 TLatex *tex = 0;

 cv = new TCanvas("cv","cv", 800,600); 
 
 MGG_HighRes->SetStats(false);
 MGG_HighRes->Draw("e1");
 MGG_HighRes->GetXaxis()->SetTitleSize(0.045);
 MGG_HighRes->GetYaxis()->SetTitleSize(0.045);
 MGG_HighRes->GetYaxis()->SetTitleOffset(0.8);
 
 MGG_HighRes->SetLineColor(kBlack); 
 MGG_HighRes->SetLineWidth(1); 
 MGG_HighRes->SetMarkerStyle(8); 
 MGG_HighRes->SetMarkerSize(1.3); 
 MGG_HighRes->SetMarkerColor(kBlack); 

 tex = new TLatex();
 tex->SetNDC();
 tex->SetTextSize(0.040);
 tex->SetTextFont(42);
 tex->SetTextColor(kBlack);
 tex->DrawLatex(0.1,0.92,"CMS Preliminary");
 tex->DrawLatex(0.6, 0.92, "#sqrt{s} = 8 TeV, L = 19.8 fb^{-1}");
 tex->Draw();
  
 TBox *box = new TBox(121,0.2,129,6);
 box->SetFillColor(kRed-6);
 box->Draw();

 MGG_HighRes->Draw("e1same");

 cv->SetGrid();
 cv->SaveAs("HggMassPlot_HighRes.gif");

 

}
