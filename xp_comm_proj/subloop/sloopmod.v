
flibrary SubLoopMods <build_dir="xp_comm_proj/subloop",out_hdr_file="gen.h"> {

	group+OPort SubLoopParams {
		int+Port2 run;
		int+Port2 run_back;
		int+Port2 step;
		int+Port2 step_back;
		int+Port2 reset;
		int+Port2 reset_back;
		int+Port2 cycle;
		float+Port2 start;		
		float+Port2 end;
		float+Port2 incr;		
		float+Port2 count;		
	};

	module SubLoopCore <src_file="subloop.c">
	{
		int+IPort2 trigger;
		int+IPort2 run;
		int+IPort2 run_back;
		int+IPort2 step;
		int+IPort2 step_back;
		int+IPort2 reset;
		int+IPort2 reset_back;
		int+IPort2 cycle;
		float+Port2 start;		
		float+Port2 end;
		float+Port2 incr;		
		int direction <NEvisible=0> = 1;
	    omethod+notify_inst+req update(
	    	.run+read+write,
	   	 	.run_back+read+write,
	   	 	.trigger+notify,
			.step+read+write+notify,
  			.step_back+read+write+notify,
			.reset+read+write+notify,
  			.reset_back+read+write+notify,
  			.cycle+read+req,
			.start+read+req,
			.end+read+req,
			.incr+read+req,
    		.count+read+write+req,
			.direction+read+write+req,
			.done+write) = "subloop";
		float+Port2 count;
		int+Port2 done = 0;
	};

};
