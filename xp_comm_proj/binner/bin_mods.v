
flibrary BinnerMods <build_dir="xp_comm_proj/binner",
                     out_hdr_file="gen.h",
                     out_src_file="gen.cxx"> {

   enum CoordinateSystemEnum {
      choices = {"cartesian", "polar", "normalized polar"};
   };


   group+OPort BinnerParams {
      CoordinateSystemEnum+Port2 coordinate_system;
		int+Port2 comp;
	};



	module BinnerCore <src_file="binner.cxx",
	                   libdeps="FLD",
                      cxx_hdr_files="fld/Xfld.h">
   {
      // Input field data
      Header+Grid+Node_Data+Float+IPort2 &fld;

      // Binner module parameters
      BinnerParams+IPort2 &BinnerParams;

      //	Coordinate type to use
      CoordinateSystemEnum coordinate_system => BinnerParams.coordinate_system;

      //	Data component to bin, ignored if ndata_bins <= 1
      int comp => BinnerParams.comp;

      //	The number of bins requested on coordinates and data.
      //	If a value is 1 don't bin along this dimension
      int+IPort2 ncoord_bins[];
      int+IPort2 ndata_bins;


      cxxmethod+req+notify_inst update (
         .fld+read+notify+req,
         .coordinate_system+read+notify+req,
         .ncoord_bins+read+notify+req,
         .ndata_bins+read+notify,
         .comp+read+notify,
         .out_fld+write
      );

      //	Output uniform field containing coordinate and data bins
      Field_Unif+Int+OPort2 out_fld;
   };

};

