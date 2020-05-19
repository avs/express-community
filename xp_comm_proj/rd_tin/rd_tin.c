#include <stdio.h>
#include <avs/f_utils.h>

#include "gen.h"

#define ARR_FREE {\
   if (filename!=NULL) free(filename);\
   if (nodes!=NULL) free(nodes);\
   if (edges!=NULL) free(edges);\
   if (triangles!=NULL) free(triangles);\
}


int
ReadTinCore_Update(OMobj_id Read_Tin_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   char  *filename = NULL;
   double  ZScale;
   int  ZeroElevations;
   OMobj_id Geometry_id;
   int Geometry_nspace, Geometry_nnodes, Geometry_size;
   int  Geometry_nsets, Geometry_set_count, Geometry_ncells;
   int  Geometry_cell_nnodes, *Geometry_node_connect;
   OMobj_id Geometry_cell_set;
   float *Geometry_coord;

/* Our own variables */

    char filename_buf[AVS_PATH_MAX];
    FILE *fp;

    char buffer[512];
    char *read_str;

    int current, isset=0;

    float x,y,z;
    int edgep1, edgep2, edgep3;
    int triedge1,triedge2,triedge3;
    int triangl0, triangl1, triangl2, triangl3, triangl4, triangl5;

    float *nodes=NULL;     /* x1,y1,z1, x2,y2,z2...xnnodes,ynnodes,znnodes */
    int *edges=NULL;       /* edgep1, edgep2 ... */
    int *triangles=NULL;   /* tri_edge1, tri_edge2, tri_edge3 ... */
    float *newnodes;
    int *newedges;
    int *newtri;
    int maxnodes;
    int maxedges;
    int maxtri;

    //float xmin, ymin, zmin, xmax, ymax, zmax;
    float zmin;
    int e,i;

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get filename's value */
   if (OMget_name_str_val(Read_Tin_id, OMstr_to_name("filename"), &filename, 0) != OM_STAT_SUCCESS) {
      fprintf(stderr,"READ_TIN: Error, could not get filename value.\n");
      return(0);
   }

   /* Get ZScale's value */
   if (OMget_name_real_val(Read_Tin_id, OMstr_to_name("ZScale"), &ZScale) != OM_STAT_SUCCESS)
      ZScale = 1.0;

   /* Get ZeroElevations's value */ 
   if (OMget_name_int_val(Read_Tin_id, OMstr_to_name("ZeroElevations"), &ZeroElevations) != OM_STAT_SUCCESS)
      ZeroElevations = 0;


   /***********************/
   /* Function's Body     */
   /***********************/

   

/* Module Compute Routine */

   if (filename == NULL)  {
      fprintf(stderr,"READ_TIN: Error, could not get filename value.\n");
      return(0);
   }

   /* Use AVS/Express function to map enviroment variables */
   FILEmap_variables(filename, filename_buf);

   if ((fp = fopen(filename_buf, "r")) == NULL) {
      fprintf(stderr,"READ_TIN: Error, could not open TIN file.\n");
      ARR_FREE;
      return(0);
   }
  
   read_str = fgets(buffer, 10, fp);
  
   if ((read_str==NULL) || (strcmp(read_str,"NODES\n")!=0)) {
      fprintf(stderr,"READ_TIN: Error, expected NODES string\n");
      fclose(fp);
      ARR_FREE;
      return(0);
   }
   
/* nodes */   
   maxnodes = 1000;
   nodes = (float *)malloc(3*maxnodes*sizeof(float));
   Geometry_nnodes=0;

   while (!feof(fp)) { 
      read_str = fgets(buffer,512,fp);
      if (read_str==NULL) {
         fprintf(stderr,"READ_TIN: Error, expected more nodes data.\n");
         fclose(fp);
         ARR_FREE;
         return(0);
      }
      if (sscanf(read_str, "%d %f %f %f", &current, &x, &y, &z)!= 4)
         break;
      Geometry_nnodes++;

/*
      if (!isset){
         xmin = xmax = x;
         ymin = ymax = y;
         zmin = zmax = z;
         isset = 1;
      } 
      else {
         xmin = xmin<x?xmin:x;
         ymin = ymin<y?ymin:y;
         zmin = zmin<z?zmin:z;

         xmax = xmax>x?xmax:x;
         ymax = ymax>y?ymax:y;
         zmax = zmax>z?zmax:z;
      }
*/

      while (current >= maxnodes) {
         maxnodes += 1000;
         /* creating new memory */
         newnodes = (float *) realloc(nodes,3*maxnodes*sizeof(float));

         if (!newnodes){ 
            /* if newnodes = NULL then send error and exit */
            fprintf(stderr,"There is not enough memory to read the nodes\n");
            fclose(fp);
            ARR_FREE;
            return(0);
   		}

         nodes = newnodes;
	   	newnodes = NULL;
      }

      nodes[(current-1)*3 + 0] = x;
      nodes[(current-1)*3 + 1] = y;
      nodes[(current-1)*3 + 2] = z;
   }

   if (feof(fp)) {
      fprintf(stderr,"READ_TIN: Error, unexpected end of file.\n");
      fclose(fp);
      ARR_FREE;
      return(0);
   }

   if ((read_str==NULL) || (strcmp(read_str,"EDGES\n")!=0)) {
      fprintf(stderr,"READ_TIN: Error, expected EDGES string.\n");
      fclose(fp);
      ARR_FREE;
      return(0);
   }

/* edges */
   maxedges=1000;
   edges = (int *)malloc(2*maxedges*sizeof(int));

   while (!feof(fp)) {
      read_str = fgets(buffer,512,fp);
      if (read_str==NULL) {
         fprintf(stderr,"READ_TIN: Error, expected more edges data.\n");
         fclose(fp);
         ARR_FREE;
         return(0);
      }
      if (sscanf(read_str, "%d %d %d %d", &current, &edgep1, &edgep2, &edgep3)!=4)
         break; 

      while (current >=maxedges) {
         maxedges += 1000;
         newedges = (int *) realloc(edges,2*maxedges*sizeof(int));

         if (!newedges){ 
            /* if newedges = NULL then send error and exit */
            fprintf(stderr,"There is not enough memory to read the edges\n");
            fclose(fp);
            ARR_FREE;
            return(0);
         }
         edges=newedges;
         newedges=NULL;
      }


      /* 2 points for each edge */
      /* The third value read appears to be either 0 or 1 and is currently ignored */

      /* we are starting to count from 1 */
      /* we are correcting this to start counting from 0 */
      /* odd and even points */

      edges[(current-1)*2 + 0] = edgep1 - 1;
      edges[(current-1)*2 + 1] = edgep2 - 1;
   }
	 
  
   if (feof(fp)) {
      fprintf(stderr,"READ_TIN: Error, unexpected end of file.\n");
      fclose(fp);
      ARR_FREE;
      return(0);
   }

   if ((read_str==NULL) || (strcmp(read_str,"TRIANGLES\n")!=0)) {
      fprintf(stderr,"READ_TIN: Error, expected TRIANGLE string.\n");
      fclose(fp);
      ARR_FREE;
      return(0);
   }


/* triangles */
   maxtri=1000;
   triangles = (int *)malloc(3*maxtri*sizeof(int)); 
   Geometry_ncells = 0;
   
   while (!feof(fp)) {
      read_str = fgets(buffer,512,fp);
      if (read_str==NULL)
         break;
      if (sscanf(read_str, "%d %d %d %d", &current, &triedge1, &triedge2, &triedge3)!=4)
         break; 

      Geometry_ncells++;

      while (current >=maxtri) {
         maxtri += 1000;
         newtri = (int *) realloc(triangles,3*maxtri*sizeof(int));

         if (!newtri) { 
            /* if newtri = NULL then send error and exit */
            fprintf(stderr,"There is not enough memory to read the triangles\n");
            fclose(fp);
            ARR_FREE;
            return(0);
         }
         triangles=newtri;
         newtri=NULL;
      }


      /* Find the 3 points for each triangle by considering the 3 edges read */
      /* we are starting to count from 1 */
      /* we are correcting this to start counting from 0 */
      /* odd and even points */

      triangles[(current-1)*3 + 0] = triangl0 = edges[(triedge1-1)*2+0];
      triangles[(current-1)*3 + 1] = triangl1 = edges[(triedge1-1)*2+1];
      triangl2 = edges[(triedge2-1)*2+0];
      triangl3 = edges[(triedge2-1)*2+1];
      triangl4 = edges[(triedge3-1)*2+0];
      triangl5 = edges[(triedge3-1)*2+1];

      /* We can determine the first two points of triangle by considering first edge */
      /* Determine final point by looking at other edges */
      /* If triangle is not degenerate we should only need to look at the second edge */
      if ((triangl2 != triangl0 ) && (triangl2 != triangl1)) {
         triangles[(current-1)*3 + 2] = triangl2;
      }
      else if ((triangl3 != triangl0 ) && (triangl3 != triangl1)) {
         triangles[(current-1)*3 + 2] = triangl3;
      }
      else if ((triangl4 != triangl0 ) && (triangl4 != triangl1)) {
         triangles[(current-1)*3 + 2] = triangl4;
      }
      else if ((triangl5 != triangl0 ) && (triangl5 != triangl1)) {
         triangles[(current-1)*3 + 2] = triangl5;
      }
      else {
         fprintf(stderr, "READ_TIN: Error, didn't find the third point of triangle %d\n", current);
         fclose(fp);
         ARR_FREE;
         return(0);
      }
   }
	 

   /* We've finished with TIN file.  Close it */
   fclose(fp);


   /***********************/
   /*  Set output values  */
   /***********************/
   /* set  Geometry mesh */

   /*  Get mesh id */
   Geometry_id = OMfind_subobj(Read_Tin_id, OMstr_to_name("Geometry"), OM_OBJ_RW);

   /* Set mesh nnodes */
   FLDset_nnodes (Geometry_id, Geometry_nnodes);

/* nspace = xyz - hence 3 position points */
   Geometry_nspace = 3;

   /* Set mesh nspace */
   FLDset_nspace (Geometry_id, Geometry_nspace);

   /* Set mesh coordinates */
   /* First allocate Geometry_coord array */
   
   Geometry_coord = (float *)ARRalloc(NULL, DTYPE_FLOAT, Geometry_nspace*Geometry_nnodes, NULL);
   
   /*** Fill in Geometry_coord array with [X,Y,Z] values at each node ***/

   if (ZeroElevations) {
      /* Find out minimum z value by scanning nodes array */
      zmin = nodes[2];
      for (e=5; e<Geometry_nspace*Geometry_nnodes; e+=3) {
         zmin = zmin < nodes[e] ? zmin : nodes[e];
      }

	   for (e=0; e<Geometry_nspace*Geometry_nnodes; e+=3) {
	   	Geometry_coord[e+0]=nodes[e+0];
	   	Geometry_coord[e+1]=nodes[e+1];
	   	Geometry_coord[e+2]=(nodes[e+2]-zmin)*ZScale;
		}
	}
	else {
	   for (e=0; e<Geometry_nspace*Geometry_nnodes; e+=3) {
	   	Geometry_coord[e+0]=nodes[e+0];
	   	Geometry_coord[e+1]=nodes[e+1];
	   	Geometry_coord[e+2]=nodes[e+2]*ZScale;
		}
	}


   /* copying coordinates into Express' coordinate space */
   FLDset_coord (Geometry_id, Geometry_coord, Geometry_nspace*Geometry_nnodes, OM_SET_ARRAY_FREE);
   

   /* Set_number of cell sets to 0 initially to clear existing data, 
      the real number of cell_sets should be assigned to Geometry_nsets variable */
   FLDset_ncell_sets(Geometry_id, 0);
   Geometry_nsets=1;

   /* For each cell set set information about cells */
   for (Geometry_set_count=0; Geometry_set_count<Geometry_nsets; Geometry_set_count++) {
      /* add cell set, the name can be one of the following:
         Point, Line, Polyline, Tri, Polytri, Polhedron, 
         Tet, Hex, Prism, Pyr,
      */
      FLDadd_cell_set(Geometry_id, "Tri");

      /* Get cell set id */
      FLDget_cell_set(Geometry_id, Geometry_set_count, &Geometry_cell_set);
       
    
      /* Set number of cells */
      FLDset_ncells(Geometry_cell_set, Geometry_ncells);
  

      /* Set node connectivity list */
      /* First allocate Geometry_node_connect */

      FLDget_cell_set_nnodes(Geometry_cell_set,  &Geometry_cell_nnodes);

      Geometry_node_connect = (int *)ARRalloc(NULL, DTYPE_INT, Geometry_ncells*Geometry_cell_nnodes, NULL);

      /*** fill in  Geometry_node_connect array with node indecies for each cell ***/
      for (e=0 ; e<Geometry_ncells*Geometry_cell_nnodes; e++) 
      {
	      /* Geometry_node_connect[e]=e; */
	      Geometry_node_connect[e]=triangles[e];
      }

      FLDset_node_connect(Geometry_cell_set, Geometry_node_connect, Geometry_ncells*Geometry_cell_nnodes, OM_SET_ARRAY_FREE);
   }
   

   /*************************/
   /*  Free input variables */
   /*************************/
   ARR_FREE;

   return(1);
}

