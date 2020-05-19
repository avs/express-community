
flibrary SetCursorMods <build_dir="xp_comm_proj/setcur",
                        out_hdr_file="gen.h">
{
   module SetCursorUtilityCode<NEvisible=0,src_file="cursor.c">;

   module SetCursorExampleMod<src_file="cur_eg.c"> {
      int+IPort2 busy_enable;

      omethod+notify_inst+req update(
         .busy_enable+read+notify+req
      ) = "SetCursorExampleMod_update";
   };

};

