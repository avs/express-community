
// Defines library used to hold high-level components
// Also specifies that this library should not be considered during compilation
flibrary EllipseMacs <compile_subs=0> {


   // User Interface macro
   // Based on Standard IAC UI found in Common project
   XP_COMM_PROJ.Common.UIWidgets.IAC_StandardUI EllipseUI {
      panel {
         message = "Select Ellipse control panel.";   // Information message
      };
      mainTitleLabel {
         label = "Ellipse Control Panel";             // Title shown in control panel
      };

      // Reference to external parameter blocks
      XP_COMM_PROJ.Ellipse.EllipseMods.EllipseParams &EllipseParams<NEportLevels={2,1}>;
      XP_COMM_PROJ.Common.CommonMods.RandomNumParams &RandomNumParams<NEportLevels={2,1}>;

      // Slider UI components used to adjust project parameters.
      // The value of each slider is connected to the appropriate item in
      // the parameter block.  This allows changes to propagate to the
      // Functional Macro and the low-level modules.
      UIslider NumEllipsesUIslider {
         parent => <-.panel;
         value => <-.EllipseParams.num_ellipses;
         title => "Number of Ellipses";
         min = 2.;
         max = 50.;
         mode = "integer";
      };
      UIslider NumPointsUIslider {
         parent => <-.panel;
         value => <-.EllipseParams.num_points;
         title => "Number of Points per Ellipse";
         min = 4.;
         max = 60.;
         mode = "integer";
      };
      UIslider MinValUIslider {
         parent => <-.panel;
         value => <-.RandomNumParams.min_val;
         title => "Minimum Radius of Ellipse";
         min = 0.;
         max = 0.5;
         mode = "real";
      };
      UIslider MaxValUIslider {
         parent => <-.panel;
         value => <-.RandomNumParams.max_val;
         title => "Maximum Radius of Ellipse";
         min = 0.;
         max = 0.5;
         mode = "real";
      };
   };




   // Functional Macro.
   // Contains all components necessary to generate AVS meshes.
   // Does not contain real parameters or UI components.
   macro Ellipse {

      // Reference to external parameter blocks
      XP_COMM_PROJ.Ellipse.EllipseMods.EllipseParams &EllipseParams<NEportLevels={2,1}>;
      XP_COMM_PROJ.Common.CommonMods.RandomNumParams &RandomNumParams<NEportLevels={2,1}>;

      // Random Number Generator modules.
      // Used to generate data for EllipseCore module.
      // Seed values are set so that the resultant shape
      // of the elliptical tube is repeatable.
      XP_COMM_PROJ.Common.CommonMods.RandomNumCore x_random {
         RandomNumParams => <-.RandomNumParams;
         rseed => 5;
      };
      XP_COMM_PROJ.Common.CommonMods.RandomNumCore y_random {
         RandomNumParams => <-.RandomNumParams;
         rseed => 4;
      };

      // Core low-level module. Generates ellipse data from
      // input data.
      XP_COMM_PROJ.Ellipse.EllipseMods.EllipseCore EllipseCore {
         EllipseParams => <-.EllipseParams;
         x_dims => <-.x_random.out_vals;
         y_dims => <-.y_random.out_vals * 2;  // Input data is scaled to give results a definite elliptical shape
      };


      // Field mapping module that generates a mesh
      // from coordinates and polyline connectivity data
      FLD_MAP.polyline_mesh polyline_mesh {
         coord => <-.EllipseCore.line_coords;
         connect => <-.EllipseCore.line_conn;
         DataObject {
            Props {          // Properties changed to separate polyline from quad mesh
               jitter = 1;
               inherit = 0;
               line_width = 2;
            };
            Obj {
               name => (name_of(<-.<-.<-.<-) + "_polyline");    // name changed to assist object picking 
            };
         };
      };

      // Field mapping module that generates a mesh
      // from coordinates and quad connectivity data
      FLD_MAP.quad_mesh quad_mesh {
         coord => <-.EllipseCore.line_coords;
         connect => <-.EllipseCore.quad_conn;
         DataObject {
            Obj {
               name => (name_of(<-.<-.<-.<-) + "_surface");    // name changed to assist object picking 
            };
         };
      };

      // Group object combines DataObjects into a single viewable object
      GDM.GroupObject GroupObject {
         child_objs => {
            <-.polyline_mesh.obj,<-.quad_mesh.obj
         };
         Top {
            name => name_of(<-.<-.<-);    // name changed to assist object picking 
         };
      };

      // Output links. These help simplify generated V-code. 
      olink out_polyline_mesh => .polyline_mesh.out;
      olink out_quad_mesh => .quad_mesh.out;
      olink out_obj => .GroupObject.obj;
   };




   // User Macro.
   // Contains and connects together the real instances of the parameter blocks,
   // Functional macro and UI macros.
   macro ellipse {

      // Real instance of the Ellipse parameter block.
      // This is the only real instance used in the project.
      XP_COMM_PROJ.Ellipse.EllipseMods.EllipseParams EllipseParams {
         num_ellipses = 6;
         num_points = 30;
         aboutxc = 0.0;
         aboutyc = 0.0;
      };

      // Real instance of the RandomNum parameter block.
      // This is the only real instance used in the project.
      XP_COMM_PROJ.Common.CommonMods.RandomNumParams RandomNumParams {
         num_vals => <-.EllipseParams.num_ellipses;
         min_val = 0.25;
         max_val = 0.5;
      };

      // UI macro with connections to parameter blocks
      XP_COMM_PROJ.Ellipse.EllipseMacs.EllipseUI EllipseUI {
         EllipseParams => <-.EllipseParams;
         RandomNumParams => <-.RandomNumParams;
      };

      // Functional macro with connections to parameter blocks
      XP_COMM_PROJ.Ellipse.EllipseMacs.Ellipse Ellipse {
         EllipseParams => <-.EllipseParams;
         RandomNumParams => <-.RandomNumParams;

         polyline_mesh {
            DataObject {
               Obj {
                  name => (name_of(<-.<-.<-.<-.<-) + "_polyline");
               };
            };
         };
         quad_mesh {
            DataObject {
               Obj {
                  name => (name_of(<-.<-.<-.<-.<-) + "_surface");
               };
            };
         };
         GroupObject {
            Top {
               name => name_of(<-.<-.<-.<-);
            };
         };
      };


      // Output links. These help simplify generated V-code. 
      olink out_polyline_mesh => .Ellipse.out_polyline_mesh;
      olink out_quad_mesh => .Ellipse.out_quad_mesh;
      olink out_obj => .Ellipse.out_obj;
   };




   // Example Application
   APPS.SingleWindowApp EllipseEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.ellipse.out_obj    // name changed to assist object picking
               };
            };
         };
      };

      XP_COMM_PROJ.Ellipse.EllipseMacs.ellipse ellipse;
   };


};

