
flibrary VolumeIntegralMacs <compile_subs=0> {

	macro VolumeIntegralUI {
		XP_COMM_PROJ.VolumeIntegral.VolumeIntegralMods.VolumeIntegralParams+IPort2 &params;
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);			
		};

		UIlabel Conversion {
			parent => <-.UImod_panel;
			width => parent.clientWidth;
		};
		UIfield Conversion_data {
			parent => <-.UImod_panel;
			y => <-.Conversion.y + <-.Conversion.height+10;
			width => parent.clientWidth;
			value => <-.params.conversion;
		};
		
		UIlabel Volume {
			parent => <-.UImod_panel;
			y => <-.Conversion_data.y + <-.Conversion_data.height+10;
			width => parent.clientWidth;
		};
		UIfield Volume_data {
			parent => <-.UImod_panel;
			y => <-.Volume.y + <-.Volume.height+10;
			width => parent.clientWidth;
			value => <-.params.volume;
			outputOnly=1;
		};

		UIlabel Area {
			parent => <-.UImod_panel;
			y => <-.Volume_data.y + <-.Volume_data.height+10;
			width => parent.clientWidth;
		};
		UIfield Area_data {
			parent => <-.UImod_panel;
			y => <-.Area.y + <-.Area.height+10;
			value => <-.params.area;
			width => parent.clientWidth;
			outputOnly=1;
		};
	};

	macro VolumeIntegralFunc {
		Mesh+IPort2 &in {
			int+opt nnode_data;
			Data_Array+opt node_data[.nnode_data] {
				nvals => <-.nnodes;
			};
			xform+nonotify;
		};
		XP_COMM_PROJ.VolumeIntegral.VolumeIntegralMods.VolumeIntegralParams+IPort2 &params;

		XP_COMM_PROJ.VolumeIntegral.VolumeIntegralMods.VolumeIntegralCore VolumeIntegralCore	 {
			in => <-.in;
			conversion => <-.params.conversion;
			area=> <-.params.area;
			volume=> <-.params.volume;
		};
		olink area => VolumeIntegralCore.area;
		olink volume => VolumeIntegralCore.volume;
	};

	macro VolumeIntegral {
		ilink in;
		XP_COMM_PROJ.VolumeIntegral.VolumeIntegralMods.VolumeIntegralParams params {
			conversion = 1.0;
			area = 0.0;
			volume = 0.0;
		};
		XP_COMM_PROJ.VolumeIntegral.VolumeIntegralMacs.VolumeIntegralUI VolumeIntegralUI {
			params => <-.params;
		};
		XP_COMM_PROJ.VolumeIntegral.VolumeIntegralMacs.VolumeIntegralFunc VolumeIntegralFunc {
			in => <-.in;
			params => <-.params;
		};
		olink area => VolumeIntegralFunc.area;
		olink volume => VolumeIntegralFunc.volume;
	};	
};



