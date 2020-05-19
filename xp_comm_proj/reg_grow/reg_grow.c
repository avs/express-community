
/*************************************************************************/
/**** THIS PROGRAM IS AN AVS/EXPRESS MODULE PROGRAMMED IN C.        ******/
/**** THE MODULE ACCEPTS ANY UNIFORM FIELD AND PERFORMS A REGION    ******/
/**** GROWING ALGORITHEM. THE USER MUST SUPPLY A SEED POINT FROM    ******/
/**** WHICH THE ALGORITHM CAN START TO GROW (THIS CAN BE SUPPLIED   ******/ 
/**** BY THE PROBE MODULE) AND A GAUSSIAN PROBABILITY ARRAY         ******/
/**** PRODUCED BY THE GENGUAS MODULE. THE OUTPUT FIELD CONTAINS     ******/
/**** VALUES RELATED TO HOW LIKELY THE POINT IS TO BE IN THE REGION ******/
/**** THIS IS NOT A PRCISE VALUE BUT ALLOWS THE REGION TO BE        ******/
/**** ISOSURFACED.                                                  ******/ 
/****                                                               ******/
/****                                                               ******/
/**** THIS WAS DEVELOPED BY JO LENG AND ALEX ALLEN IN 1998/99.      ******/
/****                                                               ******/
/*************************************************************************/


#define XP_WIDE_API

#include "xp_comm_proj/reg_grow/gen.h"
#include <math.h>
#include <avs/fld.h>

/* Changeable constants */
#define DIST_MOD 0.99        /* Defines now probability falls off with distance from first point */
#define MIN_COHERANCE 0.01   /* Defines the minimum coherance value that is accepted */



#define DIFF(a,b) (1 - fabs(a - b))


#define ERR_RETURN(MESS) {\
  ERRerror("RegionGrowCore.update",1,ERR_ORIG, MESS);\
  return(0);\
}

#define ERR_ARR_FREE {\
  if (in_dims) ARRfree((char *)in_dims);\
  if (in_points) ARRfree((char *)in_points);\
  if (in_data) ARRfree((char *)in_data);\
  if (probe_in_coord) ARRfree((char *)probe_in_coord);\
  if (coherance!=NULL) ARRfree((char *)coherance);\
  if (q_arr!=NULL) free(q_arr);\
}



/* define a 3D point structure */
typedef struct point3d{xp_long x; xp_long y; xp_long z;} Point3D;

/* define a queue structure and functions */
typedef struct q_element{Point3D point; int elemID; struct q_element *next;} Q_element;

typedef struct pointqueue{Q_element *head; Q_element *tail; char *q_check; int size;} PointQueue;


/*checks the start point and corrects values if field is cropped*/
int check_start(xp_long start_size, xp_long points_size, float *start, float *points, xp_long *new, xp_long *dims)
{
  xp_long n;
  
  if ((start_size * 2) != points_size)
     ERR_RETURN("Start dims and field dims differ.");
  
  for (n=0;n<start_size;n++) {
      new[n]=(xp_long)(start[n] - points[n]);
  }
  
  for (n=0;n<start_size;n++) {
     if ((new[n] < 0) || (new[n] >= dims[n]))
        ERR_RETURN("Incompatable in_dims and start");          
  }

  return 1;
}


/*return 0 if cannot initialise queue */
int init_queue(PointQueue *queue, char *q_arr)
{
  queue->head= NULL;
  queue->tail= NULL;
  queue->size= 0;
  queue->q_check= q_arr;
  return 1;
}

/*return 1 if new point is already in list */
/* NOTE: function is not currently used */
int queue_scan(PointQueue *queue, xp_long scanID)
{
  Q_element *curr;

  if (queue->size == 0)
     return 0;

  curr = queue->head;

  do {
     if (curr->elemID == scanID)
        return 1;

     curr = curr->next;
  } while (curr != NULL);

  return 0;
}


/*return 0 if new element is already in queue or can't be added */
int queue_push(PointQueue *queue, Point3D newpoint, xp_long newID)
{
  Q_element *new_element;

/* Scan queue to determine if point is already in queue. */
/*
  if (queue_scan(queue, newID)==1) {
     return 0;
  }
*/

  if (queue->q_check[newID]) {
     /* Point is already in queue. Don't add again. */
     return 0;
  }


  new_element= (Q_element *)(malloc(sizeof(Q_element)));

  if (new_element != NULL) {

     /* assemble new element's components */
     new_element->point = newpoint;
     new_element->elemID = newID;
     new_element->next = NULL;
      
     /* add to queue */
     if (queue->size == 0)
        queue->head= new_element;
     else
        queue->tail->next= new_element; /*change old tail element to point to new element*/

     queue->tail = new_element;       /*make new_element the new tail */
     queue->size++;

     /* Point is in queue.  Set checking flag. */
     queue->q_check[newID] = TRUE;

     return 1;
  }
  else {
     return 0;
  }
}


