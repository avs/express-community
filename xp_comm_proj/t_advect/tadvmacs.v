
flibrary TailedAdvectorMacs <compile_subs=0> {

   macro TailedAdvectorUI {
      UImod_panel &UIpanel<NEportLevels={2,1}>;

      XP_COMM_PROJ.TailedAdvector.TailedAdvectorMods.TailedAdvectorParams &params<NEportLevels={2,1}>;

      UIslider tracer_slider {
         parent => <-.UIpanel;
         title => "Tracer Length";
         max = 100.;
         value<NEportLevels={2,0}> => params.tracer_len;
         mode = "integer";
         y = 0;
         width => <-.UIpanel.width;
      };
      UIradioBoxLabel UIradioBoxLabel_tracer {
         parent => <-.UIpanel;
         labels<NEportLevels={2,0}> => {"cap","cycle","add"};
         &selectedItem => params.tracer_style;
         visible => <-.UIpanel.visible;
         title => "Tracer Style";
         y => ((<-.tracer_slider.y + <-.tracer_slider.height) + 4);
         width => <-.UIpanel.width;
      };
      UIslider nseg_slider {
         parent => <-.UIpanel;
         title => "N-segments";
         max = 16.;
         value<NEportLevels={2,0}> => params.nseg;
         mode = "integer";
         y => ((<-.UIradioBoxLabel_tracer.y + <-.UIradioBoxLabel_tracer.height) + 4);
         width => <-.UIpanel.width;
      };
      UIslider max_seg_slider {
         parent => <-.UIpanel;
         title => "Max Segments";
         min = 1.;
         max = 10000.;
         value<NEportLevels={2,0}> => params.max_seg;
         mode = "integer";
         y => ((<-.nseg_slider.y + <-.nseg_slider.height) + 4);
         width => <-.UIpanel.width;
      };
      UIslider order_slider {
         parent => <-.UIpanel;
         title => "Order";
         min = 1.;
         max = 4.;
         value<NEportLevels={2,0}> => params.order;
         mode = "integer";
         y => ((<-.max_seg_slider.y + <-.max_seg_slider.height) + 4);
         width => <-.UIpanel.width;
      };
      UIslider vel_slider {
         parent => <-.UIpanel;
         title => "Min Velocity";
         value<NEportLevels={2,0}> => params.min_vel;
         y => ((<-.order_slider.y + <-.order_slider.height) + 4);
         width => <-.UIpanel.width;
      };
      UIradioBoxLabel UIradioBoxLabel_dir {
         parent => <-.UIpanel;
         labels<NEportLevels={2,0}> => {"backward","forward"};
         &selectedItem => params.forw_back;
         visible => <-.UIpanel.visible;
         title => "Direction";
         y => ((<-.vel_slider.y + <-.vel_slider.height) + 4);
         width => <-.UIpanel.width;
      };
      UIradioBoxLabel UIradioBoxLabel_mode {
         parent => <-.UIpanel;
         labels<NEportLevels={2,0}> => {"scalar","vector","components"};
         &selectedItem => params.vector;
         visible => <-.UIpanel.visible;
         title => "Mode";
         y => ((<-.UIradioBoxLabel_dir.y + <-.UIradioBoxLabel_dir.height) + 4);
         width => <-.UIpanel.width;
      };
      UItoggle norm_toggle {
         parent => <-.UIpanel;
         label => "Glyph Normalize";
         set<NEportLevels={2,0}> => params.normalize;
         y => ((<-.UIradioBoxLabel_mode.y + <-.UIradioBoxLabel_mode.height) + 4);
         width => <-.UIpanel.width;
      };
      UIslider scale_slider {
         parent => <-.UIpanel;
         title => "Glyph Scale";
         value<NEportLevels={2,0}> => params.scale;
         min = -10.;
         max = 10.;
         y => ((<-.norm_toggle.y + <-.norm_toggle.height) + 4);
         width => <-.UIpanel.width;
      };
      UIslider start_slider {
         parent => <-.UIpanel;
         title => "Start Time";
         value<NEportLevels={2,0}> => params.start;
         y => ((<-.scale_slider.y + <-.scale_slider.height) + 4);
         width => <-.UIpanel.width;
      };
      UIslider end_slider {
         parent => <-.UIpanel;
         title => "End Time";
         value<NEportLevels={2,0}> => params.end;
         y => ((<-.start_slider.y + <-.start_slider.height) + 4);
         width => <-.UIpanel.width;
      };
      UIslider incr_slider {
         parent => <-.UIpanel;
         title => "Step";
         max = 10.;
         value<NEportLevels={2,0}> => params.incr;
         y => ((<-.end_slider.y + <-.end_slider.height) + 4);
         width => <-.UIpanel.width;
      };
/*
      UIslider release_slider {
         parent => <-.UIpanel;
         title => "Release Interval";
         max = 10.;
         value<NEportLevels={2,0}> => params.release_interval;
         y => ((<-.incr_slider.y + <-.incr_slider.height) + 4);
         width => <-.UIpanel.width;
      };
*/
      UIslider time_step_slider {
         parent => <-.UIpanel;
         title => "Time Step Interval";
         max = 10.;
         value<NEportLevels={2,0}> => params.time_step_interval;
         y => ((<-.incr_slider.y + <-.incr_slider.height) + 4);
         //y => ((<-.release_slider.y + <-.release_slider.height) + 4);
         width => <-.UIpanel.width;
      };
      UIfieldTypein time_typein {
         UIparent => <-.UIpanel;
         flabel => "Time";
         fval<NEportLevels={2,0}> => params.count;
         panel {
            y => ((<-.<-.time_step_slider.y + <-.<-.time_step_slider.height) + 4);
            width => <-.<-.UIpanel.width;
         };
      };
      UItoggle run_toggle {
         parent => <-.UIpanel;
         label => "Run";
         set<NEportLevels={2,0}> => params.run;
         y => ((<-.time_typein.panel.y + <-.time_typein.panel.height) + 4);
         width => <-.UIpanel.width;
      };
      UItoggle reset_toggle {
         parent => <-.UIpanel;
         label => "Reset Time";
         set<NEportLevels={2,0}> => params.reset;
         y => ((<-.run_toggle.y + <-.run_toggle.height) + 4);
         width => <-.UIpanel.width;
      };
      UItoggle cycle_toggle {
         parent => <-.UIpanel;
         label => "Cycle";
         set<NEportLevels={2,0}> => params.cycle;
         y => ((<-.reset_toggle.y + <-.reset_toggle.height) + 4);
         width => <-.UIpanel.width;
      };
   };


   macro TailedAdvectorFunc {
      ilink in_fld;
      ilink in_prb;
      ilink in_glp;

      XP_COMM_PROJ.TailedAdvector.TailedAdvectorMods.TailedAdvectorParams &params<NEportLevels={2,1}>;

      DVM.DVswitch DVswitch {
         in => {in_prb, TailedAdvectorCore.new_probe};
         index => switch( ((TailedAdvectorCore.time_step_number == 0) + 1), 1, 0);
      };
      DVstream DVstream {
         in => in_fld;
         probe => DVswitch.out;
         &stream_comp => params.component;
         &order => params.order;
         &forw_back => params.forw_back;
         &nseg => params.nseg;
         &max_seg => params.max_seg;
         &min_vel => params.min_vel;
         ribbons = 0;
         rib_width = 0.;
         rib_angle = 0.;
      };
   	GMOD.loop loop {
   		&run => params.run;
   		&reset => params.reset;
   		&cycle => params.cycle;
   		&start_val => params.start;
   		&end_val => params.end;
   		&incr => params.incr;
   		&count => params.count;
   	};
      XP_COMM_PROJ.TailedAdvector.TailedAdvectorMods.TailedAdvectorCore TailedAdvectorCore {
         in => DVstream.out;
         &time => params.count;
         &direction => params.forw_back;
         &start_time => params.start;
         &release_interval => params.release_interval;
         &time_step_interval => params.time_step_interval;
         &time_step_number => params.time_step_number;
         &tracer_len => params.tracer_len;
         &tracer_style => params.tracer_style;
      };

      DVglyph DVglyph {
         in => <-.TailedAdvectorCore.out;
         glyph => <-.in_glp;
         vector => params.vector;
         glyph_comp = 0;
         map_comp = 0;
         scale_comp = 0;
         &scale => params.scale;
         &normalize => params.normalize;
         scale_x = 1;
         scale_y = 1;
         scale_z = 1;
      };

      olink out_fld => DVglyph.out;
      olink out_stream => DVstream.out;
      olink out_tracer => TailedAdvectorCore.out_tracer;
   };


   macro tailed_advector {
      ilink in_field<export_all=1>;
      ilink in_probe<export_all=1>;
      ilink in_glyph<export_all=1>;

      XP_COMM_PROJ.TailedAdvector.TailedAdvectorMods.TailedAdvectorParams params<NEportLevels=1,export_all=2> {
         component = 0;
         order = 2;
         forw_back = 1;
         nseg = 2;
         max_seg = 256;
         min_vel = 1e-005;
         run = 0;
         reset = 0;
         cycle = 0;
         start = 0.;
         end = 1.;
         incr = 0.2;
         vector = 1;
         scale = 1.;
         normalize = 0.;
         release_interval = 0.;
         time_step_interval = 0.;
         time_step_number = 0;
         tracer_len = 0;
         tracer_style = 0;
      };

      TailedAdvectorFunc TailedAdvectorFunc {
         in_fld => <-.in_field;
         in_prb => <-.in_probe;
         in_glp => <-.in_glyph;
         params => <-.params;
      };

      GMOD.instancer instancer {
         Value => UIpanel.visible;
         active = 2;
         Group => TailedAdvectorUI;
      };
      UImod_panel UIpanel {
         parent<NEportLevels={3,0}>;
         title => name_of(<-.<-);
         message = "Select advector control panel.";
      };
      TailedAdvectorUI TailedAdvectorUI<instanced=0> {
         UIpanel => <-.UIpanel;
         params => <-.params;
      };

      GDM.DataObject obj {
         in => TailedAdvectorFunc.out_fld;
         Obj {
            type = "Dynamic";
            xform_mode = "Alternate";
            alt_xform+nres => <-.<-.plane.xform;
         };
      };
      GDM.DataObject obj_stream {
         in => TailedAdvectorFunc.out_stream;
         Obj {
            xform_mode = "Alternate";
            alt_xform+nres => <-.<-.in_probe.xform;
         };
      };
      GDM.DataObject obj_tracer {
         in => TailedAdvectorFunc.out_tracer;
         Obj {
            xform_mode = "Alternate";
            alt_xform+nres => <-.<-.in_probe.xform;
         };
      };

      olink time_step_number => params.time_step_number;
      olink out_fld<export_all=2> => TailedAdvectorFunc.out_fld;
      olink out_stream<export_all=2> => TailedAdvectorFunc.out_stream;
      olink out_tracer<export_all=2> => TailedAdvectorFunc.out_tracer;
      olink out_obj => obj.obj;
      olink stream_obj => obj_stream.obj;
      olink tracer_obj => obj_tracer.obj;
   };



   macro TimeDependentAdvector {
      ilink in_glyph;
      string filename<NEportLevels={2,1}>;

      GMOD.copy_on_change copy_on_change {
         trigger => <-.time_step_number;
         input => <-.time_step_number;
         output = 0;
         on_inst = 0;
      };

      DVread_ucd DVread_ucd {
         filename => <-.filename;
         store_all = 1;
         current_step => (<-.copy_on_change.output <+> 1);
      };


      MODS.extract_component extract_component {
         in_field => <-.DVread_ucd.out;
      };
      MODS.combine_vect combine_vect {
         in_field => <-.DVread_ucd.out;
         CombineVectParam {
            components = {1,2,3};
         };
      };
      GEOMS.FPlane FPlane {
         in => <-.DVread_ucd.out;
      };

      XP_COMM_PROJ.TailedAdvector.TailedAdvectorMacs.tailed_advector tailed_advector {
         in_field => <-.combine_vect.out_fld;
         //in_field => <-.extract_component.out_fld;
         in_glyph => <-.in_glyph;
         in_probe => <-.FPlane.out_fld;

         params {
            count = 0.;
            incr = 0.1;
            scale = 0.2;
            end = 2.;
         };
      };

      int+Port time_step_number => .tailed_advector.time_step_number;

      MODS.magnitude magnitude {
         in_field => <-.tailed_advector.out_tracer;
      };

      olink out_obj => .tailed_advector.out_obj;
      olink tracer_obj => .magnitude.out_obj;
   };


   TimeDependentAdvector AltTimeDependentAdvector {
      DVread_ucd {
         filename => str_format(<-.filename + ".%d",
                                switch( (is_valid(<-.copy_on_change.output)+1), 0, <-.copy_on_change.output));
         store_all = 0;
         current_step => 1;
      };
   };


};

