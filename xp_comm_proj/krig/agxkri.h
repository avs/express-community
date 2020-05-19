/* "$RCSfile: agxkri.h,v $ $Revision: 2.13 $ $Date: 1997/08/18 14:54:17 $"
//
//                      Copyright (c) 1996 by
//                      Advanced Visual Systems Inc.
//                      All Rights Reserved
//
//      This software comprises unpublished confidential information of
//      Advanced Visual Systems Inc. and may not be used, copied or made
//      available to anyone, except in accordance with the license
//      under which it is furnished.
//
//      This file is under CVS/RCS control in:
//      $Source: /build1/toolmstr/cvs/repository/agx/inc/agX/agxkri.h,v $
//
// UNIRAS function declarations for KRI
//--------------------------------------------------------------------*/

#ifndef _agxkri_h
#define _agxkri_h

#ifndef _agx64_h
#ifndef _agx32_h
#include <agX/agx32.h>
#endif
#endif

#ifndef NeedFunctionPrototypes
#if defined(FUNCPROTO) || defined(__STDC__) || defined(__cplusplus)
#define NeedFunctionPrototypes 1
#else
#define NeedFunctionPrototypes 0
#endif
#endif                                         /* NeedFunctionPrototypes */

#ifndef NeedWidePrototypes
#if defined(NARROWPROTO)
#define NeedWidePrototypes 0
#else
#define NeedWidePrototypes 1        /* default to make interropt. easier */
#endif
#endif

#ifdef __cplusplus                  /* do not leave open across includes */
extern "C" {                                            /* for C++ V2.0 */
#endif

#ifndef _F77Float
#define _F77Float
typedef float F77Float;
#endif
/*                                                                       */
/* XuKrigingSemivarBlockCalc - Calculate semi-variograms for bore-hole   */
/*                                                                       */
extern XuType XuCallCnv XuKrigingSemivarBlockCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xarr  , /* ra  in  X-coordinates of core sections           */
  void      *yarr  , /* ra  in  Y-coordinates of core sections           */
  void      *zarr  , /* ra  in  Z-coordinates of core sections           */
  void      *tdn   , /* ra  in  Bore-hole identifiers                    */
  void      *value , /* ra  in  Values for data points                   */
  int       np     , /* i   in  Number of data points                    */
  void      *hlag  , /* ra  in  Lag in each of three directions          */
  int       nh     , /* i   in  Number of lags                           */
  int       nb     , /* i   in  Number of bore holes                     */
  int       *nbhl  , /* ia  in  \XWroutine work array                    */
  void      *semv  , /* ra  out Calculated values for each lag           */
  int       *npair   /* ia  out Number of data point pairs at lag        */
#endif
);
/*                                                                       */
/* XuKrigingSphericalModel - Set spherical model for block kriging       */
/*                                                                       */
extern XuType XuCallCnv XuKrigingSphericalModel (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *a    , /* ra  in  Influence range of each component         */
  void      *c    , /* ra  in  Sills for each component                  */
  int       msph  , /* i   in  Number of spherical components            */
  double    co    , /* r   in  Nugget effect                             */
  double    anie  , /* r   in  Anisotropy factor in X-direction          */
  double    anin  , /* r   in  Anisotropy factor in Y-direction          */
  double    anib    /* r   in  Anisotropy factor in Z-direction          */
#endif
);
/*                                                                       */
/* XuKrigingSemivar - Set semi-variogram calculation parameters          */
/*                                                                       */
extern XuType XuCallCnv XuKrigingSemivar (
#if NeedFunctionPrototypes
/*                                                                       */
  double    vsrch   , /* r   in  Deviation or tolerance (degrees)        */
  double    hsrch   , /* r   in  Direction or tolerance (degrees)        */
  double    hvsear    /* r   in  Tolerance (degrees)                     */
#endif
);
/*                                                                       */
/* XuKrigingSearchVolume - Define search volume for block kriging        */
/*                                                                       */
extern XuType XuCallCnv XuKrigingSearchVolume (
#if NeedFunctionPrototypes
/*                                                                       */
  double    xsrch  , /* r   in  Length in X-direction of search volume    */
  double    ysrch  , /* r   in  Length in Y-direction of search volume    */
  double    zsrch    /* r   in  Length in Z-direction of search volume    */
#endif
);
/*                                                                       */
/* XuKrigingBenchCompositeCalc - Calculate bench composites              */
/*                                                                       */
extern XuType XuCallCnv XuKrigingBenchCompositeCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xa    , /* ra  in  X-coordinates of core sections           */
  void      *ya    , /* ra  in  Y-coordinates of core sections           */
  void      *za    , /* ra  in  Z-coordinates of core sections           */
  void      *tdn   , /* ra  in  Bore-hole identifiers                    */
  void      *gth   , /* ra  in  Lengths of core sections                 */
  void      *value , /* ra  in  Values of core sections                  */
  int       np     , /* i   in  Number of data points                    */
  int       npz    , /* i   in  Number of benches                        */
  int       nc     , /* i   in  Number of bench composites               */
  void      *xc    , /* ra  out X-coordinates of bench composites        */
  void      *yc    , /* ra  out Y-coordinates of bench composites        */
  void      *zc    , /* ra  out Z-coordinates of bench composites        */
  void      *gthc  , /* ra  out Lengths of composites                    */
  void      *comp  , /* ra  out Values of composites                     */
  int       *ncomp   /* i   out Number of composites                     */
