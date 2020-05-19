flibrary NurbsMacs 
{
	macro Load_Nurbs_UI
	{
		XP_COMM_PROJ.Nurbs.NurbsMods.Load_Params &Load_Params<NEportLevels={2,0}>;
		UIfileDialog FileDialog
		{
			visible => <-.Button.do;
			title => "NURBS Data Filename";
			filename => <-.Load_Params.Filename;
			ok = 1;
			searchPattern = "*";
			dirMaskCache = "*";
		};
		UItext Text
		{
			parent => <-.Panel;
			text => <-.Load_Params.Filename;
			y => <-.Label.height+5;
			width => parent.clientWidth*2/3 - 5;
		};
		UIlabel Label
		{
			parent => <-.Panel;
			label => "NURBS Data Filename";
			width => parent.clientWidth;
			alignment = "left";
		};
		UIbutton Button
		{
			parent => <-.Panel;
			height => <-.Text.height;
			label => "Browse...";
			x => (<-.Text.width)+5;
			y => <-.Label.height+5;
			width => parent.clientWidth*1/3 - 5;
		};
		UImod_panel Panel
		{
			message = "Select read nurbs control panel.";
			title = "Load_Nurbs";
            width => parent.clientWidth;
            parent<NEportLevels={3,0}>;
		};
	};
	macro Load_Nurbs_func 
	{
		XP_COMM_PROJ.Nurbs.NurbsMods.Load_Params &Load_Params<NEportLevels={2,0}>;
		XP_COMM_PROJ.Nurbs.NurbsMods.rd_nurbs rd_nurbs{
			filename=> <-.Load_Params.Filename;
		};
		XP_COMM_PROJ.Nurbs.NurbsMods.nurbs_type out<NEportLevels={1,2}> => rd_nurbs.nurbs; 
	};
	macro Load_Nurbs
	{
		XP_COMM_PROJ.Nurbs.NurbsMods.Load_Params Load_Params;
		XP_COMM_PROJ.Nurbs.NurbsMacs.Load_Nurbs_UI Load_Nurbs_UI
		{
			Load_Params => <-.Load_Params;
		};
		XP_COMM_PROJ.Nurbs.NurbsMacs.Load_Nurbs_func Load_Nurbs_func
		{
			Load_Params => <-.Load_Params;
		};
		XP_COMM_PROJ.Nurbs.NurbsMods.nurbs_type out<NEportLevels={1,2}> => .Load_Nurbs_func.out;
	};

	macro Nurbs_to_field_UI
	{
		XP_COMM_PROJ.Nurbs.NurbsMods.Field_Params &Field_Params <NEportLevels={2,0}>;
		UImod_panel panel
		{
			parent<NEportLevels={3,0}>;
			message = "Select nurbs to field control panel.";
			title => "Nurbs_To_Field";
		};
		UIslider UIslider {
			y = 60;
			width => parent.clientWidth;
			parent => <-.panel;
			min = 2.;
			max = 255.;
			value => <-.Field_Params.nu;
			mode = "integer";
			immediateMode = 1;
			title => "U subdivisions";
		};
		UIslider UIslider1 {
			y = 0;
			width => parent.clientWidth;
			parent => <-.panel;
			min = 2.;
			max = 255.;
			value => <-.Field_Params.nv;
			mode = "integer";
			immediateMode = 1;
			title => "V subdivisions";
		};
	};
	macro Nurbs_to_field_func
	{
		ilink nurbs_in;
		XP_COMM_PROJ.Nurbs.NurbsMods.Field_Params &Field_Params<NEportLevels={2,0}>;
		NurbsMods.nurbs_to_field nurbs_to_field
		{
			nu=> <-.Field_Params.nu;
			nv=> <-.Field_Params.nv;
			nurbs => <-.nurbs_in;
		};
		olink out => .nurbs_to_field.out;
	};
	macro Nurbs_to_field
	{
		XP_COMM_PROJ.Nurbs.NurbsMods.nurbs_type nurbs_in<NEportLevels={2,1}>;
		XP_COMM_PROJ.Nurbs.NurbsMods.Field_Params Field_Params
		{
			nu=12;
			nv=12;
		};
		XP_COMM_PROJ.Nurbs.NurbsMacs.Nurbs_to_field_UI Nurbs_to_field_UI
		{
			Field_Params => <-.Field_Params;
		};
		
		XP_COMM_PROJ.Nurbs.NurbsMacs.Nurbs_to_field_func Nurbs_to_field_func
		{
			nurbs_in => <-.nurbs_in;
			Field_Params => <-.Field_Params;
		};
		olink out => Nurbs_to_field_func.out;
		GDM.DataObject DataObject {
			in => <-.Nurbs_to_field_func.out;
		};
		olink obj_out => DataObject.obj;
	};
	
	macro Nurbs_to_mesh_UI
	{
		XP_COMM_PROJ.Nurbs.NurbsMods.Mesh_Params &Mesh_Params<NEportLevels={2,0}>;
		UImod_panel panel 
		{
			parent<NEportLevels={3,0}>;
			message = "Select nurbs to mesh control panel.";
			title => "Nurbs_To_Mesh";
		};
		UItoggle UItoggle 
		{
			width => parent.clientWidth;
			parent => <-.panel;
			label => "Show Control Mesh";
			set => <-.Mesh_Params.do;
		};
	};
	macro Nurbs_to_mesh_func
	{
		XP_COMM_PROJ.Nurbs.NurbsMods.nurbs_type nurbs_in<NEportLevels={2,1}>;
		XP_COMM_PROJ.Nurbs.NurbsMods.Mesh_Params+IPort2 &Mesh_Params;
		XP_COMM_PROJ.Nurbs.NurbsMods.nurbs_ctrl_to_mesh nurbs_ctrl_to_mesh
		{
			nurbs => <-.nurbs_in;
			show_ctrl=><-.Mesh_Params.do;
		};
		olink out => .nurbs_ctrl_to_mesh.out2;
		
	};
	macro Nurbs_to_mesh
	{
		XP_COMM_PROJ.Nurbs.NurbsMods.nurbs_type nurbs_in<NEportLevels={2,1}>;
		XP_COMM_PROJ.Nurbs.NurbsMods.Mesh_Params Mesh_Params;
		XP_COMM_PROJ.Nurbs.NurbsMacs.Nurbs_to_mesh_UI Nurbs_to_mesh_UI
		{
			Mesh_Params => <-.Mesh_Params;
		};
		XP_COMM_PROJ.Nurbs.NurbsMacs.Nurbs_to_mesh_func Nurbs_to_mesh_func
		{
			Mesh_Params => <-.Mesh_Params;
            nurbs_in => <-.nurbs_in;
		};
		olink mesh_out => .Nurbs_to_mesh_func.out;
		GDM.DataObject DataObject 
		{
			in => <-.Nurbs_to_mesh_func.out;
		};
		olink obj_out => .DataObject.obj;
	};
};
