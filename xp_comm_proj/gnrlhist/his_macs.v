flibrary GeneralHistogramMacs {


    macro GeneralHistogramUI {
       link in_fld<NEportLevels={2,1}>;
       XP_COMM_PROJ.GeneralHistogram.GeneralHistogramMods.GeneralHistogramParams
            &GeneralHistogramParams<NEx=660.,NEy=77.,NEportLevels={2,0}> ;
       UImod_panel UImod_panel {
          title => "General Histogram";
       };
       UIslider ignore_max {
	  parent => <-.UImod_panel;
	  value => <-.GeneralHistogramParams.ignore_min;
	  x = 10;
	  y = 10;
	  min+nres => <-.in_fld.node_data[0].min;
	  max+nres => <-.in_fld.node_data[0].max;
          message="minimum bound of values not to be ignored";
       };
       UIslider ignore_min {
	  parent => <-.UImod_panel;
	  value => <-.GeneralHistogramParams.ignore_max;
	  x = 10;
	  y = 70;
	  min+nres => <-.in_fld.node_data[0].min;
	  max+nres => <-.in_fld.node_data[0].max;
          message="maximum bound of values not to be ignored";
       };
       UItoggle keep_range {
	  parent => <-.UImod_panel;
	  label => "Keep range?";
	  set => <-.GeneralHistogramParams.keep_range;
	  x = 10;
	  y = 140;
          message="Keep original range of data or just show non-ignored";
       };
       UIfield nbins {
	  parent => <-.UImod_panel;
	  value => <-.GeneralHistogramParams.nbins;
	  x = 54;
	  y = 190;
          message="Number of bins in histogram";
       };
       UIlabel nbins_lable {
	  parent => <-.UImod_panel;
	  label => "nbins:";
	  x = 10;
	  y = 195;
	  width = 44;
          alignment= "right";
       };
       UIframe stats_frame {
          parent => <-.UImod_panel;
          x= 10;
          y= 250;
          width= 230;
          height= 140;
       };
       UIlabel stats_label {
	  parent => <-.stats_frame;
	  label => "Statistics";
	  x = 10;
	  y = 10;
	  width = 90;
          alignment= "left";
       };
       UIfield mean {
	  parent => <-.stats_frame;
	  value => <-.GeneralHistogramParams.mean;
          outputOnly= 1;
	  x = 100;
	  y = 40;
          message="mean of values in histogram";
       };
       UIlabel mean_lable {
	  parent => <-.stats_frame;
	  label => "mean:";
	  x = 32;
	  y = 45;
	  width = 60;
          alignment= "right";
       };
       UIfield stddev {
	  parent => <-.stats_frame;
	  value => <-.GeneralHistogramParams.std_deviation;
          outputOnly= 1;
	  x = 100;
	  y = 90;
          message="standard deviation of values in histogram";
       };
       UIlabel stddev_lable {
	  parent => <-.stats_frame;
	  label => "std deviation:";
	  x = 10;
	  y = 95;
	  width = 86;
          alignment= "right";
       };
    };


    macro GeneralHistogram {
       XP_COMM_PROJ.GeneralHistogram.GeneralHistogramMods.GeneralHistogramNodeData GeneralHistogramNodeData {
	  in<NEportLevels={3,0}>;
	  GHParams => <-.GeneralHistogramParams;
	  binval<NEportLevels={1,3}>;
	  binfreq<NEportLevels={1,3}>;
       };
       XP_COMM_PROJ.GeneralHistogram.GeneralHistogramMods.GenerateStats GenerateStats {
          binval+nres => <-.GeneralHistogramNodeData.binval;
          binfreq+nres => <-.GeneralHistogramNodeData.binfreq;
          mean+nres => <-.GeneralHistogramParams.mean;
          std_deviation+nres => <-.GeneralHistogramParams.std_deviation;
       };
       XP_COMM_PROJ.GeneralHistogram.GeneralHistogramMods.GeneralHistogramParams &GeneralHistogramParams<NEportLevels={2,1}>;
    };
    

    macro general_histogram {
       link+nres in<NEportLevels={2,1}>;

       XP_COMM_PROJ.GeneralHistogram.GeneralHistogramMods.GeneralHistogramParams GeneralHistogramParams{
             ignore_min+nres => cache(in.node_data[0].min);
             ignore_max+nres => cache(in.node_data[0].max);
 
       };

       XP_COMM_PROJ.GeneralHistogram.GeneralHistogramMacs.GeneralHistogram GeneralHistogram {
	  GeneralHistogramNodeData {
	     in => <-.<-.in;
	     binval<NEportLevels={1,4}>;
	     binfreq<NEportLevels={1,4}>;
	  };
          GenerateStats {
             mean<NEportLevels={1,4}>;
             std_deviation<NEportLevels={1,4}>;
          };
	  &GeneralHistogramParams => <-.GeneralHistogramParams;
       };


       XP_COMM_PROJ.GeneralHistogram.GeneralHistogramMacs.GeneralHistogramUI GeneralHistogramUI {
	  &GeneralHistogramParams => <-.GeneralHistogramParams;
	  in_fld => <-.in;
	  UImod_panel {
	    title => name_of(<-.<-.<-);
          };
       };

    };





    APPS.SingleWindowApp GeneralHistogramEg<NEdisplayMode="maximized"> {
       UI {
	  Modules {
	     IUI {
		optionList {
		   selectedItem = 1;
		};
	     };
	  };
       };
       GDM.Uviewer2D Uviewer2D<NEx=603,NEy=453> {
	  Scene {
	     Top {
		child_objs => {
		   <-.<-.<-.General_Graph.GraphViewport.obj};
	     };
	     View {
		View {
		   trigger = 1;
		};
	     };
	  };
       };
       MODS.Read_Field Read_Field<NEx=220.,NEy=88.> {
	  read_field_ui {
	     file_browser {
		x = 490;
		y = 267;
		width = 300;
		height = 390;
		ok = 1;
		dirMaskCache = "$XP_ROOT/data/field/*.fld";
	     };
	     filename = "$XP_ROOT/data/field/lobster.fld";
	  };
	  DVread_field {
	     Mesh_Unif+Node_Data Output_Field;
	  };
       };
       XP_COMM_PROJ.GeneralHistogram.GeneralHistogramMacs.general_histogram general_histogram<NEx=385.,NEy=209.> {
	  in => <-.Read_Field.field;
	  GeneralHistogramParams {
	     ignore_min = 149.05;
	     ignore_max = 255.;
	     keep_range = 0;
	  };
	  GeneralHistogramUI {
	     UImod_panel {
		option {
		   set = 1;
		};
	     };
	     ignore_min<NEx=176.,NEy=385.>;
	  };
       };
       AGHLM.General_Graph General_Graph<NEx=495.,NEy=319.> {
	  ValuesX => <-.general_histogram.GeneralHistogram.GeneralHistogramNodeData.binval;
	  ValuesY1 => <-.general_histogram.GeneralHistogram.GeneralHistogramNodeData.binfreq;
	  Graph#1 {
	     Gparams {
		Graph_params1 {
		   graphtype = 1;
		};
	     };
	  };
	  Graph#2 {
	     UIframe<instanced=0>;
	  };
	  Graph#3 {
	     UIframe<instanced=0>;
	  };
	  Graph#4 {
	     UIframe<instanced=0>;
	  };
	  Graph#5 {
	     UIframe<instanced=0>;
	  };
	  GeneralGraph {
	     option {
		set = 0;
	     };
	  };
	  SelectParam {
	     label_cmd {
		cmd = {
		   {
		      set=1,,,
		   },,,,
		   {
		      set=0,,,
		   }
		};
	     };
	  };
	  GraphWorld {
	     GraphWorldUI {
		scale_type_X {
		   label_cmd {
		      cmd[];
		   };
		};
		scale_type_Y {
		   label_cmd {
		      cmd[];
		   };
		};
	     };
	  };
       };
    };



};
