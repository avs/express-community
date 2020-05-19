//-----------------------------------------------------------------------------
//--- Texture Advection for Unsteady Flow Visualization
//--- Bruno Jobard - 2000
//--- CSIT -  Florida State University
//-----------------------------------------------------------------------------

#ifndef _ADVECTION_
#define _ADVECTION_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <GL/glut.h>
//#include <GL/gl.h>
#include <math.h>
#ifndef WIN32
#include <sys/time.h>
#endif
#include "interpolation.h"
#include "alic.h"

//--- OpenMP compilation tags
#ifdef _OPENMP
# include <omp.h>
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

class Advection
{
public:  
  int sizeX;    // Size of the final advected noise
  int sizeY;    //
  int size;     // (sizeX*sizeY)

  float h;      // Integration step in array cells
  int hmax;     // max integration step allowed

  int border;   // border size around arrays = ceil(h)
  int dimx;     // array width  including borders         dimx > sizeX
  int dimy;     // array height including borders         dimy > sizeY
  int dim;      // array size   (dimx * dimy)

  int mini;     // boundaries of indices in arrays of size (dimx*dimy)
  int maxi;     //
  int minj;     //
  int maxj;     //

  float swapPercent;  // percentage of noise injection in the noise array
  float blendCoef1;   // alpha blending of the new noise with the old ones

protected:
 
  //--- Arrays (sizeX*sizeY) 
  float *cx1;      // Coords  x
  float *cy1;      // Coords  y 
  float *cx2;      // Coords  x'
  float *cy2;      // Coords  y'
  float *n1 ;     // Noise
  float *n2 ;     // Noise advected with constant interp. (sizeX*sizeY)
  float *n3 ;     // Noise advected with bilinear interp. (dimX *dimY )
  float *nb;      // Noise blended

  float *nc;       // Noise current      ptr on n1 or n2
  float *na;       // Noise advected     ptr on n1 or n2
  float *nf;       // Noise final        ptr on nb or ngf or nwf or ncf;
  
public:

  Advection(int size_X, int size_Y, int h_max);
  ~Advection(){  delete [] cx1; delete [] cy1; delete [] cx2; delete [] cy2;
                 delete [] n1 ; delete [] n2 ; delete [] nb ;
               }

  void SetBlendCoef(float bc){blendCoef1=bc;}
  void SetSwapPercentage(float sp){swapPercent=sp;}
  
  float* Compute(const float* vxx, const float* vyy,
		     int nx, int ny, float _h, float swapPercentage,
		     float blendCoef);  

  const float* GetNoise (){return na;}
  float* GetBlendedNoise(){return nb;}
  float* GetFinalNoise  (){return nf;}

protected:
  void ComputeIntegration(const float* vxx, const float* vyy,
		                   const int nx, const int ny, const float _h);
  void ComputeCoordUpdate();
  void ComputeNoiseUpdate();
  void ComputeEdgeTreatment();
  void ComputeNoiseInjection();
  void ComputeNoiseBlending();  
};

//void GetVectorField(float *uvfdata, int nnx, int nny, int nnt,
//		    float t, float *vx, float *vy);

void WritePGM(char* filename, int width, int height, float* image);



#endif
