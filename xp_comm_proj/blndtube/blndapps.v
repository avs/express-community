
flibrary BlendyTubesApps <compile_subs=0> {

  // example application
  APPS.SingleWindowApp SimpleTubesEg {
    GDM.Uviewer3D Uviewer3D {
      Scene {
        Top {
          child_objs => {
            <-.<-.<-.bounds.out_obj,
            <-.<-.<-.Line3D.out_obj,
            <-.<-.<-.simple_tubes.out_obj
          };
          Xform {
            mat = {
              0.320307,0.0733338,-0.148807,0.,
              -0.0235839,0.340404,0.116991,0.,
              0.164211,-0.0941555,0.307063,0.,
              0.,0.,0.,1.
            };
            xlate = {-1.18071,-1.50195,-1.04656};
            center = {3.27323,4.16378,2.90132};
          };
        };
        Camera {
          Camera {
            perspec = 1;
            front = 1.;
            auto_norm = "None";
          };
        };
      };
    };


    MODS.Read_Field Read_Field {
      read_field_ui {
        file_browser {
          dirMaskCache = "$XP_ROOT/data/field/*.fld";
        };
        filename = "$XP_ROOT/data/field/bluntfin.fld";
      };
    };


    MODS.bounds bounds {
      in_field => <-.Read_Field.field;
    };

    MODS.combine_vect combine_vect {
      in_field => <-.Read_Field.field;
      CombineVectParam {
        components = {1,2,3};
      };
    };

    GEOMS.Line3D Line3D {
      line3 {
        xform {
          mat = {
            2.11102,-0.0167101,-5.29822,0.,
            4.76114,-2.49615,1.9049,0.,
            -2.32444,-5.12804,-0.90997,0.,
            0.,0.,0.,1.
          };
          xlate = {-6.95756,0.929703,5.48915};
        };
	   };
    };

    MODS.streamlines streamlines {
      in_field => <-.combine_vect.out_fld;
      in_probe => <-.Line3D.out_fld;
    };


    XP_COMM_PROJ.BlendyTubes.BlendyTubesMacs.simple_tubes simple_tubes {
      lines => <-.streamlines.out_fld;

      params {
        scale = 0.3;
        facets = 6;
      };
    };

  };



  // example application
  APPS.SingleWindowApp BlendyTubesEg {
    GDM.Uviewer3D Uviewer3D {
      Scene {
        Top {
          child_objs => {
            <-.<-.<-.bounds.out_obj,
            <-.<-.<-.Line3D.out_obj,
            <-.<-.<-.blendy_tubes.out_obj
          };
          Xform {
            mat = {
              0.320307,0.0733338,-0.148807,0.,
              -0.0235839,0.340404,0.116991,0.,
              0.164211,-0.0941555,0.307063,0.,
              0.,0.,0.,1.
            };
            xlate = {-1.18071,-1.50195,-1.04656};
            center = {3.27323,4.16378,2.90132};
          };
        };
        Camera {
          Camera {
            perspec = 1;
            front = 1.;
            auto_norm = "None";
          };
        };
      };
    };


    MODS.Read_Field Read_Field {
      read_field_ui {
        file_browser {
          dirMaskCache = "$XP_ROOT/data/field/*.fld";
        };
        filename = "$XP_ROOT/data/field/bluntfin.fld";
      };
    };


    MODS.bounds bounds {
      in_field => <-.Read_Field.field;
    };

    MODS.combine_vect combine_vect {
      in_field => <-.Read_Field.field;
      CombineVectParam {
        components = {1,2,3};
      };
    };

    GEOMS.Line3D Line3D {
      line3 {
        xform {
          mat = {
            2.11102,-0.0167101,-5.29822,0.,
            4.76114,-2.49615,1.9049,0.,
            -2.32444,-5.12804,-0.90997,0.,
            0.,0.,0.,1.
          };
          xlate = {-6.95756,0.929703,5.48915};
        };
	   };
    };

    MODS.streamlines streamlines {
      in_field => <-.combine_vect.out_fld;
      in_probe => <-.Line3D.out_fld;
    };


    XP_COMM_PROJ.BlendyTubes.BlendyTubesMacs.blendy_tubes blendy_tubes {
      lines => <-.streamlines.out_fld;
      data => <-.Read_Field.field;

      params {
        scale = 2.50;
        facets = 6;
      };

      BlendyTubesUI {
        Scale_Component {
          selectedItem = 0.;
        };
        Colour_Component {
          selectedItem = 4.;
        };
      };
    };

  };

};

