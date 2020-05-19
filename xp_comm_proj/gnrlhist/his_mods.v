flibrary GeneralHistogramMods <build_dir="xp_comm_proj/gnrlhist",
		       process="express",
		       out_hdr_file="gen.h",
		       out_src_file="gen.cxx"> {

   group GeneralHistogramParams <NEportLevels={0,1}> {
       int nbins <NEportLevels={2,2}>;
       prim ignore_min <NEportLevels={2,2}>;
       prim ignore_max <NEportLevels={2,2}>;
       boolean keep_range <NEportLevels={2,2}>;
       double mean<NEportLevels={2,2}>;
       double std_deviation<NEportLevels={2,2}>;
   };

    module GeneralHistogramNodeData  {
      	 Node_Data &in<NEportLevels={2,0}>;
         GeneralHistogramParams &GHParams <NEportLevels={2,1}>;
     	 int nbins => GHParams.nbins;
         prim ignore_min => GHParams.ignore_min;
         prim ignore_max => GHParams.ignore_max;
         boolean keep_range => GHParams.keep_range;
     	 prim binval<NEportLevels={1,2}>[];
      	 int binfreq<NEportLevels={1,2}>[];
       omethod+notify_inst+req update<src_file="genhisto.c">(
             in+read+notify+req,
             nbins+read+notify,
             ignore_min+read+notify+nres,
             ignore_max+read+notify+nres,
             keep_range+read+notify,
             binval+write,
             binfreq+write ) = "generalhistogram";
    };

    module GenerateStats  {
     	 prim binval<NEportLevels={2,1}>[];
      	 int binfreq<NEportLevels={2,1}>[];
         double mean<NEportLevels={1,2}>;
         double std_deviation<NEportLevels={1,2}>;
       omethod+notify_inst+req update<src_file="genstats.c">(
             binval+read+notify+req,
             binfreq+read+notify+req,
             mean+write,
             std_deviation+write ) = "generatestats";
    };



};
