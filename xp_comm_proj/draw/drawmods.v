
flibrary DrawMods <build_dir="xp_comm_proj/draw",
                   out_src_file="gen.cxx">
{

   group+OPort DrawLineParams {
   	int+Port2 command=0;
   	int+Port2 type=0;
   	int+Port2 x=0;
   	int+Port2 y=0;
   	int+Port2 state=0;
      GDobject_templ+Port2 &obj;
   	GDview_templ+Port2 &view;
   };

   group+OPort LineResamplerParams {
       int+Port2 in_npt;
       int+Port2 type;
       int+Port2 npts;
   };



   //
   //      Creates a region with set pixels around the input contour
   //

   module RegionResamplerCore <src_file="rgresamp.cxx",
                               out_hdr_file="rgrs_gen.h">
   {
      float+IPort2 in[];     // array of 2D points
      int+IPort2   dims[];   // dimensions of the active area

      cxxmethod+req Resampler (
         .in+read+notify,
         .dims+read+notify+req,
         .points+write
      );

      byte+OPort2 points[prod(dims)];   // resulting points
   };



   //
   //      Resamples a given contour with npts equally spaced points
   //      If requested clip the points inside a given active area
   //

   module LineResamplerCore <src_file="lnresamp.cxx",
                             out_hdr_file="lnrs_gen.h">
   {
      int+IPort2   in_npt;   // n. of input points
      float+IPort2 in[];     // array of 2D points
      int+IPort2   dims[];   // dimensions of the active area

      enum+IPort2  type {    // if the resulting points must be clipped inside the active area
         choices => {"normal", "clipped"};
      } = "normal";
      int+IPort2   npts;     // requested output points (must be no less than input points)

      int+IPort2   trigger = 0;   // trigger that copies the computed points back to the input array

      cxxmethod+req Resampler(
         .in+read+notify,
         .dims+read+notify,
         .type+read+notify,
         .npts+read+notify,
         .npoints+write,
         .points+write
      );

      cxxmethod+req UpdateInput(
         .trigger+read+write+notify+req,
         .points+read,
         .in_npt+write,
         .in+write
      );

      int   npoints;
      float+OPort2 points[npoints][2];   // resulting points
   };



   module SuperDrawCore <src_file="sup_draw.cxx",
                         out_hdr_file="sd_gen.h",
                         cxx_hdr_files="fld/Xfld.h sd_stat.h",
                         cxx_src_files="distance.cxx",
                         c_hdr_files="avs/gd.h",
                         cxx_members="   private:\n      void output_points(struct SuperDrawStatus *st);\n      int setup_environment(struct SuperDrawStatus *st);\n      void setup_pt(struct SuperDrawStatus *st);\n">
   {
      enum+IPort2 command {
         choices => {"inactive", "append", "insert", "move", "delete", "reset", "translate"};
      } = "inactive";
      enum+IPort2 line_type {
         choices => {"open", "closed"};
      } = "open";

      float+IPort2 colors[3] = {1., 0., 0.};
      int+IPort2   width = 0;

      int+IPort2 x;
      int+IPort2 y;
      int+IPort2 state;

      GDobject_templ+IPort2 &obj;
      GDview_templ+IPort2   &view;

      GDcamera_templ &camera => .view.picked_camera;
      ptr   local_ptr<NEvisible=0>;

      cxxmethod+notify_inst OnInstance(
           .local_ptr+read+write
      );

      cxxmethod+notify_deinst OnDeinstance(
           .local_ptr+read+write
      );

      cxxmethod+req OnDraw(
         .x+read,
         .y+read,
         .state+read+notify+req,
         .camera+read,
         .obj+read+notify+req,
         .view+read+req,
         .command+read+write+notify+req,
         .line_type+read+write+notify+req,
         .colors+read,
         .width+read,
         .nconn+write,
         .conn+write,
         .npoints+write,
         .points+read+write,
         .local_ptr+read
      );

      int+OPort2   npoints;
      float+OPort2 points[npoints][2];
   };


};

