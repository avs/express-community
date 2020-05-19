APPS.SingleWindowApp TriFaultExample {
   GDM.Uviewer2D Uviewer2D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Triangulate_Area.out_obj
            };
            Xform {
               ocenter = {4.5,4.5,0.};
               dcenter = {4.,4.,0.};
            };
         };
      };
      Scene_Editor {
         Object_Editor {
            GDmodes_edit {
               lines = "Regular";
               surf = "Inherit";
            };
         };
      };
   };

   FLD_MAP.uniform_mesh uniform_mesh {
      in_dims = {10,10};
   };


   XP_COMM_PROJ.Triangle.TriangleMacs.Triangulate_Area Triangulate_Area {
      in_fld   => <-.uniform_mesh.out;
      in_fault => <-.ContinuousSketch.cont_sketch.GDmap2d_ss.out_mesh;

      tri_mesh {
         DataObject {
   			Props {
   				col = {1,1,1};
   			};
            Modes {
               mode = {0,2,1,0,0};
            };
         };
      };
      combine_mesh_cell_data {
/*
         cell_data {
            cell_data = {
               {
                  null_flag=1,null_value=1,,,,,,
               }
            };
         };
*/
         DataObject {
            Modes {
               mode = {0,2,0,0,0};
            };
         };
      };

      GroupObject {
         Modes {
            mode = {0,0,2,0,0};
         };
      };
   };


   GDM.ContinuousSketch ContinuousSketch {
      view_in => <-.Uviewer2D.Scene_Selector.curr_view;
      obj_in => <-.Triangulate_Area.out_obj;
      cont_sketch {
         option_polyl {
            set = 1;
         };
         draw_copy {
            set = 1;
         };
         GDroi2d_cont {
            immed = "On Button Up";
            mode = "Clear";
         };
      };
   };
};

