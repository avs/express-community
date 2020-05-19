flibrary SeededThreshold_Macs<compile_subs=0>
{
	macro SeededThresholdUI
	{
		XP_COMM_PROJ.SeededThreshold.SeededThreshold_Mods.SeededThresholdParams &params;

    	UImod_panel UImod_panel
		{
	      	parent<NEportLevels={4,0}>;
			title => name_of(<-.<-.<-,1);
		};
		UIlabel minlabel
		{
			parent => <-.UImod_panel;
			label => "Minimum:";
			x = 10;
			y = 10;
			width => parent.clientWidth/3;
			alignment = 2;
		};
		UIfield minfield
		{
			parent => <-.UImod_panel;
			value => <-.params.min;
			y => (minlabel.y - 2);
			x => ((minlabel.x + minlabel.width) + 5);
			decimalPoints = 1;
		};
		UIlabel maxlabel
		{
			parent => <-.UImod_panel;
			label => "Maximum:";
			x = 10;
			y => ((minlabel.y + minlabel.height) + 15);
			width => minlabel.width;
			alignment = 2;
		};
		UIfield maxfield
		{
			parent => <-.UImod_panel;
			value => <-.params.max;
			x => minfield.x;
			y => (maxlabel.y - 2);
			decimalPoints = 1;
		};
		UIlabel seedlabel
		{
			parent => <-.UImod_panel;
			label => "Seed:";
			x = 10;
			y => ((maxlabel.y + maxlabel.height) + 15);
			width => parent.clientWidth;
			alignment = "left";
		};
		UIslider x 
		{
			parent => <-.UImod_panel;
			x = 10;
			y => ((seedlabel.y + seedlabel.height));
			value => params.seed_x;
			width => parent.clientWidth-20;
			max => params.seed_max_x;
			mode = "integer";
		};
		UIslider y
		{
			parent => <-.UImod_panel;
			x = 10;
			y => ((<-.x.y + <-.x.height));
			value => params.seed_y;
			width => parent.clientWidth-20;
			max => params.seed_max_y;
			mode = "integer";
		};
		UIslider z
		{
			parent => <-.UImod_panel;
			x = 10;
			y => ((<-.y.y + <-.y.height));
			value => params.seed_z;
			width => parent.clientWidth-20;
			max => params.seed_max_z;
			mode = "integer";
		};
	};
	
	macro SeededThresholdFunc
	{
		ilink in_fld;
		ilink seed => {params.seed_x,params.seed_y,params.seed_z};
		XP_COMM_PROJ.SeededThreshold.SeededThreshold_Mods.SeededThresholdParams &params;
		
		XP_COMM_PROJ.SeededThreshold.SeededThreshold_Mods.SeededThresholdCore SeededThresholdCore
		{
			Input_Field => <-.in_fld;
			seed => <-.seed;
			min => <-.params.min;
			max => <-.params.max;
		};
		olink out_fld => SeededThresholdCore.Output_Field;
	};
	
	macro SeededThreshold
	{
		ilink in_fld;
		XP_COMM_PROJ.SeededThreshold.SeededThreshold_Mods.SeededThresholdParams params
		{
			min = 0.0;
			max = 255.0;
			seed_max_x+nres => <-.in_fld.dims[0];
			seed_max_y+nres => <-.in_fld.dims[1];
			seed_max_z+nres => <-.in_fld.dims[2];
			seed_x = 0;
			seed_y = 0;
			seed_z = 0;
		};
		XP_COMM_PROJ.SeededThreshold.SeededThreshold_Macs.SeededThresholdUI SeededThresholdUI
		{
			params => <-.params;
		};
		XP_COMM_PROJ.SeededThreshold.SeededThreshold_Macs.SeededThresholdFunc SeededThresholdFunc
		{
			in_fld => <-.in_fld;
			params => <-.params;
		};
		GDM.DataObject DataObject
		{
			in => <-.SeededThresholdFunc.out_fld;
		};
		olink out_fld => SeededThresholdFunc.out_fld;
		olink out_obj => DataObject.obj;
    };
};
