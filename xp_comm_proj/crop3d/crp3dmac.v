flibrary Crop3DMacs {

	macro Crop3DFunc {
		ilink in;
		XP_COMM_PROJ.Crop3D.Crop3DMods.Crop3DParams+IPort2 &params; 
		XP_COMM_PROJ.Crop3D.Crop3DMods.Crop3DCore Crop3DCore {
			in => <-.in;
			params => <-.params;
		};
		olink out => Crop3DCore.out;
	};

	macro Crop3DUI {
		Field+IPort2 &in;
		XP_COMM_PROJ.Crop3D.Crop3DMods.Crop3DParams+IPort2 &params; 
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
			option {
				set = 1;
			};
		};
		VUIRangeTypein x_range {
			parent => <-.UImod_panel;
			width => parent.clientWidth;
			title = "X limits";
			min => <-.in.coordinates.min_vec[0];
			max => <-.in.coordinates.max_vec[0];
			value1 => <-.params.xmin;
			value2 => <-.params.xmax;
		};
		VUIRangeTypein y_range {
			parent => <-.UImod_panel;
			width => parent.clientWidth;
			title = "Y limits";
			min => <-.in.coordinates.min_vec[1];
			max => <-.in.coordinates.max_vec[1];
			value1 => <-.params.ymin;
			value2 => <-.params.ymax;
			y => <-.x_range.y + <-.x_range.height + 10;
		};
		VUIRangeTypein z_range {
			parent => <-.UImod_panel;
			width => parent.clientWidth;
			title = "Z limits";
			min => <-.in.coordinates.min_vec[2];
			max => <-.in.coordinates.max_vec[2];
			value1 => <-.params.zmin;
			value2 => <-.params.zmax;
			y => <-.y_range.y + <-.y_range.height + 10;
		};
	};

	macro Crop3D {
		Field &in<NEportLevels={2,1}>;
		GMOD.parse_v parse_v {
			v_commands = "params.xmin=in.coordinates.min_vec[0];params.xmax=in.coordinates.max_vec[0];params.ymin=in.coordinates.min_vec[1];params.xmax=in.coordinates.max_vec[1];params.zmin=in.coordinates.min_vec[2];params.zmax=in.coordinates.max_vec[2];";
			on_inst = 0;
			trigger => <-.in.nnodes;
			relative => <-;
		};
		XP_COMM_PROJ.Crop3D.Crop3DMods.Crop3DParams params; 
		XP_COMM_PROJ.Crop3D.Crop3DMacs.Crop3DUI Crop3DUI{
			in => <-.in;
			params => <-.params;
		};
		XP_COMM_PROJ.Crop3D.Crop3DMacs.Crop3DFunc Crop3DFunc{
			in => <-.in;
			params => <-.params;
		};
		olink out => Crop3DFunc.out;
		GDM.DataObject DataObject {
			in => <-.Crop3DFunc.out;
		};
		olink obj => DataObject.obj;
	};		

};
