
// Comment out the lines below if the Animator Kit is not available
#define ANIM_KIT_ENABLED

// Edit the lines below to point to the appropriate zlib library
#define ZLIB_LINK_DOS  "zlibdll.lib"
#define ZLIB_LINK_UNIX "-lz"



#ifdef MSDOS
#define ZLIB_HDR_DIRS "$(XP_PATH0)\\animator\\zlib include ."
#define ZLIB_LINK ZLIB_LINK_DOS
#else
#define ZLIB_HDR_DIRS $(XP_PATH0)/animator/zlib include ."
#define ZLIB_LINK ZLIB_LINK_UNIX
#endif



// Defines library used to hold low-level components
// Also specifies details of how modules should be compiled
flibrary ZipIOMods <build_dir="xp_comm_proj/zip_io",
                    out_hdr_file="gen.h",
                    out_src_file="gen.cxx",
#ifdef ANIM_KIT_ENABLED
#ifdef ANIM_KIT
                    hrd_dirs=ZLIB_HDR_DIRS,
#else
                    link_files=ZLIB_LINK,
#endif
#else
                    link_files=ZLIB_LINK,
#endif
                    c_src_files="fn_util.c"> {


   // Common Parameter Block
   // Enables easy connection of separate components
   group+OPort ZipIOParams {
      string+Port2 filename;
      string+Port2 output_dir;
      int+Port2    remove_orig;
   };


   // Low-level modules.
   // Acts as wrappers for the project C, C++ or Fortran code

   module CompressFileCore <src_file="comp.c"> {
      string+IPort2 &in_filename;

      // Reference to external parameter block
      ZipIOParams+IPort2 &params;

      omethod+notify_inst+req update (
         .in_filename+read+notify,
         .params+read+req+notify,
         .params.filename+read+notify,
         .params.output_dir+read+req+notify,
         .params.remove_orig+read+req+notify,
         .out_filename+write
      ) = "zipio_compress";

      string+OPort2 out_filename;
   };


   module DecompressFileCore <src_file="decomp.c"> {
      string+IPort2 &in_filename;

      // Reference to external parameter block
      ZipIOParams+IPort2 &params;

      omethod+notify_inst+req update (
         .in_filename+read+notify,
         .params+read+req+notify,
         .params.filename+read+notify,
         .params.output_dir+read+req+notify,
         .params.remove_orig+read+req+notify,
         .out_filename+write
      ) = "zipio_decompress";

      string+OPort2 out_filename;
   };

};

