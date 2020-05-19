//
flibrary DVdownsize_scatMacs<compile_subs=0>
{
	macro DVdownsize_scat_func
	{
		ilink in;
		XP_COMM_PROJ.DVdownsize_scat.DVdownsize_scatMods.DVdownsizeParams &params;
		XP_COMM_PROJ.DVdownsize_scat.DVdownsize_scatMods.DVdownsize_core Downsize_core
		{
			in => <-.in;
			one_factor => <-.params.factor; 
		};
		olink out => Downsize_core.out;
	};

	macro DVdownsize_scat_UI 
	{
		XP_COMM_PROJ.DVdownsize_scat.DVdownsize_scatMods.DVdownsizeParams &params;
		UImod_panel Panel
		{
			title => "DVdownsize_scat";
			width => parent.clientWidth;
			parent<NEportLevels={3,0}>;
		};
		UIslider nleveles 
		{
			parent => <-.Panel;
			title = "Downsize factor";
			min = 1;
			max = 100;
			mode = 1;
			value+IPort2 => params.factor;
			width+nres	=> parent.clientWidth;
		};
	};
	
	macro DVdownsize_scat {
		ilink in;
		XP_COMM_PROJ.DVdownsize_scat.DVdownsize_scatMods.DVdownsizeParams params
		{
			factor = 10;
		};
		XP_COMM_PROJ.DVdownsize_scat.DVdownsize_scatMacs.DVdownsize_scat_UI DVdownsize_scat_UI 
		{
			params => <-.params;
		};
		XP_COMM_PROJ.DVdownsize_scat.DVdownsize_scatMacs.DVdownsize_scat_func DVdownsize_scat_func 
		{
			in => <-.in;
			params => <-.params;
		};
		olink out => DVdownsize_scat_func.out;
		GDM.DataObject DataObject 
		{
			in => <-.DVdownsize_scat_func.out;
		};
		olink obj_out => .DataObject.obj;
	};
	
	macro Oriented_Streamline 
	{
		ilink in_field;
		ilink in_probe;
		ilink in_glyph;
		MODS.streamlines streamlines 
		{
			in_field => <-.in_field;
			in_probe => <-.in_probe;
		};
		MODS.interp_data interp_data 
		{
			in_field => <-.in_field;
			in_probe => <-.DVdownsize_scat.out;
		};
		MODS.glyph glyph 
		{
			in_field => <-.interp_data.out_fld;
			in_glyph => <-.in_glyph;
		};
		XP_COMM_PROJ.DVdownsize_scat.DVdownsize_scatMacs.DVdownsize_scat DVdownsize_scat 
		{
			in => <-.streamlines.out_fld;
		};
		olink out_strline =>streamlines.out_fld;
		olink out_glyph => glyph.out_fld;
		GDM.GroupObject GroupObject 
		{
			child_objs => {<-.streamlines.out_obj,<-.glyph.out_obj};
		};
		olink out_obj => GroupObject.obj;
	};
	
};
