#include <math.h>
#include "xp_comm_proj/gnrlhist/gen.h"
#define ERROR(MESS) { ERRerror("GenerateStats",1,ERR_ORIG, MESS); return 0; }


int
generatestats(OMobj_id GenerateStats_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int   binfreq_size = 0;
   int  *binfreq = NULL; 
   int   binval_size = 0;
   char *binval = NULL;
   int   binval_type;

   /* pointers for every type of binval */
   char   *binval_char;
   char   *binval_byte;
   short  *binval_short;
   int    *binval_int;
   float  *binval_float;
   double *binval_double;

   double  mean= 0;
   double  std_deviation= 0;

   int index;
   int n_values;
   double mean_sum;
   double std_deviation_sum= 0;

   /***********************/
   /*  Get input values   */
   /***********************/
   binfreq = (int *)OMret_name_array_ptr(GenerateStats_id, OMstr_to_name("binfreq"), OM_GET_ARRAY_RD,
			&binfreq_size, NULL);

   binval = (char *)OMret_name_array_ptr(GenerateStats_id, OMstr_to_name("binval"), OM_GET_ARRAY_RD,
			&binval_size, &binval_type);

   /***********************/
   /* Function's Body     */
   /***********************/

#define GENERATEMEAN(D_TYPE,BINVAL)					\
   BINVAL= ((D_TYPE *)binval);						\
   for (index= 0; index < binval_size; index++){			\
      n_values+= binfreq[index];					\
      mean_sum+= (double)(BINVAL[index]) * (double)(binfreq[index]); }	\
   mean= mean_sum / (double)n_values ;					\
   for (index= 0; index < binval_size; index++){			\
      std_deviation_sum+= pow(mean - (double)(BINVAL[index]),2) * (double)(binfreq[index]); }	\
   std_deviation= sqrt(std_deviation_sum / (double)n_values);


    switch (binval_type) {

            case DTYPE_CHAR: 
	      GENERATEMEAN( char, binval_char)
	      break;

       	    case DTYPE_BYTE:
	      GENERATEMEAN( char, binval_byte)
	      break;
          
	    case DTYPE_SHORT:
	      GENERATEMEAN( short, binval_short)    
	      break;

	    case DTYPE_INT:
	      GENERATEMEAN( int, binval_int)
	      break;

	    case DTYPE_FLOAT:
	      GENERATEMEAN( float, binval_float)
	      break; 

	    case DTYPE_DOUBLE:
	      GENERATEMEAN( double, binval_double)
	      break;

	    default:
	      ERROR("Node data is of an unknown data type.");
        }

   

   /***********************/
   /*  Set output values  */
   /***********************/
   /* Set mean's value */
   OMset_name_real_val(GenerateStats_id, OMstr_to_name("mean"), mean);
   /* Set variance's value */
   OMset_name_real_val(GenerateStats_id, OMstr_to_name("std_deviation"), std_deviation);

   /*************************/
   /*  Free input variables */
   /*************************/
   if (binfreq != NULL) 
      ARRfree((char *)binfreq);

   return(1);
}
