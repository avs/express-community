
flibrary PresentDemoMods <compile_subs=0>
                       /*<build_dir="xp_comm_proj/p_demo",
                          out_src_file="gen.cxx",
                          out_hdr_file="gen.hxx">*/
{

   group+OPort PresentDemoParams {
      string+Port2 title;
      int+Port2 rotate;
      int+Port2 scale;
      int+Port2 translate;
      int+Port2 rnc;

      int+Port2 use_mpu;
      int+Port2 show_menu;
   };

};

