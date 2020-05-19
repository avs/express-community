flibrary CoordinatesSliceApps <compile_subs=0> {

	APPS.SingleWindowApp CoordinatesSliceEg<NEdisplayMode="maximized"> {
		GEOMS.Sphere Sphere {
			sphere_ui {
				panel {
					option {
						set = 1;
					};
					height = 984;
				};
			};
			subdiv = 19;
		};
		XP_COMM_PROJ.CoordinatesSlice.CoordinatesSliceMods.Coords_3D Coords_3D {
			xyz_Coords => <-.Sphere.bounds.out.coordinates.values;
			Dimension => <-.Sphere.bounds.out.nnodes;
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.point_mesh.obj};
					Xform {
						ocenter = {0.,-5.21541e-08,0.};
						dcenter = {-125.274,-47.8653,-122.335};
						mat = {
							-0.60384,-14.4311,-3.96195,0.,
							12.7164,1.59316,-7.74866,0.,
							7.88769,-3.67745,12.1895,0.,
							0.,0.,0.,1.
						};
						xlate = {125.274,47.8653,122.335};
					};
				};
				View {
					View {
						trigger = 1;
					};
					ViewUI {
						ViewPanel {
							UI {
								panel {
									defaultX = 653;
									defaultY = -15;
								};
							};
						};
					};
				};
			};
		};
		FLD_MAP.point_mesh point_mesh {
			coord => <-.CoordinatesSlice.xy_out;
		};
		XP_COMM_PROJ.CoordinatesSlice.CoordinatesSliceMacs.CoordinatesSlice CoordinatesSlice {
			params {
				plane = 0.1;
				axis = 1;
				dec = 1;
			};
			Coords_3D => <-.Coords_3D;
		};
	};

};
