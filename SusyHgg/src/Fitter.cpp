#include "Fitter.hpp"

#include "TObjArray.h"
#include "TMath.h"

#define NUM_CPU 1

Fitter::Fitter(TString inputFileName,TString outputFileName) {
  inputFile = new TFile(inputFileName);
  inputWs = (RooWorkspace*)inputFile->Get("susy_hgg_workspace");

  outputFile = new TFile(outputFileName,"RECREATE");
  outputWs = new RooWorkspace("susy_hgg_workspace","");

  TObjArray *mc_names_array = (TObjArray*)inputFile->Get("mc_name_array");
  for(int i=0;i<mc_names_array->GetEntries();i++) {
    this->addMcName( ((TObjString*)mc_names_array->At(i))->String() );
  }

  if(inputWs->var("N_data")!=0) {
    lumi = inputWs->var("N_data")->getVal();
    outputWs->import( *inputWs->var("N_data") );
  }else{
    std::cout << "Cannot Determine Lumi From Workspace" << std::endl;
    lumi=0;
  }
  DefineBinning();
}

Fitter::~Fitter() {
  inputFile->Close();
  outputFile->Close();
  delete inputWs;
  delete outputWs;

}

void Fitter::Run() {
  MakeSMTot();

  //
  defineCats();
  setupCats();

//   saveAll();
//   return;


  for(int catIndex=0;catIndex < cats.size();catIndex++) {
    buildHistograms(catIndex);
    
    runFits(catIndex);
    computeScaleFactor(catIndex);

    save_histograms[Form("data_%s_Sideband",cats.at(catIndex).Data())]->Scale( signal_integrals.at(catIndex).first/save_histograms[Form("data_%s_Sideband",cats.at(catIndex).Data())]->Integral() );
    


    buildSubtractedHistograms(catIndex);
    
  }
  for(auto& it: save_histograms) {
    
    TH2F* tmp = getSignalRegionHistogram(*it.second,it.first+"_SigRegions");
    save_SigRegion_histograms[tmp->GetName()] = tmp;
  }
  SMFit();
  SMFitSidebandSub();

  for(auto& it: sm_higgs_names) {
    outputWs->import(*buildSimultaneousPdf(it+"_%s_Signal_SigRegions_pdf"));
  }
  if(sm_higgs_names.size()>0) outputWs->import(*buildSimultaneousPdf("SMTot_%s_Signal_SigRegions_pdf"));

  for(auto& it: sms_names) {
    outputWs->import(*buildSimultaneousPdf(it+"_%s_Signal_SigRegions_pdf"));
  }

  saveAll();
}

void Fitter::buildHistograms(int catIndex) {
  std::cout << "buildHistograms " << catIndex << std::endl;

  if(processData) {
    save_histograms[Form("data_%s_Signal",cats.at(catIndex).Data())] = build_histogram("data",catIndex,kSignal);
    save_histograms[Form("data_%s_Sideband",cats.at(catIndex).Data())] = build_histogram("data",catIndex,kSideband);
    save_histograms[Form("data_%s_All",cats.at(catIndex).Data())] = build_histogram("data",catIndex,kAll);
  }
  for(auto& mc_name : sm_higgs_names) {
    std::cout << mc_name << std::endl;
    RooRealVar *n = inputWs->var("N_"+mc_name);
    float w = xsecs[mc_name] * 2.28E-3 * lumi*1000/n->getVal();
    if(mc_name == "SMTot") w=1;
    save_histograms[Form("%s_%s_Signal",mc_name.Data(),cats.at(catIndex).Data())] = build_histogram(mc_name,catIndex,kSignal,w);
    outputWs->import(*n);
  }

  for(auto& mc_name : sms_names) {
    std::cout << mc_name << std::endl;
    RooRealVar *n = inputWs->var("N_"+mc_name);
    save_histograms[Form("%s_%s_Signal",mc_name.Data(),cats.at(catIndex).Data())] = build_histogram(mc_name,catIndex,kSignal,1.0 * 2.28E-3 * lumi * 1000/n->getVal());      
    outputWs->import(*n);
  }
  
 }


