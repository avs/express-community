/*
 */

/* ----------------------------------------------------------------------
 * CFDRangeNodeData Module
 * ----------------------------------------------------------------------
 * Description:
 *   
 *   CFD Range Module, transforms node data from input to output,
 * 
 * Author: Ian Curington, March 3, 1997
 * 
 * Revision:
 *    4 March 97 Ian Curington: V3.1 on W95 port testing
 *   12 March 97 Ian Curington: min-max locally, fewer OM calls
 *                              new mem alloc scheme for output fields
 *   27 May   97 Ian Curington: Derived Range from CFD Values module
 *    4 June  97 Ian Curington: parameter update
 *    4 Feb   98 Paul G. Lever: modified for IAC repository
 *
 * ----------------------------------------------------------------------
 * Note:
 *   The gen.h include file is generated by Express when the module is 
 *   compiled. It avoids including "user.h" or "express.h" directly, so 
 *   that the module may be moved across the processes by changing the V
 *   properties in the library.
 * ----------------------------------------------------------------------
 */

#include "xp_comm_proj/cfdrange/gen.h"

/* to enable debug message, please uncomment the following symbol:	 */

/* #define DEBUG_PRINT 1	 */


/* MACROS	 */

/* Error path: use ERRerror instead of printf,
 * since it reports nested context in messages
 */

#define CFD_ERROR(MESS) {ERRerror("CFD Values",1,ERR_ORIG,MESS);return 0;}
#define SILENT_RTN(MESS) { return 0; }

#define MAX_BLOCKS 200
#define MAX_VECLEN 6

#define CFD_MAXFLOAT ((float)3.0e+30)

static scan_minmax_range ( float *, int, int, float *, float *, int );

/*********************************
 * Express Module Entry Point    *
 *********************************/

