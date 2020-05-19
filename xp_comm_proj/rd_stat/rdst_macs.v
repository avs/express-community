
flibrary ReadStatMacs {

  // ReadStatUI
  //   UI Macro
  //   UImod_panel containing a label and two dials

  macro ReadStatUI {
    XP_COMM_PROJ.ReadStations.ReadStatMods.ReadStatParams
      &Params <NEportLevels={2,1}>;

    UI.UImod_panel UImod_panel {
      title = "Read Stations";
    };

    UIbutton BrowseButton<NEx=11.,NEy=55.> {
         parent => UImod_panel;
         label => "Browse...";
         y = 50;
         width => parent.clientWidth;
    };

    UIfileDialog FileDialog<NEx=11.,NEy=88.> {
        searchPattern = "*.txt";
        filename => <-.Params.filename;
        visible => <-.BrowseButton.do;
    };

    UItext FilenameText<NEx=11.,NEy=121.> {
        parent => <-.UImod_panel;
        y => ((BrowseButton.y + BrowseButton.height) + 3);
        width => parent.clientWidth;
        text => FileDialog.filename;
    };

    UIdial UIdial_StartLat {
      parent => <-.UImod_panel;
      value => <-.Params.StartLatitude;
      title = "Start Latitude";
      y = FilenameText.y + FilenameText.height+10;
	  x = 0;
      width = 100;
	  height = 150;
	  min = -90.0;
	  max = 90.0;
    };

    UIdial UIdial_StartLon {
      parent => <-.UImod_panel;
      value => <-.Params.StartLongitude;
      title = "Start Longtitude";
      y = FilenameText.y + FilenameText.height+10;
      x = 120;
      width = 100;
	  height = 150;
  	  min = -180.0;
      max = 359.999;
    };

    UIdial UIdial_EndLat {
      parent => <-.UImod_panel;
      value => <-.Params.EndLatitude;
      title = "End Latitude";
      y = UIdial_StartLat.y + UIdial_StartLat.height;
	  x = 0;
      width = 100;
	  height = 150;
	  min = -90.0;
	  max = 90.0;
    };

    UIdial UIdial_EndLong {
      parent => <-.UImod_panel;
      value => <-.Params.EndLongitude;
      title = "End Longitude";
      y = UIdial_StartLon.y + UIdial_StartLon.height;
	  x = 120;
      width = 100;
	  height = 150;
	  min = -180.0;
	  max = 359.999;
    };

    Controls.UIlabel UIlabel {
      parent => <-.UImod_panel;
      label = "ReadStations Module controls:";
      y = 10;
      width = 200;
    };
  };

  //   ReadStatFunc
  //   Functional macro (Field I/O)

  macro ReadStatFunc {

    XP_COMM_PROJ.ReadStations.ReadStatMods.ReadStatParams
        &Params <NEportLevels={2,1}>;

    XP_COMM_PROJ.ReadStations.ReadStatMods.read_Stations
        read_Stations {
      Params => <-.Params;
    };

    olink StationLocation => read_Stations.StationLocation;
	olink StationID       => read_Stations.StationID;
  };

  //   ReadStations
  //   User Macro (Field I/O and ThresholdUI)

  macro ReadStations {

    XP_COMM_PROJ.ReadStations.ReadStatMods.ReadStatParams
        ReadStatParams;

    XP_COMM_PROJ.ReadStations.ReadStatMacs.ReadStatFunc ReadStatFunc{
      &Params => <-.ReadStatParams;
    };

    XP_COMM_PROJ.ReadStations.ReadStatMacs.ReadStatUI
        ReadStatUI {
      &Params => <-.ReadStatParams;
      UImod_panel {
        title => name_of(<-.<-.<-);
      };
    };

    olink StationLocation => ReadStatFunc.StationLocation;
	olink StationID       => ReadStatFunc.StationID;
  };
};