flibrary ReadVRMLApps 
{
	APPS.SingleWindowApp ReadVRMLEg <NEdisplayMode="maximized"> {
		XP_COMM_PROJ.ReadVRML1.ReadVRMLMacs.ReadVRML ReadVRML<NEx=341.,NEy=55.> {
			params {
				filename = "xp_comm_proj/rd_vrml1/data/vrml1eg.wrl";
				Subdivision = 52;
			};
			ReadVRMLUI {
				UIfileDialog {
					dirMaskCache = "xp_comm_proj/rd_vrml1/data/*.wrl";
				};
			};
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.ReadVRML.obj};
				};
			};
		};
	};
};
