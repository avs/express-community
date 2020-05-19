
library ReadHelpMods <build_dir="xp_comm_proj/rd_help",out_hdr_file = "gen.h">{
	// module: ReadHelpCore
	// purpose: Read ascii text from file, pad to specified width/height

	//Define the parameter block 
	group+OPort ReadHelpParams {
		int+Port2 width;
		string+Port2 filename;
		string+Port2 text;
		int+Port2 rows;
		int+Port2 cols;
		int+Port2 active;
	};

	module ReadHelpCore<src_file="rd_help.c"> {

		//Define the inputs
		ReadHelpParams+IPort2 &params;
		int+IPort width => params.width;
		string+IPort filename => params.filename;		
		omethod+notify_inst+req update(
			.width+read+notify+req,
			.filename+read+notify+req,
			.text+write,
			.rows+write,
			.cols+write
		) = "readHelp";

		//Define the outputs
		string+OPort2 text=>params.text;
		int+OPort2 rows=>params.rows;
		int+OPort2 cols=>params.cols; 	
	};
};

