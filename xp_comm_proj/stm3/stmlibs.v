flibrary STM3libs {
	flibrary libSTM3 <NEvisible=0,
				build_dir="xp_comm_proj/stm3/lib",
				build_cmd="$(MAKE)"
			>;

	flibrary libsginfo <NEvisible=0,
				build_dir="xp_comm_proj/stm3/extern/sginfo",
				build_cmd="$(MAKE) all"
			>;
			
	flibrary libANN <NEvisible=0,
				build_dir="xp_comm_proj/stm3/extern/ann",
#ifdef MSDOS
				build_cmd="$(MAKE)"
#else
				build_cmd="$(MAKE) $(MACHINE)"
#endif
			>;
};
