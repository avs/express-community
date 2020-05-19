APPS.SingleWindowApp TriDelauneyTriangulationExample {
   GDM.Uviewer2D Uviewer2D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Delauney.out_obj
            };
         };
      };
   };


   GDM.ClickSketch ClickSketch {
      view_in => <-.Uviewer2D.Scene_Selector.curr_view;
      click_sketch {
         option_polyl {
            set = 1;
         };
      };
   };

   XP_COMM_PROJ.Triangle.TriangleMacs.Delauney Delauney {
      in_fld => <-.ClickSketch.click_sketch.GDmap2d_ss.out_mesh;
   };
};

