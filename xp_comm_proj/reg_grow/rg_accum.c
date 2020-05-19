
/*************************************************************************/
/**** THIS PROGRAM IS AN AVS/EXPRESS MODULE PROGRAMMED IN C.        ******/
/**** THE MODULE ACCEPTS ANY UNIFORM FIELD AND SETS AN INITIALLY    ******/
/**** BLANK FIELD. THE USER CAN THEN MERGE IN_FLD NODE DATA INTO    ******/
/**** THE OUT_FLD NODE DATA, ACCUMULATING DATA VALUES FROM THE      ******/
/**** IN_FLD. THE IN_FLD MUST HAVE THE SAME DIMENSIONS AS THE       ******/ 
/**** OUT_FLD.                                                      ******/
/****                                                               ******/
/****                                                               ******/
/**** THIS WAS DEVELOPED BY JOANNA LENG IN 1998/99.                 ******/
/****                                                               ******/
/*************************************************************************/


#include "xp_comm_proj/reg_grow/gen.h"

#define ERR_RETURN(MESS) {\
  ERRerror("AccumulateCore.update",1,ERR_ORIG, MESS);\
  return(0);\
}


int
rg_accum_update(OMobj_id RG_Accumulate_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
  
   /*Variables that read or write from the module interface*/ 
   OMobj_id in_fld_id;
   int      in_fld_ndim, *in_fld_dims, in_fld_nspace, in_fld_npoints;
   float   *in_fld_points;
   int      in_fld_ncomp, in_fld_veclen;
   int      in_fld_data_type, in_fld_ndata;
   char    *in_fld_data;


   int  dims_choice;
   int  go;
   int  reset;
   int  dtype;


   OMobj_id out_fld_id;
   int      out_fld_ndim, *out_fld_dims, out_fld_nspace, out_fld_npoints;
   float   *out_fld_points;
   int      out_fld_ncomp, out_fld_veclen;
   int      out_fld_data_type, out_fld_ndata;
   char    *out_fld_data;
   
   /*Extra variables for read/write variables for internal conversion*/
   
   double *in_fld_double;
   float  *in_fld_float;
   int    *in_fld_int;
   short  *in_fld_short;
   char   *in_fld_char;

   /*parameters that are not read or write*/

   int n;                      /*counter*/
   
   OMobj_id  dims_choice_id;   /*object id for dims_choice, to see if */
                               /*dims_choice has changed value        */


   /***************************************/
   /*  Check if we need to process data   */
   /***************************************/
   /* Get dims_choice's value */ 
   if (OMget_name_int_val(RG_Accumulate_id, OMstr_to_name("dims_choice"), &dims_choice) != 1) 
      dims_choice = 0;

   if (dims_choice != 0) {
      return(1);
   }


   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get in_fld uniform mesh */

   /* Get mesh id */
   in_fld_id = OMfind_subobj(RG_Accumulate_id, OMstr_to_name("in_fld"), OM_OBJ_RD);
   
   if (OMis_null_obj(in_fld_id))
      ERR_RETURN("No input field.\n");   


   /* Get mesh nspace */
   /* If we cannot get nspace we assume that field is not completely defined yet */
   if (FLDget_nspace (in_fld_id, &in_fld_nspace)!=OM_STAT_SUCCESS)
      return(0);

   /* Get mesh dims array */
   FLDget_dims (in_fld_id, &in_fld_dims, &in_fld_ndim);

   /* Get mesh extents */
   FLDget_points (in_fld_id, &in_fld_points, &in_fld_npoints, OM_GET_ARRAY_RD);


   /* Get in_fld's node data */

   /* Get number of node data components */
   FLDget_node_data_ncomp (in_fld_id, &in_fld_ncomp);

   if (in_fld_ncomp != 1)
      ERR_RETURN("Incorrect number of components");

   /* There is only one data component get veclen, type and data array itself */
   
   /* Get veclen *** only read the first component*/
   FLDget_node_data_veclen (in_fld_id, 0, &in_fld_veclen);
   
   if (in_fld_veclen != 1)
      ERR_RETURN("Incorrect veclen");   

   /* Get data array and data_type which is one of the following: 
         DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
         DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE
   */

   FLDget_node_data (in_fld_id, 0, &in_fld_data_type, &in_fld_data, &in_fld_ndata, OM_GET_ARRAY_RD);

    
   switch (in_fld_data_type)
   {
      case DTYPE_DOUBLE: in_fld_double = (double *)in_fld_data;
                         break;
   
      case DTYPE_FLOAT:  in_fld_float = (float *)in_fld_data;
                         break;
   
      case DTYPE_INT:    in_fld_int = (int *)in_fld_data;
                         break;
   
      case DTYPE_SHORT:  in_fld_short = (short *)in_fld_data;
                         break;
   
      case DTYPE_CHAR: 
      case DTYPE_BYTE:   in_fld_char = (char *)in_fld_data;
                         break;	   
    }



   /* Get go's value */ 
   if (OMget_name_int_val(RG_Accumulate_id, OMstr_to_name("go"), &go) != 1) 
      go = 0;

   /* Get reset's value */ 
   if (OMget_name_int_val(RG_Accumulate_id, OMstr_to_name("reset"), &reset) != 1) 
      reset = 0;

   /* Get dtypes's value */ 
   if (OMget_name_int_val(RG_Accumulate_id, OMstr_to_name("dtype"), &dtype) != 1) 
      dtype = 0;



   /***********************/
   /* Function's Body     */
   /***********************/
   /* ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: rg_accum_update generated from method: RG_Accumulate.update\n"); */


   /*This is used to see if dims_choice has changed*/
   dims_choice_id=OMfind_subobj(RG_Accumulate_id, OMstr_to_name("dims_choice"), OM_OBJ_RD);
   
   
 


   /*if reset is selected or a new field is read then out_fld is initialized*/
   /*a new field is tested for by a change in dimensions or change in node  */
   /*data type                                                              */
   if ( (reset == 1) || (OMchanged(dims_choice_id, seq_num) == 1) || (in_fld_data_type != dtype) )
   {
      /*reset reset's value*/
      reset = 0;
      OMset_name_int_val(RG_Accumulate_id, OMstr_to_name("reset"), reset);    

       
      /***********************************************/
      /*  Set initial output values for out_field    */
      /***********************************************/
      /* Set out_fld uniform mesh */

      /*  Get mesh id */
      out_fld_id = OMfind_subobj(RG_Accumulate_id, OMstr_to_name("out_fld"), OM_OBJ_RW);

      /* Set mesh dimensionality, out_fld_ndim can be 1,2 or 3 */
      out_fld_ndim=in_fld_ndim;   
      FLDset_ndim (out_fld_id, out_fld_ndim);

      /* Set mesh dims array */
      out_fld_dims = (int *)ARRalloc(NULL, DTYPE_INT, out_fld_ndim, NULL);

      /*** fill in dims array with your values ***/
      for(n=0;n<=out_fld_ndim;n++)
        {out_fld_dims[n]=in_fld_dims[n];}

      FLDset_dims (out_fld_id, out_fld_dims);
      if (out_fld_dims)
         ARRfree((char *)out_fld_dims);


   
      /* Set mesh nspace, out_fld_nspace can be 1,2 or 3 */
      out_fld_nspace=in_fld_nspace;   
      FLDset_nspace (out_fld_id, out_fld_nspace);

      /* Set mesh extents */
      out_fld_npoints = 2*out_fld_nspace;
      out_fld_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, out_fld_npoints, NULL);

      /*** fill in points array with values for 2 points: low left and high right corners of the mesh ***/

      for(n=0;n<=out_fld_npoints;n++)
         {out_fld_points[n]=in_fld_points[n];}

      FLDset_points (out_fld_id, out_fld_points, out_fld_npoints, OM_SET_ARRAY_FREE);


      /* set out_fld node data */

      /* Get field id */
      out_fld_id = OMfind_subobj(RG_Accumulate_id, OMstr_to_name("out_fld"), OM_OBJ_RW);

      /* Set number of node data components */
      out_fld_ncomp=in_fld_ncomp;      
      FLDset_node_data_ncomp (out_fld_id, out_fld_ncomp);

      /* There is only one component set veclen, type and data arry itself */

      /* Set veclen, assign out_fld_veclen before next call */
      out_fld_veclen=in_fld_veclen;   
      FLDset_node_data_veclen (out_fld_id, 0, out_fld_veclen);

      /* Set data array */
      /* data_type should be set to one of the following: 
         DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
         DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE) */

      out_fld_data_type = in_fld_data_type;  
      
      /* allocate out_fld_data array first */
      /* assume float array and out_fld_ndata is set to number of nodes */
      out_fld_ndata=in_fld_ndata;

      if ( (out_fld_data = (char *)ARRalloc(NULL, in_fld_data_type, in_fld_ndata,NULL)) == NULL)
         ERR_RETURN(" Can not allocate memory for output data. \n");
     
           
      switch (out_fld_data_type)
      {
         case DTYPE_DOUBLE: for (n=0;n<out_fld_ndata;n++)
                            {
                               ((double *)out_fld_data)[n]=0.0;
                            }
                            break;
         
         case DTYPE_FLOAT:  for (n=0;n<out_fld_ndata;n++)
                            {
                               ((float *)out_fld_data)[n]=(float) 0.0;
                            }
                            break;
         
         case DTYPE_INT:    for (n=0;n<out_fld_ndata;n++)
                            {
                               ((int *)out_fld_data)[n]=(int) 0;
                            } 
                            break;
         
         case DTYPE_SHORT:  for (n=0;n<out_fld_ndata;n++)
                            {
                               ((short int *)out_fld_data)[n]=(short int) 0;
                            }
                            break;
         
         case DTYPE_CHAR:
         case DTYPE_BYTE:   for(n=0;n<out_fld_ndata;n++)
                            {
                               ((char *)out_fld_data)[n]=(char) 0;
                            } 
                            break;
      }

      /*set the data type of the node data to dtype as a read write parameter*/
      dtype=in_fld_data_type;
      OMset_name_int_val(RG_Accumulate_id, OMstr_to_name("dtype"),dtype);   
    
      FLDset_node_data (out_fld_id, 0, out_fld_data, in_fld_data_type, in_fld_ndata, OM_SET_ARRAY_FREE); 	        
     
   }
   
   
   /*if go is selected add the node data of in_fld to out_fld and write   */
   /*out_fld out, in this way the out_fld node data accumulates values    */ 
   if (go == 1)
   {        
      /*reset go's value*/
      go=0;
      OMset_name_int_val(RG_Accumulate_id, OMstr_to_name("go"), go);

      /*************************************************/
      /* Read and write values for uniform mesh out_fld*/
      /*************************************************/
      /* Get out_fld uniform mesh */

      /* Get mesh id */
      out_fld_id = OMfind_subobj(RG_Accumulate_id,OMstr_to_name("out_fld"), OM_OBJ_RW);

      if (OMis_null_obj(out_fld_id))
         ERR_RETURN("Check the dims used or use reset.\n");

      /* Get data array and data_type which is one of the following: 
         DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
         DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE */

      out_fld_data_type=in_fld_data_type;
      out_fld_ndata=in_fld_ndata;
     
     
      FLDget_node_data (out_fld_id, 0, &out_fld_data_type, &out_fld_data,&out_fld_ndata, OM_GET_ARRAY_RW);
     

      switch (out_fld_data_type)
      {
         case DTYPE_DOUBLE: for (n=0;n<out_fld_ndata;n++)
                            {
                               if ( ((double *)out_fld_data)[n]<in_fld_double[n])
                               {
                                  ((double *)out_fld_data)[n]=in_fld_double[n];
                               }
                            }
                            break;
         
         case DTYPE_FLOAT:  for (n=0;n<out_fld_ndata;n++)
                            {
                               if ( ((float *)out_fld_data)[n]<in_fld_float[n])
                               {
                                  ((float *)out_fld_data)[n]=in_fld_float[n];
                               }
                            }
                            break;
         
         case DTYPE_INT:    for (n=0;n<out_fld_ndata;n++)
                            {
                               if ( ((int *)out_fld_data)[n]<in_fld_int[n])
                               {
                                  ((int *)out_fld_data)[n]=in_fld_int[n];
                               }
                            }
                            break;
         
         case DTYPE_SHORT:  for(n=0;n<out_fld_ndata;n++)
                            {
                               if ( ((short int *)out_fld_data)[n]<in_fld_short[n])
                               {
                                  ((short int *)out_fld_data)[n]=in_fld_short[n];
                               }
                            }
                            break;
         
         case DTYPE_CHAR:
         case DTYPE_BYTE:   for(n=0;n<out_fld_ndata;n++)
                            {
                               if(((char *)out_fld_data)[n]<in_fld_char[n])
                               {
                                  ((char *)out_fld_data)[n]=in_fld_char[n];
                               }
                            }
                            break;	   
      } 
     
     
      FLDset_node_data (out_fld_id, 0, out_fld_data, out_fld_data_type, out_fld_ndata, OM_SET_ARRAY_FREE); 	     

   }
   


   /*************************/
   /*  Free input variables */
   /*************************/
   if (in_fld_dims)
     ARRfree((char *)in_fld_dims);
 
   if (in_fld_points)
     ARRfree((char *)in_fld_points);
  
   if (in_fld_data)
     ARRfree((char *)in_fld_data);

   return(1);
}