/* return 0 if queue empty*/
int queue_pop(PointQueue *queue, Point3D *result)
{
  Q_element *new_head;
  Q_element *old_head;

  if (queue->size > 0) {

     /* get point from head of queue */
     *result= queue->head->point;

     /* update queue*/
     new_head= queue->head->next;
     old_head= queue->head;
     queue->head= new_head;
     queue->size--;

     /* if size of queue is now 0, the set tail to NULL so it doesn't point to freed memory */
     if (queue->size == 0)
        queue->tail= NULL;

     /* Point is no longer in queue.  Clear checking flag. */
     queue->q_check[old_head->elemID] = FALSE;

     /* free old head */
     free(old_head);

     return 1;
  }
  else {
     return 0;
  }
}

/* rg_pos1D  - gives position in 1D array represeting a 3D array */
/* given the x,y & co-ordinates & dimentions of the array */
xp_long rg_pos1D(xp_long x, xp_long y, xp_long z, xp_long *dims)
{ 
   return ((z * dims[1] + y) * dims[0]) + x;
}


/*
float diff(float a, float b)
{
   float val;

   if (a > b)
      val = (a-b);
   else if (a < b)
      val = (b-a);
   else
      val = 0;

   return (1-val);
} 
*/




/* area growing routine for node data of DATA_TYPE_DOUBLE*/
void grow_double(double *in, xp_byte *out, Point3D parent_point, xp_long *dims, float *Coherance, PointQueue *point_queue_ptr, int reg_threshold)
{
  
  xp_long oldpos;

  /* loop indexs */
  xp_long dx,dy,dz;
  xp_long x,y,z;

  xp_long parent_value;
  
  float distance, difference, coherance, parent_coherance;

  /* new possition in 1D representation */
  xp_long newpos;

  xp_long new_value;

  Point3D new_point;
 
  x= parent_point.x;
  y= parent_point.y;
  z= parent_point.z;
  

  oldpos=rg_pos1D(x,y,z,dims);

  parent_coherance= Coherance[(xp_long) in[oldpos]];
  if (parent_coherance < MIN_COHERANCE)
    return;

  parent_value= out[oldpos];

  /* check surrounding pixels */
  
  for(dz= z-1; dz <= z+1; dz++)
  { if ((dz >= 0) && (dz < dims[2])) {
  
    for(dy= y-1; dy <= y+1; dy++)
    { if ((dy >= 0) && (dy < dims[1])) {
    
      for(dx= x-1; dx <= x+1; dx++)
      { if ((dx >= 0) && (dx < dims[0])) {

       if ((dx != x) || (dy != y) || (dz != z))
       {

   	    newpos= rg_pos1D(dx,dy,dz,dims);
    
   	    coherance= Coherance[(xp_long) in[newpos]];
     
          if (coherance < MIN_COHERANCE)
              continue;

   	    difference= DIFF(coherance,parent_coherance);
    
   	    distance= 1 / (sqrt( pow((dx-x),2) + pow((dy-y),2) + pow((dz-z),2) ));
	    
   	    new_value= (xp_long)(((double)parent_value) * difference * distance * DIST_MOD);

   	    if ( (new_value > out[newpos]) && (new_value > reg_threshold) ) {
              out[newpos] = (xp_byte)new_value;

              new_point.x= dx; new_point.y= dy; new_point.z= dz;
              queue_push(point_queue_ptr, new_point, newpos);
          }

       }
      }}
    }}
  }}

}





