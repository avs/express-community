
flibrary SphereSurfaceApps {

  // SphereSurfaceEg
  //   Example Applications (SingleWindowApp)

  APPS.SingleWindowApp SphereSurfaceEg1 {
    GDM.Uviewer3D Uviewer3D {
      Scene {
        Top {
          child_objs => {
            <-.<-.<-.sphere_surface.out_obj
          };
          Props {
            material = {0.,0.702,0.4,50.};
            col = {1.,0.928,0.28};
          };
          Xform {
            mat = {
               4.,0.,0.,0.,
               0.,4.,0.,0.,
               0.,0.,4.,0.,
               0.,0.,0.,1.
            };
          };
        };
        Lights {
          LightXform = {
            { mat={0.670865,-0.576382,-0.46661,0.,
                   0.101795,0.694828,-0.711936,0.,
                   0.73456,0.430114,0.524808,0.,
                   0.,0.,0.,1.},},,,
          };
        };
        View {
          View {
            trigger = 2;
          };
        };
      };
    };

    XP_COMM_PROJ.SphereSurface.SphereSurfaceMacs.sphere_surface sphere_surface {
      SphereSurfaceParams {
        longdim = 50;
        latdim = 50;
      };
    };
  };



  APPS.SingleWindowApp SphereSurfaceEg2 {
    GDM.Uviewer3D Uviewer3D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.sphere_surface_data.out_obj
            };
            Xform {
               mat = {
                  4.,0.,0.,0.,
                  0.,4.,0.,0.,
                  0.,0.,4.,0.,
                  0.,0.,0.,1.
               };
            };
         };
         Camera {
            Camera {
               perspec = 1;
               front = 1.;
            };
         };
         View {
            View {
               trigger = 2;
            };
         };
      };
    };


    MODS.Read_Image Read_Image {
      read_image_ui {
        file_browser {
          filename = "$XP_ROOT/data/image/avs.x";
          dirMaskCache = "$XP_ROOT/data/image/*";
        };
        flip_toggle {
          set = 1;
        };
      };
    };

    MODS.extract_scalar extract_scalar {
      in_field => <-.Read_Image.field;
      ExtractScalarParam {
        component = 1;
      };
    };

    MODS.downsize downsize {
      in_field => <-.extract_scalar.out_fld;
      DownsizeParam {
        factor0 = 2.;
        factor1 = 2.;
      };
    };


    XP_COMM_PROJ.SphereSurface.SphereSurfaceMacs.sphere_surface_data sphere_surface_data {
      in => <-.downsize.out_fld;
    };
  };

};

