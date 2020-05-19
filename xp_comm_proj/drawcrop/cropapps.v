flibrary DrawCropApps //<user_library=0> 
{
	//DrawCrop_Eg

	APPS.SingleWindowApp DrawCrop_Eg<NEdisplayMode="maximized"> {
		UI {
			shell {
				x = 24;
				y = 32;
			};
			Modules {
				IUI {
					optionList {
						selectedItem = 1;
					};
				};
			};
		};
		GDM.Uviewer2D Uviewer2D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.DrawCrop.Principal.obj};
					Xform {
						ocenter = {59.5,59.5,0.};
						dcenter = {4.,4.,0.};
					};
				};
				View {
					View {
						trigger = 2;
					};
				};
			};
			Scene_Selector {
				input_views => {Scene.View.View,
					<-.Scene#1.View.View,<-.Scene#2.View.View,<-.Scene#3.View.View};
			};
			Scene_Editor {
				View_Editor {
					IUI {
						optionList {
							selectedItem = 3;
						};
						General {
							IUI {
								Renderer {
									OPcmdList = {

										{
											set=1,,,
										},
										,,,
									};
								};
								Timer {
									y = 0;
								};
							};
						};
						Create {
							IUI {
								panel {
									visible = 1;
								};
								Camera_Mode {
									OPcmdList = {
										{
											set=1,,,
										},
										,};
								};
								Scene_Mode {
									OPcmdList = {
										{
											set=1,,,
										},
										};
								};
							};
						};
					};
				};
			};
			GDM.Uscene2D Scene#1 {
				Top {
					child_objs<NEportLevels={4,1}> => {
						<-.<-.<-.DrawCrop.Secondary.obj};
					Xform {
						ocenter = {59.5,16.5,0.};
						dcenter = {4.,1.10924,0.};
					};
				};
				View {
					View {
						trigger = 2;
					};
					ViewUI {
						ViewPanel {
							UI {
								panel {
									visible = 0;
								};
							};
						};
					};
				};
			};
			GDM.Uscene2D Scene#2 {
				Top {
					child_objs<NEportLevels={4,1}> => {
						<-.<-.<-.DrawCrop.Tertiary.obj};
					Xform {
						ocenter = {59.5,16.5,0.};
						dcenter = {4.,1.10924,0.};
					};
				};
				View {
					View {
						trigger = 2;
					};
					ViewUI {
						ViewPanel {
							UI {
								panel {
									visible = 0;
								};
							};
						};
					};
				};
			};
			GDM.Uscene3D Scene#3 {
				Top {
					child_objs<NEportLevels={4,1}> => {
						<-.<-.<-.isosurface.out_obj};
				};
				View {
					View {
						trigger = 2;
					};
					ViewUI {
						ViewPanel {
							UI {
								panel {
									visible = 0;
								};
							};
						};
					};
				};
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					x = 490;
					y = 267;
					width = 300;
					height = 390;
					ok = 1;
					dirMaskCache = "$XP_ROOT/data/field/*.fld";
				};
				portable = 0;
				filename = "$XP_ROOT/data/field/lobster.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCrop DrawCrop {
			fieldin => <-.Read_Field.field;
			DrawCropFunc {
				link => <-.<-.Uviewer2D.Scene_Selector.curr_view;
			};
		};
		MODS.isosurface isosurface {
			in_field => <-.DrawCrop._3DFld;
			DVcell_data_labels {
				labels[];
			};
		};
	};
};
