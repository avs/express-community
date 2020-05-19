//
// use right mouse to click a trail of polyline nodes, including concave
// shape, then hit "Done" to see the resulting triangulation.
//

APPS.SingleWindowApp TriConcavePolygonExample {
   GDM.Uviewer2D Uviewer2D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Concave_Tri.out_obj
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
      obj_in => <-.Concave_Tri.out_obj;
      click_sketch {
         option_polyg {
            set = 1;
         };
         GDroi2d_click {
            option = "Polygon";
         };
      };
   };

   XP_COMM_PROJ.Triangle.TriangleMacs.Concave_Tri Concave_Tri {
      in_fld => <-.ClickSketch.click_sketch.GDmap2d_ss.out_mesh;
      tri_mesh {
         out {
            xform {
               xlate = {0.0418325,0., 0.};
            };
         };
         DataObject {
            Modes {
               mode = {0,2,0,0,0};
            };
         };
      };
   };

};

