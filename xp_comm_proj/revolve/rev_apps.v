flibrary RevolveApps <compile_subs=0>{
	APPS.SingleWindowApp RevolveEg<NEdisplayMode="maximized"> {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.revolve.obj};
					Xform {
						ocenter = {2.5,0.,0.};
						dcenter = {3.33333,5.35161e-07,-2.62936e-06};
						mat = {
							-0.808674,0.259571,-0.219269,0.,
							-0.33316,-0.494431,0.6434,0.,
							0.0667978,0.676441,0.554414,0.,
							0.,0.,0.,1.
						};
						xlate = {-3.33333,-7.15256e-07,2.86102e-06};
						center = {2.5,0.,0.};
					};
				};
			};
		};
		XP_COMM_PROJ.Revolve.RevolveMacs.revolve revolve {
			RevolveParams {
				resolution = 30;
			};
			in => <-.int;
		};
		int+OPort int [][] = {0,0,2,2,3,1,4,3,5,1};
	};
};
