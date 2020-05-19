// 
// file: rdtpmods.v
// author: I.Hallstein, DLR, WT-DV
//
// modifications: Tobias Schiebeck, IAC
//
// purpose: High level macro library for read Tecplot-Data-File module
//

flibrary ReadTecplotMacs<compile_subs=0>
{
	macro ReadTecplotUI
	{
		XP_COMM_PROJ.ReadTecplot.ReadTecplotMods.ReadTecplotParams &params;
		UImod_panel Panel
		{
			title => "Read_Tecplot";
			width => parent.clientWidth;
			parent<NEportLevels={3,0}>;
		};
		UIfileDialog FileDialog
		{
			visible => <-.Button.do;
			title => "Tecplot Data Filename";
			filename => <-.params.filename;
			ok = 1;
			searchPattern = "*";
			dirMaskCache = "*";
		};
		UItext Text
		{
			parent => <-.Panel;
			text => <-.params.filename;
			y => <-.Label.height+5;
			width => parent.clientWidth*2/3 - 5;
		};
		UIlabel Label
		{
			parent => <-.Panel;
			label => "Tecplot Data Filename";
			width => parent.clientWidth;
			y = 0;
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
	};
	
	macro ReadTecplotFunc
	{
		XP_COMM_PROJ.ReadTecplot.ReadTecplotMods.ReadTecplotParams &params;
		XP_COMM_PROJ.ReadTecplot.ReadTecplotMods.ReadTecplotCore ReadTecplotCore
		{
			filename => <-.params.filename;
		};
		olink out_mblock => ReadTecplotCore.mb;
		mlink out_fields<NEportLevels={0,2}> => ReadTecplotCore.mb.fields;
	};

	macro ReadTecplot 
	{
		XP_COMM_PROJ.ReadTecplot.ReadTecplotMods.ReadTecplotParams params
		{
			filename = "";
		};
		XP_COMM_PROJ.ReadTecplot.ReadTecplotMacs.ReadTecplotUI ReadTecplotUI
		{
			params => <-.params;
		};
		XP_COMM_PROJ.ReadTecplot.ReadTecplotMacs.ReadTecplotFunc ReadTecplotFunc
		{
			params => <-.params;
		};
		olink out_mblock => ReadTecplotFunc.out_mblock;
		mlink out_fields<NEportLevels={0,2}> => ReadTecplotFunc.out_fields;
	};
};
