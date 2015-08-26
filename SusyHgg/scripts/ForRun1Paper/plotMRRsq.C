

void plotMRRsq(string category = "highpt") {

  TFile *fileData = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/AlexRaidDisk/DoublePhoton_22Jan2013_Run2012ABCD_v9.root","READ");
  TTree *tree = (TTree*)fileData->Get("SusyHggTree");
  

  string categoryCutString = "";
  if (category == "highpt") categoryCutString = " && ptgg >= 110";
  else if (category == "hbb") categoryCutString = " && ptgg < 110 && abs(mbb-125) < 25";
  else if (category == "zbb") categoryCutString = " && ptgg < 110 && abs(mbb-91.2) < 25 ";
  else if (category == "highres") categoryCutString = "&& abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && ptgg < 110 && pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015 ";
  else if (category == "lowres") categoryCutString = " && abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && ptgg < 110 && !(pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015) ";

  string signalRegionCutString = "";
  if (category == "highpt") signalRegionCutString = " && mgg > 121.88 && mgg < 129.12";
  else if (category == "hbb") signalRegionCutString = " && mgg > 121 && mgg < 130";
  else if (category == "zbb") signalRegionCutString = " && mgg > 121 && mgg < 130 ";
  else if (category == "highres") signalRegionCutString = " && mgg > 122.08 && mgg < 128.92 ";
  else if (category == "lowres") signalRegionCutString = " && mgg > 120 && mgg < 131";

  string transferFactorString = "";
  if (category == "highpt") transferFactorString = "0.162";
  else if (category == "hbb") transferFactorString = "0.212";
  else if (category == "zbb") transferFactorString = "0.204";
  else if (category == "highres") transferFactorString = "0.162";
  else if (category == "lowres") transferFactorString = "0.259";

  TH2F *MRRsqSideband = new TH2F("MRRsqSideband",";M_{R} (GeV); R^{2} ; ", 85, 150,1000, 20 , 0.0, 0.2);
  TH2F *MRRsqSignal = new TH2F("MRRsqSignal",";M_{R} (GeV); R^{2} ; ", 85, 150,1000, 20 , 0.0, 0.2);
  //TH2F *MRRsqSideband = new TH2F("MRRsqSideband",";M_{R} (GeV); R^{2} ; ", 85, 150,1000, 20 , 0.0, 200);
  //TH2F *MRRsqSignal = new TH2F("MRRsqSignal",";M_{R} (GeV); R^{2} ; ", 85, 150,1000, 20 , 0.0, 200);

  tree->Draw("t1Rsq:MR>>MRRsqSideband", (transferFactorString + "*(ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && pho1_pass_iso && pho2_pass_iso && ((mgg > 105 && mgg < 120) || (mgg > 131 && mgg < 160)) && MR > 150 && t1Rsq > 0.0 " + categoryCutString+ " )").c_str(),"");
  tree->Draw("t1Rsq:MR>>MRRsqSignal", ("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && pho1_pass_iso && pho2_pass_iso && MR > 150 && t1Rsq > 0.0 " + categoryCutString + signalRegionCutString).c_str(),"");
  //tree->Draw("t1MET:HT>>MRRsqSideband", (transferFactorString + "*(ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && pho1_pass_iso && pho2_pass_iso && ((mgg > 105 && mgg < 120) || (mgg > 131 && mgg < 160)) && MR > 150 && t1Rsq > 0.0 " + categoryCutString+ " )").c_str(),"");
  //tree->Draw("t1MET:HT>>MRRsqSignal", ("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && pho1_pass_iso && pho2_pass_iso && MR > 150 && t1Rsq > 0.0 " + categoryCutString + signalRegionCutString).c_str(),"");

  cout << "draw string: " <<  (transferFactorString + "*(ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && ((mgg > 105 && mgg < 120) || (mgg > 131 && mgg < 160)) && MR > 150 && t1Rsq > 0.0 " + categoryCutString+ " )") << "\n";


  //color palette
  const Int_t Number = 2;
  Double_t Red[Number]    =  { 0.90, 1.00};
  Double_t Green[Number]  =  { 0.90, 0.00};
  Double_t Blue[Number]   =  { 0.90, 0.00};
  Double_t Length[Number] =  { 0.00, 1.00};
  Int_t nb=50;
  TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
  

 //Draw Plots
 TCanvas *cv = 0;
 TLegend *legend= 0;
 TLatex *tex = 0;

 cv = new TCanvas("cv","cv", 800,600); 
 MRRsqSideband->SetStats(0);
 MRRsqSignal->SetStats(0);

 MRRsqSideband->Draw("colz");
 MRRsqSignal->Draw("scat,same");
 MRRsqSignal->SetMarkerStyle(20);
 MRRsqSignal->SetMarkerColor(kBlack);
 if (category == "lowres") {
   MRRsqSignal->SetMarkerSize(0.6);
 } else {
   MRRsqSignal->SetMarkerSize(0.8);
 }

 string categoryText = "";
 if (category == "highpt") categoryText = "HighPt Category";
  else if (category == "hbb") categoryText = "Hbb Category";
  else if (category == "zbb") categoryText = "Zbb Category";
  else if (category == "highres") categoryText = "HighRes Category";
  else if (category == "lowres") categoryText = "LowRes Category";

 tex = new TLatex();
 tex->SetNDC();
 tex->SetTextSize(0.040);
 tex->SetTextFont(42);
 tex->SetTextColor(kBlack);
 tex->DrawLatex(0.7, 0.92, "19.8 fb^{-1} (8 TeV)");
 tex->DrawLatex(0.45, 0.92, categoryText.c_str());
 tex->Draw();
 
 TLatex *CMSLabel = new TLatex();
 CMSLabel->SetNDC();
 CMSLabel->SetTextSize(0.050);
 CMSLabel->SetTextFont(42);
 CMSLabel->SetTextColor(kBlack);
 CMSLabel->DrawLatex(0.1,0.92,"#bf{CMS}");
 CMSLabel->SetTextSize(0.045);
 CMSLabel->DrawLatex(0.19,0.92,"Preliminary");
 CMSLabel->Draw();
 
 MRRsqSideband->SetFillColor(kRed);

 legend = new TLegend(0.50,0.70,0.87,0.87);
 legend->SetTextSize(0.04);
 legend->SetBorderSize(0);
 //legend->SetFillStyle(0);
 legend->AddEntry(MRRsqSignal , "Signal Region" , "P");
 legend->AddEntry(MRRsqSideband , "Sideband Prediction" , "F");
 legend->Draw();


 if (category == "highpt") {
   TLine *l1 = new TLine(200,0,200,0.2);
   l1->SetLineWidth(3);
   l1->SetLineStyle(1);
   l1->SetLineColor(kGray+2);
   l1->Draw();

   TLine *l2 = new TLine(300,0,300,0.2);
   l2->SetLineWidth(3);
   l2->SetLineStyle(1);
   l2->SetLineColor(kGray+2);
   l2->Draw();

   TLine *l3 = new TLine(500,0,500,0.2);
   l3->SetLineWidth(3);
   l3->SetLineStyle(1);
   l3->SetLineColor(kGray+2);
   l3->Draw();

   TLine *l10 = new TLine(150,0.05,200,0.05);
   l10->SetLineWidth(3);
   l10->SetLineStyle(1);
   l10->SetLineColor(kGray+2);
   l10->Draw();

   TLine *l11 = new TLine(150,0.10,200,0.10);
   l11->SetLineWidth(3);
   l11->SetLineStyle(1);
   l11->SetLineColor(kGray+2);
   l11->Draw();

   TLine *l12 = new TLine(150,0.15,200,0.15);
   l12->SetLineWidth(3);
   l12->SetLineStyle(1);
   l12->SetLineColor(kGray+2);
   l12->Draw();

   TLine *l13 = new TLine(200,0.05,300,0.05);
   l13->SetLineWidth(3);
   l13->SetLineStyle(1);
   l13->SetLineColor(kGray+2);
   l13->Draw();

   TLine *l14 = new TLine(200,0.10,300,0.10);
   l14->SetLineWidth(3);
   l14->SetLineStyle(1);
   l14->SetLineColor(kGray+2);
   l14->Draw();

   TLine *l15 = new TLine(200,0.15,300,0.15);
   l15->SetLineWidth(3);
   l15->SetLineStyle(1);
   l15->SetLineColor(kGray+2);
   l15->Draw();

   TLine *l16 = new TLine(300,0.05,500,0.05);
   l16->SetLineWidth(3);
   l16->SetLineStyle(1);
   l16->SetLineColor(kGray+2);
   l16->Draw();

   TLine *l17 = new TLine(300,0.10,500,0.10);
   l17->SetLineWidth(3);
   l17->SetLineStyle(1);
   l17->SetLineColor(kGray+2);
   l17->Draw();

   TLine *l18 = new TLine(500,0.05,1000,0.05);
   l18->SetLineWidth(3);
   l18->SetLineStyle(1);
   l18->SetLineColor(kGray+2);
   l18->Draw();

 }


 if (category == "hbb") {
   TLine *l2 = new TLine(300,0,300,0.2);
   l2->SetLineWidth(3);
   l2->SetLineStyle(1);
   l2->SetLineColor(kGray+2);
   l2->Draw();

   TLine *l10 = new TLine(150,0.05,300,0.05);
   l10->SetLineWidth(3);
   l10->SetLineStyle(1);
   l10->SetLineColor(kGray+2);
   l10->Draw();

 }

 if (category == "zbb") {
   TLine *l2 = new TLine(450,0,450,0.2);
   l2->SetLineWidth(3);
   l2->SetLineStyle(1);
   l2->SetLineColor(kGray+2);
   l2->Draw();

   TLine *l10 = new TLine(150,0.05,450,0.05);
   l10->SetLineWidth(3);
   l10->SetLineStyle(1);
   l10->SetLineColor(kGray+2);
   l10->Draw();

 }

 if (category == "highres") {
   TLine *l1 = new TLine(250,0,250,0.2);
   l1->SetLineWidth(3);
   l1->SetLineStyle(1);
   l1->SetLineColor(kGray+2);
   l1->Draw();

   TLine *l2 = new TLine(400,0,400,0.2);
   l2->SetLineWidth(3);
   l2->SetLineStyle(1);
   l2->SetLineColor(kGray+2);
   l2->Draw();

   TLine *l3 = new TLine(1400,0,1400,0.2);
   l3->SetLineWidth(3);
   l3->SetLineStyle(1);
   l3->SetLineColor(kGray+2);
   l3->Draw();

   TLine *l10 = new TLine(150,0.05,250,0.05);
   l10->SetLineWidth(3);
   l10->SetLineStyle(1);
   l10->SetLineColor(kGray+2);
   l10->Draw();

   TLine *l11 = new TLine(150,0.10,250,0.10);
   l11->SetLineWidth(3);
   l11->SetLineStyle(1);
   l11->SetLineColor(kGray+2);
   l11->Draw();

   TLine *l12 = new TLine(150,0.15,250,0.15);
   l12->SetLineWidth(3);
   l12->SetLineStyle(1);
   l12->SetLineColor(kGray+2);
   l12->Draw();

   TLine *l13 = new TLine(250,0.05,400,0.05);
   l13->SetLineWidth(3);
   l13->SetLineStyle(1);
   l13->SetLineColor(kGray+2);
   l13->Draw();

   TLine *l14 = new TLine(250,0.10,400,0.10);
   l14->SetLineWidth(3);
   l14->SetLineStyle(1);
   l14->SetLineColor(kGray+2);
   l14->Draw();

   TLine *l16 = new TLine(400,0.05,1000,0.05);
   l16->SetLineWidth(3);
   l16->SetLineStyle(1);
   l16->SetLineColor(kGray+2);
   l16->Draw();



 }


 if (category == "lowres") {
   TLine *l1 = new TLine(200,0,200,0.2);
   l1->SetLineWidth(3);
   l1->SetLineStyle(1);
   l1->SetLineColor(kGray+2);
   l1->Draw();

   TLine *l2 = new TLine(250,0,250,0.2);
   l2->SetLineWidth(3);
   l2->SetLineStyle(1);
   l2->SetLineColor(kGray+2);
   l2->Draw();

   TLine *l3 = new TLine(400,0,400,0.2);
   l3->SetLineWidth(3);
   l3->SetLineStyle(1);
   l3->SetLineColor(kGray+2);
   l3->Draw();

   TLine *l10 = new TLine(150,0.05,200,0.05);
   l10->SetLineWidth(3);
   l10->SetLineStyle(1);
   l10->SetLineColor(kGray+2);
   l10->Draw();

   TLine *l11 = new TLine(150,0.10,200,0.10);
   l11->SetLineWidth(3);
   l11->SetLineStyle(1);
   l11->SetLineColor(kGray+2);
   l11->Draw();

   TLine *l12 = new TLine(150,0.15,200,0.15);
   l12->SetLineWidth(3);
   l12->SetLineStyle(1);
   l12->SetLineColor(kGray+2);
   l12->Draw();

   TLine *l13 = new TLine(200,0.05,250,0.05);
   l13->SetLineWidth(3);
   l13->SetLineStyle(1);
   l13->SetLineColor(kGray+2);
   l13->Draw();

   TLine *l14 = new TLine(200,0.10,250,0.10);
   l14->SetLineWidth(3);
   l14->SetLineStyle(1);
   l14->SetLineColor(kGray+2);
   l14->Draw();

   TLine *l15 = new TLine(200,0.15,250,0.15);
   l15->SetLineWidth(3);
   l15->SetLineStyle(1);
   l15->SetLineColor(kGray+2);
   l15->Draw();

   TLine *l16 = new TLine(250,0.05,400,0.05);
   l16->SetLineWidth(3);
   l16->SetLineStyle(1);
   l16->SetLineColor(kGray+2);
   l16->Draw();

   TLine *l17 = new TLine(250,0.10,400,0.10);
   l17->SetLineWidth(3);
   l17->SetLineStyle(1);
   l17->SetLineColor(kGray+2);
   l17->Draw();

   TLine *l18 = new TLine(400,0.05,1000,0.05);
   l18->SetLineWidth(3);
   l18->SetLineStyle(1);
   l18->SetLineColor(kGray+2);
   l18->Draw();

 }




   MRRsqSignal->Draw("scat,same");


   //cv->SetGrid();
 cv->SaveAs( ("HggRazor_MRRsq_" + category + ".png").c_str());
 cv->SaveAs( ("HggRazor_MRRsq_" + category + ".pdf").c_str());

 

}