#endif
);
/*                                                                       */
/* XuKrigingGeoregression - Set georegression option for GKRBLK          */
/*                                                                       */
extern XuType XuCallCnv XuKrigingGeoregression (
#if NeedFunctionPrototypes
/*                                                                       */
  double    umhat  , /* r   in  Estimate of mean value                   */
  double    umds   , /* r   in  Estimate of standard deviation           */
  int       nu       /* i   in  Nugget effect option                     */
#endif
);
/*                                                                       */
/* XuKrigingBlockCalc - Perform block kriging from bore-hole data        */
/*                                                                       */
extern XuType XuCallCnv XuKrigingBlockCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xarr  , /* ra  in  X-coordinates of core sections           */
  void      *yarr  , /* ra  in  Y-coordinates of core sections           */
  void      *zarr  , /* ra  in  Z-coordinates of core sections           */
  void      *gth   , /* ra  in  Lengths of core sections                 */
  void      *value , /* ra  in  Values for core sections                 */
  int       np     , /* i   in  Number of data points                    */
  int       nz     , /* i   in  Number of blocks in Z direction          */
  int       ny     , /* i   in  Number of blocks in Y direction          */
  int       nx     , /* i   in  Number of blocks in X direction          */
  void      *zest  , /* ra  out Estimated block values                   */
  void      *zstd    /* ra  out Standard deviations of block values      */
#endif
);
/*                                                                       */
/* XuKriging2DSemivarCalc - Calculate semi-variogram for 2D data         */
/*                                                                       */
extern XuType XuCallCnv XuKriging2DSemivarCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xarr  , /* ra  in  X-coordinates of samples                 */
  void      *yarr  , /* ra  in  Y-coordinates of samples                 */
  void      *value , /* ra  in  Sample values                            */
  int       np     , /* i   in  Number of samples                        */
  double    h      , /* r   in  Lag                                      */
  int       nh     , /* i   in  Number of lags                           */
  double    deg    , /* r   in  Direction (degrees)                      */
  double    tol    , /* r   in  Tolerance (degrees)                      */
  void      *semv  , /* ra  out Semi-variogram values at each lag        */
  int       *npair   /* ia  out Number of sample pairs for each lag      */
#endif
);
/*                                                                       */
/* XuKriging3DSemivarCalc - Calculate semi-variogram for 3D data         */
/*                                                                       */
extern XuType XuCallCnv XuKriging3DSemivarCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xarr  , /* ra  in  X-coordinates of samples                 */
  void      *yarr  , /* ra  in  Y-coordinates of samples                 */
  void      *zarr  , /* ra  in  Z-coordinates of samples                 */
  void      *value , /* ra  in  Sample values                            */
  int       np     , /* i   in  Number of samples                        */
  int       h      , /* i   in  Lag                                      */
  int       nh     , /* i   in  Number of lags                           */
  void      *semv  , /* ra  out Semi-variogram values at each lag        */
  int       *npair   /* ia  out Number of sample pairs for each lag      */
