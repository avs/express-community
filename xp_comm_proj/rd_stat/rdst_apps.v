
flibrary ReadStatApps{

	APPS.SingleWindowApp ReadStat_Eg{
		UI {
		  shell {
			 x = 158;
			 y = 39;
		  };
		  Modules {
			 IUI {
				optionList {
					selectedItem = 1;
				};
			 };
		  };
		};
		GDM.Uviewer3D Uviewer3D {
		  Scene {
			 Top {
				child_objs => {
					<-.<-.<-.GISMapTransform.MapObject.obj,
					<-.<-.<-.GISMapTransform.MapObject#2.obj,
					<-.<-.<-.GISMapTransform.MapObject#3.obj,
					<-.<-.<-.GISMapTransform.MapObject#4.obj,
					<-.<-.<-.text_glyph.out_obj};
				Xform {
					ocenter = {-7527.75,-22902.3,-7067.};
					dcenter = {7.35207,7.25595,6.69787};
					mat = {
					  -5.05696e-007,2.7143e-008,5.71024e-007,0.,
					  5.00437e-007,-3.47483e-007,4.59711e-007,0.,
					  2.7632e-007,6.78982e-007,2.12433e-007,0.,
					  0.,0.,0.,1.
					};
					xlate = {-7.35207,-7.25595,-6.69786};
				};
			 };
		  };
		  Scene_Editor {
			 View_Editor {
				InfoPanel {
					curobj_sel {
					  width = 238;
					  height = 245;
					  x = 0;
					  y = 0;
					};
				};
			 };
		  };
		};
		XP_COMM_PROJ.ReadStations.ReadStatMacs.ReadStations ReadStations {
			ReadStatUI {
				FileDialog {
					dirMaskCache = "xp_comm_proj/rd_stat/*.txt";
					ok = 1;
				};
			};
			ReadStatParams {
				filename = "xp_comm_proj/rd_stat/nsd_bbss.txt";
				StartLatitude = 2.82;
				StartLongitude = -157.38;
				EndLatitude = 5.13;
				EndLongitude = 175.76;
			};
		};
		GIS_UI.GISWorldDataBank GISWorldDataBank {
		  west_longitude {
			 value = 0.;
		  };
		  east_longitude {
			 value = 359.9999;
		  };
		  south_latitude {
			 value = -90.;
		  };
		  north_latitude {
			 value = 90.;
		  };
		  geographic {
			 List {
				selectedItem = 0;
			 };
			 selectedText = "All";
		  };
		  political {
			 List {
				selectedItem = 0;
			 };
			 selectedText = "All";
		  };
		  UImod_panel {
			 option {
				set = 1;
			 };
		  };
		  ingest {
		 	do = 1;
		  };
		};
		GIS_UI.GISMapTransform GISMapTransform {
			GISMeshProject {
				in => <-.<-.GISWorldDataBank.GISCIAMapII.geoPolylines;
			};
			GISMeshProject#2 {
				in => <-.<-.GISWorldDataBank.GISCIAMapII.polPolylines;
			};
			GISMeshProject#3 {
				in => <-.<-.GISWorldDataBank.GISCIAMapII.coastalPolylines;
			};
			GISMeshProject#4 {
				in => <-.<-.ReadStations.StationLocation;
			};
		};
		MODS.text_glyph text_glyph {
			in_mesh => <-.GISMapTransform.GISMeshProject#4.out;
			in_text => <-.ReadStations.StationID;
			TextUI {
				Underline {
					set = 1;
				};
				Leadline {
					set = 1;
				};
				Offset {
					set = 1;
				};
				Font {
					text = "-adobe-times-bold-r-*-*-10-*-*-*-*-*-*-*";
				};
			};
		};
	};

};
