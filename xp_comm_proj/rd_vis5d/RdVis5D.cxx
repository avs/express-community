//
//	Vis5D reader for Express
//
//	Author: Mario Valle
//	Copyright (C) 2001 Mario Valle.
//
//	Version 0.2
//

// changes marked with ts23 were made by Tobias Schiebeck, IAC

#include "RdV5Dgen.h"
#include <avs/mat.h>
#include <avs/f_utils.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "RdVis5D.h"
#include "proj.h"

//#define DEBUG

extern char v5d_error_msg[];

//
//	Routines to format date and time for output
//
static char *FormatTime(int time)
{
	static char str[9];
	int h, m, s;

	h = time/10000;
	m = (time/100)%100;
	s = time%100;

	sprintf(str, "%02d:%02d:%02d", h, m, s);

	return str;
}

static int months[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static char *FormatDate(int date)
{
	static char str[11];
	int i;
	int a = date/1000;
	if(a < 50)  a += 2000; /* WLH 31 July 96 << 31 Dec 99 */
	else		a += 1900;

    // Check for leap year.
    months[2] = (a % 100 != 0 && a % 4 == 0) ? 29 : 28;

	int d = date % 1000;
	if(d < 1)
	{
		return "**NoDate**";
	}
	else
	{
		for(i=1; i <= 12; i++)
		{
			if(d <= months[i]) break;
			d -= months[i];
		}

		sprintf(str, "%02d/%02d/%04d", d, i, a);
	}
	return str;
}

int
ReadVis5D_ReadVis5DCore::ReadVis5Dfile(OMevent_mask event_mask, int seq_num)
{
	// filename (OMXstr read req notify)
	// time_step (OMXint write)
	// time_steps (OMXstr_array write)
	// fld (Field_Unif write)
	// local_ptr (OMXptr read write)
	int i, j, intr;
	char str[512];

	//
	//	If the user unchecked the "Load all" toggle, do nothing
	//	(the data is already in the cache)
	//
	if(!filename.changed(seq_num) && load_all_steps.changed(seq_num) && (int)load_all_steps == 0) return 0;

	//
	//	Initalize some variables to have a consistent output on error
	//
	fld.nnode_data = 0;
	time_steps.set_array_size(0);
	time_step = 0;
	var_names.set_array_size(0);
	var_info.set_array_size(0);

	fld.ndim   = 3;
	fld.nspace = 3;
	int *fld_dims = (int *)fld.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	if(fld_dims)
	{
		fld_dims[0] = 0;
		fld_dims[1] = 0;
		fld_dims[2] = 0;

		ARRfree(fld_dims);
	}

	vars2d.nnode_data = 0;
	vars2d.ndim    = 2;
	vars2d.nspace  = 2;
	fld_dims = (int *)vars2d.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	if(fld_dims)
	{
		fld_dims[0] = 0;
		fld_dims[1] = 0;

		ARRfree(fld_dims);
	}

	//
	//	Open the Vis5D file (converting $XP_PATH<1> variables)
	//
	char file_buf[AVS_PATH_MAX], *fn;
    fn = FILEmap_variables(filename, file_buf);
	v5dstruct *v5d = v5dOpenFile(fn, NULL);
	if(v5d == NULL) 
	{
		if(v5d_error_msg[0])
		{
			status = v5d_error_msg;
		}
		else
		{
			sprintf(str, "Cannot read file %s", (char *)filename);
			status = str;
		}
		return 1;
	}

	//
	//	Initial sanity check
	//
	if(v5d->VerticalSystem < 0 || v5d->VerticalSystem > VERT_LAST)
	{
		sprintf(str, "Bad VerticalSystem value: %d", v5d->VerticalSystem);
		status = str;
		v5dCloseFile(v5d);
		return 1;
	}
	if(v5d->Projection < 0 || v5d->Projection > PROJ_LAST)
	{
		sprintf(str, "Bad Projection value: %d", v5d->Projection);
		status = str;
		v5dCloseFile(v5d);
		return 1;
	}

	//
	//	Initialize the running status report
	//
	OMstatus_check(0, "Read Vis5D start", &intr);
	if(intr)
	{
		v5dCloseFile(v5d);
		return 0;
	}

	//
	//	If the global structure exists delete it and reassign the pointer
	//
	ReaderStatus *st;
	OMget_ptr_val((OMobj_id)local_ptr, (void **)&st, 0);
	if(st)
	{
		if(st->ucache)
		{
			for(i=0; i < st->v5d->NumVars * st->v5d->NumTimes; i++)	delete [] st->ucache[i];
			delete [] st->ucache;
		}
		if(!st->caching) v5dCloseFile(st->v5d);
		v5dFreeStruct(st->v5d);
		delete [] st->tmp;
		delete [] st->ga;
		delete [] st->gb;
	}
	else
	{
		st = new ReaderStatus;
	}
	st->caching = (int)load_all_steps > 0;
	st->ucache  = 0;
	st->v5d     = v5d;
	st->tmp     = 0;
	st->ga      = 0;
	st->gb      = 0;
	st->nvars2d = 0;
	st->nvars3d = 0;

	OMset_ptr_val((OMobj_id)local_ptr, (void *)st, 0);

	//
	//	Fill the timestep selector (and preselect the first step)
	//
	time_steps.set_array_size(v5d->NumTimes);
	for(i=0; i < v5d->NumTimes; i++)
	{
		sprintf(str, "%03d:  %s  %s",
				i+1,
				FormatDate(v5d->DateStamp[i]),
				FormatTime(v5d->TimeStamp[i]));

		time_steps.set_str_array_val(i, str);
	}
	time_step = 0;

	//
	//	Fill the variables names list
	//
	var_names.set_array_size(v5d->NumVars);
	for(i=0; i < v5d->NumVars; i++)
	{
		var_names.set_str_array_val(i, v5d->VarName[i]);
	}

	//
	//	Fill the variables infos
	//
	var_info.set_array_size(v5d->NumVars);
	for(i=0; i < v5d->NumVars; i++)
	{
		char u[100];
		if(v5d->Units[i][0] != '\0') sprintf(u, "(%s)", v5d->Units[i]);
		else u[0] = '\0';

		if(v5d->MinVal[i] > v5d->MaxVal[i])
		{

			sprintf(str, "%s %s\nRows: %d\nCols: %d\nLevels: %d\nLowLev: %d\nMinVal: MISSING\nMaxVal: MISSING",
						v5d->VarName[i], u,
						v5d->Nr, v5d->Nc, v5d->Nl[i], v5d->LowLev[i] );
		}
		else
		{
			sprintf(str, "%s %s\nRows: %d\nCols: %d\nLevels: %d\nLowLev: %d\nMinVal: %g\nMaxVal: %g",
						v5d->VarName[i], u,
						v5d->Nr, v5d->Nc, v5d->Nl[i], v5d->LowLev[i], v5d->MinVal[i], v5d->MaxVal[i] );
		}
		var_info.set_str_array_val(i, str);
	}

	//
	//	Find the wind component variables
	//
	int *wc = (int *)wind_components.ret_array_ptr(OM_GET_ARRAY_WR);
	if(wc)
	{
		wc[0] = wc[1] = wc[2] = 0;
		for(i=0; i < v5d->NumVars; i++)
		{
			if(!strcmp(v5d->VarName[i], "U")) {wc[0] = i+1; break;}
		}
		for(i=0; i < v5d->NumVars; i++)
		{
			if(!strcmp(v5d->VarName[i], "V")) {wc[1] = i+1; break;}
		}
		for(i=0; i < v5d->NumVars; i++)
		{
			if(!strcmp(v5d->VarName[i], "W")) {wc[2] = i+1; break;}
		}
		ARRfree(wc);
	}

	//
	//	Compute the number of vertical levels
	//
	int maxnl = 0;
	for(i=0; i < v5d->NumVars; i++)
	{
		if(v5d->Nl[i]+v5d->LowLev[i] > maxnl)
		{
			maxnl = v5d->Nl[i]+v5d->LowLev[i];
		}
	}
	st->totl = maxnl;

	//
	//	Set the fixed field parts
	//
	fld.ndim = (maxnl > 1) ? 3 : 2;
	fld_dims = (int *)fld.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	if(fld_dims)
	{
		fld_dims[0] = v5d->Nc;
		fld_dims[1] = v5d->Nr;
		if(maxnl > 1) fld_dims[2] = maxnl;

		ARRfree(fld_dims);
	}

	fld_dims = (int *)vars2d.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	if(fld_dims)
	{
		fld_dims[0] = v5d->Nc;
		fld_dims[1] = v5d->Nr;

		ARRfree(fld_dims);
	}

	//
	//	Compute the grid and the vertical scale
	//
	float zscale;
	char *p = compute_grid(st, &zscale);
	if(p)
	{
		status = p;
		return 1;
	}

	//
	//	Count the 2D and 3D variables
	//
	for(i = 0; i < v5d->NumVars; i++)
	{ 
		if(v5d->Nl[i] > 1)	st->nvars3d++;
		else				st->nvars2d++;
	}

	//
	//	Set the common data on all the variables
	//
	fld.nnode_data    = st->nvars3d;
	vars2d.nnode_data = st->nvars2d;

	for(i = j = 0; i < v5d->NumVars; i++)
	{
		if(v5d->Nl[i] <= 1) continue;

		fld.node_data[j].veclen     = 1;
		fld.node_data[j].null_flag  = 1;
		fld.node_data[j].null_value = MISSING;
		fld.node_data[j].labels     = v5d->VarName[i];
		fld.node_data[j].units      = v5d->Units[i];

		//
		//	Set the min/max value to the global ones
		//
		if(v5d->MinVal[i] <= v5d->MaxVal[i])
		{
			fld.node_data[j].min = v5d->MinVal[i];
			fld.node_data[j].max = v5d->MaxVal[i];
		}
		else
		{
			OMparse_obj_ref(fld.node_data[j].min.obj_id(OM_OBJ_RW), 
							"cache(min_array(magnitude(values), null_flag, null_value))");
			OMparse_obj_ref(fld.node_data[j].max.obj_id(OM_OBJ_RW),
							"cache(max_array(magnitude(values), null_flag, null_value))");
		}

		j++;
	}

	for(i = j = 0; i < v5d->NumVars; i++)
	{ 
		if(v5d->Nl[i] > 1) continue;

		vars2d.node_data[j].veclen     = 1;
		vars2d.node_data[j].null_flag  = 1;
		vars2d.node_data[j].null_value = MISSING;
		vars2d.node_data[j].labels     = v5d->VarName[i];
		vars2d.node_data[j].units      = v5d->Units[i];

		//
		//	Set the min/max value to the global ones
		//
		if(v5d->MinVal[i] <= v5d->MaxVal[i])
		{
			vars2d.node_data[j].min = v5d->MinVal[i];
			vars2d.node_data[j].max = v5d->MaxVal[i];
		}
		else
		{
			OMparse_obj_ref(vars2d.node_data[j].min.obj_id(OM_OBJ_RW), 
							"cache(min_array(magnitude(values), null_flag, null_value))");
			OMparse_obj_ref(vars2d.node_data[j].max.obj_id(OM_OBJ_RW),
							"cache(max_array(magnitude(values), null_flag, null_value))");
		}

		j++;
	}

	//
	//	Read all timesteps
	//
	if(read_all_steps(st))
	{
		status = "Error loading timesteps";
		return 1;
	}

	//
	//	Close the input file if not needed anymore
	//
	if(st->caching) v5dCloseFile(st->v5d);

	//
	//	Read the first time step
	//
	if(get_one_step(st, 0))
	{
		sprintf(str, "Error reading timestep: %d", 0);
		status = str;
		return 1;
	}

	//
	//	Set the initial z scale factor
	//
	float *xm = (float *)fld.xform.mat.ret_array_ptr(OM_GET_ARRAY_RW);
	if(xm)
	{
		MATmat4_scale((Matr4 *)xm, 1.0F, 1.0F, zscale);

		ARRfree(xm);
	}

	//
	//	Final success message
	//
	char m1[100], m2[100];
	if(v5d->FileFormat == 0)
	{
	   if(v5d->FileVersion[0] == 0)
	   {
		   sprintf(m1, "File format:  v5d  version: (4.0 or 4.1)");
	   }
	   else 
	   {
		   sprintf(m1, "File format:  v5d  version: %s", v5d->FileVersion);
	   }
	}
	else 
	{
	   sprintf(m1, "File format:  comp5d  (Vis5d 3.3 or older)");
	}

	if(v5d->CompressMode == 1) 
	{
	   sprintf(m2, "Compression: 1 byte per gridpoint");
	}
	else
	{
	   sprintf(m2, "Compression: %d bytes per gridpoint", v5d->CompressMode);
	}

	char *m3;
	switch (v5d->VerticalSystem)
	{
	case VERT_GENERIC:
		m3 = "Generic linear vertical coordinate system";
		break;
	case VERT_EQUAL_KM:
		m3 = "Equally spaced levels in km";
		break;
	case VERT_NONEQUAL_KM:
		m3 = "Unequally spaced levels in km";
		break;
	case VERT_NONEQUAL_MB:
		m3 = "Unequally spaced levels in mb";
		break;
	}

	char *m4;
	switch (v5d->Projection)
	{
	case PROJ_GENERIC:
	   m4 = "Generic linear projection";
	   break;
	case PROJ_LINEAR:
	   m4 = "Cylindrical Equidistant projection";
	   break;
	case PROJ_LAMBERT:
	   m4 = "Lambert Conformal projection";
	   break;
	case PROJ_STEREO:
	   m4 = "Stereographic projection";
	   break;
	case PROJ_ROTATED:
	   m4 = "Rotated equidistant projection";
	   break;
	case PROJ_MERCATOR:
	   m4 = "Mercator projection";
	   break;
	}

	sprintf(str, "%s\n%s\n%s\n%s", m1, m2, m3, m4);
	status = str;

	//
	//	Close all
	//
	return 1;
}

int
ReadVis5D_ReadVis5DCore::SelectTimeStep(OMevent_mask event_mask, int seq_num)
{
	// time_step (OMXint read req notify)
	// fld (Field_Unif write)
	// local_ptr (OMXptr read write)

	//
	//	Access the local status and check for correct initialization
	//
	v5dstruct    *v5d;
	ReaderStatus *st;
	OMget_ptr_val((OMobj_id)local_ptr, (void **)&st, 0);
	if(!st) return 0;
	v5d = st->v5d;
	if(!v5d) return 0;

	//
	//	Check for valid timestep
	//
	if(time_step < 0 || time_step >= v5d->NumTimes) return 0;

	//
	//	Format and return one timestep
	//
	if(get_one_step(st, (int)time_step))
	{
		char str[100];
		sprintf(str, "Error reading timestep: %d", (int)time_step);
		status = str;
		return 1;
	}
	return 1;
}

int
ReadVis5D_ReadVis5DCore::OnDeinstance(OMevent_mask event_mask, int seq_num)
{
	// local_ptr (OMXptr read write)
	
	//
	//	Retrieve and destroy the reader status structure
	//
	ReaderStatus *st;

#ifdef DEBUG
	fprintf(stderr,"in OnDeinstance\n",st);fflush(stderr);
#endif

	OMget_ptr_val((OMobj_id)local_ptr, (void **)&st, 0);
	if(!st) return 0;

#ifdef DEBUG
	fprintf(stderr,"st: %lp\n",st);fflush(stderr);
#endif

	if(st->ucache)
	{
#ifdef DEBUG
		fprintf(stderr,"st-ucache: %d\n",st->ucache);fflush(stderr);
#endif
		for(int i=0; i < st->v5d->NumVars * st->v5d->NumTimes; i++)	delete [] st->ucache[i];
		delete [] st->ucache;
#ifdef DEBUG
		fprintf(stderr,"st-ucache: %d\n",st->ucache);fflush(stderr);
#endif
	}

	if(st->v5d)
	{
#ifdef DEBUG
		fprintf(stderr,"st-v5d: %d\n",st->v5d);fflush(stderr);
#endif
		if(!st->caching) v5dCloseFile(st->v5d);
		v5dFreeStruct(st->v5d);
	}
#ifdef DEBUG
		fprintf(stderr,"delete structures\n");fflush(stderr);
#endif

	delete [] st->tmp;
	delete [] st->ga;
	delete [] st->gb;

	delete st;
#ifdef DEBUG
		fprintf(stderr,"structures deleted\n");fflush(stderr);
#endif

	//
	//	To be sure, set the pointer to null
	//
	OMset_ptr_val((OMobj_id)local_ptr, (void *)0, 0);
#ifdef DEBUG
		fprintf(stderr,"pointer changed\n");fflush(stderr);
#endif

	return 1;
}

char* ReadVis5D_ReadVis5DCore::compute_grid(ReaderStatus *s, float *zscale)
{
	int i, j;
	float z0, zlast;
	static char msg[100];
	float x0, incrx;
	float y0, incry;
	bool grid_rectilinear;

	//
	//	Compute the z values
	//
	float *z = 0;

	switch(s->v5d->VerticalSystem) 
	{
	case VERT_GENERIC:
		//Generic linear vertical coordinate system
		//Bottom Bound:				v5d->VertArgs[0]
		//Increment between levels:	v5d->VertArgs[1]
		//for(i = 0, z0 = s->v5d->VertArgs[0]; i < s->totl; i++, z0 += s->v5d->VertArgs[1])
		//{
		//	z[i] = z0;
		//}
		z0    = s->v5d->VertArgs[0];
		zlast = z0 + s->v5d->VertArgs[1] * (s->totl-1);
		grid_rectilinear = false;
		break;

	case VERT_EQUAL_KM:
		//Equally spaced levels in km
		//Bottom Bound:				v5d->VertArgs[0]
		//Increment:				v5d->VertArgs[1]
		//for(i = 0, z0 = s->v5d->VertArgs[0]; i < s->totl; i++, z0 += s->v5d->VertArgs[1])
		//{
		//	z[i] = z0;
		//}
		z0    = s->v5d->VertArgs[0];
		zlast = z0 + s->v5d->VertArgs[1] * (s->totl-1);
		grid_rectilinear = false;
		break;

	case VERT_NONEQUAL_KM:
		//Unequally spaced levels in km
		z = new float[s->totl];
		for(i=0; i < s->totl; i++,j++)
		{
			z[i] = s->v5d->VertArgs[i];
		}
		z0 = z[0];
		zlast = z[s->totl-1];
		grid_rectilinear = true;
		break;

	case VERT_NONEQUAL_MB:
		//Unequally spaced levels in mb
		z = new float[s->totl];
		for(i=0; i < s->totl; i++,j++) 
		{
			z[i] = height_to_pressure(s->v5d->VertArgs[i]);
		}
		z0 = z[0];
		zlast = z[s->totl-1];
		grid_rectilinear = true;
		break;

	default:
		sprintf(msg, "Bad VerticalSystem number: %d", s->v5d->VerticalSystem);
		delete [] z;
		return msg;
	}

	//
	//	Compute limits and increments along x and y axis
	//
	switch (s->v5d->Projection)
	{
	case PROJ_GENERIC:
		// Generic linear projection
		//printf("\tNorth Boundary: %f\n", v->ProjArgs[0] );
		//printf("\tWest Boundary: %f\n", v->ProjArgs[1] );
		//printf("\tRow Increment: %f\n", v->ProjArgs[2] );
		//printf("\tColumn Increment: %f\n", v->ProjArgs[3] );
		x0    =  s->v5d->ProjArgs[1];
		y0    =  s->v5d->ProjArgs[0];
		incrx = -s->v5d->ProjArgs[3];
		incry = -s->v5d->ProjArgs[2];
		break;

	case PROJ_LINEAR:
		//printf("Cylindrical Equidistant projection:\n");
		//printf("\tNorth Boundary: %f degrees\n", v->ProjArgs[0] );
		//printf("\tWest Boundary: %f degrees\n", v->ProjArgs[1] );
		//printf("\tRow Increment: %f degrees\n", v->ProjArgs[2] );
		//printf("\tColumn Increment: %f degrees\n", v->ProjArgs[3] );
		/*
		printf("\tSouth Boundary: %f degrees\n",
		v->NorthBound - v->RowInc * (v->Nr-1) );
		printf("\tEast Boundary: %f degrees\n",
		v->WestBound - v->ColInc * (v->Nc-1) );
		*/
		x0    =  s->v5d->ProjArgs[1];
		y0    =  s->v5d->ProjArgs[0];
		incrx = -s->v5d->ProjArgs[3];
		incry = -s->v5d->ProjArgs[2];
		break;

	case PROJ_LAMBERT:
		//printf("Lambert Conformal projection:\n");
		//printf("\tStandard Latitude 1: %f\n", v->ProjArgs[0] );
		//printf("\tStandard Latitude 2: %f\n", v->ProjArgs[1] );
		//printf("\tNorth/South Pole Row: %f\n", v->ProjArgs[2] );
		//printf("\tNorth/South Pole Column: %f\n", v->ProjArgs[3] );
		//printf("\tCentral Longitude: %f\n", v->ProjArgs[4] );
		//printf("\tColumn Increment: %f km\n", v->ProjArgs[5] );
		x0    =  0.0F;
		y0    =  0.0F;
		incrx = -s->v5d->ProjArgs[5];
		incry = -s->v5d->ProjArgs[5];
		break;

	case PROJ_STEREO:
		//printf("Stereographic:\n");
		//printf("\tCenter Latitude: %f\n", v->ProjArgs[0] );
		//printf("\tCenter Longitude: %f\n", v->ProjArgs[1] );
		//printf("\tCenter Row: %f\n", v->ProjArgs[2] );
		//printf("\tCenter Column: %f\n", v->ProjArgs[3] );
		//printf("\tColumn Spacing: %f\n", v->ProjArgs[4] );
		x0    =  s->v5d->ProjArgs[4]*(s->v5d->Nc/2);
		y0    =  s->v5d->ProjArgs[4]*(s->v5d->Nr/2);
		incrx = -s->v5d->ProjArgs[4];
		incry = -s->v5d->ProjArgs[4];
		break;

	case PROJ_ROTATED:
		//printf("Rotated equidistant projection:\n");
		//printf("\tLatitude of grid(0,0): %f\n", v->ProjArgs[0] );
		//printf("\tLongitude of grid(0,0): %f\n", v->ProjArgs[1] );
		//printf("\tRow Increment: %f degress\n", v->ProjArgs[2] );
		//printf("\tColumn Increment: %f degrees\n", v->ProjArgs[3] );
		//printf("\tCenter Latitude: %f\n", v->ProjArgs[4] );
		//printf("\tCenter Longitude: %f\n", v->ProjArgs[5] );
		//printf("\tRotation: %f degrees\n", v->ProjArgs[6] );
		x0    =  s->v5d->ProjArgs[1];
		y0    = (s->v5d->ProjArgs[0] - s->v5d->ProjArgs[2]*(s->v5d->Nr-1));
		incrx = - s->v5d->ProjArgs[3];
		incry = - s->v5d->ProjArgs[2];
		break;

	case PROJ_MERCATOR:
		//printf("Mercator:\n");
		//printf("\tCenter Latitude: %f\n", v->ProjArgs[0] );
		//printf("\tCenter Longitude: %f\n", v->ProjArgs[1] );
		//printf("\tRow Increment: %f Kilometers\n", v->ProjArgs[2] );
		//printf("\tCol Increment: %f Kilometers\n", v->ProjArgs[3] );
		x0    =  0.0F;
		y0    =  0.0F;
		incrx = -s->v5d->ProjArgs[3];
		incry = -s->v5d->ProjArgs[2];
		break;

	default:
		sprintf(msg, "Bad projection number: %d", s->v5d->Projection);
		return msg;
	}

	//
	//	Setup the grid to be uniform or rectilinear
	//
	if(grid_rectilinear)
	{
		fld.npoints = s->v5d->Nc + s->v5d->Nr + s->totl;
		fld.grid_type = 2;
		OMparse_obj_ref(fld.coordinates.values.obj_id(OM_OBJ_RW), 
						"get_coords_rect(ndim,dims,nspace,points)");
	}
	else
	{
		fld.npoints = 2;
		fld.grid_type = 1;
		OMparse_obj_ref(fld.coordinates.values.obj_id(OM_OBJ_RW), 
						"get_coords_unif(ndim,dims,nspace,points)");
	}

	//
	//	Compute the points for the rectilinear field
	//
	float *fld_points = (float *)fld.points.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!fld_points)
	{
		return "Cannot access: coordinates.points";
	}
	if(grid_rectilinear)
	{
		for(j=i=0; i < s->v5d->Nc; i++,j++)
		{
			fld_points[j*3+0] = (float)i; //x0+i*incrx;
			fld_points[j*3+1] = (float)(s->v5d->Nr - 1 - 0); //y0;
			fld_points[j*3+2] = z[0];
		}
		for(i=0; i < s->v5d->Nr; i++,j++)  // ts23 - swap y coordinates
		{
			fld_points[j*3+0] = 0.0F; //x0;
// ts23 - swap y coordinates - start			
//			fld_points[j*3+1] = (float)(s->v5d->Nr - 1 - i); //y0+i*incry;
			fld_points[j*3+1] = (float)(i); //y0+i*incry;
// ts23 - swap y coordinates - end			
			fld_points[j*3+2] = z[0];
		}
		for(i=0; i < s->totl; i++,j++)
		{
			fld_points[j*3+0] = 0.0F; //x0;
			fld_points[j*3+1] = (float)(s->v5d->Nr - 1 - 0); //y0;
			fld_points[j*3+2] = z[i];
		}
	}
	else
	{
		fld_points[0] = 0.0F; //x0;
// ts23 - swap y coordinates - start			
//		fld_points[1] = (float)(s->v5d->Nr - 1 - 0); //y0+i*incry;
		fld_points[1] = 0.0F; //y0+i*incry;
// ts23 - swap y coordinates - end			
		fld_points[2] = z0;

		fld_points[3] = (float)(s->v5d->Nc - 1); //x0;
// ts23 - swap y coordinates - start			
//		fld_points[4] = 0.0F; //y0+i*incry;
		fld_points[4] = (float)(s->v5d->Nr - 1 - 0); //y0+i*incry;
// ts23 - swap y coordinates - end			
		fld_points[5] = zlast;
	}
	ARRfree(fld_points);

	fld_points = (float *)vars2d.points.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!fld_points)
	{
		return "Cannot access: coordinates.points for vars2d";
	}
	fld_points[0] = 0.0F; //x0;
