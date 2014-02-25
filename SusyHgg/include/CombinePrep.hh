/**

sets up the files and datacards for combine.  Works with the FitterNew classes.

*/

#ifndef CombinePrep_hh
#define CombinePrep_hh

#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

#include "assert.h"

#include <map>
#include <memory>

#include <iostream>
#include <fstream>


class CombinePrep {
public:
  CombinePrep(){}

  void addDataFilePath(TString s){dataFilePath=s;}
  void addSMHiggsFilePath(TString name, TString path) { smHiggsFilePaths[name] = path; }
  void addSMSFilePath(TString name, TString path) { smsFilePaths[name] = path; }
  
  void setOutputFolder(TString s){outputFolder=s;} //file make <outputFolder>/combine/ for txt files and root files

  static std::unique_ptr<TH1F> compressHistogram(const TH2F& input);

  void setCatNames(const std::vector<TString>* names) { catNames = names; }
  void setSysNames(const std::vector<TString>* names) { sysNames = names; } 


  void Make();

protected:
  TString dataFilePath;
  std::map<TString,TString> smHiggsFilePaths; //map of process name --> filePath
  std::map<TString,TString> smsFilePaths; //map of SMS name --> filePath

  TString outputFolder;
  


  std::unique_ptr<TFile> dataFile;
  std::map<TString,std::unique_ptr<TFile>> smHiggsFiles;
  std::map<TString,std::unique_ptr<TFile>> smsFiles;

  const std::vector<TString>* catNames; //not owned, owner must ensure this exists for the duration of this class scope
  const std::vector<TString>* sysNames;

  void openFiles(); //opens all the files set in the file paths
  void makeOnePoint(TString smsName,TString smsPoint); // make a single combine point for the given sms
  void makeRootFile();

  std::map<TString,float> yields;

  TH1F* makeCategoryHistogram(TFile* file,TString histName, TString postfix,TString sms_pt="");

};

#endif