int
cfd_range_update(OMobj_id cfd_range_id, OMevent_mask event_mask, int seq_num)
{
  /***********************/
  /*  Declare variables  */
  /***********************/

  OMobj_id     in_id, in_fields[MAX_BLOCKS];
  int          in_ncomp, in_comp_count, in_veclen, local_comp_count;
  int          out_ncomp, out_comp_count, out_veclen;
  int          in_data_type, in_ndata;
  float        *values;
  double       min_param;
  double       max_param;
  int          lock_mode;
  int          nnodes, nblocks, nbndims, nbdims[20];
  int          i,j,m;
  int          interrupt;
  float        min, max, rmin[100], rmax[100];

  OMstatus_check(1,"cfd values started",&interrupt);
  /***********************/
  /*  Get input values   */
  /***********************/

  /* Get min's value */
  if (OMget_name_real_val(cfd_range_id,     /* id of whole module */
			  OMstr_to_name("min"), /* name of param */
			  &min_param) != 1)           /* return value */
    CFD_ERROR("Could not get min param value.");

   /* Get max value */
  if (OMget_name_real_val(cfd_range_id,     /* id of whole module */
			  OMstr_to_name("max"), /* name of param */
			  &max_param) != 1)           /* return value */
    CFD_ERROR("Could not get max param value.");

   /* Get lock_mode value */
  if (OMget_name_int_val(cfd_range_id,     /* id of whole module */
			 OMstr_to_name("lock_mode"), /* name of param */
			 &lock_mode) != 1)           /* return value */
    CFD_ERROR("Could not get lock_mode param value.");


  /* Get the field id's so we can access the fields */
  /* Get field id */
  in_id= OMfind_subobj(cfd_range_id, OMstr_to_name("in"), OM_OBJ_RW);

  OMget_array_dims( in_id, &nbndims, nbdims );
  if ( nbndims <= 0 )
    CFD_ERROR("input multiblock list empty");
  if ( nbndims >= 2 )
    CFD_ERROR("input multiblock list has too many dimensions");
  if ( nbdims[0] <= 0 )
    CFD_ERROR("input multiblock list empty");
  nblocks = nbdims[0];

  /*
   * establish id's for each block in both input and ouput
   */
  for (i=0; i< nblocks; i++) {
    if ((OMget_array_val(in_id, i, in_fields+i, OM_OBJ_RD)) != 1)
      CFD_ERROR("could not obtain obj id of sub-field in input mblock");
  }
  OMstatus_check(5,"cfd values params resolve",&interrupt);

  /*
   * Loop over all Fields in Multi-Block Structure
   */
  for (j=0; j< nblocks; j++) {
    OMstatus_check(10+((j*70)/nblocks),"cfd range",&interrupt);

    in_id = in_fields[j];

    /* get the number of nodes */
    if (FLDget_nnodes (in_id, &nnodes) != 1) 
      CFD_ERROR("error setting output nnodes");

    /* Get number of node data components */
    FLDget_node_data_ncomp (in_id, &in_ncomp);
    
    
    /* loop over each component */
    for (in_comp_count=0; in_comp_count < in_ncomp; in_comp_count++) {
      FLDget_node_data_veclen (in_id, in_comp_count, &in_veclen);
      if ( in_veclen <= 0 || in_veclen > MAX_VECLEN)
	CFD_ERROR("unexpected input veclen");

      /*
       * get the input field values prior to computation
       */
      if (FLDget_node_data (in_id, in_comp_count,
			    &in_data_type, &values, &in_ndata, OM_GET_ARRAY_RW) != 1)
	CFD_ERROR("Could not get input field node data.");


      if ( in_ndata != nnodes )
	CFD_ERROR("problem with number of nodes not matching on two calls");
    

      /*************************
       * range check           *
       *************************/

      scan_minmax_range ( values, nnodes, in_veclen,
			  &(rmin[in_comp_count*MAX_VECLEN]),
			  &(rmax[in_comp_count*MAX_VECLEN]),j );
    
    
      /*************************
       * Clean Up              *
       *************************/

      ARRfree((char *)values);


    } /* end of component loop */
       
  } /* end of multi-block loop */


  OMstatus_check(83,"cfd values set",&interrupt);

  /*
   * Set the Min-Max ranges on all components of all blocks for consistancy
   */
  if ( lock_mode ) {
    for( m = 0; m < 100; m++ ) {
      rmin[m] = min_param;
      rmax[m] = max_param;
    }
  }

  for ( j = 0; j < nblocks; j++ )	{
    if (FLDget_node_data_ncomp (in_fields[j], &in_ncomp) != 1) {
      CFD_ERROR("Error setting nnode_data");
    }
    for( m = 0; m < in_ncomp; m++ ) {
      if (FLDset_node_data_minmax(in_fields[j], m, 
				  (char *)&rmin[m*MAX_VECLEN],
				  (char *)&rmax[m*MAX_VECLEN], DTYPE_FLOAT) != 1) {
	CFD_ERROR("Error setting minmax data");
      }
    }
  }
  
  if ( lock_mode == 0 ) {
    /* Set min & max value back to interface */
    min_param = rmin[0];
    max_param = rmax[0];
    
    if (OMset_name_real_val(cfd_range_id,     /* id of whole module */
			    OMstr_to_name("min"), /* name of param */
			    min_param) != 1)      /* return value */
      CFD_ERROR("Could not set min param value.");
    
    if (OMset_name_real_val(cfd_range_id,     /* id of whole module */
			    OMstr_to_name("max"), /* name of param */
			    max_param) != 1)      /* return value */
      CFD_ERROR("Could not set max param value.");
  }
  
  
  OMstatus_check(99,"cfd range finished",&interrupt);
  
  /*
   * return to Object Manager with success flag
   */
  return(1);
}

/*
 ************************************************************
 *       Numerical Routines for derived quantities          *
 ************************************************************
 */

static scan_minmax_range ( in_data, nnodes, veclen, rmin, rmax , block )
     float *in_data, *rmin, *rmax;
     int   nnodes, veclen, block;   
{
  int i,j;
  
  if (block == 0)
    {
      for( j = 0; j < veclen; j++ )
	rmin[j] = rmax[j] = in_data[j];
    }
  
  for( j = 0; j < veclen; j++ )
    {
      for( i = 0; i < nnodes; i++ ) {
	if (in_data[i*veclen+j] < rmin[j])
	  rmin[j] = in_data[i*veclen+j];
	if (in_data[i*veclen+j] > rmax[j])
	  rmax[j] = in_data[i*veclen+j];
      }
#ifdef DEBUG_PRINT
      printf("  range scan comp %d min= %f max= %f \n",j, rmin[j], rmax[j]);
#endif
    }
}
