

/*************************************************************************/
/**** THIS PROGRAM IS AN AVS/EXPRESS MODULE PROGRAMMED IN C.        ******/
/**** THE MODULE READS ANALYZE FILES INTO AVS/EXPRESS. IT DOES NOT  ******/
/**** READ IN ANALYZE RGB FILES AS AVS/EXPRESS DOES NOT SUPPORT     ******/
/**** RGB VOLUMES, IT CREATES IT'S OWN DATAMAP, AND IT DOES NOT     ******/ 
/**** SUPPORT COMPLEX DATA TYPES BECAUSE I HAD NO TEST DATA OF      ******/
/**** THIS TYPE.                                                    ******/
/**** THE OUTPUT UNIFORM FIELD IS STRETCHED IN THE Z AXIS TO        ******/
/**** ACCOUNT FOR THE DIFFERENCE IN THE INTER SLICE DISTANCE.       ******/ 
/**** THE INFORMATION USED TO WRITE THIS MODULE WAS OBTAINED FROM   ******/
/**** DAVID CLUNIES MEDICAL IMAGE FILE FORMAT WEB SITE WHICH IS     ******/ 
/**** CURRENTLY AT:                                                 ******/
/**** http://idt.net/~dclunie/medical-image-faq/html/index.html     ******/ 
/****                                                               ******/
/**** ANALYZE IS THE FILE FORMAT OF THE SOFTWARE PRODUCED BY THE    ******/
/**** BIOMEDICAL IMAGING RESOURCE GROUP AT THE MAYO CLINIC/         ******/
/**** FOUNDATION.
/**** http://www.mayo.edu/bir/                                      ******/
/**** I HAVE INCLUDED THEIR COPYWRITE FROM THE SOURCE I GOT THIS    ******/
/**** FROM.                                                         ******/
/****                                                               ******/
/**** THIS WAS DEVELOPED BY JO LENG AND ANDREW DODD IN 1999.        ******/
/****                                                               ******/
/*************************************************************************/
/*
 * (c) Copyright, 1986-1994
 * Biomedical Imaging Resource
 * Mayo Foundation
 *
 *
 */


#include "xp_comm_proj/rd_anlyz/gen.h"

#define ERROR_RETURN(MESS) {\
                ERRerror("ReadAnalyzeFile",1,ERR_ORIG,MESS);\
                if (filename_img) free(filename_img);\
                if (filename_in) free(filename_in);\
                return 0;\
                }

#define SIZE sizeof(struct hdr)

#define  DT_NONE           0
#define  DT_UNKNOWN        0
#define  DT_BINARY         1
#define  DT_UNSIGNED_CHAR  2
#define  DT_SIGNED_SHORT   4
#define  DT_SIGNED_INT     8
#define  DT_FLOAT          16
#define  DT_COMPLEX        32
#define  DT_DOUBLE         64
#define  DT_RGB            128
#define  DT_ALL            255

/* The single bytes of a word (16 bit) */
#define LOWBYTE(w)  (  (w)    &0x00FF )
#define HIGHBYTE(w) ( ((w)>>8)&0x00FF )

/* b1 b0 -> b0 b1: Note the "+" has a higher precedence to the"<<"  */
#define SWAPWORD(w) ( (LOWBYTE(w)<<8) + HIGHBYTE(w) )


/*The single bytes of a double word (32 bit) */
#define FIRST(dw)   ( (dw)     &0x000000FF)
#define SECOND(dw)  (((dw)>> 8)&0x000000FF)
#define THIRD(dw)   (((dw)>>16)&0x000000FF)
#define FORTH(dw)   (((dw)>>24)&0x000000FF)
#define SWAPDOUBLEWORD(dw) ( FORTH(dw) + (THIRD(dw)<<8) + (SECOND(dw)<<16) + (FIRST(dw)<<24) )

float swap_float(float val)
{
   char *old_float;
   char new_float[4];

   old_float = (char *)&val;

   new_float[0] = *(old_float+3);
   new_float[1] = *(old_float+2);
   new_float[2] = *(old_float+1);
   new_float[3] = *(old_float);

   return  *((float *)new_float);
}

