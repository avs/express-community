
#include "gen.h"
#define CHUNK_SIZE 20000
#define SUCCESS 1
#define FAILURE 0

//#define DEBUG 

typedef struct _ptr
   {
   unsigned short *b;
   struct _ptr    *next;
   } ptr;


typedef struct _ptr_list
   {
   ptr    list[CHUNK_SIZE];
   struct _ptr_list   *next_list;
   } ptr_list;
   
ptr_list            *ptr_lists;
ptr                 *free_ptr;
int                  n_ptrs, n_lists;

int allocate_ptrs()
{
int i;
ptr_list *newlist;
   n_lists+=1;
   newlist = (ptr_list *)malloc(sizeof(ptr_list));
   if (newlist==NULL) return FAILURE;
   newlist->next_list  = ptr_lists;
   ptr_lists = newlist;
   free_ptr  = newlist->list;
   for (i=0;i<CHUNK_SIZE-1;i++)
      free_ptr[i].next = free_ptr+i+1;
   free_ptr[CHUNK_SIZE-1].next=NULL;
   return SUCCESS;
}

ptr *add_ptr()
{
   ptr *c_ptr;
   if (free_ptr == NULL) allocate_ptrs();
   if (free_ptr == NULL) 
      {
      printf ("cannot allocate new points\n");
      return NULL;
      }
   c_ptr=free_ptr;
   free_ptr=free_ptr->next;
   c_ptr->next = NULL;
   n_ptrs+=1;
   return (c_ptr);
}

void return_ptr(ptr *p)
{
   p->next = free_ptr;
   free_ptr = p;
   n_ptrs-=1;
}

void clean_ptr()
{
ptr_list *list, *nlist;
for (list=ptr_lists;list!=NULL;list=nlist)
   {
   nlist=list->next_list;
   free(list);
   }
}


