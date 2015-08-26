

//*************************************************************************************************
//Normalize Hist
//*************************************************************************************************
void NormalizeHist(TH1D *hist) {
  // TH1D* hist = (TH1D*)originalHist->Clone((string(originalHist->GetName())+"_normalized").c_str());
  Double_t norm = 0;
  //hist->SetTitle("");

  for (UInt_t b=0; int(b)<hist->GetXaxis()->GetNbins()+2; ++b) {
    norm += hist->GetBinContent(b);
  }
  cout << "norm: " << norm << "\n";
  for (UInt_t b=0; int(b)<hist->GetXaxis()->GetNbins()+2; ++b) {
    cout << "bin: " << b << " " << hist->GetBinContent(b) << "\n";
    hist->SetBinContent(b,hist->GetBinContent(b) / norm);
    hist->SetBinError(b,hist->GetBinError(b) / norm);
    cout << "after norm bin: " << b << " " << hist->GetBinContent(b) << "\n";;
  }

}


void PlotMggShapeVsRsq(bool useMC = false, string category = "highres") {

  TFile *fileData = 0;
  if (!useMC) {
    fileData = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/AlexRaidDisk/DoublePhoton_22Jan2013_Run2012ABCD_v9.root","READ");
  } else {
    fileData = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/AlexRaidDisk/DiPhotonJetsBox_M60_8TeV-sherpa__Summer12_DR53X-PU_S10_START53_V7C-v1_v9__WEIGHTED.root","READ");
  }
  TTree *tree = (TTree*)fileData->Get("SusyHggTree");
  

  TH1D *MGG_MR150ToInf_Rsq0p00ToInf = new TH1D("MGG_MR150ToInf_Rsq0p00ToInf",";m_{#gamma#gamma} (GeV); Fraction ", 30,100,400);
  TH1D *MGG_MR400ToInf_Rsq0p00To0p015 = new TH1D("MGG_MR400ToInf_Rsq0p00To0p015",";m_{#gamma#gamma} (GeV); Fraction ", 30,100,400);
  TH1D *MGG_MR400ToInf_Rsq0p015To0p03 = new TH1D("MGG_MR400ToInf_Rsq0p015To0p03",";m_{#gamma#gamma} (GeV); Fraction ", 30,100,400);
  TH1D *MGG_MR400ToInf_Rsq0p03To0p05 = new TH1D("MGG_MR400ToInf_Rsq0p03To0p05",";m_{#gamma#gamma} (GeV); Fraction ", 30,100,400);
  TH1D *MGG_MR400ToInf_Rsq0p05ToInf = new TH1D("MGG_MR400ToInf_Rsq0p05ToInf",";m_{#gamma#gamma} (GeV); Fraction ", 30,100,400);

  string categoryCutString = "";
  if (category == "highpt") categoryCutString = " && ptgg >= 110";
  else if (category == "highres") categoryCutString = "  abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && ptgg < 110 && pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015 ";
  else if (category == "lowres") categoryCutString = " abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && ptgg < 110 && !(pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015) ";

  string controlRegionCutString = "";
  if (useMC) controlRegionCutString = "";
  else controlRegionCutString = "&& (!pho1_pass_iso || !pho2_pass_iso) ";

  tree->Draw("mgg>>MGG_MR150ToInf_Rsq0p00ToInf", ("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && mgg>100&&mgg<400 && MR > 150 && t1Rsq > 0.0 " + categoryCutString + controlRegionCutString).c_str(),"");
  tree->Draw("mgg>>MGG_MR400ToInf_Rsq0p00To0p015",("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && mgg>100&&mgg<400 && MR > 400 && t1Rsq > 0.0 && t1Rsq<0.015 " + categoryCutString + controlRegionCutString).c_str(),"");
  tree->Draw("mgg>>MGG_MR400ToInf_Rsq0p015To0p03",("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && mgg>100&&mgg<400 && MR > 400 && t1Rsq > 0.015 && t1Rsq<0.03 " + categoryCutString + controlRegionCutString).c_str(),"");
  tree->Draw("mgg>>MGG_MR400ToInf_Rsq0p03To0p05",("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && mgg>100&&mgg<400 && MR > 400 && t1Rsq > 0.03 && t1Rsq<0.045 " + categoryCutString + controlRegionCutString).c_str(),"");
  tree->Draw("mgg>>MGG_MR400ToInf_Rsq0p05ToInf",("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && mgg>100&&mgg<400 && MR > 400 && t1Rsq > 0.045 " + categoryCutString + controlRegionCutString).c_str(),"");


 NormalizeHist(MGG_MR150ToInf_Rsq0p00ToInf);
 NormalizeHist(MGG_MR400ToInf_Rsq0p00To0p015);
 NormalizeHist(MGG_MR400ToInf_Rsq0p015To0p03);
 NormalizeHist(MGG_MR400ToInf_Rsq0p03To0p05);
 NormalizeHist(MGG_MR400ToInf_Rsq0p05ToInf);


 //Draw Plots
 TCanvas *cv = 0;
 TLegend *legend= 0;
 TLatex *tex = 0;

 cv = new TCanvas("cv","cv", 800,600); 
 MGG_MR400ToInf_Rsq0p00To0p015->SetStats(0);
 MGG_MR400ToInf_Rsq0p015To0p03->SetStats(0);
 MGG_MR400ToInf_Rsq0p03To0p05->SetStats(0);
 MGG_MR400ToInf_Rsq0p05ToInf->SetStats(0);
 MGG_MR400ToInf_Rsq0p00To0p015->Draw("hist");
 MGG_MR400ToInf_Rsq0p00To0p015->SetMaximum(0.3);
 MGG_MR400ToInf_Rsq0p00To0p015->SetMinimum(0.0);
 MGG_MR400ToInf_Rsq0p00To0p015->GetXaxis()->SetTitleSize(0.045);
 MGG_MR400ToInf_Rsq0p00To0p015->GetYaxis()->SetTitleSize(0.045);
 MGG_MR400ToInf_Rsq0p00To0p015->GetYaxis()->SetTitleOffset(0.8);
 
 MGG_MR400ToInf_Rsq0p00To0p015->SetLineColor(kBlack); 
 MGG_MR400ToInf_Rsq0p00To0p015->SetLineWidth(3); 
 MGG_MR400ToInf_Rsq0p00To0p015->SetMarkerStyle(8); 
 MGG_MR400ToInf_Rsq0p00To0p015->SetMarkerSize(1.3); 
 MGG_MR400ToInf_Rsq0p00To0p015->SetMarkerColor(kBlack); 
 
 MGG_MR400ToInf_Rsq0p015To0p03->SetLineWidth(3); 
 MGG_MR400ToInf_Rsq0p03To0p05->SetLineWidth(2); 
 MGG_MR400ToInf_Rsq0p05ToInf->SetLineWidth(2); 
 MGG_MR150ToInf_Rsq0p00ToInf->SetLineWidth(4); 
 MGG_MR400ToInf_Rsq0p015To0p03->SetLineColor(kRed);
 MGG_MR400ToInf_Rsq0p03To0p05->SetLineColor(kBlue);
 MGG_MR400ToInf_Rsq0p05ToInf->SetLineColor(kGreen+2);
 MGG_MR150ToInf_Rsq0p00ToInf->SetLineColor(kMagenta); 
 MGG_MR400ToInf_Rsq0p015To0p03->Draw("histsame");
 MGG_MR400ToInf_Rsq0p03To0p05->Draw("e1same");
 MGG_MR150ToInf_Rsq0p00ToInf->Draw("histsame");

 MGG_MR400ToInf_Rsq0p05ToInf->Draw("histsame");
 TH1D* MGG_MR400ToInf_Rsq0p05ToInf_Clone = (TH1D*)MGG_MR400ToInf_Rsq0p05ToInf->Clone();
 MGG_MR400ToInf_Rsq0p05ToInf_Clone->SetFillColor(kGreen+2);
 MGG_MR400ToInf_Rsq0p05ToInf_Clone->SetFillStyle(3002);
 MGG_MR400ToInf_Rsq0p05ToInf_Clone->Draw("e3same");

 legend = new TLegend(0.40,0.50,0.90,0.84);
 legend->SetTextSize(0.04);
 legend->SetBorderSize(0);
 legend->SetFillStyle(0);
 legend->AddEntry(MGG_MR150ToInf_Rsq0p00ToInf , "MR > 150 & Inclusive in R^{2}" , "L");
 legend->AddEntry(MGG_MR400ToInf_Rsq0p00To0p015 , "MR > 400 & R^{2} : (0.0,0.015)" , "L");
 legend->AddEntry(MGG_MR400ToInf_Rsq0p015To0p03 , "MR > 400 & R^{2} : (0.015,0.03)" , "L");
 legend->AddEntry(MGG_MR400ToInf_Rsq0p03To0p05 , "MR > 400 & R^{2} : (0.03,0.05)" , "L");
 legend->AddEntry(MGG_MR400ToInf_Rsq0p05ToInf , "MR > 400 & R^{2} : (0.05,Inf)" , "L");
 legend->Draw();

 tex = new TLatex();
 tex->SetNDC();
 tex->SetTextSize(0.040);
 tex->SetTextFont(42);
 tex->SetTextColor(kBlack);
 tex->DrawLatex(0.1,0.92,"CMS Preliminary");
 tex->DrawLatex(0.6, 0.92, "#sqrt{s} = 8 TeV, L = 19.8 fb^{-1}");
 tex->Draw();

 if (!useMC) {
   cv->SaveAs(Form("HggMassShape_VsRsq_Data_%s.gif",category.c_str()));
   cv->SaveAs(Form("HggMassShape_VsRsq_Data_%s.pdf",category.c_str()));
 } else {
   cv->SaveAs(Form("HggMassShape_VsRsq_MC_%s.gif",category.c_str()));
   cv->SaveAs(Form("HggMassShape_VsRsq_MC_%s.pdf",category.c_str()));
 }

 

}


