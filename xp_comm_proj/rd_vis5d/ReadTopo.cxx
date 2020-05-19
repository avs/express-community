//
//	Vis5D Topography reader for Express
//
//	Author: Mario Valle
//	Copyright (C) 2001 Mario Valle.
//
//	Version 0.1
//
#include "RdTopgen.h"
#include "pcio.h"
#include "v5d.h"
#include "binio.h"
#include "proj.h"
#include <avs/f_utils.h>
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265
#endif

#define DEG2RAD (M_PI/180.0)
#define RAD2DEG (180.0/M_PI)

#define sign(a)	((a) >= 0)

float compute_scale(v5dstruct *v5d)
{
	double dx, dy, dz;
	
	//
	//	Compute the number of vertical levels
	//
	int maxnl = 0;
	for(int i=0; i < v5d->NumVars; i++)
	{
		if(v5d->Nl[i]+v5d->LowLev[i] > maxnl)
		{
			maxnl = v5d->Nl[i]+v5d->LowLev[i];
		}
	}

	//
	//	Compute min/max Z values
	//
	switch(v5d->VerticalSystem) 
	{
	case VERT_EQUAL_KM:
	case VERT_GENERIC:
		dz = v5d->VertArgs[1] * (maxnl-1);
		break;

	case VERT_NONEQUAL_KM:
		dz = v5d->VertArgs[maxnl-1] - v5d->VertArgs[0];
		break;

	case VERT_NONEQUAL_MB:
		dz = height_to_pressure(v5d->VertArgs[maxnl-1]) - height_to_pressure(v5d->VertArgs[0]);
		break;

	default:
		return 1.0F;
	}

	//
	//	Try to have a nice scaling
	//
	dx = v5d->Nc;
	dy = v5d->Nr;

	float zscale = (float)((dx+dy)/(2.0*dz));

	if(zscale > 10.0F) zscale /= 200000.0F;
	else               zscale  = 1.0F;

	return zscale;
}

/* Pete rotated sphere to Earth */
static void pandg_back( float *lat, float *lon, float a, float b, float r)
{
	float pr, gr, pm, gm;
	
	/* NOTE - longitude sign switches - b too! */
	
	pr = DEG2RAD * *lat;
	gr = -DEG2RAD * *lon;
	pm = asin( cos(pr) * cos (gr) );
	gm = atan2(cos(pr) * sin (gr), -sin(pr) );
	
	*lat = RAD2DEG * asin( sin(a) * sin(pm) - cos(a) * cos(pm) * cos (gm - r));
	*lon = -RAD2DEG * (-b + atan2(cos(pm) * sin (gm - r),
		sin(a) * cos(pm) * cos (gm - r) + cos(a) * sin(pm)));
}

