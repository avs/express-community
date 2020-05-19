
/*************************************************************************/
/**** THIS PROGRAM IS AN AVS/EXPRESS MODULE PROGRAMMED IN C.        ******/
/**** THE MODULE ACCEPTS ANY UNIFORM FIELD AND SETS AN INITIALLY    ******/
/**** BLANK FIELD. THE USER CAN THEN MERGE IN_FLD NODE DATA INTO    ******/
/**** THE OUT_FLD NODE DATA, ACCUMULATING DATA VALUES FROM THE      ******/
/**** IN_FLD. THE IN_FLD CAN BE A CROP OF THE OUT_FLD WITH SMALLER  ******/ 
/**** DIMENSIONS TO THE OUT_FLD.                                    ******/
/****                                                               ******/
/****                                                               ******/
/**** THIS WAS DEVELOPED BY JOANNA LENG IN 1998/99.                 ******/
/****                                                               ******/
/*************************************************************************/


#include "xp_comm_proj/reg_grow/gen.h"

#define ERR_RETURN(MESS) {\
  ERRerror("AccumulateCore.update_dims",1,ERR_ORIG, MESS);\
  return(0);\
}


int
rg_accum_update_dims(OMobj_id RG_Accumulate_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
  
   /*Variables that are read or write from the module interface*/ 
   OMobj_id in_fld_id;
   int      in_fld_ndim, *in_fld_dims, in_fld_nspace, in_fld_npoints;
   float   *in_fld_points;
   int      in_fld_ncomp, in_fld_veclen;
   int      in_fld_data_type, in_fld_ndata;
   char    *in_fld_data;
   
   
   
   int  dims_size = 0;
   int *dims = NULL; 
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

   int n;                                      /*counters*/
   int m=0,x=0,y=0,z=0;
   int sml_zdims=0, sml_ydims=0, sml_xdims=0;  /*3d location counters*/
   long zshift_fst=0, zshift_lst=0;
   long yshift_fst=0, yshift_lst=0;
   long xshift_fst=0, xshift_lst=0;
   
   
   OMobj_id  dims_choice_id;   /*object id for dims_choice, to see if */
                               /*dims_choice has changed value        */

   OMobj_id  dims_id;   
   
   
   /***************************************/
   /*  Check if we need to process data   */
   /***************************************/
   /* Get dims_choice's value */ 
   if (OMget_name_int_val(RG_Accumulate_id, OMstr_to_name("dims_choice"), &dims_choice) != 1) 
      dims_choice = 0;

   if (dims_choice != 1) {
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

   /* Get field id */
   in_fld_id = OMfind_subobj(RG_Accumulate_id, OMstr_to_name("in_fld"), OM_OBJ_RD);

   /* Get number of node data components */
   FLDget_node_data_ncomp (in_fld_id, &in_fld_ncomp);

   if (in_fld_ncomp != 1)
      ERR_RETURN("Incorrect number of components");

   /* There is only one data component get veclen, type and data array itself */
   
   /* Get veclen *** only read the first component */
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
   

   dims = (int *)OMret_name_array_ptr(RG_Accumulate_id, OMstr_to_name("dims"), OM_GET_ARRAY_RD, &dims_size, NULL);


   /* Get go's value */ 
   if (OMget_name_int_val(RG_Accumulate_id, OMstr_to_name("go"), &go) != 1) 
      go = 0;

   /* Get reset's value */
   if (OMget_name_int_val(RG_Accumulate_id, OMstr_to_name("reset"), &reset) != 1) 
     reset = 0;
   
   /* Get dtype's value */
   if (OMget_name_int_val(RG_Accumulate_id, OMstr_to_name("dtype"), &dtype) != 1) 
     dtype = 0;
   
   
   
   


   /***********************/
   /* Function's Body     */
   /***********************/
   /* ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: rg_acc_dims_update generated from method: RG_Accumulate.update_dims\n"); */
   
   /*This is used to see if dims_choice has changed*/   
   dims_choice_id=OMfind_subobj(RG_Accumulate_id, OMstr_to_name("dims_choice"), OM_OBJ_RD);
         
   /*This is used to see if dims have changed*/
   dims_id=OMfind_subobj(RG_Accumulate_id, OMstr_to_name("dims"), OM_OBJ_RD);
   


   /*if reset is selected or a new field is read then out_fld is initialized*/
   /*a new field is tested for by a change in dimensions or change in node  */
   /*data type                                                              */ 
   if ( (reset == 1) || (OMchanged(dims_choice_id, seq_num) == 1) || (OMchanged(dims_id, seq_num) == 1) || (in_fld_data_type != dtype) )
   {   
      /*reset reset's value*/
      reset = 0;
      OMset_name_int_val(RG_Accumulate_id, OMstr_to_name("reset"), reset);    
      
      
      /*check the dims are compatible with the input field*/
      
      if(dims_size < in_fld_ndim) ERR_RETURN("The input field is larger than the given dimensions");
       
      out_fld_ndata=1; /*use this to find the size of the out_fld node data*/
       
      for(n=0;n<in_fld_ndim;n++)
      {
         if (dims[n] < in_fld_dims[n])
            ERR_RETURN("The input field is larger than the given dimensions");
         out_fld_ndata=out_fld_ndata*dims[n];
      }
      
      
      /*******************************************/
      /*  Set initial output values for out_fld  */
      /*******************************************/
      /* Set out_fld uniform mesh */

      /*  Get mesh id */
      out_fld_id = OMfind_subobj(RG_Accumulate_id, OMstr_to_name("out_fld"), OM_OBJ_RW);

      /* Set mesh dimensionality, out_fld_ndim can be 1,2 or 3 */
      out_fld_ndim=dims_size;
      FLDset_ndim (out_fld_id, out_fld_ndim);

      /* Set mesh dims array */
      out_fld_dims = (int *)ARRalloc(NULL, DTYPE_INT, out_fld_ndim, NULL);

      /*** fill in dims array with your values ***/
      for(n=0;n<=out_fld_ndim;n++)
        {out_fld_dims[n]=dims[n];}
   
      FLDset_dims (out_fld_id, out_fld_dims);
      if (out_fld_dims)
         ARRfree((char *)out_fld_dims);

   
   
      /* Set mesh nspace, out_fld_nspace can be 1,2 or 3 */
      out_fld_nspace=dims_size;
      FLDset_nspace (out_fld_id, out_fld_nspace);

      /* Set mesh extents */
      out_fld_npoints = 2*out_fld_nspace;
      out_fld_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, out_fld_npoints, NULL);

      /*** fill in points array with values for 2 points: low left and high right corners of the mesh ***/
   
      for (n=0;n<=(dims_size-1);n++) {
         out_fld_points[n]=0;
      }
      
      for (n=dims_size;n<=((dims_size*2-1));n++) {
         out_fld_points[n]=dims[(n-dims_size)];
      }
      

      FLDset_points (out_fld_id, out_fld_points, out_fld_npoints, OM_SET_ARRAY_FREE);
   
   
   
      /* set  out_fld node data */

      /* Get field id */
      out_fld_id = OMfind_subobj(RG_Accumulate_id, OMstr_to_name("out_fld"), OM_OBJ_RW);

      /* Set number of node data components */
      out_fld_ncomp=in_fld_ncomp;
      FLDset_node_data_ncomp (out_fld_id, out_fld_ncomp);

      /* There is only one component set veclen, type and data arry itself */
   
      /* Set veclen, assign out_fld_veclen before next call */
      out_fld_veclen=in_fld_veclen;      
      FLDset_node_data_veclen (out_fld_id, 0,out_fld_veclen);

      /* Set data array */        
      /* data_type should be set to one of the following: 
         DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
         DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE) */

      out_fld_data_type = in_fld_data_type;    

      /* allocate out_fld_data array first */
      /* assume float array and out_fld_ndata is set to number of nodes */
      
      /*out_fld_ndata is already calculated*/      
      
      if ( (out_fld_data = (char *)ARRalloc(NULL, out_fld_data_type, out_fld_ndata,NULL)) == NULL)
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
    

      FLDset_node_data (out_fld_id, 0, out_fld_data, out_fld_data_type, out_fld_ndata*out_fld_veclen, OM_SET_ARRAY_FREE);
   }
   
   
   
   
   /*if go is selected merge the in_fld node data to the out_fld node data   */
   /*and write out_fld, in this way the out_fld node data accumulates values */
   if (go == 1)
   {
      go=0;
      OMset_name_int_val(RG_Accumulate_id, OMstr_to_name("go"), go);  
      
      
      /*check the in_fld for compatibility and set up parameters for mapping*/
      /*cropped node data into uncropped node data                          */
       
      if ((in_fld_ndim==1) || (in_fld_ndim>3))
         ERR_RETURN("The input field has not been cropped properly.\n");           
       
      if (in_fld_ndim==3)
      {
         zshift_fst=dims[0] * dims[1] * in_fld_points[2];
         zshift_lst=dims[0] * dims[1] * (dims[2] - in_fld_points[5] - 1);

         sml_zdims=in_fld_points[5]-in_fld_points[2]+1;	   
      }
       
      if (in_fld_ndim==2)
      {
         zshift_fst=0;
         zshift_lst=0;

         sml_zdims=0;
      }
       
      yshift_fst=dims[0] * in_fld_points[1];
      yshift_lst=dims[0] * (dims[1] - in_fld_points[4] - 1);
      xshift_fst=in_fld_points[0];
      xshift_lst=dims[0]-in_fld_points[3] - 1;
      
      sml_ydims=in_fld_points[4]-in_fld_points[1]+1;
      sml_xdims=in_fld_points[3]-in_fld_points[0]+1;
      
      n=zshift_fst;
      m=0;
              
              
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
        case DTYPE_DOUBLE: for (z=0;z<sml_zdims;z++)
                           {
                              n=n+yshift_fst;
                              for (y=0;y<sml_ydims;y++)
                              {
                                 n=n+xshift_fst;
                                 for (x=0;x<sml_xdims;x++)
                                 {
                                    if (((double *)out_fld_data)[n]<in_fld_double[m]) {
                                       ((double *)out_fld_data)[n]=in_fld_double[m];
                                    }
                                    m++;
                                    n++;
                                 }
                                 n=n+xshift_lst;
                              }
                              n=n+yshift_lst;
                           }
                           break;
         
        case DTYPE_FLOAT:  for(z=0;z<sml_zdims;z++)
                           {
                              n=n+yshift_fst;
                              for(y=0;y<sml_ydims;y++)
                              {
                                 n=n+xshift_fst;
                                 for(x=0;x<sml_xdims;x++)
                                 {
                                    if(((float *)out_fld_data)[n]<in_fld_float[m]) {
                                       ((float *)out_fld_data)[n]=in_fld_float[m];
                                    }
                                    m++;
                                    n++;
                                 }
                                 n=n+xshift_lst;
                              }
                              n=n+yshift_lst;
                           }
                           break;
         
        case DTYPE_INT:    for(z=0;z<sml_zdims;z++)
                           {
                              n=n+yshift_fst;
                              for(y=0;y<sml_ydims;y++)
                              {
                                 n=n+xshift_fst;
                                 for(x=0;x<sml_xdims;x++)
                                 {
                                    if(((int *)out_fld_data)[n]<in_fld_int[m]) {
                                       ((int *)out_fld_data)[n]=in_fld_int[m];
                                    }
                                    m++;
                                    n++;
                                 }
                                 n=n+xshift_lst;
                              }
                              n=n+yshift_lst;
                           } 
                           break;
         
        case DTYPE_SHORT:  for(z=0;z<sml_zdims;z++)
                           {
                              n=n+yshift_fst;
                              for(y=0;y<sml_ydims;y++)
                              {
                                 n=n+xshift_fst;
                                 for(x=0;x<sml_xdims;x++)
                                 {
                                    if(((short *)out_fld_data)[n]<in_fld_short[m]) {
                                       ((short *)out_fld_data)[n]=in_fld_short[m];
                                    }
                                    m++;
                                    n++;
                                 }
                                 n=n+xshift_lst;
                              }
                              n=n+yshift_lst;
                           }
                           break;
         
        case DTYPE_CHAR: 
        case DTYPE_BYTE:   for(z=0;z<sml_zdims;z++)
                           {
                              n=n+yshift_fst;
                              for(y=0;y<sml_ydims;y++)
                              {
                                 n=n+xshift_fst;
                                 for(x=0;x<sml_xdims;x++)
                                 {
                                    if(((char *)out_fld_data)[n]<in_fld_char[m]) {
                                       ((char *)out_fld_data)[n]=in_fld_char[m];
                                    }
                                    m++;
                                    n++;
                                 }
                                 n=n+xshift_lst;
                              }
                              n=n+yshift_lst;
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

   if (dims != NULL) 
      ARRfree((char *)dims);


   return(1);
}
