flibrary CineMacs{

  APPS.SingleWindowApp CineInteractEg {
   
	GDM.Uviewer2D Uviewer2D {
		Scene {
			Top {
				child_objs => {
					<-.<-.<-.Read_Field.out_obj
				};
			};
		}; 
	};
	XP_COMM_PROJ.Cine.CineMacs.CineInteract CineInteract {
		view => <-.Uviewer2D.Scene_Selector.curr_view;
	};
	MODS.Read_Field Read_Field {
		read_field_ui {
			filename = "./xp_comm_proj/cine/mri.fld";
		};
	};
  };


APPS.SingleWindowApp CineLoopEg {
   
	GDM.Uviewer2D Uviewer2D {
		Scene {
			Top {
				child_objs => {
					<-.<-.<-.Read_Field.out_obj
				};
			};
		};      
	};
	XP_COMM_PROJ.Cine.CineMacs.CineLoop CineLoop {
		view => <-.Uviewer2D.Scene_Selector.curr_view;
		end => <-.UIfield.value;
	};
	MODS.Read_Field Read_Field {
		read_field_ui {
			filename = "./xp_comm_proj/cine/mri.fld";
		};
	};
	UImod_panel EndframeUI;
	UIlabel Endframe {
		parent => <-.EndframeUI;
		y = 0;
	};
	UIfield UIfield {
		parent => <-.EndframeUI;
		y => <-.Endframe.y + <-.Endframe.height + 10;
		mode = "integer";
	};
  };

};
