
flibrary Catmull_2Macs {

   // CatmullUI
   //   UI Macro
   //   UImod_panel containing 2 radio buttons and 2 sliders

   macro Catmull_2UI {
      XP_COMM_PROJ.Catmull_2.Catmull_2Mods.Catmull_2Params &Catmull_2Params<NEportLevels={2,1}>;

      UImod_panel panel {
         message = "Select Catmull Rom Spline control panel.";
         title => name_of(<-.<-.<-);
         parent<NEportLevels={4,0}>;
      };

      UIlabel Catmull_2Label {
         parent => <-.panel;
         x = 0;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Catmull Rom Spline";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };


      // Open/Closed Radiobox
      UIradioBoxLabel LineTypeRadioBox {
         parent => <-.panel;
         x => <-.Catmull_2Label.x;
         y => <-.Catmull_2Label.y + <-.Catmull_2Label.height + 10;
         labels => {"closed", "open"};
         selectedItem => <-.Catmull_2Params.line_type;
         title => "Make Spline...";
      };

      UIslider SmoothnessSlider {
         parent => <-.panel;
         x => <-.Catmull_2Label.x;
         y => <-.LineTypeRadioBox.y + <-.LineTypeRadioBox.height + 15;
         value => <-.Catmull_2Params.smoothness;
         title => "Spline Smoothness";
         min = 0.5;
         max = 5.;
         mode = "real";
         decimalPoints = 1;
      };

      UIslider PointsSlider {
         parent => <-.panel;
         x => <-.Catmull_2Label.x;
         y => <-.SmoothnessSlider.y + <-.SmoothnessSlider.height + 5;
         value => <-.Catmull_2Params.points_added;
         title => "Num. Points to Add";
         min = 1.;
         max = 15.;
         mode = "integer";
         decimalPoints = 0;
      };

   };


   // CatmullFldUI
   //   UI Macro
   //   UImod_panel containing 3 sliders

   Catmull_2UI CatmullFldUI {
      -LineTypeRadioBox;

      SmoothnessSlider {
         y => <-.Catmull_2Label.y + <-.Catmull_2Label.height + 10;
      };

      UIslider EpsilonSlider {
         parent => <-.panel;
         x => <-.Catmull_2Label.x;
         y => <-.PointsSlider.y + <-.PointsSlider.height + 5;
         value => <-.Catmull_2Params.DistanceEps;
         title => "Distance Epsilon";
         min = 0.0;
         max = 1.0;
         mode = "real";
         decimalPoints = 5;
      };
   };



   // CatmullSplineFunc
   //   Functional Macro (CatmullSplineCore low level module, CatmullParams)

   macro CatmullSpline_2Func {
      ilink in;

      XP_COMM_PROJ.Catmull_2.Catmull_2Mods.Catmull_2Params &Catmull_2Params<NEportLevels={2,1}>;

      XP_COMM_PROJ.Catmull_2.Catmull_2Mods.CatmullSpline_2Core CatmullSpline_2Core {
         points => <-.in;
         Catmull_2Params => <-.Catmull_2Params;
      };

      FLD_MAP.polyline_mesh polyline_mesh {
         coord => <-.CatmullSpline_2Core.out;
         connect => {0, (<-.CatmullSpline_2Core.out_size - 1)};
      };

      olink out_data => .CatmullSpline_2Core.out;
      olink out_mesh => .polyline_mesh.out;
      olink out_obj  => .polyline_mesh.obj;
   };



   // CatmullSplineFldFunc
   //   Functional Macro (CatmullSplineFldCore low level module, CatmullParams)

   macro CatmullSplineFldFunc {
      ilink in_fld;

      XP_COMM_PROJ.Catmull_2.Catmull_2Mods.Catmull_2Params &Catmull_2Params<NEportLevels={2,1}>;

      XP_COMM_PROJ.Catmull_2.Catmull_2Mods.CatmullSplineFldCore CatmullSplineFldCore {
         in_fld => <-.in_fld;
         Catmull_2Params => <-.Catmull_2Params;
      };

      GDM.DataObject DataObject {
         in => <-.CatmullSplineFldCore.out;
      };

      olink out_fld => .CatmullSplineFldCore.out;
      olink out_obj => .DataObject.obj;
   };



   // catmull_spline_2
   //   User Macro (CatmullSplinefunc functional macro, CatmullParams, and CatmullUI)

   macro catmull_spline_2 {
      ilink in;

      XP_COMM_PROJ.Catmull_2.Catmull_2Mods.Catmull_2Params Catmull_2Params {
         line_type = "closed";
         spline_type = "CatmullRom";
         smoothness = 1.;
         points_added = 10;
         DistanceEps = 0.01;
      };

      XP_COMM_PROJ.Catmull_2.Catmull_2Macs.Catmull_2UI Catmull_2UI {
         Catmull_2Params => <-.Catmull_2Params;
      };

      XP_COMM_PROJ.Catmull_2.Catmull_2Macs.CatmullSpline_2Func CatmullSpline_2Func {
         in => <-.in;
         Catmull_2Params => <-.Catmull_2Params;
      };

      olink out_data => .CatmullSpline_2Func.out_data;
      olink out_mesh => .CatmullSpline_2Func.out_mesh;
      olink out_obj  => .CatmullSpline_2Func.out_obj;
   };



   // catmull_spline_fld_2
   //   User Macro (CatmullSplineFldFunc functional macro, CatmullParams, and CatmullFldUI)

   macro catmull_spline_fld_2 {
      ilink in_fld;

      XP_COMM_PROJ.Catmull_2.Catmull_2Mods.Catmull_2Params Catmull_2Params {
         line_type = "closed";
         spline_type = "CatmullRom";
         smoothness = 1.;
         points_added = 10;
         DistanceEps = 0.01;
      };

      XP_COMM_PROJ.Catmull_2.Catmull_2Macs.CatmullFldUI CatmullFldUI {
         Catmull_2Params => <-.Catmull_2Params;
      };

      XP_COMM_PROJ.Catmull_2.Catmull_2Macs.CatmullSplineFldFunc CatmullSplineFldFunc {
         in_fld => <-.in_fld;
         Catmull_2Params => <-.Catmull_2Params;
      };

      olink out_fld => .CatmullSplineFldFunc.out_fld;
      olink out_obj => .CatmullSplineFldFunc.out_obj;
   };


};

