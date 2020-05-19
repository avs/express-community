
flibrary DisplayErrorMods <build_dir="xp_comm_proj/diserr",
                           out_hdr_file="gen.h">
{
   module DisplayErrorUtilityCode<NEvisible=0,src_file="error.c">;

   module DisplayErrorExampleMod<src_file="err_eg.c"> {
      int+IPort2 trigger;
      string+IPort2 title;
      string+IPort2 message;

      omethod+notify_inst+req update(
         .trigger+notify,
         .title+read+notify+req,
         .message+read+notify+req
      ) = "DisplayErrorExampleMod_update";
   };


   // The error/warning dialog macro, a group containing an
   // error dialog box with title and message. The C utility
   // function sets the visible subobject on demand.
   group Error {
      ilink parent;
      int visible = 0;
      string title;
      string message;

      UIwarningDialog dialog {
         parent => <-.parent;
         isModal = 1;
         visible => <-.visible;
         title => <-.title;
         message => <-.message;
      };
   };

};

