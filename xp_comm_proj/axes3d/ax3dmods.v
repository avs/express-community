flibrary Axes3DMods<build_dir="xp_comm_proj/axes3d", out_hdr_file="gen.h">
{
	group axes_mod_params {
		long n_labels<NEportLevels={0,2}>;
		float intervals<NEportLevels={0,2}>[3];
		float min_extent<NEportLevels={0,2}>[3];
		float max_extent<NEportLevels={0,2}>[3];
		long ixyz<NEportLevels={0,2}>[3];
		long pos<NEportLevels={0,2}>[6];
		long label_pos<NEportLevels={0,2}>[3];
		long frames<NEportLevels={0,2}>[3];
		float min_vals<NEportLevels={0,2}>[];
		float max_vals<NEportLevels={0,2}>[];
		long num_labels<NEportLevels={0,2}>[3];
	};
	group axes_geom_params<NEx=506.,NEy=22.> {
		string labels<NEportLevels={0,2}>[3];
		int h<NEportLevels={0,2}>;
		int lh<NEportLevels={1,1}> => (1.5 * .h);
		int planes_vis<NEportLevels={1,2}>[3];
		int labels_align<NEportLevels={1,2}>[6];
		string font<NEportLevels={1,2}> => (("-adobe-helvetica-*-r-*-*-" + .h) + "-*-*-*-*-*-*-*");
		string l_font<NEportLevels={1,2}> => (("-adobe-helvetica-*-r-*-*-" + .lh) + "-*-*-*-*-*-*-*");
	};
	group axis_data {
		Mesh_Struct &labels<NEportLevels={0,2}>;
		float labels_v[];
		string labels_f;
	};
	group TextVals {
		int align_horiz<NEportLevels={2,0}>;
		int align_vert<NEportLevels={2,0}> = 0;
		int drop_shadow<NEportLevels={2,0}> = 0;
		int background<NEportLevels={2,0}> = 0;
		int bounds<NEportLevels={2,0}> = 0;
		int underline<NEportLevels={2,0}> = 0;
		int lead_line<NEportLevels={2,0}> = 0;
		int radial<NEportLevels={2,0}> = 0;
		int do_offset<NEportLevels={2,0}> = 0;
		float offset<NEportLevels={2,0}>[3] = {0.,0.,0.};
		int xform_mode;
		int color;
		string text_values[];
		int stroke = 0;
		group StrokeTextAttribs {
			int font_type;
			int style;
			int plane;
			int orient;
			int path;
			int space_mode;
			float spacing;
			float angle;
			float height;
			float expansion;
			float width;
		};
	};

	module Axes3DCore<src_file="axes.c"> {
		omethod+notify_inst+req axes(
			n_labels+read+notify,intervals+read+notify,
			min_extent+read+notify,max_extent+read+notify,
			ixyz+read+notify+req,frames+read+notify+req,pos+read+notify+req,
			label_pos+read+notify+req,min_vals+read+notify,
			max_vals+read+notify,xy_mesh+write,xz_mesh+write,yz_mesh+write,
			xy_frame+write,xz_frame+write,yz_frame+write,x_labels+write,
			y_labels+write,z_labels+write,x_labels_v+write,y_labels_v+write,
			z_labels_v+write,x_labels_f+write,y_labels_f+write,
			z_labels_f+write,axe_labels_loc+write,num_labels+write
		) = "axes";
		axes_mod_params &params<NEportLevels={2,0}>;
		long n_labels => params.n_labels;
		float intervals[] => params.intervals;
		float min_extent[] => params.min_extent;
		float max_extent[] => params.max_extent;
		long ixyz[] => params.ixyz;
		long pos[] => params.pos;
		long label_pos[] => params.label_pos;
		long frames[] => params.frames;
		float min_vals[] => params.min_vals;
		float max_vals[] => params.max_vals;
		long num_labels[3] => params.num_labels;
		Mesh_Struct xy_mesh;
		Mesh_Struct xz_mesh;
		Mesh_Struct yz_mesh;
		Mesh_Struct xy_frame;
		Mesh_Struct xz_frame;
		Mesh_Struct yz_frame;
		Mesh_Struct planes<NEportLevels={0,2}>[6] => {
			.xy_mesh,.xz_mesh,.yz_mesh,.xy_frame,.xz_frame,.yz_frame
		};
		float axe_labels_loc<NEportLevels={0,2}>[3][3] = {-9999.,0.,0.,0.,1.,0.,0.,0.,1.};
		Mesh_Struct x_labels;
		float x_labels_v[];
		string x_labels_f;
		axis_data x_axis {
			labels => <-.x_labels;
			labels_v => <-.x_labels_v;
			labels_f => <-.x_labels_f;
		};
		Mesh_Struct y_labels;
		float y_labels_v[];
		string y_labels_f;
		axis_data y_axis {
			labels => <-.y_labels;
			labels_v => <-.y_labels_v;
			labels_f => <-.y_labels_f;
		};
		Mesh_Struct z_labels;
		float z_labels_v[];
		string z_labels_f;
		axis_data z_axis {
			labels => <-.z_labels;
			labels_v => <-.z_labels_v;
			labels_f => <-.z_labels_f;
		};
		axis_data axes_data<NEportLevels={0,2}>[3] => {.x_axis,.y_axis,
			.z_axis};
	};
};
