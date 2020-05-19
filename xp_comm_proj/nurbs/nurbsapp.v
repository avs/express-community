flibrary NurbsApps {
	APPS.SingleWindowApp ExampleNurbs{
		XP_COMM_PROJ.Nurbs.NurbsMacs.Load_Nurbs Load_Nurbs {
			Load_Nurbs_UI {
				Panel {
					parent => <-.<-.<-.UI.Modules.IUI.mod_panel;
				};
			};
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.Nurbs_to_field.obj_out,
						<-.<-.<-.Nurbs_to_mesh.obj_out};
				};
			};
		};
		XP_COMM_PROJ.Nurbs.NurbsMacs.Nurbs_to_field Nurbs_to_field {
			nurbs_in => <-.Load_Nurbs.out;
		};
		XP_COMM_PROJ.Nurbs.NurbsMacs.Nurbs_to_mesh Nurbs_to_mesh {
			nurbs_in => <-.Load_Nurbs.out;
		};
	};
};
