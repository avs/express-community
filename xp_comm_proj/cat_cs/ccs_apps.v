
flibrary CatCSApps
{
	APPS.SingleWindowApp CatCSEg<NEdisplayMode="maximized"> {
		MODS.Read_UCD Read_UCD {
			read_ucd_ui {
				file_browser {
					dirMaskCache = "$XP_PATH<0>/data/ucd/*.inp";
				};
				filename = "$XP_PATH<0>/data/ucd/cells.inp";
			};
		};
		MODS.select_cells select_cells_in {
			in_field => <-.Read_UCD.field;
			sets = {0,1,2,4};
		};
		XP_COMM_PROJ.CatCellSets.CatCSMacs.cat_cell_sets cat_cell_sets {
			in => <-.select_cells_in.out_fld;
		};
		MODS.select_cells select_cells_out {
			in_field => <-.cat_cell_sets.out;
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.select_cells_in.out_obj,
						<-.<-.<-.select_cells_out.out_obj
					};
				};
			};
		};
	};
};
