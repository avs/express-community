flibrary RegionGrowApps
{

	APPS.MultiWindowApp RegionGrowEg
	{
		MODS.Read_Field Read_Field {
			read_field_ui {
				filename = "$XP_PATH<0>/data/field/bluntfin.fld";
			};
		};
		MODS.combine_vect combine_vect {
			in_field => <-.Read_Field.field;
			CombineVectParam {
				components = {1,2,3};
			};
		};
		MODS.extract_component extract_component {
			in_field => <-.Read_Field.field;
		};
		MODS.extract_component extract_component#1 {
			in_field => <-.Read_Field.field;
			ExtrCompParam {
				component = 4;
			};
		};
		FLD_MAP.combine_node_datas combine_node_datas {
			in => {
				<-.extract_component.out_fld,<-.combine_vect.out_fld,
				<-.extract_component#1.out_fld};
		};
		FLD_MAP.combine_mesh_data combine_mesh_data {
			in_mesh => <-.Read_Field.field;
			in_nd => <-.combine_node_datas.out;
		};
		XP_COMM_PROJ.Region.RegionGrowMacs.RegionGrow RegionGrow {
			in_data => <-.combine_mesh_data.out;
			params {
				set_null_value = 1;
				invert_region = 0;
				and_precedence = 1;
			};
			RegionUI {
				CombinePanel {
					CombineItem = {
						{
							sel_and=1,
						},
						{
							sel_and=1,
						},
						{
							sel_and=1,
						}};
				};
			};
		};
		MODS.thresh_null thresh_null {
			in_field => <-.RegionGrow.out;
			component = 3;
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {<-.<-.<-.thresh_null.out_obj,<-.<-.<-.bounds.out_obj};
					Xform {
						ocenter = {3.27323,4.16378,2.86213};
						dcenter = {2.48756,-31.4113,-196.294};
						mat = {
							0.199146,-0.0615811,0.265733,0.,
							-0.025452,0.323386,0.0940161,0.,
							-0.271585,-0.0754629,0.186044,0.,
							0.,0.,0.,1.
						};
						xlate = {-2.48756,31.4113,196.294};
					};
				};
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		
	};	
	
	APPS.MultiWindowApp RegionGrowEg2 {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.bounds.out_obj,<-.<-.<-.thresh_null.out_obj};
					Xform {
						ocenter = {31.5,31.5,31.5};
						dcenter = {4.,4.,4.};
						mat = {
							0.0519199,0.0459556,-0.0381229,0.,
							-0.0559736,0.055049,-0.009873,0.,
							0.0207878,0.0334465,0.0686303,0.,
							0.,0.,0.,1.
						};
						xlate = {-4.,-4.,-4.};
						center = {31.5,31.5,31.5};
					};
				};
			};
		};
		XP_COMM_PROJ.Region.RegionGrowMacs.RegionGrow RegionGrow {
			in_data => <-.Read_Field.field;
			params {
				set_null_value = 1;
				invert_region = 1;
				and_precedence = 1;
			};
			RegionUI {
				CombinePanel {
					CombineItem = {
						{
							sel_and=1,
						}
					};
				};
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		MODS.thresh_null thresh_null {
			in_field => <-.RegionGrow.out;
			component = 1;
		};
	};
};
