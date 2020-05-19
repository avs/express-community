
flibrary IsonestMods <build_dir="xp_comm_proj/isonest"> {

      module group_swap_order <src_file="gswap.c",
                               out_src_file="gswap_gen.cxx",
                               out_hdr_file="gswap_gen.h">
      {
 
         group+read+notify+req &in_groups<NEportLevels={2,0}>[];
         group+write+nonotify  &out_groups<NEportLevels={0,2}>[];

         omethod+notify+req update = "group_swap_order";
      };

      module Dmap2Color  <src_file="dmap2col.cxx",
                          out_src_file="d2c_gen.cxx",
                          out_hdr_file="d2c_gen.hxx",
                          export_cxx=1> {

         cxxmethod+req+notify_inst update (
            X_Resolution+read+notify+req,
            dmap+notify,
            values+read+notify,
            output+write,
            output_dims+write
         );

         DMAP.DatamapTempl+IPort2 &dmap<export=0>;
         float+IPort2             values[];
         int+IPort2               X_Resolution;
         int+OPort2               output_dims[1];
         float+OPort2             output[(output_dims[0])*3];
      };

     group IsonestParams <NEportLevels={0,1}>{
          float min;          // lower range boundary
          float max;          // upper range boundary
          int num_levels;     // number of isosurfaces
          int num_component;  // which data component to use
          float transparency; // base transparency level
          int   trans_ramp;   // transparency - all the same or ramped
          float ramp_delta;   // width of range to vary transparency

      };

};