/* area gowing routine for node data of DATA_TYPE_FLOAT*/
void grow_float(float *in, xp_byte *out, Point3D parent_point, xp_long *dims, float *Coherance, PointQueue *point_queue_ptr, xp_long reg_threshold)
{
  
  xp_long oldpos;

  /* loop indexs */
  xp_long dx,dy,dz;
  xp_long x,y,z;

  xp_long parent_value;
  
  float distance, difference, coherance, parent_coherance;

  /* new possition in 1D representation */
  xp_long newpos;

  xp_long new_value;

  Point3D new_point;
 
  x= parent_point.x;
  y= parent_point.y;
  z= parent_point.z;
  

  oldpos=rg_pos1D(x,y,z,dims);

  parent_coherance= Coherance[(xp_long) in[oldpos]];
  if (parent_coherance < MIN_COHERANCE)
    return;

  parent_value= out[oldpos];
  
  /* check surrounding pixels */
  
  for(dz= z-1; dz <= z+1; dz++)
  { if ((dz >= 0) && (dz < dims[2])) {
  
    for(dy= y-1; dy <= y+1; dy++)
    { if ((dy >= 0) && (dy < dims[1])) {
    
      for(dx= x-1; dx <= x+1; dx++)
      { if ((dx >= 0) && (dx < dims[0])) {

       if ((dx != x) || (dy != y) || (dz != z))
       {

   	    newpos= rg_pos1D(dx,dy,dz,dims);
    
   	    coherance= Coherance[(xp_long) in[newpos]];
     
          if (coherance < MIN_COHERANCE)
              continue;

   	    difference= DIFF(coherance,parent_coherance);
    
   	    distance= 1 / (sqrt( pow((dx-x),2) + pow((dy-y),2) + pow((dz-z),2) ));
	    
   	    new_value= (xp_long)(((double)parent_value) * difference * distance * DIST_MOD);
	    
   	    if ( (new_value > out[newpos]) && (new_value > reg_threshold) ) {
              out[newpos] = (xp_byte)new_value;

              new_point.x= dx; new_point.y= dy; new_point.z= dz;
              queue_push(point_queue_ptr, new_point, newpos);
          }

       }
      }}
    }}
  }}


}





/* area gowing routine for node data of DATA_TYPE_INT*/
void grow_int(int *in, xp_byte *out, Point3D parent_point, xp_long *dims, float *Coherance, PointQueue *point_queue_ptr, int reg_threshold)
{
  
  xp_long oldpos;

  /* loop indexs */
  xp_long dx,dy,dz;
  xp_long x,y,z;

  xp_long parent_value;
  
  float distance, difference, coherance, parent_coherance;

  /* new possition in 1D representation */
  xp_long newpos;

  xp_long new_value;

  Point3D new_point;
 
  x= parent_point.x;
  y= parent_point.y;
  z= parent_point.z;
  

  oldpos=rg_pos1D(x,y,z,dims);

  parent_coherance= Coherance[(xp_long) in[oldpos]];
  if (parent_coherance < MIN_COHERANCE)
    return;

  parent_value= out[oldpos];

  /* check surrounding pixels */
  
  for(dz= z-1; dz <= z+1; dz++)
  { if ((dz >= 0) && (dz < dims[2])) {
  
    for(dy= y-1; dy <= y+1; dy++)
    { if ((dy >= 0) && (dy < dims[1])) {
    
      for(dx= x-1; dx <= x+1; dx++)
      { if ((dx >= 0) && (dx < dims[0])) {

       if ((dx != x) || (dy != y) || (dz != z))
       {

   	    newpos= rg_pos1D(dx,dy,dz,dims);
    
   	    coherance= Coherance[in[newpos]];
     
          if (coherance < MIN_COHERANCE)
              continue;

   	    difference= DIFF(coherance,parent_coherance);
    
   	    distance= 1 / (sqrt( pow((dx-x),2) + pow((dy-y),2) + pow((dz-z),2) ));
	    
   	    new_value= (xp_long)(((double)parent_value) * difference * distance * DIST_MOD);
	    
   	    if ( (new_value > out[newpos]) && (new_value > reg_threshold) ) {
              out[newpos] = (xp_byte)new_value;

              new_point.x= dx; new_point.y= dy; new_point.z= dz;
              queue_push(point_queue_ptr, new_point, newpos);
          }

       }
      }}
    }}
  }}
  
}



/* area gowing routine for node data of DATA_TYPE_SHORT*/
void grow_short(short *in, xp_byte *out, Point3D parent_point, xp_long *dims, float *Coherance, PointQueue *point_queue_ptr, int reg_threshold)
{
  
  xp_long oldpos;

  /* loop indexs */
  xp_long dx,dy,dz;
  xp_long x,y,z;

  xp_long parent_value;
  
  float distance, difference, coherance, parent_coherance;

  /* new possition in 1D representation */
  xp_long newpos;

  xp_long new_value;

  Point3D new_point;
 
  x= parent_point.x;
  y= parent_point.y;
  z= parent_point.z;
  

  oldpos=rg_pos1D(x,y,z,dims);

  parent_coherance= Coherance[(xp_long) in[oldpos]];
  if (parent_coherance < MIN_COHERANCE)
    return;

  parent_value= out[oldpos];

  /* check surrounding pixels */
  
  for(dz= z-1; dz <= z+1; dz++)
  { if ((dz >= 0) && (dz < dims[2])) {
  
    for(dy= y-1; dy <= y+1; dy++)
    { if ((dy >= 0) && (dy < dims[1])) {
    
      for(dx= x-1; dx <= x+1; dx++)
      { if ((dx >= 0) && (dx < dims[0])) {

       if ((dx != x) || (dy != y) || (dz != z))
       {

   	    newpos= rg_pos1D(dx,dy,dz,dims);
    
   	    coherance= Coherance[in[newpos]];
     
          if (coherance < MIN_COHERANCE)
              continue;

   	    difference= DIFF(coherance,parent_coherance);
    
   	    distance= 1 / (sqrt( pow((dx-x),2) + pow((dy-y),2) + pow((dz-z),2) ));
	    
   	    new_value= (xp_long)(((double)parent_value) * difference * distance * DIST_MOD);
	    
   	    if ( (new_value > out[newpos]) && (new_value > reg_threshold) ) {
              out[newpos] = (xp_byte)new_value;

              new_point.x= dx; new_point.y= dy; new_point.z= dz;
              queue_push(point_queue_ptr, new_point, newpos);
          }

       }
      }}
    }}
  }}

}



