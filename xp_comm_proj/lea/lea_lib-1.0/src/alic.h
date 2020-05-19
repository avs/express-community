#ifndef _ALIC_H_
#define _ALIC_H_

#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "interpolation.h"

//typedef unsigned char uchar;

//--- OpenMP compilation tags
#ifdef _OPENMP
# include <omp.h>
#endif

#define LIC_BOX        1
#define LIC_ROUNDEDBOX 2
#define LIC_TRIANGLE   3
#define LIC_CUBIC      4

class ALIC
{
public:
  
  enum LicKernelShape {   // different filter kernel types for LIC
    K_BOX,                // Box filter
    K_ROUNDEDBOX,         // Power 4 filter
    K_TRIANGLE,           // Triangle filter
    K_CUBIC               // Cubic filter
  };

protected:

  int nx_;    // Size of the final advected noise
  int ny_;    //
  int nxy_;   // (sizeX*sizeY)

  //float* cx0_;   // initial x coord array
  //float* cy0_;   // initial y coord array
  float* cx1_;   // integrated x coord
  float* cy1_;   // integrated y coord
  float* lic_;   // lic output image

  int intensityCorrectionOn_;
  float* max_;   

  float  h_;                // integration step for LIC
  int    kernelLength_;     // half kernel size
  float* kernelCoefs_;      // LIC kernel coefficients
  int    normalizationOn_;  // normalize velocity
  LicKernelShape   kernelShape_;    // Kernel Shape

public:
  ALIC();
  ~ALIC(){ //delete [] cx0_;      delete [] cy0_;
           delete [] cx1_;      delete [] cy1_;
	   delete [] lic_;
	   delete [] kernelCoefs_;
	   delete [] max_;
           }
  
  void Init(int KernelLength, float h, LicKernelShape KernelShape);

  void CopyNoise(const float* in);
  void CopyLIC  (      float* out);

  void SetNormalization(int norm){normalizationOn_ = norm;}
  void SetIntensityCorrection(int ic){intensityCorrectionOn_ = ic;}
  void SetKernelLength(int lkl);
  void SetKernelShape(LicKernelShape lks);
  void SetKernel(int length, LicKernelShape shape);

  float* Compute(const float* inputNoise, int nx, int ny,
	      const float* vx, const float* vy, int nvx, int nvy);

protected:
   void ComputeKernelCoefs(); 
};



#endif // _ALIC_H_
