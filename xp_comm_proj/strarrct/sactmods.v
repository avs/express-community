
flibrary StringArrayConcatenateMods <build_dir = "xp_comm_proj/strarrct",
	   	                out_hdr_file = "gen.h",
		                out_src_file = "gen.cxx"> {

	group+OPort StringArrayConcatenateParams {
		string+Port2 leader;
		string+Port2 separator;
		string+Port2 trailer;
		int+Port2 trigger;
	};

	// define the low-level module

	module StringArrayConcatenateCore <src_file = "strarrct.cxx"> {
		int+IPort2 trigger;
		int num_strings => array_size(input_strings);
		string+IPort2 input_strings[];
		string+IPort2 leader_string = "";
		string+IPort2 separator_string = "";
		string+IPort2 trailer_string = "";
		string+OPort2 output_string;
		int+OPort2 done;

		cxxmethod+notify_inst+req update(
			trigger+notify,
			num_strings+read+nonotify,
			input_strings+read+notify,
			leader_string+read+nonotify+req,
			separator_string+read+nonotify+req,
			trailer_string+read+nonotify+req,
			output_string+write+nonotify,
			done+write+nonotify
		);
	};
};
