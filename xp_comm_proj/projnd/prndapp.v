
flibrary ProjectNodeDataApps
{
	APPS.SingleWindowApp ProjectNodeData2DEg<NEdisplayMode="maximized"> {
		MODS.Read_Field Read_Field {
			read_field_ui {
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
		};
		XP_COMM_PROJ.ProjectNodeData.ProjectNodeDataMacs.ProjectNodeData ProjectNodeData {
			in => <-.Read_Field.field;
		};
		MODS.reset_xform reset_xform {
			in_field => <-.ProjectNodeData.out;
		};
		GDM.Uviewer2D Uviewer2D {
			Scene {
				Top {
					child_objs => {<-.<-.<-.reset_xform.out_obj};
				};
			};
		};
	};
	APPS.SingleWindowApp ProjectNodeData3DEg<NEdisplayMode="maximized"> {
		MODS.Read_Field Read_Field {
			read_field_ui {
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		MODS.isosurface isosurface {
			in_field => <-.Read_Field.field;
		};
		XP_COMM_PROJ.ProjectNodeData.ProjectNodeDataMacs.ProjectNodeData ProjectNodeData {
			in => <-.Read_Field.field;
			params {
				axis = 0;
				method = "Maximum";
			};
		};
		XP_COMM_PROJ.ProjectNodeData.ProjectNodeDataMacs.ProjectNodeData ProjectNodeData#1 {
			in => <-.Read_Field.field;
			params {
				axis = 1;
				method = "Maximum";
			};
		};
		XP_COMM_PROJ.ProjectNodeData.ProjectNodeDataMacs.ProjectNodeData ProjectNodeData#2 {
			in => <-.Read_Field.field;
			params {
				axis = 2;
				method = "Depth+Maximum";
			};
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.bounds.out_obj,
						<-.<-.<-.isosurface.out_obj,
						<-.<-.<-.ProjectNodeData.obj,
						<-.<-.<-.ProjectNodeData#1.obj,
						<-.<-.<-.ProjectNodeData#2.obj
					};
					Xform {
						ocenter = {31.5,31.5,31.5};
						dcenter = {4.,4.,4.};
						mat = {
							0.0663746,0.0020215,0.038806,0.,
							-0.0228962,0.064094,0.0358234,0.,
							-0.0313967,-0.0424671,0.055914,0.,
							0.,0.,0.,1.
						};
						xlate = {-4.,-4.,-4.};
					};
				};
			};
		};
	};
};
