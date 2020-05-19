//
// click in graphics window with right mouse,
// build up a concave polyline, then hit "Done".
// Module will draw convex hull in blue
//
APPS.SingleWindowApp TriConvexHullExample {
   GDM.Uviewer2D Uviewer2D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Convex_Hull.out_obj,
               <-.<-.<-.ClickSketch.click_sketch.DataObject.obj
            };
         };
         Camera {
            Camera {
               auto_norm = "None";
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

   XP_COMM_PROJ.Triangle.TriangleMacs.Convex_Hull Convex_Hull {
      in_fld => <-.ClickSketch.click_sketch.GDmap2d_ss.out_mesh;
   };
};