void Fitter::runFits(int catIndex) {
  if(!processData) return;
  std::cout << "runFits " << catIndex << std::endl;

  RooRealVar *mgg = inputWs->var("mgg");

  TString tag = cats.at(catIndex);

  //mgg->setRange(110,150);

  RooRealVar a1(Form("%s_alpha1",tag.Data()),"",-0.1,-1.,0.);
  RooRealVar a2(Form("%s_alpha2",tag.Data()),"",-0.1,-1.,0.);
  RooRealVar f (Form("%s_f",tag.Data()),"",0.1,0.,1.);

  RooExponential exp1(Form("%s_exp1",tag.Data()),"",*mgg,a1);
  RooExponential exp2(Form("%s_exp2",tag.Data()),"",*mgg,a2);
  RooAddPdf      dexp(Form("%s_dexp",tag.Data()),"",RooArgList(exp1,exp2),f);

  RooRealVar Nbkg(Form("%s_Nbkg",tag.Data()),"",1000,0,1e9);

  RooExtendPdf  bkgModel(Form("%s_bkgModel",tag.Data()),"",dexp,Nbkg);

  RooDataSet *ds = (RooDataSet*)outputWs->data("data_Combined")->reduce(Form("evtcat==evtcat::%s",tag.Data()));
  bkgModel.fitTo(*ds,RooFit::Save(kFALSE),RooFit::Strategy(0),RooFit::NumCPU(NUM_CPU));
  RooFitResult *res =   bkgModel.fitTo(*ds,RooFit::Save(kTRUE),RooFit::Strategy(2),RooFit::NumCPU(NUM_CPU));
  res->SetName( Form("%s_fitResult",tag.Data()) );

  outputWs->import(bkgModel);
  outputWs->import(*res);
}

void Fitter::computeScaleFactor(int catIndex) {
  if(!processData) return;
  std::cout << "computeScaleFactor " << catIndex << std::endl;
  TString tag = cats.at(catIndex);
  RooRealVar *a1 = outputWs->var( Form("%s_alpha1",tag.Data()) );
  RooRealVar *a2 = outputWs->var( Form("%s_alpha2",tag.Data()) );
  RooRealVar *f = outputWs->var( Form("%s_f",tag.Data()) );
  RooRealVar *Nbkg = outputWs->var( Form("%s_Nbkg",tag.Data()) );

  RooRealVar *mgg = outputWs->var("mgg");

  assert(a1!=0 && a2!=0 && f!=0);
  
  RooRealVar min("min","",mgg->getMin());
  RooRealVar max("max","",mgg->getMax());

  fitInfo thisFitInfo = per_cat_fit_ranges.at(catIndex);

  RooRealVar min_side_low("min_side_low","",thisFitInfo.sideband_low_min);
  RooRealVar max_side_low("max_side_low","",thisFitInfo.sideband_low_max);
  RooRealVar min_side_high("min_side_high","",thisFitInfo.sideband_high_min);
  RooRealVar max_side_high("max_side_high","",thisFitInfo.sideband_high_max);
  RooRealVar min_signal("min_signal","",thisFitInfo.signal_min);
  RooRealVar max_signal("max_signal","",thisFitInfo.signal_max);
    
  RooFormulaVar integral_total("integral_total_"+tag,"","(@2/@0*(exp(@0*@4)-exp(@0*@3))+(1-@2)/@1*(exp(@1*@4)-exp(@1*@3)))",RooArgList(*a1,*a2,*f,min,max));
  RooFormulaVar integral_side_low("integral_side_low_"+tag,"","@5*(@2/@0*(exp(@0*@4)-exp(@0*@3))+(1-@2)/@1*(exp(@1*@4)-exp(@1*@3)))/@6",RooArgList(*a1,*a2,*f,min_side_low,max_side_low,*Nbkg,integral_total));
  RooFormulaVar integral_side_high("integral_side_high_"+tag,"","@5*(@2/@0*(exp(@0*@4)-exp(@0*@3))+(1-@2)/@1*(exp(@1*@4)-exp(@1*@3)))/@6",RooArgList(*a1,*a2,*f,min_side_high,max_side_high,*Nbkg,integral_total));
  RooFormulaVar integral_signal("integral_signal_"+tag,"","@5*(@2/@0*(exp(@0*@4)-exp(@0*@3))+(1-@2)/@1*(exp(@1*@4)-exp(@1*@3)))/@6",RooArgList(*a1,*a2,*f,min_signal,max_signal,*Nbkg,integral_total));

  RooFitResult *fitres = (RooFitResult*)outputWs->obj( Form("%s_fitResult",tag.Data()) );

  RooFormulaVar ratio("ratio_"+tag,"","@0/(@1+@2)",RooArgList(integral_signal,integral_side_low,integral_side_high));

  std::cout << Nbkg->GetName() << std::endl;
  std::cout << ratio.getVal() << std::endl << ratio.getPropagatedError(*fitres) << std::endl;

  RooRealVar bkg_scale(Form("bkg_scale_factor_%s",tag.Data()),"",ratio.getVal());
  bkg_scale.setError( ratio.getPropagatedError(*fitres) ); //deal with the correlations correctly

  RooRealVar sig_int(Form("signal_fit_int_%s",tag.Data()),"",integral_signal.getVal());
  sig_int.setError(integral_signal.getPropagatedError(*fitres));

  RooRealVar side_low_int(Form("sideband_low_fit_int_%s",tag.Data()),"",integral_side_low.getVal());
  side_low_int.setError(integral_side_low.getPropagatedError(*fitres));
  
  RooRealVar side_high_int(Form("sideband_high_fit_int_%s",tag.Data()),"",integral_side_high.getVal());
  side_high_int.setError(integral_side_high.getPropagatedError(*fitres));

  RooFormulaVar integral_side("integral_side_"+tag,"@0+@1",RooArgList(side_high_int,side_low_int));

  RooRealVar side_int(Form("sideband_fit_int_%s",tag.Data()),"",integral_side.getVal());
  side_int.setError(integral_side.getPropagatedError(*fitres));

  outputWs->import(bkg_scale);
  outputWs->import(ratio);

  outputWs->import(sig_int);
  outputWs->import(side_low_int);
  outputWs->import(side_high_int);
  outputWs->import(side_int);

  sideband_integrals.at(catIndex) = std::make_pair(side_int.getVal(),side_int.getError());
  signal_integrals.at(catIndex)   = std::make_pair(sig_int.getVal(), sig_int.getError());
  sideband_to_signal_scale_factors.at(catIndex) = std::make_pair(bkg_scale.getVal(),bkg_scale.getError());



  //use createIntegral
  RooAbsPdf *bkg_fit = outputWs->pdf( Form("%s_bkgModel",tag.Data()) );
  
  mgg->setRange("sig_region",thisFitInfo.signal_min,thisFitInfo.signal_max);

  RooAbsReal *sig_region_integral = bkg_fit->createIntegral(*mgg,RooFit::NormSet(*mgg),RooFit::Range("sig_region"));
  

  RooRealVar sri(Form("signal_fit_int_%s_createIntegral",tag.Data()), "",sig_region_integral->getVal());
  //sri.setError(sig_region_integral->getPropogatedError(*fitres));


  outputWs->import(sri);
  assert(sig_region_integral->getVal() < 0.9);
  assert(sri.getVal() < 0.9);
}

