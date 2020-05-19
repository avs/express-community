flibrary ReadPolygonApps {
	APPS.SingleWindowApp ReadPolygonEg <NEdisplayMode="maximized"> {
		XP_COMM_PROJ.ReadPolygon.ReadPolygonMacs.ReadPolygon ReadPolygon {
			params {
				filename = "./xp_comm_proj/rd_poly/test.poly";
			};
		};
		GDM.Uviewer3D Uviewer3D {
		Scene {
			Top {
				child_objs => {
					<-.<-.<-.ReadPolygon.obj};
				};
			};
		};
	};	
};
