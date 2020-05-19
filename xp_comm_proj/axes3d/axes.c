
#define XP_WIDE_API

#include "gen.h"
#include <math.h>

#define DEBUG

int subdiv(float v0, float v1, float interval, xp_long *n, float *x0, float *d);
void make_axis(OMobj_id *mod_id, char *name, char *vname, int m, int *pos,
          xp_long *n, float *xyz, float *d, float *c0, float *v0, float *dv,
          int *label_pos, float *axe_labels_loc);
void make_plane(OMobj_id *mod_id, char *name, int m,
           xp_long *n, float *d, float *c0, float *xyz,
           char *fname,  int frame);


int axes(OMobj_id axes_id, OMevent_mask event_mask, int seq_num)
{
   
float   *min_extent = NULL;
xp_long  min_extent_size = 0;
float   *max_extent = NULL;
xp_long  max_extent_size = 0;

int      n_labels;
   
xp_long  intervals_size = 0;
float   *intervals = NULL;
   
xp_long  ixyz_size = 0;
int     *ixyz = NULL;

   
xp_long  pos_size = 0;
int     *pos = NULL;

xp_long  label_pos_size = 0;
int     *label_pos = NULL;

xp_long  frames_size = 0;
int     *frames = NULL;
   
xp_long  min_vals_size = 0;
float   *min_vals = NULL;
xp_long  max_vals_size = 0;
float   *max_vals = NULL;
  
float   *axe_labels_loc;
xp_long *num_labels = NULL;

xp_long  n[3];
float    d[3], c0[3], dv[3], v0[3], iv[3];
float    xyz[3], minv[3], maxv[3];

float    len;
xp_long  i, j;
char     format[10];
char    *names[3][5]=
           {"yz_mesh","x_labels","x_labels_v","x_labels_f","yz_frame",
            "xz_mesh","y_labels","y_labels_v","y_labels_f","xz_frame",
            "xy_mesh","z_labels","z_labels_v","z_labels_f","xy_frame"};


   if (OMget_name_int_val(axes_id, OMstr_to_name("n_labels"),
                         &n_labels)!=OM_STAT_SUCCESS)
       n_labels=15;
       
   intervals  =  (float *)OMret_name_array_ptr(axes_id,
                          OMstr_to_name("intervals"), OM_GET_ARRAY_RD,
			                &intervals_size, NULL);     
            
   min_extent =  (float *)OMret_name_array_ptr(axes_id,
                          OMstr_to_name("min_extent"), OM_GET_ARRAY_RD,
			                &min_extent_size, NULL);

   max_extent =  (float *)OMret_name_array_ptr(axes_id,
                          OMstr_to_name("max_extent"), OM_GET_ARRAY_RD,
			                &max_extent_size, NULL);

   ixyz =  (int *)OMret_name_array_ptr(axes_id,
                  OMstr_to_name("ixyz"), OM_GET_ARRAY_RD,
			        &ixyz_size, NULL);

   pos = (int *)OMret_name_array_ptr(axes_id,
                OMstr_to_name("pos"), OM_GET_ARRAY_RD,
			      &pos_size, NULL);

   label_pos = (int *)OMret_name_array_ptr(axes_id,
                OMstr_to_name("label_pos"), OM_GET_ARRAY_RD,
			      &label_pos_size, NULL);

   frames = (int *)OMret_name_array_ptr(axes_id,
                   OMstr_to_name("frames"), OM_GET_ARRAY_RD,
			         &frames_size, NULL);

	if (pos==NULL || pos_size<6 ||
	    label_pos==NULL || label_pos_size<3 ||
	    min_extent==NULL || max_extent==NULL ||
       ixyz==NULL)
	   {
      if (ixyz != NULL)       ARRfree(ixyz);
      if (min_extent != NULL) ARRfree(min_extent);
      if (max_extent != NULL) ARRfree(max_extent);
      if (pos != NULL)        ARRfree(pos);
      if (label_pos != NULL)  ARRfree(label_pos);
	   return(0);		
	   }

   min_vals = (float *)OMret_name_array_ptr(axes_id,
                       OMstr_to_name("min_vals"), OM_GET_ARRAY_RD,
			             &min_vals_size, NULL);
   max_vals = (float *)OMret_name_array_ptr(axes_id,
                       OMstr_to_name("max_vals"), OM_GET_ARRAY_RD,
			             &max_vals_size, NULL);

   if (min_vals==NULL)
      {
      for (i=0;i<3;i++)
         minv[i]=min_extent[i];
      }   
   else   
      {
      for (i=0;i<3;i++)
         minv[i]=min_vals[i];
      }   
   if (max_vals==NULL) 
      {
      for (i=0;i<3;i++)
         maxv[i]=max_extent[i];
      }   
   else   
      {
      for (i=0;i<3;i++)
         maxv[i]=max_vals[i];
      }

   if (intervals!=NULL)
      {
      for (j=0;j<3;j++)
         iv[j] = intervals[j];
      }
      for (j=0, len=0.;j<3;j++)
         len += max_extent[j] - min_extent[j];
      for (j=0;j<3;j++)
         {
         i = n_labels * (max_extent[j] - min_extent[j]) / len +1 ;
         if (i<1) i=1;
         if (iv[j] == 0) iv[j] = -i;
		   }
      
   for (j=0;j<3;j++)
      {
      i=subdiv(minv[j], maxv[j], iv[j], &n[j], &v0[j], &dv[j]);
      d[j] = dv[j]*(max_extent[j]-min_extent[j])/(maxv[j]-minv[j]);
      c0[j] = min_extent[j]-d[j]*(minv[j]-v0[j])/dv[j];
      sprintf(format,"  %s.%df  ","%",i);
      OMset_name_str_val(axes_id, OMstr_to_name(names[j][3]), format);
      if      (ixyz[j]<0)      xyz[j] = c0[j];
      else if (ixyz[j]>n[j]-1) xyz[j] = c0[j]+(n[j]-1)*d[j];
      else                     xyz[j] = c0[j]+ixyz[j]*d[j];
      }
         
   axe_labels_loc = (float *)ARRalloc(NULL, DTYPE_FLOAT, 9, NULL);
   
   for (j=0;j<3;j++)
      {
      make_plane(&axes_id, names[j][0], j, n, d, c0, xyz,
      names[j][4], frames[j]);
      make_axis(&axes_id, names[j][1], names[j][2], j, pos, n, xyz,
        d, c0, v0, dv, label_pos, axe_labels_loc);
      }
      
   OMset_name_array(axes_id, OMstr_to_name("axe_labels_loc"), DTYPE_FLOAT, 
                   (void *)axe_labels_loc, 9, OM_SET_ARRAY_FREE);

   num_labels = (xp_long *)ARRalloc(NULL, DTYPE_LONG, 3, NULL);
   for (i=0;i<3;i++)
   		num_labels[i]=n[i];
   OMset_name_array(axes_id, OMstr_to_name("num_labels"), DTYPE_LONG, 
                   (void *)num_labels, 3, OM_SET_ARRAY_FREE);

   if (ixyz != NULL)       ARRfree(ixyz);
   if (frames != NULL)     ARRfree(frames);
   if (pos != NULL)        ARRfree(pos);
   if (min_vals != NULL)   ARRfree(min_vals);
   if (max_vals != NULL)   ARRfree(max_vals);
   if (min_extent != NULL) ARRfree(min_extent);
   if (max_extent != NULL) ARRfree(max_extent);
   if (label_pos != NULL)  ARRfree(label_pos);

   return(1);
}

