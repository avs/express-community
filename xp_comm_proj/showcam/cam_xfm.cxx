//
// part of showcam module
//

#include "xp_comm_proj/showcam/gen.h"
#include <avs/mat.h>

int
Showcam_ShowcamMods_prep_cam_xform::update(OMevent_mask event_mask, int seq_num)
{
   // xform_in (GDxform read notify)
   // xform_out (DefaultXform write)

   float *mat_in, *mat_out;

   /***********************/
   /* Function's Body     */
   /***********************/

   mat_in = (float *)xform_in.mat.ret_array_ptr( OM_GET_ARRAY_RD );
   if( !mat_in ) {
      return(0);
   }

   mat_out = (float *)xform_out.mat.ret_array_ptr( OM_GET_ARRAY_WR );
   if( !mat_out ) {
      ARRfree( (char *)mat_in );
      return(0);
   }

   // invert the incoming camera xform into the outgoing xform
   // the MAT functions are prototypes in <express>/include/avs/mat.h and
   // are found in the libtool.a library.
   MATmat4_inverse( (Matr4 *)mat_out, (Matr4 *)mat_in );

   ARRfree( (char *)mat_in );
   ARRfree( (char *)mat_out );

   // return 1 for success
   return(1);
}
// end of file

