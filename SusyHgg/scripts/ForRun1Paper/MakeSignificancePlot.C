

void MakeTFTable(string category = "highres") {

  TFile *fileData=0;
  fileData = new TFile("/afs/cern.ch/user/s/sixie/eos/cms/store/group/phys_susy/razor/run1/HggRazor/AlexRaidDisk/DiPhotonJetsBox_M60_8TeV-sherpa__Summer12_DR53X-PU_S10_START53_V7C-v1_v9__WEIGHTED.root","READ");
 
  TTree *tree = (TTree*)fileData->Get("SusyHggTree");
  
  TH1D *counts = new TH1D("counts","counts",1,0,1);

  string categoryCutString = "";
  if (category == "highpt") categoryCutString = " && ptgg >= 110 ";
  else if (category == "hbb") categoryCutString = " && ptgg < 110 && abs(mbb-125)<25 ";
  else if (category == "zbb") categoryCutString = " && ptgg < 110 && abs(mbb-91.2)<25";
  else if (category == "highres") categoryCutString = " && ptgg < 110 && abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015 ";
  else if (category == "lowres") categoryCutString = " && ptgg < 110 && abs(mbb-125)>=25 && abs(mbb-91.2)>=25 && !(pho1_sigEoE < 0.015 && pho2_sigEoE < 0.015) ";

  string sidebandCutString = " && ( (mgg > 103 && mgg < 120) || (mgg > 131 && mgg<160))";
  string signalRegionCutString = "";
  if (category == "highpt") signalRegionCutString = " && (mgg > 121.88 && mgg < 129.12)";
  if (category == "hbb") signalRegionCutString = " && (mgg > 121 && mgg < 130)";
  if (category == "zbb") signalRegionCutString = " && (mgg > 121 && mgg < 130)";
  if (category == "highres") signalRegionCutString = " && (mgg > 122.08 && mgg < 128.92)";
  if (category == "lowres") signalRegionCutString = " && (mgg > 120 && mgg < 131)";

  int NBins = 0;
  if (category == "highpt")  NBins = 15;
  if (category == "hbb")     NBins = 3;
  if (category == "zbb")     NBins = 3;
  if (category == "highres") NBins = 10;
  if (category == "lowres")  NBins = 15;


  cout << "\n\n";
  cout << "Category = " << category << "\n";
  double tf_inclusive = 0;
  double tf_inclusiveErr = 0;
  Double_t x[NBins];  
  Double_t y[NBins];
  Double_t exh[NBins];
  Double_t exl[NBins];
  Double_t eyh[NBins];
  Double_t eyl[NBins];
  for (int i=0; i<=NBins; i++) {
    x[i] = i;
    exh[i] = 0;
    exl[i] = 0;
  }
  TGraphAsymmErrors *gr =0;

  for (int BinSelection=-1; BinSelection<NBins; BinSelection++) {

    string BinCutString = "";
    if (category == "highpt") {
      if (BinSelection == -1) BinCutString = " && MR > 150 ";
      if (BinSelection == 0) BinCutString = " && MR > 150 && MR <= 200 && t1Rsq < 0.05 ";
      if (BinSelection == 1) BinCutString = " && MR > 150 && MR <= 200 && t1Rsq >= 0.05 && t1Rsq < 0.10 ";
      if (BinSelection == 2) BinCutString = " && MR > 150 && MR <= 200 && t1Rsq >= 0.10 && t1Rsq < 0.15 ";
      if (BinSelection == 3) BinCutString = " && MR > 150 && MR <= 200 && t1Rsq >= 0.15 && t1Rsq < 0.20 ";
      if (BinSelection == 4) BinCutString = " && MR > 150 && MR <= 200 && t1Rsq >= 0.20 ";
      if (BinSelection == 5) BinCutString = " && MR > 200 && MR <= 300 && t1Rsq < 0.05";
      if (BinSelection == 6) BinCutString = " && MR > 200 && MR <= 300 && t1Rsq >= 0.05 && t1Rsq < 0.10 ";
      if (BinSelection == 7) BinCutString = " && MR > 200 && MR <= 300 && t1Rsq >= 0.10 && t1Rsq < 0.15 ";
      if (BinSelection == 8) BinCutString = " && MR > 200 && MR <= 300 && t1Rsq >= 0.15 ";
      if (BinSelection == 9) BinCutString = " && MR > 300 && MR <= 500 && t1Rsq < 0.05";
      if (BinSelection == 10) BinCutString = " && MR > 300 && MR <= 500 && t1Rsq >= 0.05 && t1Rsq < 0.10 ";
      if (BinSelection == 11) BinCutString = " && MR > 300 && MR <= 500 && t1Rsq >= 0.10 ";
      if (BinSelection == 12) BinCutString = " && MR > 500 && MR <= 1600 && t1Rsq < 0.05";
      if (BinSelection == 13) BinCutString = " && MR > 500 && MR <= 1600 && t1Rsq >= 0.05 ";
      if (BinSelection == 14) BinCutString = " && MR > 1600  ";
    }
    if (category == "hbb") {
      if (BinSelection == -1) BinCutString = " && MR > 150 ";
      if (BinSelection == 0) BinCutString = " && MR > 150 && MR <= 300 && t1Rsq < 0.05 ";
      if (BinSelection == 1) BinCutString = " && MR > 150 && MR <= 300 && t1Rsq >= 0.05 ";
      if (BinSelection == 2) BinCutString = " && MR > 300 && MR <= 3000 ";
    }
    if (category == "zbb") {
      if (BinSelection == -1) BinCutString = " && MR > 150 ";
      if (BinSelection == 0) BinCutString = " && MR > 150 && MR <= 450 && t1Rsq < 0.05 ";
      if (BinSelection == 1) BinCutString = " && MR > 150 && MR <= 450 && t1Rsq >= 0.05 ";
      if (BinSelection == 2) BinCutString = " && MR > 450 && MR <= 3000 ";
    }
    if (category == "highres") {
      if (BinSelection == -1) BinCutString = " && MR > 150 ";
      if (BinSelection == 0) BinCutString = " && MR > 150 && MR <= 250 && t1Rsq < 0.05 ";
      if (BinSelection == 1) BinCutString = " && MR > 150 && MR <= 250 && t1Rsq >= 0.05 && t1Rsq < 0.10 ";
      if (BinSelection == 2) BinCutString = " && MR > 150 && MR <= 250 && t1Rsq >= 0.10 && t1Rsq < 0.15 ";
      if (BinSelection == 3) BinCutString = " && MR > 150 && MR <= 250 && t1Rsq >= 0.15 ";
      if (BinSelection == 4) BinCutString = " && MR > 250 && MR <= 400 && t1Rsq < 0.05";
      if (BinSelection == 5) BinCutString = " && MR > 250 && MR <= 400 && t1Rsq >= 0.05 && t1Rsq < 0.10 ";
      if (BinSelection == 6) BinCutString = " && MR > 250 && MR <= 400 && t1Rsq >= 0.10 ";
      if (BinSelection == 7) BinCutString = " && MR > 400 && MR <= 1400 && t1Rsq < 0.05";
      if (BinSelection == 8) BinCutString = " && MR > 400 && MR <= 1400 && t1Rsq >= 0.05  ";
      if (BinSelection == 9) BinCutString = " && MR > 1400 ";
    }
    if (category == "lowres") {
      if (BinSelection == -1) BinCutString = " && MR > 150 ";
      if (BinSelection == 0) BinCutString = " && MR > 150 && MR <= 200 && t1Rsq < 0.05 ";
      if (BinSelection == 1) BinCutString = " && MR > 150 && MR <= 200 && t1Rsq >= 0.05 && t1Rsq < 0.10 ";
      if (BinSelection == 2) BinCutString = " && MR > 150 && MR <= 200 && t1Rsq >= 0.10 && t1Rsq < 0.15 ";
      if (BinSelection == 3) BinCutString = " && MR > 150 && MR <= 200 && t1Rsq >= 0.15 && t1Rsq < 0.20 ";
      if (BinSelection == 4) BinCutString = " && MR > 150 && MR <= 200 && t1Rsq >= 0.20 ";
      if (BinSelection == 5) BinCutString = " && MR > 200 && MR <= 250 && t1Rsq < 0.05";
      if (BinSelection == 6) BinCutString = " && MR > 200 && MR <= 250 && t1Rsq >= 0.05 && t1Rsq < 0.10 ";
      if (BinSelection == 7) BinCutString = " && MR > 200 && MR <= 250 && t1Rsq >= 0.10 && t1Rsq < 0.15 ";
      if (BinSelection == 8) BinCutString = " && MR > 200 && MR <= 250 && t1Rsq >= 0.15 ";
      if (BinSelection == 9) BinCutString = " && MR > 250 && MR <= 400 && t1Rsq < 0.05";
      if (BinSelection == 10) BinCutString = " && MR > 250 && MR <= 400 && t1Rsq >= 0.05 && t1Rsq < 0.10 ";
      if (BinSelection == 11) BinCutString = " && MR > 250 && MR <= 400 && t1Rsq >= 0.10 ";
      if (BinSelection == 12) BinCutString = " && MR > 400 && MR <= 1200 && t1Rsq < 0.05";
      if (BinSelection == 13) BinCutString = " && MR > 400 && MR <= 1200 && t1Rsq >= 0.05 ";
      if (BinSelection == 14) BinCutString = " && MR > 1200  ";
    }

    double alpha = 1.- 0.682689492;

    tree->Draw("0.5>>counts",("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && pho1_pass_iso && pho2_pass_iso " + categoryCutString + signalRegionCutString + BinCutString).c_str(),"");
    double SignalRegionCount = counts->GetBinContent(1);
    double SignalRegionCountErr = ROOT::Math::gamma_quantile_c( alpha/2, SignalRegionCount+1, 1) - SignalRegionCount;

    tree->Draw("0.5>>counts",("ptgg > 20 && abs(pho1_eta) <1.44 && abs(pho2_eta)<1.44 && (pho1_pt>40||pho2_pt>40)&&pho1_pt>25 && pho2_pt>25 && pho1_pass_id && pho2_pass_id && pho1_pass_iso && pho2_pass_iso " + categoryCutString + sidebandCutString + BinCutString).c_str(),"");
    double SidebandCount = counts->GetBinContent(1);
    double SidebandCountErr = ROOT::Math::gamma_quantile_c( alpha/2, SidebandCount+1, 1) - SidebandCount;


    double transferFactor = SignalRegionCount/SidebandCount;
    double transferFactorErr = 0;
    if (SignalRegionCount > 0) {
      transferFactorErr = transferFactor*sqrt( pow(SignalRegionCountErr/SignalRegionCount,2) + pow(SidebandCountErr/SidebandCount,2));
    } else {
      transferFactorErr = SignalRegionCountErr / SidebandCount ;
    }

    cout << "Bin " << BinSelection << " : TransferFactor = " << SignalRegionCount << "+/-" <<  SignalRegionCountErr << " / " << SidebandCount << "+/-" << SidebandCountErr << " = " 
	 << transferFactor << " +/- " << transferFactorErr
	 << "\n";

    if (BinSelection == -1) {
      tf_inclusive = transferFactor;
      tf_inclusiveErr = transferFactorErr;
    } else {
      y[BinSelection] = transferFactor;
      eyh[BinSelection] = transferFactorErr;
      if (SignalRegionCount > 0) {
	eyl[BinSelection] = transferFactorErr;
      } else {
	eyl[BinSelection] = 0;
      }
    }

  }

  TCanvas *cv = new TCanvas("cv","cv",800,600);

  gr = new TGraphAsymmErrors(NBins,x,y,exl,exh,eyl,eyh);
  gr->SetTitle("");
  gr->Draw("ap");
  gr->SetLineWidth(2);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(1);
  gr->GetXaxis()->SetTitle("Bin Number");
  gr->GetYaxis()->SetTitle("Sideband Extrapolation Transfer Factor");
  gr->GetYaxis()->SetTitleOffset(1.3);

  if (category == "highpt")  gr->GetYaxis()->SetRangeUser(0.0, 0.5);
  if (category == "hbb")     gr->GetYaxis()->SetRangeUser(0.16, 0.30);
  if (category == "zbb")     gr->GetYaxis()->SetRangeUser(0.16, 0.30);
  if (category == "highres") gr->GetYaxis()->SetRangeUser(0.0, 0.35);
  if (category == "lowres")  gr->GetYaxis()->SetRangeUser(0.0, 1.0);

  double binx[NBins+1];
  for (int i=0; i<NBins+1; ++i)  binx[i] = i-0.5;
  gr->GetXaxis()->Set(NBins,  binx);
  for (int i=0; i<NBins; ++i) gr->GetXaxis()->SetBinLabel(i+1, Form("%d",i+1));
  gr->GetXaxis()->SetLabelSize(0.07);
  gr->GetXaxis()->SetTitleSize(0.05);
  gr->GetXaxis()->SetTitleOffset(0.8); 
  
  TBox *box = new TBox(gr->GetXaxis()->GetXmin(),tf_inclusive - tf_inclusiveErr, gr->GetXaxis()->GetXmax(), tf_inclusive + tf_inclusiveErr);
  box->SetFillColor(kBlue);
  box->SetFillStyle(3001);
  box->Draw();

 TLegend *legend = new TLegend(0.30,0.70,0.80,0.84);
 legend->SetTextSize(0.04);
 legend->SetBorderSize(0);
 legend->SetFillStyle(0);
 legend->AddEntry(box , "Transfer Factor from Inclusive Region" , "F");
 legend->Draw();

  cv->SaveAs( ("TransferFactor_"+category+".png").c_str());
  cv->SaveAs( ("TransferFactor_"+category+".pdf").c_str());

}





