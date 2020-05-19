/*
 * routine: ConvertConnectList
 * purpose: Convert from an AVS5 style poly connect list to an
 *   AVS/Express style list. This includes breaking up the AVS5
 *   list into two lists, and changing the indexing from 1 based
 *   to 0 based for the connect values.
 */
#include "gen.h"

int ConvertConnectList(OMobj_id cvt_connect_list_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  input_size = 0;
   int *input = NULL; 
   int  poly_nodes_size = 0;
   int *poly_nodes = NULL; 
   int  connect_list_size = 0;
   int *connect_list = NULL; 

   int polysize;
   int *iptr;
   int *pptr;
   int *cptr;
   int i, j;

   /***********************/
   /*  Get input values   */
   /***********************/
   input = (int *)OMret_name_array_ptr(cvt_connect_list_id, 
                                       OMstr_to_name("input"), 
                                       OM_GET_ARRAY_RD,
                                       &input_size, NULL);

   /* First count the number of polys in the input list so we know how
    *  to dimension our arrays
    */
   iptr = input;
   for( i = 0; i < input_size; i++ )
     {
       polysize = *iptr++;
       if( polysize > 0 )
         {
           poly_nodes_size += 1;
           connect_list_size += polysize;
           for( j = 0; j < polysize; j++, i++, iptr++ );
         }
     }

   /* Now we can allocate the output arrays, and fill them in
    */
   poly_nodes = (int *)ARRalloc(NULL, DTYPE_INT, poly_nodes_size, NULL);
   connect_list = (int *)ARRalloc(NULL, DTYPE_INT, connect_list_size, NULL);

   iptr = input;
   pptr = poly_nodes;
   cptr = connect_list;
   for( i = 0; i < input_size; i++ )
     {
       polysize = *iptr++;
       if( polysize > 0 )
         {
           *pptr++ = polysize;
           for( j = 0; j < polysize; j++, i++ )
             {
               *cptr++ = *iptr++ - 1;
             }
         }
     }

   /* Output the results
    */
   OMset_name_array(cvt_connect_list_id, 
                    OMstr_to_name("poly_nodes"), 
                    DTYPE_INT, (void *)poly_nodes, 
                    poly_nodes_size, OM_SET_ARRAY_FREE);

   OMset_name_array(cvt_connect_list_id, 
                    OMstr_to_name("connect_list"), 
                    DTYPE_INT, (void *)connect_list, 
                    connect_list_size, OM_SET_ARRAY_FREE);

   /*************************/
   /*  Free input variables */
   /*************************/
   if (input != NULL) 
      ARRfree((char *)input);

   return(1);
}