void ComputeLatLon(v5dstruct *s, float row, float col, float *lat, float *lon)
{
	float xldif;
	float xedif;
	float radius, tlat, tlon;
	float x0, y0, incrx, incry;
	const float GlobeRadius = 6371.23F;
	
	switch (s->Projection)
	{
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
		x0    =  s->ProjArgs[1];
		y0    =  s->ProjArgs[0];
		incrx = -s->ProjArgs[3];
		incry = -s->ProjArgs[2];
		*lon = x0 + col*incrx;
		*lat = y0 + row*incry;
		break;
		
	case PROJ_LAMBERT:
		//printf("Lambert Conformal projection:\n");
		//printf("\tStandard Latitude 1: %f\n", v->ProjArgs[0] );
		//printf("\tStandard Latitude 2: %f\n", v->ProjArgs[1] );
		//printf("\tNorth/South Pole Row: %f\n", v->ProjArgs[2] );
		//printf("\tNorth/South Pole Column: %f\n", v->ProjArgs[3] );
		//printf("\tCentral Longitude: %f\n", v->ProjArgs[4] );
		//printf("\tColumn Increment: %f km\n", v->ProjArgs[5] );
		
#define Lat1       s->ProjArgs[0]
#define Lat2       s->ProjArgs[1]
#define PoleRow    s->ProjArgs[2]
#define PoleCol    s->ProjArgs[3]
#define CentralLon s->ProjArgs[4]
#define ColInc     s->ProjArgs[5]
		
		float Hemisphere, ConeFactor, Cone, lat1, lat2;
		
		if(Lat1 == Lat2)
		{
            /* polar stereographic? */
            if(Lat1 > 0.0) 
			{
				lat1 = (90.0 - Lat1) * DEG2RAD;
				/* Cone = 1.0; */
            }
            else
			{
				lat1 = (90.0 + Lat1) * DEG2RAD;
				/* Cone = -1.0; */
            }
            Cone = cos(lat1); /* WLH 9-27-96 */
            Hemisphere = 1.0;
		}
		else 
		{
            /* general Lambert conformal */
            float a, b;
            if(sign(Lat1) != sign(Lat2))
			{
				printf("Error: standard latitudes must have the same sign");
				return;
            }
            if (Lat1<Lat2)
			{
				printf("Error: Lat1 must be >= Lat2");
				return;
            }
            Hemisphere = 1.0;
            lat1 = (90.0 - Lat1) * DEG2RAD;
            lat2 = (90.0 - Lat2) * DEG2RAD;
            a = log( sin(lat1) ) - log( sin(lat2) );
            b = log( tan(lat1/2.0) ) - log( tan(lat2/2.0) );
            Cone = a / b;
		}
		
		/* Cone is in [-1,1] */
		ConeFactor = GlobeRadius * sin(lat1) / (ColInc * Cone * pow(tan(lat1/2.0), Cone));
		
		xldif = Hemisphere * (row-PoleRow) / ConeFactor;
		xedif = (PoleCol-col) / ConeFactor;
		
		if(xldif==0.0 && xedif==0.0)
			tlon = CentralLon;
		else
			tlon = atan2(xedif, xldif) / Cone * RAD2DEG + CentralLon;
		
		if(tlon > 180.0) 
			tlon -= 360.0;
		
		radius = sqrt(xldif*xldif + xedif*xedif);
		if(radius < 0.0001)
			tlat = 90.0 * Hemisphere;   /* +/-90 */
		else
			tlat = Hemisphere * (90.0 - 2.0*atan(exp(log(radius)/Cone))*RAD2DEG);
		
		*lat = tlat;
		*lon = tlon;
		
#undef Lat1
#undef Lat2
#undef PoleRow
#undef PoleCol
#undef CentralLon
#undef ColInc
		break;
		
	case PROJ_STEREO:
		//printf("Stereographic:\n");
		//printf("\tCenter Latitude: %f\n", v->ProjArgs[0] );
		//printf("\tCenter Longitude: %f\n", v->ProjArgs[1] );
		//printf("\tCenter Row: %f\n", v->ProjArgs[2] );
		//printf("\tCenter Column: %f\n", v->ProjArgs[3] );
		//printf("\tColumn Spacing: %f\n", v->ProjArgs[4] );
#define CentralLat  s->ProjArgs[0]
#define CentralLon  s->ProjArgs[1]
#define CentralRow  s->ProjArgs[2]
#define CentralCol  s->ProjArgs[3]
#define ColInc      s->ProjArgs[4]
		
		float CosCentralLat;
        float SinCentralLat;
        float Scale;
        float InvScale;
        float xrow, xcol, rho, c, cc, sc;
		
		CosCentralLat = cos( CentralLat * DEG2RAD );
        SinCentralLat = sin( CentralLat * DEG2RAD );
        Scale = (2.0 * GlobeRadius / ColInc);
        InvScale = 1.0 / Scale;
		
		xrow = CentralRow - row;
		xcol = CentralCol - col;
		rho = xrow*xrow + xcol*xcol;
		if(rho<1.0e-20)
		{
			*lat = CentralLat;
			*lon = CentralLon;
		}
		else 
		{
			rho = sqrt( rho );
			c = 2.0 * atan( rho * InvScale);
			cc = cos(c);
			sc = sin(c);
			*lat = RAD2DEG * asin( cc*SinCentralLat + xrow*sc*CosCentralLat / rho );
			*lon = CentralLon + RAD2DEG * atan2( xcol * sc,
				(rho * CosCentralLat * cc - xrow * SinCentralLat * sc) );
			if(*lon < -180.0)     *lon += 360.0;
			else if(*lon > 180.0) *lon -= 360.0;
		}
		
#undef CentralLat
#undef CentralLon
#undef CentralRow
#undef CentralCol
#undef ColInc
		break;
		
	case PROJ_ROTATED:
		/* WLH 4-21-95 */
		//printf("Rotated equidistant projection:\n");
		//printf("\tLatitude of grid(0,0): %f\n", v->ProjArgs[0] );
		//printf("\tLongitude of grid(0,0): %f\n", v->ProjArgs[1] );
		//printf("\tRow Increment: %f degress\n", v->ProjArgs[2] );
		//printf("\tColumn Increment: %f degrees\n", v->ProjArgs[3] );
		//printf("\tCenter Latitude: %f\n", v->ProjArgs[4] );
		//printf("\tCenter Longitude: %f\n", v->ProjArgs[5] );
		//printf("\tRotation: %f degrees\n", v->ProjArgs[6] );
#define NorthBound  s->ProjArgs[0]
#define SouthBound (s->ProjArgs[0] - s->ProjArgs[2]*(s->Nr-1))
#define WestBound   s->ProjArgs[1]
#define EastBound  (s->ProjArgs[1] - s->ProjArgs[3]*(s->Nc-1))
#define CentralLat  s->ProjArgs[4]
#define CentralLon  s->ProjArgs[5]
#define Rotation   (s->ProjArgs[6] * (float)DEG2RAD)
		
		*lat = NorthBound - row * (NorthBound-SouthBound) / (float) (s->Nr-1);
		*lon = WestBound - col * (WestBound-EastBound) / (float) (s->Nc-1);
		pandg_back(lat, lon, CentralLat, CentralLon, Rotation);
		
#undef NorthBound
#undef SouthBound
#undef WestBound
#undef EastBound
#undef CentralLat
#undef CentralLon
#undef Rotation
		break;
		
	case PROJ_MERCATOR:
		//printf("Mercator:\n");
		//printf("\tCenter Latitude: %f\n", v->ProjArgs[0] );
		//printf("\tCenter Longitude: %f\n", v->ProjArgs[1] );
		//printf("\tRow Increment: %f Kilometers\n", v->ProjArgs[2] );
		//printf("\tCol Increment: %f Kilometers\n", v->ProjArgs[3] );
		
#define CentralLat s->ProjArgs[0]
#define CentralLon s->ProjArgs[1]
#define RowIncKm   s->ProjArgs[2]
#define ColIncKm   s->ProjArgs[3]
		
        float YC, ic, jc, alpha;
        YC = GlobeRadius * log((1.0 + sin(DEG2RAD*CentralLat))/cos(DEG2RAD*CentralLat));
        ic = (s->Nr-1)/2.0;
        jc = (s->Nc-1)/2.0;
		
		alpha = ((ic-row) * RowIncKm + YC) / GlobeRadius;
		*lat = 2. * RAD2DEG * atan( exp(alpha) ) - 90.0;
		*lon = CentralLon - RAD2DEG * (col-jc) * ColIncKm / GlobeRadius;
		
#undef CentralLat
#undef CentralLon
#undef RowIncKm
#undef ColIncKm
		break;
		
	default:
		printf("Bad projection number: %d", s->Projection);
		return;
	}
}