// ts23 - swap y coordinates - start			
//	fld_points[1] = (float)(s->v5d->Nr - 1 - 0); //y0+i*incry;
	fld_points[1] = 0.0F; //y0+i*incry;
// ts23 - swap y coordinates - end			

	fld_points[2] = (float)(s->v5d->Nc - 1); //x0;
// ts23 - swap y coordinates - start			
//	fld_points[3] = 0.0F; //y0+i*incry;
	fld_points[3] = (float)(s->v5d->Nr - 1 - 0); //y0+i*incry;
// ts23 - swap y coordinates - end			
	ARRfree(fld_points);

	//
	//	Compute the vertical scale
	//
	if(s->totl == 1) *zscale = 1.0;
	else
	{
		double dx, dy, dz;

		dx = s->v5d->Nc; //fabs(incrx * s->v5d->Nc);
		dy = s->v5d->Nr; //fabs(incry * s->v5d->Nr);
		dz = zlast-z0;

		*zscale = (float)(((dx+dy)/2.0)/dz);
	}

	//
	//	Release all
	//
	delete [] z;
	return NULL;
}

int ReadVis5D_ReadVis5DCore::read_all_steps(ReaderStatus *s)
{
	int intr, time, var;

	//
	//	Initalize the read exchange area and the two decompression data areas
	//
	s->tmp = new float[s->v5d->Nc*s->v5d->Nr*s->totl];
	s->ga  = new float[s->v5d->NumVars*s->totl];
	s->gb  = new float[s->v5d->NumVars*s->totl];

	//
	//	Don't preread anything if not requested to cache
	//
	if(!s->caching) return 0;

	//
	//	Update the status
	//
	OMstatus_check(0, "Read Vis5D caching", &intr);
	if(intr)
	{
		return 1;
	}

	//
	//	Initialize the cache
	//
	s->ucache = new unsigned char*[s->v5d->NumVars * s->v5d->NumTimes];
	int size;
	switch(s->v5d->CompressMode)
	{
	case 1:  size = sizeof(unsigned char);  break;
	case 2:  size = sizeof(unsigned short); break;
	case 4:  size = sizeof(float);          break;
	default: size = sizeof(float);          break;
	}
	for(time=0; time < s->v5d->NumTimes; time++)
	{
		for(var = 0; var < s->v5d->NumVars; var++) 
		{
			s->ucache[time * s->v5d->NumVars + var] = new unsigned char[s->v5d->Nc*s->v5d->Nr*s->v5d->Nl[var]*size];
			if(!s->ucache[time * s->v5d->NumVars + var])
			{
				s->caching = false;
				return 0;
			}
		}
	}

	//
	//	Read all the steps
	//
	for(time=0; time < s->v5d->NumTimes; time++)
	{
		for(var = 0; var < s->v5d->NumVars; var++) 
		{
			if(!v5dReadCompressedGrid(s->v5d, time, var, &(s->ga[var*s->totl]), &(s->gb[var*s->totl]), (void*)(s->ucache[time * s->v5d->NumVars + var])))
			{
				return 1;
			}

			//
			//	Update the status bar
			//
			OMstatus_check((((time+1)*s->v5d->NumVars+var+1)*100)/(s->v5d->NumTimes*s->v5d->NumVars), "Read Vis5D caching", &intr);
			if(intr)
			{
				return 1;
			}
		}
	}

	return 0;
}

