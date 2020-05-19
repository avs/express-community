flibrary LEAApps {
	APPS.SingleWindowApp LEA_Example<NEdisplayMode="maximized"> {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.LEA_Advector.obj};
					Xform {
						ocenter = {12.,12.,0.};
						dcenter = {4.,4.,0.};
						mat = {
							0.333333,0.,0.,0.,
							0.,0.333333,0.,0.,
							0.,0.,0.333333,0.,
							0.,0.,0.,1.
						};
						xlate = {-4.,-4.,0.};
						center = {12.,12.,0.};
					};
				};
				View {
					View {
						trigger = 2;
					};
				};
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				filename = "$XP_PATH<0>/data/field/wind.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
		MODS.orthoslice orthoslice {
			in_field => <-.Read_Field.field;
			OrthoSliceParam {
				axis = 2;
			};
		};
		MODS.combine_comp combine_comp {
			in_field1 => <-.extract_scalar_u.out_fld;
			in_field2 => <-.extract_scalar_v.out_fld;
		};
		MODS.combine_vect combine_vect {
			in_field => <-.combine_comp.out_fld;
			CombineVectParam {
				veclen = 2;
				components = {0,1};
			};
		};
		MODS.extract_scalar extract_scalar_u {
			in_field => <-.orthoslice.out_fld;
		};
		MODS.extract_scalar extract_scalar_v {
			in_field => <-.orthoslice.out_fld;
			ExtractScalarParam {
				vector = 1;
			};
		};
		XP_COMM_PROJ.LEA.LEAMacs.LEA_Advector LEA_Advector {
			Vector_Field => <-.combine_vect.out_fld;
			params {
				licOn = 1;
				maskOn = 1;
				kernelLength = 8;
				velocityMaskVersion = 3;
				nStepsAtOnce = 5;
			};
		};
	};
	APPS.SingleWindowApp LEA_Stack_Example {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.LEA_Advector.textured_obj,
						<-.<-.<-.LEA_Advector#1.textured_obj,<-.<-.<-.bounds.out_obj,
						<-.<-.<-.LEA_Advector#2.textured_obj};
					Xform {
						mat = {
							0.147887,0.0829824,-0.13088,0.,
							-0.154648,0.0906858,-0.117247,0.,
							0.00998664,0.175432,0.122515,0.,
							0.,0.,0.,1.
						};
						xlate = {-4.18155,-3.50369,-5.62757};
						center = {12.,12.,15.5};
						ocenter = {12.,12.,15.5};
						dcenter = {4.18155,3.50369,5.62757};
					};
				};
				Camera {
					Camera {
						perspec = 1;
						front = 1.;
					};
				};
				Lights {
					LightXform = {
						{
							mat=
							{
								1.,0.,0.,0.,
								0.,1.,0.,0.,
								0.,0.,1.,0.,
								0.,0.,0.,1.
							},,,
						},
						,,};
				};
			};
			Scene_Editor {
				Camera_Editor {
					GDcamera_edit {
						front = 1.;
					};
				};
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				filename = "$XP_PATH<0>/data/field/wind.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
		MODS.orthoslice orthoslice {
			in_field => <-.Read_Field.field;
			OrthoSliceParam {
				axis = 2;
			};
		};
		MODS.combine_vect combine_vect {
			in_field => <-.orthoslice.out_fld;
			CombineVectParam {
				veclen = 2;
				components = {0,1};
			};
		};
		XP_COMM_PROJ.LEA.LEAMacs.LEA_Advector LEA_Advector {
			Vector_Field => <-.combine_vect.out_fld;
			LEAUI {
				ComputationUI {
					looprun {
						count = 0.;
					};
				};
			};
			params {
				nx = 64;
				integrationStep = 4.;
				blendCoef = 0.11;
				nInitialSteps = 5;
				nStepsAtOnce = 2;
				licOn = 1;
				kernelLength = 8;
				maskOn = 1;
				velocityMaskVersion = 3;
				alphaOn = 1;
			};
		};
		MODS.orthoslice orthoslice#1 {
			in_field => <-.Read_Field.field;
			OrthoSliceParam {
				axis = 2;
				plane => 7;
			};
		};
		MODS.combine_vect combine_vect#1 {
			in_field => <-.orthoslice#1.out_fld;
			CombineVectParam {
				veclen = 2;
				components = {0,1};
			};
		};
		XP_COMM_PROJ.LEA.LEAMacs.LEA_Advector LEA_Advector#1 {
			Vector_Field => <-.combine_vect#1.out_fld;
			
			Vector_Field => <-.combine_vect#1.out_fld;
			LEAUI { 
 				ComputationUI {
					looprun {
						count = 1.;
					};
					copy_on_change2 {
						trigger<NEportLevels={4,0}> => <-.<-.<-.<-.LEA_Advector.LEAUI.ComputationUI.looprun.count;
					};
				};
			};
			params {
				licOn = 1;
				maskOn = 1;
				kernelLength = 8;
				velocityMaskVersion = 3;
				nStepsAtOnce = 2;
				nx = 64;
				integrationStep = 4.;
				blendCoef = 0.11;
				nInitialSteps = 5;
				alphaOn = 1;
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		MODS.orthoslice orthoslice#2 {
			in_field => <-.Read_Field.field;
			OrthoSliceParam {
				axis = 2;
				plane => 26;
			};
		};
		MODS.combine_vect combine_vect#2 {
			in_field => <-.orthoslice#2.out_fld;
			CombineVectParam {
				veclen = 2;
				components = {0,1};
			};
		};
		XP_COMM_PROJ.LEA.LEAMacs.LEA_Advector LEA_Advector#2 {
			Vector_Field => <-.combine_vect#2.out_fld;
			LEAUI {
				ComputationUI {
					looprun {
						count = 1.;
					};
					copy_on_change2 {
						trigger<NEportLevels={4,0}> => <-.<-.<-.<-.LEA_Advector.LEAUI.ComputationUI.looprun.count;
					};
				};
			};
			params {
				licOn = 1;
				maskOn = 1;
				kernelLength = 8;
				velocityMaskVersion = 3;
				nStepsAtOnce = 2;
				nx = 64;
				integrationStep = 4.;
				blendCoef = 0.11;
				nInitialSteps = 5;
				alphaOn = 1;
			};
		};
	};
};
