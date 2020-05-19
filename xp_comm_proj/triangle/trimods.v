

// This library contains the V definitions of the main triangle
// modules
flibrary TriangleMods <build_dir="xp_comm_proj/triangle",
                       out_src_file="gen.cxx",
                       out_hdr_file="gen.h">
{

	// the triangle input/output structure
	// - directly equivalent to struct triangulateio in triangle
	group+Port struct_triangulateio {
		int numberofpoints;
		int+opt numberofpointattributes;
		float pointlist[numberofpoints][2];
		float+opt pointattributelist[numberofpoints*numberofpointattributes];
		int+opt pointmarkerlist[numberofpoints];
			
		int+opt trianglelist[numberofcorners*numberoftriangles];
		float+opt triangleattributelist[numberoftriangleattributes*numberoftriangles];
		float+opt trianglearealist[numberoftriangles];
		int+opt neighborlist[3*numberoftriangles];
		int+opt numberoftriangles;
		int+opt numberofcorners = 3;
		int+opt numberoftriangleattributes;

		int+opt numberofsegments;
		int+opt segmentlist[2*numberofsegments];
		int+opt segmentmarkerlist[numberofsegments];

		int+opt numberofholes;
		float+opt holelist[numberofholes][2];

		int+opt numberofregions;
		float+opt regionlist[4*numberofregions];

		int+opt numberofedges;
		int+opt edgelist[2*numberofedges];
		int+opt edgemarkerlist[numberofedges];
		float+opt normlist[numberofedges][2];
	};


	// the triangle module
	module triangle <src_file="tri.cxx",
                    c_src_files="triangle.c barycent.c">
   {
		string+IPort2 params;

		struct_triangulateio+IPort2 in;

		cxxmethod+req update (
			params+read+req+notify,
			in+read+req+notify,
			out+write,
			vorout+write
		);

		struct_triangulateio+OPort2 out;
		struct_triangulateio vorout;	
	};



	// this module fixes the edges in voronoi edgelists
	module fix_voronoi_edges <src_file="edge.cxx"> {
		int+IPort2 in_edges[];

		cxxmethod+req update (
			in_edges+read+req+notify,
			out_edges+write
		);

		int+OPort2 out_edges[];
	};



	// this module computes triangle area
	module compute_triangle_areas <src_file="area.cxx"> {
		int ntris;
		int+IPort2   connect[];
		float+IPort2 coords[];
		
		float min_area => min_array(areas);
		float max_area => max_array(areas);
		float avg_area => (sum(areas)/ntris);

		//float min_angle => min_array(angles);
		//float max_angle => max_array(angles);
		//float avg_angle => (sum(angles)/ntris);

		cxxmethod+req update (
			connect+read+req+notify,
			coords+read+req+notify,
			ntris+write,
			areas+write,
			angles+write
		);

		float+OPort2 areas[ntris];
		//float+OPort2 angles[ntris];
	};



	// this module determines if a point is contained within
	// a list of triangles; if so it returns the index of the
	// triangle. if the neighbours list is present and mode = 1
	// it uses a faster algorithm based on a directed search trough the
	// mesh (this is *not* suitable for concave areas or areas with holes)
	// also outputs a count list (no. of points in each triangle)
	module point_in_triangle <src_file="point.cxx",
	                          c_src_files="barycent.c">
	{
		int+IPort2 mode = 0;

		int+IPort2   ncoords;
		float+IPort2 coords[ncoords][2];

		int+IPort2 ntris;
		int+IPort2 connect[3*ntris];
		int+IPort2 neighbours[3*ntris];

		int+IPort2 npoints;
		float+IPort2 point[npoints][2];

		cxxmethod+req update (
			mode+read+req+notify,
			ncoords+read+req+notify,
			coords+read+req+notify,
			ntris+read+req+notify,
			connect+read+req+notify,
			neighbours+read+notify+opt,
			npoints+read+req+notify,
			point+read+req+notify,
			index+write,
			count+write
		);

		int+OPort2 index[npoints];
		int+OPort2 count[ntris];
	};



	// fault line variable grids functionality

	// module which finds which cells contain a fault line
	// set the data in the cells to be null, and output a polyline
	// connection list and coordinate values
	// containing the outline of the nulled cells. this can
	// be combined with the input fault line(s) to create the
	// meshed area
	module fault_region <src_file="fault.cxx"> {

		// input structured mesh coordinates
		float+IPort2 mesh_coords[];
		// structured mesh node connect list
		int+IPort2   mesh_connect[];

		// poly coordinates
		float+IPort2 poly_coords[];

		// the update method
		cxxmethod update (
			mesh_coords+read+req+notify,
			mesh_connect+read+req+notify,
			poly_coords+read+req+notify,
			npts+write,
			pts+write,
			nsegs+write,
			segments+write,
			ncells+write,
			celldat+write
		);

		// output coordinates
		int npts;
		float+OPort2 pts[npts][2];

		// output segment list
		int nsegs;
		int+OPort2 segments[.nsegs*2];

		// cell data (to be applied to input struct mesh)
		int ncells;
		int+OPort2 celldat[.ncells];
	};


};

