#include "alic.h"

//-----------------------------------------------------------------------------
ALIC::ALIC()
{
  nx_  = 0;
  ny_  = 0;
  nxy_ = nx_*ny_;
  //cx0_ = 0;
  //cy0_ = 0;
  cx1_ = 0;
  cy1_ = 0;
  lic_ = 0;
  max_ = 0;
  h_   = 1.0;
  kernelLength_  = 5;
  kernelShape_   = K_BOX;
  kernelCoefs_   = 0;
  
  intensityCorrectionOn_ = 1;
}
//-----------------------------------------------------------------------------
void
ALIC::Init(int kernelLength, float h,
			 LicKernelShape kernelShape)
{
  int mustComputeKernelCoefs = 0;
  if (kernelLength_ != kernelLength || kernelShape_ != kernelShape ||
      kernelCoefs_  == 0) {
    delete [] kernelCoefs_;
    kernelCoefs_ = 0;
    mustComputeKernelCoefs = 1;
  }

  kernelLength_  = kernelLength;
  h_             = h;
  kernelShape_   = kernelShape;
  
  if (mustComputeKernelCoefs) this->ComputeKernelCoefs();
}
//-----------------------------------------------------------------------------
float*
ALIC::Compute(const float* in, int nx, int ny,
		      const float* vx, const float* vy, int nvx, int nvy)
{
  
  if (!kernelCoefs_) this->ComputeKernelCoefs();
  
  int i,j,jj,ind,ind2;

  if (nx != nx_ || ny != ny_) { // array reallocation is necessary
    nx_ = nx;
    ny_ = ny;
    nxy_ = nx * ny;
    //delete [] cx0_; delete [] cy0_;
    delete [] cx1_; delete [] cy1_;
    delete [] lic_; delete [] max_;
    //cx0_   = new float[nxy_];
    //cy0_   = new float[nxy_];
    cx1_   = new float[nxy_];
    cy1_   = new float[nxy_];
    lic_   = new float[nxy_];
    max_   = new float[nxy_];

    if (!(/*cx0_ && cy0_ &&*/ cx1_ && cy1_ && lic_)) {
      cerr << "ALIC::Compute: Error, not enough memory!" << endl;
      return 0;
    }

    // Init coordinates
    /*
    for(j=0; j<ny_; j++){ jj = j*nx_; for(i=0; i<nx_; i++){ ind = jj+i;
      cx0_[ind] = i;
      cy0_[ind] = j;
    }}
    */
  }
  
  if (intensityCorrectionOn_) { 
    memset(max_,0,sizeof(float)*nxy_); // int max_ array to zero
  }
  
  float velx, vely;
  float newx, newy;
  float m;
  float rx = float(nvx)/float(nx_);
  float ry = float(nvy)/float(ny_);
  float nval;
  float maxval;

  // set the out lic image with the middle kernel coef
  for(j=0; j<ny_; j++){ jj = j*nx_; for(i=0; i<nx_; i++){ ind = jj+i;
    lic_[ind] = in[ind]*kernelCoefs_[kernelLength_];
  }}
  
  //--- Cumulate noise values found backward and forward the flow
  int k;
  float signed_lic_h;
  int l=1;
  while (l<=2) { // 2 phases: one backward, one forward
    
    //--- Define integration orientation
    if (l==1) { signed_lic_h = -h_; }  // backward
    else      { signed_lic_h =  h_; }  // forward
    
    //--- Init Coords
    for(j=0; j<ny_; j++){ jj = j*nx_; for(i=0; i<nx_; i++){ ind = jj+i;
    //cx1_[ind] = cx0_[ind];
    //cy1_[ind] = cy0_[ind];
      cx1_[ind] = i;
      cy1_[ind] = j;
    }}

    //--- Loop on half of the convolution kernel size
    for(k=0; k<kernelLength_; k++) {
      
      for(j=0; j<ny_; j++){ jj = j*nx_; for(i=0; i<nx_; i++){ ind = jj+i;
      
        linearInterpol2D(vx,vy,nvx,nvy,cx1_[ind]*rx,cy1_[ind]*ry,&velx,&vely);
	
	//--- Integration

	if (normalizationOn_) {
	  m = sqrt(velx*velx+vely*vely);
	  if (m<0.01) m = 1.0;
	  newx = cx1_[ind] + signed_lic_h*velx/m;
	  newy = cy1_[ind] + signed_lic_h*vely/m;
	} else {
	  newx = cx1_[ind] + signed_lic_h*velx;
	  newy = cy1_[ind] + signed_lic_h*vely;
	}
	
	if (newx>=0 && newx<nx_ && newy>=0 && newy<ny_) {
	  cx1_[ind] = newx;
	  cy1_[ind] = newy;
	}
	
	//cx1_[ind] += signed_lic_h*velx;
	//cy1_[ind] += signed_lic_h*vely;

	/*
	  //--- Integration with Normalization
	  float m = sqrt(velx*velx+vely*vely);
	  if (m<0.001){
	  cx2[ind1] += signed_lic_h*velx;
	  cy2[ind1] += signed_lic_h*vely;
	  } else {
	  cx2[ind1] += signed_lic_h*velx/m;
	  cy2[ind1] += signed_lic_h*vely/m;
	  }
	  */
      }}
      
      //--- Noise Accumulation
      for(j=0; j<ny_; j++){ jj = j*nx_; for(i=0; i<nx_; i++){ ind = jj+i;
	
        ind2 = int(cy1_[ind])*nx + int(cx1_[ind]);

	  linearInterpol2D(in, nx,ny, cx1_[ind2],cy1_[ind2], &nval);

	if (intensityCorrectionOn_) {
	  maxval = nval - 0.5;
	  maxval = (maxval<0 ? -maxval : maxval);
	  maxval += 0.5;
	  if (maxval>max_[ind]) max_[ind] = maxval;
	}

	// cumulate intensity
	lic_[ind] +=  nval*kernelCoefs_[k];

      }}
    } // end for k
    l++;
  } // end while l

  if (intensityCorrectionOn_) {
    for(j=0; j<ny_; j++){ jj = j*nx_; for(i=0; i<nx_; i++){ ind = jj+i;
      lic_[ind] =  ((lic_[ind]-0.5)/max_[ind] + 0.5);
    }}
  }
  return lic_;
}
//-----------------------------------------------------------------------------
void
ALIC::CopyLIC(float* out)
{
  int i;
  for(i=0; i<nxy_; i++){
    out[i] = float(lic_[i]);
  }
}
//-----------------------------------------------------------------------------
void
ALIC::SetKernelShape(LicKernelShape lks)
{
  if (kernelShape_ != lks) {
    kernelShape_ = lks;
    delete [] kernelCoefs_;
    kernelCoefs_ = 0;
    this->ComputeKernelCoefs();
  }
}
//-----------------------------------------------------------------------------
void
ALIC::SetKernelLength(int lkl)
{
  if (kernelLength_ != lkl/2) {
    kernelLength_ = lkl/2;
    delete [] kernelCoefs_;
    kernelCoefs_ = 0;
    this->ComputeKernelCoefs();
  }
}
//-----------------------------------------------------------------------------
void
ALIC::SetKernel(int lkl, LicKernelShape lks)
{
  if (kernelShape_ != lks || kernelLength_ != lkl/2) {
    kernelShape_ = lks;
    kernelLength_ = lkl/2;
    delete [] kernelCoefs_;
    kernelCoefs_ = 0;
    this->ComputeKernelCoefs();
  }
}
//-----------------------------------------------------------------------------
void
ALIC::ComputeKernelCoefs()
{
  if (!kernelCoefs_) kernelCoefs_ = new float[2*kernelLength_+1];
  
  switch(kernelShape_) {
  case K_TRIANGLE : // Triangle Kernel
    {
      float incr = 1.0/float(kernelLength_+1);
      float val,sum = 0;
      int i;
      for(i=0; i<kernelLength_+1; i++) {
	val = (i+1)*incr;
	kernelCoefs_[i]                 = val;
	kernelCoefs_[2*kernelLength_-i] = val;
      }
      for(i=0; i<2*kernelLength_+1; i++) {
	sum += kernelCoefs_[i];
      }
      for(i=0; i<2*kernelLength_+1; i++) {
	kernelCoefs_[i] /= sum;
      }
    }
  break;
  case K_ROUNDEDBOX : // power 4 Kernel
    {
      // float a = -(kernelLength_)*(kernelLength_)/log(0.05);
      float val,sum = 0;
      int i;
      for(i=0; i<kernelLength_+1; i++) {
	val = float(i)/float(kernelLength_+1);
	val = 1.0-val*val*val*val;
	kernelCoefs_[kernelLength_+i] = val;
	kernelCoefs_[kernelLength_-i] = val;
      }
      for(i=0; i<2*kernelLength_+1; i++) {
	sum += kernelCoefs_[i];
      }
      for(i=0; i<2*kernelLength_+1; i++) {
	kernelCoefs_[i] /= sum;
      }
    }
    break;
  case K_CUBIC : // Gaussian Kernel
    {
      float a = -(kernelLength_)*(kernelLength_)/log(0.05);
      float val,sum = 0;
      int i;
      for(i=0; i<kernelLength_+1; i++) {
	val = exp(-i*i/a);
	kernelCoefs_[kernelLength_+i] = val;
	kernelCoefs_[kernelLength_-i] = val;
      }
      for(i=0; i<2*kernelLength_+1; i++) {
	sum += kernelCoefs_[i];
      }
      for(i=0; i<2*kernelLength_+1; i++) {
	kernelCoefs_[i] /= sum;
      }
    }
    break;
  case K_BOX : // Box kernel
    
    {for(int i=0; i<2*kernelLength_+1; i++) {
      kernelCoefs_[i] = 1.0/float(2*kernelLength_+1);
    }}
  break;
  }
  /*
  float sum=0;
  {for(int i=0; i<2*kernelLength_+1; i++) {
    cout << kernelCoefs_[i] << endl;
    sum+=kernelCoefs_[i];
    }}
  cout << endl <<"sum = "<<sum << endl;
  */
}
