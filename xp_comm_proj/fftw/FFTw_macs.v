
//
// Copyright (C) Chris Pudney, The University of Western Australia, 1998.
// All rights reserved.
// 
// Permission to use, copy, modify and distribute this software and its
// documentation only for the purposes of teaching and research is hereby
// granted without fee, provided that the above copyright notice and this
// permission notice appear in all copies of this software/documentation and
// that you do not sell the software.  No commercial use or distribution
// of the software is permitted without the consent of the copyright
// owners.  Commercial licensing is available by contacting the author(s).
//
// THIS SOFTWARE/DOCUMENTATION IS PROVIDED WITH NO WARRANTY, EXPRESS OR
// IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTY OF MERCHANTABILITY OR
// FITNESS FOR A PARTICULAR PURPOSE.
//
flibrary FFTwMacs
{
  // FFT
  // Functional Macro - Contains low-level module FFTw_raw and parameter
  // group FFTw_params
  //
  macro FFT
    {
      link in<NEportLevels={2,1},NEx=70.,NEy=75.>;
      link out<NEportLevels={1,2},NEx=350.,NEy=435.> => .FFTw_raw.out_field;
      XP_COMM_PROJ.FFTw.FFTwMods.FFTw_params &FFTw_params<NEx=560.,NEy=75.,
      NEportLevels={2,1}>;
      XP_COMM_PROJ.FFTw.FFTwMods.FFTw_raw FFTw_raw<NEx=350.,NEy=255.>
	{
	  in_field => <-.in;
	  FFTw_params => <-.FFTw_params;
	};
    };


  // FFTw_UI
  // UI Macro - Contains parameter group FFTw_params
  //
  macro FFTw_UI
    {
      link parent<NEportLevels={2,1},NEx=35.,NEy=120.>;
      XP_COMM_PROJ.FFTw.FFTwMods.FFTw_params &FFTw_params<NEportLevels={2,1},
      NEx=243.,NEy=120.>;
      UIradioBoxLabel rescale_radioboxlabel<NEx=125.,NEy=390.> {
	parent<NEx=14.,NEy=39.> => <-.parent;
	labels<NEx=182.,NEy=39.> => <-.rescale_choices;
	selectedItem<NEx=350.,NEy=39.> => <-.FFTw_params.rescale;
	title<NEx=518.,NEy=39.> => "Rescaling";
	label_cmd<NEx=350.,NEy=255.> {
	  cmd[3] = {
	    {
	      set=1,,,,,do=1,,,,,,,,,,,,,,,
	    },
	    {
	      do=1,,,,,,,,,,,,,,,
	    },
	    {
	      do=1,,,,,,,,,,,,,,,
	    }};
	};
	selectedString<NEx=560.,NEy=363.>;
	height<NEx=686.,NEy=147.>;
	UIradioBox<NEx=560.,NEy=471.>;
	UIlabel<NEx=140.,NEy=471.>;
	UIpanel<NEx=140.,NEy=363.>;
	x => ;
	y => ;
      };
      string rescale_choices<NEportLevels=1,NEx=455.,NEy=120.>[] =
	{"None",
	 "Product of Dimensions","Sqrt of Product of Dimensions"};
      string direction_choices<NEportLevels=1,NEx=662.,NEy=120.>[] =
	{"Forward",
	 "Inverse"};
      UIradioBoxLabel direction_radioboxlabel {
	parent<NEx=14.,NEy=39.> => <-.parent;
	labels<NEx=182.,NEy=39.> => <-.direction_choices;
	selectedItem<NEx=350.,NEy=39.> => <-.FFTw_params.direction;
	title<NEx=518.,NEy=39.> => "Direction";
	label_cmd<NEx=350.,NEy=255.> {
	  cmd[3] = {
	    {
	      set=1,,,,,do=1,,,,,,,,,,,,,,,
	    },
	    {
	      do=1,,,,,,,,,,,,,,,
	    },
	    {
	      do=1,,,,,,,,,,,,,,,
	    }};
	};
	selectedString<NEx=560.,NEy=363.>;
	height<NEx=686.,NEy=147.>;
	UIradioBox<NEx=560.,NEy=471.>;
	UIlabel<NEx=140.,NEy=471.>;
	UIpanel<NEx=140.,NEy=363.>;
	x => ;
	y => ;
      };
    };


  // FFTw
  // User Macro - contains Functional Macro FFT, UI Macro FFTw_UI and
  // parameter group FFTw_params
  //
  macro FFTw
    {
      link in<NEportLevels={2,1},NEx=70.,NEy=75.>;
      link out<NEportLevels={1,2},NEx=350.,NEy=435.> => .FFT.out;
      XP_COMM_PROJ.FFTw.FFTwMacs.FFT FFT<NEx=140.,NEy=255.>
	{
	  in<NEx=140.,NEy=75.> => <-.in;
	  FFTw_params<NEx=560.,NEy=75.> => <-.FFTw_params;
	};

      XP_COMM_PROJ.FFTw.FFTwMods.FFTw_params FFTw_params<NEx=350.,NEy=75.,
      NEportLevels={0,1}>;

      XP_COMM_PROJ.FFTw.FFTwMacs.FFTw_UI FFTw_UI<NEx=560.,NEy=255.>
	{
	  parent => <-.UImod_panel;
	  FFTw_params<NEportLevels={2,1},NEx=243.,NEy=120.> => <-.FFTw_params;
	};

      UImod_panel UImod_panel<NEx=630.,NEy=75.>
	{
	  parent<NEportLevels={3,0}>;
	  title => name_of(<-.<-);
	};
    };


  // FFTwEg
  // Example Application - contains User Macro FFTw
  //
  APPS.MultiWindowApp FFTwEg<NEdisplayMode="maximized"> {
    UI {
      shell {
	x = 0;
	y = 0;
      };
      Modules {
	IUI {
	  panel {
	    visible = 1;
	  };
	  optionList {
	    selectedItem = 4;
	    cmdList => {
	      <-.<-.<-.<-.extract_scalar.ExtractScalarUI.UIpanel.option,
	      <-.<-.<-.<-.FFTw.UImod_panel.option,
	      <-.<-.<-.<-.FFTw#1.UImod_panel.option,
	      <-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
	      <-.<-.<-.<-.orthoslice.OrthoSliceUI.UIpanel.option,
	      <-.<-.<-.<-.bounds.UIpanel.option
	      };
	  };
	  mod_panel {
	    x = 0;
	    y = 0;
	  };
	};
      };
      Editors {
	IUI {
	  optionList {
	    selectedItem = 0;
	  };
	};
      };
    };
    MODS.extract_scalar extract_scalar {
      in_field => <-.FFTw#1.out;
      ExtractScalarUI {
	DVnode_data_labels {
	  labels[];
	};
	UIradioBoxLabel {
	  label_cmd {
	    cmd[];
	  };
	};
      };
    };
    XP_COMM_PROJ.FFTw.FFTwMacs.FFTw FFTw {
      in => <-.Read_Field.field;
      FFTw_params {
	rescale = 2;
      };
      FFTw_UI {
	rescale_radioboxlabel {
	  label_cmd {
	    cmd = {
	      {
		set=0,,,,,,,,,,,,,,,,,,,,
	      },
	      ,
	      {
		set=1,,,,,,,,,,,,,,,,,,,,
	      }};
	  };
	  x = 0;
	  y = 76;
	};
	direction_radioboxlabel {
	  label_cmd {
	    cmd[2];
	  };
	  x = 0;
	  y = 0;
	};
      };
    };
    XP_COMM_PROJ.FFTw.FFTwMacs.FFTw FFTw#1 {
      in => <-.FFTw.out;
      FFTw_params {
	direction = 1;
	rescale = 2;
      };
      FFTw_UI {
	rescale_radioboxlabel {
	  label_cmd {
	    cmd = {
	      {
		set=0,,,,,,,,,,,,,,,,,,,,
	      },
	      ,
	      {
		set=1,,,,,,,,,,,,,,,,,,,,
	      }};
	  };
	  x = 0;
	  y = 76;
	};
	direction_radioboxlabel {
	  label_cmd {
	    cmd[2] = {
	      {
		set=0,,,,,,,,,,,,,,,,,,,,
	      },
	      {
		set=1,,,,,,,,,,,,,,,,,,,,
	      }};
	  };
	  x = 0;
	  y = 0;
	};
      };
    };
    MODS.Read_Field Read_Field {
      read_field_ui {
	file_browser {
	  ok = 1;
	  dirMaskCache = "/usr/express/data/field/*.fld";
	};
	filename = "/usr/express/data/field/hydrogen.fld";
      };
      DVread_field {
	Mesh_Unif+Node_Data Output_Field;
      };
    };
    MODS.orthoslice orthoslice {
      in_field => <-.extract_scalar.out_fld;
      OrthoSliceParam {
	axis = 2;
      };
      OrthoSliceUI {
	UIpanel {
	  option {
	    set = 1;
	  };
	};
      };
    };
    MODS.bounds bounds {
      in_field => <-.extract_scalar.out_fld;
    };
    GDM.Uviewer3D Uviewer3D {
      Scene {
	Top {
	  child_objs => {
	    <-.<-.<-.orthoslice.out_obj,<-.<-.<-.bounds.out_obj};
	  Xform {
	    mat = {
	      5.04454,-2.019,2.03439,0.,2.86283,3.3496,-3.77449,0.,
	      0.138972,4.28559,3.90857,0.,0.,0.,0.,1.
	      };
	    xlate = {-4.7446,-4.68566,
		     -5.};
	  };
	};
	Camera {
	  Camera {
	    perspec = 1;
	    front = 1.;
	  };
	};
      };
      Scene_Editor {
	Camera_Editor {
	  GDcamera_edit {
	    front = 1.;
	  };
	};
      };
    };
  };
};

