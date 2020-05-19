
#include "xp_comm_proj/ellipse/gen.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define PI 3.142

#define Z_STEP 0.2            // Distance between each ellipse

// To ensure that each ellipse has the same major and minor axis
// an offset is added to the supplied x and y dimensions.
//#define X_BASE_RADIUS 0.25    // Offset for x-axis values
//#define Y_BASE_RADIUS 0.5     // Offset for y-axis values

#define ERROR_RETURN(MESS) {\
                ERRerror("EllipseCore",1,ERR_ORIG,MESS);\
                if (x_dims_arr!=NULL) ARRfree(x_dims_arr);\
                if (y_dims_arr!=NULL) ARRfree(y_dims_arr);\
                if (line_conn_arr!=NULL) ARRfree(line_conn_arr);\
                if (quad_conn_arr!=NULL) ARRfree(quad_conn_arr);\
                if (line_coords_arr!=NULL) ARRfree(line_coords_arr);\
                return 0;\
                }


int Ellipse_EllipseMods_EllipseCore::update(OMevent_mask event_mask, int seq_num)
{
  // num_ellipses (OMXint read req notify)  - Total number of ellipses
  // num_points (OMXint read req notify)    - Number of coordinates per ellipse
  // aboutxc (OMXfloat read req notify)     - X coordinate of ellipse centre
  // aboutyc (OMXfloat read req notify)     - Y coordinate of ellipse centre
  // nconn (OMXint write)                   - Number of polyline connections
  // nquads (OMXint write)                  - Number of quads generated
  // ncoords (OMXint write)                 - Total number of coordinates generated

  // x_dims (OMXfloat_array read req notify)  - Input array containing x dimensions of ellipses
  int x_dims_size;
  float *x_dims_arr = NULL;

  // y_dims (OMXfloat_array read req notify)  - Input array containing y dimensions of ellipses
  int y_dims_size;
  float *y_dims_arr = NULL;


  // line_conn (OMXint_array write)           - Output Array containing polyline connectivity
  int line_conn_size;
  int *line_conn_arr = NULL;

  // quad_conn (OMXint_array write)           - Output Array containing quad connectivity
  int quad_conn_size;
  int *quad_conn_arr = NULL;
  
  // line_coords (OMXfloat_array write)       - Output Array containing generated coordinates
  int line_coords_size;
  float *line_coords_arr = NULL;
  

  int i, nume;         // loop variables

  float xc;            // ellipse centre x
  float yc;            // ellipse centre y
  float ellip_xcoeff;  // maximum x dimension
  float ellip_ycoeff;  // maximum y dimension


  // Check validity of starting values
  if (num_ellipses<2) {
    ERROR_RETURN("Cannot generate less than 2 ellipses");
  }
  if (num_points<4) {
    ERROR_RETURN("Cannot generate ellipse with less than 4 points");
  }


  // Calculate numbers of coordinates and connections so that output arrays can be retrived
  ncoords = num_points * num_ellipses;
  nconn = num_ellipses * 2;
  
  // Get input arrays
  x_dims_arr = (float *)x_dims.ret_array_ptr(OM_GET_ARRAY_RD,&x_dims_size);
  if ((x_dims_arr==NULL) || (x_dims_size!=num_ellipses)) {
    ERROR_RETURN("unable to access x dimension array");
  }
  y_dims_arr = (float *)y_dims.ret_array_ptr(OM_GET_ARRAY_RD,&y_dims_size);
  if ((y_dims_arr==NULL) || (y_dims_size!=num_ellipses)) {
    ERROR_RETURN("unable to access y dimension array");
  }

  // Get output coordinate and polyline connectivity arrays
  line_conn_arr = (int *)line_conn.ret_array_ptr(OM_GET_ARRAY_WR,&line_conn_size);
  if ((line_conn_arr==NULL) || (line_conn_size!=nconn)) {
    ERROR_RETURN("unable to access polyline connectivity array");
  }
  line_coords_arr = (float *)line_coords.ret_array_ptr(OM_GET_ARRAY_WR,&line_coords_size);
  if ((line_coords_arr==NULL) || (line_coords_size!=ncoords*3)) {
    ERROR_RETURN("unable to access coordinates array");
  }

  // Loop through each ellipse
  for (nume=0 ; nume < num_ellipses; nume++)
  {
    // Setup centre point.  Individual ellipses could be offset from centre point here.
    xc = aboutxc;
    yc = aboutyc;

    // Use x and y dimensions to setup maximum x and y sizes.
    //ellip_xcoeff = X_BASE_RADIUS + x_dims_arr[nume];
    //ellip_ycoeff = Y_BASE_RADIUS + y_dims_arr[nume];
    ellip_xcoeff = x_dims_arr[nume];
    ellip_ycoeff = y_dims_arr[nume];

    // Loop through each point on current ellipse
    for (i=0; i < (num_points-1); i++)
    {
      // Calculate coordinates of current point
      line_coords_arr[nume*num_points*3+i*3] = 
        ellip_xcoeff*sin( (float)i/(float)(num_points-1) * 2.0 * PI) + xc;
      line_coords_arr[nume*num_points*3+i*3+1] = 
        ellip_ycoeff*cos( (float)i/(float)(num_points-1) * 2.0 * PI) + yc;
      line_coords_arr[nume*num_points*3+i*3+2] = nume * Z_STEP;
    }

    // Set last point to be the first point. This closes the ellipse.
    line_coords_arr[nume*num_points*3+i*3]   = line_coords_arr[nume*num_points*3];
    line_coords_arr[nume*num_points*3+i*3+1] = line_coords_arr[nume*num_points*3+1];
    line_coords_arr[nume*num_points*3+i*3+2] = line_coords_arr[nume*num_points*3+2];

    // Generate polyline connectivity
    line_conn_arr[nume*2]   = nume*num_points;         // start
    line_conn_arr[nume*2+1] = (nume+1)*num_points - 1; // end
  }

  // set nquads prior to getting quad connectivity array
  // NB: N ellipses need N-1 sets of quads
  nquads = (num_points - 1) * (num_ellipses - 1);

  // Get quad connectivity array
  quad_conn_arr = (int *)quad_conn.ret_array_ptr(OM_GET_ARRAY_WR,&quad_conn_size);
  if ((quad_conn_arr==NULL) || (quad_conn_size!=nquads*4)) {
    ERROR_RETURN("unable to access quad connectivity array");
  }

  // Loop through all ellipses and quads
  for (nume = 0; nume < (num_ellipses - 1) ; nume++)
  {
    for (i=0; i < num_points-1 ; i++)
    {
      // Generate quad connectivity
      quad_conn_arr[(num_points-1)*nume*4+i*4]   = num_points*nume + i;
      quad_conn_arr[(num_points-1)*nume*4+i*4+1] = num_points*nume + i + num_points;
      quad_conn_arr[(num_points-1)*nume*4+i*4+2] = num_points*nume + i + num_points + 1;
      quad_conn_arr[(num_points-1)*nume*4+i*4+3] = num_points*nume + i + 1;
    }
  }
  
  // Free allocated arrays

  if (x_dims_arr != NULL) {
    ARRfree(x_dims_arr);
  }

  if (y_dims_arr != NULL) {
    ARRfree(y_dims_arr);
  }

  if (line_conn_arr != NULL) {
    ARRfree(line_conn_arr);
  }
  
  if (quad_conn_arr != NULL) {
    ARRfree(quad_conn_arr);
  }
  
  if (line_coords_arr != NULL) {
    ARRfree(line_coords_arr);
  }


  // return 1 for success
  return(1);
}

/* end of file */