double swap_double(double val)
{
   char *old_double;
   char new_double[8];

   old_double = (char *)&val;

   new_double[0] = *(old_double+7);
   new_double[1] = *(old_double+6);
   new_double[2] = *(old_double+5);
   new_double[3] = *(old_double+4);
   new_double[4] = *(old_double+3);
   new_double[5] = *(old_double+2);
   new_double[6] = *(old_double+1);
   new_double[7] = *(old_double);

   return  *((double *)new_double);
}


int
read_analyze_update(OMobj_id Read_Analyze_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   char  *filename_in = NULL;
  
  
   OMobj_id out_id;
   int out_ndim, *out_dims, out_nspace, out_npoints;
   float *out_points;
   int out_ncomp, out_comp_count, out_veclen;
   int    out_data_type, out_ndata;
   char  *out_data;
   
   /*parameters to help with write data type conversion*/
   
   double  double_data;
   float   float_data;
   int     int_data;
   short   short_data;
   char    char_data;
   
   
   int total_ndata;
   
   /*parameters that are not read or write*/
   
   typedef struct hdr{
      int       sizeof_hdr;    /*size of header file*/
      char      data_type[10];
      char      db_name[18];
      int       extents;       /*must be 16384*/
      short int session_error;      
      char      regular;       /*must be 'r'*/
      char      hkey_un0;
     
      short int dims;          /*number of dimensions*/
      short int x_dim;         /*width*/
      short int y_dim;         /*height*/
      short int z_dim;         /*depth*/
      short int t_dim;         /*number of volumes/vectors*/
      short int dim_0;
      short int dim_1;
      short int dim_2;     
      char      vox_units[4];
      char      cal_units[8];
      short int unused1;
      short int datatype;      /*data type of data*/
      short int bits;          /*bits per voxel*/
      short int dim_un0;
      float     _size_;
      float     x_size;        /*voxel width*/
      float     y_size;        /*voxel height*/
      float     z_size;        /*voxel depth*/
      float     size_0;
      float     size_1; 
      float     size_2; 
      float     size_3;      
      float     vox_offset;    /*offset of data from start of .img file*/
      float     roi_scale;     
      float     funused1;     
      float     funused2;
      float     cal_max;
      float     cal_min;
      int       compressed;
      int       verified;
      int       glmax;
      int       glmin;
     
      char      descrip[80];
      char      aux_file[24];
      char      orient;
      char      originator[10];
      char      generated[10];
      char      scannum[10];
      char      patient_id[10];
      char      exp_date[10];
      char      exp_time[10];
      char      hist_un0[3];
      int       views;
      int       vols_added;
      int       start_field;
      int       field_skip;
      int       omax;
      int       omin;
      int       smax;
      int       smin;} hdr;
     
   
   hdr HDR;                   /*to hold the header info*/
   
   FILE *fp_hdr,*fp_img;      /*for file manipulation*/     
   
   char *filename_img;
  
   int i;                     /*counter*/
   char byte_swap = 0;        /*flag, set true if byte swapping is required*/
   double scale;

   /*filename_img is malloced*/
   
   filename_img = malloc(sizeof(char)*150);
   
   if (filename_img == NULL){
      ERROR_RETURN("unable to meet malloc request for filename_img");
   }     
      
   
   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get filename_in's value */
   if (OMget_name_str_val(Read_Analyze_id, OMstr_to_name("filename_in"), &filename_in, 0) != 1)
      filename_in = NULL;


   /***********************/
   /* Function's Body     */
   /***********************/
   /*ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: read_an generated from method: Read_Analyze.update\n");*/
   
   /*get the file name of the file holding the data*/
   
   /*check the validity of the file name before we start*/     

   
   strcpy(filename_img,filename_in); 

   if (!(strrchr(filename_img,'.')==NULL))
   {
      /*if filename contains a "/" */
      if (!(strrchr(filename_img,'/')==NULL))
      {
         /*if '.' is before '/' */
         if (strrchr(filename_img,'/')>strrchr(filename_img,'.')) 
         {             
            ERROR_RETURN("Is this the right file in the right directory. There is a dot ('.') in the path.");
         } 	   
	   }      
   }

   /*remove the file extension*/
   filename_img[strlen(filename_img)-strlen(strrchr(filename_img,'.'))]='\0';

   /*add the new file extension*/
   strcat(filename_img,".img");
   
   
   
   
   if ((fp_hdr= fopen(filename_in,"rb")) == NULL)
   {
      char buf[200];
      sprintf( buf, "cannot open file %s for reading", filename_in);
      ERROR_RETURN(buf);
   }

   /**********read in the header file******************/
   if (fread(&HDR,sizeof(hdr),1,fp_hdr) != sizeof(hdr))
      
      
   /*CLOSE THE HEADER FILE*/
   
      if (fclose(fp_hdr) == EOF) {}

   /* if extents is not 16384 then the byte order might be the other way round */
   /* therefore we swap the byte order before testing the header again */
   if (HDR.extents != 16384) {
      HDR.extents = SWAPDOUBLEWORD(HDR.extents);
      byte_swap = 1;
   }


   /* check the data is valid before printing to standard output */
   if ((HDR.extents != 16384) || (HDR.regular != 'r'))
	   ERROR_RETURN("The header file is not valid");

   if (byte_swap) {
      HDR.sizeof_hdr = SWAPDOUBLEWORD(HDR.sizeof_hdr);
      HDR.session_error = SWAPWORD(HDR.session_error);

      HDR.dims = SWAPWORD(HDR.dims);
      HDR.x_dim = SWAPWORD(HDR.x_dim);
      HDR.y_dim = SWAPWORD(HDR.y_dim);
      HDR.z_dim = SWAPWORD(HDR.z_dim);
      HDR.t_dim = SWAPWORD(HDR.t_dim);
      HDR.dim_0 = SWAPWORD(HDR.dim_0);
      HDR.dim_1 = SWAPWORD(HDR.dim_1);
      HDR.dim_2 = SWAPWORD(HDR.dim_2);

      HDR.unused1 = SWAPWORD(HDR.unused1);
      HDR.datatype = SWAPWORD(HDR.datatype);
      HDR.bits = SWAPWORD(HDR.bits);
      HDR.dim_un0 = SWAPWORD(HDR.dim_un0);
      
      HDR._size_ = swap_float(HDR._size_);
      HDR.x_size = swap_float(HDR.x_size);        /*voxel width*/
      HDR.y_size = swap_float(HDR.y_size);        /*voxel height*/
      HDR.z_size = swap_float(HDR.z_size);        /*voxel depth*/
      HDR.size_0 = swap_float(HDR.size_0);
      HDR.size_1 = swap_float(HDR.size_1);
      HDR.size_2 = swap_float(HDR.size_2);
      HDR.size_3 = swap_float(HDR.size_3);
      HDR.vox_offset = swap_float(HDR.vox_offset);    /*offset of data from start of .img file*/
      HDR.roi_scale = swap_float(HDR.roi_scale);
      HDR.funused1 = swap_float(HDR.funused1);
      HDR.funused2 = swap_float(HDR.funused2);
      HDR.cal_max = swap_float(HDR.cal_max);
      HDR.cal_min = swap_float(HDR.cal_min);

      HDR.compressed = SWAPDOUBLEWORD(HDR.compressed);
      HDR.verified = SWAPDOUBLEWORD(HDR.verified);
      HDR.glmax = SWAPDOUBLEWORD(HDR.glmax);
      HDR.glmin = SWAPDOUBLEWORD(HDR.glmin);

      HDR.views = SWAPDOUBLEWORD(HDR.views);
      HDR.vols_added = SWAPDOUBLEWORD(HDR.vols_added);
      HDR.start_field = SWAPDOUBLEWORD(HDR.start_field);
      HDR.field_skip = SWAPDOUBLEWORD(HDR.field_skip);
      HDR.omax = SWAPDOUBLEWORD(HDR.omax);
      HDR.omin = SWAPDOUBLEWORD(HDR.omin);
      HDR.smax = SWAPDOUBLEWORD(HDR.smax);
      HDR.smin = SWAPDOUBLEWORD(HDR.smin);
   }
      
       
      
          
    printf("\n\nThe opened file is now %s \n\n",filename_in);
    
    printf("\nHEADER INFORMATION\n\n");
    printf("sizeof_hdr:     %d \n",HDR.sizeof_hdr);   
    printf("data_type:      %s \n",HDR.data_type);
    printf("db_name:        %s \n",HDR.db_name);    
    printf("extents:        %d \n",HDR.extents); 
    printf("session_error:  %d \n",HDR.session_error);
    printf("regular:        %c \n",HDR.regular);
    printf("hkey_un0:       %c \n\n",HDR.hkey_un0);
    
    printf("\nIMAGE DIMENSION INFORMATION\n\n");    
    printf("ndims:          %d \n",HDR.dims);
    printf("x_dim:          %d \n",HDR.x_dim);    
    printf("y_dim:          %d \n",HDR.y_dim);    
    printf("z_dim:          %d \n",HDR.z_dim); 
    printf("t_dim:          %d \n",HDR.t_dim); 
    printf("dim_0:          %d \n",HDR.dim_0);
    printf("dim_1:          %d \n",HDR.dim_1);
    printf("dim_2:          %d \n",HDR.dim_2);
    printf("vox_units:      %s \n",HDR.vox_units); 
    printf("cal_units:      %s \n",HDR.cal_units);
    printf("unused1:        %d \n",HDR.unused1);        
    printf("datatype:       %d \n",HDR.datatype);
    printf("bits:           %d \n",HDR.bits);
    printf("dim_un0:        %d \n",HDR.dim_un0);    
    printf("_size_:         %f \n",HDR._size_);    
    printf("x_size:         %f \n",HDR.x_size);    
    printf("y_size:         %f \n",HDR.y_size);
    printf("z_size:         %f \n",HDR.z_size);
    printf("size_0:         %f \n",HDR.size_0);    
    printf("size_1:         %f \n",HDR.size_1);
    printf("size_2:         %f \n",HDR.size_2);
    printf("size_3:         %f \n",HDR.size_3);     
    printf("vox_offset:     %f \n",HDR.vox_offset);    
    printf("roi_scale:      %f \n",HDR.roi_scale);
    printf("funused1:       %f \n",HDR.funused1);
    printf("funused2:       %f \n",HDR.funused2);
    printf("cal_max:        %f \n",HDR.cal_max);
    printf("cal_min:        %f \n",HDR.cal_min);
    printf("compressed:     %d \n",HDR.compressed);
    printf("verified:       %d \n",HDR.verified);    
    printf("glmax:          %d \n",HDR.glmax);      
    printf("glmin:          %d \n\n",HDR.glmin);
    
    printf("\nDATA HISTORY INFORMATION\n\n");      
    printf("descrip:        %s \n",HDR.descrip);
    printf("aux_file:       %s \n",HDR.aux_file);
    printf("orient:         %d \n",HDR.orient); 
    printf("originator:     %s \n",HDR.originator);
    printf("generated:      %s \n",HDR.generated);    
    printf("scannum:        %s \n",HDR.scannum);
    printf("patient_id:     %s \n",HDR.patient_id);    
    printf("exp_date:       %s \n",HDR.exp_date);
    printf("exp_time:       %s \n",HDR.exp_time);
    printf("hist_un0:       %s \n",HDR.hist_un0);
    printf("views:          %d \n",HDR.views);     
    printf("vols_added:     %d \n",HDR.vols_added);
    printf("start_field:    %d \n",HDR.start_field);
    printf("field_skip:     %d \n",HDR.field_skip);     
    printf("omax:           %d \n",HDR.omax);
    printf("omin:           %d \n",HDR.omin);    
    printf("smax:           %d \n",HDR.smax);
    printf("smin:           %d \n\n",HDR.smin);     

      
      
                 
    /*open the image file*/ 
    if((fp_img= fopen(filename_img,"rb")) == NULL)
    {
      char buf[200];
      sprintf( buf, "cannot open file %s for reading", filename_img);
      ERROR_RETURN(buf);
    }
     
    fseek(fp_img,HDR.vox_offset,SEEK_CUR);      

   /***********************/
   /*  Set output values  */
   /***********************/
   /* Set out uniform mesh */

   /*  Get mesh id */
   out_id = OMfind_subobj(Read_Analyze_id, OMstr_to_name("out"), OM_OBJ_RW);

   /* Set mesh dimensionality, out_ndim can be 1,2 or 3 */
   if (HDR.x_dim>0 & HDR.y_dim>0 & HDR.z_dim>0)
      {out_ndim=3;}
   else if (HDR.x_dim>0 & HDR.y_dim>0 & HDR.z_dim==0)
      {out_ndim=2;}
   else
      {ERROR_RETURN("The dimensions of the data are not standard.\n");}       
   
   FLDset_ndim (out_id, out_ndim);

   /* Set mesh dims array */
   out_dims = (int *)ARRalloc(NULL, DTYPE_INT, out_ndim, NULL);

   /*** fill in dims array with your values ***/
   out_dims[0]=HDR.x_dim;
   out_dims[1]=HDR.y_dim;
   out_ndata=out_dims[0]*out_dims[1];
   if(out_ndim==3) 
     {
       out_dims[2]=HDR.z_dim;
       out_ndata=out_ndata*out_dims[2];
     }
   
   FLDset_dims (out_id, out_dims);
   if (out_dims)
      ARRfree((char *)out_dims);

   /* Set mesh nspace, out_nspace can be 1,2 or 3 */
   out_nspace=out_ndim;
   FLDset_nspace (out_id, out_nspace);

   /* Set mesh extents */
   out_npoints = 2*out_nspace;
   out_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                               out_npoints, NULL);

   /*** fill in points array with values for 2 points: low left and high right corners of the mesh ***/

   for(i=0;i<out_npoints;i++)
     {
       if(i<out_ndim)
	 {out_points[i]=0;}
       else
	 { if ((i-out_ndim) ==0)
	     {out_points[i]=(float) HDR.x_dim;}
           if ((i-out_ndim) ==1)
	     {out_points[i]=(float) HDR.y_dim;}	 
           if ((i-out_ndim) ==2)
	     {       
	       if(HDR.x_size == HDR.y_size)
		 {
		   scale=(double) (HDR.z_size/HDR.x_size);
	           out_points[i]=(float) (HDR.z_dim*scale);
	           printf("The data has been scaled by a factor of %f in the z axis.\n\n\n",scale);
		 }
	       else
		 {
		   out_points[i]=(float) HDR.z_dim;
		   printf("The data has not been scaled in the z axis\n\n\n");
		 }
	     }
	 }
       
     }

   FLDset_points (out_id, out_points, out_npoints, 
                  OM_SET_ARRAY_FREE);
   
   
   
   
   /* set  out node data */

   /* Get field id */
   out_id = OMfind_subobj(Read_Analyze_id, OMstr_to_name("out"), OM_OBJ_RW);

   
   out_ncomp=1;
   FLDset_node_data_ncomp (out_id, out_ncomp);
   
   
  /* Set veclen, assign out_veclen before next call */
  out_veclen=HDR.t_dim;
  FLDset_node_data_veclen (out_id,0,out_veclen);


     
  switch(HDR.datatype)
    {
    case DT_ALL: ERROR_RETURN("Datatype DT_ALL is not valid for this reader.");
      break;
	 
    case DT_COMPLEX: ERROR_RETURN("Datatype DT_COMPLEX is NOT compatible with AVS/Express field file.");
      break;
	 
    case DT_RGB: ERROR_RETURN("AVS/Express does not support RGB volumes."); 
      break;
	 
    case DT_NONE: ERROR_RETURN("There is no specified datatype for this file.");
      break;	 
	 
    case DT_DOUBLE: out_data_type = DTYPE_DOUBLE;
      break;
	 
    case DT_FLOAT: out_data_type = DTYPE_FLOAT;
      break; 
	 
    case DT_SIGNED_INT: out_data_type = DTYPE_INT;
      break;
	 
    case DT_SIGNED_SHORT: out_data_type = DTYPE_SHORT;
      break;
	 
    case DT_UNSIGNED_CHAR: out_data_type = DTYPE_CHAR;
      break;
	 
    case DT_BINARY: out_data_type = DTYPE_BYTE;
      break; 	 
	 	 
    }

   total_ndata=out_ndata*out_veclen;
   
   /* allocate out_data array first */
   /* assume float array and out_ndata is set to number of nodes */
   if((out_data = (char *)ARRalloc(NULL, out_data_type, 
				   total_ndata, NULL))==NULL)
     ERROR_RETURN("Can not allocate enough memory for the output field");     
   
   
  switch(out_data_type)
    {
	 
    case DTYPE_DOUBLE:
      if (byte_swap) {
          for (i=0;i<total_ndata;i++)
          {
            fread(&double_data, sizeof(double), 1, fp_img);
            double_data = swap_double(double_data);
            ((double *)out_data)[i]=(double) double_data;
          }
      }
      else
      {
          for (i=0;i<total_ndata;i++)
          {
            fread(&double_data, sizeof(double), 1, fp_img);
            ((double *)out_data)[i]=(double) double_data;
          }
      }
      break;
	 
    case DTYPE_FLOAT:
      if (byte_swap) {
          for (i=0;i<total_ndata;i++)
          {
            fread(&float_data, sizeof(float), 1, fp_img);
            float_data = swap_float(float_data);
            ((float *)out_data)[i]=(float) float_data;
          }
      }
      else
      {
          for (i=0;i<total_ndata;i++)
          {
            fread(&float_data, sizeof(float), 1, fp_img);
            ((float *)out_data)[i]=(float) float_data;
          }
      }
      break; 
	 
    case DTYPE_INT:
      if (byte_swap) {
          for (i=0;i<total_ndata;i++)
          {
            fread(&int_data, sizeof(int), 1, fp_img);
            int_data = SWAPDOUBLEWORD(int_data);
            ((int *)out_data)[i]=(int) int_data;
          }
      }
      else
      {
          for (i=0;i<total_ndata;i++)
          {
            fread(&int_data, sizeof(int), 1, fp_img);
            ((int *)out_data)[i]=(int) int_data;
          }
      }
      break;
	 
    case DTYPE_SHORT: for (i=0;i<total_ndata;i++)
      if (byte_swap) {
          for (i=0;i<total_ndata;i++)
          {
            fread(&short_data, sizeof(short), 1, fp_img);
            short_data = SWAPWORD(short_data);
            ((short int *)out_data)[i]=(short int) short_data;
          }
      }
      else
      {
          for (i=0;i<total_ndata;i++)
          {
            fread(&short_data, sizeof(short), 1, fp_img);
            ((short int *)out_data)[i]=(short int) short_data;
          }
      }
      break;
      
    case DTYPE_BYTE:       
    case DTYPE_CHAR: for (i=0;i<total_ndata;i++)
      {
	fread(&char_data, sizeof(char), 1, fp_img);
	((char *)out_data)[i]=(char) char_data;
      }   
      break;
	 	 
    }
   
  
         
   FLDset_node_data (out_id, 0, out_data, out_data_type,
                        out_ndata*out_veclen, OM_SET_ARRAY_FREE);


   
   fclose(fp_img);
   
   
   /*free other variables*/
   if (filename_img)
      free(filename_img);

   
   /*************************/
   /*  Free input variables */
   /*************************/
   if (filename_in)
      free(filename_in);

   return(1);
}

