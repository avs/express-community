

flibrary FiniteWingMacs<compile_subs=0> {

   macro FiniteWingUI {
      XP_COMM_PROJ.FiniteWing.FiniteWingMods.FiniteWingInputParams &in_params<NEportLevels={2,1}>;
      XP_COMM_PROJ.FiniteWing.FiniteWingMods.FiniteWingOutputParams &out_params<NEportLevels={2,1}>;
      
      UImod_panel UImod_panel {
         parent<NEportLevels={4,0}>;
         title => "Finite Wing";
      };

      UIlabel FiniteWing_Title {
         parent => <-.UImod_panel;
         x = 0;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Finite Wing Simulation";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

      UIslider aoa_slider {
         parent => <-.UImod_panel;
         title => "Angle of Attack";
         mode = "real";
         decimalPoints = 1;
         value => <-.in_params.Angle_of_Attack;
         min = -20.;
         max = 20.;
         x => <-.FiniteWing_Title.x;
         y => <-.FiniteWing_Title.y + <-.FiniteWing_Title.height + 15;
         width => parent.width;
      };
      UIslider wingspan_slider {
         parent => <-.UImod_panel;
         title => "Wing span";
         mode = "integer";
         decimalPoints = 0;
         value => <-.in_params.Wing_span;
         min = 2.;
         max = 50.;
         x => <-.aoa_slider.x;
         y => <-.aoa_slider.y + <-.aoa_slider.height + 10;
         width => parent.width;
      };
      UIslider gridsize_slider {
         parent => <-.UImod_panel;
         title => "Grid size";
         mode = "integer";
         decimalPoints = 0;
         value => <-.in_params.Grid_size;
         min = 11.;
         max = 51.;
         x => <-.wingspan_slider.x;
         y => <-.wingspan_slider.y + <-.wingspan_slider.height + 10;
         width => parent.width;
      };

      UIslider probe_pos_slider {
         parent => <-.UImod_panel;
         title => "Probe Height";
         mode = "real";
         decimalPoints = 1;
         value => <-.out_params.probe_z;
         min = 0.;
         max => <-.in_params.Grid_size - 1;
         x => <-.gridsize_slider.x;
         y => <-.gridsize_slider.y + <-.gridsize_slider.height + 15;
         width => parent.width;
      };
      UItoggle scan_probe_toggle {
         parent => <-.UImod_panel;
         label => "Scan Probe";
         set+IPort2 => <-.in_params.scan_probe;
         x => <-.probe_pos_slider.x + 25;
         y => <-.probe_pos_slider.y + <-.probe_pos_slider.height + 10;
      };
   };


   macro FiniteWingFunc {
      XP_COMM_PROJ.FiniteWing.FiniteWingMods.FiniteWingInputParams &in_params<NEportLevels={2,1}>;
      
      XP_COMM_PROJ.FiniteWing.FiniteWingMods.FiniteWingCore FiniteWingCore {
         in_params => <-.in_params;
      };

      FLD_MAP.interleave_3_arrays interleave_3_arrays {
         in1 => <-.FiniteWingCore.Output_x;
         in2 => <-.FiniteWingCore.Output_y;
         in3 => <-.FiniteWingCore.Output_z;
      };

      int+Port dims[3] => {in_params.Grid_size, in_params.Grid_size, in_params.Grid_size};

      FLD_MAP.uniform_vector_field uniform_vector_field {
         mesh {
            in_dims => <-.<-.dims;
         };
         data {
            in_data => <-.<-.interleave_3_arrays.out;
         };
      };

      olink out_fld    => .uniform_vector_field.out;
      olink out_obj    => .uniform_vector_field.obj;
      olink out_params => .FiniteWingCore.out_params;
   };



   macro finite_wing {
      XP_COMM_PROJ.FiniteWing.FiniteWingMods.FiniteWingInputParams in_params {
         Angle_of_Attack = 8.0;
         Wing_span = 20;
         Grid_size = 21;
         scan_probe = 0;
      };
      
      FiniteWingFunc FiniteWingFunc {
         in_params => <-.in_params;
      };

      XP_COMM_PROJ.FiniteWing.FiniteWingMods.FiniteWingOutputParams+Port &out_params => .FiniteWingFunc.out_params;

      FiniteWingUI FiniteWingUI {
         in_params => <-.in_params;
         out_params => <-.out_params;
      };

      XP_COMM_PROJ.FiniteWing.FiniteWingMods.HorizontalLine3D Probe_Line3D {
         dim1 = 50;
         line_x => <-.out_params.probe_x;
         line_y => <-.out_params.probe_y;
         line_z => <-.out_params.probe_z;
         line_scale => <-.out_params.probe_scale;
      };

      XP_COMM_PROJ.FiniteWing.FiniteWingMods.HorizontalLine3D Wing_Line3D {
         dim1 = 8;
         line_x => <-.out_params.wing_x;
         line_y => <-.out_params.wing_y;
         line_z => <-.out_params.wing_z;
         line_scale => <-.out_params.wing_scale;
      };

      GMOD.loop loop {
         run => <-.in_params.scan_probe;
         done = 1;
         start_val = 0.;
         end_val => (<-.in_params.Grid_size - 1);
         incr = 0.1;
         count+Port2 => <-.out_params.probe_z;
      };

      olink out_fld =>  .FiniteWingFunc.out_fld;
      olink out_obj =>  .FiniteWingFunc.out_obj;

      olink out_probe     => .Probe_Line3D.out_fld;
      olink out_probe_obj => .Probe_Line3D.out_obj;
      olink out_wing_obj  => .Wing_Line3D.out_obj;
   };


};

