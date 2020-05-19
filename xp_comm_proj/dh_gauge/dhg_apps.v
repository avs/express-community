
flibrary DashGaugeApps{

	//
	// dashboard array system
	//

	macro DashGaugeArrayEg {

	   int+Port num_dashes = 3;
	   int+Port spacing   = 210;
	   string+Port titles[num_dashes] => {
		   "Fuel Level",
		   "Temperature",
		   "Fuel Rate"
		   };

	   GMOD.loop loop {
		   reset = 0;
		   run   => <-.UIoption.set;
		   cycle   = 1;
		   start_val = 0.0;
		   end_val = 100.0;
		   incr = 0.1;
	   };

	   float+Port t => loop.count * 0.1;

	   float+Port values[num_dashes] => {
		   (dial_1.value * (10 - t)),
		   (t  * 20.0 + dial_2.value),
		   (sin(t)*50.0 + dial_3.value)
		 };

	   // define gauges

	   //
	   // position formula based on 3 per row
	   //

	   XP_COMM_PROJ.DashGauge.DashGaugeMacs.DashGaugeA dash_gauge {
		  step_value => <-.values[0];
		  UIpanel {
			  x = 0;
			  y = 0;
          };
		  title => <-.titles[0];
		  parent => <-.UIshell;
	   };
	   XP_COMM_PROJ.DashGauge.DashGaugeMacs.DashGaugeB dash_gauge_b {
		  step_value => <-.values[1];
		  UIpanel {
			  x => 1 * <-.<-.spacing;
			  y = 0;
          };
		  title => <-.titles[1];
		  parent => <-.UIshell;
	   };

	   XP_COMM_PROJ.DashGauge.DashGaugeMacs.DashGaugeC dash_gauge_c {
		  step_value => <-.values[2];
		  UIpanel {
			  x => 2 * <-.<-.spacing;
			  y = 0; 
          };
		  title => <-.titles[2];
		  parent => <-.UIshell;
	   };

	   UIdial dial_1 {
		  parent => <-.UIshell;
		  width = 200;
		  height = 200;
		  x => 0;
		  y => <-.spacing;
		  min = 0.0;
		  max = 10.0;
		  value=5.0;
	   };

       UIdial dial_2 {
		  parent => <-.UIshell;
		  width = 200;
		  height = 200;
		  x => 1 * <-.spacing;
		  y => <-.spacing;
		  min = -5.0;
		  max = 5.0;
		  value=2.5;
	   };

	   UIdial dial_3 {
		  parent => <-.UIshell;
		  width = 200;
		  height = 200;
		  x => 2 * <-.spacing;
		  y => <-.spacing;
		  min = -50.0;
		  max = 50.0;
		  value=25.0;
	   };

	  
	   UIshell UIshell  {
		  width = 645;
		  height = 490;
		  title = "Engineering Gauge Demo";
		  menu => <-.UIcmdList;
	   };

	   Commands.UIcmdList UIcmdList {
		  cmdList => {<-.UIcmdList#1};
	   };
	   Commands.UIcmdList UIcmdList#1 {
		  cmdList => {<-.UIoption, <-.Quit};
		  label = "Controls";
	   };
	   Commands.UIoption UIoption{
		  label = "Run";
		   set = 0;
	   };


	   UIwarningDialog UIwarningDialog {
		  visible => <-.Quit.do;
		  message = "Do you really want to quit?";
		  ok<NEportLevels={1,2}>;
		  cancel = 1;
		  okButton = 1;
		  cancelButton = 1;
	   };
	   UIcmd Quit;
	   GMOD.parse_v parse_v {
		 v_commands = "-DashGaugeArrayEg;";
		 trigger => <-.UIwarningDialog.ok;
		 on_inst = 0;
		 relative => Applications;
	   };
	};


};