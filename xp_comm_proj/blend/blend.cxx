
#include "xp_comm_proj/blend/gen.h"


int
BlendNodeData_BlendNodeDataMods_BlendNodeData::update(OMevent_mask event_mask, int seq_num)
{
  // in1 (Node_Data read req notify)
  // in1.nnodes (int)
  // in1.nnode_data (int)
  // in1.node_data[in1_data_comp].veclen (int) 
  // in1.node_data[in1_data_comp].values (char [])
  
  // in2 (Node_Data read req notify)
  // in2.nnodes (int)
  // in2.nnode_data (int)
  // in2.node_data[in2_data_comp].veclen (int) 
  // in2.node_data[in2_data_comp].values (char [])
  
  // ratio (OMXfloat read req notify)
  
  // out (Node_Data write)
  // out.nnodes (int)
  // out.nnode_data (int)
  // out.node_data[out_data_comp].veclen (int) 
  // out.node_data[out_data_comp].values (char [])
  
  int comp, elem, node, index;
  char *data1, *data2, *dataout;
  int data1_size, data1_type;
  int data2_size, data2_type;
  int dataout_size, dataout_type;
  float ratio1, ratio2;
  
  /* check inputs are compatible	 */
  
  if( (int)in1.nnode_data != (int)in2.nnode_data ) {
    printf( "Two input fields have different number of components.\n" );
    fflush(stdout);
    ERRverror( "", ERR_NO_HEADER | ERR_PRINT,
	       "BlendNodeData::input fields have different #comps\n" );
    return( 1 );
  };
  
  if( (int)in1.nnodes != (int)in2.nnodes ) {
    printf( "Two input fields have different number of nodes.\n" );
    fflush(stdout);
    ERRverror( "", ERR_NO_HEADER | ERR_PRINT,
	       "BlendNodeData::input fields have different #nodes\n" );
    return( 1 );
  };
  
  /* set output	 */
  
  out.nnode_data = in1.nnode_data;
  out.nnodes = in1.nnodes;
  
  /* local ratios	 */
  
  ratio1 = 1.0 - (float)ratio;
  ratio2 = (float)ratio;
  
  /* loop through components and interpolate the node data 	 */
  
  for( comp=0; comp<(int)in1.nnode_data; comp++ ) {
    
    data1 = (char *)in1.node_data[comp].values.
      ret_array_ptr( OM_GET_ARRAY_RD, &data1_size, &data1_type);
    
    /* check if unset, if so get as float	 */
    
    if( data1_type == OM_TYPE_UNSET ) {
      if( data1 )
	ARRfree( (char *)data1 );
      data1 = (char *)in1.node_data[comp].values.
	ret_typed_array_ptr( OM_GET_ARRAY_RD, OM_TYPE_FLOAT, &data1_size );
      data1_type = OM_TYPE_FLOAT;
    }
    
    data2 = (char *)in2.node_data[comp].values.
      ret_array_ptr( OM_GET_ARRAY_RD, &data2_size, &data2_type);

    /* check if unset, if so get as float	 */
    
    if( data2_type == OM_TYPE_UNSET ) {
      if( data2 )
	ARRfree( (char *)data2 );
      data2 = (char *)in1.node_data[comp].values.
	ret_typed_array_ptr( OM_GET_ARRAY_RD, OM_TYPE_FLOAT, &data2_size );
      data2_type = OM_TYPE_FLOAT;
    }
    
    /* check for compatible datatypes	 */
    
    if( data1_type != data2_type ) {
      printf( "Two input fields have different components.\n" );
      fflush(stdout);
      ERRverror( "", ERR_NO_HEADER | ERR_PRINT,
		 "BlendNodeData::input fields have different components\n" );
      return( 1 );
    }
    
    /* check veclen	 */
    
    if( (int)in1.node_data[comp].veclen != (int)in2.node_data[comp].veclen ) {
      printf( "Two input fields have different veclen.\n" );
      fflush(stdout);
      ERRverror( "", ERR_NO_HEADER | ERR_PRINT,
		 "BlendNodeData::input fields have different veclen\n" );
      return( 1 );
    }
    
    /* set veclen in output	 */
    
    out.node_data[comp].veclen = (int)in1.node_data[comp].veclen;
    
    /* set data type of component values array	 */
    
    OMset_data_type( out.node_data[comp].values, data1_type );
    
    /* get output ptr	 */
    
    dataout = (char *)out.node_data[comp].values.
      ret_array_ptr( OM_GET_ARRAY_WR, &dataout_size, &dataout_type );
    
    /* loop through array	 */
    
    index = 0;
    for( node=0; node<(int)in1.node_data[comp].nvals; node++ ) {
    
      /* loop through veclen	 */
      
      for( elem=0; elem<(int)in1.node_data[comp].veclen; elem++ ) {
	
	switch( data1_type ) {
	case OM_TYPE_FLOAT:
	  
	  ((float *)dataout)[index] = (float)
	    ((ratio1 * ((float *)data1)[index]) +
	    (ratio2 * ((float *)data2)[index]));
	  
	  break;
	case OM_TYPE_DOUBLE:
	  
	  ((double *)dataout)[index] = (double)
	    ((ratio1 * (float)((double *)data1)[index]) +
	    (ratio2 * (float)((double *)data2)[index]));
	  	  
	  break;
	case OM_TYPE_INT:
	  
	  ((int *)dataout)[index] = (int)
	    ((ratio1 * (float)((int *)data1)[index]) +
	    (ratio2 * (float)((int *)data2)[index]));

	  break;
	case OM_TYPE_SHORT:
	  
	  ((short int *)dataout)[index] = (short int)
	    ((ratio1 * (float)((short int *)data1)[index]) +
	    (ratio2 * (float)((short int *)data2)[index]));

	  break;
	case OM_TYPE_CHAR:
	case OM_TYPE_BYTE:
	  
	  ((char *)dataout)[index] = (char)
	    ((ratio1 * (float)((char *)data1)[index]) +
	    (ratio2 * (float)((char *)data2)[index]));
	  
	  break;
	  
	}
	index++;
      }
    }
    
    /* free ptrs	 */
    
    if( data1 )
      ARRfree( (char *)data1 );
    
    if( data2 )
      ARRfree( (char *)data2 );
    
    if( dataout )
      ARRfree( (char *)dataout );
    
  }
  
  // return 1 for success
  
  return( 1 );
}
