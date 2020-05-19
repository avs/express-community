
flibrary ReadTinApps {

	APPS.SingleWindowApp ReadTinEg {
   	GDM.Uviewer3D Uviewer3D {
      	Scene {
         	Top {
            	child_objs => {
               	<-.<-.<-.read_tin.out_obj
            	};
         	};
         	Camera {
            	Camera {
               	perspec = 1;
               	front = 1.;
            	};
         	};
      	};
   	};


   	XP_COMM_PROJ.ReadTin.ReadTinMacs.read_tin read_tin {
      	params {
         	filename = "$XP_PATH<1>/xp_comm_proj/rd_tin/data/tahoetin.net";
         	ZScale = 1.;
         	ZeroElevations = 1;
      	};
   	};
	};

};