int ReadVis5D_ReadVis5DCore::get_one_step(ReaderStatus *s, int time)
{
	int i, j, k, kk, var, var2d, var3d;
	float *fld_node_data;

	//
	//	If caching read from the cache
	//
	if(s->caching)
	{
		for(var = var2d = var3d = 0; var < s->v5d->NumVars; var++) 
		{
			v5dDecompressGrid(s->v5d->Nr, s->v5d->Nc, s->v5d->Nl[var], s->v5d->CompressMode,
							 (void *)(s->ucache[time * s->v5d->NumVars + var]),
							 &(s->ga[var*s->totl]), &(s->gb[var*s->totl]), s->tmp);

			//
			//	If a 2D var
			//
			if(s->v5d->Nl[var] <= 1)
			{
		   		fld_node_data = (float *)vars2d.node_data[var2d].values.ret_array_ptr(OM_GET_ARRAY_WR);
				if(fld_node_data)
				{
					k = s->v5d->LowLev[var];
					for(j=0; j < s->v5d->Nr; j++)
					{
						for(i=0; i < s->v5d->Nc; i++)
						{
// ts23 - swap y coordinates - start
//							fld_node_data[((k*s->v5d->Nr) + j)*(s->v5d->Nc) + i] = s->tmp[i*(s->v5d->Nr) + j];
							fld_node_data[((k*s->v5d->Nr) + (s->v5d->Nr - 1 - j))*(s->v5d->Nc) + i] = s->tmp[i*(s->v5d->Nr) + j];
// ts23 - swap y coordinates - end
						}
					}
				}
				ARRfree(fld_node_data);
				var2d++;
				continue;
			}

			//
			//	Else it is a 3D var
			//
		   	fld_node_data = (float *)fld.node_data[var3d].values.ret_array_ptr(OM_GET_ARRAY_WR);
			if(fld_node_data)
			{
				// fill the levels
				for(k=s->v5d->LowLev[var],kk=0; k < s->v5d->LowLev[var]+s->v5d->Nl[var]; k++,kk++)
				{
					for(j=0; j < s->v5d->Nr; j++)
					{
						for(i=0; i < s->v5d->Nc; i++)
						{
// ts23 - swap y coordinates - start
//							fld_node_data[((k*s->v5d->Nr) + j)*(s->v5d->Nc) + i] = s->tmp[i*(s->v5d->Nr) + j];
							fld_node_data[((k*s->v5d->Nr) + (s->v5d->Nr - 1 - j))*(s->v5d->Nc) + i] = s->tmp[((kk*s->v5d->Nc) + i)*(s->v5d->Nr) + j];
// ts23 - swap y coordinates - end
						}
					}
				}

				// fill the levels below the first one
				for(k=0; k < s->v5d->LowLev[var]; k++)
				{
					for(j=0; j < s->v5d->Nr; j++)
					{
						for(i=0; i < s->v5d->Nc; i++)
						{
							fld_node_data[((k*s->v5d->Nr) + j)*(s->v5d->Nc) + i] = (float)MISSING;
						}
					}
				}

				// fill the levels above the last one
				for(k=s->v5d->LowLev[var]+s->v5d->Nl[var]; k < s->totl; k++)
				{
					for(j=0; j < s->v5d->Nr; j++)
					{
						for(i=0; i < s->v5d->Nc; i++)
						{
							fld_node_data[((k*s->v5d->Nr) + j)*(s->v5d->Nc) + i] = (float)MISSING;
						}
					}
				}
				
				ARRfree(fld_node_data);
				var3d++;
			}
		}
	}
	else
	{
		//
		//	Fill the requested timestep
		//
		for(var = var2d = var3d = 0; var < s->v5d->NumVars; var++) 
		{ 
			//
			//	If a 2D var
			//
			if(s->v5d->Nl[var] <= 1)
			{
		   		fld_node_data = (float *)vars2d.node_data[var2d].values.ret_array_ptr(OM_GET_ARRAY_WR);
				if(fld_node_data)
				{
					if(!v5dReadGrid(s->v5d, time, var, s->tmp))
					{
						ARRfree(fld_node_data);
						return 1;
					}
					k = s->v5d->LowLev[var];
					for(j=0; j < s->v5d->Nr; j++)
					{
						for(i=0; i < s->v5d->Nc; i++)
						{
// ts23 - swap y coordinates - start
//							fld_node_data[((k*s->v5d->Nr) + j)*(s->v5d->Nc) + i] = s->tmp[i*(s->v5d->Nr) + j];
							fld_node_data[((k*s->v5d->Nr) + (s->v5d->Nr - 1 - j))*(s->v5d->Nc) + i] = s->tmp[i*(s->v5d->Nr) + j];
// ts23 - swap y coordinates - end
						}
					}
				}
				ARRfree(fld_node_data);
				var2d++;
				continue;
			}

			fld_node_data = (float *)fld.node_data[var3d].values.ret_array_ptr(OM_GET_ARRAY_WR);
			if(fld_node_data)
			{
				if(!v5dReadGrid(s->v5d, time, var, s->tmp))
				{
					ARRfree(fld_node_data);
					return 1;
				}

				// fill the levels
				for(k=s->v5d->LowLev[var],kk=0; k < s->v5d->LowLev[var]+s->v5d->Nl[var]; k++,kk++)
				{
					for(j=0; j < s->v5d->Nr; j++)
					{
						for(i=0; i < s->v5d->Nc; i++)
						{
// ts23 - swap y coordinates - start
//							fld_node_data[((k*s->v5d->Nr) + j)*(s->v5d->Nc) + i] = s->tmp[((kk*s->v5d->Nc) + i)*(s->v5d->Nr) + j];
							fld_node_data[((k*s->v5d->Nr) + (s->v5d->Nr - 1 - j))*(s->v5d->Nc) + i] = s->tmp[((kk*s->v5d->Nc) + i)*(s->v5d->Nr) + j];
// ts23 - swap y coordinates - end
						}
					}
				}

				// fill the levels below the first one
				for(k=0; k < s->v5d->LowLev[var]; k++)
				{
					for(j=0; j < s->v5d->Nr; j++)
					{
						for(i=0; i < s->v5d->Nc; i++)
						{
							fld_node_data[((k*s->v5d->Nr) + j)*(s->v5d->Nc) + i] = (float)MISSING;
						}
					}
				}

				// fill the levels above the last one
				for(k=s->v5d->LowLev[var]+s->v5d->Nl[var]; k < s->totl; k++)
				{
					for(j=0; j < s->v5d->Nr; j++)
					{
						for(i=0; i < s->v5d->Nc; i++)
						{
							fld_node_data[((k*s->v5d->Nr) + j)*(s->v5d->Nc) + i] = (float)MISSING;
						}
					}
				}
				
				ARRfree(fld_node_data);
				var3d++;
			}
		}
	}

	//
	//	Compute the wind component
	//
	int *wc = (int *)wind_components.ret_array_ptr(OM_GET_ARRAY_RD);
	if(wc)
	{
		if(wc[0] > 0 || wc[1] > 0 || wc[2] > 0)
		{
			//
			//	Remove components that are 2d
			//
			int vx, vy, vz;
			for(vx=i=0; i < wc[0]; i++)
			{
				if(s->v5d->Nl[i] > 1) vx++;
			}
			for(vy=i=0; i < wc[1]; i++)
			{
				if(s->v5d->Nl[i] > 1) vy++;
			}
			for(vz=i=0; i < wc[2]; i++)
			{
				if(s->v5d->Nl[i] > 1) vz++;
			}

			if(fld.node_data[fld.nnode_data-1].veclen != 3)
			{
				// If not already present, create it
				fld.nnode_data += 1;

				// Get the units
				char *u = "";
				if(     vx > 0 && s->v5d->Units[vx-1][0]) u = s->v5d->Units[vx-1];
				else if(vy > 0 && s->v5d->Units[vy-1][0]) u = s->v5d->Units[vy-1];
				else if(vz > 0 && s->v5d->Units[vz-1][0]) u = s->v5d->Units[vz-1];

				fld.node_data[fld.nnode_data-1].veclen     = 3;
				fld.node_data[fld.nnode_data-1].null_flag  = 1;
				fld.node_data[fld.nnode_data-1].null_value = MISSING;
				fld.node_data[fld.nnode_data-1].labels     = "COMPWIND";
				fld.node_data[fld.nnode_data-1].units      = u;
			}

			float *fld_node_data = (float *)fld.node_data[fld.nnode_data-1].values.ret_array_ptr(OM_GET_ARRAY_WR);
			if(fld_node_data)
			{
				float *c1 = 0;
				float *c2 = 0;
				float *c3 = 0;
				float a, b, c;
				float maxa = -FLT_MAX;
				float mina = FLT_MAX;
				float maxb = -FLT_MAX;
				float minb = FLT_MAX;
				float maxc = -FLT_MAX;
				float minc = FLT_MAX;
				float maxv = 0.0F;
				float minv = FLT_MAX;

				if(vx) c1 = (float *)fld.node_data[vx - 1].values.ret_array_ptr(OM_GET_ARRAY_RD);
				if(vy) c2 = (float *)fld.node_data[vy - 1].values.ret_array_ptr(OM_GET_ARRAY_RD);
				if(vz) c3 = (float *)fld.node_data[vz - 1].values.ret_array_ptr(OM_GET_ARRAY_RD);

				for(i=0; i < s->v5d->Nc*s->v5d->Nr*s->totl; i++)
				{
					fld_node_data[3*i+0] = a = (c1) ? c1[i] : 0.0F;
					fld_node_data[3*i+1] = b = (c2) ? c2[i] : 0.0F;
					fld_node_data[3*i+2] = c = (c3) ? c3[i] : 0.0F;

					//
					//	Compute max and min values (only for non null values)
					//
					if(IS_MISSING(a) || IS_MISSING(b) || IS_MISSING(c)) continue;
					if(a > maxa)      maxa = a;
					else if(a < mina) mina = a;
					if(b > maxb)      maxb = b;
					else if(b < minb) minb = b;
					if(c > maxc)      maxc = c;
					else if(c < minc) minc = c;
					float mod = a*a+b*b+c*c;
					if(mod > maxv)      maxv = mod;
					else if(mod < minv) minv = mod;
				}

				if(c1) ARRfree(c1);
				if(c2) ARRfree(c2);
				if(c3) ARRfree(c3);

				ARRfree(fld_node_data);

				//
				//	Set min/max values on the vector
				//
				if(minv == FLT_MAX)
				{
					minv = maxv = mina = maxa = minb = maxb = minc = maxc = 0.0F;
				}
				fld.node_data[fld.nnode_data-1].min = (float)sqrt(minv);
				fld.node_data[fld.nnode_data-1].max = (float)sqrt(maxv);
				c1 = (float *)fld.node_data[fld.nnode_data-1].min_vec.ret_array_ptr(OM_GET_ARRAY_WR);
				if(c1)
				{
					c1[0] = mina;
					c1[1] = minb;
					c1[2] = minc;
					ARRfree(c1);
				}
				c1 = (float *)fld.node_data[fld.nnode_data-1].max_vec.ret_array_ptr(OM_GET_ARRAY_WR);
				if(c1)
				{
					c1[0] = maxa;
					c1[1] = maxb;
					c1[2] = maxc;
					ARRfree(c1);
				}
			}
		}
		else
		{
			if(fld.node_data[fld.nnode_data-1].veclen == 3) fld.nnode_data -= 1;
		}
		ARRfree(wc);
	}

	return 0;
}

