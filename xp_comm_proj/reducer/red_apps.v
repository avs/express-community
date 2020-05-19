flibrary ReducerApps 
{
	APPS.SingleWindowApp ReducerEg<NEdisplayMode="maximized"> {
		XP_COMM_PROJ.Reducer.ReducerMacs.Reducer Reducer {
			in => <-.extract_data_array.data;
			params {
				factor = 12;
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				filename = "$XP_PATH<0>/data/field/cylinder.fld";
				panel {
					option {
						set = 1;
					};
				};
			};
		};
		FLD_MAP.extract_data_array extract_data_array {
			in => <-.Read_Field.field;
			UIpanel {
				option {
					set = 1;
				};
			};
		};
		link+Port in => .extract_data_array.data;
		link+Port out => .Reducer.out;
	};
};
