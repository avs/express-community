// Adaptive Intensity Correction Filter
//
// Bruno Jobard - 2001
//
// Modify the intensity and opacity of a graylevel image in function of an
// intensity map.


#ifndef _INTENSITY_MASK_H_
#define _INTENSITY_MASK_H_

//#include "system_includes.h"
#include "interpolation.h"

#define IM_LINEAR 1
#define IM_QUAD   2
#define IM_CUBIC  3

class IntensityMaskFilter
{
public:
  enum IMFOption {
    O_LINEAR,
    O_QUAD,
    O_CUBIC
  };

private:
  float* resImg_;         // filtered image
  int nx_;
  int ny_;
  int nc_;                // nb components in output image
                          // (1->intensity, 2->intensity + alpha)
  
public:

  IntensityMaskFilter();
  ~IntensityMaskFilter(){ delete [] resImg_;}
  
  float* Compute(const float* img, int nxi, int nyi,
		 const float* map, int nxm, int nym,
		 IMFOption imfo, int alphaOn);

};


#endif
