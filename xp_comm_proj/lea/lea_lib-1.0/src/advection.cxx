//-----------------------------------------------------------------------------
//--- Texture Advection for Unsteady Flow Visualization
//--- Bruno Jobard - 2000
//--- CSIT -  Florida State University
//-----------------------------------------------------------------------------

#include "advection.h"

#ifdef WIN32
double
drand48()
{
	return ((double)rand())/(double)RAND_MAX;
}

#endif

#ifdef __sun
#define ceilf(_x)	(float)ceil((double)_x)
#endif

#define random rand

Advection::Advection(int size_X, int size_Y, int h_max)
{
	sizeX = size_X;
	sizeY = size_Y;
	size  = sizeX*sizeY;
	hmax  = h_max;
	h     = hmax;
	
	border = int(ceilf(hmax));
	dimx   = sizeX + 2*border;
	dimy   = sizeY + 2*border;
	dim    = dimx*dimy;
	
	border = hmax;              // border size around arrays = [h]+1
	mini = border;
	maxi = dimx - border;
	minj = border;
	maxj = dimy - border;
	
	swapPercent = 2.0;
	blendCoef1  = 0.1;
	
	cx1 = new float[size];
	cy1 = new float[size];
	cx2 = new float[size];
	cy2 = new float[size];
	n1  = new float[dim];
	n2  = new float[dim];
	nb  = new float[size];
	n3  = new float[size];
	nc  = n1;
	na  = n2;
	nf  = nb;
	
	int i, j, j1, ind1, ind2;
    
	//--- Init Coords
	for(j=0; j<sizeY; j++)
	{
		j1 = j*sizeX;
		for(i=0; i<sizeX; i++)
		{
			ind1 = j1+i;
			cx1[ind1] = i+drand48();
			cy1[ind1] = j+drand48();
		}
	}
	
	//--- Init Noise
	for(i=0; i<dim; i++)
	{
		nc[i] = (drand48()<0.5 ? 0.0 : 1.0);
		//nc[i] = float(random()&01)*0.999;
		//if (nc[i]>1.0 || nc[i]<0.0) {cout<<"ca craint3"<<endl; exit(0);} 	
	}
	//memcpy(na, nc, dim*sizeof(float));  // na = nc
	
	for(j=0; j<sizeY; j++)
	{
		j1=j*sizeX;
		for(i=0; i<sizeX; i++)
		{
			ind1=j1+i;
			ind2 = (j+minj)*dimx + i+mini;
			nb[ind1] = nc[ind2];
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
float *
Advection::Compute(const float* vxx, const float* vyy, int nx, int ny,
				   const float _h, float swapPercentage, float blendCoef)
{
	h = _h;
	swapPercent = swapPercentage;
	blendCoef1  = blendCoef;
	
	ComputeIntegration(vxx,vyy,nx,ny,_h);
	ComputeCoordUpdate();
	ComputeNoiseUpdate();
	ComputeEdgeTreatment();
	ComputeNoiseInjection();
	ComputeNoiseBlending();
	
	// Swap noises
	float* ntmp;
	ntmp = nc; nc = na; na = ntmp;
	
	/*
	#ifdef _OPENMP
	this->na = na;
	this->nc = nc;
	this->nf = nf;
	#endif
	*/

	// Return the final noise array
	return nf;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
Advection::ComputeIntegration(const float* vxx, const float* vyy,
							  const int nx, const int ny,
							  const float _h)
{
	int i,j,j1,ind1;
	float velx, vely;
	
	float rx = float(nx-1)/float(sizeX);
	float ry = float(ny-1)/float(sizeY);
	
	// OpenMP Init
#ifdef _OPENMP
	// copy class attributes in current scope for OpenMP use
	// (shared and local variables cannot be part of a class or structure...)
	float* cx1 = this->cx1;
	float* cy1 = this->cy1;
	float* cx2 = this->cx2;
	float* cy2 = this->cy2;
	int& sizeX = this->sizeX;
	int& sizeY = this->sizeY;
		
#   pragma parallel
#   pragma shared(nx,ny)
#   pragma local(i,j,j1,ind1)
#   pragma local(velx,vely)
	//--- Integration
#   pragma pfor 
#   pragma local (i,j,j1,ind1,velx,vely)

#endif
	
    for(j=0; j<sizeY; j++)
	{
		j1=j*sizeX;
		for(i=0; i<sizeX; i++)
		{
			ind1=j1+i;
    
			//--- Linear interpolation of velocity for each coordinate in (cx1,cy1)
			linearInterpol2D(vxx,vyy, nx,ny, cx1[ind1]*rx,cy1[ind1]*ry, &velx,&vely);
    
			//--- Integration
			cx2[ind1] = cx1[ind1] - h*velx;
			cy2[ind1] = cy1[ind1] - h*vely;
		}
	} 
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
Advection::ComputeCoordUpdate()
{
	int i,j,j1,ind1;
	
	// OpenMP Init
#ifdef _OPENMP  
	// copy class attributes in current scope for OpenMP use
	// (shared and local variables cannot be part of a class or structure...)
	float* cx1 = this->cx1;
	float* cy1 = this->cy1;
	float* cx2 = this->cx2;
	float* cy2 = this->cy2;
	int& sizeX = this->sizeX;
	int& sizeY = this->sizeY;
	
#   pragma parallel
#   pragma local(i,j,j1,ind1,ind2)
#endif
	{
		
		//--- Coordinates updates
#ifdef _OPENMP  
# pragma pfor 
# pragma local (i,j,j1,ind1)
#endif
		for(j=0; j<sizeY; j++)
		{
			j1=j*sizeX;
			for(i=0; i<sizeX; i++)
			{
				ind1=j1+i;
		
				// Keep fractional part from x2 and y2:
				//                                       x1(i) = i + frac(x2(i))
				//                                       y1(j) = j + frac(x2(j))
				cx1[ind1]=float(i)+cx2[ind1]-int(cx2[ind1]);
				cy1[ind1]=float(j)+cy2[ind1]-int(cy2[ind1]);
			}
		}
		
	} // pragma parallel
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
Advection::ComputeNoiseUpdate()
{
	int i,j,j1,ind1,ind2,ind3;
	
	// OpenMP Init
#ifdef _OPENMP  
	// copy class attributes in current scope for OpenMP use
	// (shared and local variables cannot be part of a class or structure...)
	float* cx2 = this->cx2;
	float* cy2 = this->cy2;
	float* na  = this->na;
	float* nc  = this->nc;
	int& sizeX = this->sizeX;
	int& sizeY = this->sizeY;
	
#   pragma parallel
#   pragma local(i,j,j1,ind1,ind2)
#endif
	{
		float nval;
		
		//--- Noise update
#ifdef _OPENMP  
# pragma pfor 
# pragma local (i,j,j1,ind1,ind2)
#endif
		for(j=0; j<sizeY; j++)
		{
			j1=j*sizeX;
			for(i=0; i<sizeX; i++)
			{
				ind1 = j1+i;
				ind2 = (j+minj)*dimx + i+mini;
				ind3 = int(cy2[ind1]+minj)*dimx + int(cx2[ind1]+mini);
				
				// constant interpolation advection of current noise
				na[ind2] = nc[ind3];
				
				// linear interpolation advection for final display
				linearInterpol2D(nc, dimx,dimy, float(cx2[ind1]+mini),float(cy2[ind1]+minj), &nval);
				n3[ind1] = nval;
				/*
				if (n3[ind1]>1.0 || n3[ind1]<0.0) {cerr<<"Warning: LEA out of bound "<<n3[ind1]<<endl; n3[ind1]=0.5;} 
				*/
			}
		}
	} // pragma parallel
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
Advection::ComputeEdgeTreatment()
{ 
	int i,j,j1,ind1;
	
	for(j=0; j<minj; j++)
	{
		j1=j*dimx;
		for(i=0; i<dimx; i++)
		{
			ind1=j1+i;
			na[ind1] = float(random()&01);
		}
	}
	for(j=maxj; j<dimy; j++)
	{
		j1=j*dimx;
		for(i=0; i<dimx; i++)
		{
			ind1=j1+i;
			na[ind1] = float(random()&01);
		}
	}
	for(j=minj; j<maxj; j++)
	{
		j1=j*dimx;
		for(i=0; i<mini; i++)
		{
			ind1=j1+i;
			na[ind1] = float(random()&01);
		}
		for(i=maxi; i<dimx; i++)
		{
			ind1=j1+i;
			na[ind1] = float(random()&01);
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
Advection::ComputeNoiseInjection()
{
	int i,ind1;
	
	// OpenMP Init
#ifdef _OPENMP  
	// copy class attributes in current scope for OpenMP use
	// (shared and local variables cannot be part of a class or structure...)
	float* na  = this->na;
	
#   pragma parallel
#   pragma local(i,ind1)
#endif
	{
		
		//--- Noise injection
		int incr = int(100/swapPercent);
#ifdef _OPENMP  
# pragma pfor 
# pragma local (i,ind1)
#endif
		for(i=minj*dimx; i<maxj*dimx-incr; i+=incr)
		{
			ind1 = int(i+drand48()*incr);     // swap one pixel in each incr interval
			
			//int isbefore = 0;
			//if (na[ind1]>1.0 || na[ind1]<0.0) isbefore = 1;
			na[ind1] = 1.0 - na[ind1];
			//if ((na[ind1]>1.0 || na[ind1]<0.0)&&!isbefore) {cout<<"ca craint"<<endl; exit(0);} 	
		}
	} // pragma parallel
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
Advection::ComputeNoiseBlending()
{
	int i,j,j1,ind1;
	
	// OpenMP Init
#ifdef _OPENMP  
	// copy class attributes in current scope for OpenMP use
	// (shared and local variables cannot be part of a class or structure...)
	float* nb  = this->nb;
	int& sizeX = this->sizeX;
	int& sizeY = this->sizeY;
	float& blendCoef1 = this->blendCoef1;
	
#   pragma parallel
#   pragma local(i,j,j1,ind1)
#endif
	{
		//--- Noise Blending		
#ifdef _OPENMP  
# pragma pfor 
# pragma local (i,j,j1,ind1)
#endif
		for(j=0; j<sizeY; j++)
		{
			j1=j*sizeX;
			for(i=0; i<sizeX; i++)
			{
				ind1 = j1+i;

				nb[ind1] += blendCoef1*(n3[ind1]-nb[ind1]);
			}
		}
	} // pragma parallel
	
	nf = nb;   // Current final noise is the blended version
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
WritePGM(char *filename, int width, int height, float* image)
{
	FILE *pgmfile;
	
	pgmfile = fopen(filename, "wb");
	
	//--- header
	fprintf(pgmfile, "P5\n%d %d 255\n", width, height);
	
	//--- image
	fwrite(image, sizeof(unsigned char), width*height, pgmfile);
	
	fclose(pgmfile);
}