int
dis_fill(OMobj_id dis_fill_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int       init_mask;
   int       tolerance;
   int       invert_mask;
   OMobj_id  in_id;
   int       in_ndim, *in_dims, in_nspace, in_npoints;
   float    *in_points; 
   int       in_comp_count, in_nnode_data, in_veclen;
   int       in_data_type, in_ndata;
   unsigned char     *in_data[100];
   int       init_point0_size = 0;
   int      *init_point0 = NULL; 
   int       mask_num;
   int       mask_size = 0;
   unsigned char     *mask = NULL; 
   int       weight_size = 0;
   int      *weight = NULL; 
   OMobj_id  out_id;
   int       out_ndim, *out_dims, out_nspace, out_npoints;
   float    *out_points;
   int       out_ncomp, out_comp_count, out_veclen;
   int       out_data_type, out_ndata;
   unsigned char     *out_data;
   
   unsigned  short  od, *cd, *d, *outd;
   int       i, j, k, l, ll, m, step, n_discard, off[6], r_val;
   ptr      *queue[65536], *c_ptr, *a_ptr;
   
#ifdef DEBUG
   fprintf(stderr,"dist_fill called\n");
   fflush(stderr);
#endif

   r_val = 0;
   if (OMget_name_int_val(dis_fill_id, OMstr_to_name("tolerance"), &tolerance) != 1) 
      tolerance = 0;

   if (OMget_name_int_val(dis_fill_id, OMstr_to_name("invert_mask"), &invert_mask) != 1) 
      invert_mask = 0;
   if (OMget_name_int_val(dis_fill_id, OMstr_to_name("init_mask"), &init_mask) != 1) 
      init_mask = 0;
   if (OMget_name_int_val(dis_fill_id, OMstr_to_name("mask_num"), &mask_num) != 1) 
      mask_num = 0;
   mask = (unsigned char *)OMret_name_array_ptr(dis_fill_id, 
                           OMstr_to_name("mask"), OM_GET_ARRAY_RD,
			                  &mask_size, NULL);
   if (init_mask==0)
      for (i=0;i< mask_size; i++)
          if (mask[i]==mask_num) mask[i]=0;
		  
/*   if (invert_mask==0)
      for (i=0;i< mask_size; i++)
          if (mask[i]==mask_num) mask[i]=0;
*/            
   init_point0 = (int *)OMret_name_array_ptr(dis_fill_id, OMstr_to_name("init_point0"), 
                        OM_GET_ARRAY_RD, &init_point0_size, NULL);
   weight = (int *)OMret_name_array_ptr(dis_fill_id, OMstr_to_name("weight"), 
                   OM_GET_ARRAY_RD,&weight_size, NULL);

   in_id = OMfind_subobj(dis_fill_id, OMstr_to_name("in"), OM_OBJ_RD);
   FLDget_dims (in_id, &in_dims, &in_ndim);
   FLDget_nspace (in_id, &in_nspace);
   FLDget_points (in_id, &in_points, &in_npoints, 
                  OM_GET_ARRAY_RD);

   in_comp_count=0; 
   FLDget_node_data_ncomp(in_id, &in_nnode_data);
   for (in_comp_count=0; in_comp_count<in_nnode_data; in_comp_count++)
      {
      FLDget_node_data_veclen (in_id, in_comp_count, &in_veclen);
      FLDget_node_data (in_id, in_comp_count, &in_data_type, 
                       (char **)&in_data[in_comp_count],&in_ndata, 
                       OM_GET_ARRAY_RD);
      }
   if (in_ndim!=3 || in_data_type!=DTYPE_BYTE || in_nnode_data!=weight_size)
      goto clean_ex;
   ptr_lists = NULL;
   free_ptr= NULL;
   n_ptrs  = n_lists = 0;
   for (i=0;i<65536;i++)
      queue[i]=NULL;
   
   out_id = OMfind_subobj(dis_fill_id, OMstr_to_name("out"), OM_OBJ_RW);

   out_ndim = in_ndim;
   FLDset_ndim (out_id, out_ndim);
   out_dims = (int *)ARRalloc(NULL, DTYPE_INT, 
                           out_ndim, NULL);
   for (i=0;i<out_ndim;i++)
      out_dims[i]=in_dims[i];
   FLDset_dims (out_id, out_dims);
   if (out_dims)
      ARRfree(out_dims);
   out_nspace = 3;
   FLDset_nspace (out_id, out_nspace);
   out_npoints = 2*out_nspace;
   out_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                               out_npoints, NULL);
   for (i=0;i<2*out_nspace;i++)
      out_points[i]=in_points[i];
   FLDset_points (out_id, out_points, out_npoints, 
                  OM_SET_ARRAY_FREE);
   FLDset_node_data_ncomp (out_id, 1);

   out_comp_count=0;
   out_veclen = 1;
   FLDset_node_data_veclen (out_id,out_comp_count,out_veclen);
   out_data_type = DTYPE_BYTE;
   out_ndata = in_ndata;
   outd = (unsigned short *)malloc(out_ndata*sizeof(short));

   //left/right
   off[0] = -1;
   off[1] =  1;
   //aboth/underneight
   off[2] = -in_dims[0];
   off[3] =  in_dims[0];
   //next slice, slice before
   off[4] = -in_dims[0]*in_dims[1];
   off[5] =  in_dims[0]*in_dims[1];

#ifdef DEBUG
   fprintf(stderr,"in_dims %i %i %i\n",in_dims[0],in_dims[1],in_dims[2]);
   fprintf(stderr,"out_ndata %i %i  %i %i\n",out_ndata, in_dims[0]*in_dims[1]*(in_dims[2]-1),
		in_dims[0]*(in_dims[1]-1),k=in_dims[0]-1);
   fflush(stderr);
