
flibrary PresentDemoApps <compile_subs=0> {

   APPS.MultiWindowApp PresentDemoApp {
      Scheduler.status_check = 0;

      UI {
         shell.visible = 0;

/*
         // Ugly hack required because Express doesn't set these values
         //   if the shell is not visible initally
         // If you have problems then set shell.visible = 1 and remove these two lines.
         InfoBar.y => <-.shell.clientHeight - (status_bar.height * 2) - 2;
         status_bar.y => <-.shell.clientHeight - height;
*/
      };

      GDM.Uviewer Uviewer {
         Scene {
            Top {
               child_objs => <-.<-.<-.present_demo.out_objs;
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
                  auto_norm = "None";
               };
            };
         };
#ifdef MPU_RENDERER
         Scene_Editor {
            View_Editor {
               HwSwSwitcher {
                  v_commands = "GDview_edit.renderer=switch(HW_SW.set+1,Templates.CONFIG.gd_default_renderer+0,5);";
               };
            };
         };
#endif
      };


      XP_COMM_PROJ.PresentDemo.PresentDemoMacs.present_demo present_demo {
         params {
            title = "##Enter your title here##";
#ifdef MPU_RENDERER
            use_mpu+nres => <-.<-.Uviewer.Scene_Editor.View_Editor.HW_SW.set;
#endif
         };
      };

      macro application_UI {
         ilink parent => <-.present_demo.module_panel;
      };
   };




   XP_COMM_PROJ.PresentDemo.PresentDemoApps.PresentDemoApp PresentDemoEg1 {
      Uviewer {
         Scene {
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,
               };
            };
         };
      };

      MODS.Read_Geom Read_Geom {
         read_geom_ui {
            filename = "$XP_PATH<0>/data/geom/carib.geo";
         };
      };

      present_demo {
         in_objs => {<-.Read_Geom.geom};
         objectnames = {"Geometry"};
         params {
            title = "EG1: Simple Geometry Demo";
         };
      };
   };



   XP_COMM_PROJ.PresentDemo.PresentDemoApps.PresentDemoApp PresentDemoEg2 {
      Uviewer {
         Scene {
            Top {
               Xform {
                  ocenter = {0.1524,0.0762,0.3048};
                  dcenter = {4.,2.,8.};
                  mat = {
                     -0.266993,12.4262,2.6858,0.,
                     -6.85572,-2.40304,10.4365,0.,
                     10.7063,-1.2289,6.74996,0.,
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

      GEOMS.Arrow1 Arrow1;
      MODS_ARR.advect_multi_block advect_multi_block {
         in_field => <-.fields_to_mblock.multi_block;
         in_probe => <-.FPlane.out_fld;
         in_glyph => <-.Arrow1.out_fld;
         AdvectParam {
            min_vel = 0.;
            run<NEportLevels={0,3}> = 0;
            cycle = 1;
            end = 0.4;
            incr = 0.01;
            scale = 0.05;
         };
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
            iso_level<NEportLevels={2,3}> = -0.8;
         };
      };

      present_demo {
         in_objs => {
            <-.bounds_ARR.out_obj,
            <-.isosurface_ARR.out_obj,
            <-.stream_multi_block.out_obj,
            <-.advect_multi_block.out_obj,
            <-.FPlane.out_obj
         };
         inc_objects = {0,1,2,3};
         objectnames => {"Bounds", "Isosurface", "Streamlines", "Advectors", "Probe"};
         params {
            title = "EG2: Multi-block data Demo";
         };
      };

      application_UI {
         UIslider IsosurfaceSlider {
            parent => <-.parent;
            title => "Isosurface Level";
            x = 0;
            y = 0;
            width => parent.width;
            min => <-.<-.isosurface_ARR.IsoUI.UIiso_level.min;
            max => <-.<-.isosurface_ARR.IsoUI.UIiso_level.max;
            value<NEportLevels={3,0}> => <-.<-.isosurface_ARR.IsoParam.iso_level;
            mode = "real";
         };
         UItoggle RunAdvectorToggle {
            parent => <-.parent;
            label => "Run Advection";
            x => <-.IsosurfaceSlider.x;
            y => <-.IsosurfaceSlider.y + <-.IsosurfaceSlider.height + 15;
            width => parent.width;
            set<NEportLevels={3,0}> => <-.<-.advect_multi_block.AdvectParam.run;
         };
      };
   };


};