void Fitter::buildSubtractedHistograms(int catIndex) {
  if(!processData) return;
  std::cout << "buildSubtractedHistogram " << catIndex << std::endl;
  TH2F* signal   = save_histograms[ "data_"+cats.at(catIndex)+"_Signal"];
  TH2F* sideband = save_histograms[ "data_"+cats.at(catIndex)+"_Sideband"];

  TH2F* bkg_sub = (TH2F*)signal->Clone("data_"+cats.at(catIndex)+"_Signal_sidebandSub");
  //bkg_sub->Add(sideband,-1*sideband_to_signal_scale_factors.at(catIndex).first);
  std::cout << "Scaling: " << catIndex << "\t\t" << signal_integrals.at(catIndex).first << "\t" << sideband->Integral() << std::endl;
  bkg_sub->Add(sideband,-1);

  for(int iX=1;iX < bkg_sub->GetNbinsX();iX++) {
    for(int iY=1;iY < bkg_sub->GetNbinsY();iY++) {
      bkg_sub->SetBinError(iX,iY,TMath::Sqrt(signal->GetBinContent(iX,iY)+sideband->GetBinContent(iX,iY)+1));
    }
  }

  save_histograms["data_"+cats.at(catIndex)+"_Signal_sidebandSub"] = bkg_sub;
}

