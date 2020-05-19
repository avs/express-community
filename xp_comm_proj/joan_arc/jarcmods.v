
flibrary JoanArcMods <build_dir="xp_comm_proj/joan_arc",
                      out_src_file = "gen.cxx",
                      out_hdr_file = "gen.hxx">
{

   module JoanArcCore <src_file = "joan_arc.cxx">
   {
      float+IPort2 start[][3];
      float+IPort2 end[][3];
      float+IPort2 vert[] = {0};
      int+IPort2   nseg = 20;

      cxxmethod+notify_inst+req update (
         start+read+notify+req,
         end+read+notify+req,
         vert+read+notify+req,
         nseg+read+notify+req,
         ncoords+write,
         polyline_out+write,
         connections_out+write
      );

      int ncoords;
      float+OPort2 polyline_out[ncoords][3];
      int+OPort2   connections_out[];
   };

};

