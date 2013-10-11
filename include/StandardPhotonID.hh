#ifndef StandardPhotonID_hh
#define StandardPhotonID_hh

#include "VecbosEGObject.hh"

#include <map>

class StandardPhotonID{
public:
  StandardPhotonID();

  enum WP:unsigned int{kLoose,kMedium,kTight};
  enum Region:unsigned int{kEB,kEE};

  bool passID(const VecbosPho& pho,WP wp);
  bool passIso(const VecbosPho& pho,WP wp);
private:
  typedef std::map<std::pair<StandardPhotonID::WP,StandardPhotonID::Region>,float> cutMap;
  cutMap cut_HE;
  cutMap cut_sieie;
  cutMap cut_pfcharged;
  cutMap cut_pfneutral;
  cutMap cut_pfphoton;
};

#endif