void Fitter::SMFit() {
  std::cout << "SMFit" << std::endl;
  if(!processData) return;
  std::map<std::string,RooDataHist*> hists;
  RooSimultaneous sideband("data_Sideband_SigRegions_norm_pdf","data_Sideband_SigRegions_norm_pdf",*roocat);
  RooSimultaneous SMTot("SMTot_Signal_SigRegions_norm_pdf","SMTot_Signal_SigRegions_norm_pdf",*roocat);

  for(int catIndex=0;catIndex < cats.size();catIndex++) {
    std::string name = cats.at(catIndex).Data();

    RooDataHist *data =  (RooDataHist*)outputWs->data( Form("data_%s_Signal_SigRegions_norm_hist",name.c_str()) );
    assert(data != 0 && Form("data named data_%s_Signal_SigRegions_norm_hist NOT FOUND",name.c_str()));
    hists[name] = data;

    RooAbsPdf *side = outputWs->pdf( Form("data_%s_Sideband_SigRegions_norm_pdf",name.c_str()) );
    sideband.addPdf(*side,name.c_str());

    RooAbsPdf *sm = outputWs->pdf( Form("SMTot_%s_Signal_SigRegions_norm_pdf",name.c_str()) );
    SMTot.addPdf(*sm,name.c_str());


    //get the value of the integral of the pdf in the signal region
    RooRealVar *signal_int = outputWs->var(Form("signal_fit_int_%s",name.c_str()));
    RooRealVar signal_int_mean  (Form("N_FIT_data_%s_Sideband_SigRegions_norm_pdf__Constraint_SigRegion_mean",name.c_str()),"",signal_int->getVal());
    RooRealVar signal_int_sigma (Form("N_FIT_data_%s_Sideband_SigRegions_norm_pdf__Constraint_SigRegion_sigma",name.c_str()),"",signal_int->getError());


    RooRealVar N_side_cat(Form("N_FIT_data_%s_Sideband_SigRegions_norm_pdf",name.c_str()),"",signal_int->getVal(),signal_int->getVal()-4*signal_int->getError(),signal_int->getVal()+4*signal_int->getError());
    RooGaussian signal_int_constraint(Form("N_FIT_data_%s_Sideband_SigRegions_norm_pdf__Constraint_SigRegion",name.c_str()),"",N_side_cat,signal_int_mean,signal_int_sigma);


    RooRealVar N_sm_cat(Form("N_FIT_SMTot_%s_Sideband_SigRegions_norm_pdf",name.c_str()),"",100,0,2e4);
    //RooExtendPdf side_cat(Form("data_%s_Sideband_SigRegions_norm_pdf_FIT",name.c_str()),"",*side,N_side_cat);
    RooAddPdf sum_pdf(Form("data_plus_SMTot_%s_Sideband_SigRegions_norm_pdf",name.c_str()),"",RooArgList(*side,*sm),RooArgList(N_side_cat,N_sm_cat));

    sum_pdf.fitTo(*data,RooFit::Save(kFALSE),RooFit::Strategy(0),RooFit::NumCPU(NUM_CPU),RooFit::Extended(kFALSE),RooFit::Range("all"),RooFit::ExternalConstraints(signal_int_constraint));
    RooFitResult* res = sum_pdf.fitTo(*data,RooFit::Save(kTRUE),RooFit::Strategy(2),RooFit::NumCPU(NUM_CPU),RooFit::Extended(kFALSE),RooFit::Range("all"),RooFit::ExternalConstraints(signal_int_constraint)); 
    res->SetName( Form("data_plus_SMTot_%s_Sideband_SigRegions_norm_FITRESULT",name.c_str()) );
    outputWs->import(*res);
    outputWs->import(sum_pdf);
    

  }
  //build the multicategory data set
  RooRealVar *sig_region = outputWs->var("signal_region");

  RooDataHist data_tot("data_Signal_SigRegions_norm_hist","",*sig_region,*roocat,hists);

  RooRealVar NSM("N_FITTED_SMTot","N_FITTED_SMTot",100,0,2e4);
  RooRealVar NSide("N_FITTED_Sideband","N_FITTED_Sideband",10000,0,1e9);

  RooAddPdf simultaneous_sum("data_plus_SMTot_Sideband_SigRegions_norm_pdf","",RooArgList(sideband,SMTot),RooArgList(NSide,NSM));

  simultaneous_sum.fitTo(data_tot,RooFit::Save(kFALSE),RooFit::Strategy(0),RooFit::NumCPU(NUM_CPU),RooFit::Extended(kFALSE),RooFit::Range("all"));
  RooFitResult* res = simultaneous_sum.fitTo(data_tot,RooFit::Save(kTRUE),RooFit::Strategy(2),RooFit::NumCPU(NUM_CPU),RooFit::Extended(kFALSE),RooFit::Range("all"));
  res->SetName( "data_plus_SMTot_Sideband_SigRegions_norm_FITRESULT" );

  outputWs->import(simultaneous_sum);
  outputWs->import(data_tot);
  outputWs->import(*res);

}

