
flibrary ChromaDepthApps<compile_subs=0> {

   APPS.MultiWindowApp ChromaDepthEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Geom.geom
               };
               Xform {
                  ocenter = {0.21691,0.,-0.4};
                  dcenter = {0.133605,0.451568,0.370991};
                  mat = {
                     1.12426,0.170126,0.0462769,0.,
                     -0.0615778,0.658797,-0.925875,0.,
                     -0.165205,0.91219,0.660045,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-0.133605,-0.451568,-0.370991};
               };
            };
         };

         GDM.Uscene2D ChromaDepthScene {
            Top {
               child_objs<NEportLevels={4,1}> => {
                  <-.<-.<-.chroma_depth.chromadepth_obj
               };
/*
               Xform {
                  ocenter = {249.5,249.5,0.};
                  dcenter = {-732.5,-732.5,0.};
               };
*/
            };
            View {
               ViewUI {
                  ViewPanel {
                     UI {
                        panel {
                           defaultWidth = 800;
                           defaultHeight = 800;
                        };
                     };
                  };
               };
            };
         };
      };


      MODS.Read_Geom Read_Geom {
         read_geom_ui {
            filename = "$XP_PATH<0>/data/geom/teapot.geo";
         };
      };


      XP_COMM_PROJ.ChromaDepth.ChromaDepthMacs.chroma_depth chroma_depth {
         view_in => <-.Uviewer3D.Scene_Selector.curr_view;
      };
   };

};

