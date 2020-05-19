// by James S Perrin  Time-stamp: <Monday 14/05/01 15:10:17 zzcgujp>

flibrary IsoObjsReduceMacs 
{
	macro IsoObjsReduceUI 
	{
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMods.IsoObjsReduceParams &params; 
		UImod_panel UImod_panel 
		{
			option 
			{
				set = 1;
			};
			title => "Isosurface Obj Reduce";
		};
		UIslider UInObjs 
		{
			parent => <-.UImod_panel;
			title => "Number of Objs";
			y = 0;
			width => parent.clientWidth;
			max => <-.params.totObjs;
			min = 0.;
			mode = "integer";
			value => <-.params.nObjs;
		};
	};
	macro IsoObjsReduceFunc 
	{
		ilink in;
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMods.IsoObjsReduceParams &params; 
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMods.iso_objs_reduce iso_objs_reduce 
		{
			in_mesh => <-.in;
			get_objs => <-.params.nObjs;
			tot_objs => <-.params.totObjs;
		};
		olink out => iso_objs_reduce.out_mesh;
	};
	macro IsoObjsReduce 
	{
		ilink in;
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMods.IsoObjsReduceParams params; 
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMacs.IsoObjsReduceUI iso_objs_reduce_ui
		{
			params => <-.params;
		}; 
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMacs.IsoObjsReduceFunc iso_objs_reduce_func
		{
			in => <-.in;
			params => <-.params;
		}; 
		GDM.DataObject DataObject 
		{
			in => <-.iso_objs_reduce_func.out;
		};
		olink out => iso_objs_reduce_func.out;
		olink obj => DataObject.obj;
	};
	
	macro IsoObjsReduce2UI 
	{
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMods.IsoObjsReduce2Params &params; 
		UImod_panel UImod_panel 
		{
			option 
			{
				set = 1;
			};
			title => "Isosurface Obj Reduce2";
		};
		UIslider UInObjs 
		{
			width => parent.clientWidth;
			parent => <-.UImod_panel;
			title => "Number of Objs to List";
			y = 0;
			width => parent.clientWidth;
			max => <-.params.totObjs;
			min = 0.;
			mode = "integer";
		};
		int objs<NEportLevels=1>[] => init_array(.UInObjs.value,0,(.UInObjs.value - 1));
		UImultiList UIobjList 
		{
			parent => <-.UImod_panel;
			height => (10 * UIdata.UIfonts[0].lineHeight);
			selectedItems=><-.params.show_objs;
			selectedText[0];
			strings => <-.objs;
			y => ((<-.UInObjs.y + <-.UInObjs.height) + 4);
			width => parent.clientWidth;
		};
		UIlabel UIlabel 
		{
			parent => <-.UImod_panel;
			y => <-.UIobjList.y + <-.UIobjList.height +4;
			width => parent.clientWidth;
			alignment = "center";
			label = "Selected Objs To View";
		};
	};

	macro IsoObjsReduce2Func 
	{
		ilink in;
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMods.IsoObjsReduce2Params &params; 
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMods.iso_objs_reduce2 iso_objs_reduce2 
		{
			in_mesh => <-.in;
			show_objs => <-.params.show_objs;
			tot_objs => <-.params.totObjs;
		};
		olink out => .iso_objs_reduce2.out_mesh;
	};

	macro IsoObjsReduce2 
	{
		ilink in;
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMods.IsoObjsReduce2Params params; 
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMacs.IsoObjsReduce2UI iso_objs_reduce2_ui
		{
			params => <-.params; 
		};
		XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMacs.IsoObjsReduce2Func iso_objs_reduce2_func
		{
			in => <-.in;
			params => <-.params; 
		};
		GDM.DataObject DataObject 
		{
			in => <-.iso_objs_reduce2_func.out;
		};
		olink out => .iso_objs_reduce2_func.out;
		olink obj => DataObject.obj;
	};
};
