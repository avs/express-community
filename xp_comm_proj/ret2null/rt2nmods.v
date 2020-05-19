
flibrary Ret2NullMods <build_dir="xp_comm_proj/ret2null",
                       out_hdr_file="gen.h",
                       out_src_file="gen.c">
{

   module ReturnToNull<src_file="ret2null.c"> {
      UIfield+IPort2 &ui_field;
      int+IPort2 trigger;

      omethod+req update (
         ui_field+read+notify+req,
         trigger+read+notify+req
      ) = "ret2null_update";
   };


};