#endif
);
/*                                                                       */
/* XuKrigingFault - Define fault lines                                   */
/*                                                                       */
extern XuType XuCallCnv XuKrigingFault (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *x , /* ra  in  X-coordinates of fault lines                 */
  void      *y , /* ra  in  Y-coordinates of fault lines                 */
  int       n    /* i   in  Number of coordinate pairs                   */
#endif
);
/*                                                                       */
/* XuKrigingRegionPolygon - Define convex polygon                        */
/*                                                                       */
extern XuType XuCallCnv XuKrigingRegionPolygon (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xp , /* ra  in  X-coordinates of polygon vertices           */
  void      *yp , /* ra  in  Y-coordinates of polygon vertices           */
  int       n     /* i   in  Number of vertices                          */
#endif
);
/*                                                                       */
/* XuKrigingEquations - Set number of kriging equations                  */
/*                                                                       */
extern XuType XuCallCnv XuKrigingEquations (
#if NeedFunctionPrototypes
/*                                                                       */
  int       m    /* i   in  Maximum number samples                       */
#endif
);
/*                                                                       */
/* XuKrigingOpen - Open KRIGING                                          */
/*                                                                       */
extern XuType XuCallCnv XuKrigingOpen (
#if NeedFunctionPrototypes
#endif
);
/*                                                                       */
/* XuKriging2DCalc - Gridding in 2D by kriging                           */
/*                                                                       */
extern XuType XuCallCnv XuKriging2DCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xarr   , /* ra  in  X-coordinates of samples                */
  void      *yarr   , /* ra  in  Y-coordinates of samples                */
  void      *value  , /* ra  in  Sample values                           */
  int       np      , /* i   in  Number of samples                       */
  double    radius  , /* r   in  Search radius                           */
  int       ny      , /* i   in  Number of grid nodes in Y direction     */
  int       nx      , /* i   in  Number of grid nodes in X direction     */
  void      *zest   , /* ra  out Estimated values at grid nodes          */
  void      *zstd   , /* ra  out Standard deviations at grid nodes       */
  float     (*gam)(F77Float *)  /* rp  in  Semi-variogram function       */
#endif
);
/*                                                                       */
/* XuKriging2DPointCalc - Point kriging estimation in 2D                 */
/*                                                                       */
extern XuType XuCallCnv XuKriging2DPointCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xarr   , /* ra  in  X-coordinates of samples                */
  void      *yarr   , /* ra  in  Y-coordinates of samples                */
  void      *value  , /* ra  in  Sample values                           */
  int       np      , /* i   in  Number of samples                       */
  double    radius  , /* r   in  Search radius                           */
  void      *est    , /* ra  out Sample estimates                        */
  void      *s      , /* ra  out Sample variances                        */
  float     (*gam)(F77Float *)  /* rp  in  Semi-variogram function       */
#endif
);
/*                                                                       */
/* XuKriging3DCalc - Gridding in 3D by kriging                           */
/*                                                                       */
extern XuType XuCallCnv XuKriging3DCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xarr   , /* ra  in  X-coordinates of samples                */
  void      *yarr   , /* ra  in  Y-coordinates of samples                */
  void      *zarr   , /* ra  in  Z-coordinates of samples                */
  void      *value  , /* ra  in  Sample values                           */
  int       np      , /* i   in  Number of data points                   */
  double    radius  , /* r   in  Search radius                           */
  int       nz      , /* i   in  Number of grid nodes in Z direction     */
  int       ny      , /* i   in  Number of grid nodes in Y direction     */
  int       nx      , /* i   in  Number of grid nodes in X direction     */
  void      *zest   , /* ra  out Estimated values at grid nodes          */
  void      *zstd   , /* ra  out Standard deviations at grid nodes       */
  float     (*gam)(F77Float *)  /* rp  in  Semi-variogram function       */
