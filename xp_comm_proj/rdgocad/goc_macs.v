
flibrary GocadMacs {

  macro GocadUI {
    XP_COMM_PROJ.ReadGocad.GocadMods.GocadParams &parameters<NEportLevels={2,1}>;

    UIslider x_scale {
      y = 0;
      parent => <-.gocad;
      min = 1.;
      max = 10.;
      value => <-.parameters.x;
    };

    UIslider y_scale {
      y = 60;
      parent => <-.gocad;
      min = 1.;
      max = 10.;
      value => <-.parameters.y;
    };

    UIslider z_scale {
      y = 120;
      parent => <-.gocad;
      min = 0.01;
      max = 1.;
      value => <-.parameters.z;
    };

    UImod_panel gocad {
      title = "ReadGOCAD";
    };
  
    UItoggle scale {
      parent => <-.gocad;
      set<NEportLevels={2,2}> => <-.parameters.scale;
      y = 180;
      active = 1;
    };

    UIfileDialog UIfileDialog {
      visible => <-.open_file.do;
      parent => <-.gocad;
      filename<NEportLevels={2,2}> => <-.parameters.file_name;
      dirMaskCache = "./*";
    };

    UIbutton open_file {
      parent => <-.gocad;
      label => "Browse...";
      y = 204;
    };

    UItext file_name {
      parent => <-.gocad;
      text <NEportLevels={2,2}> => <-.UIfileDialog.filename;
      y = 228;
      width = 250;
    };
  };


  macro Gocad {
    XP_COMM_PROJ.ReadGocad.GocadMods.GocadParams &GocadParams<NEportLevels={2,1}>;

    XP_COMM_PROJ.ReadGocad.GocadMods.ReadGOCAD ReadGOCAD {
      group_ref => <-.GocadParams;
    };

    GDM.DataObject DataObject {
      in => <-.ReadGOCAD.Mesh;
      Obj {
         name => name_of(<-.<-.<-);
      };
    };

    olink out_mesh => .ReadGOCAD.Mesh;
    olink out_obj => .DataObject.obj;
  };


  macro read_gocad {
    XP_COMM_PROJ.ReadGocad.GocadMods.GocadParams GocadParams {
      x = 1;
      y = 1;
      z = 1;
      scale = 1;
    };

    XP_COMM_PROJ.ReadGocad.GocadMacs.GocadUI GocadUI {
      parameters => <-.GocadParams;
      gocad {
        title => name_of(<-.<-.<-);
      };
    };
    XP_COMM_PROJ.ReadGocad.GocadMacs.Gocad Gocad {
      GocadParams => <-.GocadParams;
      DataObject {
         Obj {
            name => name_of(<-.<-.<-.<-);
         };
      };
    };

    olink out_mesh => .Gocad.out_mesh;
    olink out_obj => .Gocad.out_obj;
  };


  APPS.SingleWindowApp ReadGocadEg {
    GDM.Uviewer3D Uviewer3D {
      Scene {
        Top {
          child_objs => {
            <-.<-.<-.read_gocad.out_obj
          };
          Xform {
            mat = {
              0.879135,-0.00703489,-0.0695449,0.,
              0.00737698,0.881869,0.00404469,0.,
              0.0695095,-0.00461348,0.879153,0.,
              0.,0.,0.,1.
            };
          };
        };
      };
    };

    XP_COMM_PROJ.ReadGocad.GocadMacs.read_gocad read_gocad {
      GocadUI {
        UIfileDialog {
          dirMaskCache = "./xp_comm_proj/rdgocad/*";
        };
      };
      GocadParams {
        z = 1.;
        file_name = "./xp_comm_proj/rdgocad/worldmap.tsurf";
      };
    };
  };

};

