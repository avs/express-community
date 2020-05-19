//-----------------------------------------------------------------------------
//--- Utility Series - Linear and directional interpolation in arrays
//--- Bruno Jobard - 2000
//-----------------------------------------------------------------------------

#ifndef _INTERPOLATION_H_
#define _INTERPOLATION_H_

#define ABS(a) (((a)<0)?(-(a)):(a))

#include <iostream.h>

//-----------------------------------------------------------------------------
template <typename T, typename U>
inline void
linearInterpol2D(const T* array, const int& dimx, const int& dimy,
		 float x, float y, U* res)
{
  int ix, iy, ox, oy, ind;
  float fx, fy;

  ix = int(x);         iy = int(y);
  fx = x-float(ix);    fy = y-float(iy);
  ox = 1;              oy = 1;
  
  if (fx<1.e-3 || ix==dimx-1) {fx=0; ox = 0;}
  if (fy<1.e-3 || iy==dimy-1) {fy=0; oy = 0;}
  
  ind = iy*dimx+ix;

  *res = array[ind]-fy*(array[ind]   -array[ind+oy*dimx])
	                 -fx*(array[ind]   -array[ind+ox]
	                 -fy*(array[ind]   -array[ind+oy*dimx]
	                     -array[ind+ox]+array[ind+oy*dimx+ox]));

  /*
  if (*res<0.0 || *res>1.0) {
    cerr << "---" << endl;
    cerr << "(x,y)= " << x <<" "<< y << endl;
    cerr << array[ind] << "\t" << array[ind+ox] << endl;
    cerr << array[ind+oy*dimx] << "\t" << array[ind+oy*dimx+ox] << endl;
    cerr << "---" << endl;
  }
  */
}
//-----------------------------------------------------------------------------
template <class T>
inline void
linearInterpol2D(const T* array1, const T* array2,
		 const int& dimx, const int& dimy,
		 float x, float y, T* res1, T* res2)
{
  int ix, iy, ox, oy, ind;
  float fx, fy;

  ix = int(x);         iy = int(y);
  fx = x-float(ix);    fy = y-float(iy);
  ox = 1;              oy = 1;
  
  if (fx<1.e-5 || ix==dimx-1) {fx=0; ox = 0;}
  if (fy<1.e-5 || iy==dimy-1) {fy=0; oy = 0;}
  
  ind = iy*dimx+ix;
  
  *res1 = array1[ind]-fy*(array1[ind]   -array1[ind+oy*dimx])
                     -fx*(array1[ind]   -array1[ind+ox]
	             -fy*(array1[ind]   -array1[ind+oy*dimx]
	                 -array1[ind+ox]+array1[ind+oy*dimx+ox]));

  *res2 = array2[ind]-fy*(array2[ind]   -array2[ind+oy*dimx])
                     -fx*(array2[ind]   -array2[ind+ox]
	             -fy*(array2[ind]   -array2[ind+oy*dimx]
	                 -array2[ind+ox]+array2[ind+oy*dimx+ox]));

  /*
  if (fabs(*res2)>1.0) {
    cerr << "===" << endl;
    cerr << "(x,y)= " << x <<" "<< y << endl;
    cerr << array2[ind] << "\t" << array2[ind+ox] << endl;
    cerr << array2[ind+oy*dimx] << "\t" << array2[ind+oy*dimx+ox] << endl;
    cerr << "===" << endl;
  }
  */
}


