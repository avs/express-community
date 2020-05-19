
flibrary DatamapFunctionApps <compile_subs=0> {

   // DatamapFunctionEg
   //   Example Application (SingleWindowApp)

   APPS.SingleWindowApp DatamapFunctionEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.datamap_function_isosurface.out_obj,
                  <-.<-.<-.datamap_function_orthoslice.out_obj
               };
               Xform {
                  mat = {
                     0.0324233,0.0211879,-0.0689506,0.,
                     -0.00141428,0.0757682,0.0226179,0.,
                     0.0721188,-0.00803985,0.0314425,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-4.,-4.,-4.};
                  center = {31.5,31.5,31.5};
               };
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,
               };
            };
         };
      };


      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/hydrogen.fld";
         };
      };

      MODS.bounds bounds {
         in_field => <-.Read_Field.field;
      };
      MODS.isosurface isosurface {
         in_field => <-.Read_Field.field;
         IsoParam {
            iso_level => 200.0;
         };
      };
      MODS.orthoslice orthoslice {
         in_field => <-.Read_Field.field;
         OrthoSliceParam {
            plane => 30;
         };
      };


      XP_COMM_PROJ.DatamapFunction.DatamapFunctionMacs.datamap_function datamap_function_isosurface {
         in => <-.isosurface.out_fld;
         params {
            datamap_function = 1;
         };
      };
      XP_COMM_PROJ.DatamapFunction.DatamapFunctionMacs.datamap_function datamap_function_orthoslice {
         in => <-.orthoslice.out_fld;
         params {
            datamap_function = 6;
         };
      };
   };



   APPS.SingleWindowApp InterpolateDataEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.datamap_function.out_obj
               };
               Xform {
                  mat = {
                     -0.0305013,-0.00846298,0.102594,0.,
                     -0.000475415,-0.10699,-0.00896697,0.,
                     0.102941,-0.00300188,0.0303571,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-4.,-4.,-4.};
                  center = {31.5,31.5,31.5};
               };
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };


      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/hydrogen.fld";
         };
      };
      MODS.bounds bounds {
         in_field => <-.Read_Field.field;
      };

      GEOMS.Plane Plane {
         dim1 = 300;
         dim2 = 300;

         plane {
            xform {
               mat = {
                  0.,0.,-63.,0.,
                  0.,63.,0.,0.,
                  63.,0.,0.,0.,
                  0.,0.,0.,1.
               };
               xlate = {30.,0.,63.};
            };
         };

         plane_ui {
            dim1_slider {
               value => ;
            };
            dim2_slider {
               value => ;
            };
         };
      };

      MODS.interp_data interp_data {
         in_field => <-.Read_Field.field;
         in_probe => <-.Plane.out_fld;
      };

      XP_COMM_PROJ.DatamapFunction.DatamapFunctionMacs.datamap_function datamap_function {
         in => <-.interp_data.out_fld;
         params {
            datamap_function = 6;
         };
      };
   };


};

