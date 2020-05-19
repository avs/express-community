
flibrary SelectColumnMacs {

	macro SelectColumnUI {

		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1); 
		};
		UIoptionBoxLabel Columns {
			parent => <-.UImod_panel;
			labels => <-.params.labels;
			selectedItems => <-.params.selected;
			UIoptionBox {
				height => max_array({1,((.dims + 1) * UIdata.UIfonts[0].lineHeight)});
			};
		};
		XP_COMM_PROJ.SelectColumn.SelectColumnMods.SelectColumnParams+IPort2 &params; 
	};

	macro SelectColumnFunc {
 		ilink Columns_In;
		XP_COMM_PROJ.SelectColumn.SelectColumnMods.SelectColumnParams+IPort2 &params; 
		XP_COMM_PROJ.SelectColumn.SelectColumnMods.SelectColumnCore SelectColumnCore {
			Columns_In => <-.Columns_In;
			selected => <-.params.selected;
			labels => <-.params.labels;
		};
		olink Columns_Out => SelectColumnCore.Columns_Out;
	};

	macro SelectColumn {
		ilink Columns_In;

		XP_COMM_PROJ.SelectColumn.SelectColumnMods.SelectColumnParams params;
                
		XP_COMM_PROJ.SelectColumn.SelectColumnMacs.SelectColumnFunc SelectColumnFunc {
			Columns_In => <-.Columns_In;
			params => <-.params;
		};
		XP_COMM_PROJ.SelectColumn.SelectColumnMacs.SelectColumnUI SelectColumnUI {
			params => <-.params;
		};
		olink Columns_Out => SelectColumnFunc.Columns_Out;
	};

};  


