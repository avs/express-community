

// This library contains a number of higher level wrapper macros to the triangle module.
// These perform simple delauney triangulation of a set of points, construct the 
// voronoi diagram, create a constrained triangulation and so on.

flibrary TriangleMacs<compile_subs=0> {

	// straightforward delauney triangulation of the input points. the convex
	// hull is included in the output
	macro Delauney {
		ilink in_fld;

		DVxform_coord DVxform_coord {
			in => in_fld; 
			comp = {0,1};
		};
		XP_COMM_PROJ.Triangle.TriangleMods.triangle triangle {
			params = "z";
			in {
				numberofpoints+nres => <-.<-.in_fld.nnodes;
				pointlist => <-.<-.DVxform_coord.coord;
			};
		};
		
		FLD_MAP.tri_mesh tri_mesh {
			coord => <-.triangle.out.pointlist;
			connect => <-.triangle.out.trianglelist;
   		DataObject {
   			Modes {
   				mode = {0,2,1,0,0};
   			};
   			Props {
   				col = {1,0,0};
   			};
			   Obj.name => name_of(<-.<-.<-.<-);
   		};
		};
		
		olink out_mesh => tri_mesh.out;
		olink out_obj  => tri_mesh.obj;
	};



	// this macro returns the voronoi diagram of the input
	// points as a line mesh. the output is sent through
	// fix_voronoi_edges to remove the infinite segments
	// which have -1 indices.
	macro Voronoi {
		ilink in_fld;

		DVxform_coord DVxform_coord {
			in => in_fld;
			comp = {0,1};
		};
		XP_COMM_PROJ.Triangle.TriangleMods.triangle triangle {
			params => "vz";
			in {
				numberofpoints+nres => <-.<-.in_fld.nnodes;
				pointlist => <-.<-.DVxform_coord.coord;
			};
		};
		XP_COMM_PROJ.Triangle.TriangleMods.fix_voronoi_edges fix_voronoi_edges {
			in_edges => <-.triangle.vorout.edgelist;
		};

		FLD_MAP.line_mesh line_mesh {
			coord => <-.triangle.vorout.pointlist;
			connect => <-.fix_voronoi_edges.out_edges;
			DataObject {
   			Props {
   				col = {0,1,0};
   			};
			   Obj.name => name_of(<-.<-.<-.<-);
			};
		};

		olink out_mesh => line_mesh.out;
		olink out_obj  => line_mesh.obj;
	};



	// this macro simply returns the convex hull of the input
	// points as a line mesh
	macro Convex_Hull {
		ilink in_fld;

		DVxform_coord DVxform_coord {
			in => in_fld;
			comp = {0,1};
		};
		XP_COMM_PROJ.Triangle.TriangleMods.triangle triangle {
			params => "cz";
			in {
				numberofpoints+nres => <-.<-.in_fld.nnodes;
				pointlist => <-.<-.DVxform_coord.coord;
			};
		};

		FLD_MAP.line_mesh line_mesh {
			coord => <-.triangle.out.pointlist;
			connect => <-.triangle.out.segmentlist;
			DataObject {
   			Props {
   				col = {0,0,1};
   			};
			   Obj.name => name_of(<-.<-.<-.<-);
			};
		};

		olink out_mesh => line_mesh.out;
		olink out_obj  => line_mesh.obj;
	};


	// this macro triangulates a concave area defined by a polygon
	macro Concave_Tri {
		ilink in_fld;

		DVxform_coord DVxform_coord {
			in => in_fld;
			comp = {0,1};
		};
		XP_COMM_PROJ.Triangle.TriangleMods.triangle triangle {
			params => "pqz";
			in {
				numberofpoints+nres => <-.<-.in_fld.nnodes;
				pointlist => <-.<-.DVxform_coord.coord;
				numberofsegments+nres => <-.<-.in_fld.cell_set[0].ncells;
				segmentlist+nres => <-.<-.in_fld.cell_set[0].node_connect_list;
				segmentmarkerlist+nres => init_array(numberofsegments,1,1);
			};
		};

		FLD_MAP.tri_mesh tri_mesh{
			coord => <-.triangle.out.pointlist;
			connect => <-.triangle.out.trianglelist;
			DataObject {
			   Props {
			      col = {0.,1.,1.};
			   };
			   Obj.name => name_of(<-.<-.<-.<-);
			};
		};

		olink out_mesh => tri_mesh.out;
		olink out_obj  => tri_mesh.obj;
	};



	// this is a slightly more complicated macro, in that it requires an input
	// polygon which defines the external boundary segments and an input
	// polyline which specifies internal points. it is assumed that there
	// are no internal segments or holes.
	macro Triangulate_Area {
		ilink in_fld;
		ilink in_fault;

		DVxform_coord DVxform_coord1 {
			in => <-.in_fld;
			comp = {0,1};
		};
		DVxform_coord DVxform_coord2 {
			in => in_fault;
			comp = {0,1};
		};

		XP_COMM_PROJ.Triangle.TriangleMods.fault_region fault_region {
			mesh_coords+nres => <-.DVxform_coord1.coord;
			mesh_connect+nres => <-.in_fld.cell_set.node_connect_list;
			poly_coords+nres => <-.DVxform_coord2.coord;
		};

		XP_COMM_PROJ.Triangle.TriangleMods.triangle triangle {
			params = "Qpqz";
			in {
				numberofpoints+nres => <-.<-.fault_region.npts;
				pointlist<NEportLevels={3,0}> => <-.<-.fault_region.pts;
				numberofsegments => <-.<-.fault_region.nsegs;
				segmentlist<NEportLevels={3,0}> => <-.<-.fault_region.segments;
			};
			out.pointlist<NEportLevels={0,3}>;
			out.trianglelist<NEportLevels={0,3}>;
		};

		FLD_MAP.tri_mesh tri_mesh {
			coord => <-.triangle.out.pointlist;
			connect => <-.triangle.out.trianglelist;
			DataObject {
   			Props {
   				col = {1,0,1};
   			};
   			Modes {
   				mode = {0,2,0,0,0};
   			};
			   Obj.name => name_of(<-.<-.<-.<-) + "_tri";
			};
		};

		FLD_MAP.combine_mesh_cell_data combine_mesh_cell_data {
			in_mesh => <-.in_fld;
			in_data => <-.fault_region.celldat;
         cell_data {
            cell_data = {
               {
                  null_flag=1,null_value=1,,,,,,
               }
            };
         };
/*
			!cell_data[0] {
				null_flag = 1;
				null_value = 1;
			};
*/
			DataObject {
			   Obj.name => name_of(<-.<-.<-.<-) + "_grid";
			};
		};

		GDM.GroupObject GroupObject {
			child_objs => {<-.combine_mesh_cell_data.obj, <-.tri_mesh.obj};
		   Top.name => name_of(<-.<-.<-);
		};

		olink out_mesh => tri_mesh.out;
		olink out_fld  => combine_mesh_cell_data.out;
		olink out_obj  => GroupObject.obj;
	};



	// this one is an (almost) fully featured mesh generator. it takes
	// as input a boundary mesh and any holes/segments. it then
	// produces an initial mesh which is passed copied into the second
	// triangle module to refine according to the min angle / max area
	// criteria
/*
	macro Mesh_Generator {

		link in_bdry<NEportLevels={2,1}>;
		link in_holes<NEportLevels={2,1}>;
		link in_regions<NEportLevels={2,1}>;

		int mode;
		float max_area;
		float min_angle;

		XP_COMM_PROJ.Triangle.TriangleMods.triangle triangle1 {

		};

		XP_COMM_PROJ.Triangle.TriangleMods.triangle triangle2 {


		};

	};
*/

	
	// compute some statistics about the triangle mesh (area
	// of each triangle and the min angle in each triangle).
	// turn this into cell data and combine with the mesh
	macro Triangle_Stats {
		ilink in_fld;
		
		int component = 0;

		XP_COMM_PROJ.Triangle.TriangleMods.compute_triangle_areas compute_triangle_areas {
			coords+nres => in_fld.coordinates.values;
			connect+nres => in_fld.cell_set.node_connect_list;
		};

		//float+nres stat[] => switch(component+1,compute_triangle_areas.areas,compute_triangle_areas.angles);
		float+nres stat[] => compute_triangle_areas.areas;
		FLD_MAP.combine_mesh_cell_data combine_mesh_cell_data {
			in_mesh => in_fld;
			in_data => stat;
			DataObject {
			   Obj.name => name_of(<-.<-.<-.<-);
			};
		};

		olink out_fld => combine_mesh_cell_data.out;
		olink out_obj => combine_mesh_cell_data.obj;
	};



	group struct_to_fld {
		XP_COMM_PROJ.Triangle.TriangleMods.struct_triangulateio &struct_triangulateio<NEportLevels={2,1}>;

		Mesh+Tri+Node_Data+Cell_Data tri_mesh<NEportLevels={0,2}> {
			nnodes => <-.struct_triangulateio.numberofpoints;
			nspace = 2;
			ncell_sets = 1;

			coordinates {
				values => <-.<-.struct_triangulateio.pointlist;
			};

			nnode_data => <-.struct_triangulateio.numberofpointattributes;
			node_data {
				values => <-.<-.struct_triangulateio.pointattributelist;
				labels = "Point Attributes";
			};
		};
	};

};

