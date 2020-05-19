flibrary ReadVRMLApps 
{
	APPS.SingleWindowApp ReadVRMLEg <NEdisplayMode="maximized"> {
		XP_COMM_PROJ.ReadVRML.ReadVRMLMacs.ReadVRML ReadVRML {
			params {
				filename = "xp_comm_proj/rd_vrml/data/vrml1eg.wrl";
				Subdivision = 52;
			};
			ReadVRMLUI {
				UIfileDialog {
					dirMaskCache = "xp_comm_proj/rd_vrml/data/*.wrl";
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
