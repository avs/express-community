
flibrary WiggleMacs <compile_subs=0> {

        macro WiggleUI {
                UImod_panel mod_panel {
			title = "Wiggle";
			parent+nres => <-.<-.<-.UI.Modules.IUI.mod_panel;
		};
                UIradioBoxLabel Wiggle_Direction {
			parent => <-.mod_panel;
			labels => <-.string;
                        selectedItem => <-.params.direction;
		};
                string string [] = {"X axis","Y axis"};
                UIslider Wiggle_Scale {
			parent => <-.mod_panel;
                        value => <-.params.scale;
			y => <-.Wiggle_Direction.y + <-.Wiggle_Direction.height + 10;
                        width => parent.clientWidth;
			min = -0.1;
			max = 0.1;
			decimalPoints = 4;
		};
                UIradioBoxLabel Wiggle_Component {
                        parent => <-.mod_panel;
			labels => <-.data_labels;
                        selectedItem => <-.params.component;
			label_cmd {
				outItem => inItem;
			};
			y => <-.offset.y + <-.offset.height + 10;
		};
                UIoption No_Fill;
                UIoptionMenu Wiggle_Mode {
			parent => <-.mod_panel;
			cmdList => {
				<-.No_Fill,<-.Fill_Above,<-.Fill_Below,<-.Above_and_Below
			};
                        selectedItem => <-.params.mode;
			y => <-.Wiggle_Scale.y + <-.Wiggle_Scale.height + 10;
                        width => parent.clientWidth;
		};
                UIoption Fill_Above {
			set = 1;
		};
                UIoption Fill_Below;
                UIfield offset {
			parent => <-.mod_panel;
                        value => <-.params.offset;
			y => <-.Wiggle_Mode.y + <-.Wiggle_Mode.height+10;
			x => <-.Wiggle_Offset.x + Wiggle_Offset.width;
		};
                UIlabel Wiggle_Offset {
			parent => <-.mod_panel;
			y => <-.offset.y;
			alignment = "left";
		};
		UIoption Above_and_Below;
                string data_labels [];


                XP_COMM_PROJ.Wiggle.WiggleMods.WiggleParams+IPort2 &params;

	};


	macro WiggleFunc {


                ilink in_fld;

                XP_COMM_PROJ.Wiggle.WiggleMods.WiggleParams+IPort2 &params; 
                XP_COMM_PROJ.Wiggle.WiggleMods.WiggleCore WiggleCore {
			in_fld => <-.in_fld;
			params => <-.params;
		};

		FLD_MAP.polyline_mesh polyline_mesh {
			coord => <-.WiggleCore.line_pts;
			connect => <-.WiggleCore.line_connect;
		};
		FLD_MAP.polytri_mesh polytri_mesh {
			coord => <-.WiggleCore.tri_pts;
			connect => <-.WiggleCore.tri_connect;
			DataObject {
				Obj {
					visible => <-.<-.<-.WiggleCore.mode;
				};
			};
		};
                olink out_line => .polyline_mesh.out;
                olink out_tri => .polytri_mesh.out;
		GroupObject objs {
			child_objs => { 
				<-.polyline_mesh.obj, <-.polytri_mesh.obj
			};
		};
                olink out_obj => objs.obj;
			
	};

	macro Wiggle {


                ilink in_fld;

                XP_COMM_PROJ.Wiggle.WiggleMods.WiggleParams params{
			direction = 0;
			scale = 0.01;
			mode = 1;
			component = 0;
			offset = 0;

		}; 
                XP_COMM_PROJ.Wiggle.WiggleMacs.WiggleFunc WiggleFunc {
			in_fld => <-.in_fld;
			params => <-.params;
		};
		XP_COMM_PROJ.Wiggle.WiggleMacs.WiggleUI WiggleUI {
			params => <-.params;
		};

		olink out_line => WiggleFunc.out_line;
                olink out_tri => WiggleFunc.out_tri;
		olink out_obj => WiggleFunc.out_obj;
	};
};
