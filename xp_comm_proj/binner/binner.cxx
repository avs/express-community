/*
	Binner for scatter data

	This module accepts a scatter field and returns a count of occurrences of values
	binned along the coordinates and/or the values.

	The binning on the coordinates can be done with:
		- a cartesian grid for 1D, 2D or 3D input fields.
		- a polar grid along R and rho. Optionally the resulting count value can be
		  adjusted to correct the disuniform bin size.

	Inputs:

		fld					the input scatter field.
							The limitations on the nspace values are:
							1	valid for cartesian with or without binning on data
							2	valid for cartesian with or without binning on data
								valid for polar and normalized polar
							3	valid for cartesian with or without binning on data
							The node data is converted to float before being processed.


	Parameters:

		coordinate_system:	specify the binning type:
							"cartesian"			uniform binning along X, Y, Z (default)
							"polar"				regular binning along R and rho
							"normalized polar"	binning along R and rho adjusted to have equal
												area bins

		ncoord_bins			array of number of bins along every coordinate.
							To ignore binning along one dimension, put the number
							of bins <= 1

		ndata_bins			number of bins on the data.
							To disable data binning, set this value to <= 1

		comp				if data binning has been requested, here the data component to bin
							is specified. Default 0. If veclen > 1 only the first element
							of the vector is used.


	Outputs:

		out_fld             Uniform Field containing the coordinate and data bin counts.

		                    If data binning is not being performed the output contains a set
		                    of scalar node data that states now many points fall within each
		                    coordinate bin.

		                    If data binning is being performed the output contains a set of
		                    scalar node data for each data bin.  Each node data component
		                    states for each coordinate bin how many points fall within that
		                    data bin.



	Author:	Mario Valle - AVS Italy
	Date:	14-feb-2001

    Modification: MA YingLiang - Manchester Visualization Centre
	Date :  26-Feb-2001

    Modification: Andrew Dodd - International AVS Centre
	Date :  29-March-2001
*/


#include <math.h>
#include <float.h>

#include "gen.h"

#ifdef WIN32
#define hypot _hypot
#endif


/* coordinate_system enum values */

#define	CARTESIAN_COORD_TYPE	0
#define	POLAR_COORD_TYPE		1
#define	NORM_POLAR_COORD_TYPE	2