void Fitter::SMFitSidebandSub() {
  std::cout << "SMFitSidebandSub" << std::endl;
  if(!processData) return;
  std::map<std::string,RooDataHist*> hists;
  RooSimultaneous sideband("data_Sideband_sidebandSub_SigRegions_norm_pdf","data_Sideband_SigRegions_norm_pdf",*roocat);
  RooSimultaneous SMTot("SMTot_Signal_sidebandSub_SigRegions_norm_pdf","SMTot_Signal_SigRegions_norm_pdf",*roocat);

  for(int catIndex=0;catIndex < cats.size();catIndex++) {
    std::string name = cats.at(catIndex).Data();

    RooDataHist *data =  (RooDataHist*)outputWs->data( Form("data_%s_Signal_sidebandSub_SigRegions_norm_hist",name.c_str()) );
    assert(data != 0 && Form("data named data_%s_Signal_SigRegions_norm_hist NOT FOUND",name.c_str()));
    hists[name] = data;

    RooAbsPdf *side = outputWs->pdf( Form("data_%s_Sideband_SigRegions_norm_pdf",name.c_str()) );
    sideband.addPdf(*side,name.c_str());

    RooAbsPdf *sm = outputWs->pdf( Form("SMTot_%s_Signal_SigRegions_norm_pdf",name.c_str()) );
    SMTot.addPdf(*sm,name.c_str());

    //get the value of the integral of the pdf in the signal region
    RooRealVar *signal_int = outputWs->var(Form("signal_fit_int_%s",name.c_str()));
    RooRealVar signal_int_mean  (Form("N_FIT_data_%s_Sideband_sidebandSub_SigRegions_norm_pdf__Constraint_SigRegion_mean",name.c_str()),"",0);
    RooRealVar signal_int_sigma (Form("N_FIT_data_%s_Sideband_sidebandSub_SigRegions_norm_pdf__Constraint_SigRegion_sigma",name.c_str()),"",signal_int->getError());


    RooRealVar N_side_cat(Form("N_FIT_data_%s_Sideband_sidebandSub_SigRegions_norm_pdf",name.c_str()),"",0,-4*signal_int->getError(),4*signal_int->getError());
    RooGaussian signal_int_constraint(Form("N_FIT_data_%s_Sideband_sidebandSub_SigRegions_norm_pdf__Constraint_SigRegion",name.c_str()),"",N_side_cat,signal_int_mean,signal_int_sigma);


    RooRealVar N_sm_cat(Form("N_FIT_SMTot_%s_Sideband_sidebandSub_SigRegions_norm_pdf",name.c_str()),"",100,0,2e4);
    //RooExtendPdf side_cat(Form("data_%s_Sideband_sidebandSub_SigRegions_norm_pdf_FIT",name.c_str()),"",*side,N_side_cat);
    RooAddPdf sum_pdf(Form("data_plus_SMTot_%s_Sideband_sidebandSub_SigRegions_norm_pdf",name.c_str()),"",RooArgList(*side,*sm),RooArgList(N_side_cat,N_sm_cat));

    sum_pdf.fitTo(*data,RooFit::Save(kFALSE),RooFit::Strategy(0),RooFit::NumCPU(NUM_CPU),RooFit::Extended(kFALSE),RooFit::Range("all"),RooFit::ExternalConstraints(signal_int_constraint));
    RooFitResult* res = sum_pdf.fitTo(*data,RooFit::Save(kTRUE),RooFit::Strategy(2),RooFit::NumCPU(NUM_CPU),RooFit::Extended(kFALSE),RooFit::Range("all"),RooFit::ExternalConstraints(signal_int_constraint)); 
    res->SetName( Form("data_plus_SMTot_%s_Sideband_sidebandSub_SigRegions_norm_FITRESULT",name.c_str()) );
    outputWs->import(*res);
    outputWs->import(sum_pdf);
    

  }
  //build the multicategory data set
  RooRealVar *sig_region = outputWs->var("signal_region");

  RooDataHist data_tot("data_Signal_sidebandSub_SigRegions_norm_hist","",*sig_region,*roocat,hists);

  RooRealVar NSM("N_FITTED_sidebandSub_SMTot","N_FITTED_SMTot",100,0,2e4);
  RooRealVar NSide("N_FITTED_sidebandSub_Sideband","N_FITTED_Sideband",10000,-1e9,1e9);

  RooAddPdf simultaneous_sum("data_plus_SMTot_Sideband_sidebandSub_SigRegions_norm_pdf","",RooArgList(sideband,SMTot),RooArgList(NSide,NSM));

  simultaneous_sum.fitTo(data_tot,RooFit::Save(kFALSE),RooFit::Strategy(0),RooFit::NumCPU(NUM_CPU),RooFit::Extended(kFALSE),RooFit::Range("all"));
  RooFitResult* res = simultaneous_sum.fitTo(data_tot,RooFit::Save(kTRUE),RooFit::Strategy(2),RooFit::NumCPU(NUM_CPU),RooFit::Extended(kFALSE),RooFit::Range("all"));
  res->SetName( "data_plus_SMTot_Sideband_sidebandSub_SigRegions_norm_FITRESULT" );

  outputWs->import(simultaneous_sum);
  outputWs->import(data_tot);
  outputWs->import(*res);

}

void Fitter::buildRooHistograms(TString binning) {
  
}

