// gam_apps.v
// by James S. Perrin Time-stamp: <Monday 10/09/01 16:00:50 zzcgujp>
library GroupArrManApps {
	APPS.MultiWindowApp GroupArrManEg<NEdisplayMode="maximized"> {
		UI {
			shell {
				x = 11;
				y = 24;
			};
			Modules {
				IUI {
					optionList {
						selectedItem = 0;
					};
					mod_panel {
						x = 0;
						y = 0;
					};
				};
			};
		};
		GDM.Uviewer2D Uviewer2D {
			Scene {
				Top {
					child_objs => {
					<-.<-.<-.GroupArrayManager.obj};
				};
				View {
					View {
						trigger = 1;
					};
				};
			};
		};
		MODS.Read_Image Read_Image;
		XP_COMM_PROJ.GroupArrMan.GroupArrayManager GroupArrayManager {
			grouparrmanParams {
				nGroups = 0;
			};
			in => <-.Read_Image.field;
		};
	};

};
