/* "$RCSfile: agxint.h,v $ $Revision: 2.12 $ $Date: 1997/08/18 14:54:14 $"
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
//      $Source: /cvs/repository/agx/inc/agX/agxint.h,v $
//
// UNIRAS function declarations for INT
//--------------------------------------------------------------------*/

#ifndef _agxint_h
#define _agxint_h

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
/*                                                                       */
/* XuGriddingLocalFitDelta - Set gridding resolution                     */
/*                                                                       */
extern XuType XuCallCnv XuGriddingLocalFitDelta (
#if NeedFunctionPrototypes
/*                                                                       */
  double    delta    /* r   in  Distance                                 */
#endif
);
/*                                                                       */
/* XuGriddingLocalFitMethod - Set interpolation method                   */
/*                                                                       */
extern XuType XuCallCnv XuGriddingLocalFitMethod (
#if NeedFunctionPrototypes
/*                                                                       */
  int       method_code    /* i   in  Method code                        */
#endif
);
/*                                                                       */
/* method_code: Distance weighted average                                */
#ifndef XuWEIGHTED_AVERAGE
#define XuWEIGHTED_AVERAGE 1
#endif
/* method_code: Distance weighted average (2 quadrants)                  */
#ifndef XuWEIGHTED_AVERAGE_2Q
#define XuWEIGHTED_AVERAGE_2Q 4
#endif
/* method_code: Distance weighted average (3 quadrants)                  */
#ifndef XuWEIGHTED_AVERAGE_3Q
#define XuWEIGHTED_AVERAGE_3Q 5
#endif
/* method_code: Distance weighted average (4 quadrants)                  */
#ifndef XuWEIGHTED_AVERAGE_4Q
#define XuWEIGHTED_AVERAGE_4Q 6
#endif
/* method_code: Bilinear interpolation                                   */
#ifndef XuBILINEAR
#define XuBILINEAR 2
#endif
/* method_code: Bilinear interpolation (4 quadrants)                     */
#ifndef XuBILINEAR_STRICT
#define XuBILINEAR_STRICT 3
#endif
/*                                                                       */
/*                                                                       */
/* XuGriddingThinPlateSplineCalc - Gridding of scattered point data      */
/*                                                                       */
extern XuType XuCallCnv XuGriddingThinPlateSplineCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *x_array      , /* ra  in  X-coordinates of samples          */
  void      *y_array      , /* ra  in  Y-coordinates of samples          */
  void      *v_array      , /* ra  in  Values at sample points           */
  int       num_points    , /* i   in  Number of samples                 */
  void      *result_array , /* ra  out Estimated values at grid nodes    */
  int       num_y         , /* i   in  Number of grid nodes in Y direction    */
  int       num_x           /* i   in  Number of grid nodes in X direction    */
#endif
);
/*                                                                       */
/* XuGriddingFastLocalFitCalc - Gridding of scattered point data         */
/*                                                                       */
extern XuType XuCallCnv XuGriddingFastLocalFitCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *x_array      , /* ra  in  X-coordinates of control points    */
  void      *y_array      , /* ra  in  Y-coordinates of control points    */
  void      *v_array      , /* ra  in  Data values at the control points    */
  int       num_points    , /* i   in  Number of control points          */
  void      *result_array , /* ra  out Gridded data                      */
  int       num_y         , /* i   in  Number of grid nodes in Y direction    */
  int       num_x           /* i   in  Number of grid nodes in X direction    */
#endif
);
/*                                                                       */
/* XuGriddingTriangulationCalc - Triangulation based point gridding      */
/*                                                                       */
extern XuType XuCallCnv XuGriddingTriangulationCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *x_array      , /* ra  in  X-coordinates of control points    */
  void      *y_array      , /* ra  in  Y-coordinates of control points    */
  void      *v_array      , /* ra  in  Data values at the control points    */
  int       num_points    , /* i   in  Number of data points             */
  void      *result_array , /* ra  out Gridded data                      */
  int       num_y         , /* i   in  Number of grid nodes in Y direction    */
  int       num_x         , /* i   in  Number of grid nodes in X direction    */
  int       *int_array    , /* ia  in  Work array                        */
  void      *float_array    /* ra  in  Work array                        */
