#ifndef SMSFitterNew_hh
#define SMSFitterNew_hh

#include "FitterNew.hpp"

class SMSFitter : public Fitter {
public:
  SMSFitter(TString inputFileName,TString outputFileName,bool useHT=false);


  virtual ~SMSFitter();

  void setNEntriesFile(TString fileName);
  
  virtual void Run();

  static void getSMSPoints(std::vector<TString>* pts, bool fullSIM=true);

  static TString getSMSPoint(float M22,float M23);

  void setIsFulLSIM(bool b=true){isFullSIM=b;}
  bool getIsFulLSIM(){return isFullSIM;}
  
private:
  virtual void buildHistograms() override;
  virtual void processEntry(bool doSyst=true) override;

  void scalePhotons();
  float getPhotonScale(float eta,float r9);

  bool isFullSIM=true; //using FullSIM SMS point
  
  TFile* nEntriesFile=0;
  TH2F* nEntriesHist=0;

  std::vector<TString> sms_points;
};

#endif
