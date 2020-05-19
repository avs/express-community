//
// fast_advector
//
// Simplified advector module with point cells sets only
// for faster generation & rendering. Avoids DVglyph overheads.
// Just draws single pixel dots for each pixel, as a point cloud.
// Performance varies with platform, data set, glyph type, etc., but
// appears to be around 3X faster than standard advector module.
// Exactly the same controls as standard advector module except this one
// is missing glyph normalize, glyph scale controls, as it does not
// use the glyph system. It is also missing the glyph input port
// for the same reason. Remember to turn off status check and module
// flashing for timing tests. The faster speed allows many more
// sample points to be animated, such as 1000 or more at one time.
// Because this re-implements the advector module from lower level
// parts, it is only available to users of the Developer Edition.
//
// Author: I. Curington, <ianc@avs.com>  August 29th, 2000
//
//


flibrary FastAdvectorMacs <compile_subs=0> {

   macro FastAdvectUI {
      ilink in_fld;                               // => <-.in_field;
      DV_Param_advect &param<NEportLevels={2,0}>; // => <-.AdvectParam;

      UImod_panel UIpanel {
         parent<NEportLevels={4,0}>;
         message = "Select Fast Advector control panel.";
         title => name_of(<-.<-.<-);
      };


      UIslider nseg_slider {
         y = 0;
         width => <-.UIpanel.width;
         parent => <-.UIpanel;
         max = 16.;
         value<NEportLevels={2,0}> => param.nseg;
         mode = "integer";
         title => "N-segments";
      };
      UIslider max_seg_slider {
         y => ((<-.nseg_slider.y + <-.nseg_slider.height) + 4);
         width => <-.UIpanel.width;
         parent => <-.UIpanel;
         min = 1.;
         max = 10000.;
         value<NEportLevels={2,0}> => param.max_seg;
         mode = "integer";
         title => "Max Segments";
      };
      UIslider order_slider {
         y => ((<-.max_seg_slider.y + <-.max_seg_slider.height) + 4);
         width => <-.UIpanel.width;
         parent => <-.UIpanel;
         min = 1.;
         max = 4.;
         value<NEportLevels={2,0}> => param.order;
         mode = "integer";
         title => "Order";
      };
      UIslider vel_slider {
         y => ((<-.order_slider.y + <-.order_slider.height) + 4);
         width => ((<-.UIpanel.width * 11) / 12);
         parent => <-.UIpanel;
         value<NEportLevels={2,0}> => param.min_vel;
         title => "Min Velocity";
      };
      VUIslider_typein vel_slider_typein {
         slider => <-.vel_slider;
      };
      UIradioBoxLabel UIradioBoxLabel_dir {
         parent => <-.UIpanel;
         labels<NEportLevels={2,0}> => {"Backward","Forward"};
         &selectedItem => param.forw_back;
         title => "Direction";
         visible => <-.UIpanel.visible;
         y => ((<-.vel_slider.y + <-.vel_slider.height) + 4);
         width => <-.UIpanel.width;
         label_cmd {
            cmd[];
         };
      };
      UIradioBoxLabel UIradioBoxLabel_mode {
         parent => <-.UIpanel;
         labels<NEportLevels={2,0}> => {"Scalar","Vector","Components"};
         &selectedItem => param.vector;
         title => "Mode";
         visible => <-.UIpanel.visible;
         y => ((<-.UIradioBoxLabel_dir.y + <-.UIradioBoxLabel_dir.height) + 4);
         width => <-.UIpanel.width;
         label_cmd {
            cmd[];
         };
      };
      UIslider start_slider {
         y => ((<-.UIradioBoxLabel_mode.y + <-.UIradioBoxLabel_mode.height) + 4);
         width => ((<-.UIpanel.width * 11) / 12);
         parent => <-.UIpanel;
         value<NEportLevels={2,0}> => param.start;
         title => "Start Time";
      };
      VUIslider_typein start_slider_typein {
         slider => <-.start_slider;
      };
      UIslider end_slider {
         y => ((<-.start_slider.y + <-.start_slider.height) + 4);
         width => ((<-.UIpanel.width * 11) / 12);
         parent => <-.UIpanel;
         value<NEportLevels={2,0}> => param.end;
         title => "End Time";
      };
      VUIslider_typein end_slider_typein {
         slider => <-.end_slider;
      };
      UIslider incr_slider {
         y => ((<-.end_slider.y + <-.end_slider.height) + 4);
         width => ((<-.UIpanel.width * 11) / 12);
         parent => <-.UIpanel;
         max = 10.;
         value<NEportLevels={2,0}> => param.incr;
         title => "Step";
      };
      VUIslider_typein incr_slider_typein {
         slider => <-.incr_slider;
      };
      UIslider release_slider {
         y => ((<-.incr_slider.y + <-.incr_slider.height) + 4);
         width => ((<-.UIpanel.width * 11) / 12);
         parent => <-.UIpanel;
         max = 10.;
         value<NEportLevels={2,0}> => param.release_interval;
         title => "Release Interval";
      };
      VUIslider_typein release_slider_typein {
         slider => <-.release_slider;
      };
      UIfieldTypein time_typein {
         UIparent => <-.UIpanel;
         flabel => "Time";
         fval<NEportLevels={2,0}> => param.count;
         panel {
            y => ((<-.<-.release_slider.y + <-.<-.release_slider.height) + 4);
            width => <-.<-.UIpanel.width;
         };
      };
      UItoggle run_toggle {
         y => ((<-.time_typein.panel.y + <-.time_typein.panel.height) + 4);
         width => <-.UIpanel.width;
         parent => <-.UIpanel;
         label => "Run";
         set<NEportLevels={2,0}> => param.run;
      };
      UItoggle reset_toggle {
         y => ((<-.run_toggle.y + <-.run_toggle.height) + 4);
         width => <-.UIpanel.width;
         parent => <-.UIpanel;
         label => "Reset Time";
         set<NEportLevels={2,0}> => param.reset;
      };
      UItoggle cycle_toggle {
         y => ((<-.reset_toggle.y + <-.reset_toggle.height) + 4);
         width => <-.UIpanel.width;
         parent => <-.UIpanel;
         label => "Cycle";
         set<NEportLevels={2,0}> => param.cycle;
      };
   };



   macro FastAdvect {
      ilink in_fld;
      ilink in_prb;
      DV_Param_advect+IPort2 &param;
      DVstream DVstream {
         in => <-.in_fld;
         probe => <-.in_prb;
         &order => param.order;
         &forw_back => param.forw_back;
         &nseg => param.nseg;
         &min_vel => param.min_vel;
         &max_seg => param.max_seg;
         &stream_comp => param.component;
         ribbons = 0;
         rib_width = 0.;
         rib_angle = 0.;
      };
      GMOD.loop loop {
         &reset => param.reset;
         &run => param.run;
         &cycle => param.cycle;
         done = 1;
         &start_val => param.start;
         &end_val => param.end;
         &incr => param.incr;
         &count => param.count;
      };
      DVadvect DVadvect {
         in => <-.DVstream.out;
         &time => param.count;
         &direction => param.forw_back;
         &start_time => param.start;
         &release_interval => param.release_interval;
      };
      olink out_fld => .DVadvect.out;
   };


   macro fast_advector_noui {
      ilink in_field<export_all=1>;
      ilink in_probe<export_all=1>;
      DV_Param_advect+Port AdvectParam<export_all=2> {
         component = 0;
         order = 2;
         forw_back = 1;
         nseg = 4;
         max_seg = 256;
         min_vel = 0.;
         run = 0;
         reset = 0;
         cycle = 0;
         start = 0.;
         end = 1.;
         incr = 0.03;
         count = 1.;
         release_interval = 0.;
         vector = 1;
         scale = 0.0;
         normalize = 0.;
      };
      FastAdvect FastAdvect {
         in_fld => <-.in_field;
         in_prb => <-.in_probe;
         param => <-.AdvectParam;
      };
      DataObjectLite DataObjectLite {
         in => <-.FastAdvect.out_fld;
      };
      olink out_fld<export_all=2> => .FastAdvect.out_fld;
      olink out_obj<export_all=2> => .DataObjectLite.obj;
   };



   fast_advector_noui fast_advector {
      FastAdvectUI FastAdvectUI {
         in_fld => <-.in_field;
         &param => <-.AdvectParam;
      };
   };


};

