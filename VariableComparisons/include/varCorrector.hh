#ifndef varCorrector_hh
#define varCorrector_hh

#include "TString.h"
#include <map>
#include <cmath>

class varCorrector{
public:
  varCorrector();
  TString getCorrectString(TString var,bool isEB);
protected:
  std::map<TString,TString> correctionsEB;
  std::map<TString,TString> correctionsEE;
};

class varCorrector4cat{
public:
  varCorrector4cat();
  TString getCorrectString(TString var,float eta);
protected:
  std::map<TString,TString> correctionsEBlow;
  std::map<TString,TString> correctionsEBhigh;
  std::map<TString,TString> correctionsEElow;
  std::map<TString,TString> correctionsEEhigh;
};

#endif
