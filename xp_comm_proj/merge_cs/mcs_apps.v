flibrary MergeCellSetsApps {
	APPS.SingleWindowApp MergeCellSetsEg<NEdisplayMode="maximized"> {
		MODS.Read_UCD Read_UCD {
			read_ucd_ui {
				file_browser {
					dirMaskCache = "$XP_PATH<0>/data/ucd/*.inp";
				};
				filename = "$XP_PATH<0>/data/ucd/cells.inp";
			};
		};
		MODS.select_cells cells_before {
			in_field => <-.Read_UCD.field;
		};
		XP_COMM_PROJ.MergeCellSets.MergeCellSetsMacs.MergeCellSets MergeCellSets {
			in => <-.cells_before.out_fld;
		};
		MODS.select_cells cells_after {
			in_field => <-.MergeCellSets.out;
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => { <-.<-.<-.cells_after.out_obj };
				};
			};
		};
	};
};
