#ifndef _ADVECTION_MASTER_
#define _ADVECTION_MASTER_

#include "advection.h"
#include "alic.h"
#include "intensity_mask.h"

class AdvectionMaster
{
protected:
  int nx_; // image size
  int ny_;

  //--- Vector Field
  int nvx_;
  int nvy_;
  const float* vx_;      // pointer on x velocity
  const float* vy_;      // pointer on y velocity
  const float* vmag_;    // pointer on velocity magnitude

  //--- Advection
  Advection* advection_;
  int    advIntegrationStepMax_;
  float  advIntegrationStep_;
  float  advSwapPercentage_;
  float  advBlendCoef_;
  float* advImg_;  // pointer on blended advected image
  
  //--- Lic Filter
  ALIC*  lic_;     // lic filter
  int    licOn_;   // Enable lic filtering
  int    licKernelLength_;
  int    licIntensityCorrectionOn_;
  int    licNormalizationOn_;
  ALIC::LicKernelShape licKernelShape_;
  float* licImg_;  // pointer on lic image

  //--- Velocity Mask
  IntensityMaskFilter* velocityMask_;  // velocity mask filter
  int vmOn_;           // Enable velocity mask filtering
  int vmAlphaOn_;      // Add an alpha component to the resulting image
  IntensityMaskFilter::IMFOption vmVersion_; // O_LINEAR, O_QUAD or O_CUBIC
  float* vmImg_;       // pointer on velocity mask filtered image

  //--- Output Image
  float* outImg_;      // pointer on the output image
  int    outnc_;       // number of components in output img: 1 for intensity
                       // 2 for intensity + alpha (vmAlphaOn_ should be 1)
  
public:
  AdvectionMaster();
  ~AdvectionMaster(){ delete advection_; delete lic_; delete velocityMask_;}
  
  void SetResolution(int nx, int ny, int integrationStepMax);
  void SetCurrentVectorField(const float* vx, const float* vy, 
			     const float* mag, int nvx,int nvy);
  void SetAdvectionOptions(float integrationStep, float swapPercentage,
			   float blendCoef);
  void SetLicOn(int licOn){licOn_ = licOn;}
  void SetLicOptions(int kernelLength, int kernelShape,
		     int intensityCorrectionOn, int normalizationOn);
  void SetVelocityMaskOn(int maskOn){vmOn_ = maskOn;}
  void SetVelocityMaskOptions(int velocityMaskVersion, int alphaOn);
  void Compute();

  float* GetOutput(int& nbComponents);

};

#endif
