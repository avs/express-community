
flibrary BodyOfRevolutionApps {

	// Example Applications (SingleWindowApp)

	APPS.SingleWindowApp BodyOfRevolutionEg <NEdisplayMode="maximized"> {
		float+OPort r_outside[] = {2.,5.,3.,4.,7.,1.,2.,2.};
		float+OPort z_outside[] = {-0.5,1.,3.,6.,7.,9.,9.5,10.};
		float+OPort r_inside[] = {1.5,4.5,2.5,3.5,6.5,0.5,1.5,1.5};
		IAC.MAPPERS.BodyOfRevolution BodyOfRevolution {
			r_outside => <-.r_outside;
			z_outside => <-.z_outside;
			r_inside => <-.r_inside;
			params {
				nose_cap_on = 1;
			};
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => { <-.<-.<-.BodyOfRevolution.obj};
					Xform {
						ocenter = {0,0.,5.7499};
						dcenter = {0.0425846,0.00514413,2.89372};
						mat = {
							0.0102605,0.467069,0.0543638,0.,
							0.169608,-0.0543915,0.435302,0.,
							0.438571,0.0101097,-0.169617,0.,
							0.,0.,0.,1.
						};
						xlate = {-0.0425845,-0.00514412,-2.89372};
					};
				};
			};
		};
	};

};

