

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
  gr->GetYaxis()->SetRangeUser(-7,5);

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

  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(0.040);
  tex->SetTextFont(42);
  tex->SetTextColor(kBlack);
  tex->DrawLatex(0.7, 0.92, "19.8 fb^{-1} (8 TeV)");
  tex->DrawLatex(0.4, 0.92, "Razor H#rightarrow#gamma#gamma Search");
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


  TLine *l1 = new TLine(14.5,-7,14.5,5);
  l1->SetLineWidth(2);
  l1->SetLineStyle(2);
  l1->SetLineColor(kBlack);
  l1->Draw();

  TLine *l2 = new TLine(17.5,-7,17.5,5);
  l2->SetLineWidth(2);
  l2->SetLineStyle(2);
  l2->SetLineColor(kBlack);
  l2->Draw();

   TLine *l3 = new TLine(20.5,-7,20.5,5);
  l3->SetLineWidth(2);
  l3->SetLineStyle(2);
  l3->SetLineColor(kBlack);
  l3->Draw();

   TLine *l4 = new TLine(35.5,-7,35.5,5);
  l4->SetLineWidth(2);
  l4->SetLineStyle(2);
  l4->SetLineColor(kBlack);
  l4->Draw();

  TLatex *HighPtLabel = new TLatex();
  HighPtLabel->SetNDC();
  HighPtLabel->SetTextAngle(90);
  HighPtLabel->SetTextSize(0.035);
  HighPtLabel->SetTextFont(42);
  HighPtLabel->SetTextColor(kBlack);
  HighPtLabel->DrawLatex(0.32,0.13,"#bf{HighPt Category}");
  HighPtLabel->Draw();

  TLatex *HbbLabel = new TLatex();
  HbbLabel->SetNDC();
  HbbLabel->SetTextAngle(90);
  HbbLabel->SetTextSize(0.035);
  HbbLabel->SetTextFont(42);
  HbbLabel->SetTextColor(kBlack);
  HbbLabel->DrawLatex(0.36,0.13,"#bf{Hbb Category}");
  HbbLabel->Draw();

  TLatex *ZbbLabel = new TLatex();
  ZbbLabel->SetNDC();
  ZbbLabel->SetTextAngle(90);
  ZbbLabel->SetTextSize(0.035);
  ZbbLabel->SetTextFont(42);
  ZbbLabel->SetTextColor(kBlack);
  ZbbLabel->DrawLatex(0.405,0.13,"#bf{Zbb Category}");
  ZbbLabel->Draw();

   TLatex *HighResLabel = new TLatex();
  HighResLabel->SetNDC();
  HighResLabel->SetTextAngle(90);
  HighResLabel->SetTextSize(0.035);
  HighResLabel->SetTextFont(42);
  HighResLabel->SetTextColor(kBlack);
  HighResLabel->DrawLatex(0.64,0.13,"#bf{HighRes Category}");
  HighResLabel->Draw();

   TLatex *LowResLabel = new TLatex();
  LowResLabel->SetNDC();
  LowResLabel->SetTextAngle(90);
  LowResLabel->SetTextSize(0.035);
  LowResLabel->SetTextFont(42);
  LowResLabel->SetTextColor(kBlack);
  LowResLabel->DrawLatex(0.88,0.13,"#bf{LowRes Category}");
  LowResLabel->Draw();

 

  cv->SaveAs("SignificanceVsBin.png");
  cv->SaveAs("SignificanceVsBin.pdf");


}