/* area gowing routine for node data of DATA_TYPE_CHAR*/
void grow_char(xp_byte *in, xp_byte *out, Point3D parent_point, xp_long *dims, float *Coherance, PointQueue *point_queue_ptr, int reg_threshold)
{
  
  xp_long oldpos;

  /* loop indexs */
  xp_long dx,dy,dz;
  xp_long x,y,z;

  xp_long parent_value;
  
  float distance, difference, coherance, parent_coherance;

  /* new position in 1D representation */
  xp_long newpos;

  xp_long new_value;

  Point3D new_point;
 
  x= parent_point.x;
  y= parent_point.y;
  z= parent_point.z;
  

  oldpos=rg_pos1D(x,y,z,dims);

  parent_coherance= Coherance[(xp_long) in[oldpos]];
  if (parent_coherance < MIN_COHERANCE)
    return;

  parent_value= out[oldpos];

  /* check surrounding pixels */
  
  for(dz= z-1; dz <= z+1; dz++)
  { if ((dz >= 0) && (dz < dims[2])) {
  
    for(dy= y-1; dy <= y+1; dy++)
    { if ((dy >= 0) && (dy < dims[1])) {
    
      for(dx= x-1; dx <= x+1; dx++)
      { if ((dx >= 0) && (dx < dims[0])) {

       if ((dx != x) || (dy != y) || (dz != z))
       {

   	    newpos= rg_pos1D(dx,dy,dz,dims);
    
   	    coherance= Coherance[in[newpos]];
     
          if (coherance < MIN_COHERANCE) {
              continue;
          }

   	    difference= DIFF(coherance,parent_coherance);
    
   	    distance= 1 / (sqrt( (dx-x)*(dx-x) + (dy-y)*(dy-y) + (dz-z)*(dz-z) ));
	    
   	    new_value= (xp_long)(((double)parent_value) * difference * distance * DIST_MOD);

   	    if ( (new_value > out[newpos]) && (new_value > reg_threshold) ) {
              out[newpos] = (xp_byte)new_value;

              new_point.x= dx; new_point.y= dy; new_point.z= dz;
              queue_push(point_queue_ptr, new_point, newpos);
          }

       }
      }}
    }}
  }}

}

/* area gowing routine for node data of DATA_TYPE_CHAR*/
void grow_long(xp_long *in, xp_byte *out, Point3D parent_point, xp_long *dims, float *Coherance, PointQueue *point_queue_ptr, int reg_threshold)
{
  
  xp_long oldpos;

  /* loop indexs */
  xp_long dx,dy,dz;
  xp_long x,y,z;

  xp_long parent_value;
  
  float distance, difference, coherance, parent_coherance;

  /* new position in 1D representation */
  xp_long newpos;

  xp_long new_value;

  Point3D new_point;
 
  x= parent_point.x;
  y= parent_point.y;
  z= parent_point.z;
  

  oldpos=rg_pos1D(x,y,z,dims);

  parent_coherance= Coherance[(xp_long) in[oldpos]];
  if (parent_coherance < MIN_COHERANCE)
    return;

  parent_value= out[oldpos];

  /* check surrounding pixels */
  
  for(dz= z-1; dz <= z+1; dz++)
  { if ((dz >= 0) && (dz < dims[2])) {
  
    for(dy= y-1; dy <= y+1; dy++)
    { if ((dy >= 0) && (dy < dims[1])) {
    
      for(dx= x-1; dx <= x+1; dx++)
      { if ((dx >= 0) && (dx < dims[0])) {

       if ((dx != x) || (dy != y) || (dz != z))
       {

   	    newpos= rg_pos1D(dx,dy,dz,dims);
    
   	    coherance= Coherance[in[newpos]];
     
          if (coherance < MIN_COHERANCE) {
              continue;
          }

   	    difference= DIFF(coherance,parent_coherance);
    
   	    distance= 1 / (sqrt( pow((dx-x),2) + pow((dy-y),2) + pow((dz-z),2) ));
	    
   	    new_value= (xp_long)(((double)parent_value) * difference * distance * DIST_MOD);

   	    if ( (new_value > out[newpos]) && (new_value > reg_threshold) ) {
              out[newpos] = (xp_byte)new_value;

              new_point.x= dx; new_point.y= dy; new_point.z= dz;
              queue_push(point_queue_ptr, new_point, newpos);
          }

       }
      }}
    }}
  }}

}



