// Express path: Utility.Modules.METARS.LIBMETARS

flibrary ReadDICOMLib <
                    build_dir="xp_comm_proj/rd_dicom/Papyrus3",
                    build_cmd="$(MAKE)",
#ifdef MSDOS
      link_files="PC/lib/Papyrus3.lib"
#else
      link_files="-L./xp_comm_proj/rd_dicom/Papyrus3/${MACHINE}/lib -lP -lDicomdir -lJpglless -lJpglossy"
#endif 
>;
