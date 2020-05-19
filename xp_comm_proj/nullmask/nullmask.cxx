
#include "xp_comm_proj/nullmask/gen.h"


int
NullMask_NullMaskMods_NullMaskNodeData::update(OMevent_mask event_mask, int seq_num)
{
  // in (Node_Data read req notify)
  
  // in.nnodes (int)
  // in.nnode_data (int)
  // in.node_data[in_data_comp].veclen (int) 
  // in.node_data[in_data_comp].values (char [])
  
  // out_valid (OMXdouble read req notify)
  // out_null (OMXdouble read req notify)
  // in_null (OMXdouble read req notify)
  // user_null (OMXint read req notify)
  // invert (OMXint read req notify)
  // set_null (OMXint read req notify)
  
  // out (Node_Data write)
  
  // out.nnodes (int)
  // out.nnode_data (int)
  // out.node_data[out_data_comp].veclen (int) 
  // out.node_data[out_data_comp].values (char [])
  
  int comp, elem;
  
  char *in_ptr;
  int in_size, in_type;
  
  char *out_ptr;
  int out_size;
  
  float the_null;
  
  /* prepare output 	 */
  
  out.nnode_data = in.nnode_data;
  out.nnodes = in.nnodes;
  
  /* get access to input data and type	 */
  
  /* loop through each component	 */
  
  for( comp=0; comp<(int)in.nnode_data; comp++ ) {
    
    /* copy other attributes	 */
    
    out.node_data[comp].nvals = in.node_data[comp].nvals;
    out.node_data[comp].veclen = in.node_data[comp].veclen;
    
    /* get component values array	 */
    
    in_ptr = (char *)in.node_data[comp].values.
      ret_array_ptr( OM_GET_ARRAY_RD, &in_size, &in_type );
    
    if( in_type == OM_TYPE_UNSET ) {
      if( in_ptr )
	ARRfree( (char *)in_ptr );
      in_ptr = (char *)in.node_data[comp].values.
	ret_typed_array_ptr( OM_GET_ARRAY_RD, OM_TYPE_FLOAT, &in_size );
      in_type = OM_TYPE_FLOAT;
    }
    
    /* get null value	 */
    
    if( (int)user_null )
      the_null = (float)in_null;
    else
      the_null = (float)in.node_data[comp].null_value;
    
    /* set output null	 */
    
    out.node_data[comp].null_flag = (int)set_null;
      
    if( (int)user_null )
      out.node_data[comp].null_value = (float)out_null;
    else
      out.node_data[comp].null_value = (float)in.node_data[comp].null_value;
    
    /* get access to output array and set type	 */
    
    out_ptr = (char *)out.node_data[comp].values.
      ret_typed_array_ptr( OM_GET_ARRAY_WR, in_type, &out_size );
    
    for( elem=0; elem<in_size; elem++ ) {
    
      switch( in_type ) {
      case OM_TYPE_FLOAT:
	
	if( (float)((float *)in_ptr)[elem] == (float)the_null ) {
	  if( !(int)invert )
	    ((float *)out_ptr)[elem] = (float)out_null;
	  else
	    ((float *)out_ptr)[elem] = (float)out_valid;
	} else {
	  if( !(int)invert )
	    ((float *)out_ptr)[elem] = (float)out_valid;
	  else
	    ((float *)out_ptr)[elem] = (float)out_null;
	}
	
	break;
      case OM_TYPE_DOUBLE:
	
	if( ((double *)in_ptr)[elem] == the_null ) {
	  if( !(int)invert )
	    ((double *)out_ptr)[elem] = (double)out_null;
	  else
	    ((double *)out_ptr)[elem] = (double)out_valid;
	} else {
	  if( !(int)invert )
	    ((double *)out_ptr)[elem] = (double)out_valid;
	  else
	    ((double *)out_ptr)[elem] = (double)out_null;
	}
	
	break;
      case OM_TYPE_INT:
	
	if( ((int *)in_ptr)[elem] == the_null ) {
	  if( !(int)invert )
	    ((int *)out_ptr)[elem] = (int)out_null;
	  else
	    ((int *)out_ptr)[elem] = (int)out_valid;
	} else {
	  if( !(int)invert )
	    ((int *)out_ptr)[elem] = (int)out_valid;
	  else
	    ((int *)out_ptr)[elem] = (int)out_null;
	}
	
	break;
      case OM_TYPE_BYTE:
	
	if( ((char *)in_ptr)[elem] == the_null ) {
	  if( !(int)invert )
	    ((char *)out_ptr)[elem] = (char)out_null;
	  else
	    ((char *)out_ptr)[elem] = (char)out_valid;
	} else {
	  if( !(int)invert )
	    ((char *)out_ptr)[elem] = (char)out_valid;
	  else
	    ((char *)out_ptr)[elem] = (char)out_null;
	}

	break;
      }
    }
    
    if( in_ptr )
      ARRfree( (char *)in_ptr );
    
    if( out_ptr )
      ARRfree( (char *)out_ptr );
    
  }
  
  // return 1 for success
  
  return(1);
}