static inline float bilinear(float x0, float x1, float y0, float y1,
					   float x, float y,
					   float v00, float v01, float v10, float v11)
{
	float vm0 = v00 + (v01-v00)*(y-y0)/(y1-y0);
	float vm1 = v10 + (v11-v10)*(y-y0)/(y1-y0);
	return vm0 + (vm1-vm0)*(x-x0)/(x1-x0);
}

#define Topo_westlon	params[0]
#define Topo_eastlon	params[1]
#define	Topo_northlat	params[2]
#define Topo_southlat	params[3]
#define Topo_rows		dims[1]
#define Topo_cols		dims[0]

void ComputeRowCol(float *params,
				   int   *dims,
				   float lat, float lon,
				   int *col, int *row)
{
	*col = (int)((lon - Topo_westlon)*(Topo_cols-1)/(Topo_eastlon-Topo_westlon));
	*row = (int)((lat - Topo_northlat)*(Topo_rows-1)/(Topo_southlat-Topo_northlat));
}

void ComputeHeight(short *TopoData,
				   float *params,
				   int   *dims,
				   float lat, float lon,
				   float *height, float *color)
{
	//
	//	Return longitude in the usual range
	//
	if((lon<Topo_eastlon || lon>Topo_westlon) && (Topo_westlon-Topo_eastlon > 360.0F-0.001F))
	{
		if(lon>Topo_westlon) lon -= 360.0F;
		if(lon<Topo_eastlon) lon += 360.0F;
	}

	//
	//	Check validity
	//
	if(!TopoData || lon<Topo_eastlon  || lon>Topo_westlon
		         || lat<Topo_southlat || lat>Topo_northlat)
	{
		*height = 0.0F;
		*color  = 0.0F;
		return;
	}

	//
	//	Find the index that contains the given lat lon values
	//
	int col = (int)((lon - Topo_westlon)*(Topo_cols-1)/(Topo_eastlon-Topo_westlon));
	int row = (int)((lat - Topo_northlat)*(Topo_rows-1)/(Topo_southlat-Topo_northlat));
	if(col == Topo_cols-1) col = Topo_cols-2;
	if(row == Topo_rows-1) row = Topo_rows-2;

	//
	//	Get the four values (height and water flag)
	//
	float hTL = TopoData[(row+1)*Topo_cols+col] / 2.0F;
	float hTR = TopoData[(row+1)*Topo_cols+col+1] / 2.0F;
	float hBL = TopoData[row*Topo_cols+col] / 2.0F;
	float hBR = TopoData[row*Topo_cols+col+1] / 2.0F;
	float wTL = TopoData[(row+1)*Topo_cols+col] % 2;
	float wTR = TopoData[(row+1)*Topo_cols+col+1] % 2;
	float wBL = TopoData[row*Topo_cols+col] % 2;
	float wBR = TopoData[row*Topo_cols+col+1] % 2;

	//
	//	Bilinear interpolation
	//
	float lonL = (Topo_eastlon-Topo_westlon)/(Topo_cols-1)*col+Topo_westlon;
	float lonR = (Topo_eastlon-Topo_westlon)/(Topo_cols-1)*(col+1)+Topo_westlon;
	float latB = (Topo_southlat-Topo_northlat)/(Topo_rows-1)*row+Topo_northlat;
	float latT = (Topo_southlat-Topo_northlat)/(Topo_rows-1)*(row+1)+Topo_northlat;

	*height = bilinear(lonL, lonR, latB, latT, lon, lat, hBL, hTL, hBR, hTR);
	float w = bilinear(lonL, lonR, latB, latT, lon, lat, wBL, wTL, wBR, wTR);
	if(w < 0.5F)
		*color = *height;
	else
		*color = -200.0F;
}

