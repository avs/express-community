#include <stdio.h>
#include <string.h>

#include "gen.h"
#include "tridefs.h"

#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

extern "C" {
#include "triangle.h"
}

// tri.cxx
//	Author: D.Knight AVS(UK)Ltd Oct 97

// Entry point for triangle routine. Convert express structs
// into triangle structs and do the triangulation.
// NB:
//	1.	All the input 'numberof' scalars are set through the array size
//		rather than directly, as inconsistencies lead to crashes. The only
//		reason to dimension the arrays a priori is to make setting the output
//		structs easier.
//	2.	Params *must* contain (at least) 'z'

int
Triangle_TriangleMods_triangle::update(OMevent_mask event_mask, int seq_num)
{
	// params (OMXstr read req notify)
	// in (struct_triangulateio read req notify)
	// out (struct_triangulateio write)
	// vorout (struct_triangulateio write)

	struct triangulateio In, Out, Vorout;

	char *paramstring = (char*)params;

	// indices must be zero based
	if (strchr(paramstring, 'z') == NULL) {
		printf("Error! params must contain 'z'\n");
		return(0);
	}

	int npoints,npa,nhl,nreg,nseg,ned,ntri,nta;

	// set input triangle structs
	// In.numberofpointattributes = (int)in.numberofpointattributes;
	In.pointlist = (float*)in.pointlist.ret_array_ptr(OM_GET_ARRAY_RD,&npoints);
	In.numberofpoints = npoints/2;
	if (In.numberofpoints < 3) {
		printf("Error! cannot triangulate with less than three points\n");
		if (In.pointlist) ARRfree((char*)In.pointlist);
		return(0);
	}


	In.pointmarkerlist  = (int*)in.pointmarkerlist.ret_array_ptr(OM_GET_ARRAY_RD);
	In.pointattributelist = (float*)in.pointattributelist.ret_array_ptr(OM_GET_ARRAY_RD,&npa);
	In.numberofpointattributes = npa/npoints;

	// In.numberoftriangles = (int)in.numberoftriangles;
	// In.numberoftriangleattributes = (int)in.numberoftriangleattributes;
	In.numberofcorners = 3;
	In.trianglelist = (int*)in.trianglelist.ret_array_ptr(OM_GET_ARRAY_RD,&ntri);
	In.numberoftriangles = ntri/3;
	In.trianglearealist = (float*)in.trianglearealist.ret_array_ptr(OM_GET_ARRAY_RD);
	In.triangleattributelist = (float*)in.triangleattributelist.ret_array_ptr(OM_GET_ARRAY_RD,&nta);
	if (ntri) In.numberoftriangleattributes = nta/ntri;
	else In.numberoftriangleattributes = 0;
	In.neighborlist = (int*)in.neighborlist.ret_array_ptr(OM_GET_ARRAY_RD);

	//In.numberofsegments = (int)in.numberofsegments;
	In.segmentlist = (int*)in.segmentlist.ret_array_ptr(OM_GET_ARRAY_RD,&nseg);
	In.segmentmarkerlist = (int*)in.segmentmarkerlist.ret_array_ptr(OM_GET_ARRAY_RD);
	In.numberofsegments = nseg/2;

	//In.numberofholes = (int)in.numberofholes;
	In.holelist = (float*)in.holelist.ret_array_ptr(OM_GET_ARRAY_RD,&nhl);
	In.numberofholes = nhl/2;

	//In.numberofregions = (int)in.numberofregions;
	In.regionlist = (float*)in.regionlist.ret_array_ptr(OM_GET_ARRAY_RD,&nreg);
	In.numberofregions = nreg/4;


	//In.numberofedges = (int)in.numberofedges;
	In.edgelist = (int*)in.edgelist.ret_array_ptr(OM_GET_ARRAY_RD,&ned);
	In.numberofedges = ned/2;
	In.edgemarkerlist = (int*)in.edgemarkerlist.ret_array_ptr(OM_GET_ARRAY_RD);
	In.normlist = (float*)in.normlist.ret_array_ptr(OM_GET_ARRAY_RD);

	// reset output triangle structs
	Out.pointlist = (float*)NULL;
	Out.pointmarkerlist = (int*)NULL;
	Out.pointattributelist = (float*)NULL;
	Out.trianglelist = (int*)NULL;
	Out.trianglearealist = (float*)NULL;
	Out.triangleattributelist = (float*)NULL;
	Out.neighborlist = (int*)NULL;
	Out.segmentlist = (int*)NULL;
	Out.segmentmarkerlist = (int*)NULL;
	Out.edgelist = (int*)NULL;
	Out.edgemarkerlist = (int*)NULL;
	Out.holelist = (float*)NULL;
	Out.regionlist = (float*)NULL;
	Out.normlist = (float*)NULL;
	Out.numberofholes = 0;
	Out.numberofregions = 0;


	// reset output Voronoi structs
	Vorout.pointlist = (float*)NULL;
	Vorout.pointmarkerlist = (int*)NULL;
	Vorout.pointattributelist = (float*)NULL;
	Vorout.trianglelist = (int*)NULL;
	Vorout.trianglearealist = (float*)NULL;
	Vorout.triangleattributelist = (float*)NULL;
	Vorout.neighborlist = (int*)NULL;
	Vorout.segmentlist = (int*)NULL;
	Vorout.segmentmarkerlist = (int*)NULL;
	Vorout.edgelist  = (int*)NULL;
	Vorout.edgemarkerlist = (int*)NULL;
	Vorout.normlist = (float*)NULL;

	// do the triangulation
	triangulate(paramstring, &In, &Out, &Vorout);

	// set output scalars
	out.numberofpoints = Out.numberofpoints;
	out.numberofpointattributes = Out.numberofpointattributes;
	out.numberoftriangles = Out.numberoftriangles;
	out.numberoftriangleattributes = Out.numberoftriangleattributes;
	out.numberofsegments = Out.numberofsegments;
	out.numberofholes = Out.numberofholes;
	out.numberofregions = Out.numberofregions;
	out.numberofedges  = Out.numberofedges;

	vorout.numberofpoints = Vorout.numberofpoints;
	vorout.numberofpointattributes = Vorout.numberofpointattributes;
	vorout.numberoftriangles = 0; // these are always zero
	vorout.numberoftriangleattributes = 0;
	vorout.numberofsegments = 0;	
	vorout.numberofholes = 0;
	vorout.numberofregions = 0;
	vorout.numberofedges  = Vorout.numberofedges;

	// make sure that memory is cleared properly
	int size;
#define SET_ARRAY(SRC,DEST,TYPE) if (SRC) { \
	TYPE *tmp = (TYPE *)DEST.ret_array_ptr(OM_GET_ARRAY_WR,&size); \
	if (size) memcpy(tmp,SRC,size*sizeof(TYPE)); \
	if (tmp) ARRfree((char*)tmp); free(SRC); }
	
	// set output arrays
	SET_ARRAY(Out.pointlist,out.pointlist,float);
	SET_ARRAY(Out.pointmarkerlist,out.pointmarkerlist,int);
	SET_ARRAY(Out.pointattributelist,out.pointattributelist,float);
	SET_ARRAY(Out.trianglelist,out.trianglelist,int);
	SET_ARRAY(Out.triangleattributelist,out.triangleattributelist,float);
	SET_ARRAY(Out.trianglearealist,out.trianglearealist,float);
	SET_ARRAY(Out.neighborlist,out.neighborlist,int);
	SET_ARRAY(Out.segmentlist,out.segmentlist,int);
	SET_ARRAY(Out.segmentmarkerlist,out.segmentmarkerlist,int);

	SET_ARRAY(Out.edgelist,out.edgelist,int);
	SET_ARRAY(Out.edgemarkerlist,out.edgemarkerlist,int);
	SET_ARRAY(Out.normlist,out.normlist,float);

	// if 'p' is set these are copied from the input
	// structure, otherwise just ignore them
	if (strchr(paramstring, 'p') != NULL) {
	
		if (nhl) out.holelist.set_array(OM_TYPE_FLOAT, In.holelist, nhl, OM_SET_ARRAY_COPY);
		if (Out.holelist) free(Out.holelist);

		if (nreg) out.regionlist.set_array(OM_TYPE_FLOAT, In.regionlist, nhl, OM_SET_ARRAY_COPY);
		if (Out.regionlist) free(Out.regionlist);

	}

	// set outputs for voronoi note that most are them are never used and
	// are therefore ignored
	SET_ARRAY(Vorout.pointlist,vorout.pointlist,float);
	//SET_ARRAY(Vorout.pointmarkerlist,vorout.pointmarkerlist,int);
	//SET_ARRAY(Vorout.pointattributelist,vorout.pointattributelist,float);
	//SET_ARRAY(Vorout.trianglelist,out.trianglelist,int);
	//SET_ARRAY(Vorout.triangleattributelist,out.triangleattributelist,float);
	//SET_ARRAY(Vorout.trianglearealist,out.trianglearealist,float);
	//SET_ARRAY(Vorout.segmentlist,out.segmentlist,int);
	//SET_ARRAY(Vorout.segmentmarkerlist,out.segmentmarkerlist,int);
	//SET_ARRAY(Vorout.holelist,out.holelist,float);
	//SET_ARRAY(Vorout.regionlist,out.regionlist,float);
	SET_ARRAY(Vorout.edgelist,vorout.edgelist,int);
	SET_ARRAY(Vorout.edgemarkerlist,vorout.edgemarkerlist,int);
	SET_ARRAY(Vorout.normlist,vorout.normlist,float);


	// we need to free the input arrays.
	if (In.pointlist) ARRfree((char*)In.pointlist);
	if (In.pointmarkerlist) ARRfree((char*)In.pointmarkerlist);
	if (In.pointattributelist) ARRfree((char*)In.pointattributelist);
	if (In.trianglelist) ARRfree((char*)In.trianglelist);
	if (In.triangleattributelist) ARRfree((char*)In.triangleattributelist);
	if (In.trianglearealist) ARRfree((char*)In.trianglearealist);
	if (In.segmentlist) ARRfree((char*)In.segmentlist);
	if (In.segmentmarkerlist) ARRfree((char*)In.segmentmarkerlist);
	if (In.edgelist) ARRfree((char*)In.edgelist);
	if (In.edgemarkerlist) ARRfree((char*)In.edgemarkerlist);
	if (In.normlist) ARRfree((char*)In.normlist);
	if (In.holelist) ARRfree((char*)In.holelist);
	if (In.regionlist) ARRfree((char*)In.regionlist);

	return(1);
}
// end of file

