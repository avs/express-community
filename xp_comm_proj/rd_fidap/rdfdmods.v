
flibrary ReadFidapMods  <out_src_file="gen.cxx",
                         out_hdr_file="gen.h",
                         build_dir="xp_comm_proj/rd_fidap"> {

   group+OPort IVrdFidapInputParams {
      string+Port2 filename;
      int+Port2    read_mesh;
      int+Port2    read_ndat;
      int+Port2    time_control[3];
      int+Port2    load_items[];
   };

   group+OPort IVrdFidapOutputParams {
      int+Port2    nresults;
      string+Port2 result_labels[nresults];
      int+Port2    ntime_steps;
      int+Port2    out_ndat_dim;
      int+Port2    time_index;
   };


   module IVrdFidapResult  <src_file="rdFidap.c",
                            c_src_files="fid_util.c file_io.c header.c rd_info.c rd_mesh.c rd_ndat.c"> {

      //
      // method definitions
      //
      omethod+req update_info<status=1>(
         .filename+read+notify+req,
         .header+write,
         .nresults+write,
         .result_labels+write,
         .ntime_steps+write,
         .out_mesh+write) = "rdFidapInfo_func";

      omethod+req update_mesh <status=1>(
         .filename+read+req,
         .read_mesh+read+req+notify,
         .header+read,
         .out_mesh+write) = "rdFidapMesh_func";

      omethod+req update_ndat <status=1>(
         .filename+read+req,
         .read_ndat+read+req+notify,
         .result_labels+read+req,
         .ntime_steps+read+req,
         .time_control+read+req,
         .load_items+read+req,
         .out_ndat_dim+write,
         .time_index+write,
         .out_ndat+write) = "rdFidapNdat_func";

      //
      // inputs
      //
      string+IPort2 filename;
      int+IPort2    read_mesh;
      int+IPort2    read_ndat;
      int+IPort2    time_control[3];
      int+IPort2    load_items[];


      //
      // outputs
      //
      group header {
         string+IPort2 title;
         float revision_level;
         int int_version;
         int numnp;
         int nelem;
         int ngrps;
         int ndfcd;
         int ndfvl;
         int itim;
         int iturb;
         int ifree;
         int icompr;
         int idcts;
         int ipeny;
         int mpdf;
      };


      int+OPort2    nresults;
      string+OPort2 result_labels[nresults];
      int+OPort2    ntime_steps;
      int+OPort2    out_ndat_dim;
      int+OPort2    time_index;


      float time_table[out_ndat_dim];
      float+OPort2 time => time_table[time_index];

      Mesh out_mesh;
      Node_Data+Float out_ndat_set[];

      Node_Data &out_ndat => out_ndat_set[time_index];
      Mesh+Node_Data+OPort2 &out => merge(out_mesh,out_ndat);
   };

};