#endif
);
/*                                                                       */
/* XuKriging3DPointCalc - Point kriging estimation in 3D                 */
/*                                                                       */
extern XuType XuCallCnv XuKriging3DPointCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xarr   , /* ra  in  X-coordinates of samples                */
  void      *yarr   , /* ra  in  Y-coordinates of samples                */
  void      *zarr   , /* ra  in  Y-coordinates of samples                */
  void      *value  , /* ra  in  Sample values                           */
  int       np      , /* i   in  Number of samples                       */
  double    radius  , /* r   in  Search radius                           */
  void      *est    , /* ra  out Sample estimates                        */
  void      *s      , /* ra  out Sample variances                        */
  float     (*gam)(F77Float *)  /* rp  in  Semi-variogram function       */
#endif
);
/*                                                                       */
/* XuKrigingSemivarDraw - Plot experimental semi-variogram               */
/*                                                                       */
extern XuType XuCallCnv XuKrigingSemivarDraw (
#if NeedFunctionPrototypes
/*                                                                       */
  double    h      , /* r   in  Lag                                      */
  void      *semv  , /* ra  in  Semi-variogram values at each lag        */
  int       *npair , /* ia  in  Number of sample pairs for each lag      */
  int       nh     , /* i   in  Number of lags                           */
  int       iop    , /* i   in  Option                                   */
  float     (*gam)(F77Float *)  /* rp  in  Semi-variogram function       */
#endif
);
/*                                                                       */
/* XuKrigingMeanCalc - Calculate mean and variance                       */
/*                                                                       */
extern XuType XuCallCnv XuKrigingMeanCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *x  , /* ra  in  Input values                                */
  int       ns  , /* i   in  Number of input values                      */
  XuFloat   *xm , /* r   out Statistical mean of X-values                */
  XuFloat   *xv   /* r   out Statistical variance of X-values            */
#endif
);
/*                                                                       */
/* XuKriging2DUniversalCalc - Gridding in 2D by universal kriging        */
/*                                                                       */
extern XuType XuCallCnv XuKriging2DUniversalCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xarr   , /* ra  in  X-coordinates of samples                */
  void      *yarr   , /* ra  in  Y-coordinates of samples                */
  void      *value  , /* ra  in  Sample values                           */
  int       np      , /* i   in  Number of samples                       */
  int       nd      , /* i   in  Number of terms in trend function       */
  double    radius  , /* r   in  Search radius                           */
  int       ny      , /* i   in  Number of grid nodes in Y direction     */
  int       nx      , /* i   in  Number of grid nodes in X direction     */
  void      *zest   , /* ra  out Estimated values at grid nodes          */
  void      *zstd   , /* ra  out Standard deviations at grid nodes       */
  float     (*gam)(F77Float *), /* rp  in  Semi-variogram function       */
  void      (*trend)(F77Float *,F77Float *,int *,F77Float *)  /* fp  in  Terms in trend function    */
#endif
);
/*                                                                       */
/* XuKriging2DUniversalPointCalc - Universal point kriging estimation in */
/*                                                                       */
extern XuType XuCallCnv XuKriging2DUniversalPointCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *xarr   , /* ra  in  X-coordinates of samples                */
  void      *yarr   , /* ra  in  Y-coordinates of samples                */
  void      *value  , /* ra  in  Sample values                           */
  int       np      , /* i   in  Number of samples                       */
  int       nd      , /* i   in  Number of terms in trend function       */
  double    radius  , /* r   in  Search radius                           */
  void      *est    , /* ra  out Sample estimates                        */
  void      *s      , /* ra  out Sample variances                        */
  float     (*gam)(F77Float *), /* rp  in  Semi-variogram function       */
  void      (*trend)(F77Float *,F77Float *,int *,F77Float *)  /* fp  in  Terms in trend function    */
#endif
);
/*                                                                       */
/* XuKrigingPointInfo - Write point kriging information                  */
/*                                                                       */
extern XuType XuCallCnv XuKrigingPointInfo (
#if NeedFunctionPrototypes
/*                                                                       */
  int       lun  , /* i   in  Fortran logical unit number                */
  int       n      /* i   in  Number of points to examined               */
#endif
);

#ifdef __cplusplus
}                                                        /* for C++ V2.0 */
#endif

#endif /* _agxkri_h */
