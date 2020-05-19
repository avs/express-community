
flibrary ExtractObjectsApps <compile_subs=0> {

   APPS.SingleWindowApp ExtractObjectsEg {

      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => <-.<-.<-.extract_view_objs.out_objs;
               Xform {
                  mat = {
                     0.354129,12.4068,3.08697,0.,
                     5.31723,2.66548,-11.3228,0.,
                     -11.627,1.59687,-5.08415,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-4.,-2.,-8.};
                  center = {0.1524,0.0762,0.3048};
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,
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


      MODS.Plot3d_Multi_Block Plot3d_Multi_Block {
         filename_xyz = "$XP_PATH<0>/data/plot3d/multb_x.dat";
         filename_q = "$XP_PATH<0>/data/plot3d/multb_q.dat";
      };


      MODS_ARR.combine_vect_ARR combine_vect_ARR {
         in_field => <-.Plot3d_Multi_Block.fields;
         CombineVectParam {
            components = {1,2,3};
         };
      };
      MODS.fields_to_mblock fields_to_mblock {
         fields => <-.combine_vect_ARR.out_fld;
      };


      GEOMS.FPlane FPlane {
         in => <-.Plot3d_Multi_Block.mblock;
         dim1 = 5;
         dim2 = 6;
         plane {
            xform {
               xlate = {0.01, 0.0, 0.0};
            };
         };
      };
      MODS.stream_multi_block stream_multi_block {
         in_field => <-.fields_to_mblock.multi_block;
         in_probe => <-.FPlane.out_fld;
      };


      MODS_ARR.bounds_ARR bounds_ARR {
         in_field => <-.Plot3d_Multi_Block.fields;
      };

      XP_COMM_PROJ.ExtractObjects.ExtractObjectsMacs.extract_objects extract_data_objs {
         in_objs => <-.Plot3d_Multi_Block.fields;
         inc_objects = {0,2};
      };
      MODS_ARR.isosurface_ARR isosurface_ARR {
         in_field => <-.extract_data_objs.out_objs;
         IsoParam {
            iso_component = 4;
            map_component = {1};
            iso_level = -0.8;
         };
      };

      XP_COMM_PROJ.ExtractObjects.ExtractObjectsMacs.extract_objects extract_view_objs {
         in_objs => {
            <-.bounds_ARR.out_obj,
            <-.isosurface_ARR.out_obj,
            <-.stream_multi_block.out_obj,
            <-.FPlane.out_obj
         };
         inc_objects = {0,1,2};
         objectnames => {"Bounds", "Isosurface", "Streamlines", "Streamline Probe"};
      };
   };


};

