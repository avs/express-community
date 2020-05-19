
#include "xp_comm_proj/subloop/gen.h"


int
subloop(OMobj_id SubLoopCore_id, OMevent_mask event_mask, int seq_num)
{
	/***********************/
	/*	 Declare variables  */
	/***********************/
	int  run;
	int  run_back;
	int  step;
	int  step_back;
	int  reset;
	int  reset_back;
	int  cycle;
	double  start;
	double  end;
	double  incr;
	int  direction;
	double  count;
	int  done=0;

	/***********************/
	/*	 Get input values	  */
	/***********************/
	/* Get run's value */ 
	if (OMget_name_int_val(SubLoopCore_id, OMstr_to_name("run"), &run) != 1) 
		run = 0;

	/* Get run_back's value */ 
	if (OMget_name_int_val(SubLoopCore_id, OMstr_to_name("run_back"), &run_back) != 1) 
		run_back = 0;

	/* Get step's value */ 
	if (OMget_name_int_val(SubLoopCore_id, OMstr_to_name("step"), &step) != 1) 
		step = 0;

	/* Get step_back's value */ 
	if (OMget_name_int_val(SubLoopCore_id, OMstr_to_name("step_back"), &step_back) != 1) 
		step_back = 0;

	/* Get reset's value */ 
	if (OMget_name_int_val(SubLoopCore_id, OMstr_to_name("reset"), &reset) != 1) 
		reset = 0;

	/* Get reset_back's value */ 
	if (OMget_name_int_val(SubLoopCore_id, OMstr_to_name("reset_back"), &reset_back) != 1) 
		reset_back = 0;

	/* Get cycle's value */ 
	if (OMget_name_int_val(SubLoopCore_id, OMstr_to_name("cycle"), &cycle) != 1) 
		cycle = 0;

	/* Get start's value */
	if (OMget_name_real_val(SubLoopCore_id, OMstr_to_name("start"), &start) != 1)
		start = 0.0;

	/* Get end's value */
	if (OMget_name_real_val(SubLoopCore_id, OMstr_to_name("end"), &end) != 1)
		end = 0.0;

	/* Get incr's value */
	if (OMget_name_real_val(SubLoopCore_id, OMstr_to_name("incr"), &incr) != 1)
		incr = 0.0;

	/* Get direction's value */ 
	if (OMget_name_int_val(SubLoopCore_id, OMstr_to_name("direction"), &direction) != 1) 
		direction = 0;

	/* Get count's value */
	if (OMget_name_real_val(SubLoopCore_id, OMstr_to_name("count"), &count) != 1)
		count = 0.0;


	/***********************/
	/* Function's Body	  */
	/***********************/

	do
	{
		if (reset)
		{
			reset=0;
			run=0;
			run_back=0;
			count=start;
			direction=1;
			done=0;
			break;
		}; 
		if (reset_back)
		{
			reset_back=0;
			run=0;
			run_back=0;
			count=end;
			direction=1;
			done=0;
			break;
		};
		done=0;
		switch (cycle)
		{ 
			case 0: /* once */
			{ 
				if (step || run) 
					if (count<end) 
					{
						count += incr;
						step = 0;
						if (count >= end)
						{
							done = 1;
							run = 0;
							count = end;
						}
						break;
					} else {
						ERRverror("",ERR_NO_HEADER | ERR_INFO,"Can't run past end value!\n");					
						done = 0;
						run = 0;
						step = 0;
						break;
					};
				if (step_back || run_back)
					if (count>start) 
					{
						count -= incr;
						step_back = 0;
						if (count <= start)
						{
							done = 1;
							count = start;
							run_back = 0;
						}
						break;
					} else {
						ERRverror("",ERR_NO_HEADER | ERR_INFO,"Can't run back past start value!\n");					
						done = 0;
						run_back = 0;
						step_back = 0;
						break;
					};
				break;
			};
			case 1: /* cycle */ 
			{ 
				if (step || run) 
				{
					count += incr;
					step = 0;
					if (count > end)
						count = start;
					break;
				};
				if (step_back || run_back)
				{
					count -= incr;
					step_back = 0;
					if (count < start)
						count = end;
					break;
				};
				break;
			};
			case 2: /* bounce */
			{ 
				if (step || run) 
				{
					count += direction*incr;
					step = 0;
					if (direction<0) 
					{
						if (count <= start)
						{
							direction *= -1;
							count = start;
						};
					} else {
						if (count >= end)
						{
							direction *= -1;
							count = end;
						};
					};					
					break;
				};
				if (step_back || run_back)
				{
					count -= direction*incr;
					step_back = 0;
					if (direction>0) 
					{
						if (count <= start)
						{
							direction *= -1;
							count = start;
						};
					} else {
						if (count >= end)
						{
							direction *= -1;
							count = end;
						};
					};					
					break;
				};
				break;
			};
		}
	}
	while (0);

	/***********************/
	/*	 Set output values  */
	/***********************/
	/* Set run's value */  
	OMset_name_int_val(SubLoopCore_id, OMstr_to_name("run"), run);
	/* Set run_back's value */	 
	OMset_name_int_val(SubLoopCore_id, OMstr_to_name("run_back"), run_back);
	/* Set step's value */	
	OMset_name_int_val(SubLoopCore_id, OMstr_to_name("step"), step);
	/* Set step_back's value */  
	OMset_name_int_val(SubLoopCore_id, OMstr_to_name("step_back"), step_back);
	/* Set reset's value */	 
	OMset_name_int_val(SubLoopCore_id, OMstr_to_name("reset"), reset);
	/* Set reset_back's value */	
	OMset_name_int_val(SubLoopCore_id, OMstr_to_name("reset_back"), reset_back);
	/* Set direction's value */  
	OMset_name_int_val(SubLoopCore_id, OMstr_to_name("direction"), direction);
	/* Set count's value */
	OMset_name_real_val(SubLoopCore_id, OMstr_to_name("count"), count);
	/* Set done's value */
	OMset_name_int_val(SubLoopCore_id, OMstr_to_name("done"), done);
	return(1);
}
