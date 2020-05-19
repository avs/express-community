#include "intensity_mask.h"

//-----------------------------------------------------------------------------
IntensityMaskFilter::IntensityMaskFilter() : resImg_(0), nx_(0), ny_(0)
{
}
//-----------------------------------------------------------------------------
float*
IntensityMaskFilter::Compute(const float* img, int nxi, int nyi,
			     const float* map, int nxm, int nym,
			     IMFOption imfo, int alphaOn)
{  
  int i,j,jj,ind1,ind2;
  float ci, cm, alpha;

  int nc = (alphaOn==1 ? 2 : 1);  // number of components in the output image

  if (nx_ != nxi || ny_ != nyi || nc_ != nc || !resImg_) { // reallocation!
    nx_ = nxi;
    ny_ = nyi;
    nc_ = nc;
    delete [] resImg_;
    resImg_ = new float [nx_*ny_*nc_];
  }
  
  float rx = float(nxm)/float(nxi);
  float ry = float(nym)/float(nyi);
  float mapval;
  
  switch (imfo) {
  case O_LINEAR:
    {      
      for(j=0; j<nyi; j++){ jj=j*nxi; for(i=0; i<nxi; i++){

	ind1 = (jj+i)*nc_;
        ind2 =  jj+i;

	// find current map intensity
        linearInterpol2D(map, nxm, nym, float(i)*rx,float(j)*ry, &mapval);
	
	//- Set Intensity
	resImg_[ind1] = img[ind2];
	
	//- Set Alpha
	if (alphaOn) {
	  resImg_[ind1+1] = img[ind2]*mapval;
	} else {
	  resImg_[ind1] *= mapval;
	}
      }}
    }
    break;
  case O_QUAD:
    {
      for(j=0; j<nyi; j++){ jj=j*nxi; for(i=0; i<nxi; i++){

	ind1 = (jj+i)*nc_;
        ind2 =  jj+i;

	// find current map intensity
        linearInterpol2D(map, nxm, nym, float(i)*rx,float(j)*ry, &mapval);
	
	//- Set Intensity
	resImg_[ind1] = img[ind2];
	
	//- Set Alpha
	ci = img[ind2];
	cm = mapval;
	alpha = (1.0-(1.0-ci)*(1.0-ci))*(1.0-(1.0-cm)*(1.0-cm));
	
	if (alphaOn) {
	  resImg_[ind1+1] = alpha;
	} else {
	  resImg_[ind1] *= alpha;
	}
      }}
    }
    break;
  case O_CUBIC:
    {
      for(j=0; j<nyi; j++){ jj=j*nxi; for(i=0; i<nxi; i++){

	ind1 = (jj+i)*nc_;
        ind2 =  jj+i;

	// find current map intensity
        linearInterpol2D(map, nxm, nym, float(i)*rx,float(j)*ry, &mapval);
	
	//- Set Intensity
	resImg_[ind1] = img[ind2];
	
	//- Set Alpha
	ci = img[ind2];
	cm = mapval;
      alpha =(1.0-(1.0-ci)*(1.0-ci)*(1.0-ci))*(1.0-(1.0-cm)*(1.0-cm)*(1.0-cm));

        if (alphaOn) {
	  resImg_[ind1+1] = alpha;
	} else {
	  resImg_[ind1] *= alpha;
	}
      }}
    }
    break;
  }
  
  return resImg_;
}

