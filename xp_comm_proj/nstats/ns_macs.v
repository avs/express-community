
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
flibrary NStatsMacs
{
  // nstats
  // Function Macro - contains low-level module nstats_raw
  //
  macro nstats
    {
      link in_fld<NEportLevels={2,1},NEx=350.,NEy=75.>;
      link info<NEportLevels={1,2},NEx=350.,NEy=435.>;
      
      XP_COMM_PROJ.NodeStats.NStatsMods.nstats_raw nstats_raw
	{
	  input => <-.in_fld;
	};
    };

  // nstat_UI
  // UI Macro
  //
  macro nstats_UI
    {
      UItext UItext
	{
	  parent => <-.parent;
	  text => <-.info;
	  outputOnly = 1;
	  width => parent.width;
	  height => (parent.height * 0.5);
	};
      link parent<NEportLevels={2,1}>;
      string label<NEportLevels=1> = "Node Statistics";
      link info<NEportLevels={2,1}>;
      UIlabel UIlabel
	{
	  parent => <-.parent;
	  label => <-.label;
	};
    };

  // node_statistics
  // User Macro - contains Functional Macro nstats and UI Macro nstats_UI
  //
  macro node_statistics
    {
      XP_COMM_PROJ.NodeStats.NStatsMacs.nstats nstats<NEx=350.,NEy=187.>
	{
	  in_fld => <-.in_fld;
	  info => .nstats_raw.info;

	  XP_COMM_PROJ.NodeStats.NStatsMods.nstats_raw nstats_raw
	    {
	      input => <-.in_fld;
	    };
	};

      link in_fld<NEportLevels={2,1},NEx=140.,NEy=52.>;
      link text<NEportLevels={1,1},NEx=350.,NEy=322.> => .nstats.info;

      XP_COMM_PROJ.NodeStats.NStatsMacs.nstats_UI nstats_UI<NEx=350.,NEy=457.>
	{
	  parent => <-.UImod_panel;
	  info => <-.text;
	};

      UImod_panel UImod_panel<NEx=560.,NEy=52.>
	{
	  title => name_of(<-.<-);
	};
    };

  // Node_Stats_Eg
  // Example application - contain User Macro node_statistics
  //
  APPS.MultiWindowApp Node_Stats_Eg <NEdisplayMode="maximized"> {
    UI {
      shell {
	x = 0;
	y = 0;
      };
      Modules {
        IUI {
          optionList {
            selectedItem = 0;
          };
        };
      };
    };
    GDM.Uviewer2D Uviewer2D {
      Scene {
	Top {
	  child_objs => {
	    <-.<-.<-.Read_Image.image};
	};
      };
    };
    MODS.Read_Image Read_Image {
      read_image_ui {
	file_browser {
	  filename = "/usr/express/data/image/mandrill.x";
	  ok = 1;
	  dirMaskCache = "/usr/express/data/image/*";
	};
      };
    };
    XP_COMM_PROJ.NodeStats.NStatsMacs.node_statistics node_statistics {
      in_fld => <-.Read_Image.field;
      nstats_UI {
	UItext {
	  y = 24;
	};
	UIlabel {
	  y = 0;
	};
      };
      UImod_panel {
	option {
	  set = 1;
	};
      };
    };
  };
};
