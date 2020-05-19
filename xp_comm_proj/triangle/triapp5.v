APPS.SingleWindowApp TriPointInPolygonExample {
   GDM.Uviewer2D Uviewer2D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Delauney.out_obj,
               <-.<-.<-.cell_centers.out_obj,
               <-.<-.<-.text_glyph.out_obj,
               <-.<-.<-.TextTitle.DefaultObject
            };
         };
      };

      Scene_Editor {
         Object_Editor {
            GDmodes_edit {
               lines = "Regular";
            };
         };
      };
   };


   XP_COMM_PROJ.Triangle.TriangleMacs.Delauney Delauney {
      in_fld => <-.ClickSketch.click_sketch.GDmap2d_ss.out_mesh;
      triangle {
         params = "zn";
      };
      tri_mesh {
         out {
            xform {
               xlate = {-0.085241,-0.230652,0.};
            };
         };
         DataObject {
            Modes {
               mode = {0,2,0,0,0};
            };
         };
      };

      XP_COMM_PROJ.Triangle.TriangleMods.point_in_triangle point_in_triangle {
         mode = 1;
         ncoords => <-.triangle.out.numberofpoints; 
         coords => <-.triangle.out.pointlist;
         ntris => <-.triangle.out.numberoftriangles; 
         connect => <-.triangle.out.trianglelist;
         neighbours => <-.triangle.out.neighborlist;
         npoints = 1;
         point => {
            <-.tri_mesh.DataObject.PickInfo.pick_data[0].point[0],
            <-.tri_mesh.DataObject.PickInfo.pick_data[0].point[1]
         };
         //index = {};
         // npoints = 0;
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
   MODS.cell_centers cell_centers {
      in_field => <-.Delauney.out_mesh;
   };
   int+Port tricount[] => init_array(.Delauney.triangle.out.numberoftriangles,0,(.Delauney.triangle.out.numberoftriangles - 1));

   MODS.text_glyph text_glyph {
      in_mesh => <-.cell_centers.out_fld;
      in_text => <-.tricount;
   };


   int+nres+Port selected[] => .Delauney.point_in_triangle.index;
   string+Port label_string => "Triangle Picked : " + switch(is_valid(selected[0])+1, "None", str_format("%d\n", selected[0]) );
   //string+Port label_string => "Triangle\n";

   GEOMS.TextTitle TextTitle {
      text {
         str<NEportLevels={3,0}> => <-.<-.label_string;
      };
      TextUI {
         Font {
            text = "-adobe-times-bold-r-*-*-20-*-*-*-*-*-*-*";
         };
      };
   };

};