#endif
);
/*                                                                       */
/* XuGriddingLeastSquaresCalc - Trend surface point gridding             */
/*                                                                       */
extern XuType XuCallCnv XuGriddingLeastSquaresCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *x_array       , /* ra  in  X-coordinates of control points    */
  void      *y_array       , /* ra  in  Y-coordinates of control points    */
  void      *v_array       , /* ra  in  Data values at the control points    */
  int       num_points     , /* i   in  Number of control points         */
  void      *result_array  , /* ra  out Gridded data                     */
  int       num_y          , /* i   in  Number of grid nodes in Y direction    */
  int       num_x          , /* i   in  Number of grid nodes in X direction    */
  double    side_length    , /* r   in  Rectangle side length            */
  int       num_y_subgrid  , /* i   in  No.\ of subgrid nodes in Y direction    */
  int       num_x_subgrid    /* i   in  No.\ of subgrid nodes in X direction    */
#endif
);
/*                                                                       */
/* XuGriddingLocalFitCalc - Gridding of scattered point data             */
/*                                                                       */
extern XuType XuCallCnv XuGriddingLocalFitCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *x_array      , /* ra  in  X-coordinates of control points    */
  void      *y_array      , /* ra  in  Y-coordinates of control points    */
  void      *v_array      , /* ra  in  Data values at the control points    */
  int       num_points    , /* i   in  Number of control points          */
  void      *result_array , /* ra  out Gridded data                      */
  int       num_y         , /* i   in  Number of grid nodes in Y direction    */
  int       num_x           /* i   in  Number of grid nodes in X direction    */
#endif
);
/*                                                                       */
/* XuGridResamplingCalc - Grid resampling                                */
/*                                                                       */
extern XuType XuCallCnv XuGridResamplingCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *array        , /* ra  in  Original grid                     */
  int       num_y         , /* i   in  Number of rows in original grid    */
  int       num_x         , /* i   in  Number of columns in original grid    */
  void      *result_array , /* ra  out Resampled grid                    */
  int       num_y_result  , /* i   in  Number of rows in new grid        */
  int       num_x_result    /* i   in  Number of columns in new grid     */
#endif
);
/*                                                                       */
/* XuGriddingSmoothLevel - Set gridding smoothing level                  */
/*                                                                       */
extern XuType XuCallCnv XuGriddingSmoothLevel (
#if NeedFunctionPrototypes
/*                                                                       */
  int       level    /* i   in  Smoothing level                          */
#endif
);
/*                                                                       */
/* XuGriddingLocalFitFilter - Set quadratic interpolation parameters     */
/*                                                                       */
extern XuType XuCallCnv XuGriddingLocalFitFilter (
#if NeedFunctionPrototypes
/*                                                                       */
  double    radius  , /* r   in  Radius factor                           */
  double    factor    /* r   in  Feedback factor                         */
#endif
);
/*                                                                       */
/* XuGriddingLocalFitRadius - Set search radius                          */
/*                                                                       */
extern XuType XuCallCnv XuGriddingLocalFitRadius (
#if NeedFunctionPrototypes
/*                                                                       */
  double    radius    /* r   in  Radius of search circle (user units)    */
#endif
);
/*                                                                       */
/* XuGriddingPartCalc - Regrid a part of a grid                          */
/*                                                                       */
extern XuType XuCallCnv XuGriddingPartCalc (
#if NeedFunctionPrototypes
/*                                                                       */
  void      *array      , /* ra  io  Gridded data                        */
  int       num_y       , /* i   in  Number of grid nodes in Y direction    */
  int       num_x       , /* i   in  Number of grid nodes in X direction    */
  void      *x_array    , /* ra  in  X-coordinates of control points     */
  void      *y_array    , /* ra  in  Y-coordinates of control points     */
  void      *v_array    , /* ra  in  Data values at the control points    */
  int       num_points  , /* i   in  Number of control points            */
  void      *x_poly     , /* ra  in  X-coordinates of polygon vertices    */
  void      *y_poly     , /* ra  in  Y-coordinates of polygon vertices    */
  int       num_poly      /* i   in  Number of polygon vertices          */
#endif
);

#ifdef __cplusplus
}                                                        /* for C++ V2.0 */
#endif

#endif /* _agxint_h */
