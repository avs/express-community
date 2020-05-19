flibrary Vector3dFontsApps <compile_subs=0> {

	// Vector3dFontsEg - Example-Application
	APPS.SingleWindowApp Vector3dFontsEg<NEdisplayMode="maximized"> {
		UI {
			Modules {
				IUI {
					optionList {
						selectedItem = 0;
					};
				};
			};
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {<-.<-.<-.Vector3dFonts.obj};
					Xform {
						mat = {
							0.996771,-0.0594922,0.0539295,0.,
							0.0799963,0.793867,-0.602807,0.,
							-0.00695049,0.605175,0.796062,0.,
							0.,0.,0.,1.
						};
					};
				};
				View {
					View {
						trigger = 1;
					};
				};
			};
		};
		XP_COMM_PROJ.Vector3dFonts.Vector3dFontsMacs.Vector3dFonts Vector3dFonts {
			params {
				label = "hello";
				kerning = 0.8;
				stroke_width = 0.5;
				font = 7;
				linestyle = 2;
			};
		};
	};

};