int
region_grow_update(OMobj_id RegionGrow_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   OMobj_id in_id;
   int      in_ndim, in_nspace;
   xp_long  *in_dims=NULL, in_npoints;
   float   *in_points=NULL;
   int      in_ncomp, in_veclen;
   int      in_data_type;
   xp_long  in_ndata;
   double   in_data_min, in_data_max;
   char    *in_data=NULL;
 
   
   OMobj_id probe_in_id;
   int      probe_in_nspace;
   xp_long  probe_in_nnodes, probe_in_size;
   float   *probe_in_coord=NULL;
   int      probe_in_ncomp, probe_in_veclen;
/*
   int      probe_in_nsets, probe_in_ncells;
   int      probe_in_cell_nnodes, *probe_in_node_connect;
   OMobj_id probe_in_cell_set;
   int      probe_in_data_type, probe_in_ndata;*/
  

   xp_long      coherance_size = 0;
   float   *coherance=NULL; 
   int      go;
   int      reg_threshold;

   OMobj_id out_id;
   int      out_ndim, out_nspace;
   xp_long *out_dims, out_npoints;
   float   *out_points;
   int      out_ncomp, out_veclen;
   int      out_data_type;
   xp_long  out_ndata;
   xp_byte    *out_data=NULL;
   
   /*Extra variables for read/write internal conversion*/
   
   double *in_data_double;
   float  *in_data_float;
   xp_long    *in_data_long;
   int    *in_data_int;
   short  *in_data_short;
   xp_byte   *in_data_char;   
   
   xp_byte   in_data_min_char;
   short  in_data_min_short;
   xp_long    in_data_min_long;
   int    in_data_min_int;
   float  in_data_min_float;
   double in_data_min_double;
   
   xp_byte   in_data_max_char;
   short  in_data_max_short;
   xp_long    in_data_max_long;
   int    in_data_max_int;
   float  in_data_max_float;
   double in_data_max_double;
   
   
   /*parameters that are not read or write*/
   
   xp_long n,index=0,i=0,progress=0;  /*counters*/
   
   
   xp_long new_start[3];           /*array to hold the start position, it may be*/
                               /*corrected if the field is cropped          */
   
   
   Point3D start_point;   /*variables to control region growing and the stack*/
   Point3D new_point;

   PointQueue point_queue;
   char *q_arr=NULL;

   Q_element test_elm;  



   /***************************************/
   /*  Check if we need to process data   */
   /***************************************/
   /* Get go's value */ 
   if (OMget_name_int_val(RegionGrow_id, OMstr_to_name("go"), &go) != 1) 
      go = 0;

   if (go != 1) {
      /*make sure that go toggle is set back to zero*/
      go = 0;
      OMset_name_int_val(RegionGrow_id, OMstr_to_name("go"), go);

      return(1);
   }

   /* Get the region threshold value */ 
   if (OMget_name_int_val(RegionGrow_id, OMstr_to_name("reg_threshold"), &reg_threshold) != 1) 
      reg_threshold = 90;


   start_point.x=0;
   start_point.y=0;
   start_point.z=0;
   
   new_point.x=0;
   new_point.y=0;
   new_point.z=0;
   
   test_elm.point=start_point;
   test_elm.next=NULL;
   
   point_queue.size=0;
   
   for(n=0;n<3;n++)
     {new_start[n]=0;}


   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get in uniform mesh */

   /* Get mesh id */
   in_id = OMfind_subobj(RegionGrow_id, OMstr_to_name("in"), OM_OBJ_RD);

   if (OMis_null_obj(in_id)) {
      ERR_RETURN("No input field.\n");
   }


   /* Get mesh nspace */
   /* If we cannot get nspace we assume that field is not completely defined yet */
   if (FLDget_nspace (in_id, &in_nspace)!=OM_STAT_SUCCESS)
      return(0);

   /* Get mesh dims array */
   FLDget_dims (in_id, &in_dims, &in_ndim);

   /* Get mesh extents */
   FLDget_points (in_id, &in_points, &in_npoints, OM_GET_ARRAY_RD);

   /* Get in's node data */

   /* Get number of node data components */
   FLDget_node_data_ncomp (in_id, &in_ncomp);

  
   if (in_ncomp != 1) {
      ERR_ARR_FREE;
      ERR_RETURN("Incorrect number of components");
   }

   /* There is only one data component. Get veclen, type and data array itself */

   /* Get veclen *** only read the first component */
   if (FLDget_node_data_veclen(in_id, 0, &in_veclen)!=OM_STAT_SUCCESS) {
      ERR_ARR_FREE;
      ERR_RETURN("Error getting in_veclen");
   }
      
   if (in_veclen != 1) {
      ERR_ARR_FREE;
      ERR_RETURN("Incorrect in_veclen");
   }

	if (FLDget_node_data_minmax(in_id, 0, (char *)&in_data_min, (char *)&in_data_max) != 1) {
      ERR_ARR_FREE;
      ERR_RETURN("Error getting node minmax");
	}


   if (FLDget_node_data (in_id, 0, &in_data_type, &in_data, &in_ndata, OM_GET_ARRAY_RD)!=OM_STAT_SUCCESS) {
      ERR_ARR_FREE;
      ERR_RETURN("Error getting node data");    
   }


        /* Get probe_in mesh */

   /* Get mesh id */
   probe_in_id = OMfind_subobj(RegionGrow_id, OMstr_to_name("probe_in"), OM_OBJ_RD);

   /* Get mesh number of nodes */
   FLDget_nnodes (probe_in_id, &probe_in_nnodes);

   if (probe_in_nnodes != 1) {
      ERR_ARR_FREE;
      ERR_RETURN("The probe field has more than one value.");
   }

   /* Get mesh nspace */
   FLDget_nspace (probe_in_id, &probe_in_nspace);

   /* Get mesh coordinates */
   FLDget_coord (probe_in_id, &probe_in_coord, &probe_in_size, OM_GET_ARRAY_RD);
   
   /* Check probe_in's node data */

   /* Get number of node data components */
   if (FLDget_node_data_ncomp (probe_in_id, &probe_in_ncomp)!=OM_STAT_SUCCESS) {
      ERR_ARR_FREE;
      ERR_RETURN("Error getting probe ncomp");
   }

   if (in_ncomp != 1) {
      ERR_ARR_FREE;
      ERR_RETURN("Incorrect number of components");
   }


   /* Only one node data component. Check veclen */    
   
   /* Get veclen */
   if (FLDget_node_data_veclen(probe_in_id, 0, &probe_in_veclen)!=OM_STAT_SUCCESS) {
      ERR_ARR_FREE;
      ERR_RETURN("Error getting probe veclen");
   }

   if (probe_in_veclen != 1) {
      ERR_ARR_FREE;
      ERR_RETURN("Incorrect probe veclen");
   }

   coherance = (float *)OMret_name_array_ptr(RegionGrow_id, OMstr_to_name("coherance"), OM_GET_ARRAY_RD, &coherance_size, NULL);

   if (coherance == NULL) {
      ERR_ARR_FREE;
      ERR_RETURN("Error getting coherance array.\n");
   }


   switch (in_data_type)
   {
      case DTYPE_DOUBLE: in_data_min_double = *(double *)&in_data_min;
                         in_data_max_double = *(double *)&in_data_max;
                         if (in_data_min_double < 0) {
                            ERR_ARR_FREE;
                            ERR_RETURN("Minimum data value must be greater than 0");
                         }
                         if (coherance_size < in_data_max_double) {
                            ERR_ARR_FREE;
                            ERR_RETURN("Coherance array is too small for Maximum data value");
                         }
                         break;

      case DTYPE_FLOAT:  in_data_min_float = *(float *)&in_data_min;
                         in_data_max_float = *(float *)&in_data_max;
                         if (in_data_min_float < 0) {
                            ERR_ARR_FREE;
                            ERR_RETURN("Minimum data value must be greater than 0");
                         }
                         if (coherance_size < in_data_max_float) {
                            ERR_ARR_FREE;
                            ERR_RETURN("Coherance array is too small for Maximum data value");
                         }
                         break;

      case DTYPE_LONG:   in_data_min_long = *(xp_long *)&in_data_min;
                         in_data_max_long = *(xp_long *)&in_data_max;
                         if (in_data_min_long < 0) {
                            ERR_ARR_FREE;
                            ERR_RETURN("Minimum data value must be greater than 0");
                         }
                         if (coherance_size < in_data_max_long) {
                            ERR_ARR_FREE;
                            ERR_RETURN("Coherance array is too small for Maximum data value");
                         }
                         break;
      case DTYPE_INT:    in_data_min_int = *(int *)&in_data_min;
                         in_data_max_int = *(int *)&in_data_max;
                         if (in_data_min_int < 0) {
                            ERR_ARR_FREE;
                            ERR_RETURN("Minimum data value must be greater than 0");
                         }
                         if (coherance_size < in_data_max_int) {
                            ERR_ARR_FREE;
                            ERR_RETURN("Coherance array is too small for Maximum data value");
                         }
                         break;

      case DTYPE_SHORT:  in_data_min_short = *(short *)&in_data_min;
                         in_data_max_short = *(short *)&in_data_max;
                         if (in_data_min_short < 0) {
                            ERR_ARR_FREE;
                            ERR_RETURN("Minimum data value must be greater than 0");
                         }
                         if (coherance_size < in_data_max_short) {
                            ERR_ARR_FREE;
                            ERR_RETURN("Coherance array is too small for Maximum data value");
                         }
                         break;

      case DTYPE_CHAR:
      case DTYPE_BYTE:   in_data_min_char = *(xp_byte *)&in_data_min;
                         in_data_max_char = *(xp_byte *)&in_data_max;
                         /* Char is unsigned number so we don't have to check for less than 0 */
                         if (coherance_size < in_data_max_char) {
                            ERR_ARR_FREE;
                            ERR_RETURN("Coherance array is too small for Maximum data value");
                         }
                         break;
   }





   /***********************/
   /* Function's Body     */
   /***********************/
   /*ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: region_grow_update generated from method: RegionGrow.update\n");*/
   
   /*This is the part of the program that contains the region growing*/   





   /*set go toggle back to zero*/
   go = 0;
   OMset_name_int_val(RegionGrow_id, OMstr_to_name("go"), go);

   /*checks the start point*/
   if (check_start(probe_in_size,in_npoints,&probe_in_coord[0],&in_points[0],&new_start[0],&in_dims[0])==0) {
      ERR_ARR_FREE;
      ERR_RETURN ("The start point is not valid");
   }
          
   
   start_point.x=new_start[0];
   start_point.y=new_start[1];
   start_point.z=new_start[2];     

   
   /***************************************/
   /*  Set output values for the out_fld  */
   /***************************************/
   /* Set out uniform mesh */ 

   /*  Get mesh id */
   out_id = OMfind_subobj(RegionGrow_id, OMstr_to_name("out"), OM_OBJ_RW);

   /* Set mesh dimensionality, out_ndim can be 1,2 or 3 */
   out_ndim=in_ndim;
   FLDset_ndim (out_id, out_ndim);

   /* Set mesh dims array */
   out_dims = (xp_long *)ARRalloc(NULL, DTYPE_LONG, out_ndim, NULL);

   /*** fill in dims array with your values ***/
   for (n=0;n<out_ndim;n++)
     {out_dims[n]=in_dims[n];}

   FLDset_dims (out_id, out_dims);
   if (out_dims)
      ARRfree((char *)out_dims);


   /* Set mesh nspace, out_nspace can be 1,2 or 3 */
   out_nspace=in_nspace;
   FLDset_nspace (out_id, out_nspace);

   /* Set mesh extents */
   out_npoints = 2*out_nspace;
   out_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, out_npoints, NULL);

   /*** fill in points array with values for 2 points: low left and high right corners of the mesh ***/

   for (n=0;n<out_npoints;n++)
      {out_points[n]=in_points[n];}

   FLDset_points (out_id, out_points, out_npoints, OM_SET_ARRAY_FREE);



   /* set out node data */

   /* Get field id */
   out_id = OMfind_subobj(RegionGrow_id, OMstr_to_name("out"), OM_OBJ_RW);

   /* Set number of node data components */
   out_ncomp=in_ncomp;   
   FLDset_node_data_ncomp (out_id, out_ncomp);

   /* There is only one component set veclen, type and data arry itself */

   /* Set veclen, assign out_veclen before next call */
   out_veclen=in_veclen;
   FLDset_node_data_veclen (out_id,0,out_veclen);

   /* Set data array */
   out_data_type = DTYPE_BYTE;      

   /* allocate out_data array first */
   /* assume float array and out_ndata is set to number of nodes */
   out_ndata=in_ndata;

   if ((out_data = (xp_byte *)ARRalloc(NULL, out_data_type ,in_ndata, NULL)) == NULL) {
      ERR_ARR_FREE;
      ERR_RETURN("Can not allocate memory for output data.\n");    
   }


   /* allocate a temporary array so that queue usage can be tracked */
   if ((q_arr = malloc(sizeof(char) * in_ndata)) == NULL) {
      ERR_ARR_FREE;
      ERR_RETURN("Can not allocate memory for temporary data.\n");
   }


   /************************************************/
   /*The main region growing part is about to start*/
   /************************************************/
    
   /*initialize the values in out to 0*/
   for (n=0;n<out_ndata;n++) {
      out_data[n]=0;
      q_arr[n]=0;
   }
     
   /*set the start point to 255 in out's data values*/ 
   out_data[rg_pos1D(new_start[0],new_start[1],new_start[2],in_dims)]=255;

   /*initializes queue and checks it*/
   if (init_queue(&point_queue, q_arr) == 0 ) {
      ERR_ARR_FREE;
      ERR_RETURN ("The queue can not be initialised");
   }
   
   /*put start point on the stack with function queue_push*/
   queue_push(&point_queue, start_point, rg_pos1D(new_start[0],new_start[1],new_start[2],in_dims) ); 	   


   /* Indicate on status bar that module is computing */
   /* Save time by NOT updating status during computation */
   OMpush_status_range(0, 100);
   OMstatus_check(1, "Growing Region", NULL);

   printf("Growing Started\n");

   switch (in_data_type)
   {
      case DTYPE_DOUBLE : in_data_double = (double *)in_data;
                          while (queue_pop(&point_queue, &new_point)) 
                          {
                             grow_double(in_data_double,out_data,new_point,in_dims,coherance,&point_queue,reg_threshold);
                             progress++;
                             if ((progress % 50)==0) {
                                printf(".");
                                fflush(stdout);
                             }
                          }
                          break;
   
      case DTYPE_FLOAT :  in_data_float = (float *)in_data;
                          while (queue_pop(&point_queue, &new_point)) 
                          {
                             grow_float(in_data_float,out_data,new_point,in_dims,coherance,&point_queue,reg_threshold);
                             progress++;
                             if ((progress % 50)==0) {
                                printf(".");
                                fflush(stdout);
                             }
                          }
                          break;
	      
      case DTYPE_INT :    in_data_int = (int *)in_data;
                          while (queue_pop(&point_queue, &new_point)) 
                          {
                             grow_int(in_data_int,out_data,new_point,in_dims,coherance,&point_queue,reg_threshold); 
                             progress++;
                             if ((progress % 50)==0) {
                                printf(".");
                                fflush(stdout);
                             }
                          }
                          break;

      case DTYPE_LONG :    in_data_long = (xp_long *)in_data;
                          while (queue_pop(&point_queue, &new_point)) 
                          {
                             grow_long(in_data_long,out_data,new_point,in_dims,coherance,&point_queue,reg_threshold); 
                             progress++;
                             if ((progress % 50)==0) {
                                printf(".");
                                fflush(stdout);
                             }
                          }
                          break;
	      
      case DTYPE_SHORT :  in_data_short = (short *)in_data;
                          while (queue_pop(&point_queue, &new_point)) 
                          {
                             grow_short(in_data_short,out_data,new_point,in_dims,coherance,&point_queue,reg_threshold);
                             progress++;
                             if ((progress % 50)==0) {
                                printf(".");
                                fflush(stdout);
                             }
                          }
                          break;		      
	      
      case DTYPE_BYTE :		      
      case DTYPE_CHAR :   in_data_char = (xp_byte *)in_data;
                          while (queue_pop(&point_queue, &new_point)) 
                          {
                             grow_char(in_data_char,out_data,new_point,in_dims,coherance,&point_queue,reg_threshold);
                             progress++;
                             if ((progress % 50)==0) {
                                printf(".");
                                fflush(stdout);
                             }
                          }
                          break;
   }

   /* Indicate on status bar that module has finished computing */
   OMstatus_check(100, "Region Growing Complete", NULL);
   OMpop_status_range();

   printf("\nGrowing Finished\n");

   FLDset_node_data (out_id, 0, out_data, out_data_type, out_ndata, OM_SET_ARRAY_FREE);
   

   /*************************/
   /*  Free input variables */
   /*************************/
   if (in_dims)
      ARRfree((char *)in_dims);
   if (in_points)
      ARRfree((char *)in_points);
   if (in_data)
      ARRfree((char *)in_data);
   if (probe_in_coord)
      ARRfree((char *)probe_in_coord);

   if (coherance != NULL) 
      ARRfree((char *)coherance);

   if (q_arr!=NULL)
      free(q_arr);

   return(1);
}

