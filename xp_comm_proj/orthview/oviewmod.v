
flibrary OrthogonalViewerMods <build_dir="xp_comm_proj/orthview",
				 out_hdr_file="gen.h",
				 out_src_file="gen.cxx">{

	group+OPort OrthogonalViewerParams {
		int+Port2 showPlane[3];
		int+Port2 plane[3];
		int+Port2 zoom[3];
		int+Port2 globalZoom;
		int+Port2 DataRangeSize[3];
		int+Port2 globalDataRangeSize;
		int+Port2 useGlobal;
		int+Port2 WindowCenter[2];
	};

	module copy_on_modify <src_file="cmodify.cxx"> 
	{
		int+IPort2 in;
		int old;
		int count;
		int reset;		
		int+IPort2 min;
		int+IPort2 max;
		cxxmethod+notify_inst+req update(
			.in+read+notify+req,
			.min+read+notify,
			.reset+read+notify,
			.max+read+notify,
			.old+read+write,
			.count+read+write,
			.val+read+write,
			.change+write,
			.diff+write,
			.out+write
		);
		int+OPort2 change;
		int+OPort2 diff;
		int+OPort2 val;
		int+IPort2 out;
	};
	
	module TransSliceCute<src_file="translice.c"> {
		Mesh+Node_Data+IPort2  &in;
		float+IPort2 plane;
		int+IPort2 axis;
		int+IPort2 dec;
	
		group &_3D_Coords {
			int+Port2 Dimension => in.coordinates.nvals;
			float+Port2 &xyz_Coords[.Dimension][3] => in.coordinates.values;
		};

		omethod+notify_inst+req update(
			.plane+read+notify,
			.axis+read+notify,
			._3D_Coords+read+notify,
			.Dimension+write,
			.xy_out+write,
			.dec+write
		) = "TransSlice_Mesh";

		int Dimension = 0;
		float xy_out[.Dimension][2];

		Mesh Mesh {
			nnodes => <-.Dimension;
			nspace => 2;
			coordinates {
				&values => <-.<-.xy_out;
			};
			ncell_sets => in.ncell_sets;
			cell_set => tmp_cell_set.field;
		};
		
		group tmp_node [in.nnode_data]{
			Data_Array &field => in.node_data[index_of(<-.tmp_node)];
			};
		
		group tmp_cell_set [in.ncell_sets]{
			Data_Array &field => in.cell_set[index_of(<-.tmp_cell_set)];
			};

		Node_Data ND {
			nnodes => in.nnodes;
			nnode_data => in.nnode_data;
			node_data => tmp_node.field;
			};

		FLD_MAP.combine_mesh_data CMD1 {
			in_mesh => switch(<-.Dimension>0,<-.Mesh);
			in_nd => switch(<-.Dimension>0,<-.ND);
		};
		olink out => CMD1.out;
	};
};
