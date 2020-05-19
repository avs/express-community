
flibrary WiggleMods <build_dir="xp_comm_proj/wiggle",
                      out_hdr_file="gen.h",
                      out_src_file="gen.cxx">
{

	group+OPort WiggleParams {

		int direction;
		float scale;
		int mode;
		int component;
		float offset;
	};

        module WiggleCore <src_file = "wig.cxx",
				cxx_hdr_files="fld/Xfld.h"> {
		
                WiggleParams+IPort2 &params;
		
		// input mesh
                Mesh_Struct+Node_Data+IPort2 &in_fld {
			ndim = 2;
		};

		float in_data[] => in_fld.node_data[params.component].values;

		// input params
		int direction => params.direction;
		float scale => params.scale;
		int mode => params.mode;
		int component => params.component;
		float offset => params.offset;

		// outputs
                float+OPort2 line_pts[in_fld.nnodes][2];
                int+OPort2 line_connect[2*in_fld.dims[!direction]];
                float+OPort2 tri_pts[4*in_fld.nnodes][2];
                int+OPort2 tri_connect[2*in_fld.dims[!direction]];

		// method:
		cxxmethod+req update<status=1>(
			in_fld+read+req+notify,
			direction+read+req+notify,
			scale+read+req+notify,
			mode+read+req+notify,
			component+read+req+notify,
			offset+read+req+notify,
			line_pts+write,
			line_connect+write,
			tri_pts+write,
			tri_connect+write
		);
	};
};
