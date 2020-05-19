/*
 */

/* ----------------------------------------------------------------------
 * modCreatePolyList Module
 * ----------------------------------------------------------------------
 * Description:
 *   
 *   CreatePolyList creates the poly_connect_list for a polyline mesh. It
 *   searches the input id_array array looking for common ids.  If the id
 *   number changes while searching down the array, the start and end
 *   indices are stored and the number of polylines incremented.
 *
 * Authors: 
 *   Brian Selle, Leon Thrane, Advanced Visual Systems Inc.
 *   Documentation written by Ian Curington
 *
 * Revision: 9th February 2000 - Paul G. Lever, IAC
 *   Converted to IAC format.
 *
 * ----------------------------------------------------------------------
 * Note:
 *   The gen.h include file is generated by Express when the module is 
 *   compiled. It avoids including "user.h" or "express.h" directly, so 
 *   that the module may be moved across the processes by changing the V
 *   properties in the library.
 * ----------------------------------------------------------------------
 */

#include "xp_comm_proj/cr_polyl/gen.h"
#include "XP_OM_CALL.h"

int
CreatePolyList_CreatePolyListMods_CreatePolyList::update(OMevent_mask , int seq_num) {
  int status;         // Function return status 
  int id_array_size = id_array.ret_array_size(); 
   
  // Check for valid input array size

  if( int id_array_size = id_array.ret_array_size() == 0 )
  {
    return( XP_SUCCESS );
  }

  // Get a pointer the id array

  int *id_array_ptr = NULL;
  OMCALLR_NONNULLPTR( "Get the id array",     
		      id_array_ptr, (int *)id_array.
		      ret_array_ptr( OM_GET_ARRAY_RD ) );

  // Count the number of poly lines

  int poly_count = 1;
  int old_id = id_array_ptr[0];  
  int i;
  for( i = 1; i < id_array_size; i++ )
  {
    if( id_array_ptr[i] != old_id )
    {
      old_id = id_array_ptr[i];
      poly_count++;
    }
  }

  // Set the number of poly lines and size of poly_connect_list array
  num_polys = poly_count; 
  OMCALLR_SUCCESS( "Set the size of the poly_connect_list",
		   poly_connect_list.set_array_size( poly_count * 2 ) );   

  // Get a pointer the poly_connect_list array
  int *poly_connect_list_ptr = NULL;
  OMCALLR_NONNULLPTR( "Get the poly_connect_list array",     
		      poly_connect_list_ptr, (int *)poly_connect_list.
		      ret_array_ptr( OM_GET_ARRAY_RW ) );

  // Set the poly_connect_list values
  poly_count = 1;  
  old_id = id_array_ptr[0];  
  poly_connect_list_ptr[0] = 0;  
  for( i = 1; i < id_array_size; i++ )
  {
    if( id_array_ptr[i] != old_id )
    {
      old_id = id_array_ptr[i];
      poly_connect_list_ptr[poly_count*2-1] = i - 1;
      poly_connect_list_ptr[poly_count*2] = i;
      poly_count++;
    }
  }
  poly_connect_list_ptr[poly_count*2-1] = id_array_size - 1;

  // Clean up
  ARRfree( id_array_ptr );
  ARRfree(poly_connect_list_ptr );

  // Successful return
  return( XP_SUCCESS );
};

/* end of file */
