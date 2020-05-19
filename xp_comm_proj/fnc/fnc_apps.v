flibrary FilterNodeApps {

	APPS.SingleWindowApp FilterNodeConnectEg1 {
		MODS.Read_UCD Read_UCD{
			read_ucd_ui {
				panel {
					filename = "./xp_comm_proj/fnc/cube.inp";
				};
			};
		};
		XP_COMM_PROJ.FilterNodeConnect.FilterNodeMacs.FilterNodeConnect FilterNodeConnect{
			in => <-.select_cells.out_fld;
		};
		MODS.select_cells select_cells{
			in_field => <-.Read_UCD.field;
			sets = {0,1};
		};
		MODS.glyph glyph{
		      in_field => <-.FilterNodeConnect.out;
		      in_glyph => <-.Diamond3D.out_fld;
		      GlyphParam {
		              scale = 0.5;
		              normalize = 1;
      		      };
		 };
		GEOMS.Diamond3D Diamond3D;
		GDM.Uviewer3D Uviewer3D{
			Scene {
				Top {
					child_objs => {<-.<-.<-.glyph.out_obj,<-.<-.<-.select_cells.out_obj};
				};
			};
		};
	};
	
	APPS.MultiWindowApp FilterNodeConnectEg2 {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.glyph.out_obj,<-.<-.<-.select_cells.out_obj};
					Xform {
						ocenter = {7.,-1.625,2.};
						dcenter = {-3.49817,0.823795,-0.99329};
						mat = {
							0.223582,0.0383659,-0.113165,0.,
							0.0287916,0.215716,0.130017,0.,
							0.115971,-0.127521,0.185893,0.,
							0.,0.,0.,1.
						};
						xlate = {3.49817,-0.823795,0.99329};
					};
				};
			};
		};
		MODS.glyph glyph {
			in_glyph => <-.Arrow1.out_fld;
			GlyphUI {
				scale_slider {
					max = 100000000.;
				};
			};
			in_field => <-.FilterNodeConnect.out;
			GlyphParam {
				normalize = 1;
			};
		};
		MODS.Read_UCD Read_UCD {
			read_ucd_ui {
				file_browser {
					ok = 1;
					dirMaskCache = "$XP_PATH<1>/xp_comm_proj/fnc/*";
				};
				filename = "$XP_PATH<1>/xp_comm_proj/fnc/bridge.inp";
			};
		};
		FLD_MAP.extract_data_array extract_data_array {
			in => <-.Read_UCD.field;
		};
		FLD_MAP.extract_data_array extract_data_array#1 {
			in => <-.Read_UCD.field;
			comp = 1;
		};
		FLD_MAP.extract_data_array extract_data_array#2 {
			in => <-.Read_UCD.field;
			comp = 2;
		};
		FLD_MAP.interleave_3_arrays interleave_3_arrays {
			in1 => <-.extract_data_array.data;
			in2 => <-.extract_data_array#1.data;
			in3 => <-.extract_data_array#2.data;
		};
		FLD_MAP.combine_mesh_data combine_mesh_data {
			in_mesh => <-.Read_UCD.field;
			in_nd => <-.node_vector.out;
		};
		FLD_MAP.node_vector node_vector {
			in_data => <-.interleave_3_arrays.out;
			out {
				node_data = {
					{
						labels="Displacement"
					}
				};
			};
		};
		GEOMS.Arrow1 Arrow1;
		MODS.select_cells select_cells {
			in_field => <-.combine_mesh_data.out;
			obj {
				Props {
					trans = 0.3;
				};
			};
		};
		XP_COMM_PROJ.FilterNodeConnect.FilterNodeMacs.FilterNodeConnect FilterNodeConnect {
			in => <-.select_cells.out_fld;
		};
	};

};