int subdiv(float v0, float v1, float interval, xp_long *n, float *x0, float *d)
{
xp_long k, l;
float r, s;

   if (interval==0.)
      {
      l = log10(v1-v0);
      r = pow(10,l);
      l=-l+1;
      if (l<0) l=0;
      k = (v1-v0)/r;
      if      (k<2) r/=5;
      else if (k<5) r/=2;
      }
   else if  (interval>0.)
      {
      r = interval;
      l = log10(r)-1;
      }
   else
      {
      r = (v0-v1)/interval;
      l = log10(r);
      if (r<1.) l-=1;
      s = pow(10,l);
      if      (r<2.*s) r=s;
      else if (r<5.*s) r=2.*s;
      else             r=5.*s;
      }
   k   = v0/r;
   if (v0<0) k-=1;
   *d  = r;
   *x0 = k * r;
   *n  = (v1-*x0)/r;
   if (v1>*x0+r*(*n)) *n+=1;
   *n+=1;
   if (l>0) l=0;
   return (-l);
}

void make_plane(OMobj_id *mod_id, char *name, int m,
           xp_long *n, float *d, float *c0, float *xyz, 
           char *fname, int frame)
{
xp_long i, j, k, l;

OMobj_id mesh_id;
xp_long     *mesh_dims, mesh_nnodes;
float   *mesh_coord;

   k=(m+1)%3;
   l=(m+2)%3;
   if (frame)
      {
      mesh_id = OMfind_subobj(*mod_id, OMstr_to_name(fname), OM_OBJ_RW);
      FLDset_ndim (mesh_id, 2);
      mesh_dims = (xp_long *)ARRalloc(NULL, DTYPE_LONG, 2, NULL);
      mesh_dims[0] = n[k];
      mesh_dims[1] = n[l];
      FLDset_dims (mesh_id, mesh_dims);
      FLDset_nspace (mesh_id, 3);
      mesh_nnodes = mesh_dims[0]*mesh_dims[1];
      mesh_coord = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                            3*mesh_nnodes, NULL);
      for (j=0; j<mesh_dims[1]; j++)
         for (i=0; i<mesh_dims[0]; i++)
            {
            mesh_coord[(mesh_dims[0]*j+i)*3+m] = xyz[m];
            mesh_coord[(mesh_dims[0]*j+i)*3+k] = c0[k]+i*d[k];
            mesh_coord[(mesh_dims[0]*j+i)*3+l] = c0[l]+j*d[l];
            }
      FLDset_coord (mesh_id, mesh_coord, 3*mesh_nnodes, 
                    OM_SET_ARRAY_FREE);
      if (mesh_dims)
         ARRfree(mesh_dims);
         
      mesh_id = OMfind_subobj(*mod_id, OMstr_to_name(name), OM_OBJ_RW);
      FLDset_ndim (mesh_id, 2);
      mesh_dims = (xp_long *)ARRalloc(NULL, DTYPE_LONG, 2, NULL);
      mesh_dims[0] = 2*n[k]-1;
      mesh_dims[1] = 2*n[l]-1;
      FLDset_dims (mesh_id, mesh_dims);
      FLDset_nspace (mesh_id, 3);
      mesh_nnodes = mesh_dims[0]*mesh_dims[1];
      mesh_coord = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                            3*mesh_nnodes, NULL);
      for (j=0; j<mesh_dims[1]; j++)
         for (i=0; i<mesh_dims[0]; i++)
            {
            mesh_coord[(mesh_dims[0]*j+i)*3+m] = xyz[m];
            mesh_coord[(mesh_dims[0]*j+i)*3+k] = c0[k]+i*d[k]/2.;
            mesh_coord[(mesh_dims[0]*j+i)*3+l] = c0[l]+j*d[l]/2.;
            }
      FLDset_coord (mesh_id, mesh_coord, 3*mesh_nnodes, 
                    OM_SET_ARRAY_FREE);
      if (mesh_dims)
         ARRfree(mesh_dims);
      }
   else  
      {
      mesh_id = OMfind_subobj(*mod_id, OMstr_to_name(name), OM_OBJ_RW);
      FLDset_ndim (mesh_id, 2);
      mesh_dims = (xp_long *)ARRalloc(NULL, DTYPE_LONG, 2, NULL);
      mesh_dims[0] = n[k];
      mesh_dims[1] = n[l];
      FLDset_dims (mesh_id, mesh_dims);
      FLDset_nspace (mesh_id, 3);
      mesh_nnodes = mesh_dims[0]*mesh_dims[1];
      mesh_coord = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                            3*mesh_nnodes, NULL);
      for (j=0; j<mesh_dims[1]; j++)
         for (i=0; i<mesh_dims[0]; i++)
            {
            mesh_coord[(mesh_dims[0]*j+i)*3+m] = xyz[m];
            mesh_coord[(mesh_dims[0]*j+i)*3+k] = c0[k]+i*d[k];
            mesh_coord[(mesh_dims[0]*j+i)*3+l] = c0[l]+j*d[l];
            }
      FLDset_coord (mesh_id, mesh_coord, 3*mesh_nnodes, 
                    OM_SET_ARRAY_FREE);
      if (mesh_dims)
         ARRfree(mesh_dims);
         
      mesh_id = OMfind_subobj(*mod_id, OMstr_to_name(fname), OM_OBJ_RW);
      FLDset_ndim (mesh_id, 2);
      mesh_dims = (xp_long *)ARRalloc(NULL, DTYPE_LONG, 2, NULL);
      mesh_dims[0] = 2;
      mesh_dims[1] = 2;
      FLDset_dims (mesh_id, mesh_dims);
      FLDset_nspace (mesh_id, 3);
      mesh_nnodes = mesh_dims[0]*mesh_dims[1];
      mesh_coord = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                            3*mesh_nnodes, NULL);
      for (j=0; j<mesh_dims[1]; j++)
         for (i=0; i<mesh_dims[0]; i++)
            {
            mesh_coord[(mesh_dims[0]*j+i)*3+m] = xyz[m];
            mesh_coord[(mesh_dims[0]*j+i)*3+k] = c0[k]+i*(n[k]-1)*d[k];
            mesh_coord[(mesh_dims[0]*j+i)*3+l] = c0[l]+j*(n[l]-1)*d[l];
            }
      FLDset_coord (mesh_id, mesh_coord, 3*mesh_nnodes, 
                    OM_SET_ARRAY_FREE);
      if (mesh_dims)
         ARRfree(mesh_dims);
      }
}

