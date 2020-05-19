macro Write_POV_Ray<NEhelpContextID=1,NEhelpFile="STM3/Write_POV_Ray.html"> {
   link molecule_in<NEportLevels={2,1},NEx=55.,NEy=66.>;
   mlink fld<NEportLevels={2,1},NEx=77.,NEy=22.>;
   link step<NEportLevels={2,1},NEx=231.,NEy=55.>;
   XP_COMM_PROJ.STM3.BASE.DisplayParams &display_params<NEx=231.,NEy=22.,NEportLevels={2,1}>;
   XP_COMM_PROJ.STM3.POV.WritePovCore WritePovCore<NEx=33.,NEy=363.> {
      in => <-.molecule_in;
      fld => <-.fld;
      filename => <-.filename.text;
		step => <-.step;
		display_params => <-.display_params;
   };
   UImod_panel UImod_panel<NEx=484.,NEy=22.> {
      title => name_of(<-.<-,1);
   };
   UIlabel title1<NEx=605.,NEy=88.> {
      parent => <-.UImod_panel;
      label => "POV-Ray scene description file";
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UItext filename<NEx=649.,NEy=143.> {
      parent => <-.UImod_panel;
      text<NEportLevels={2,2}> => <-.UIfileDialog.filename;
      y = 40;
      width = 180;
   };
   UIbutton browse_btn<NEx=792.,NEy=143.> {
      parent => <-.UImod_panel;
      height = 30;
      label => "Browse...";
      x = 180;
      y = 40;
      width = 70;
   };
   UIfileDialog UIfileDialog<NEx=792.,NEy=187.> {
      visible => <-.browse_btn.do;
      title => "Select output scene file";
      searchPattern = "*.pov";
   };
   UItoggle animation<NEx=473.,NEy=143.> {
      parent => <-.UImod_panel;
      label => "Animation";
      set => <-.WritePovCore.enable_animation;
      y = 75;
      x = 2;
   };
   UIlabel title4<NEx=473.,NEy=451.> {
      parent => <-.UImod_panel;
      label => "Rendering options";
      y = 110;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
	UIslider transparency<NEx=473.,NEy=572.> {
   	parent => <-.UImod_panel;
   	value => <-.WritePovCore.transparency;
   	title => "Surface transparency";
   	y = 260;
   	max = 1.;
   	min = 0.;
   	width => parent.clientWidth;
   	decimalPoints = 1;
	};
	UIlist bkg_list<NEx=473.,NEy=528.> {
   	parent => <-.UImod_panel;
   	height => 60;
   	strings = {"Sky: Solid blue", "Sky: Sunny day"};
   	y = 200;
   	selectedItem => <-.WritePovCore.background_type;
   	width => parent.clientWidth;
	};
	UIlist finish_list<NEx=473.,NEy=484.> {
   	parent => <-.UImod_panel;
   	height => 60;
   	strings = {"Normal finish", "Phong", "Phong lite"};
   	y = 140;
   	selectedItem => <-.WritePovCore.finish;
   	width => parent.clientWidth;
	};
	UIerrorDialog UIerrorDialog<NEx=297.,NEy=594.> {
   	visible => <-.WritePovCore.err;
   	parent => <-.UImod_panel;
   	title => "Error in Write POV-Ray";
   	message => <-.WritePovCore.err_str;
   	x = 400;
   	y = 400;
	};
};

