#include "advection_master.h"

AdvectionMaster::AdvectionMaster() : nx_(-1), ny_(-1),
                                     nvx_(-1), nvy_(-1),vx_(0),vy_(0),vmag_(0),
                                     advection_(0),
                                     advImg_(0), licImg_(0), vmImg_(0),
                                     outImg_(0)
{
  lic_          = new ALIC;
  velocityMask_ = new IntensityMaskFilter;

  
}
//-----------------------------------------------------------------------------
void
AdvectionMaster::SetResolution(int nx, int ny, int integrationStepMax)
{
  if (nx_!=nx || ny_!=ny || advIntegrationStepMax_ != integrationStepMax) {
    nx_ = nx;
    ny_ = ny;
    advIntegrationStepMax_ = integrationStepMax;

    if (advection_) delete advection_;
    advection_ = new Advection(nx_, ny_, advIntegrationStepMax_);
  }
}
//-----------------------------------------------------------------------------
void
AdvectionMaster::SetCurrentVectorField(const float* vx, const float* vy, 
				       const float* vmag,
				       int nvx, int nvy)
{
  nvx_  = nvx;
  nvy_  = nvy;
  vmag_ = vmag;
  vx_   = vx;
  vy_   = vy;
}
//-----------------------------------------------------------------------------
void
AdvectionMaster::SetAdvectionOptions(float integrationStep,
				     float swapPercentage,
				     float blendCoef)
{
  advIntegrationStep_ = (integrationStep < float(advIntegrationStepMax_) ?
			 integrationStep : float(advIntegrationStepMax_) );
  advSwapPercentage_  = swapPercentage;
  advBlendCoef_       = blendCoef;
}
//-----------------------------------------------------------------------------
void
AdvectionMaster::SetLicOptions(int kernelLength, int kernelShape,
			       int intensityCorrectionOn, int normalizationOn)
{

  licKernelLength_          = kernelLength;
  licIntensityCorrectionOn_ = intensityCorrectionOn;
  licNormalizationOn_       = normalizationOn;

  switch (kernelShape) {
  case LIC_BOX       : licKernelShape_ = ALIC::K_BOX;        break;
  case LIC_ROUNDEDBOX: licKernelShape_ = ALIC::K_ROUNDEDBOX; break;
  case LIC_TRIANGLE  : licKernelShape_ = ALIC::K_TRIANGLE;   break;
  case LIC_CUBIC     : licKernelShape_ = ALIC::K_CUBIC;      break;
  default            : licKernelShape_ = ALIC::K_BOX;        break;
  }
}
//-----------------------------------------------------------------------------
void
AdvectionMaster::SetVelocityMaskOptions(int velocityMaskVersion, int alphaOn)
{
  vmAlphaOn_ = alphaOn;
  
  switch (velocityMaskVersion) {
  case IM_LINEAR: vmVersion_ = IntensityMaskFilter::O_LINEAR; break;
  case IM_QUAD  : vmVersion_ = IntensityMaskFilter::O_QUAD;   break;
  case IM_CUBIC : vmVersion_ = IntensityMaskFilter::O_CUBIC;  break;
  default       : vmVersion_ = IntensityMaskFilter::O_CUBIC;  break;
  }

}
//-----------------------------------------------------------------------------
void
AdvectionMaster::Compute()
{
  if (!vx_ || !vy_ || nx_<=0 || ny_<=0) {
    outImg_ = 0;
    return;
    cerr <<"AdvectionMaster::Compute: object not sufficiently initialized"
	 <<endl;
  }
    
  //---  Advection
  advImg_ = advection_->Compute(vx_, vy_, nvx_, nvy_, advIntegrationStep_,
			       advSwapPercentage_, advBlendCoef_);
  outImg_ = advImg_;
  outnc_  = 1;

  //--- Lic filter
  if (licOn_) {
    lic_->SetKernel(licKernelLength_, licKernelShape_);
    lic_->SetIntensityCorrection(licIntensityCorrectionOn_);
    lic_->SetNormalization(licNormalizationOn_);
    licImg_ = lic_->Compute(outImg_, nx_, ny_, vx_, vy_, nvx_, nvy_);
    outImg_ = licImg_;
    outnc_  = 1;
  }

  //--- Velocity Mask filter
  if (vmOn_ && vmag_) {

    vmImg_ = velocityMask_->Compute(outImg_, nx_, ny_, vmag_, nvx_, nvy_,
				    vmVersion_, vmAlphaOn_);
    outImg_ = vmImg_;
    outnc_  = (vmAlphaOn_==1 ? 2 : 1);
  }
}
//-----------------------------------------------------------------------------
float*
AdvectionMaster::GetOutput(int& nbComponents)
{
  nbComponents = outnc_;
  return outImg_;
}