#undef Topo_westlon
#undef Topo_eastlon
#undef Topo_northlat
#undef Topo_southlat
#undef Topo_rows
#undef Topo_cols

#define Topo_westlon	topo_params_arr[0]
#define Topo_eastlon	topo_params_arr[1]
#define	Topo_northlat	topo_params_arr[2]
#define Topo_southlat	topo_params_arr[3]
#define Topo_rows		topo_size_arr[1]
#define Topo_cols		topo_size_arr[0]

int
ReadVis5D_ReadTopoCore::ComputeTopography(OMevent_mask event_mask, int seq_num)
{
	int i, j;
	int *fld_dims;
	float lat, lon, height, color;
	int row,  col;
	int col0, col1;
	int row0, row1;

	//
	//	Check if the topo file has been loaded
	//
	int *topo_size_arr = (int *)topo_size.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!topo_size_arr) return 0;
	if(topo_size_arr[0] <= 0 || topo_size_arr[1] <= 0)
	{
		ARRfree(topo_size_arr);
		return 0;
	}

	//
	//	If yes, load the other parameters
	//
	float *topo_params_arr = (float *)topo_params.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!topo_params_arr) 
	{
		ARRfree(topo_size_arr);
		return 0;
	}
	short *topo_grid_arr = (short *)topo_grid.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!topo_grid_arr)
	{
		ARRfree(topo_size_arr);
		ARRfree(topo_params_arr);
		return 0;
	}

	//
	//	Open the Vis5D file (converting $XP_PATH<1> variables)
	//
	char file_buf[AVS_PATH_MAX], *fn;
    fn = FILEmap_variables(filename, file_buf);
	v5dstruct *v5d = v5dOpenFile(fn, NULL);
	if(v5d == NULL) 
	{
		ARRfree(topo_size_arr);
		ARRfree(topo_grid_arr);
		ARRfree(topo_params_arr);
		printf("Cannot read file %s", (char *)filename);
		return 0;
	}

	//
	//	No topography for generic non geographic coordinates, but only a reference plane
	//
	if(v5d->Projection == PROJ_GENERIC)
	{
		ARRfree(topo_size_arr);
		ARRfree(topo_grid_arr);
		ARRfree(topo_params_arr);

		//
		//	Setup the field
		//
		fld.ndim = 2; 
		fld.nspace = 3;
		fld.nnode_data = 1;
		fld.node_data[0].veclen = 1; 
		fld.node_data[0].labels = "";
		fld.node_data[0].units  = "";
		fld_dims = (int *)fld.dims.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!fld_dims)
		{
			v5dCloseFile(v5d);
			v5dFreeStruct(v5d);
			return 0;
		}
		fld_dims[0] = v5d->Nc;
		fld_dims[1] = v5d->Nr;
		ARRfree(fld_dims);

		float *fld_coordinates = (float *)fld.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!fld_coordinates)
		{
			v5dCloseFile(v5d);
			v5dFreeStruct(v5d);
			return 0;
		}
		float *fld_node_data = (float *)fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!fld_node_data)
		{
			v5dCloseFile(v5d);
			v5dFreeStruct(v5d);
			ARRfree(fld_coordinates);
			return 0;
		}
		
		for(i=0; i < v5d->Nc; i++)
		{
			for(j=0; j < v5d->Nr; j++)
			{
				fld_coordinates[(j*v5d->Nc+i)*3+0] = i;
				fld_coordinates[(j*v5d->Nc+i)*3+1] = v5d->Nr - 1 - j;
				fld_coordinates[(j*v5d->Nc+i)*3+2] = 0.0F;
				fld_node_data[j*v5d->Nc+i]         = 0.0F;
			}
		}

		v5dCloseFile(v5d);
		v5dFreeStruct(v5d);

		return 1;
	}
		
	//
	//	Compute limits and increments along x and y axis
	//
	float x0, y0, incrx, incry;
	int nx, ny;
	switch(v5d->Projection)
	{		
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
		x0    =  v5d->ProjArgs[1];
		y0    =  v5d->ProjArgs[0];
		incrx = -v5d->ProjArgs[3];
		incry = -v5d->ProjArgs[2];

		//
		//	Compute the area of earth topography to be used
		//
		ComputeRowCol(topo_params_arr, topo_size_arr, y0, x0, &col0, &row0);
		ComputeRowCol(topo_params_arr, topo_size_arr, y0+incry*v5d->Nr, x0+incrx*v5d->Nc, &col1, &row1);
		nx = col1 - col0;
		ny = row1 - row0;

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
		incrx = -v5d->ProjArgs[5];
		incry = -v5d->ProjArgs[5];

		ComputeLatLon(v5d, 0, 0, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		col0 = col1 = col;
		row0 = row1 = row;
		ComputeLatLon(v5d, 0, v5d->Nc-1, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;
		ComputeLatLon(v5d, v5d->Nr-1, v5d->Nc-1, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;
		ComputeLatLon(v5d, v5d->Nr-1, 0, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;

		nx = col1 - col0;
		ny = row1 - row0;

		break;
		
	case PROJ_STEREO:
		//printf("Stereographic:\n");
		//printf("\tCenter Latitude: %f\n", v->ProjArgs[0] );
		//printf("\tCenter Longitude: %f\n", v->ProjArgs[1] );
		//printf("\tCenter Row: %f\n", v->ProjArgs[2] );
		//printf("\tCenter Column: %f\n", v->ProjArgs[3] );
		//printf("\tColumn Spacing: %f\n", v->ProjArgs[4] );
		x0    =  v5d->ProjArgs[4]*(v5d->Nc/2);
		y0    =  v5d->ProjArgs[4]*(v5d->Nr/2);
		incrx = -v5d->ProjArgs[4];
		incry = -v5d->ProjArgs[4];


		ComputeLatLon(v5d, 0, 0, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		col0 = col1 = col;
		row0 = row1 = row;
		ComputeLatLon(v5d, 0, v5d->Nc-1, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;
		ComputeLatLon(v5d, v5d->Nr-1, v5d->Nc-1, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;
		ComputeLatLon(v5d, v5d->Nr-1, 0, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;

		nx = col1 - col0;
		ny = row1 - row0;

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
		x0    =   v5d->ProjArgs[1];
		y0    =  (v5d->ProjArgs[0] - v5d->ProjArgs[2]*(v5d->Nr-1));
		incrx = - v5d->ProjArgs[3];
		incry = - v5d->ProjArgs[2];

		ComputeLatLon(v5d, 0, 0, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		col0 = col1 = col;
		row0 = row1 = row;
		ComputeLatLon(v5d, 0, v5d->Nc-1, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;
		ComputeLatLon(v5d, v5d->Nr-1, v5d->Nc-1, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;
		ComputeLatLon(v5d, v5d->Nr-1, 0, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;

		nx = col1 - col0;
		ny = row1 - row0;

		break;
		
	case PROJ_MERCATOR:
		//printf("Mercator:\n");
		//printf("\tCenter Latitude: %f\n", v->ProjArgs[0] );
		//printf("\tCenter Longitude: %f\n", v->ProjArgs[1] );
		//printf("\tRow Increment: %f Kilometers\n", v->ProjArgs[2] );
		//printf("\tCol Increment: %f Kilometers\n", v->ProjArgs[3] );
		x0    =  0.0F;
		y0    =  0.0F;
		incrx = - v5d->ProjArgs[3];
		incry = - v5d->ProjArgs[2];

		ComputeLatLon(v5d, 0, 0, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		col0 = col1 = col;
		row0 = row1 = row;
		ComputeLatLon(v5d, 0, v5d->Nc-1, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;
		ComputeLatLon(v5d, v5d->Nr-1, v5d->Nc-1, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;
		ComputeLatLon(v5d, v5d->Nr-1, 0, &lat, &lon);
		ComputeRowCol(topo_params_arr, topo_size_arr, lat, lon, &col, &row);
		if(col > col1) col1 = col;
		if(col < col0) col0 = col;
		if(row > row1) row1 = row;
		if(row < row0) row0 = row;

		nx = col1 - col0;
		ny = row1 - row0;

		break;
		
	default:
		printf("Bad projection number: %d", v5d->Projection);
		ARRfree(topo_size_arr);
		ARRfree(topo_grid_arr);
		ARRfree(topo_params_arr);
		return 0;
	}

	//
	//	If requested, subsample the topography
	//
	if((int)subsampling > 1)
	{
		nx /= (int)subsampling;
		ny /= (int)subsampling;

		if(nx < 2) nx = 2;
		if(ny < 2) ny = 2;
	}

	//
	//	Compute the vertical scale
	//
	float zscale = compute_scale(v5d);

	//
	//	Prepare the output field
	//
	fld.ndim = 2; 
	fld.nspace = 3;
	fld.nnode_data = 1;
	fld.node_data[0].veclen = 1; 
	fld.node_data[0].labels = "HEIGHT";
	fld.node_data[0].units  = "m";

	fld_dims = (int *)fld.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	if(fld_dims)
	{
		fld_dims[0] = nx;
		fld_dims[1] = ny;
		ARRfree(fld_dims);
	}
	
	//
	//	Compute the point coordinates
	//
	float *fld_coordinates = (float *)fld.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!fld_coordinates)
	{
		ARRfree(topo_size_arr);
		ARRfree(topo_grid_arr);
		ARRfree(topo_params_arr);
		v5dCloseFile(v5d);
		v5dFreeStruct(v5d);
		return 0;
	}
	float *fld_node_data = (float *)fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!fld_node_data)
	{
		ARRfree(topo_size_arr);
		ARRfree(topo_grid_arr);
		ARRfree(topo_params_arr);
		v5dCloseFile(v5d);
		v5dFreeStruct(v5d);
		ARRfree(fld_coordinates);
		return 0;
	}
	
	for(i=0; i < nx; i++)
	{
		for(j=0; j < ny; j++)
		{
			fld_coordinates[(j*nx+i)*3+0] = i * ((v5d->Nc - 1.0F) / (nx - 1.0F)); //x0+i*incrx;
			fld_coordinates[(j*nx+i)*3+1] = (ny - 1 - j) * ((v5d->Nr - 1.0F) / (ny - 1.0F)); //y0+j*incry;
			ComputeLatLon(v5d, j * ((v5d->Nr - 1.0F) / (ny - 1.0F)), i * ((v5d->Nc - 1.0F) / (nx - 1.0F)), &lat, &lon);
			ComputeHeight(topo_grid_arr, topo_params_arr, topo_size_arr, lat, lon, &height, &color);
			if(height < min_height) height = min_height;
			fld_coordinates[(j*nx+i)*3+2] = height/1000.0F*zscale;	// heights are in Km
			fld_node_data[j*nx+i]         = color;
		}
	}
	
	//
	//	Close all
	//
	ARRfree(topo_size_arr);
	ARRfree(topo_grid_arr);
	ARRfree(topo_params_arr);
	ARRfree(fld_node_data);
	ARRfree(fld_coordinates);
   	v5dCloseFile(v5d);
	v5dFreeStruct(v5d);
	return 1;
}

#undef Topo_westlon
#undef Topo_eastlon
#undef Topo_northlat
#undef Topo_southlat
#undef Topo_rows
#undef Topo_cols

int
ReadVis5D_ReadTopoCore::OnInstance(OMevent_mask event_mask, int seq_num)
{
	// topofile (OMXstr read)
	// topo_params (OMXfloat_array write)
	float *topo_params_arr;
	
	// topo_size (OMXint_array write)
	int *topo_size_arr;
	
	// topo_grid (OMXshort_array write)
	short *topo_grid_arr;
	
	//
	//	Initialize the topo dimensions to mean file not loaded
	//
	topo_size_arr = (int *)topo_size.ret_array_ptr(OM_GET_ARRAY_WR);
	if(topo_size_arr)
	{
		topo_size_arr[0] = topo_size_arr[1] = 0;
		ARRfree(topo_size_arr);
	}

	//
	//	Open the Topography file
	//
	char file_buf[AVS_PATH_MAX], *fn;
    fn = FILEmap_variables(topofile, file_buf);
#ifdef WIN32
	int topofd = open(fn, O_RDONLY|_O_BINARY);
#else
	int topofd = open(fn, O_RDONLY);
#endif
	if(topofd == -1)
	{
		printf("Cannot read file %s", (char *)topofile);
		return 0;
	}
		
	//
	//	Read file header
	//
	char id[40];
	float Topo_westlon, Topo_eastlon, Topo_northlat, Topo_southlat;
	int Topo_rows, Topo_cols;
	
	read_bytes(topofd, id, 40);
	if(strcmp(id, "TOPO") == 0)
	{
		/* OLD STYLE: bounds given as ints */
		int k;
		read_int4(topofd, &k);   Topo_westlon  = k / 100.0F;
		read_int4(topofd, &k);   Topo_eastlon  = k / 100.0F;
		read_int4(topofd, &k);   Topo_northlat = k / 100.0F;
		read_int4(topofd, &k);   Topo_southlat = k / 100.0F;
	}
	else if(strcmp(id, "TOPO2") == 0)
	{
		/* NEW STYLE: bounds given as floats */
		read_float4(topofd, &Topo_westlon);
		read_float4(topofd, &Topo_eastlon);
		read_float4(topofd, &Topo_northlat);
		read_float4(topofd, &Topo_southlat);
	}
	else 
	{
		printf("%s is not a TOPO file\n", (char *)topofile);
		close(topofd);
		return 0;
	}
	read_int4(topofd, &Topo_rows);
	read_int4(topofd, &Topo_cols);
	
	topo_params_arr = (float *)topo_params.ret_array_ptr(OM_GET_ARRAY_WR);
	if(topo_params_arr) 
	{
		topo_params_arr[0] = Topo_westlon;
		topo_params_arr[1] = Topo_eastlon;
		topo_params_arr[2] = Topo_northlat;
		topo_params_arr[3] = Topo_southlat;
		ARRfree(topo_params_arr);
	}
	
	topo_size_arr = (int *)topo_size.ret_array_ptr(OM_GET_ARRAY_WR);
	if(topo_size_arr)
	{
		topo_size_arr[1] = Topo_rows;
		topo_size_arr[0] = Topo_cols;
		ARRfree(topo_size_arr);
	}

	//
	//	Read the topo values
	//
	topo_grid_arr = (short *)topo_grid.ret_array_ptr(OM_GET_ARRAY_WR);
	if(topo_grid_arr)
	{
		read_int2_array(topofd, topo_grid_arr, Topo_rows*Topo_cols);

		ARRfree(topo_grid_arr);
	}
	else
	{
		topo_size_arr = (int *)topo_size.ret_array_ptr(OM_GET_ARRAY_WR);
		if(topo_size_arr)
		{
			topo_size_arr[0] = topo_size_arr[1] = 0;
			ARRfree(topo_size_arr);
		}
	}

	//
	//	Close the topo file
	//
	close(topofd);
	return 1;
}