void make_axis(OMobj_id *mod_id, char *name, char *vname, int m, int *pos,
          xp_long *n, float *xyz, float *d, float *c0, float *v0, float *dv,
          int *label_pos, float *axe_labels_loc)
{
OMobj_id labels_id;
xp_long     *labels_dims, labels_nnodes;
float   *labels_coord, *vals;
xp_long      i, j, k, l;
float    ll[3];

   k=(m+1)%3;
   l=(m+2)%3;
   
   if (pos[2*m])
      {
      ll[l] = xyz[l];
      ll[k] = c0[k];
      if (pos[2*m+1])  ll[k] += (n[k]-1)*d[k];
      }
   else
      {
      ll[k] = xyz[k];
      ll[l] = c0[l];
      if (pos[2*m+1])  ll[l] += (n[l]-1)*d[l];
      }

   if (label_pos[m])
      axe_labels_loc[3*m+m] = c0[m]+n[m]*d[m];
   else   
      axe_labels_loc[3*m+m] = c0[m]-d[m];
   axe_labels_loc[3*m+k]=ll[k];   
   axe_labels_loc[3*m+l]=ll[l];
   
   labels_id = OMfind_subobj(*mod_id, OMstr_to_name(name), OM_OBJ_RW);
   
   FLDset_ndim (labels_id, 1);
   labels_dims = (xp_long *)ARRalloc(NULL, DTYPE_LONG, 1, NULL);
   labels_nnodes = labels_dims[0] = n[m];
   FLDset_dims (labels_id, labels_dims);
   if (labels_dims)
      ARRfree(labels_dims);
   FLDset_nspace (labels_id, 3);
   labels_coord = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                           3*labels_nnodes, NULL);
   FLDset_node_data_ncomp (labels_id, 1);
   vals = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                          labels_nnodes, NULL);
   for (i=0;i<n[m];i++)
      {
      labels_coord[3*i+k] = ll[k];
      labels_coord[3*i+l] = ll[l];
      labels_coord[3*i+m] = c0[m]+i*d[m];
      vals[i]             = v0[m]+i*dv[m];
      }
   FLDset_coord (labels_id, labels_coord, 3*labels_nnodes, 
                 OM_SET_ARRAY_FREE);
   OMset_name_array(*mod_id, OMstr_to_name(vname), DTYPE_FLOAT, 
                   (void *)vals, labels_nnodes, OM_SET_ARRAY_FREE);
}

