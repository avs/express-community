
/* ----------------------------------------------------------------------
 * NullBrick IAC AVS/Express Project
 * ----------------------------------------------------------------------
 * Description:
 *
 *   Inputs a uniform volume (Node_Data_Struct) and a set of parameters to
 *   define a sub-volume within, and a replacement data value. All node
 *   data within the sub-volume is replaced with the given value, in all
 *   components.
 *   This allows sub-volumes to be cut away - similar to the
 *   excavate_brick module - but doesn't convert the field to an
 *   unstructured mesh, thus allowing the volume to be volume rendered.
 *
 * Author:
 *   Paul G. Lever
 *   International AVS Centre
 *   Manchester Visualization Centre
 *   Manchester Computing
 *   University of Manchester
 *   Oxford Road
 *   Manchester
 *   United Kingdom
 *   M13 9PL
 *   Tel: +44 161 275 6095
 *   Fax: +44 161 275 6800
 *
 * Version 1.0 - 25th January 1999
 *
 * Future Revisions:
 *   - Process 2D fields
 *   - Allow definition of multiple bricks to be excavated
 *
 * ----------------------------------------------------------------------
 */

#include "xp_comm_proj/nullbrik/gen.h"

int
NullBrick_NullBrickNodeData::update(OMevent_mask event_mask, int seq_num)
{
  /* class members	 */

  // in (Node_Data_Struct read req notify)
  // in.nnodes (int)
  // in.nnode_data (int)
  // in.node_data[in_data_comp].veclen (int) 
  // in.node_data[in_data_comp].values (char [])
  
  // out (Node_Data_Struct write)
  // out.nnodes (int)
  // out.nnode_data (int)
  // out.node_data[out_data_comp].veclen (int) 
  // out.node_data[out_data_comp].values (char [])

  // xmin (OMXint read req notify)
  // xmax (OMXint read req notify)
  // ymin (OMXint read req notify)
  // ymax (OMXint read req notify)
  // zmin (OMXint read req notify)
  // zmax (OMXint read req notify)
  // nval (OMXdouble read req notify)
  // setnull (OMXint read req notify)

  /* locals	 */

  int comp;
  
  int insize, intype;
  int outsize, outtype;
  int dims_size, dims_type;
  
  char *indata;
  char *outdata;
  int *dims;

  int i, j, k, index;
  int joff, koff;
  
  /* get input dims */
  
  dims = (int *)in.dims.
    ret_array_ptr( OM_GET_ARRAY_RD, &dims_size, &dims_type );
  
  /* loop through each component	 */
  
  for( comp=0; comp<(int)in.nnode_data; comp++ ) {
    
    /* set veclen in output      */
    
    out.node_data[comp].veclen = in.node_data[comp].veclen;
    
    /* get input data	 */
    
    indata = (char *)in.node_data[comp].values.
      ret_array_ptr( OM_GET_ARRAY_RD, &insize, &intype );
    
    /* check if unset, if so get as float        */
    
    if( intype == OM_TYPE_UNSET ) {
      if( indata )
	ARRfree( (char *)indata );
      indata = (char *)in.node_data[comp].values.
	ret_typed_array_ptr( OM_GET_ARRAY_RD, OM_TYPE_FLOAT, &insize );
      intype = OM_TYPE_FLOAT;
    }
    
    /* set data type of component values array   */
    
    OMset_data_type( out.node_data[comp].values, intype );
    
    /* set null flag and null value	 */

    out.node_data[comp].null_flag = (int)setnull;
    out.node_data[comp].null_value = (double)nval;

    /* get output ptr    */
    
    outdata = (char *)out.node_data[comp].values.
      ret_array_ptr( OM_GET_ARRAY_WR, &outsize, &outtype );
    
    /* intially, copy input to output	 */
    
    switch( intype ) {
    case OM_TYPE_INT:
      
      memcpy( outdata, indata, insize * sizeof(int) );
      break;
      
    case OM_TYPE_FLOAT:
      
      memcpy( outdata, indata, insize * sizeof(float) );
      break;
      
    case OM_TYPE_DOUBLE:
      
      memcpy( outdata, indata, insize * sizeof(double) );
      break;
      
    case OM_TYPE_SHORT:
      
      memcpy( outdata, indata, insize * sizeof(short int) );
      break;
      
    case OM_TYPE_CHAR:
    case OM_TYPE_BYTE:
      
      memcpy( outdata, indata, insize * sizeof(char) );
      break;
    }
    
    /* remove data from output        */
    
    index = ((int)zmin * dims[0] * dims[1])
      + ((int)ymin * dims[0])
      + (int)xmin;
    joff = dims[0] - ((int)xmax-(int)xmin+1);
    koff = (dims[0] * dims[1]) - (((int)ymax-(int)ymin+1)*dims[0]);
    
    switch( intype ) {
    case OM_TYPE_INT:
      
      for( k=(int)zmin; k<=(int)zmax; k++ ) {
	for( j=(int)ymin; j<=(int)ymax; j++ ) {
	  for( i=(int)xmin; i<=(int)xmax; i++ ) {
	    ((int *)(outdata))[index++] = (int)nval;
	  }
	  index += joff;
	}
	index += koff;
      }
      
      break;
      
    case OM_TYPE_FLOAT:
      
      for( k=(int)zmin; k<=(int)zmax; k++ ) {
	for( j=(int)ymin; j<=(int)ymax; j++ ) {
	  for( i=(int)xmin; i<=(int)xmax; i++ ) {
	    ((float *)(outdata))[index++] = (float)nval;
	  }
	  index += joff;
	}
	index += koff;
      }
      
      break;
      
    case OM_TYPE_DOUBLE:
      
      for( k=(int)zmin; k<=(int)zmax; k++ ) {
	for( j=(int)ymin; j<=(int)ymax; j++ ) {
	  for( i=(int)xmin; i<=(int)xmax; i++ ) {
	    ((double *)(outdata))[index++] = (double)nval;
	  }
	  index += joff;
	}
	index += koff;
      }
      
      break;
      
    case OM_TYPE_SHORT:
      
      for( k=(int)zmin; k<=(int)zmax; k++ ) {
	for( j=(int)ymin; j<=(int)ymax; j++ ) {
	  for( i=(int)xmin; i<=(int)xmax; i++ ) {
	    ((short int *)(outdata))[index++] = (short int)nval;
	  }
	  index += joff;
	}
	index += koff;
      }
      
      break;
      
    case OM_TYPE_CHAR:
    case OM_TYPE_BYTE:
      
      for( k=(int)zmin; k<=(int)zmax; k++ ) {
	for( j=(int)ymin; j<=(int)ymax; j++ ) {
	  for( i=(int)xmin; i<=(int)xmax; i++ ) {
	    ((char *)(outdata))[index++] = (char)nval;
	  }
	  index += joff;
	}
	index += koff;
      }
      
      break;
      
    }

    
    if( indata )
      ARRfree( (char *)indata );
    
    if( outdata )
      ARRfree( (char *)outdata );
  }
  
  /* free dims array	 */
  
  if( dims )
    ARRfree( (char *)dims );
  
  /* end	 */

  return(1);
}
