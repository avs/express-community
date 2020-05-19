flibrary ReadFluentApps<compile_subs=0> {

	APPS.SingleWindowApp ReadFluentEg 
	{
   		GDM.Uviewer3D Uviewer3D 
		{
			Scene 
			{
				Top 
				{
					child_objs => 
					{
						<-.<-.<-.isosurface.out_obj,
						<-.<-.<-.bounds.out_obj
					};
				};
			};
		};
		XP_COMM_PROJ.ReadFluent.ReadFluentMacs.ReadFluent ReadFluent;
		MODS.bounds bounds 
		{
			in_field => <-.ReadFluent.out_mesh;
		};
		MODS.isosurface isosurface 
		{
			in_field => <-.ReadFluent.out_mesh;
		};
	};
};
