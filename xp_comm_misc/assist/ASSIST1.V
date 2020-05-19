Applications.assist0 
   {
   mainbutton.ui.visible=0;
   macro assist1<NEx=0.,NEy=72.> 
      {
      string my_dir<NEx=22.,NEy=22.,NEportLevels={2,0}> => 
	     <-.mainbutton.app_home;
      string my_local_dir<NEx=176.,NEy=22.> =>  my_dir;

      string button_labels<NEx=22.,NEy=55.>[] ;
      string butlabels<NEx=400.,NEy=21>[];
      int start<NEx=22.,NEy=319.,NEportLevels={0,1}>[];
      int num_viz<NEx=176.,NEy=55.> => array_size(.button_labels);
      string beg<NEx=22.,NEy=99.,NEportLevels={0,1}>=>(my_dir + "/begin.v");
      //$echo Assist Load 2
      GMOD.load_v_script load_begin<NEx=176.,NEy=121.> 
	 {
	 filename => beg;
	 on_inst = 0;
	 };
      Shells.UIshell UIshell<NEx=22.,NEy=143.> 
	 {
	 x = 0;
	 y = 0;
	 width = 340;
	 height = 750;
     };
    Shells.UIshell UIshell1<NEx=22.,NEy=143.> 
	 {
	 x =>UIshell.x;
	 y =>UIshell.y;
	 width =>UIshell.width;
	 height =>UIshell.height;
                visible=0;
    };

      Panels.UIscrolledWindow UIscrolledWindow<NEx=22.,NEy=286.> 
	 {
	 parent => <-.UIshell;
	 x = 0;
	 y = 27;
	 virtualHeight=3000;
	 virtualWidth=3000;
	 width => <-.UIshell.clientWidth-2;
	 height => <-.UIshell.clientHeight-27;
	 };
$push
      macro frames<NEx=176.,NEy=341.>[num_viz] 
#     macro frames<NEx=176.,NEy=341.> 
	 {
	 int index<NEx=121.,NEy=11.> => index_of(<-.frames);
	 int indexp1<NEx=275.,NEy=11.> => index+1;
	 GMOD.copy_on_change copy_on_change<NEx=11.,NEy=132.>
	    {
	    input<NEportLevels={3,1}> => <-.<-.start[index];
	    };
	 int old<NEx=275.,NEy=55.> = 0;
	 link window<NEportLevels={3,1},NEx=11.,NEy=55.> => <-.UIscrolledWindow;
	 float position_y<NEx=11.,NEy=88.,NEportLevels={1,1}> =>index * 150;
	 string which_butpix<NEx=11.,NEy=209.,NEportLevels={0,1}>  => 
		   <-.my_local_dir + "/pixmaps/butpix" + indexp1 + ".x";
	 UIframe bl<NEx=176.,NEy=132.> 
	    {
	    parent<NEportLevels={2,0}> => <-.window;
	    y<NEportLevels={2,0}> => <-.position_y;
	    width = 150;
	    height = 150;
	    shadowType=> <-.toggle_unset;
	    shadowThickness=4;
	    };
UIbutton parzoom<NEx=385.,NEy=352.> {
   parent => <-.bl;
   label<NEdisplayMode="open"> => butlabels[index];
   y<NEdisplayMode="open"> => toggle.height;
   width<NEdisplayMode="open"> => bl.clientWidth;
};

macro undo<NEx=429.,NEy=451.> {
   UIbutton UIbutton<NEx=253.,NEy=77.> {

   parent => ZoomCont;

      label<NEdisplayMode="open"> => " RETURN  ";
      y => ZoomCont.clientHeight - height;
      x=10;
      visible=>1;
      };
   GMOD.parse_v parse_v<NEx=286.,NEy=187.,NEdisplayMode="showParams"> {
      v_commands = "do=0";
      no_errors=1;
      trigger => <-.UIbutton.do;
      active => <-.UIbutton.do;
      on_inst<NEdisplayMode="open"> = 0;
      relative<NEdisplayMode="open"> => <-.<-.parzoom;
   };
};


	 UIframe br0<NEx=275.,NEy=176.> 
	    {
	    parent<NEportLevels={2,0}> => <-.window;
	    x => <-.bl.width;
	    y<NEportLevels={2,0}> => <-.position_y;
	    width =>UIscrolledWindow.clientWidth-bl.clientWidth ;
	    height => <-.bl.height;
	    };
UIscrolledWindow par1<NEx=473.,NEy=264.> {
   parent => <-.br0;
   width<NEdisplayMode="open"> => (<-.br0.clientWidth - 20);
   height<NEdisplayMode="open"> => <-.br0.clientHeight;
   virtualWidth = 3000;
   virtualHeight = 3000;
   scrollBars<NEdisplayMode="open"> = 1;
   x = 0;
   y = 0;
};
group &br<NEportLevels={1,1},NEx=561.,NEy=374.> => .DVswitch.out;
DVswitch DVswitch<NEx=539.,NEy=319.> {
   in => {<-.par1,<-.par1#1};
   index => <-.switch;
};
UIscrolledWindow par1#1<NEx=583.,NEy=220.> {
   parent => <-.ZoomCont;
   width => ZoomCont.clientWidth;
   height => ZoomCont.clientHeight - undo.UIbutton.height);
   virtualWidth = 3000;
   virtualHeight = 3000;
   scrollBars<NEdisplayMode="open"> = 1;
   x = 0;
   y = 0;
};
UIpanel  ZoomCont<NEx=462.,NEy=33.> {
    parent => UIshell1;
   visible<NEdisplayMode="open"> => switch;
   x = 0;
   y = 0;
   width => Applications.assist0.assist1.UIshell1.clientWidth;
   height => Applications.assist0.assist1.UIshell1.clientHeight;

};
int switch<NEportLevels=1,NEx=506.,NEy=154.,NEdisplayMode="open">  => .parzoom.do;
macro sh_switch<NEx=352.,NEy=187.,NEwidth=528.,NEheight=319.> {
   int switch<NEportLevels={2,1},NEx=66.,NEy=22.> => <-.switch;
   boolean ui1<NEportLevels=1,NEx=22.,NEy=88.> => .switch;
   boolean ui0<NEportLevels=1,NEx=33.,NEy=143.> => (.ui1 - 1);
   GMOD.copy_on_change copy_on_change<NEx=275.,NEy=66.> {
      trigger => <-.ui1;
      input => <-.ui1;
      output => Applications.assist0.assist1.UIshell1.visible;
   };
   GMOD.copy_on_change copy_on_change#1<NEx=242.,NEy=121.,NEdisplayMode="open"> {
      trigger => <-.ui0;
      input => <-.ui0;
      output<NEdisplayMode="open"> => Applications.assist0.assist1.UIshell.visible;
   };
};

	 UItoggle toggle<NEx=132.,NEy=253.> 
	    {
	    parent => <-.bl;
/*            height => <-.bl.clientHeight-1;
	    width => <-.bl.clientWidth-1;*/
	    x = -30;
	    y = 0;
	    width  => <-.bl.clientWidth-x;
	    height => <-.bl.clientHeight-25;
	    labelPixmap.fileType = "avs_image";
	    labelPixmap 
	       {
	       fileType = "avs_image";
	       filename<NEportLevels={3,1}> => <-.<-.which_butpix;
	       };
	    set<NEportLevels={2,3}> => <-.copy_on_change.output;
	    };
	 int toggle_unset<NEx=282.,NEy=253.> => 1-toggle.set;
	 GMOD.copy_on_change toggle_set<NEx=11.,NEy=132.>
	    {
	    input<NEportLevels={3,1}> => <-.toggle.set;
	    };
	 macro int_val_change<NEx=11.,NEy=297.,NEwidth=275.,NEheight=231.> 
	    {
	    link in_int<NEportLevels={2,1},NEx=11.,NEy=22.> => <-.toggle.set;
	    GMOD.copy_on_change copy_on_change
		 <NEx=11.,NEy=60.,NEportLevels={0,1}> 
	       {
	       input => <-.in_int;
	       output<NEportLevels={0,3}>;
	       };
	    GMOD.instancer instancer<NEx=120.,NEy=90.> 
	       {
	       Value => abs(in_int-oldval);
	       Group => <-.copy_on_change;
	       };
	    int oldval<NEportLevels=1,NEx=120.,NEy=125.> => 
		    copy_on_change.output;
	   };
	 int on<NEportLevels={1,1},NEx=11.,NEy=352.> => 
	      int_val_change.copy_on_change.output;
	 int off<NEportLevels={0,1},NEx=165.,NEy=352.> => (1 - on);

	 //$echo Assist Load 3
	 GMOD.load_v_script load<NEx=11.,NEy=396.> 
	    {
	    filename => my_local_dir + "/ldbut" + indexp1 + ".v";
	    trigger<NEportLevels={2,0}> => <-.on;
	    active => <-.on;
	    on_inst => <-.toggle.set;
	    };
	 //$echo Assist Load 4
	 GMOD.load_v_script del<NEx=165.,NEy=396.> 
	    {
	    filename =>  my_local_dir + "/delbut" + indexp1 + ".v";
	    trigger<NEportLevels={2,0}> => <-.off;
	    active => <-.old*<-.off;
	    on_inst = 0;
	    };
	 UItext Explain_the_main_functions<NEx=165.,NEy=440.> 
	    {
	    visible<NEportLevels={2,0}> => <-.off;
	    parent<NEportLevels={2,0}> => <-.br;
	    text<NEportLevels={2,0}> => <-.<-.button_labels[<-.index];
	    y = 0;
	    width => <-.br0.clientWidth;
	    height => <-.br0.clientHeight;
	    resizeToText = 0;
	    multiLine = 2;
	    outputOnly = 0;
	    };
	 };
      int dummy<NEx=176.,NEy=385.,NEportLevels={1,0}>[num_viz] =>
	       frames.toggle.set;
