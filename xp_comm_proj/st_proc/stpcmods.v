
flibrary StartProcessMods <build_dir="xp_comm_proj/st_proc",
                           out_hdr_file="gen.hxx",
                           out_src_file="gen.cxx">
{

   // define the parameter block group
   group+OPort StartProcessParams {
      int+Port2    trigger;
      int+Port2    wait_on_exit;
      string+Port2 command;
   };


   // utility module that starts a new process
   module StartProcessFunc <src_file="st_proc.cxx">
   {
      StartProcessParams+IPort2 &params;

      int trigger => params.trigger;
      int wait_on_exit => params.wait_on_exit;
      string command => params.command;

      cxxmethod+req update (
         trigger+read+req+notify,
         wait_on_exit+read+req,
         command+read+req
      );

      int+OPort2 result;
   };

};