int
Binner_BinnerMods_BinnerCore::update(OMevent_mask event_mask, int seq_num)
{
	int nbins_size;
	int *nbins_arr;
	
	int bins_size;
	float *bins_arr;
	
	int i, j, idx, out_nspace;
	float *coords, *min_c, *max_c, min_d, max_d;

	int fld_nspace, fld_nnodes, coord_sys, l_ndata_bins;

	/*****************************************/
	/*	Field 1D, 2D and 3D                  */
	/*	For polar binning only 2D is valid   */
	/*****************************************/

	fld_nspace = fld.nspace;
	fld_nnodes = fld.nnodes;
	coord_sys  = coordinate_system;

	if (fld_nspace < 1 || fld_nspace > 3)
		return 0;
	if ((coord_sys != CARTESIAN_COORD_TYPE) && (fld_nspace != 2))
		return 0;

	
	/* Setup number of bins on coordinates */
	
	int nb[3];
	nbins_arr = (int *)ncoord_bins.ret_array_ptr(OM_GET_ARRAY_RD, &nbins_size);
	if (nbins_arr)
	{
		if (nbins_size >= fld_nspace)
		{
			for(i=0; i < fld_nspace; i++) {
				if (nbins_arr[i]>0)
					nb[i] = nbins_arr[i];
				else
					nb[i] = 1;
			}
		}
		else
		{
			for(i=0; i < nbins_size; i++) {
				if (nbins_arr[i]>0)
					nb[i] = nbins_arr[i];
				else
					nb[i] = 1;
			}
			for(   ; i < fld_nspace; i++)
				nb[i] = 1;
		}
		ARRfree(nbins_arr);
	}
	else {
	    return 0;
		//for(i=0; i < fld_nspace; i++)
		//	nb[i] = 10;	/* sane default, but must never happen*/
	}
	
	
    /* Check that ndata_bins is a valid object and copy it to a local variable if it is */
    if (ndata_bins.valid_obj()) {
        l_ndata_bins = ndata_bins;
    } else {
        l_ndata_bins = 1;
    }

	/* If no node data available disable binning along the data dimension*/
	if (fld.nnode_data < 1)
		l_ndata_bins = 1;

	/* Work out output dimensions */
	if (l_ndata_bins > 1) {
		bins_size = l_ndata_bins;
	}
	else {
	    l_ndata_bins = 1;
		bins_size = 1;
	}

    out_nspace = 0;
	for (i=0; i<fld_nspace; i++) {
	    if (nb[i] > 1) {
    		bins_size *= nb[i];
    		out_nspace++;
	    }
	}

    /* AVS/Express does not support no dimension fields so return now if out_nspace < 1. */
    if (out_nspace < 1) {
        ERRverror("BinnerCore", ERR_ERROR, "Binner does not support output of fields with unitary dimensions.\n");
        return 0;
    }

	
	/************************************/
	/*	Find coordinates min/max        */
	/************************************/

	float *fld_coordinates = (float *)fld.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
	if (!fld_coordinates) {
        //ERRverror("BinnerCore", ERR_ERROR, "Could not get input coordinate array\n");
		return 0;
	}

	if (coord_sys != CARTESIAN_COORD_TYPE)
	{
		/*	convert the coordinates to polar */
		coords = new float[fld_nnodes*2];
		min_c  = new float[2];
		max_c  = new float[2];

		min_c[0] = FLT_MAX;
		min_c[1] = -(float)M_PI;
		max_c[0] = 0.0F;
		max_c[1] = (float)M_PI;
		for(i=0; i < fld_nnodes; i++)
		{
			float rho, r;
			float x = fld_coordinates[2*i+0];
			float y = fld_coordinates[2*i+1];
			if(x == 0.0F)
			{
				rho = (y >= 0.0F) ? (float)M_PI/2.0F : -(float)M_PI/2.0F;
			}
			else
			{
				rho = (float)atan2(y, x);
			}	
			r = (float)hypot(x, y);
			if(r > max_c[0])      max_c[0] = r;
			else if(r < min_c[0]) min_c[0] = r;
			
			coords[2*i+0] = (float)r;
			coords[2*i+1] = (float)rho;
		}
	}
	else
	{
		coords = fld_coordinates;
		
		min_c = (float *)fld.coordinates.min_vec.ret_array_ptr(OM_GET_ARRAY_RD);
		if (!min_c) {
            ERRverror("BinnerCore", ERR_ERROR, "Could not get input minimum coordinates array\n");
			ARRfree(fld_coordinates);
			return 0;
		}
		max_c = (float *)fld.coordinates.max_vec.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!max_c) {
            ERRverror("BinnerCore", ERR_ERROR, "Could not get input maximum coordinates array\n");
			ARRfree(min_c);
			ARRfree(fld_coordinates);
			return 0;
		}
	}


	/****************************************/
	/*	Allocate output array and zero it   */
	/****************************************/

	bins_arr = new float[bins_size];
	memset(bins_arr, 0, bins_size*sizeof(float));




	/***********************************/
	/*       Do the binning            */
	/***********************************/

	if (l_ndata_bins > 1)
	{
		/****************************************/
		/*	Sanity check on the component value */
		/****************************************/
		int data_comp;

        /* Check that comp is a valid object and copy it to a local variable if it is */
        if (comp.valid_obj()) {
            data_comp = comp;
        } else {
            data_comp = 0;
        }

		if ((data_comp < 0) || (data_comp >= fld.nnode_data)) {
			data_comp = 0;
		}


		/*******************************/
		/*	Find data min/max          */
		/*******************************/

		int veclen = fld.node_data[data_comp].veclen;

		int vec_size;
		float *vec = (float *)fld.node_data[data_comp].min_vec.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_FLOAT, &vec_size);
		if (!vec) {
			min_d = -FLT_MAX;
		}
		else {
			min_d = vec[0];
			ARRfree(vec);
		}

		vec = (float *)fld.node_data[data_comp].max_vec.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_FLOAT, &vec_size);
		if (!vec) {
			max_d = FLT_MAX;
		}
		else {
			max_d = vec[0];
			ARRfree(vec);
		}

		int fld_node_data_size;
		float *fld_node_data = (float *)fld.node_data[data_comp].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_FLOAT, &fld_node_data_size);
		if (!fld_node_data) {
            ERRverror("BinnerCore", ERR_ERROR, "Could not get input node data array\n");
		    if (coord_sys != CARTESIAN_COORD_TYPE) {
        		delete [] coords;
        		delete [] min_c;
        		delete [] max_c;
		    }
		    else {
    			ARRfree(min_c);
    			ARRfree(max_c);
		    }
			ARRfree(fld_coordinates);
        	delete [] bins_arr;
			return 0;
		}


		for(i=0; i<fld_nnodes; i++)
		{
			idx = 0;
			for(j = fld_nspace-1; j >= 0; j--) {
				idx = idx * nb[j] + (int)((((nb[j]-1)*(coords[fld_nspace*i+j]-min_c[j]))/(max_c[j]-min_c[j]))+0.5F);
			}
			idx = idx * l_ndata_bins + (int)((((l_ndata_bins-1)*(fld_node_data[veclen*i+0]-min_d))/(max_d-min_d))+0.5F);

			if (coord_sys != NORM_POLAR_COORD_TYPE) {
				bins_arr[idx]++;
			}
			else {
				int n = (int)((((nb[0]-1)*(coords[fld_nspace*i]-min_c[0]))/(max_c[0]-min_c[0]))+0.5F);
				bins_arr[idx] += (2.0F*nb[0]-1.0F)/(2.0F*n+1.0F);
			}
		}

		ARRfree(fld_node_data);
	}
	else
	{
		for(i=0; i<fld_nnodes; i++)
		{
			idx = 0;
			for(j = fld_nspace-1; j >= 0; j--) {
				idx = idx * nb[j] + (int)((((nb[j]-1)*(coords[fld_nspace*i+j]-min_c[j]))/(max_c[j]-min_c[j]))+0.5F);
			}

			if(coord_sys != NORM_POLAR_COORD_TYPE) {
				bins_arr[idx]++;
			}
			else
			{
				int n = (int)((((nb[0]-1)*(coords[fld_nspace*i]-min_c[0]))/(max_c[0]-min_c[0]))+0.5F);
				bins_arr[idx] += (2.0F*nb[0]-1.0F)/(2.0F*n+1.0F);
			}
		}
	}



	/******************************/
	/* Set Output Uniform Field   */
	/******************************/
	int *out_dims = NULL;
    int out_count, out_nnodes;

	out_fld.nspace = out_nspace;
	out_fld.ndim   = out_nspace;

	out_dims = (int *)out_fld.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	if (!out_dims) {
        ERRverror("BinnerCore", ERR_ERROR, "Could not get output dimensions array\n");
	    if (coord_sys != CARTESIAN_COORD_TYPE) {
    		delete [] coords;
    		delete [] min_c;
    		delete [] max_c;
	    }
	    else {
			ARRfree(min_c);
			ARRfree(max_c);
	    }
		ARRfree(fld_coordinates);
    	delete [] bins_arr;
		return 0;
	}

    out_count = 0;
    out_nnodes = 1;

    for (i=0; (i<fld_nspace) && (out_count<out_nspace); i++) {
        if (nb[i] > 1) {
            out_nnodes *= nb[i];
    		out_dims[out_count++] = nb[i];
	    }
	}

    out_fld.nnodes = out_nnodes;

    ARRfree(out_dims);



	/**************************/
	/* Set Node Data          */
	/**************************/

    /* clear the old node data component */
	out_fld.nnode_data = 0;

	out_fld.nnode_data = l_ndata_bins;

	int m;
	int *data_out = NULL;
	int dataout_size, dataout_type;
	char lbl_str[100];
    float min_range, max_range;

    for (m=0; m<l_ndata_bins; m++)
    {
        if (l_ndata_bins==1) {
            sprintf(lbl_str, "Coordinate bins");
        }
        else {
            min_range = ((float)m / l_ndata_bins) * 100;
            max_range = ((float)(m+1) / l_ndata_bins) * 100;
            sprintf(lbl_str, "%.0f%% to %.0f%% data bin", min_range, max_range);
        }

    	out_fld.node_data[m].veclen = 1;
    	out_fld.node_data[m].labels.set_str_val(lbl_str);
    	out_fld.node_data[m].units.set_str_val("");

    	data_out = (int *)out_fld.node_data[m].values.ret_array_ptr(OM_GET_ARRAY_WR, &dataout_size, &dataout_type);
    	if (!data_out) {
            ERRverror("BinnerCore", ERR_ERROR, "Could not get output node data array\n");
    	    if (coord_sys != CARTESIAN_COORD_TYPE) {
        		delete [] coords;
        		delete [] min_c;
        		delete [] max_c;
    	    }
    	    else {
    			ARRfree(min_c);
    			ARRfree(max_c);
    	    }
    		ARRfree(fld_coordinates);
        	delete [] bins_arr;
    		return 0;
    	}

    	for(i=0; i<out_nnodes; i++) {
   			data_out[i] = (int)bins_arr[i*l_ndata_bins + m];
    	}

        ARRfree(data_out);
    }


	/*******************************/
	/*	Output min/max limits      */
	/*******************************/

	float *points = (float *)out_fld.points.ret_array_ptr(OM_GET_ARRAY_WR);
	if (points) {
	    out_count = 0;

		for (i=0; (i<fld_nspace) && (out_count<out_nspace); i++) {
		    if (nb[i] > 1) {
    			points[out_count]              = min_c[i];
    			points[out_count + out_nspace] = max_c[i];
    			out_count++;
		    }
		}

		ARRfree(points);
	}


	/*******************************/
	/*    	Release all            */
	/*******************************/

	if (coord_sys != CARTESIAN_COORD_TYPE)
	{
		delete [] coords;
		delete [] min_c;
		delete [] max_c;
	}
	else
	{
		ARRfree(min_c);
		ARRfree(max_c);
	}

	ARRfree(fld_coordinates);

	delete [] bins_arr;

	return 1;
}

