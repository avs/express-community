
flibrary DatamapFunctionMacs <compile_subs=0> {

   // DatamapFunctionUI
   //   UI Macro
   //   UImod_panel containing radiobox so that datamap function can be selected

   macro DatamapFunctionUI {
      XP_COMM_PROJ.DatamapFunction.DatamapFunctionMods.DatamapFunctionParams &params <NEportLevels={2,1}>;

      UImod_panel UImod_panel {
         title = "Datamap Function";
      };

      UIlabel TitleLabel {
         parent => <-.UImod_panel;
         label => "Datamap Function";
         x = 0;
         y = 0;
         width => parent.width;
         color {
           foregroundColor = "white";
           backgroundColor = "blue";
         };
      };

      UIradioBoxLabel DatamapFunctionRadioBox {
         parent => <-.UImod_panel;
         x => <-.TitleLabel.x;
         y => <-.TitleLabel.y + <-.TitleLabel.height + 10;
         width => parent.width;

         title => "Select Datamap function...";
         labels => {
            "User Defined",
            "Standard AVS Datamap",
            "Greyscale",
            "Hot Iron",
            "Cool Blue",
            "Black/White 10% Bands",
            "Colour 10% Bands"
         };
         selectedItem => <-.params.datamap_function;
      };

   };



   // DatamapFunctionFunc
   //   Functional macro (Field I/O)
   macro DatamapFunctionFunc {
      ilink in;

      XP_COMM_PROJ.DatamapFunction.DatamapFunctionMods.DatamapFunctionParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.DatamapFunction.DatamapFunctionMods.DatamapFunctionCore DatamapFunctionCore {
         in => <-.in;
         params => <-.params;
      };

      FLD_MAP.combine_mesh_data combine_mesh_data {
         in_mesh => <-.in;
         in_nd => <-.DatamapFunctionCore.out;
         obj.name => name_of(<-.<-.<-.<-);
      };

      olink out_fld => .combine_mesh_data.out;
      olink out_obj => .combine_mesh_data.obj;
   };




   // datamap_function
   //   User Macro (Field I/O and DatamapFunctionUI)

   macro datamap_function {
      ilink in;

      XP_COMM_PROJ.DatamapFunction.DatamapFunctionMods.DatamapFunctionParams params {
         datamap_function = 0;
      };

      XP_COMM_PROJ.DatamapFunction.DatamapFunctionMacs.DatamapFunctionFunc DatamapFunctionFunc {
         in => <-.in;
         params => <-.params;
         out_obj.name => name_of(<-.<-.<-.<-.<-);
      };

      XP_COMM_PROJ.DatamapFunction.DatamapFunctionMacs.DatamapFunctionUI DatamapFunctionUI {
         params => <-.params;
         UImod_panel.title => name_of(<-.<-.<-);
      };

      olink out_fld => DatamapFunctionFunc.out_fld;
      olink out_obj => DatamapFunctionFunc.out_obj;
   };



   // user_defined_datamap
   //   Functional macro hard-wired so that it always uses the user defined function
   macro user_defined_datamap {
      ilink in;

      XP_COMM_PROJ.DatamapFunction.DatamapFunctionMods.DatamapFunctionCore DatamapFunctionCore {
         in => <-.in;
         datamap_function => 0;
      };

      FLD_MAP.combine_mesh_data combine_mesh_data {
         in_mesh => <-.in;
         in_nd => <-.DatamapFunctionCore.out;
         obj.name => name_of(<-.<-.<-.<-);
      };

      olink out_fld => .combine_mesh_data.out;
      olink out_obj => .combine_mesh_data.obj;
   };


};

