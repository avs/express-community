
#include "gen.h"

/*#define DEBUG /**/

int ExtractNodeData(OMobj_id Extract_Node_Data_id, OMevent_mask event_mask, int seq_num)
{
   OMobj_id input_data_id;
   int input_data_data_type;
   char *input_data_data;
   int input_data_veclen;
   int input_data_ncomp;
   int input_data_ndata;
   int node_index_size;
   float *output_ptr;
   int output_size;
   int *node_index;
   float *output;
   int offset;
   int i, j;

   /*Added by Ma YingLiang 6th August,2001*/
   int data_component;

   /* Get node_index's and offset's value 
    */ 
   node_index = (int *)OMret_name_array_ptr(Extract_Node_Data_id, 
                                            OMstr_to_name("node_index"), 
                                            OM_GET_ARRAY_RD, &node_index_size, NULL);

   if( node_index == NULL )
     {
       fprintf( stderr, "ExtractNodeData: Unable to read node_index array\n" );
       return( 0 );
     }

   if (OMget_name_int_val(Extract_Node_Data_id, OMstr_to_name("offset"), &offset) != 1) 
      offset = 0;

   /* Get field id 
    */
   input_data_id = OMfind_subobj(Extract_Node_Data_id, OMstr_to_name("input_data"), OM_OBJ_RD);

   /*Get data component*/
   if (OMget_name_int_val(Extract_Node_Data_id, OMstr_to_name("data_component"), &data_component) != 1) 
      data_component = 0;

   /* Get number of node data components 
    */
   FLDget_node_data_ncomp (input_data_id, &input_data_ncomp);

   /* Allocate output array
    */
   output_size = input_data_ncomp * node_index_size;
   output = (float *)ARRalloc(NULL, DTYPE_FLOAT, output_size, NULL);

   /* For each node data component get veclen, type and data arry itself 
    */
   for( i = 0; i < input_data_ncomp; i++ ) 
     {
       /* Get veclen
        */
       FLDget_node_data_veclen(input_data_id, i, &input_data_veclen);

       if( input_data_veclen != 1 )
         {
           fprintf( stderr, "Extract_Node_Data: veclen != 1 not supported\n" );
           return( 0 );
         }

       /* Get node data
        */
       FLDget_node_data(input_data_id, ((i + offset) % input_data_ncomp), &input_data_data_type, 
                        &input_data_data, &input_data_ndata, OM_GET_ARRAY_RD);

#ifdef DEBUG
	   fprintf(stderr,"input_data_ndata: %i\n",input_data_ndata);
#endif

       if (input_data_data)
         {
           for( j = 0; j < node_index_size; j++ )
             {
			   if (node_index[j] >= input_data_ndata)
			   {
			   	  fprintf(stderr,"Extract_Node_Data: invalid node_index[%i]: %i\n", j, node_index[j]);
				  continue;
			   }
               output_ptr = output + (j * input_data_ncomp) + i;
               switch( input_data_data_type )
                 {
                 case OM_TYPE_BYTE:
                 case OM_TYPE_CHAR:
                   *output_ptr = (float)*(((char *)input_data_data) + node_index[j]);
                   break;
                 case OM_TYPE_SHORT:
                   *output_ptr = (float)*(((short *)input_data_data) + node_index[j]);
                   break;
                 case OM_TYPE_INT:
                   *output_ptr = (float)*(((int *)input_data_data) + node_index[j]);
                   break;
                 case OM_TYPE_FLOAT:
                   *output_ptr = (float)*(((float *)input_data_data) + node_index[j]);
                   break;
                 case OM_TYPE_DOUBLE:
                   *output_ptr = (float)*(((double *)input_data_data) + node_index[j]);
                   break;
                 default:
                   fprintf( stderr, "Extract_Node_Data: Unrecognized data format %d\n", input_data_data_type );
                   return( 0 );
                 }
             }

           ARRfree((char *)input_data_data);
         }
       else
         {
           fprintf( stderr, "Extract_Node_Data: Error reading node data for component %d\n", 
                    ((i + offset) % input_data_ncomp) );

           return( 0 );
         }
     }

   /* fill in array output
    */
   OMset_name_array(Extract_Node_Data_id, OMstr_to_name("output"), 
                    DTYPE_FLOAT, (void *)output, output_size, OM_SET_ARRAY_FREE);

   /* Free up input array
    */
   if (node_index != NULL) 
      ARRfree((char *)node_index);

   return(1);
}