TH2F* Fitter::getSignalRegionHistogram(const TH2F& hist,const char* name) {
 
  const int nXbins =5;
  double xBins[nXbins] = {0,200,400,1000,2000};
  const int nYbins =6;
  double yBins[nYbins] = {0,0.05,0.1,0.2,0.5,1.0};
 
  /*
  const int nXbins =5;
  double xBins[nXbins] = {0,200,400,1000,2000};
  const int nYbins =5;
  double yBins[nYbins] = {0,0.1,0.2,0.5,1.0};
  */
  TH2F *signalHist = new TH2F(name,"",nXbins-1,xBins,nYbins-1,yBins);
  
  for(int iX=1;iX<hist.GetNbinsX();iX++) {
    for(int iY=1;iY<hist.GetNbinsY();iY++) {
      signalHist->Fill( hist.GetXaxis()->GetBinCenter(iX), hist.GetYaxis()->GetBinCenter(iY), hist.GetBinContent(iX,iY) );
    }
  }
  
  RooRealVar sigregion("signal_region","",-0.5,nXbins*nYbins-0.5);
  sigregion.setBins(nXbins*nYbins);
  
  RooDataHist norm_rdh( Form("%s_norm_hist",name),"",sigregion);
  RooDataHist rdh( Form("%s_hist",name),"",sigregion);
  
  for(int iX=0;iX<nXbins;iX++) {
    for(int iY=0;iY<nYbins;iY++) {
      
      sigregion.setVal( iX*(nYbins-1)+iY );
      if( (iX==0 && iY < 3) ||
	  (iX==1 && iY < 2) ||
 	  (iX==2 && iY < 1) )  {
 	norm_rdh.set(sigregion,signalHist->GetBinContent(iX+1,iY+1));
      }
      rdh.set(sigregion,signalHist->GetBinContent(iX+1,iY+1));
      
    }
  }
  
  RooHistPdf norm_rhp( Form("%s_norm_pdf",name), "", sigregion, norm_rdh);
  RooHistPdf rhp( Form("%s_pdf",name), "", sigregion, rdh);
  
  
  outputWs->import(sigregion);
  outputWs->import(rhp);
  outputWs->import(norm_rhp);
  
  return signalHist;
}

void Fitter::addMcName(TString name) {
  if(name.Index("_H_")!=-1) sm_higgs_names.push_back(name);
  else sms_names.push_back(name);
}

std::pair<float,float> Fitter::getMeanAndSigEff(const RooAbsData& data,RooRealVar& var) {
  float mean = data.mean(var);

  float total = data.sumEntries();

  assert(total!=0 && "A category has 0 SM higgs events!");
  
  float thisR=0;
  float se=0.1;
  const float step_size=0.01;
  for(;se<5.0;se+=step_size) {
    thisR = data.sumEntries( Form( "mgg > %f-%f && mgg < %f+%f",mean,se,mean,se) ) / total;
    if(thisR >= 0.68269) break;
  }
  float lastR = data.sumEntries( Form( "mgg > %f-%f && mgg < %f+%f",mean,se-step_size,mean,se-step_size) ) / total;
  
  float sigeff = (thisR-0.68269)/(thisR-lastR)*step_size+se-step_size;

  return std::make_pair(mean,sigeff);
}

void Fitter::SetupSignalRegions() {
  RooAbsData * smtot = outputWs->data("SMTot_Combined");
  RooRealVar *mgg    = outputWs->var("mgg");
  for(int i_cat=0;i_cat < cats.size(); i_cat++) {
    TString cat = cats.at(i_cat);
    std::cout << cat.Data() << std::endl;
    RooAbsData * cat_data = smtot->reduce( Form("evtcat==evtcat::%s",cat.Data()) );
    assert(cat_data != 0 && cat.Data());
    std::pair<float,float> meanAndSigma = getMeanAndSigEff(*cat_data,*mgg);
    RooRealVar cat_mean( Form("%s_SMTot_mean",cat.Data()), "", meanAndSigma.first );
    RooRealVar cat_sig ( Form("%s_SMTot_sigEff",cat.Data()), "", meanAndSigma.second );

    per_cat_fit_ranges.at(i_cat).signal_min = meanAndSigma.first-meanAndSigma.second*2;
    per_cat_fit_ranges.at(i_cat).signal_max = meanAndSigma.first+meanAndSigma.second*2;
    outputWs->import(cat_mean);
    outputWs->import(cat_sig);
  }
}


TH2F* Fitter::build_histogram(TString name,int catIndex,region reg,float weight) {
  RooRealVar* MR = outputWs->var("MR");
  RooRealVar* R  = outputWs->var("Rsq");

  MR->setRange(0,2000.);
  MR->setBins(50);

  R->setRange(0,1.0);
  R->setBins(25);

  TString fullname = Form("%s_%s",name.Data(),cats.at(catIndex).Data());

  TString selectionString = "";

  fitInfo *info = & (per_cat_fit_ranges.at(catIndex));

  bool isSMS=false;
  if(name.Index("sms")!=-1) isSMS=true;

  switch(reg) {
  case kSignal:
    fullname+="_Signal";
    selectionString = Form("mgg > %f && mgg < %f",info->signal_min+isSMS,info->signal_max+isSMS);
    break;
  case kSideband:
    fullname+="_Sideband";
    selectionString = Form("(mgg > %f && mgg < %f) || (mgg > %f && mgg < %f)",
			   info->sideband_low_min+isSMS,  info->sideband_low_max+isSMS,
			   info->sideband_high_min+isSMS, info->sideband_high_max+isSMS);
    break;
  case kAll:
    fullname+="_All";
    selectionString = "1";
    break;
  default:
    throw new std::runtime_error( Form("invalid region %d",reg) );
  }


  //TH2F* hist = ((RooDataSet*)ws->data(Form("%s_%s",name.Data(),cats.at(catIndex).Data()))->reduce(selectionString))->createHistogram(*MR,*R,"",fullname);
  TH2F* hist = ((RooDataSet*)outputWs->data(name+"_Combined")->reduce(Form("evtcat==evtcat::%s",cats.at(catIndex).Data()))->reduce(selectionString))->createHistogram(*MR,*R,"",fullname);
  hist->SetName(fullname);
  hist->Scale(weight);

  std::cout << selectionString << std::endl;
  std::cout << fullname << "   " << hist->Integral() << std::endl;

  /*
  RooDataHist roohist(Form("%s_hist",fullname.Data()),"",RooArgSet(*MR,*R),*((RooDataSet*)ws->data(Form("%s_%s",name.Data(),cats.at(catIndex).Data()))->reduce(selectionString)),
		   weight);
  RooHistPdf roopdf(Form("%s_pdf",fullname.Data()),"",RooArgSet(*MR,*R),roohist);

  std::cout << roohist.GetName() << std::endl;

  ws->import(roohist);
  ws->import(roopdf);
  */

  return hist;
}

