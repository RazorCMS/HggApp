#ifndef DataFitterNew_hh
#define DataFitterNew_hh

#include "FitterNew.hpp"

#include "RooWorkspace.h"

class DataFitter : public Fitter {
public:
  DataFitter(TString inputFileName, TString outputFileName,bool useHT=false): Fitter(inputFileName,outputFileName,useHT) {
    isSMS=false;
  }

  static RealVar doFitGetScale(TTree *data,float width,RooWorkspace* ws=0,bool dExp=true); //!< for real data, do the background fit and use that to compute the sideband->signal scale factor

  virtual void Run();

  virtual void fixNorm(TString catName, float norm);

  void SetTriggerPath(TString triggerFilePath);
  void SetNoisePath(TString noiseFilePath);

protected:

  virtual void buildSidebandHistograms();

  virtual void processEntrySidebands();

  virtual float getSysErrPho(float eta,float r9);

  std::map<TString,RealVar> scales;

  std::map<TString,TH2F*> SidebandRegionHistograms;
  std::map<TString,TH2F*> SidebandRegionHistogramsFineBin;

  std::map<TString,TH3F*> SidebandRegions3D;

  std::vector<RooWorkspace*> mggFitWorkspaces;

  std::map<TString,float> scaleSys = {
    {"EBLow_lowR9",0.0004},{"EBLow_highR9",0.0003},{"EBHigh_lowR9",0.0008},{"EBHigh_highR9",0.0008},
    {"EELow_lowR9",0.0013},{"EELow_highR9",0.0011},{"EEHigh_lowR9",0.0010},{"EEHigh_highR9",0.0009},
  };

  bool fixScales = false;
  std::map<TString,float> normMap;
  bool hasTrigger=false;
  bool triggerBit=true;
  bool hasNoise=false;
  bool noiseBit=true;

  std::map<TString,int> nSideband;
  std::map<TString,int> nSidebandLow;
  std::map<TString,int> nSidebandHigh;

  std::map<TString,float> sfSyst = { { "HighPt",0.04}, {"Hbb",0.08}, {"Zbb",0.08}, {"HighRes",0.08}, {"LowRes",0.04} };
};

#endif

