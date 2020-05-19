

flibrary HtmlHelpMods <build_dir="xp_comm_proj/htmlhelp",
                       out_src_file="gen.cxx",
                       out_hdr_file="gen.h"> {

   // Pop up html help using netscape or
   // internet explorer
   module HtmlHelpCore <src_file="htmlhelp.cxx"> {

      string+IPort2 help_filename; // the help filename (.html)
      int+IPort2    show_help;

      cxxmethod  update (
         help_filename+read+req,
         show_help+notify
      );
   };

}; // end modules

