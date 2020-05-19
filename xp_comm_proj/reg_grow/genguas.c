#include <math.h>
#include "xp_comm_proj/reg_grow/gen.h"
#define N_ITERATION 300

#define ERR_RETURN(MESS) {\
  ERRerror("GenerateGuassianProbs.update",1,ERR_ORIG, MESS);\
  return(0);\
}


int
generate_gausian(OMobj_id GenerateGuassianProbs_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   double  mean_min;
   double  mean_max; 
   double  stddev;

   double x, mu, sigma, pi;

   double scale;

   int index;

   int  max;
   int  probs_size = 0;
   float *probs = NULL; 

   double total = 0;
   double step;
   double max_value = 0;

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get mean_min's value */
   if (OMget_name_real_val(GenerateGuassianProbs_id, OMstr_to_name("mean_min"), &mean_min) != 1)
      mean_min = 0.0;

   /* Get mean_max's value */
   if (OMget_name_real_val(GenerateGuassianProbs_id, OMstr_to_name("mean_max"), &mean_max) != 1)
      mean_max = mean_min;

   /* Ensure mean_max is greater than mean_min */
   if (mean_min > mean_max) {
      double temp = mean_min;
      mean_min = mean_max;
      mean_max = temp;
   }
   

   /* Get stddev's value */
   if (OMget_name_real_val(GenerateGuassianProbs_id, OMstr_to_name("stddev"), &stddev) != 1)
      ERR_RETURN("Unable to access Standard Deviation");

   if (stddev <= 0)
      ERR_RETURN("Standard Deviation must be greater than 0");

   /* Get max's value */ 
   if (OMget_name_int_val(GenerateGuassianProbs_id, OMstr_to_name("max"), &max) != 1) 
      ERR_RETURN("Unable to access Maximum Value");

   if (max <= 0)
      ERR_RETURN("Maximum Value must be greater than 0");


   /***********************/
   /* Function's Body     */
   /***********************/
   /*ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: generate_gausian generated from method: GenerateGuassianProbs.update\n");*/


   /***********************/
   /*  Set output values  */
   /***********************/
   /*
    *   set number of elements in array: 
    *   probs_size = ...
    *   allocate array probs:
    */
   probs_size = max + 1;

   probs = (float *)ARRalloc(NULL, DTYPE_FLOAT, probs_size, NULL);


   /*
    *  fill in array probs
    */
   /*
   For mean=mu and variance=sigma^2
    
                                   -1    ( x - mu )^2
                                   --- * (--------)
                   1                2    ( sigma  )
    f(x) = ------------------  * e
           sigma * sqrt(2*pi)
   */
       

   sigma = stddev;
   pi = acos(-1);


   OMpush_status_range(0, 75);

   /* Max equals min. Generate probablities for single values only. */
   if (mean_max == mean_min) {
      for (index=0; index<=max; index++) {
         x = (double)index;
         mu = mean_min;
         probs[index] = (float)(  exp(-0.5*pow(((x-mu)/sigma),2)) );

         if (probs[index] > max_value)
            max_value = probs[index];

         if ((index % 50) == 0)
            OMstatus_check(index * 100 / max, "Generating Probablities", NULL);
      }
   }
   else  /* Generate average probablity for reach of value */
   {
      step = (mean_max - mean_min) / N_ITERATION;

      for (index=0; index<=max; index++) {
         x = (double)index;
         total = 0;

         for (mu=mean_min; mu<=mean_max; mu+=step){
            total += (float)(  exp(-0.5*pow(((x-mu)/sigma),2)) );
         }

         probs[index] = total / (N_ITERATION+1);

         if (probs[index] > max_value)
            max_value = probs[index];

         if ((index % 50) == 0)
            OMstatus_check(index * 100 / max, "Generating Probablities", NULL);
      }
   }

   OMpop_status_range();

   OMpush_status_range(75, 100);

   /* rescale values so max_value = 1 */
   for (index=0; index <= max; index++){
      probs[index]= probs[index] / max_value;

      if ((index % 50) == 0)
         OMstatus_check(index * 100 / max, "Normalizing Probablities", NULL);
   }

   OMpop_status_range();


   OMset_name_array(GenerateGuassianProbs_id, OMstr_to_name("probs"), DTYPE_FLOAT, (void *)probs, probs_size, OM_SET_ARRAY_FREE);

   return(1);
}

