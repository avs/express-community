flibrary CineMacs
<compile_subs=0>
{
  macro CineFunc {
	ilink view;
	olink frame_out=> cache.frame_out;
	XP_COMM_PROJ.Cine.CineMods.Image_Cache cache {
		mode =><-.CineParams.emode;
		view =><-.view;
		capture => <-.CineParams.capture;
		playback => <-.CineParams.playback;
		frame_in => <-.Cine_Control.value;
		frame_out=>frame_in;
		num_frames<NEportLevels={2,0}> => <-.Cine_Control.end;
	};
		XP_COMM_PROJ.Cine.CineMods.Cine_Control Cine_Control {
		mode => <-.CineParams.mode;
		rate => <-.CineParams.rate;
		start => <-.CineParams.start;
		end<NEportLevels={2,2}> => <-.CineParams.end;
		run => <-.CineParams.playback;
		shell_visible = 1;
		true_rate => <-.CineParams.true_rate;
	};
	XP_COMM_PROJ.Cine.CineMods.CineParams &CineParams<NEportLevels={2,0}>;
   };
  
  
  
  
  macro CineInteractUI{
	UImod_panel UImod_panel {
		title => "Cache";
		height = 780;
	};
	UItoggle Capture {
		parent => <-.UImod_panel;
		x = 10;
		y = 0;
		set+Port2=><-.CineParams.capture;
	};
	UItoggle Playback {
		parent => <-.UImod_panel;
		x = 10;
		y => <-.Capture.y + <-.Capture.height + 10;
		set+Port2=><-.CineParams.playback;
	};
	UItoggle Backward {
		parent => <-.UImod_panel;
		x = 10;
		y => <-.Playback.y + <-.Playback.height + 10;
		set+Port2=><-.CineParams.mode;
	};
	UIslider Rate {
		parent => <-.UImod_panel;
		y => <-.Backward.y + <-.Backward.height + 10;
		x = 10;
		width => (parent.clientWidth - 20);
		min = 1.;
		max = 60.;
		decimalPoints = 0;
		value+Port2=><-.CineParams.rate;
	};
	XP_COMM_PROJ.Cine.CineMods.CineParams &CineParams<NEportLevels={2,1}>;
  };
  
  
  
  macro CineInteract {
	XP_COMM_PROJ.Cine.CineMacs.CineFunc CineFunc {
		view => <-.view;
		cache;
		Cine_Control;
		CineParams => <-.CineParams;
	};
		XP_COMM_PROJ.Cine.CineMacs.CineInteractUI CineInteractUI {
		CineParams => <-.CineParams;
	};
		XP_COMM_PROJ.Cine.CineMods.CineParams CineParams {
		emode="interactive";
		start=0;
		mode=0;
	};
	link view<NEportLevels={2,1}>;
   };
  

  macro CineLoopUI {
	XP_COMM_PROJ.Cine.CineMods.CineParams &CineParams<NEportLevels={2,1}>;

	UImod_panel UImod_panel {
		visible<NEportLevels={2,2}>;
		title => "cache";
		height = 780;
	};
	UItoggle Enable {
		parent => <-.UImod_panel;
		x = 10;
		y = 0;
		set+Port2=><-.CineParams.capture;
	};
	UItoggle Backward {
		parent => <-.UImod_panel;
		x = 10;
		y => <-.Enable.y + <-.Enable.height + 10;
		set+Port2=><-.CineParams.mode;
	};
	UIlabel StartFrame {
		parent => <-.UImod_panel;
		x = -12;
		y => <-.Backward.y + <-.Backward.height + 10;
	};
	UIfield UIfield {
		parent => <-.UImod_panel;
		value+Port2=> <-.CineParams.start;
		x = 10;
		y => <-.StartFrame.y + <-.StartFrame.height + 10;
		mode = "integer";
	};
	UIslider Rate {
		parent => <-.UImod_panel;
		y => <-.UIfield.y + <-.UIfield.height + 10;
		x = 10;
		width => (parent.clientWidth - 20);
		min = 1.;
		max = 100.;
		decimalPoints = 0;
		value+Port2 =><-.CineParams.rate;
	};
	UIlabel Actual_Rate {
		parent => <-.UImod_panel;
		label => <-.string;
		y => <-.Rate.y + <-.Rate.height + 10;
		x = 10;
		width => parent.clientWidth;
		alignment = 0;
	};
	string+Port string => str_format("Actual Rate: %g",.CineParams.true_rate); 
   };


  macro CineLoop {
	XP_COMM_PROJ.Cine.CineMacs.CineFunc CineFunc {
		Cine_Control.run=><-.CineParams.capture;
		CineParams => <-.CineParams;
		Cine_Control.end=> <-.CineParams.end;
		view => <-.view;
	};
	XP_COMM_PROJ.Cine.CineMacs.CineLoopUI CineLoopUI {
		CineParams => <-.CineParams;
	};
	XP_COMM_PROJ.Cine.CineMods.CineParams CineParams{
		emode="loop";
		mode=0;
		start=0;
		end=> <-.end;
	};
	ilink view;
	ilink end;
	olink frame_out => CineFunc.frame_out;
  };

};
