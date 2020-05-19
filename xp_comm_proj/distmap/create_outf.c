#include "gen.h"
#include <math.h>

#define SEG_REPLACE   0
#define SEG_ADD       1
#define SEG_INTERSECT 2
#define SEG_SUBTRACT  3

//#define DEBUG 

int create_outf(OMobj_id create_outf_id, OMevent_mask event_mask, int seq_num)
{
   int  init_mask;
   int  smooth;
   int  do_;
   int  t1;
   int  thr1;
   OMobj_id  in_id;
   int       in_ndim, *in_dims, in_nspace, in_npoints;
   float    *in_points;
   int       in_ncomp, in_comp_count, in_veclen;
   int       in_data_type, in_ndata;
   unsigned char  *in_data;
   OMobj_id  mask_id;
   int       mask_ndim, *mask_dims;
   int       mask_ncomp, mask_comp_count, mask_veclen;
   int       mask_data_type, mask_ndata;
   int       mask_num;
   int       mask_array_size = 0;
   unsigned char  *mask_array = NULL; 
   unsigned char  *mask_data;
   OMobj_id  out_id;
   int       out_ndim, *out_dims, out_nspace, out_npoints;
   float    *out_points;
   int       out_ncomp, out_comp_count, out_veclen;
   int       out_data_type, out_ndata;
   unsigned char  *out_data;
   

   int i,j,k,l,m,min,max;
   int off[125];
   float weight[125],w;

   if (OMget_name_int_val(create_outf_id, 
       OMstr_to_name("smooth"), &smooth) != 1) 
      smooth = 0;
   if (OMget_name_int_val(create_outf_id, 
       OMstr_to_name("do"), &do_) != 1) 
      do_ = 0;
   if (OMget_name_int_val(create_outf_id, 
       OMstr_to_name("t1"), &t1) != 1) 
      t1 = 0;
   if (OMget_name_int_val(create_outf_id, 
       OMstr_to_name("thr1"), &thr1) != 1) 
      thr1 = 127;

   if (OMget_name_int_val(create_outf_id, 
       OMstr_to_name("init_mask"), &init_mask) != 1) 
      init_mask = 0;

   if (OMget_name_int_val(create_outf_id, 
       OMstr_to_name("mask_num"), &mask_num) != 1) 
      mask_num = 0;
#ifdef DEBUG
	fprintf(stderr,"doing out thr %5d smooth lev %d smooth thr %d\n",t1,smooth,thr1);fflush(stderr);
#endif
    mask_array = (unsigned char *)OMret_name_array_ptr(
                                 create_outf_id, 
                                 OMstr_to_name("mask_array"), 
                                 OM_GET_ARRAY_RW,
			                        &mask_array_size, NULL);
	if (init_mask==SEG_REPLACE)		                        
      for (i=0;i< mask_array_size; i++)
         if (mask_array[i]==mask_num) mask_array[i]=0;

   /* Get in uniform mesh */
   in_id   = OMfind_subobj(create_outf_id, 
             OMstr_to_name("in"), OM_OBJ_RD);
   mask_id = OMfind_subobj(create_outf_id, 
             OMstr_to_name("mask_fld"), OM_OBJ_RD);
   FLDget_dims (in_id, &in_dims, &in_ndim);
   FLDget_dims (mask_id, &mask_dims, &mask_ndim);
   if (in_dims==NULL || in_ndim!=3 || mask_dims==NULL || mask_ndim!=3)
      {
      if (in_dims)
         ARRfree(in_dims);
      if (mask_dims)
         ARRfree(mask_dims);
	  #ifdef DEBUG
		fprintf(stderr,"return in_dim %i mask_ndim %i\n",in_ndim, mask_ndim);fflush(stderr);
	  #endif

      return 0;   
      }
   w=0;
   if (smooth==1)
   {
	  for (i=-1,l=0;i<2;i++)
		 for (j=-1;j<2;j++)
			for (k=-1;k<2;k++,l++)
			{
				off[l]=(i*in_dims[1]+j)*in_dims[0]+k;
				w+=(weight[l]=exp(-i*i+j*j+k*k));
			}
      for (l=0;l<27;l++)
		weight[l]/=w;
   }
   else if (smooth==2)
   {
	  for (i=-2,l=0;i<3;i++)
		 for (j=-2;j<3;j++)
			for (k=-2;k<3;k++,l++)
			{
				off[l]=(i*in_dims[1]+j)*in_dims[0]+k;
				w+=(weight[l]=exp(-i*i+j*j+k*k));
			}
      for (l=0;l<125;l++)
		weight[l]/=w;
   }

				
   FLDget_nspace (in_id, &in_nspace);
   FLDget_points (in_id, &in_points, &in_npoints, 
                  OM_GET_ARRAY_RD);

   in_comp_count=mask_comp_count=0; 
   FLDget_node_data (in_id, in_comp_count, &in_data_type, 
                     (char  **)&in_data, &in_ndata, OM_GET_ARRAY_RD);

   FLDget_node_data (mask_id, mask_comp_count, &mask_data_type, 
                     (char  **)&mask_data, &mask_ndata, OM_GET_ARRAY_RD);

   out_id = OMfind_subobj(create_outf_id, OMstr_to_name("out"), OM_OBJ_RW);
   out_ndim=3;
   FLDset_ndim (out_id, out_ndim);
   out_dims = (int *)ARRalloc(NULL, DTYPE_INT, 
                           out_ndim, NULL);
   for (i=0;i<out_ndim;i++)
      out_dims[i] = in_dims[i];
   FLDset_dims (out_id, out_dims);
   if (out_dims)
      ARRfree(out_dims);

   out_nspace = 3;
   FLDset_nspace (out_id, out_nspace);

   out_npoints = 2*out_nspace;
   out_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                               out_npoints, NULL);
   for (i=0;i<out_npoints;i++)
      out_points[i] = in_points[i];
   FLDset_points (out_id, out_points, out_npoints, 
                  OM_SET_ARRAY_FREE);
   out_ncomp = 1;               
   FLDset_node_data_ncomp (out_id, out_ncomp);
   out_comp_count=0; 
   out_veclen = 1;      
   FLDset_node_data_veclen (out_id,out_comp_count,out_veclen);
   out_data_type = DTYPE_BYTE;
   out_ndata = in_ndata;
   out_data = (unsigned char *)ARRalloc(NULL, out_data_type, 
                                 out_veclen*out_ndata, NULL);
   for (l=0; l<out_ndata; l++)
	  if (mask_data[l]>t1) out_data[l]=255;
      else                     out_data[l]=0;
   switch (smooth)
   {
   case 0:
      break;
   case 1:
      for (i=1;i<in_dims[2]-1;i++)
         for (j=1;j<in_dims[1]-1;j++)
            for (k=1,l=(i*in_dims[1]+j)*in_dims[0]+1;k<in_dims[0]-1;k++,l++)
               {
               w=0;
               for (m=0;m<27;m++)
			   {
				  if((l+off[m]<0) || (l+off[m]>mask_comp_count))continue;
                  if (mask_data[l+off[m]]>t1)
                     w+=weight[m];
			   }
               out_data[l]=255*w;
               }
      break;
   case 2:
      for (i=2;i<in_dims[2]-2;i++)
         for (j=2;j<in_dims[1]-2;j++)
            for (k=2,l=(i*in_dims[1]+j)*in_dims[0]+1;k<in_dims[0]-2;k++,l++)
               {     
               w=0;
               for (m=0;m<125;m++)
			   {
				  if((l+off[m]<0) || (l+off[m]>mask_comp_count))continue;
                  if (mask_data[l+off[m]]>t1)
                     w+=weight[m];
			   }
               out_data[l]=255*w;
               }
	  break;
   }
   switch(init_mask)
      {
   case SEG_REPLACE:   
      for (i=0;i<out_ndata;i++)
         if (out_data[i]>thr1 && mask_array[i]==0)
             mask_array[i]=mask_num;
      break;     
   case SEG_ADD:   
      for (i=0;i<out_ndata;i++)
         if (out_data[i]>thr1 && mask_array[i]==0)
             mask_array[i]=mask_num;
      break;     
   case SEG_INTERSECT:   
      for (i=0;i<out_ndata;i++)
	  {		 
         if (out_data[i]<=thr1 && mask_array[i]==mask_num)
             mask_array[i]=0;
	  }
	  break;
   case SEG_SUBTRACT:   
      for (i=0;i<out_ndata;i++)
         if (out_data[i]>thr1 && mask_array[i]==mask_num)
             mask_array[i]=0;
      break;     
      } 
   max=0; min=256;                              
   for (i=0;i<in_ndata;i++)
       if (mask_array[i]==mask_num)
           {
           if (in_data[i]>max) max=in_data[i];
           if (in_data[i]<min) min=in_data[i];
           }
   if (min==max) max=min+1;        
#ifdef DEBUG
   fprintf(stderr,"%d %d\n",min,max);fflush(stderr);        
#endif
   for (i=0;i<out_ndata;i++)
       if (mask_array[i]!=mask_num)
           out_data[i]=(55*in_data[i])/255;
       else   
           {
           k = 200*(in_data[i]-min);
           out_data[i]=55+k/(max-min);
          }
   
   FLDset_node_data (out_id, out_comp_count, (char *)out_data, 
                     out_data_type,
                     out_ndata*out_veclen, OM_SET_ARRAY_FREE);

   if (in_dims)
      ARRfree(in_dims);
   if (in_points)
      ARRfree(in_points);

   if (mask_dims)
      ARRfree(mask_dims);
   if (in_data)
      ARRfree(in_data);
   if (mask_data)
      ARRfree(mask_data);

   if (mask_array != NULL) 
      ARRfree(mask_array);
   return(1);
}
