
flibrary SelectColumnApps {
	APPS.MultiWindowApp SelectColumnEg<NEdisplayMode="maximized"> {
		UI {
			Modules {
				IUI {
					optionList {
						selectedItem = 1;
					};
				};
			};
		};
		MODS.Read_Column_File Read_Column_File<NEx=66.,NEy=88.> {
			param {
				filename = "xp_comm_proj/selcol/AudienceRate.dat";
				delim_ind = 1;
				header = 1;
				skip_lines = 2;
				columns = {
					{type=5,name="Time",use=1,null_value=0.},
					{name="RTBF1",type=4,use=1,null_value=0.},
					{name="RTL-TVI",type=4,use=1,null_value=0.},
					{name="CLUB-RTL",type=4,use=1,null_value=0.},
					{name="21",type=4,use=1,null_value=0.},
					{name="TF1",type=4,use=1,null_value=0.},
					{name="France2",type=4,use=1,null_value=0.},
					{name="France3",type=4,use=1,null_value=0.},
					{name="TOTAL-TV",type=4,use=1,null_value=0.}
				};
				ncol = 9;
			};
			DVtable_read_init {
				out[0];
			};
			DVtable_read {
				out[];
			};
			read_table_ui {
				file_browser {
					ok = 1;
				};
				radio_box_delim {
					label_cmd {
						cmd[] = {
							,
							{
								set=1
							},,,,
						};
					};
					int temp;
				};
				option_menu_column {
					label_cmd {
						cmd[];
					};
					int temp;
				};
				column_set {
					radio_box_type {
						label_cmd {
							cmd[];
						};
						int temp;
					};
				};
			};
		};
		XP_COMM_PROJ.SelectColumn.SelectColumnMacs.SelectColumn SelectColumn<NEx=341.,NEy=209.> {
			Columns_In => <-.Read_Column_File.table;
			SelectColumnFunc {
				SelectColumnCore<NEdisplayMode="open"> {
					labels[];
				};
			};
			SelectColumnUI {
				UImod_panel {
					option {
						set = 1;
					};
				};
				UIoptionBoxLabel {
					label_cmd {
						cmd[];
					};
				};
			};
			params<NEdisplayMode="open"> {
				selected = {0};
				labels[];
			};
		};
	};
};
