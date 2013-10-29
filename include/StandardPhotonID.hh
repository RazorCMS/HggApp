#ifndef StandardPhotonID_hh
#define StandardPhotonID_hh

#include "VecbosEGObject.hh"

#include <map>
#include <bitset>

class StandardPhotonID{
public:
  StandardPhotonID();

  enum WP:unsigned int{kLoose,kMedium,kTight};
  enum Region:unsigned int{kEB,kEE};

  bool passID(const VecbosPho& pho,WP wp);
  bool passIso(const VecbosPho& pho,WP wp);
  std::bitset<5> cutResults(const VecbosPho& pho,WP wp);
  typedef std::map<std::pair<StandardPhotonID::WP,StandardPhotonID::Region>,float> cutMap;
protected:
  cutMap cut_HE;
  cutMap cut_sieie;
  cutMap cut_pfcharged;
  cutMap cut_pfneutral;
  cutMap cut_pfphoton;
};

#endif
