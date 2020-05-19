#include <math.h>
#include "xp_comm_proj/gnrlhist/gen.h"
#define ERROR(MESS) { ERRerror("GeneralHistogram",1,ERR_ORIG, MESS); return 0; }

/* returns the integral value closest to the real */
double closest(double real_val){

   if ( fabs(real_val - ceil(real_val)) > fabs(real_val - floor(real_val)) ) 
         return floor(real_val);
    else return ceil(real_val);

}


/* takes a value in an untyped location,               */
/* the value's AVS datatype and returns it as a double */
double readvalue(int d_type, char *input)
{
   double result;

   switch (d_type){
           case DTYPE_CHAR: 
              result= (double)*(char *)(input);
              break;

       	    case DTYPE_BYTE:
              result= (double)*(char *)(input);
              break;
          
	    case DTYPE_SHORT:
              result= (double)*(short *)(input);
              break;

	    case DTYPE_INT:
              result= (double)*(int *)(input);
              break;

	    case DTYPE_FLOAT:
              result= (double)*(float *)(input);
              break; 

	    case DTYPE_DOUBLE:
              result= (double)*(double *)(input);
              break;

	    default:
	      ERROR("Node data is of an unknown data type.");
    }
  return result;
}


int
generalhistogram(OMobj_id GeneralHistogram_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
       OMobj_id  in_id;
       int       in_ncomp, in_comp_count, in_veclen;
       int       in_data_type, in_ndata;
       char     *in_data;
       char     *in_min;
       char     *in_max;
       double    in_max_real, in_min_real;
    
       int       nbins;
       int       keep_range;
    
       int       igmin_data_type, igmax_data_type;
       int       ignore_min_int, ignore_max_int;
       double    ignore_min_real, ignore_max_real;
       OMobj_id  igmax_id, igmin_id;
    
       int       binfreq_size = 0;
       int      *binfreq = NULL; 
       int       binval_size = 0;
       char     *binval = NULL; 
    
       double    max, min;
    
       double    bin_size;
       int       bin_no;
       double    range;
    
       int       index;

    /* binval pointers for every type */
       char     *binval_char, *in_data_char;
       short    *binval_short, *in_data_short;
       int      *binval_int, *in_data_int;
       float    *binval_float, *in_data_float;
       double   *binval_double, *in_data_double;
   


    /* Allocate memory for min & max values */
    /* allocate enough space so that any type can be accomodated */
       if (  ( (in_min= (char *)malloc(sizeof(double)*2)) == NULL) ||
	     ( (in_max= (char *)malloc(sizeof(double)*2)) == NULL) ) ERROR("Unable to allocate memory for parameters\n"); 




   /***********************/
   /*  Get input values   */
   /***********************/

   /**********************/
   /* Get in's node data */
   /**********************/

      /* Get field id */
      in_id = OMfind_subobj(GeneralHistogram_id, OMstr_to_name("in"), OM_OBJ_RD);

      /* check "in" is an actual object, otherwise exit */
      if (OMis_null_obj(in_id)) ERROR("No input field.\n");

      /* Get veclen */
      FLDget_node_data_veclen (in_id, 0, &in_veclen);

      /* Get data array and data_type which is one of the following: 
         DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
         DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE */

      if (FLDget_node_data (in_id, 0, &in_data_type, &in_data,
                        &in_ndata, OM_GET_ARRAY_RD) != 1) ERROR("Unable to get input field data\n");;

      /* Get Minimum and Maximum values */
         FLDget_node_data_minmax(in_id, 0, in_min, in_max);
       /* cast them from generic into double */
         in_max_real= readvalue(in_data_type,in_max);
         in_min_real= readvalue(in_data_type,in_min);


   /*******************************************************/
   /* Get parameter values.  Generate ones if not present */  
   /*******************************************************/    

      /*****************/
      /*  keep_range   */ 
      /*****************/

        if (OMget_name_int_val(GeneralHistogram_id, OMstr_to_name("keep_range"), &keep_range) != 1) 
          keep_range=TRUE;


      /*****************************/
      /*  ignore min & ignore max  */ 
      /*****************************/

      /* use double as universal data type as all other are a theoretical sub-set of double */

        /* get ids of ignore_min   & ignore_max */

          igmin_id = OMfind_subobj(GeneralHistogram_id, OMstr_to_name("ignore_min"), OM_OBJ_RD);
          igmax_id = OMfind_subobj(GeneralHistogram_id, OMstr_to_name("ignore_max" ), OM_OBJ_RD);


        /* find datatypes and check they are compatible */

          OMget_data_type(igmin_id, &igmin_data_type);
          OMget_data_type(igmax_id, &igmax_data_type);
          if (igmax_data_type != igmin_data_type) ERROR("Incompatable data type in parameters \n");


        /*get values according to data type */

  	  switch (igmin_data_type) {
	  
	     case DTYPE_CHAR:
	     case DTYPE_BYTE:
	     case DTYPE_SHORT:
	     case DTYPE_INT:
		 	OMget_int_val(igmax_id, &ignore_max_int);
			OMget_int_val(igmin_id, &ignore_min_int);
			ignore_max_real= (double)ignore_max_int;
			ignore_min_real= (double)ignore_min_int;
			break;

	     case DTYPE_FLOAT:
	     case DTYPE_DOUBLE:
			OMget_real_val(igmax_id, &ignore_max_real);
			OMget_real_val(igmin_id, &ignore_min_real);
			break;

	     default:
		      /* values unset.  set to min & max values */
		      ignore_max_real= in_max_real;
		      ignore_min_real= in_min_real;
		      break;
	  }



      /***********/
      /*  nbins  */
      /***********/

      /*  Generate suitable one if one is not provided or one provided is not suitable */ 

       /* generate value for nbins */

       switch (in_data_type) {
	    case DTYPE_CHAR:
	    case DTYPE_BYTE:
	    case DTYPE_SHORT:
	    case DTYPE_INT:
		    if (keep_range) nbins= (int)(in_max_real) - (int)(in_min_real)+ 1;
		    else nbins= (int)(ignore_max_real) - (int)(ignore_min_real)+ 1;
                    if (nbins < 2) ERROR("Illegal bounds\n");
                    /* printf("number bins: %d\n",nbins); */
		    break;

	    case DTYPE_FLOAT:
	    case DTYPE_DOUBLE:
		    if ((OMget_name_int_val(GeneralHistogram_id, OMstr_to_name("nbins"), &nbins) != 1) || (nbins < 2)) 
		       nbins=  256; /*arbitary value */
		    break;

	    default:
		    ERROR("Node data is of an unknown data type.");
       }
     

 
 

   /***********************/
   /*  Set output values  */
   /***********************/
 
    /* define sizes of output arrays */

        binfreq_size= nbins;
        binval_size= nbins;


    /* allocate memory for  output arrays */

       if (( (binfreq = (int *)ARRalloc(NULL, DTYPE_INT, binfreq_size, NULL)) == NULL ) || 
           ( (binval = (char *)ARRalloc(NULL, in_data_type, binfreq_size, NULL)) == NULL ) )
          ERROR(" Can not allocate memory for output data. \n");


    /* initialise binfreq */

       for (index= 0; index < nbins; index++) binfreq[index]= 0;


    /* initialise variables for histogram generation */

       if (keep_range)  { max= in_max_real; min= in_min_real; }              	
                  else  { max= ignore_max_real; min= ignore_min_real;}  

       range= max - min; 	
       if (range == 0.0) ERROR("Range = 0: No histogram\n");	

       bin_size= (range)  / (double)(nbins);             	

 

   /****************************************************/ 
   /* set up bin_values for each bin using generic code*/
   /****************************************************/ 

#define GENERATE_REAL(D_TYPE,BINVAL,INDATA)                         				\
                                                                                             	\
      BINVAL= ((D_TYPE *)binval);                                        			\
      for (index= 0; index < nbins; index++)                          				\
        BINVAL[index]=  (D_TYPE)(min + ((double)index * range) + (range * 0.5));   		\
			                    							\
      INDATA= ((D_TYPE *)in_data);  								\
      for(index= 0; index < in_ndata; index++) {						\
        if (( (double)INDATA[index] < ignore_max_real ) && 					\
            ( (double)INDATA[index] > ignore_min_real ))					\
        {											\
            bin_no= (int)(floor( ((double)(INDATA[index]) - min) / bin_size)) ;     		\
  												\
	    if (bin_no >= nbins) bin_no= nbins-1;	/*max case*/				\
            if (bin_no < 0) ERROR("Illegal values generated\n");				\
            BINVAL[index]= ((BINVAL[index]*binfreq[bin_no]) + INDATA[index]) / (binfreq[bin_no]+1) ;	\
             binfreq[bin_no]++; 								\
        }											\
      }												
         

#define GENERATE_INTEGER(D_TYPE,BINVAL,INDATA)							\
      BINVAL= ((D_TYPE *)binval);                                        			\
      for (index= 0; index < nbins; index++)                          				\
        BINVAL[index]= (D_TYPE)index+(D_TYPE)min;						\
												\
      INDATA= ((D_TYPE *)in_data);  								\
      for(index= 0; index < in_ndata; index++) {						\
	if (( (double)INDATA[index] < ignore_max_real ) && 					\
            ( (double)INDATA[index] > ignore_min_real ))					\
         {											\
	     bin_no= (int)(INDATA[index]) - (int)min;						\
	     if ((bin_no < 0) || (bin_no >= nbins)) ERROR("Illegal values generated\n"); 	\
	     binfreq[bin_no]++;									\
         }											\
      }

   

   /****     end of generic code        *****/


        switch (in_data_type) {

            case DTYPE_CHAR: 
	      GENERATE_INTEGER( char, binval_char, in_data_char)
	      break;

       	    case DTYPE_BYTE:
	      GENERATE_INTEGER( char, binval_char, in_data_char)
	      break;
          
	    case DTYPE_SHORT:
	      GENERATE_INTEGER( short, binval_short, in_data_short)    
	      break;

	    case DTYPE_INT:
	      GENERATE_INTEGER( int, binval_int, in_data_int)
	      break;

	    case DTYPE_FLOAT:
	      GENERATE_REAL( float, binval_float, in_data_float)
	      break; 

	    case DTYPE_DOUBLE:
	      GENERATE_REAL( double, binval_double, in_data_double)
	      break;

	    default:
	      ERROR("Node data is of an unknown data type.");
        }




   /******************************/
   /* Set arrays and free memory */
   /******************************/
    
    /* set output arrays */

	OMset_name_array(GeneralHistogram_id, OMstr_to_name("binval"), in_data_type, (void *)binval, 
			binval_size, OM_SET_ARRAY_FREE);
    
    
	OMset_name_array(GeneralHistogram_id, OMstr_to_name("binfreq"), DTYPE_INT, (void *)binfreq, 
			binfreq_size, OM_SET_ARRAY_FREE);

 
    /* free memory */

	if (in_max) free(in_max);
	if (in_min) free(in_min);
    
	if (in_data)
	      ARRfree((char *)in_data);



   return(1);
}