$pop
      macro exit<NEx=176.,NEy=165.> 
	 {
	 UIbutton RETURN<NEx=11.,NEy=20.> 
	    {
	    parent<NEportLevels={2,0}> => <-.<-.UIshell;
   		color<NEdisplayMode="open"> {
      		foregroundColor = "white";
      		backgroundColor = "red";
   		};
	    x = 0;
	    y => 4;
	    width=50;
            label => "BACK";
	    };
	 //$echo Assist Load 5
	 GMOD.load_v_script do_return<NEx=11,NEy=70> 
	    {
	    filename => ("$ASSIST_HOME/exit.v");
	    trigger => <-.RETURN.do;
	    active => <-.RETURN.do;
	    on_inst=0;
	    };
	 };
//changed to button
      UIbutton Read_data<NEx=176.,NEy=209.> 
	  {
	  parent<NEportLevels={2,0}> => <-.UIshell;
   		color<NEdisplayMode="open"> {
      		foregroundColor = "white";
      		backgroundColor = "yellow";
   		};
	  width => exit.RETURN.width;
	  x => exit.RETURN.width+10;
	  y => 4;
	  label => "Rdata";
	  };

//added - reset DISPLAY WINDOW 

	 UIbutton RESET_VIEW_button<NEx=11.,NEy=22.> 
	      {
	      parent<NEportLevels={2,0}> => <-.UIshell;
   		color<NEdisplayMode="open"> {
      		foregroundColor = "white";
      		backgroundColor = "blue";
   		};
	      x=>Read_data.x+Read_data.width+10;
	      y => 4;
	      width =>Read_data.width;
	      label = "Lview";
	      do = 1;
	      }; 
       GMOD.load_v_script view_reset<NEx=11,NEy=70> 
	    { 
	    filename => ("$ASSIST_HOME/view.v"); 
	    trigger => <-.RESET_VIEW_button.do; 
	    active => <-.RESET_VIEW_button.do; 
	    on_inst=0; 
	    }; 
       