void MakeSignificancePlot() {

  int NBins = 47;
  Double_t x[NBins];  
  Double_t y[NBins];
  for (int i=0; i<NBins; ++i)  x[i] = i;
  
  //highPt Category
  y[0] = 0.5;
  y[1] = -0.1;
  y[2] = 0.2;
  y[3] = 0.0;
  y[4] = 0.0;
  y[5] = 1.9;
  y[6] = 1.3;
  y[7] = 0.6;
  y[8] = -0.1;
  y[9] = -1.3;
  y[10] = 0.2;
  y[11] = 0.3;
  y[12] = -0.7;
  y[13] = -0.7;
  y[14] = -0.2;
  //hbb category  
  y[15] = -0.8;
  y[16] = -0.5;
  y[17] = -0.4;
  //zbb category
  y[18] = -0.2;
  y[19] = 0.2;
  y[20] = -1.1;
  //highres category
  y[21] = 0.3;
  y[22] = 0.7;
  y[23] = 0.4;
  y[24] = -0.3;
  y[25] = 0.6;
  y[26] = 1.1;
  y[27] = 0.2;
  y[28] = 0.7;
  y[30] = 2.9;
  y[31] = -0.1;
  //low res category
  y[32] = 0.8;
  y[33] = 0.3;
  y[34] = 0.8;
  y[35] = 1.2;
  y[36] = 0.0;
  y[37] = 0.3;
  y[38] = 0.6;
  y[39] = -0.9;
  y[40] = -0.4;
  y[41] = -0.6;
  y[42] = -1.3;
  y[43] = 0.5;
  y[44] = -1.6;
  y[45] = 1.0;
  y[46] = -0.3;

  TCanvas *cv = new TCanvas("cv","cv",800,600);

  TGraph *gr =0;
  gr = new TGraphAsymmErrors(NBins,x,y);
  gr->SetTitle("");
  gr->Draw("ap");
  gr->SetLineWidth(2);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(1);
  gr->GetXaxis()->SetTitle("Bin Number");
  gr->GetYaxis()->SetTitle("Observed Significance");
  gr->GetXaxis()->SetTitleSize(0.05);
  gr->GetXaxis()->SetTitleOffset(0.8);
  gr->GetYaxis()->SetTitleSize(0.05);
  gr->GetYaxis()->SetTitleOffset(0.8);
  gr->GetYaxis()->SetRangeUser(-5,5);

  TBox *TwoSigmaBand = new TBox(gr->GetXaxis()->GetXmin()+0.1, -2, gr->GetXaxis()->GetXmax(), 2);
  TwoSigmaBand->SetFillColor(kGreen);
  //box->SetFillStyle(3001);
  TwoSigmaBand->Draw();

  TBox *OneSigmaBand = new TBox(gr->GetXaxis()->GetXmin()+0.1, -1, gr->GetXaxis()->GetXmax(), 1);
  OneSigmaBand->SetFillColor(kYellow);
  //box->SetFillStyle(3001);
  OneSigmaBand->Draw();

  TLine *l = new TLine(gr->GetXaxis()->GetXmin(),0,gr->GetXaxis()->GetXmax(),0);
  l->SetLineWidth(2);
  l->SetLineColor(kBlue);
  l->Draw();

  gr->Draw("P");

  // Double_t binx[NBins];  
  // for (int i=0; i<NBins+1; ++i)  binx[i] = i-0.5;
  // gr->GetXaxis()->Set(NBins,  binx);
  // for (int i=0; i<NBins; ++i) gr->GetXaxis()->SetBinLabel(i+1, Form("%d",i+1));
  // gr->GetXaxis()->SetLabelSize(0.07);
  // gr->GetXaxis()->SetTitleSize(0.05);
  // gr->GetXaxis()->SetTitleOffset(0.8); 

  cv->SaveAs("SignificanceVsBin.png");


}
