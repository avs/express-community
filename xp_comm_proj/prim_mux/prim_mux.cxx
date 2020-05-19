/*
 */

/* ----------------------------------------------------------------------
 * modPMUX Module
 * ----------------------------------------------------------------------
 * Description:
 *   
 *   PMUX takes an array of Express primitives and outputs the value and
 *   index of the element that changed.
 * 
 * Authors: 
 *   Brian Selle, Leon Thrane, Advanced Visual Systems Inc.
 *   Documentation written by Ian Curington
 *
 * Revision: 8th February 2000 - Paul G. Lever, IAC
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

#include "xp_comm_proj/prim_mux/gen.h"
#include "XP_OM_CALL.h"

int
PrimitiveMUX_modPMUX::update(OMevent_mask , int seq_num)
{
   OMobj_id  PMUX_id;           /* Express object id for this object */
   OMobj_id  InputObjId;        /* Express object id for input */
   OMobj_id  OutputObjId;       /* Express object id for output */
   OMobj_id  InputArrayId;      /* Express object id for input array element */
   int       LoopCounter;       /* Loop counter */
   int       NumberReferences;  /* Number of input array references */
   int       DataType;          /* Data type of input */
   int       status;            /* Function return status */
   
   /* Get Obj_id for PMUX */   
   PMUX_id = (OMobj_id)this->obj_id();

   /* Get the Express object id's of input and output parameters */
   OMCALLR_NONNULLOBJ( "Get input's object id", InputObjId,
		       OMfind_subobj( PMUX_id, OMstr_to_name("input"), 
				      OM_OBJ_RD ) );
   
   OMCALLR_NONNULLOBJ( "Get output's object id", OutputObjId,
		       OMfind_subobj( PMUX_id, OMstr_to_name("output"), 
				      OM_OBJ_RW ) );
   
   /* Get the number of group references in the input */
   OMCALLR_SUCCESS( "Get the number of input references",
		    OMget_num_refs( InputObjId, &NumberReferences, 0 ) );
   
   /* Loop through all the references */
   for( LoopCounter = 0; LoopCounter < NumberReferences; LoopCounter++ )
   {
      OMCALL_SUCCESS( "Get array reference", 
		      OMget_array_ref( InputObjId, LoopCounter, 
				       &InputArrayId, OM_OBJ_RW) ); 
      if( status != XP_SUCCESS )
      {
	 continue;
      }
			  
      /* Find out which has changed */
      if(OMchanged(InputArrayId, seq_num) == 1)
      {
	 OMCALLR_SUCCESS( "Get input array element data type",
			  OMget_data_type( InputArrayId, &DataType ) );
	 
	 OMCALLR_SUCCESS( "Set output data type",
			  OMset_data_type( OutputObjId, DataType ) );
	 
	 OMCALLR_SUCCESS( "Set output value",
			  OMset_obj_val( OutputObjId, InputArrayId, 0) );
	 
	 OMCALLR_SUCCESS( "Set selected item value",
			  OMset_name_int_val( PMUX_id, 
					      OMstr_to_name("selectedItem"),
					      LoopCounter ) ); 

         /* Changed item found, now exit loop */
         break;

      }  /* End if */

   }  /* End loop */
     
   return( XP_SUCCESS );
}

/* end of file */