//
flibrary DVdownsize_scatApps<compile_subs=0>
{
	APPS.SingleWindowApp Oriented_StreamlineEg{
		MODS.Read_Field Read_Field 
		{
			read_field_ui 
			{
				filename = "$XP_PATH<0>/data/field/wind.fld";
			};
		};
		MODS.combine_vect combine_vect 
		{
			in_field => <-.Read_Field.field;
		};
		GEOMS.FPoint1D FPoint1D
		{
			in => <-.combine_vect.out_fld;
			point1 
			{
				xform 
				{
					xlate = {10.,10.,20.};
				};
			};
			probe_ui 
			{
				probe_edit 
				{
					GDxform_editor 
					{
						x_trans = 10.;
						y_trans = 10.;
						z_trans = 20.;
						abs_x_trans = 10.;
						abs_y_trans = 10.;
						abs_z_trans = 20.;
					};
				};
			};
		};
		MODS.bounds bounds
		{
			in_field => <-.combine_vect.out_fld;
		};
		GEOMS.Arrow1 Arrow1
		{
			glyph1 
			{
				coordinates 
				{
					values = {0.,0.,0.,0.35,0.,0.,-0.35,0.15,0.,-0.35,-0.15,0.};
				};
			};
			set1 
			{
				node_connect_list[4] = {1,2,1,3};
				ncells = 2;
			};
		};
		XP_COMM_PROJ.DVdownsize_scat.DVdownsize_scatMacs.Oriented_Streamline Oriented_Streamline
		{
			in_field => <-.combine_vect.out_fld;
			in_probe => <-.FPoint1D.out_fld;
			in_glyph => <-.Arrow1.out_fld;
			streamlines 
			{
				StreamParam 
				{
					forw_back = 0;
				};
			};
			glyph 
			{
				GlyphParam 
				{
					scale = 0.1;
				};
			};
		}; 
		GDM.Uviewer3D Uviewer3D
		{
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.Oriented_Streamline.out_obj,
						<-.<-.<-.bounds.out_obj
					};
				};
			};
		};
	};
};
