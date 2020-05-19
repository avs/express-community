
#include "xp_comm_proj/catmull/gen.h"
#include "spline.h"

int update(OMobj_id catmull_rom_spline_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  points_size = 0;
   float *points = NULL; 
   int  out_size = 0;
   float *out = NULL; 
   int  steps;
   int nspace;
   int real_out_size;
   int line_type;

   /***********************/
   /*  Get input values   */
   /***********************/
   points = (float *)OMret_name_array_ptr(catmull_rom_spline_id, OMstr_to_name("points"), OM_GET_ARRAY_RD,
			&points_size, NULL);
   if(points == NULL) return 0;

   /* Get steps's value */ 
   if (OMget_name_int_val(catmull_rom_spline_id, OMstr_to_name("steps"), &steps) != 1) 
      steps = 10;

   /* Get nspace's value */ 
   if (OMget_name_int_val(catmull_rom_spline_id, OMstr_to_name("nspace"), &nspace) != 1) 
      nspace = 2;

   /* Get type of spline: open or closed */ 
   if (OMget_name_int_val(catmull_rom_spline_id, OMstr_to_name("line_type"), &line_type) != 1) 
      line_type = 0;

   /*
    *   set number of elements in output array
    */
   out_size = nspace*((points_size-line_type)/nspace*steps+1);

   /*
    *   Compute the spline points
    */
   out = (float *)ARRalloc(NULL, DTYPE_FLOAT, out_size, NULL);
   real_out_size = catmull_rom_spline(points, nspace, points_size/nspace, steps, line_type, out);

   /*
    *  fill array out with spline points
    */
   if(real_out_size)
   {
		OMset_name_array(catmull_rom_spline_id, OMstr_to_name("out"), DTYPE_FLOAT, (void *)out, 
			             real_out_size*nspace, OM_SET_ARRAY_FREE);
   }

   /*************************/
   /*  Free input variables */
   /*************************/
   ARRfree((char *)points);

   return real_out_size ? 1 : 0;
}
