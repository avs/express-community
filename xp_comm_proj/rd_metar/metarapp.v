flibrary ReadMETARSApps 
{
	XP_COMM_PROJ.ReadStations.ReadStatApps.ReadStat_Eg ReadMETARS_Eg {
		UI {
			Modules {
				IUI {
					optionList {
						selectedItem = 0;
						cmdList => {
							<-.<-.<-.<-.GISMapTransform.UImod_panel.option,
							<-.<-.<-.<-.text_glyph.UImod_panel.option,
							<-.<-.<-.<-.ReadStations.ReadStatUI.UImod_panel.option,
							<-.<-.<-.<-.GISWorldDataBank.UImod_panel.option,
							<-.<-.<-.<-.ReadMETARS.ReadMETARSUI.UImod_panel.option
						};
					};
				};
			};
		};
		Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.ReadMETARS.obj,<-.<-.<-.text_glyph.out_obj};
					Xform {
						ocenter = {10016.8,3.16217e+06,3.12951e+06};
						dcenter = {9.79545,13.8757,8.74867};
						mat = {
							1.81268e-07,9.45515e-08,-6.51289e-07,0.,
							-5.60581e-07,3.7606e-07,-1.01434e-07,0.,
							3.44738e-07,5.61779e-07,1.77507e-07,0.,
							0.,0.,0.,1.
						};
						xlate = {-9.79577,-13.8763,-8.74884};
					};
				};
				View {
					View {
						trigger = 1;
					};
				};
			};
		};
		GISMapTransform {
			GISMeshProject#2 {
				in => ;
			};
			UImod_panel {
				option {
					set = 1;
				};
			};
		};
		XP_COMM_PROJ.ReadMETARS.ReadMETARSMacs.ReadMETARS ReadMETARS {
			StationLocation => <-.ReadStations.StationLocation;
			StationID => <-.ReadStations.StationID;
			params {
				filename = "xp_comm_proj/rd_metar/data/10Z.txt";
			};
			ReadMETARSUI {
				UIfileDialog {
					ok = 1;
					dirMaskCache = "xp_comm_proj/rd_metar/data/*.txt";
				};
			};
			ReadMETARSFunc {
				ReadMETARSCore {
					out<NEdisplayMode="open"> {
						node_data<NEdisplayMode="open">;
					};
				};
			};
		};
	};
};