//added - REDUCE DISPLAY WINDOW 

	 UIbutton RED_VIEW_button<NEx=15.,NEy=22.> 
	      {
	      parent<NEportLevels={2,0}> => <-.UIshell;
   		color<NEdisplayMode="open"> {
      		foregroundColor = "white";
      		backgroundColor = "red";
   		};
	      x=>Read_data.x+Read_data.width+10;
	      y => 4;
	      width => Read_data.width;
	      label = "Sview";
	      do = 1;
	      }; 
       GMOD.load_v_script view_reduce<NEx=11,NEy=70> 
	    { 
	    filename => ("$ASSIST_HOME/redview.v"); 
	    trigger => <-.RED_VIEW_button.do; 
	    active => <-.RED_VIEW_button.do; 
	    on_inst=1; 
	    }; 









      macro norm<NEx=176.,NEy=253.> 
	 {
//added - usually to perform reset/normalize/center
	 UIbutton resview<NEx=11.,NEy=22.> 
	      {
	      parent<NEportLevels={3,0}> => <-.<-.UIshell;
   		color<NEdisplayMode="open"> {
      		foregroundColor = "white";
      		backgroundColor = "green";
   		};
	      x=> ((RESET_VIEW_button.x + RESET_VIEW_button.width) + 10);
	      y => 4;
	      width => RESET_VIEW_button.width;
	      label = "reset";
	      do = 1;
	      };
	  GMOD.parse_v parse_v_reset<NEx=11.,NEy=60.> 
	      {
	      trigger => <-.resview.do;
	      on_inst=0;
	      active => <-.resview.do;
	      v_commands = "";
	      };
	 UIbutton RE_NORM_CE<NEx=190.,NEy=22.> 
	      {
	      parent<NEportLevels={3,0}> => <-.<-.UIshell;
   		color<NEdisplayMode="open"> {
      		foregroundColor = "white";
      		backgroundColor = "red";
   		};
	      x=>resview.x+resview.width+10;
	      y => 4;
	      width => resview.width;
	      label = "Norm.";
	      do = 0;
	      };
	  GMOD.parse_v parse_v<NEx=190.,NEy=60.> 
	      {
	      trigger => <-.RE_NORM_CE.do;
	      on_inst=0;
	      active => <-.RE_NORM_CE.do;
	      v_commands = "";
	      };
	  };
/*
      UIscrolledWindow.virtualHeight=>152*num_viz;
      UIscrolledWindow.virtualWidth=>650;
*/
      norm.resview.active=1;
      norm.resview.do=1;
      };
   };


