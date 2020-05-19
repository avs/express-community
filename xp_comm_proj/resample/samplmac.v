flibrary DownSampleMacs{
	macro DownSampleFunc {
      Mesh_Unif+Node_Data &in<NEportLevels={2,1}>; 
	  XP_COMM_PROJ.DownSample.DownSampleMods.DownSampleParams &params; 
	  DataObject DataObject {
          in => <-.downsample.out_field;
      };
	  XP_COMM_PROJ.DownSample.DownSampleMods.downsample downsample {
        in_field => <-.in;
		params => <-.params;
      };
      link out_fld<NEportLevels={1,2}> => .downsample.out_field;
      link out_obj<NEportLevels={1,2}> => .DataObject.obj;
	};

	macro DownSampleUI {
		UImod_panel panel {
			title => name_of(<-.<-.<-,1);
			width => (parent.clientWidth - 10);
		};
		XP_COMM_PROJ.DownSample.DownSampleMods.DownSampleParams &params<NEportLevels={2,0}>;
		UIslider size_x{
			parent => <-.panel;
			min = 2.;
			max => <-.params.dims[0];
			mode = "integer";
			value => <-.params.output_size[0];
			title = "new mesh x size";
			y => 0;
			width => parent.clientWidth;
		};
		UIslider size_y{
			parent => <-.panel;
			min = 2.;
			max => <-.params.dims[1];
			mode = "integer";
			value => <-.params.output_size[1];
			title = "new mesh y size";
			y => <-.size_x.y + <-.size_x.height + 10;
			width => parent.clientWidth;
		};
		UIslider size_z{
			visible=>switch(((<-.params.ndim == 3) + 1),0,1);
			parent => <-.panel;
			min = 2.;
			max => switch((<-.params.ndim - 1),1,<-.params.dims[2]);
			mode = "integer";
			value => <-.params.output_size[2];
			title = "new mesh z size";
			y => <-.size_y.y + <-.size_y.height + 10;
			width => parent.clientWidth;
		};
	};

	macro DownSample {
		XP_COMM_PROJ.DownSample.DownSampleMods.DownSampleParams params {
			ndim => <-.in.ndim;
			dims => <-.in.dims[];
			output_size = { 0, 0, 0 };
		};
		XP_COMM_PROJ.DownSample.DownSampleMacs.DownSampleFunc DownSampleFunc {
			in => <-.in;
			params<NEportLevels={2,1}> => <-.params;
		};
		XP_COMM_PROJ.DownSample.DownSampleMacs.DownSampleUI DownSampleUI {
			params => <-.params;
		};
		Mesh_Unif+Node_Data &in<NEportLevels={2,1}>;
		link out_field<NEportLevels={1,2}> => .DownSampleFunc.out_fld;
		link out_obj<NEportLevels={1,2}> => .DownSampleFunc.out_obj;
	};

};
