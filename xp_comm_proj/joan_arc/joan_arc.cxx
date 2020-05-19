/*************************************************************************/
/*
 * Joan of Arc Module
 *
 * Takes two end points and a height, and generates a polyline
 * arc joining the two points.
 *
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "xp_comm_proj/joan_arc/gen.hxx"


// AVS/Express header files should define this.
#ifndef       M_PI
#define       M_PI    3.141592653589793238462643383279
#endif


#define FREE_ARRAYS {\
   if (start_arr!=NULL) ARRfree((char *)start_arr);\
   if (end_arr!=NULL) ARRfree((char *)end_arr);\
   if (vert_arr!=NULL) ARRfree((char *)vert_arr);\
   if (polyline_arr!=NULL) ARRfree((char *)polyline_arr);\
   if (connect_arr!=NULL) ARRfree((char *)connect_arr);\
}


/*************************************************************************/
int
JoanArc_JoanArcMods_JoanArcCore::update(OMevent_mask event_mask, int seq_num)
{

 // start (OMXfloat_array read req notify)
 int start_size;
 float *start_arr = NULL;

 // end (OMXfloat_array read req notify)
 int end_size;
 float *end_arr = NULL;

 // vert (OMXfloat_array read req notify)
 int vert_size;
 float *vert_arr = NULL;

 // nseg (OMXint read req notify)

 // polyline_out (OMXfloat_array write)
 int polyline_size;
 float *polyline_arr = NULL;

 // connections_out (OMXint_array write)
 int connect_size, calc_connect_size;
 int *connect_arr = NULL;


 // Function's Body
 if (nseg <= 1) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Line must contain at least 2 segments.\n");
      return (0);
 }

 int l_nseg = nseg;  // Get local copy of OMX object value

 // get the start and end points

 start_arr = (float *)start.ret_array_ptr(OM_GET_ARRAY_RD, &start_size);
 if (start_arr==NULL) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Can't get start_arr.\n");
      FREE_ARRAYS;
      return (0);
 }

 end_arr = (float *)end.ret_array_ptr(OM_GET_ARRAY_RD, &end_size);
 if (end_arr==NULL) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Can't get end_arr.\n");
      FREE_ARRAYS;
      return (0);
 }
 vert_arr = (float *)vert.ret_array_ptr(OM_GET_ARRAY_RD, &vert_size);
 if (vert_arr==NULL) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Can't get vert_arr.\n");
      FREE_ARRAYS;
      return (0);
 }

 //
 // sanity check the input
 //
 if ( start_size != end_size ) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"start and end arrays must match size\n");
      FREE_ARRAYS;
      return (0);
 }
 if ( (start_size/3) != vert_size ) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"vert array must be 1/3 start size\n");
      FREE_ARRAYS;
      return (0);
 }

 polyline_size = l_nseg * start_size;
 calc_connect_size = 2 * start_size/3;

 // set the array sizes first since they don't exist
 ncoords = polyline_size / 3;
 OMset_array_size(connections_out, calc_connect_size);

 polyline_arr = (float *)polyline_out.ret_array_ptr(OM_GET_ARRAY_WR, &polyline_size);
 if (polyline_arr==NULL) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Can't get polyline_arr.\n");
      FREE_ARRAYS;
      return (0);
 }
 if (polyline_size != (ncoords*3)) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT, "polyline_arr is incorrect size.\n");
      FREE_ARRAYS;
      return (0);
 }

 connect_arr = (int *)connections_out.ret_array_ptr(OM_GET_ARRAY_WR, &connect_size);
 if (connect_arr==NULL) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Can't get connect_arr.\n");
      FREE_ARRAYS;
      return (0);
 }
 if (connect_size != calc_connect_size) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT, "connect_arr is incorrect size.\n");
      FREE_ARRAYS;
      return (0);
 }


 // Declare loop variables
 int k = 0;
 int connect_count, coord_count;

 float long_incr,lat_incr,alt_incr;

 double pi_div_nseg = M_PI / (double)(l_nseg-1);


 // Loop to all the points
 for (coord_count = 0, connect_count=0; 
     coord_count < start_size; 
     coord_count += 3, connect_count += 2)
 {
     // calculate the increments for longitude
     long_incr = -1 * (start_arr[coord_count] - end_arr[coord_count]) / (l_nseg-1);
     
     // calculate the increments for latitude
     lat_incr = -1 * (start_arr[coord_count + 1] - end_arr[coord_count + 1]) / (l_nseg-1);
     
     // calculate the increments for altitude
     alt_incr = -1 * (start_arr[coord_count + 2] - end_arr[coord_count + 2]) / (l_nseg-1);
     
     // calculate the points on the arc and stick them into two arrays 
     // for coords1 & coords2 of the line disjoint mesh
     for (int i = 0; i < l_nseg; i++, k+= 3)
     {
         polyline_arr[k]   = (float) start_arr[coord_count] + (i * long_incr);
         polyline_arr[k+1] = (float) start_arr[coord_count + 1] + (i * lat_incr);
         //polyline_arr[k+2] = (float) start_arr[coord_count + 2] + (i * alt_incr);
         polyline_arr[k+2] = (float) start_arr[coord_count + 2] + (i * alt_incr)
                              + (vert_arr[coord_count/3] * sin((double)i * pi_div_nseg) );
     }
     
     // set connection list to match the node list just generated.
     connect_arr[connect_count] = connect_count * l_nseg/2;
     connect_arr[connect_count+1] = connect_count * l_nseg/2 + l_nseg - 1 ;
 }
 
 FREE_ARRAYS;
 
 // return 1 for success
 return(1);
}

/*************************************************************************/
// end of file

