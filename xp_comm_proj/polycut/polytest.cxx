
#include "gen.h"
#include <iostream>
using namespace std;

int
CutPolylines_CutPolylinesTest::update(OMevent_mask event_mask, int seq_num)
{
   // value (OMXint read req notify)
   // in_poly (OMXobj_array read req notify)
   // in_field (OMXobj_array read req notify)
   // out (OMXobj_array write)

   /***********************/
   /* Function's Body     */
   /***********************/

   
   OMobj_id in_poly_id, out_id;
   in_poly_id = (OMobj_id) in_poly;


   if (value==0)
     {
       out.set_obj_ref((OMobj_id) in_field, 0);
     }
   else if (value!=0)
     {
       out.set_obj_ref((OMobj_id) in_poly, 0);     
     }


   // return 1 for success
   return(1);
}
