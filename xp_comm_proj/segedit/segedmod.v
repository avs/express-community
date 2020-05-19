
flibrary SegmentationEditorMods <build_dir="xp_comm_proj/segedit",
						 out_src_file="gen.cxx">
{
    enum DrawMaskCommands {
        choices => {
            "Draw Point",
            "Draw Line",
            "Close Line",
            "Fill Area",
            "New Area",
            "Invert Mask",
            "Clear Mask",
            "Grow Region 2D",
            "Grow Region 3D",
            "Threshold",
            "Save 3D Mask",            
            "Load 3D Mask",
            "Clear 3D Mask"
        };
    };

    enum DrawMaskPens {
        choices => {
            "Draw",
            "Erase"
        };
    };

    enum ApplyMaskCommands {
        choices => {
            "Add Mask",
            "Apply Mask"
        };
    };

    enum ApplyRegionCommands {
        choices => {
            "Add to Mask",
            "Replace Mask"
        };
    };

    group+OPort SegmentationEditorParams {
        int+Port axis;
        int+Port slice;
        int+Port dims[];
        int+Port clear_volume;
        int+Port clear_mask3D;
        int+Port edit_entity;
        int+Port reg_incr;
        double+Port reg_delta;
        int+Port reg_trig;
        double+Port thr_min;
        double+Port thr_max;
        int+Port thr_trig;
        int+Port thr_comp;
        string+Port label;
        string+Port filename;
        string+Port entities[];
        string+Port readentities[];
        int+Port selentities[];
        int+Port outentities[];
        int+Port edit_selentities[];
        int+Port combine_entity;
        int+Port delete_entity;
        int+Port allow_read;
        int+Port start_read;
        int+Port start_write;
        DrawMaskCommands+Port drawcommand;
        DrawMaskPens+Port pen;
        ApplyMaskCommands+Port applycommand;
        ApplyRegionCommands+Port applyregcommand;
        int+Port width;
        int+Port value;
        int+Port value_nd;
    };

	#ifdef XP_VERSION_PRE_63
	module demux <src_file="demux.cxx",out_hdr_file="gen_dmux.h"> {
		int+IPort2 input;
		int+IPort2 input_max;
		group+OPort2 output[input_max]{
			int trigger;
		};
        cxxmethod update (
            .input+read+notify+req,
			.input_max+read+req,
			.output+write
        );
	};

	module copy_node_data <
		src_file="copdata.cxx",
		cxx_hdr_files="fld/Xfld.h",
		out_hdr_file="gen_copy.h"> {
		Node_Data+IPort2 &input;
		Node_Data+IPort2 &output;
        cxxmethod update (
            .input+read+notify+req,
			.output+write
        );
	};


    module DrawMask <
		src_file="drawmask.cxx",
		out_hdr_file="gen_mask.h",
		cxx_hdr_files="fld/Xfld.h drawmask.h",
		c_hdr_files="avs/gd.h",
        cxx_members="    private:\n        int setup_environment(DrawMaskStruct * dmstr);\n        int init_environment(int command);\n">
    {
        DrawMaskCommands+IPort2 command = "Draw Point";
        DrawMaskPens+IPort2 pen = "Draw";
        ApplyRegionCommands+Port regcmd;

        int+IPort2 width = 0;

        int slice;
        int axis;
        int x;
        int y;
        double thr_min;
        double thr_max;
        int thr_trig;
        int thr_comp;
        double reg_delta;
        double reg_incr;
        int reg_trig;
        int state;
        int+IPort2 value;

        // The export properties are simply to reduce the
        // amount of generated C++ code.
        GDobject_templ+IPort2 &obj<export=0>;
        GDview_templ+IPort2 &view<export=0>;

        GDcamera_templ &camera<export=0> => .view.picked_camera;
        ptr    local_ptr<NEvisible=0>;

        Mesh_Unif+Node_Data+Port2 &mask;
        Node_Data+IPort2 &in_slice;

        cxxmethod+notify_inst OnInstance (
            .local_ptr+write
        );

        cxxmethod+notify_deinst OnDeinstance (
            .local_ptr+read+write
        );

        cxxmethod+req OnDraw (
            .x+read,
            .y+read,
            .thr_min+read,
            .thr_max+read,
            .thr_trig+read+write+notify,
            .thr_comp+read,
            .reg_trig+notify,
            .reg_delta+read+notify,
            .reg_incr+read+notify,
            .regcmd+read,
            .in_slice+read,
            .slice+read,
            .axis+read,
            .state+read+notify+req,
            .camera+read,
            .obj+read+req,
            .view+read+req,
            .value+read,
            .command+read+write+notify+req,
            .pen+read+write+notify+req,
            .seed_point+read,
            .width+read,
            .nconn+write,
            .conn+write,
            .npoints+write,
            .points+read+write,
            .local_ptr+read,
            .mask+read+write,
            .draw+write
        );

        int+OPort2 draw;
        int+OPort2 npoints;
        float+OPort2 points[npoints][2];
        int+OPort2 seed_point[3];
    };

    module GrowRegion <
		src_file="segreg.cxx",
		out_hdr_file="gen_grow.h",
		cxx_hdr_files="seggrow.h fld/Xfld.h",
		cxx_src_files="seggrow.cxx"
	>{
        int+IPort2 dims[];
        Node_Data+IPort2 &in;
        int+IPort2 seed[];
        int+IPort2 combine_and_or[];
        double+IPort2 delta[];
        int+IPort2 use_diagonals;
        int+IPort2 set_null_value;
        int+IPort2 and_precedence;
        int+IPort2 invert_region;
        int+IPort2 out_val;
        string+IPort2 out_label;
        int incremental[];
        int trigger=1;

        Node_Data+OPort2 out;

        cxxmethod+notify_inst+req update (
            .trigger+read+req,
            .dims+read+req,
            .in+read+req,
            .seed+read+notify+req,
            .combine_and_or+read+notify+req,
            .delta+read+notify+req,
            .use_diagonals+read+notify+req,
            .incremental+read+notify+req,
            .out_val+read+req,
            .out_label+read+req,
            .set_null_value+read+notify+req,
            .and_precedence+read+notify+req,
            .invert_region+read+notify+req,
            .out+write
        );
    };
	#endif  //XP_VERSION_PRE_63

    module SegmentationEditorCore <
		src_file="segedit.cxx",
		out_hdr_file="gen_seg.h",
		cxx_hdr_files="fld/Xfld.h",
        cxx_members="    private:\n        int Mask_2D_from_3D();\n">
    {
        int+IPort2 axis;
        int+IPort2 slice;
        int+IPort2 value;
        int+IPort2 use_value;
        int+IPort2 draw;
        int+IPort2 clear;
        int+IPort2 clearMask;
        int+IPort2 clearMask3D;
        int+IPort2 clearEntity;
        int+IPort2 entityList[];
        int+IPort2 outEntities[];
        int+IPort2 combineEntity;
        int+OPort2 value_nd;
        double reg_delta;
        double reg_incr;
        int grow_region_3D;
        int seed[];
        string+IPort2 label;
        ApplyMaskCommands+IPort2 command = "Apply Mask";
        ApplyRegionCommands+IPort2 regcmd = "Add to Mask";

        Mesh_Unif+Node_Data+IPort2 &in_field;
        Mesh_Unif+Node_Data+Port2 &mask;
        Mesh_Unif+Node_Data+Port2 &mask3D;
        Node_Data+IPort2 & region2D;
        Node_Data+IPort2 & region3D;

        Mesh_Unif+Node_Data+Port2 &out;

        cxxmethod+notify_inst OnInstance (
            in_field+read+notify,
            clear+read+write+notify,
            clearMask+read+write+notify,
            clearMask3D+read+write+notify,
            mask+read+write,
            mask3D+read+write,
            axis+read,
            out+read+write
        );

        cxxmethod+req OnClear (
            entityList+read+write,
            clearEntity+read+write+notify,
            combineEntity+read+write+notify,
            value+read+write,
            label+write,
            value_nd+write,
            outEntities+read+write,
//            clearMask+read+write+notify,
            clearMask3D+read+write+notify,
//            mask+read+write+req,
            mask3D+read+write+req
//            out+read+write+req,
//            axis+read,
//            slice+read,
//            command+read+req
        );

        cxxmethod+req OnChange (
            in_field+read+write,
            stop+read+write,
            axis+read+notify+req,
            slice+read+notify+req,
            grow_region_3D+read,
            value+read+notify,
            value_nd+write,
            draw+read+notify,
            use_value+read+notify,
            region2D+read+notify,
            region3D+read+notify,
            mask+read+write+req,
            mask3D+read+notify+write+req,
            seed+read+notify,
            reg_delta+read+notify,
            reg_incr+read+notify,
            command+read+notify+req,
            regcmd+read+notify+req,
            label+read+write,
            out+read+write
        );
    };

};