//
// Copyright (C) Chris Pudney, The University of Western Australia, 1998.
// All rights reserved.
// 
// Permission to use, copy, modify and distribute this software and its
// documentation only for the purposes of teaching and research is hereby
// granted without fee, provided that the above copyright notice and this
// permission notice appear in all copies of this software/documentation and
// that you do not sell the software.  No commercial use or distribution
// of the software is permitted without the consent of the copyright
// owners.  Commercial licensing is available by contacting the author(s).
//
// THIS SOFTWARE/DOCUMENTATION IS PROVIDED WITH NO WARRANTY, EXPRESS OR
// IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTY OF MERCHANTABILITY OR
// FITNESS FOR A PARTICULAR PURPOSE.
//
flibrary FFTwMacs
{
  // FFT
  // Functional Macro - Contains low-level module FFTw_raw and parameter
  // group FFTw_params
  //
  macro FFT
    {
      link in<NEportLevels={2,1},NEx=70.,NEy=75.>;
      link out<NEportLevels={1,2},NEx=350.,NEy=435.> => .FFTw_raw.out_field;
      XP_COMM_PROJ.FFTw.FFTwMods.FFTw_params &FFTw_params<NEx=560.,NEy=75.,
      NEportLevels={2,1}>;
      XP_COMM_PROJ.FFTw.FFTwMods.FFTw_raw FFTw_raw<NEx=350.,NEy=255.>
	{
	  in_field => <-.in;
	  FFTw_params => <-.FFTw_params;
	};
    };


  // FFTw_UI
  // UI Macro - Contains parameter group FFTw_params
  //
  macro FFTw_UI
    {
      link parent<NEportLevels={2,1},NEx=35.,NEy=120.>;
      XP_COMM_PROJ.FFTw.FFTwMods.FFTw_params &FFTw_params<NEportLevels={2,1},
      NEx=243.,NEy=120.>;
      UIradioBoxLabel rescale_radioboxlabel<NEx=125.,NEy=390.> {
	parent<NEx=14.,NEy=39.> => <-.parent;
	labels<NEx=182.,NEy=39.> => <-.rescale_choices;
	selectedItem<NEx=350.,NEy=39.> => <-.FFTw_params.rescale;
	title<NEx=518.,NEy=39.> => "Rescaling";
	label_cmd<NEx=350.,NEy=255.> {
	  cmd[3] = {
	    {
	      set=1,,,,,do=1,,,,,,,,,,,,,,,
	    },
	    {
	      do=1,,,,,,,,,,,,,,,
	    },
	    {
	      do=1,,,,,,,,,,,,,,,
	    }};
	};
	selectedString<NEx=560.,NEy=363.>;
	height<NEx=686.,NEy=147.>;
	UIradioBox<NEx=560.,NEy=471.>;
	UIlabel<NEx=140.,NEy=471.>;
	UIpanel<NEx=140.,NEy=363.>;
	x => ;
	y => ;
      };
      string rescale_choices<NEportLevels=1,NEx=455.,NEy=120.>[] =
	{"None",
	 "Product of Dimensions","Sqrt of Product of Dimensions"};
      string direction_choices<NEportLevels=1,NEx=662.,NEy=120.>[] =
	{"Forward",
	 "Inverse"};
      UIradioBoxLabel direction_radioboxlabel {
	parent<NEx=14.,NEy=39.> => <-.parent;
	labels<NEx=182.,NEy=39.> => <-.direction_choices;
	selectedItem<NEx=350.,NEy=39.> => <-.FFTw_params.direction;
	title<NEx=518.,NEy=39.> => "Direction";
	label_cmd<NEx=350.,NEy=255.> {
	  cmd[3] = {
	    {
	      set=1,,,,,do=1,,,,,,,,,,,,,,,
	    },
	    {
	      do=1,,,,,,,,,,,,,,,
	    },
	    {
	      do=1,,,,,,,,,,,,,,,
	    }};
	};
	selectedString<NEx=560.,NEy=363.>;
	height<NEx=686.,NEy=147.>;
	UIradioBox<NEx=560.,NEy=471.>;
	UIlabel<NEx=140.,NEy=471.>;
	UIpanel<NEx=140.,NEy=363.>;
	x => ;
	y => ;
      };
    };


  // FFTw
  // User Macro - contains Functional Macro FFT, UI Macro FFTw_UI and
  // parameter group FFTw_params
  //
  macro FFTw
    {
      link in<NEportLevels={2,1},NEx=70.,NEy=75.>;
      link out<NEportLevels={1,2},NEx=350.,NEy=435.> => .FFT.out;
      XP_COMM_PROJ.FFTw.FFTwMacs.FFT FFT<NEx=140.,NEy=255.>
	{
	  in<NEx=140.,NEy=75.> => <-.in;
	  FFTw_params<NEx=560.,NEy=75.> => <-.FFTw_params;
	};

      XP_COMM_PROJ.FFTw.FFTwMods.FFTw_params FFTw_params<NEx=350.,NEy=75.,
      NEportLevels={0,1}>;

      XP_COMM_PROJ.FFTw.FFTwMacs.FFTw_UI FFTw_UI<NEx=560.,NEy=255.>
	{
	  parent => <-.UImod_panel;
	  FFTw_params<NEportLevels={2,1},NEx=243.,NEy=120.> => <-.FFTw_params;
	};

      UImod_panel UImod_panel<NEx=630.,NEy=75.>
	{
	  parent<NEportLevels={3,0}>;
	  title => name_of(<-.<-);
	};
    };


  // FFTwEg
  // Example Application - contains User Macro FFTw
  //
  APPS.MultiWindowApp FFTwEg<NEdisplayMode="maximized"> {
    UI {
      shell {
	x = 0;
	y = 0;
      };
      Modules {
	IUI {
	  panel {
	    visible = 1;
	  };
	  optionList {
	    selectedItem = 4;
	    cmdList => {
	      <-.<-.<-.<-.extract_scalar.ExtractScalarUI.UIpanel.option,
	      <-.<-.<-.<-.FFTw.UImod_panel.option,
	      <-.<-.<-.<-.FFTw#1.UImod_panel.option,
	      <-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
	      <-.<-.<-.<-.orthoslice.OrthoSliceUI.UIpanel.option,
	      <-.<-.<-.<-.bounds.UIpanel.option
	      };
	  };
	  mod_panel {
	    x = 0;
	    y = 0;
	  };
	};
      };
      Editors {
	IUI {
	  optionList {
	    selectedItem = 0;
	  };
	};
      };
    };
    MODS.extract_scalar extract_scalar {
      in_field => <-.FFTw#1.out;
      ExtractScalarUI {
	DVnode_data_labels {
	  labels[];
	};
	UIradioBoxLabel {
	  label_cmd {
	    cmd[];
	  };
	};
      };
    };
    XP_COMM_PROJ.FFTw.FFTwMacs.FFTw FFTw {
      in => <-.Read_Field.field;
      FFTw_params {
	rescale = 2;
      };
      FFTw_UI {
	rescale_radioboxlabel {
	  label_cmd {
	    cmd = {
	      {
		set=0,,,,,,,,,,,,,,,,,,,,
	      },
	      ,
	      {
		set=1,,,,,,,,,,,,,,,,,,,,
	      }};
	  };
	  x = 0;
	  y = 76;
	};
	direction_radioboxlabel {
	  label_cmd {
	    cmd[2];
	  };
	  x = 0;
	  y = 0;
	};
      };
    };
    XP_COMM_PROJ.FFTw.FFTwMacs.FFTw FFTw#1 {
      in => <-.FFTw.out;
      FFTw_params {
	direction = 1;
	rescale = 2;
      };
      FFTw_UI {
	rescale_radioboxlabel {
	  label_cmd {
	    cmd = {
	      {
		set=0,,,,,,,,,,,,,,,,,,,,
	      },
	      ,
	      {
		set=1,,,,,,,,,,,,,,,,,,,,
	      }};
	  };
	  x = 0;
	  y = 76;
	};
	direction_radioboxlabel {
	  label_cmd {
	    cmd[2] = {
	      {
		set=0,,,,,,,,,,,,,,,,,,,,
	      },
	      {
		set=1,,,,,,,,,,,,,,,,,,,,
	      }};
	  };
	  x = 0;
	  y = 0;
	};
      };
    };
    MODS.Read_Field Read_Field {
      read_field_ui {
	file_browser {
	  ok = 1;
	  dirMaskCache = "/usr/express/data/field/*.fld";
	};
	filename = "/usr/express/data/field/hydrogen.fld";
      };
      DVread_field {
	Mesh_Unif+Node_Data Output_Field;
      };
    };
    MODS.orthoslice orthoslice {
      in_field => <-.extract_scalar.out_fld;
      OrthoSliceParam {
	axis = 2;
      };
      OrthoSliceUI {
	UIpanel {
	  option {
	    set = 1;
	  };
	};
      };
    };
    MODS.bounds bounds {
      in_field => <-.extract_scalar.out_fld;
    };
    GDM.Uviewer3D Uviewer3D {
      Scene {
	Top {
	  child_objs => {
	    <-.<-.<-.orthoslice.out_obj,<-.<-.<-.bounds.out_obj};
	  Xform {
	    mat = {
	      5.04454,-2.019,2.03439,0.,2.86283,3.3496,-3.77449,0.,
	      0.138972,4.28559,3.90857,0.,0.,0.,0.,1.
	      };
	    xlate = {-4.7446,-4.68566,
		     -5.};
	  };
	};
	Camera {
	  Camera {
	    perspec = 1;
	    front = 1.;
	  };
	};
      };
      Scene_Editor {
	Camera_Editor {
	  GDcamera_edit {
	    front = 1.;
	  };
	};
      };
    };
  };
};
