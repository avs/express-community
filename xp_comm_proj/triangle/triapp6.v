APPS.SingleWindowApp TriVoronoiExample {
   GDM.Uviewer2D Uviewer2D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.ClickSketch.click_sketch.DataObject.obj,
               <-.<-.<-.Voronoi.out_obj
             };
         };
         Camera {
            Camera {
               auto_norm = "Object Attach";
            };
         };
      };
      Scene_Editor {
         Camera_Editor {
            GDcamera_edit {
               auto_norm = "Object Attach";
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
         DataObject {
            Props {
               col = {0.14,1.,0.};
               inherit = 0;
               line_width = 7;
            };
            Modes {
               mode = {2,1,1,0,0};
            };
         };
         GDroi2d_click {
            mode = "Clear";
         };
      };
   };


   XP_COMM_PROJ.Triangle.TriangleMacs.Voronoi Voronoi {
      in_fld => <-.ClickSketch.click_sketch.GDmap2d_ss.out_mesh;
      line_mesh {
         DataObject {
            Props {
               col = {1.,0.,0.};
            };
         };
      };
   };

};

