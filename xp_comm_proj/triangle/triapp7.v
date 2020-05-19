APPS.SingleWindowApp TriMovingRegionExample {
   GDM.Uviewer2D Uviewer2D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.combine_mesh_cell_data.obj,
               <-.<-.<-.line_mesh.obj,
               <-.<-.<-.ContinuousSketch.cont_sketch.DataObject.obj,
               <-.<-.<-.point_mesh.obj,
               <-.<-.<-.tri_mesh.obj,
               <-.<-.<-.uniform_mesh#1.obj
            };
            Xform {
               ocenter = {2.62324,4.73447, 0.};
               dcenter = {3.00034,4.57708, 0.};
               mat = {
                  0.743942,0.,0.,0.,
                  0.,0.743942,0.,0.,
                  0.,0.,0.743942,0.,
                  0.,0.,0.,1.
               };
               xlate = {-3.81746,-3.99342, 0.};
            };
         };
      };
   };


   FLD_MAP.uniform_mesh uniform_mesh {
      in_dims = {9,9};
   };
   FLD_MAP.extract_coordinate_array extract_coordinate_array {
      in => <-.uniform_mesh.out;
      comp = {0,1};
   };
   FLD_MAP.extract_coordinate_array extract_coordinate_array#1 {
      in => <-.uniform_mesh#1.out;
      comp = {0,1};
   };


   XP_COMM_PROJ.Triangle.TriangleMods.fault_region fault_region {
      mesh_coords+nres => <-.extract_coordinate_array.coord;
      mesh_connect+nres => <-.uniform_mesh.out.cell_set.node_connect_list;
      poly_coords+nres => <-.extract_coordinate_array#1.coord;
   };
   FLD_MAP.combine_mesh_cell_data combine_mesh_cell_data {
      in_mesh => <-.uniform_mesh.out;
      in_data => <-.fault_region.celldat;
      DataObject {
         Modes {
            mode = {0,2,0,0,0};
         };
      };
   };


   FLD_MAP.line_mesh line_mesh {
      coord => <-.fault_region.pts;
      connect => <-.fault_region.segments;
      DataObject {
         Props {
            line_width = 5;
            inherit = 0;
            col = {0.8,1.,0.};
         };
      };
   };


   GDM.ContinuousSketch ContinuousSketch {
      view_in => <-.Uviewer2D.Scene_Selector.curr_view;
      obj_in => <-.combine_mesh_cell_data.obj;
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
         DataObject {
            Props {
               line_width = 2;
               inherit = 0;
            };
            Modes {
               mode = {2,1,0,0,0};
            };
         };
      };
   };


   FLD_MAP.point_mesh point_mesh {
      coord => <-.fault_region.pts;
      DataObject {
         Props {
            col = {0.86,0.,1.};
            inherit = 0;
            line_width = 6;
         };
         Modes {
            mode = {2,0,0,0,0};
         };
      };
   };


   XP_COMM_PROJ.Triangle.TriangleMods.triangle triangle {
      params = "pqQz";
      in {
         pointlist => <-.<-.fault_region.pts;
         numberofpoints => <-.<-.fault_region.npts;
         segmentlist => <-.<-.fault_region.segments;
         numberofsegments => <-.<-.fault_region.nsegs;
      };
      out {
         trianglelist<NEportLevels={0,3}>;
      };
   };

   FLD_MAP.tri_mesh tri_mesh {
      coord => <-.fault_region.pts;
      connect => <-.triangle.out.trianglelist;
      DataObject {
         Props {
            col = {0.02,1.,0.};
            inherit = 0;
         };
         Modes {
            mode = {0,2,1,0,0};
         };
      };
   };


   FLD_MAP.uniform_mesh uniform_mesh#1 {
      in_dims = {12,12};
      out {
         xform {
            mat = {
               -0.276715,-0.0944632,0.,0.,
               0.0944632,-0.276715,0.,0.,
               0.,0.,0.292394,0.,
               0.,0.,0.,1.
            };
            xlate = {3.8424,5.62065,0.};
            ocenter = {5.5,5.5,0.};
            dcenter = {0.210754,-1.17962,0.};
            center = {5.5,5.5,0.};
         };
      };
      DataObject {
         Modes {
            mode = {0,2,1,0,0};
         };
      };
   };
};




XP_COMM_PROJ.Triangle.TriangleApps.TriMovingRegionExample TriAnimatedRegionExample {
   Uviewer2D {
      Scene {
         Top {
            Xform {
               ocenter = {4.,4.,0.};
               dcenter = {5.0488,5.05491,0.};
            };
         };
      };
   };

   MODS.Loop Loop {
      cycle = 2;
      start = 20.;
      end = 80.;
      incr = 2.;
      count = 20.;
      UIpanel {
         option {
            set = 1;
         };
      };
   };

   float+Port angle => .Loop.count;

   uniform_mesh#1 {
      out {
         xform<NEportLevels={0,3}> {
            mat = {
               0.274761,-0.100005,0.,0.,
               0.100005,0.274761,0.,0.,
               0.,0.,0.292394,0.,
               0.,0.,0.,1.
            };
         };
      };
      float trig<NEportLevels={2,1}> => <-.angle;
      in_dims => {12, (12 + (0 * trig))};
   };

   GDM.GDxform_edit GDxform_edit {
      shell_vis = 1;
      xform => <-.uniform_mesh#1.out.xform;
      z_rot<NEportLevels={2,0}> => <-.angle;
      absolute = 1;
      abs_z_rot<export=2,NEportLevels={2,0}> = 20.;
   };
};

