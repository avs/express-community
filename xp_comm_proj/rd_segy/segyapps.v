
flibrary ReadSegyApps <compile_subs=0> {

	APPS.SingleWindowApp ReadSegyEg1 {
		XP_COMM_PROJ.ReadSegy.ReadSegyMacs.ReadSegY ReadSegY {
			params {
				filename = "";
				timescale = 2;		
				normalize = 0;
			};
			ReadSegyUI {
				SegyFileDialog {
					ok = 1;
					dirMaskCache = "xp_comm_proj/rd_segy/*.segy";
				};
			};
		};
		MODS.bounds bounds {
			in_field => <-.ReadSegY.out;
			UIpanel {
				option {
					set = 1;
				};
			};
		};
		MODS.orthoslice orthoslice {
			in_field => <-.ReadSegY.out;
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.orthoslice.out_obj,<-.<-.<-.bounds.out_obj};
					Xform {
						dcenter = {12.6559,1.52367, 4.65156};
						mat = {
							-0.121997,-0.0366635,0.0105111,0.,
							-0.0380025,0.119843,-0.0230564,0.,
							-0.00324155,-0.0251311,-0.125284,0.,
							0.,0.,0.,1.
						};
						xlate = {-8.65594,-3.60316,-4.33793};
					};
				};
				View {
					View {
						renderer => "Software";
						trigger = 1;
					};
				};
			};
		};
	};

	APPS.SingleWindowApp ReadSegyEg2<NEdisplayMode="maximized"> {
		XP_COMM_PROJ.ReadSegy.ReadSegyMacs.ReadSegY ReadSegY {
			params {
				filename = "";
				timescale = 2;		
				normalize = 0;
			};
			ReadSegyUI {
				SegyFileDialog {
					ok = 1;
					dirMaskCache = "xp_comm_proj/rd_segy/*.segy";
				};
			};
		};
		MODS.orthoslice orthoslice {
			in_field => <-.ReadSegY.out;
			OrthoSliceParam {
				axis = 1;
				plane => 64;
			};
			OrthoSliceUI {
				UIpanel {
					option {
						set = 1;
					};
				};
			};
		};
		MODS.reset_xform reset_xform {
			in_field => <-.orthoslice.out_fld;
		};
		GEOMS.Axis2D Axis2D {
			in_field => <-.reset_xform.out_fld;
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.Axis2D.out_obj,<-.<-.<-.reset_xform.out_obj};
				};
			};
		};
	};
};

