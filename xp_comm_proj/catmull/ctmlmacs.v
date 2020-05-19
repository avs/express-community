
flibrary CatmullMacs {

   // CatmullUI
   //   UI Macro
   //   UImod_panel containing 1 UIfieldTypein macro and 2 radio buttons

   macro CatmullUI<NEx=671.,NEy=165.> {
      XP_COMM_PROJ.Catmull.CatmullMods.CatmullParams &CatmullParams<NEx=352.,NEy=22.,NEportLevels={2,1}>;

      UImod_panel panel<NEx=99.,NEy=363.> {
         message = "Select Catmull Rom Spline control panel.";
         title => name_of(<-.<-.<-);
         //parent<NEportLevels={4,0}>;
      };

      UIlabel catmull_label<NEx=704.,NEy=385.> {
         parent => <-.panel;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Catmull Rom Spline Control Panel";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };


      // No. of Steps UI Components
      UIfieldTypein numStepsTypein<NEx=198.,NEy=176.> {
         UIparent => <-.panel;
         flabel => <-.string;
         fval<NEx=352.,NEy=44.> => <-.CatmullParams.steps;
         y => catmull_label.y + catmull_label.height + 5;
      };
      string string<NEportLevels=1,NEx=22.,NEy=154.> = "No. of Steps";

      // Open/Closed Spline UI Components
      UIlabel curve_state<NEx=704.,NEy=385.> {
         parent => <-.panel;
         alignment = "left";
         label => "Make spline...";
         y => numStepsTypein.y + numStepsTypein.height + 5;
      };

      UIoption closed<NEx=627.,NEy=55.> {
         set = 1;
      };
      UIoption open<NEx=627.,NEy=88.>;
      UIradioBox UIradioBox<NEx=506.,NEy=198.> {
         parent => <-.panel;
         cmdList => {<-.closed,<-.open};
         selectedItem<NEportLevels={2,2}> => <-.CatmullParams.line_type;
         y => curve_state.y + curve_state.height + 5;
      };

   };


   // catmull
   //   User Macro (CatmullRomSpline low level module, CatmullParams, and CatmullUI)

   macro catmull<NEx=616.,NEy=231.> {
      ilink in<NEx=176.,NEy=33.>;

      XP_COMM_PROJ.Catmull.CatmullMods.CatmullParams CatmullParams<NEx=517.,NEy=33.>;

      XP_COMM_PROJ.Catmull.CatmullMacs.CatmullUI CatmullUI<NEx=671.,NEy=165.> {
         CatmullParams => <-.CatmullParams;
      };

      XP_COMM_PROJ.Catmull.CatmullMods.CatmullRomSpline CatmullRomSpline<NEx=429.,NEy=154.> {
         points => {<-.in};
         CatmullParams => <-.CatmullParams;
      };

      FLD_MAP.polyline_mesh polyline_mesh<NEx=275.,NEy=231.> {
         coord => <-.CatmullRomSpline.out;
         connect => {0, ((array_size(<-.CatmullRomSpline.out) / <-.CatmullRomSpline.nspace) - 1)};
      };

      olink out<NEx=40.,NEy=330.> => .CatmullRomSpline.out;
      olink out_mesh<NEx=220.,NEy=330.> => .polyline_mesh.out;
      olink obj<NEx=473.,NEy=330.> => .polyline_mesh.obj;
   };

   APPS.SingleWindowApp CatmullEg {

      GDM.Uviewer3D Uviewer3D<NEx=330.,NEy=286.> {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.catmull.obj,<-.<-.<-.point_mesh.obj,
                  <-.<-.<-.polyline_mesh.obj};
            };
         };
      };

      //Positions of Nodes
      //float nodes<NEportLevels={0,1},NEx=187.,NEy=22.>[8][2] = {
      //   -0.2,3.,1.,0.5,2.,1.,2.,2.2,3.,3.,2.5,4.,-1.,4.5,-2.,3.
      //};

      float nodes<NEportLevels={0,1},NEx=187.,NEy=22.>[8][3] = {
         -0.2, 3., -2.,
           1.,0.5,-2.5,
           2., 1.,-3.5,
           2.,2.2, 1.5,
           3., 3., 2.5,
          2.5, 4.,  1.,
          -1.,4.5, 0.0,
          -2., 3.,-1.5
      };

      //Generates the spline curve
      XP_COMM_PROJ.Catmull.CatmullMacs.catmull catmull<NEx=451.,NEy=154.> {
         in => <-.nodes;
      };

      //Generates dots to show the position of the nodes
      FLD_MAP.point_mesh point_mesh<NEx=88.,NEy=154.> {
         coord => <-.nodes;
         DataObject {
            Props {
               line_width = 5;
               inherit = 0;
            };
         };
      };

      //Generates straight lines to show the difference with respect to the spline
      FLD_MAP.polyline_mesh polyline_mesh<NEx=286.,NEy=154.> {
         coord => <-.nodes;

         float dims[] => array_dims(coord);
         int nspace => (array_size(coord) / dims[1]);

         connect => {0, ((array_size(<-.nodes) / .nspace) - 1)};

         DataObject {
            Props {
               col = {1.,0.,0.};
               line_width = 2;
               inherit = 0;
            };
         };
      };

   };

};