void Fitter::saveAll() {
  outputFile->cd();
  
  for(auto& it: save_histograms) {
    it.second->Write();
  }
  for(auto& it: save_SigRegion_histograms) {
    it.second->Write();
  }
  
  outputWs->Write();
}

void Fitter::setupCats() {
  if(roocat) delete roocat;
  roocat = new RooCategory("evtcat","evtcat");
  
  if( inputWs->data("data") ) {
    setupCategory("data",&cats);
  }else{
    processData=false;
  }
  for(auto& it: sm_higgs_names) {
    setupCategory(it);
  }
  for(auto& it: sms_names) {
    setupCategory(it,(cats.size()==0 ? &cats : 0));
  }
  //cats.push_back("Combined");

  if(sm_higgs_names.size()>0) SetupSignalRegions();

  sideband_integrals.resize(cats.size());
  signal_integrals.resize(cats.size());
  sideband_to_signal_scale_factors.resize(cats.size());

  for(auto& it: cats) roocat->defineType(it);
}

void Fitter::setupCategory(TString name,std::vector<TString>* catNames) {
  std::map<std::string,RooDataSet*> sets ;

  RooDataSet *data = (RooDataSet*)inputWs->data(name);
  for(auto& it: selectionMap) {
    RooDataSet* cat_data = (RooDataSet*)data->reduce(baseSelection+" && "+it.second.c_str());
    //ws->import(*cat_data,RooFit::Rename(Form("%s_%s",name.Data(),it.first.c_str())) );
    if(catNames)catNames->push_back(it.first.c_str());
    sets[it.first]=cat_data;
  }

  //get the necessary variables
  RooArgSet set;
  set.add(*inputWs->var("mgg"));
  set.add(*inputWs->var("MR"));
  set.add(*inputWs->var("MR_up"));
  set.add(*inputWs->var("MR_down"));
  set.add(*inputWs->var("Rsq"));
  set.add(*inputWs->var("Rsq_up"));
  set.add(*inputWs->var("Rsq_down"));

  //RooArgSet set(*data->get(0));
  //set.add( *inputWs->var("pileupWeight") );
  RooRealVar puWeight("pileupWeight","",1,0,40);
  set.add( puWeight );
  
  RooDataSet* comb=0;
  if(name=="data") comb = new RooDataSet(name+"_Combined","",set,RooFit::Index(*roocat),RooFit::Import(sets));
  else comb = new RooDataSet(name+"_Combined","",set,RooFit::WeightVar("pileupWeight"),RooFit::Index(*roocat),RooFit::Import(sets));
  outputWs->import(*comb);


  delete comb;
}



RooSimultaneous* Fitter::buildSimultaneousPdf(TString name) {
  RooSimultaneous *sim = new RooSimultaneous( Form(name.Data(),"Simultaneous"), "", *roocat);
  
  for(int catIndex=0;catIndex < cats.size();catIndex++) {
    std::string catname = cats.at(catIndex).Data();

    RooAbsPdf *pdf = outputWs->pdf( Form(name.Data(),catname.c_str()) );
    sim->addPdf(*pdf,catname.c_str());
  }
  return sim;

}