void PlotMggShapeVsMR(bool useMC = false, string category = "highres") {

  TFile *fileData=0;
  if (!useMC) {
    fileData = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/AlexRaidDisk/DoublePhoton_22Jan2013_Run2012ABCD_v9.root","READ");
  } else {
    fileData = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/AlexRaidDisk/DiPhotonJetsBox_M60_8TeV-sherpa__Summer12_DR53X-PU_S10_START53_V7C-v1_v9__WEIGHTED.root","READ");
  }
 
  TTree *tree = (TTree*)fileData->Get("SusyHggTree");
  
  TH1D *MGG_MR150ToInf_Rsq0p00ToInf = new TH1D("MGG_MR150ToInf_Rsq0p00ToInf",";m_{#gamma#gamma} (GeV); Fraction ", 30,100,400);
  TH1D *MGG_MR150To200_Rsq0p05ToInf = new TH1D("MGG_MR150To200_Rsq0p05ToInf",";m_{#gamma#gamma} (GeV); Fraction ", 30,100,400);
  TH1D *MGG_MR200To300_Rsq0p05ToInf = new TH1D("MGG_MR200To300_Rsq0p05ToInf",";m_{#gamma#gamma} (GeV); Fraction ", 30,100,400);
  TH1D *MGG_MR300To400_Rsq0p05ToInf = new TH1D("MGG_MR300To400_Rsq0p05ToInf",";m_{#gamma#gamma} (GeV); Fraction ", 30,100,400);
  TH1D *MGG_MR400ToInf_Rsq0p05ToInf = new TH1D("MGG_MR400ToInf_Rsq0p05ToInf",";m_{#gamma#gamma} (GeV); Fraction ", 30,100,400);

  string categoryCutString = "";
  if (category == "highpt") categoryCutString = " && ptgg >= 110 ";
  else if (category == "highres") categoryCutString = " abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && ptgg < 110 && pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015 ";
  else if (category == "lowres") categoryCutString = " abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && ptgg < 110 && !(pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015) ";

  string controlRegionCutString = "";
  if (useMC) controlRegionCutString = "";
  else controlRegionCutString = "&& (!pho1_pass_iso || !pho2_pass_iso) ";


  tree->Draw("mgg>>MGG_MR150ToInf_Rsq0p00ToInf",("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && mgg>100&&mgg<400 && MR > 150 && t1Rsq > 0.00 " + categoryCutString + controlRegionCutString).c_str(),"");
  tree->Draw("mgg>>MGG_MR150To200_Rsq0p05ToInf",("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && mgg>100&&mgg<400 && MR > 150 && MR < 200 && t1Rsq > 0.05 " + categoryCutString + controlRegionCutString).c_str(),"");
  tree->Draw("mgg>>MGG_MR200To300_Rsq0p05ToInf",("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && mgg>100&&mgg<400 && MR > 200 && MR < 300 && t1Rsq > 0.05 " + categoryCutString + controlRegionCutString).c_str(),"");
  tree->Draw("mgg>>MGG_MR300To400_Rsq0p05ToInf",("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && mgg>100&&mgg<400 && MR > 300 && MR < 400 && t1Rsq > 0.05 " + categoryCutString + controlRegionCutString).c_str(),"");
  tree->Draw("mgg>>MGG_MR400ToInf_Rsq0p05ToInf",("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && mgg>100&&mgg<400 && MR > 400 && t1Rsq > 0.05 " + categoryCutString + controlRegionCutString).c_str(),"");



 NormalizeHist(MGG_MR150ToInf_Rsq0p00ToInf);
 NormalizeHist(MGG_MR150To200_Rsq0p05ToInf);
 NormalizeHist(MGG_MR200To300_Rsq0p05ToInf);
 NormalizeHist(MGG_MR300To400_Rsq0p05ToInf);
 NormalizeHist(MGG_MR400ToInf_Rsq0p05ToInf);

 //Draw Plots
 TCanvas *cv = 0;
 TLegend *legend= 0;
 TLatex *tex = 0;

 cv = new TCanvas("cv","cv", 800,600); 
 MGG_MR150To200_Rsq0p05ToInf->SetStats(0);
 MGG_MR200To300_Rsq0p05ToInf->SetStats(0);
 MGG_MR300To400_Rsq0p05ToInf->SetStats(0);
 MGG_MR400ToInf_Rsq0p05ToInf->SetStats(0);
 MGG_MR150To200_Rsq0p05ToInf->Draw("hist");
 MGG_MR150To200_Rsq0p05ToInf->SetMaximum(0.3);
 MGG_MR150To200_Rsq0p05ToInf->SetMinimum(0.0);
 MGG_MR150To200_Rsq0p05ToInf->GetXaxis()->SetTitleSize(0.045);
 MGG_MR150To200_Rsq0p05ToInf->GetYaxis()->SetTitleSize(0.045);
 MGG_MR150To200_Rsq0p05ToInf->GetYaxis()->SetTitleOffset(0.8);
 
 MGG_MR150To200_Rsq0p05ToInf->SetLineColor(kBlack); 
 MGG_MR150To200_Rsq0p05ToInf->SetLineWidth(3); 
 MGG_MR150To200_Rsq0p05ToInf->SetMarkerStyle(8); 
 MGG_MR150To200_Rsq0p05ToInf->SetMarkerSize(1.3); 
 MGG_MR150To200_Rsq0p05ToInf->SetMarkerColor(kBlack); 
 

 MGG_MR200To300_Rsq0p05ToInf->SetLineWidth(3); 
 MGG_MR300To400_Rsq0p05ToInf->SetLineWidth(2); 
 MGG_MR400ToInf_Rsq0p05ToInf->SetLineWidth(2); 
 MGG_MR150ToInf_Rsq0p00ToInf->SetLineWidth(4); 
 MGG_MR200To300_Rsq0p05ToInf->SetLineColor(kRed);
 MGG_MR300To400_Rsq0p05ToInf->SetLineColor(kBlue);
 MGG_MR400ToInf_Rsq0p05ToInf->SetLineColor(kGreen+2);
 MGG_MR150ToInf_Rsq0p00ToInf->SetLineColor(kMagenta); 
 MGG_MR200To300_Rsq0p05ToInf->Draw("histsame");
 MGG_MR300To400_Rsq0p05ToInf->Draw("e1same");
 TH1D* MGG_MR400ToInf_Rsq0p05ToInf_Clone = (TH1D*)MGG_MR400ToInf_Rsq0p05ToInf->Clone();
 MGG_MR400ToInf_Rsq0p05ToInf_Clone->SetFillColor(kGreen+2);
 MGG_MR400ToInf_Rsq0p05ToInf_Clone->SetFillStyle(3002);
 MGG_MR400ToInf_Rsq0p05ToInf->Draw("histsame");
 MGG_MR400ToInf_Rsq0p05ToInf_Clone->Draw("e3same");
 MGG_MR150ToInf_Rsq0p00ToInf->Draw("histsame");

 legend = new TLegend(0.40,0.50,0.90,0.84);
 legend->SetTextSize(0.04);
 legend->SetBorderSize(0);
 legend->SetFillStyle(0);
 legend->AddEntry(MGG_MR150ToInf_Rsq0p00ToInf , "MR > 150 & Inclusive in R^{2}" , "L");
 legend->AddEntry(MGG_MR150To200_Rsq0p05ToInf , "MR : (150,200) & R^{2} > 0.05" , "L");
 legend->AddEntry(MGG_MR200To300_Rsq0p05ToInf , "MR : (200,300) & R^{2} > 0.05" , "L");
 legend->AddEntry(MGG_MR300To400_Rsq0p05ToInf , "MR : (300,400) & R^{2} > 0.05" , "L");
 legend->AddEntry(MGG_MR400ToInf_Rsq0p05ToInf , "MR : (400,Inf) & R^{2} > 0.05" , "L");
 legend->Draw();

 tex = new TLatex();
 tex->SetNDC();
 tex->SetTextSize(0.040);
 tex->SetTextFont(42);
 tex->SetTextColor(kBlack);
 tex->DrawLatex(0.1,0.92,"CMS Preliminary");
 tex->DrawLatex(0.6, 0.92, "#sqrt{s} = 8 TeV, L = 19.8 fb^{-1}");
 tex->Draw();

 if (!useMC) {
   cv->SaveAs(Form("HggMassShape_VsMR_Data_%s.gif",category.c_str()));
   cv->SaveAs(Form("HggMassShape_VsMR_Data_%s.pdf",category.c_str()));
 } else {
   cv->SaveAs(Form("HggMassShape_VsMR_MC_%s.gif",category.c_str()));
   cv->SaveAs(Form("HggMassShape_VsMR_MC_%s.pdf",category.c_str()));
 }

 

}





void MggShapeStudy() {

  // PlotMggShapeVsRsq(false, "highres");
  // PlotMggShapeVsRsq(false, "lowres");
  // PlotMggShapeVsRsq(false, "highpt");
  // PlotMggShapeVsMR(false, "highres");
  // PlotMggShapeVsMR(false, "lowres");
  // PlotMggShapeVsMR(false, "highpt");

  // PlotMggShapeVsRsq(true, "highres");
  // PlotMggShapeVsRsq(true, "lowres");
  // PlotMggShapeVsRsq(true, "highpt");
  // PlotMggShapeVsMR(true, "highres");
  // PlotMggShapeVsMR(true, "lowres");
  // PlotMggShapeVsMR(true, "highpt");

  PlotMggShapeVsRsq(false, "inclusive");
  PlotMggShapeVsMR(false, "inclusive");
  PlotMggShapeVsRsq(true, "inclusive");
  PlotMggShapeVsMR(true, "inclusive");



}