//-----------------------------------------------------------------------------
template <class T>
inline void
directionalInterpol2D(const T* array, const int& dimx, const int& dimy,
		 float x, float y, float vx, float vy, T* res)
{
  int ix, iy, ox, oy, ind;
  float xp, yp;
  
  ix = int(x);           iy = int(y);
  xp = x-float(ix);      yp = y-float(iy);
  ox = 1;                oy = 1;
  
  if (xp<1.e-5 || ix==dimx-1) {xp=0; ox = 0;}
  if (yp<1.e-5 || iy==dimy-1) {yp=0; oy = 0;}

  ind = iy*dimx+ix;

  T a00,a10,a01,a11;
  
  a00 = array[ind];
  a10 = array[ind+ox];
  a01 = array[ind+oy*dimx];
  a11 = array[ind+oy*dimx+ox];
  
  float c1,c2;
  T v1,v2,v;
  
  float m=0;
  float mxp=0;
  int icase = 0;

  
  if (vx<0) {
    vx *= -1.0;   // keep vx positive
    vy *= -1.0;
  }

  if      (ABS(xp)<1.e-5 && ABS(yp)<1.e-5)    {icase = 17;}
  else if (ABS(vx)<1.e-5 || ABS(vy)<1.e-5)    {icase = 0; }
  else {
    
    m   = vy/vx;
    mxp = m*xp;
        
    if (vy >0) {
      if (vx<=vy) {
	if      (vy == vx && yp == xp)        {icase = 4;}
	else if (yp>=mxp)                     {icase = 1;}
	else if (yp<mxp && yp>=mxp+1-m)       {icase = 2;}
	else if (yp<mxp+1-m)                  {icase = 3;}
      }
      else if (vx>vy) {
	if      (yp>=mxp+1-m)                 {icase = 5;}
	else if (yp<mxp+1-m && yp>=mxp)       {icase = 6;}
	else if (yp<mxp)                      {icase = 7;}
      }
    }
    else if (vy<0) {
      if        (vy == -vx && yp == (1.0-xp)) {icase = 12;}
      else if (-vy<=vx) {
	if      (yp>=mxp+1)                   {icase = 9;}
	else if (yp<mxp+1 && yp>=mxp-m)       {icase = 10;}
	else if (yp<mxp-m)                    {icase = 11;}
      }
      else if (-vy>vx){
	if      (yp>=mxp-m)                   {icase = 13;}
	else if (yp>=mxp+1 && yp<mxp-m)       {icase = 14;}
	else if (yp<mxp+1)                    {icase = 15;}
      }
    }
  }

  switch (icase) {
  case 0:
    v1 = yp*(a01-a00)+a00;
    v2 = yp*(a11-a10)+a10;
    v  = xp*(v2 -v1) +v1;
    break;
  case 1:
  case 5:
    c1 = yp-mxp;
    c2 = (1.0-c1)/m;
    
    v1 = c1*(a01-a00)+a00;
    v2 = c2*(a11-a01)+a01;
    v  = (xp/c2)*(v2-v1)+v1;
    break;
  case 2:
  case 14:
    c1 = (mxp-yp)/m;
    c2 = (1.0+mxp-yp)/m;
    
    v1 = c1*(a10-a00)+a00;
    v2 = c2*(a11-a01)+a01;
    v  = yp*(v2 - v1)+v1 ;
    break;
  case 3:
  case 7:
    c1 = (mxp-yp)/m;
    c2 = m+yp-mxp;

    v1 = c1*(a10-a00)+a00;
    v2 = c2*(a11-a10)+a10;
    v  = (yp/c2)*(v2-v1)+v1;
    break;
  case 4:
    v = xp*(a11-a00)+a00;
    break;
  case 6:
  case 10:
    c1 = yp-mxp;
    c2 = m+yp-mxp;

    v1 = c1*(a01-a00)+a00;
    v2 = c2*(a11-a10)+a10;
    v  = xp*(v2 - v1)+v1 ;
    break;
  case 9:
  case 13:
    c1 = (1.0-yp+mxp)/m;
    c2 = m+yp-mxp;

    v1 = c1*(a11-a01)+a01;
    v2 = c2*(a11-a10)+a10;
    v  = ((xp-c1)/(1.0-c1))*(v2-v1)+v1;
    break;
  case 11:
  case 15:
    c1 = yp-mxp;
    c2 = (mxp-yp)/m;

    v1 = c1*(a01-a00)+a00;
    v2 = c2*(a10-a00)+a00;
    v  = (xp/c2)*(v2-v1)+v1;
    break;
  case 12:
    v = xp*(a10-a01)+a01;
    break;
  case 17:
    v = a00;
  }
  
  *res = v;
  
}

#endif //_INTERPOLATION_H_
