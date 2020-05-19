#include <math.h> 
#include <stdlib.h> 

#include <avs/math.h>
#include <avs/fld.h>
#include "Tube_gen.h"
#include <string.h>
#include <avs/gd_def.h>
/*
#define DEBUG 1
*/

static float MY_Normalize(float *x)
{
	float den;
	if((den = VEC_LEN(x)) != 0.0)
		VEC_NORMALIZE(x, den);
	return den;
}

int
makeTubes(OMobj_id tubes_id, OMevent_mask event_mask, int seq_num)
{
    OMobj_id in_id, out_id, in_cell_set, out_cell_set;
    int      in_nspace, out_nnodes, npts, in_size, cell_type;
    int      I, i, j, in_nsets, in_set_count, in_ncells, out_ncells;
    int      *in_node_connect=NULL, *out_connect=NULL;
    float    *in_coord=NULL, *out_coord=NULL, *normals=NULL;
    int      in_ncomp=0, in_comp_count, in_veclen;
    int      in_data_type, in_ndata;
    char     *out_data=NULL, *in_data=NULL;
    double   q1, q2, radius, Theta;
    int      largeRotation, facets;
    float    sPrev[3], sNext[3], q[3], w[3], normal[3], theta;
    float    *p, *pNext = 0; /* to silence the compiler... */
    float    c[3], f1, f2;
    int      jmin, jmax, ipoly, npolys;
	int		 nn;
	float 	 *pt_normals;

    /* Get mesh id */
    in_id = OMfind_subobj(tubes_id, OMstr_to_name("in"), OM_OBJ_RD);

    FLDget_nnodes (in_id, &npts);

    normals = (float *)malloc(sizeof(float) * 3 * npts);

    FLDget_nspace (in_id, &in_nspace);

	if((in_nspace != 3 && in_nspace != 2) || npts == 0)
	{
		free(normals);
		return 0;
	}
	
    /* Get mesh coordinates */

    /* Get facets's value */ 
    if (OMget_name_int_val(tubes_id, OMstr_to_name("facets"), &facets) != 1) 
        facets = 3;

    FLDget_coord (in_id, &in_coord, &in_size, OM_GET_ARRAY_RD);

    /* Get_number of cell sets */
    FLDget_ncell_sets(in_id, &in_nsets);
    out_nnodes = 0;
    out_ncells = 0;

    /* For each cell set get information about cells */
    for(in_set_count=0; in_set_count < in_nsets; in_set_count++)
    {
        /* Get cell set id */
        FLDget_cell_set(in_id, in_set_count, &in_cell_set);

        FLDget_cell_type(in_cell_set, &cell_type);

        if(cell_type != 3) continue; /* only do polylines */

        /* Get number of cells */
        FLDget_ncells(in_cell_set, &in_ncells);

        /* Get node connectivity list */
        FLDget_npolys(in_cell_set, &npolys);

        FLDget_poly_connect(in_cell_set, &in_node_connect, &in_size, OM_GET_ARRAY_RD);
 
        for(ipoly=0; ipoly < npolys; ipoly++)
		{
			jmin = in_node_connect[2*ipoly];
			jmax = in_node_connect[2*ipoly+1];
			out_ncells += (jmax-jmin) * facets;
			out_nnodes += (jmax-jmin+1) * facets;
			for(j=jmin; j <= jmax; j++)
			{
				if(j == jmin)
				{
					p = &in_coord[in_nspace*j];
					pNext = &in_coord[in_nspace*(j+1)];
					for(i=0; i<in_nspace; i++)
					{
                 		sPrev[i] = pNext[i] - p[i];
                 		sNext[i] = sPrev[i];
					}
					if(in_nspace == 2) sPrev[2] = sNext[2] = 0.;
					MATvec3_normalize(sNext);

					/* the following logic will produce a normal orthogonal
					// to the first line segment. If we have three points
					// we use special logic to select a normal orthogonal
					// to the first two line segments
					*/
					if((jmax-jmin+1) > 2)
					{
						float ftmp[3];
               
			  			ftmp[2] = 0.;
                 		for(i=0; i<in_nspace; i++)
						{
                   			ftmp[i] = in_coord[in_nspace*(j+2) + i];
                   			ftmp[i] = ftmp[i] - pNext[i];
                 		}
                 		MATvec3_normalize(ftmp);

						/*
						// now the starting normal should simply be the cross product
						// in the following if statement we check for the case where
						// the first three points are colinear 
						*/
						MATvec3_cross_prod(normal, sNext, ftmp);
						/*MATvec3_normalize(normal);*/
						if(MY_Normalize(normal) == 0.0)
						{
                   			for(i=0; i<3; i++)
							{
                     			if(sNext[i] != 0.0 )
								{
									normal[(i+2)%3] = 0.0;
									normal[(i+1)%3] = 1.0;
									normal[i] = -sNext[(i+1)%3]/sNext[i];
									break;
								}
							}
                 		}
                 		for(i=0; i<3; i++)
						{
                   			normals[3*j+i] = normal[i];
                 		}
#ifdef DEBUG
fprintf(stderr,"normals1[%d] = (%f, %f, %f)\n", j, normals[3*j], normals[3*j+1], normals[3*j+2]);
#endif
               		}
               		if((jmax-jmin+1) <= 2 || MY_Normalize(normal) == 0.0 )
					{
                 		for (i=0; i<3; i++)
						{
                   			if(sNext[i] != 0.0 )
							{
								normal[(i+2)%3] = 0.0;
								normal[(i+1)%3] = 1.0;
								normal[i] = -sNext[(i+1)%3]/sNext[i];
								break;
							}
						}
					}
					MATvec3_normalize(normal);
					for (i=0; i<3; i++)
					{
						normals[3*j+i] = normal[i];
					}
#ifdef DEBUG
fprintf(stderr,"normals2[%d] = (%f, %f, %f)\n", j, normals[3*j], normals[3*j+1], normals[3*j+2]);
#endif
				}
				else if ( j == jmax )
				{/*last point; just insert previous */
               		for (i=0; i<3; i++)
					{
						normals[3*j+i] = normal[i];
					}
#ifdef DEBUG
fprintf(stderr,"normals3[%d] = (%f, %f, %f)\n", j, normals[3*j], normals[3*j+1], normals[3*j+2]);
#endif
				}
				else
				{ /* inbetween points */
					p = pNext;
					pNext = &in_coord[in_nspace*(j+1)];
					for(i=0; i<in_nspace; i++)
					{
                 		sPrev[i] = sNext[i];
                 		sNext[i] = pNext[i] - p[i];
               		}
 					if(in_nspace == 2) sPrev[2] = sNext[2] = 0.;
					MATvec3_normalize(sNext);
					MATvec3_cross_prod(w, sPrev, normal);
					MATvec3_normalize(w);

					largeRotation = (VEC_DOT(sPrev, sNext) < 0.0) ? 1 : 0;

					/*compute rotation of line segment */
					MATvec3_cross_prod(q, sNext, sPrev);
					if((theta=asin((double)MY_Normalize(q))) == 0.0 ) 
                 	{ /* no rotation, use previous normal */
                   		for (i=0; i<3; i++)
						{
                     		normals[3*j+i] = normal[i];
                   		}
#ifdef DEBUG
fprintf(stderr,"normals4[%d] = (%f, %f, %f)\n", j, normals[3*j], normals[3*j+1], normals[3*j+2]);
#endif
						continue;
               		}
					if(largeRotation)
					{
                 		theta = (theta > 0.0 ) ? M_PI - theta : -M_PI - theta;
               		}

					// new method
               		for (i=0; i<3; i++)
					{
                 		c[i] = sNext[i] + sPrev[i];
               		}
               		MY_Normalize(c);
               		f1 = VEC_DOT(q, normal);
               		f2 = 1.0 - f1*f1;
               		if (f2 > 0.0)
					{
                 		f2 = sqrt(1.0 - f1*f1);
               		}
               		else
					{
                 		f2 = 0.0;
               		}
               		MATvec3_cross_prod(w, c, q);
               		MATvec3_cross_prod(c, sPrev, q);
               		if (VEC_DOT(normal, c)*VEC_DOT(w, c) < 0)
					{
                 		f2 = -1.0*f2;
               		}
               		for (i=0; i<3; i++)
					{
                 		normal[i] = f1*q[i] + f2*w[i];
               		}
               		for (i=0; i<3; i++)
					{
                 		normals[3*j+i] = normal[i];
               		}
#ifdef DEBUG
fprintf(stderr,"normals5[%d] = (%f, %f, %f)\n", j, normals[3*j], normals[3*j+1], normals[3*j+2]);
#endif
             	}
          	} /* end of each polyline */
        } /* for all npolys */
	} /* for all cell_set */

	if(out_nnodes == 0) return 0;
	
    /* Get number of node data components */
    FLDget_node_data_ncomp (in_id, &in_ncomp);

    /* If it exists, copy the first node_data array to the output */
    if(in_ncomp > 0)
	{
		for (in_comp_count=0; in_comp_count < 1; in_comp_count++)
		{
        	/* Get veclen */
        	FLDget_node_data_veclen (in_id, in_comp_count, &in_veclen);

        	/* Get data array and data_type which is one of the following: 
            	DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
            	DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE */

        	FLDget_node_data (in_id, in_comp_count, &in_data_type, &in_data, &in_ndata, OM_GET_ARRAY_RD);
		}
	}

    /* Get radius's value */
    if (OMget_name_real_val(tubes_id, OMstr_to_name("bondradius"), &radius) != 1)
        radius = 0.0001;

    out_id = OMfind_subobj(tubes_id, OMstr_to_name("out"), OM_OBJ_RW);

    if(FLDset_ncell_sets(out_id, 0) != 1)
	{
		fprintf(stderr,"Error setting ncell_sets");
    }
    if(FLDset_nnodes (out_id, 0) != 1)
	{
		fprintf(stderr,"Error setting nnodes");
    }
    if(FLDset_node_data_ncomp (out_id, 0) != 1)
	{
		fprintf(stderr,"Error setting nnode_data");
    }

    FLDset_ndim(out_id, 3);
    FLDset_nspace(out_id, 3);
    FLDset_nnodes(out_id, out_nnodes);
	
#ifdef DEBUG
fprintf(stderr, "out_nnodes = %d\n", out_nnodes);
#endif

    /* Get mesh coordinates */
    FLDget_coord(out_id, &out_coord, &in_size, OM_GET_ARRAY_WR);

    Theta = 2.0 * M_PI/facets;
    
	/* vertex normals */
	pt_normals = (float *)ARRalloc(NULL, DTYPE_FLOAT, 3*out_nnodes, NULL);

    I=0;
    for(ipoly=0; ipoly < npolys; ipoly++)
	{
		jmin = in_node_connect[2*ipoly];
		jmax = in_node_connect[2*ipoly+1];
		for(j=jmin; j <= jmax; j++)
		{
			p = &in_coord[in_nspace*j];
			if(j != jmax)
			{ /* for the last point, just use previous w and normal */
            	pNext = &in_coord[in_nspace*(j+1)];
     
            	for (i=0; i<in_nspace; i++)
				{
					sNext[i] = pNext[i] - p[i];
            	}
				if(in_nspace == 2) sNext[2] = 0.;
            	for (i=0; i<3; i++)
				{
					normal[i] = normals[3*j+i];
            	}
            	MATvec3_normalize(sNext);

            	MATvec3_cross_prod(w, sNext, normal);
			}
/*
fprintf(stderr,"w[%d] = (%f, %f, %f)\n", j, w[0], w[1], w[2]);
*/
			for(i=0; i < facets; i++)
			{
				q1 = cos(i * Theta);
				q2 = sin(i * Theta);

            	out_coord[3*I+0] = p[0] + radius *(q1 * normal[0] + q2 * w[0]);
            	out_coord[3*I+1] = p[1] + radius *(q1 * normal[1] + q2 * w[1]);
            	out_coord[3*I+2] = ((in_nspace == 3) ? p[2] : 0.) + radius *(q1 * normal[2] + q2 * w[2]);
			
				/* already have the normals for each vertex */
				pt_normals[3*I+0] = q1 * normal[0] + q2 * w[0];
				pt_normals[3*I+1] = q1 * normal[1] + q2 * w[1];
				pt_normals[3*I+2] = q1 * normal[2] + q2 * w[2];
            	MATvec3_normalize(&pt_normals[3*I]);
			
            	I++;
			}
		}
	}

    FLDadd_cell_set(out_id, "Quad");
    FLDget_cell_set(out_id, 0, &out_cell_set);

    FLDset_ncells(out_cell_set, out_ncells);
    FLDget_node_connect(out_cell_set, &out_connect, &in_size, OM_GET_ARRAY_WR);
    I=0;
    for(ipoly=0; ipoly < npolys; ipoly++)
	{
		jmin = in_node_connect[2*ipoly];
		jmax = in_node_connect[2*ipoly+1];
		for(j=jmin; j < jmax; j++)
		{
			for(i=0; i < facets-1; i++)
			{
            	out_connect[I++] =  j    * facets + i;
            	out_connect[I++] =  j    * facets + i + 1;
            	out_connect[I++] = (j+1) * facets + i + 1;
            	out_connect[I++] = (j+1) * facets + i;
         	}
         	out_connect[I++] =  j    * facets + i;
         	out_connect[I++] =  j    * facets;
         	out_connect[I++] = (j+1) * facets;
         	out_connect[I++] = (j+1) * facets + i;
		}
	}
    if(in_ncomp != 0 )
	{
		nn = 1;
		if(FLDset_node_data_ncomp(out_id, 2) != 1)
		{
			fprintf(stderr,"Error setting nnode_data");
		}
		FLDset_node_data_veclen(out_id, 0, in_veclen);

      	FLDget_node_data (out_id, 0, &in_data_type, &out_data, &in_ndata, OM_GET_ARRAY_WR);
      	I=0;
      	switch(in_data_type)
		{
         case DTYPE_BYTE:
         case DTYPE_CHAR:
            for(j=0; j < npts; j++){
              for(i=0; i < facets; i++){
                out_data[I++] = in_data[j];
              }
            }
         break;
         case DTYPE_SHORT:{
            short *out, *in;
            out = (short *) out_data;
            in  = (short *) in_data;
            for(j=0; j < npts; j++){
              for(i=0; i < facets; i++){
                out[I++] = in[j];
              }
            }
         }break;
         case DTYPE_INT:{
            int *out, *in;
            out = (int *) out_data;
            in  = (int *) in_data;
            for(j=0; j < npts; j++){
              for(i=0; i < facets; i++){
                out[I++] = in[j];
              }
            }
         }break;
         case DTYPE_FLOAT:{
            float *out, *in;
            out = (float *) out_data;
            in  = (float *) in_data;
            for(j=0; j < npts; j++){
              for(i=0; i < facets; i++){
                out[I++] = in[j];
              }
            }
         }break;
         case DTYPE_DOUBLE:{
            double *out, *in;
            out = (double *) out_data;
            in  = (double *) in_data;
            for(j=0; j < npts; j++){
              for(i=0; i < facets; i++){
                out[I++] = in[j];
              }
            }
         }break;
      }
    }
	else
	{
		if(FLDset_node_data_ncomp(out_id, 1) != 1)
		{
			fprintf(stderr,"Error setting nnode_data");
		}
		nn = 0;
	}

	/* set the node normals */
	FLDset_node_data_comp(out_id, nn, 3, "Normals", "");
	FLDset_node_data_id(out_id, nn, GD_NORMAL_DATA_ID);
    FLDset_node_data(out_id, nn, (char *)pt_normals, DTYPE_FLOAT, out_nnodes*3, OM_SET_ARRAY_FREE);
	
    /*************************/
    /*  Free input variables */
    /*************************/
    if (in_data)
        ARRfree(in_data);
    if (out_data)
        ARRfree(out_data);

    if (in_node_connect)
        ARRfree(in_node_connect);
    if (in_coord)
        ARRfree(in_coord);
    if (out_coord)
        ARRfree(out_coord);

    if (out_connect != NULL) 
        ARRfree(out_connect);
    if (normals != NULL) 
        free(normals);

    return 1;
}
