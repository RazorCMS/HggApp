
#include "TH1D.h"

float getSigEff(const TH1D& hist, float mean) { //compute the sigma effective about the mean

  float integral = hist.Integral();

  int center_bin = hist.FindFixBin(mean);

  float N_bin=1;

  float target = 0.682689492;

  while(hist.Integral(center_bin-N_bin,center_bin+N_bin)/integral < target) N_bin++;

  return (N_bin-1)*hist.GetBinWidth(center_bin);
}