void Fitter::defineCats() {
  //per_cat_fit_ranges.push_back({110.,120.,  124.62-1.352*2,124.62+1.352*2, 130.,140.});
  //per_cat_fit_ranges.push_back({110.,120.,  124.46-1.488*2,124.46+1.488*2, 130.,140.});
  per_cat_fit_ranges.push_back({110.,120.,  124.72-1.289*2,124.72+1.289*2, 130.,140.});
  per_cat_fit_ranges.push_back({110.,120.,  124.72-1.289*2,124.72+1.289*2, 130.,140.});
  per_cat_fit_ranges.push_back({110.,120.,  125.07-1.896*2,125.07+1.896*2, 130.,140.});

  selectionMap = {
    //{"SingleMu","mu1_pt > 20"},
    //{"SingleEle","mu1_pt <= 20. && ele1_pt > 20."},
    //{"Hadronic_HighPt","mu1_pt <= 20. && ele1_pt <=20. && (abs(pho1_eta)<1.48 && abs(pho2_eta)<1.48) && ptgg > 70"},
    //{"Hadronic_EBEB","mu1_pt <= 20. && ele1_pt <=20. && (abs(pho1_eta)<1.48 && abs(pho2_eta)<1.48) && ptgg <=70"},
    //{"Hadronic_NEBEB","mu1_pt <= 20. && ele1_pt <=20. && !(abs(pho1_eta)<1.48 && abs(pho2_eta)<1.48) && ptgg <=70"}
    {"Hadronic_HighPt","ptgg > 110"},
    //{"Hadronic_EBEB",  "(abs(pho1_eta)<1.48 && abs(pho2_eta)<1.48) && ptgg <=110"},
    //{"Hadronic_NEBEB", "!(abs(pho1_eta)<1.48 && abs(pho2_eta)<1.48) && ptgg <=110"}
    {"Hadronic_HighRes",  "ptgg <=110 && ( (abs(pho1_eta)>1.48 && pho1_sigEoE<0.024) || pho1_sigEoE<0.015) && ( (abs(pho2_eta)>1.48 && pho2_sigEoE<0.024) || pho2_sigEoE<0.015)"},
    {"Hadronic_LowRes",  "ptgg <=110 && !( ( (abs(pho1_eta)>1.48 && pho1_sigEoE<0.024) || pho1_sigEoE<0.015) && ( (abs(pho2_eta)>1.48 && pho2_sigEoE<0.024) || pho2_sigEoE<0.015) )"},
  };


  assert(per_cat_fit_ranges.size() == selectionMap.size() && "fit ranges and selection map incompatible");

  //baseSelection = "pho1_r9 > 0.9 && pho2_r9 > 0.9";
  baseSelection = "pho1_pass_iso==1 && pho2_pass_iso==1";
  //baseSelection = "1";
}


void Fitter::MakeSMTot() {
  if(sm_higgs_names.size()==0) return;
  
  RooDataSet *t = (RooDataSet*)inputWs->data( sm_higgs_names.begin()->Data() );

  RooArgSet vars(*t->get(0));

  RooRealVar weight("pileupWeight","",1,0,40);
  vars.add(weight);

  RooDataSet total("SMTot","",vars,"pileupWeight");

  RooRealVar NSMTOT("N_SMTot","",1);

  for(auto& mcName: sm_higgs_names) {
    RooDataSet *d = (RooDataSet*)inputWs->data( mcName.Data() );
    const RooArgSet * set=0;

    RooRealVar *n = inputWs->var("N_"+mcName);    
    float xsec_weight = xsecs[mcName] * 2.28E-3 * lumi*1000/n->getVal();
    std::cout << mcName << std::endl;
    std::cout << xsec_weight << "\n\t" << xsecs[mcName] << "\t" << n->getVal()
	      << std::endl;

    int i=0;
    while( (set = d->get(i++)) ) {
      total.add( *set, xsec_weight * d->weight() );      
    }
  }
  
  inputWs->import( total );
  inputWs->import( NSMTOT );
  sm_higgs_names.push_back("SMTot");
}

void Fitter::DefineBinning() {
  RooRealVar *R  = inputWs->var("Rsq");
  RooRealVar *MR = inputWs->var("MR");

  MR->setRange(0,2000.);
  R->setRange(0,1.0);

  MR->setBins(50,"Full");
  R->setBins(25,"Full");
  /*
  const int nMRbins =5;
  double mrBins[nMRbins] = {0,200,400,1000,2000};
  const int nRbins =5;
  double rBins[nRbins] = {0,0.1,0.2,0.5,1.0};
  */
  const int nMRbins =5;
  double mrBins[nMRbins] = {0,250,450,1200,2000};
  const int nRbins =6;
  double rBins[nRbins] = {0,0.05,0.1,0.3,0.6,1.0};

  RooBinning binning_MR(nMRbins,mrBins,"Regions");
  RooBinning binning_R(nRbins,rBins,"Regions");

  MR->setBinning(binning_MR,"Regions");
  R->setBinning(binning_R,"Regions");
  outputWs->import(*MR);
  outputWs->import(*R);
}


std::string Fitter::replaceAllInString(const std::string& s, const std::string& from, const std::string& to) {
  size_t start_pos = 0;
  
  std::string sout = s;
  while( (start_pos = sout.find(from,start_pos)) != std::string::npos ) {
    sout.replace(start_pos,from.length(),to);
    start_pos+=to.length();
  }
  return sout;
}
