
flibrary SegmentationEditorApps <compile_subs=0>
{
	APPS.MultiWindowApp SegmentationEditorEg {
		GDM.Uviewer2D Uviewer2D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.SegmentationEditor.mask_obj};
					Xform {
						ocenter = {31.5,31.5,0.};
						dcenter = {-23.5,-23.5,0.};
					};
				};
				View {
					View<NEportLevels={0,4}> {
						renderer = "OpenGL";
						trigger = 1;
						two_point<NEdisplayMode="open">;
					};
					ViewUI {
						PickInteractor {
							startEvent = "<Btn3Down>";
							runEvent = "<Btn3Motion>";
							stopEvent = "<Btn3Up>";
						};
						ViewPanel {
							UI {
								option<NEx=429.,NEy=110.> {
									label => "Draw Mask Input";
								};
							};
						};
					};
				};
			};
			Scene_Selector {
				input_views => {
					Scene.View.View,
					<-.Scene#1.View.View,
					<-.Scene#2.View.View
				};
			};
			GDM.Uscene3D Scene#1 {
				Top {
					child_objs<NEportLevels={4,1}> => {
						<-.<-.<-.SegmentationEditor.obj};
					Xform {
						ocenter = {31.5,31.5,31.5};
						dcenter = {5.04604,4.96514,	1.35753};
						mat = {
							0.0390662,-0.0298619,-0.0567197,0.,
							-0.0595915,-0.0413904,-0.0192533,0.,
							-0.0236148,0.0550469,-0.0452462,0.,
							0.,0.,0.,1.
						};
						xlate = {-5.04604,-4.96514,-1.35753};
					};
				};
				View {
					ViewUI {
						ViewPanel {
							UI {
								option {
									label => "Segmentation Editor Output";
								};
							};
						};
					};
				};
			};
			GDM.Uscene3D Scene#2 {
				Top {
					child_objs<NEportLevels={4,1}> => {
						<-.<-.<-.SegmentationEditor.mask3D_obj};
					Xform {
						ocenter = {31.5,31.5,31.5};
						dcenter = {2.52551,3.26337,	3.13277};
						mat = {
							0.0721697,-0.00520662,-0.0328195,0.,
							-0.00371893,0.0767125,-0.0203477,0.,
							0.0330211,0.020019,0.0694375,0.,
							0.,0.,0.,1.
						};
						xlate = {-2.52551,-3.26337,-3.13277};
					};
				};
				View {
					ViewUI {
						ViewPanel {
							UI {
								option {
									label => "Mask 3D";
								};
							};
						};
					};
				};
			};
		};
		XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMacs.SegmentationEditor SegmentationEditor {
			in => <-.Read_Field.field;
			View => <-.Uviewer2D.Scene.View.View;
			params {
				axis = 2;
				slice = 20;
				drawcommand = "Draw Point";
				pen = "Draw";
				width = 10;
				value = 4;
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					x = 556;
					y = 305;
					width = 300;
					height = 386;
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
	};
	
	APPS.MultiWindowApp SegmentationWizardEg {
		XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMacs.SegmentationWizard SegmentationWizard<NEx=319.,NEy=231.,NEdisplayMode="maximized"> {
			in => <-.Read_Field.field;
			visible => 1;
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					x = 556;
					y = 305;
					width = 300;
					height = 386;
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
	};
};
