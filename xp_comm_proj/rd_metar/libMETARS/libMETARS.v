// Express path: Utility.Modules.METARS.LIBMETARS

flibrary ReadDICOMLib <build_dir="xp_comm_proj/metars/Papyrus3",
                    build_cmd="$(MAKE)",
#ifdef MSDOS
      link_files="libMETARS.a"
#else
      link_files="-lmetars"
#endif 
					 >;


