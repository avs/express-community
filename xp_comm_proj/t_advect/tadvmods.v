
flibrary TailedAdvectorMods <build_dir="xp_comm_proj/t_advect",
                             out_src_file="gen.c",
                             out_hdr_file="gen.h">
{

   group+OPort TailedAdvectorParams {
      int+IPort2   component<animate=1>;
      int+IPort2   order<animate=1>;
      int+IPort2   forw_back<animate=1>;
      int+IPort2   nseg<animate=1>;
      int+IPort2   max_seg<animate=1>;
      float+IPort2 min_vel<animate=1>;
      int+IPort2   run<animate=1>;
      int+Port2    reset<animate=1>;
      int+IPort2   cycle<animate=1>;
      float+IPort2 start<animate=1>;
      float+IPort2 end<animate=1>;
      float+IPort2 incr<animate=1>;
      float+OPort2 count<animate=1>;
      float+IPort2 release_interval;
      float+IPort2 time_step_interval;
      int+OPort2   time_step_number;
      int+IPort2   vector<animate=1>;
      float+IPort2 scale<animate=1>;
      float+IPort2 normalize<animate=1>;
      int+IPort2   tracer_len<animate=1>;
      int+IPort2   tracer_style<animate=1>;
   };


   module TailedAdvectorCore<src_file="t_advect.c",
                             c_src_files="fld_util.c">
   {
      Mesh+Node_Data+IPort2 &in {
         nnodes+req;
         xform+nonotify;
      };

      float+IPort2 time;
      int+IPort2   direction;
      float+IPort2 start_time;
      float+IPort2 release_interval;
      float+IPort2 time_step_interval;
      int+IPort2   tracer_len;
      int+IPort2   tracer_style;

      Mesh+Node_Data isolines<NEvisible=0> {
         &xform+nowrite => in.xform;
      };

      omethod+req upd_advect(
         .in+read+notify+req,
         .time+read+notify+req,
         .direction+read+notify+req,
         .start_time+read+notify+req,
         .release_interval+read+notify+req,
         .tracer_len+read+notify+req,
         .tracer_style+read+notify+req,
         .time_step_number+write,
         .new_probe+write,
         .out+write,
         .out_tracer+write
      ) = "tailed_advect_update";

      int+OPort2 time_step_number;

      Mesh+Node_Data+OPort2 new_probe {
         &xform+nowrite => in.xform;
      };
      Mesh+Node_Data+OPort2 out {
         &xform+nowrite => in.xform;
      };
      Mesh+Node_Data+OPort2 out_tracer {
         &xform+nowrite => in.xform;
      };
   };


   macro Synchronize {
      UImod_panel UImod_panel {
         title => name_of(<-.<-);
      };
      UItoggle Run {
         parent => <-.UImod_panel;
      };
      UIbutton Reset {
         parent => <-.UImod_panel;
      };

      GMOD.copy_on_change copy_on_change#run1 {
         trigger => <-.Run.set;
         input => <-.Run.set;
         output = 0;
         on_inst = 0;
      };
      GMOD.copy_on_change copy_on_change#run2 {
         trigger => <-.Run.set;
         input => <-.Run.set;
         output = 0;
         on_inst = 0;
      };
      GMOD.copy_on_change copy_on_change#reset1 {
         trigger => <-.Reset.do;
         input => <-.Reset.do;
         output = 0;
         on_inst = 0;
      };
      GMOD.copy_on_change copy_on_change#reset2 {
         trigger => <-.Reset.do;
         input => <-.Reset.do;
         output = 0;
         on_inst = 0;
      };

      olink run1 => .copy_on_change#run1.output;
      olink run2 => .copy_on_change#run2.output;
      olink reset1 => .copy_on_change#reset1.output;
      olink reset2 => .copy_on_change#reset2.output;
   };


};