#endif

   // what is the sense of that???
   for (i=0; i<out_ndata; i++)
      outd[i]=tolerance;
   k=in_dims[0]*in_dims[1]*(in_dims[2]-1);
   l=in_dims[0]*in_dims[1];
   m=in_dims[0]*in_dims[1]*in_dims[2];
   for (i=0;i<l;i++)
      outd[i]=outd[i+k]=0;

   k=in_dims[0]*(in_dims[1]-1);   
   for (i=0;i<m;i+=l)
      for (j=0;j<in_dims[0];j++)
         outd[i+j]=outd[i+j+k]=0;
   k=in_dims[0]-1;
   for (i=0;i<m;i+=l)
       for (j=0;j<l;j+=in_dims[0])
		   outd[i+j]=outd[i+j+k]=0;

  // end of that I don't understand
         
   free_ptr  = NULL;
   ptr_lists = NULL;      
    
   //looking for first empty point
   for (i=0; i<init_point0_size/3; i++)
   {
      if (init_point0[3*i]<0) break;
      k=(in_dims[1]*init_point0[3*i+2]+init_point0[3*i+1])*in_dims[0]+init_point0[3*i];
	  //again a 0 in this vector?
      outd[k]  =0;
      c_ptr    = add_ptr();
      if (c_ptr == NULL) 
         {
         printf("cannot allocate pointers\n");
         clean_ptr();
         goto clean_ex;
         }
	  //cat new ptr onto first possition
      c_ptr->b = outd+k;
      c_ptr->next = queue[0];
      queue[0] = c_ptr;
   }
   step = n_discard = 0;

   for (i=0; i<tolerance; i++)
   {
      m = 0;
      while (queue[i]!=NULL)
      {
         m += 1;
         step += 1;
         c_ptr = queue[i];
         cd = c_ptr->b;
         queue[i] = c_ptr->next;
		 if (*cd==i)
		 {
            k  = cd - outd;
            if (mask[k]!=0 && mask[k]!=mask_num) continue;
            for (j=0; j<6; j++)
            {
				if((k+off[j]<0)  || (k+off[j]>=out_ndata))
					continue;
               if (mask[k+off[j]]!=0 && mask[k+off[j]]!=mask_num) continue;
               d = cd+off[j];
               if (*d > *cd) 
               {
                  l = *cd;
                  for (in_comp_count=0; 
                       in_comp_count<in_nnode_data; 
                       in_comp_count++)
                  {
                     ll =  in_data[in_comp_count][k+off[j]]-
                           in_data[in_comp_count][k]; 
                     l += weight[in_comp_count]*ll*ll;
                  }
                  if (*d > l)
                  {
                     *d = l;
                     a_ptr = add_ptr();
                     if (a_ptr == NULL) 
                     {
                        printf("cannot allocate pointers\n");
                        clean_ptr();
                        goto clean_ex;
                     }
                     a_ptr->b = d;
                     a_ptr->next = queue[l];
                     queue[l] = a_ptr;
                  }
               }
            }
         }
         else
		 {
            n_discard+=1;  
		 }
         return_ptr(c_ptr); 
#ifdef DEBUG
         if (step%10000==0) 
            {
            fprintf(stderr,"\r%5d %5d %6d %d\n",
            step/10000, i, n_ptrs, n_discard);
            fflush(stderr);
            }
#endif
         }
      }   
         
   clean_ptr();
   out_data = (unsigned char *)ARRalloc(NULL, out_data_type, 
                                out_veclen*out_ndata, NULL);
   for (i=0; i<out_ndata; i++)
      {
      k=(outd[i]*255)/tolerance;
      out_data[i]=255-k;
      }
   k=in_dims[0]*in_dims[1]*(in_dims[2]-1);
   l=in_dims[0]*in_dims[1];
   m=in_dims[0]*in_dims[1]*in_dims[2];
   for (i=0;i<l;i++)
      out_data[i]=out_data[i+k]=0;
   k=in_dims[0]*(in_dims[1]-1);   
   for (i=0;i<m;i+=l)
      for (j=0;j<in_dims[0];j++)
         out_data[i+j]=out_data[i+j+k]=0;
   k=in_dims[0]-1;
   for (i=0;i<m;i+=l)
       for (j=0;j<l;j+=in_dims[0])
		   out_data[i+j]=out_data[i+j+k]=0;
   
   FLDset_node_data (out_id, out_comp_count, 
                     (char *)out_data, out_data_type,
                     out_ndata, OM_SET_ARRAY_FREE);
   r_val = 1;                  
clean_ex:
   free(outd);
   if (in_dims)
      ARRfree(in_dims);
   if (in_points)
      ARRfree(in_points);

   if (init_point0 != NULL) 
      ARRfree(init_point0);
   if (weight != NULL) 
      ARRfree(weight);
   if (mask != NULL) 
      ARRfree(mask);
   for (in_comp_count=0; 
        in_comp_count<in_nnode_data; 
        in_comp_count++)
      if (in_data[in_comp_count])
         ARRfree(in_data[in_comp_count]);

#ifdef DEBUG
   fprintf(stderr,"finish\n");
   fflush (stderr);
#endif

   return r_val;
}
