
flibrary ConfocalApps{

	APPS.DefaultApplication Confocal_Microscopy_Demo{
	   Scheduler.status_check = 0;
	   macro Image_Viewer{
		  group+IPort2 &params => <-.User_Interface.Params;
		  macro ViewerPanel{
			 UIrenderView Channel_1{
				parent => <-.parent;
				y = 0;
				width => (parent.clientWidth / 2);
				height => (parent.clientHeight / 2);
			 };
			 UIrenderView Channel_2{
				parent => <-.parent;
				y = 0;
				x => (parent.clientWidth / 2);
				width => (parent.clientWidth / 2);
				height => (parent.clientHeight / 2);
			 };
			 UIrenderView Channel_3{
				parent => <-.parent;
				y => (parent.clientHeight / 2);
				width => (parent.clientWidth / 2);
				height => (parent.clientHeight / 2);
			 };
			 UIrenderView Channel_4{
				parent => <-.parent;
				y => (parent.clientHeight / 2);
				x => (parent.clientWidth / 2);
				width => (parent.clientWidth / 2);
				height => (parent.clientHeight / 2);
			 };
			 GDM.Mscene2D Mscene2D{
				Top {
				   child_objs<NEportLevels={4,1}> => {
					  <-.<-.<-.DataObject.obj};
				   Xform {
					  dcenter = {-2012.2,-2011.65,-118.07};
				   };
				};
				View {
				   View {
					  handle => <-.<-.<-.Channel_1.handle;
					  trigger = 2;
				   };
				};
			 };
			 GDM.Mscene2D Mscene2D#1{
				Top {
				   child_objs<NEportLevels={4,1}> => {
					  <-.<-.<-.DataObject#1.obj};
				   Xform {
					  dcenter = {-1760.7,-1760.15,-137.734};
				   };
				};
				View {
				   View {
					  handle => <-.<-.<-.Channel_2.handle;
					  trigger = 2;
				   };
				};
			 };
			 GDM.Mscene2D Mscene2D#2{
				Top {
				   child_objs<NEportLevels={4,1}> => {
					  <-.<-.<-.DataObject#2.obj};
				   Xform {
					  dcenter = {-1509.21,-1508.66,-10.8341};
				   };
				};
				View {
				   View {
					  handle => <-.<-.<-.Channel_3.handle;
					  trigger = 2;
				   };
				};
			 };
			 GDM.Mscene2D Mscene2D#3{
				Top {
				   child_objs<NEportLevels={4,1}> => {
					  <-.<-.<-.DataObject#3.obj};
				   Xform {
					  dcenter = {-2012.2,-2011.65,-115.119};
				   };
				};
				View {
				   View {
					  handle => <-.<-.<-.Channel_4.handle;
					  trigger = 2;
				   };
				};
			 };
			 link parent<NEportLevels={3,1}> => <-.<-.User_Interface.Viewer_Panel;
		  };
		  DataObject DataObject{
			 in => <-.DVorthoslice.out;
			 Datamap {
				currentColorModel = 1;
				DatamapValue = {
				   {
					  v4=0.,v3=0.,v2=0.,
				   },
				   {
					  v4=0.,v3=0.,v2=1.,v1=0.
				   }};
			 };
			 Obj {
				visible<NEportLevels={2,0}> => <-.valid;
			 };
			 ilink valid => <-.Channel_1_valid;
		  };
		  DataObject DataObject#1{
			 in => <-.DVorthoslice#1.out;
			 Datamap {
				currentColorModel = 1;
				DatamapValue = {
				   {
					  v4=0.,v3=0.,v2=0.,
				   },
				   {
					  v4=0.,,,
				   }};
			 };
			 Obj {
				visible<NEportLevels={2,0}> => <-.visible;
			 };
			 ilink visible => <-.Channel_2_valid;
		  };
		  DataObject DataObject#2{
			 in => <-.DVorthoslice#2.out;
			 Datamap {
				currentColorModel = 1;
				DatamapValue<NEx=341.,NEy=154.> = {
				   {
					  v4=0.,v3=0.,v2=0.,
				   },
				   {
					  v3=0.,,
				   }};
			 };
			 Obj {
				visible<NEportLevels={2,0}> => <-.visible;
			 };
			 ilink visible => <-.Channel_3_valid;
		  };
		  MODS.combine_rgb+OPort combine_rgb {
			 in_field1 => <-.DVorthoslice.out;
			 in_field2 => <-.DVorthoslice#1.out;
			 in_field3 => <-.DVorthoslice#2.out;
			 out_obj;
			 obj;
		  };
		  ilink channel_1 => <-.Data_Reader.channel_1;
		  ilink channel_2 => <-.Data_Reader.channel_2;
		  ilink channel_3 => <-.Data_Reader.channel_3;
		  DVorthoslice DVorthoslice{
			 in => <-.channel_1;
			 axis = 2;
			 plane => params.slice;
			 DVorthoslice_unif {
				out {
				   nnode_data = 1;
				};
			 };
		  };
		  DVorthoslice DVorthoslice#1{
			 in => <-.channel_2;
			 axis = 2;
			 plane => params.slice;
			 DVorthoslice_unif {
				out {
				   nnode_data = 1;
				};
			 };
		  };
		  DVorthoslice DVorthoslice#2{
			 in => <-.channel_3;
			 axis = 2;
			 plane => params.slice;
			 DVorthoslice_unif {
				out {
				   nnode_data = 1;
				};
			 };
		  };
		  DVswitch DVswitch{
			 in => {
				<-.DVorthoslice.out,<-.DVorthoslice#1.out,<-.DVorthoslice#2.out};
			 index => params.selected_channel;
			 out<NEportLevels={1,3}>;
		  };
		  GMOD.instancer instancer{
			 Value => <-._3_channels_valids;
			 Group => <-.combine_rgb;
		  };
		  int+OPort Channel_3_valid => is_valid(params.filename_3);
		  GDM.DataObject DataObject#3{
			 in => <-.combine_rgb.out_fld;
			 Obj {
				visible<NEportLevels={2,0}> => <-.visible;
			 };
			 ilink visible => <-._3_channels_valids;
		  };
		  int+OPort Channel_1_valid => is_valid(params.filename_1);
		  int+OPort Channel_2_valid => is_valid(params.filename_2);
		  int+OPort _3_channels_valids => ((.Channel_1_valid && .Channel_2_valid) && .Channel_3_valid);
	   };
	   macro Surface_Plot<NEportLevels={0,1},instanced=0> {
		  UIshell Surface_Plot{
			 x = 730;
			 y = 234;
			 width = 683;
			 height = 559;
		  };
		  UIpanel Control_Panel{
			 parent => <-.Surface_Plot;
			 y = 0;
			 width = 150;
			 height => parent.clientHeight;
		  };
		  UIrenderView UIrenderView{
			 parent => <-.Surface_Plot;
			 y = 0;
			 x => <-.Control_Panel.width;
			 width => (parent.clientWidth - .x);
			 height => parent.clientHeight;
		  };
		  GDM.Mscene3D Mscene3D{
			 Top {
				child_objs => {
				   <-.<-.DataObject.obj,<-.<-.Axis.DataObject.obj};
				Xform {
				   mat = {
					  0.00529875,-0.0047599,0.00485716,0.,
					  0.00679971,0.00381285,-0.00368155,0.,
					  -0.000115674,0.00609368,0.0060976,0.,
					  0.,0.,0.,1.
				   };
				   xlate = {-0.460696,1.2853,-2.12384};
				   dcenter = {-1.72162,-1.10186,1.79094};
				};
			 };
			 View {
				View {
				   handle => <-.<-.<-.UIrenderView.handle;
				};
			 };
			 Camera {
				Camera<NEportLevels={0,3}>;
			 };
		  };
		  DVsurf_plot DVsurf_plot{
			 in<NEportLevels={2,2}> => <-.reset_xform.out_fld;
			 component = 0;
			 scale = 0.5;
			 offset = 0.;
			 nspace = 2;
		  };
		  DataObject DataObject{
			 in => <-.DVsurf_plot.out;
		  };
		  DVdownsize DVdownsize{
			 in<NEportLevels={3,0}> => <-.<-.Image_Viewer.DVswitch.out;
			 factor = {4.,4.};
			 DVdownsize_unif {
				out {
				   nnode_data = 1;
				};
			 };
		  };
		  UItwoPoint UItwoPoint{
			 view => <-.UIrenderView;
			 x<NEportLevels={0,2}>;
			 y<NEportLevels={0,2}>;
			 time<NEportLevels={0,2}>;
			 state<NEportLevels={0,2}>;
		  };
		  GDtrack_edit GDtrack_edit{
			 camera => <-.Mscene3D.Camera.Camera;
			 obj => <-.Mscene3D.View.View.picked_obj;
			 event => <-.UItwoPoint.state;
			 x => <-.UItwoPoint.x;
			 y => <-.UItwoPoint.y;
			 view => <-.Mscene3D.View.View;
			 time<NEportLevels={2,0}> => <-.UItwoPoint.time;
		  };
		  MODS.reset_xform reset_xform{
			 in_field => <-.DVdownsize.out;
		  };
		  XP_COMM_PROJ.Confocal.ConfocalMacs.Axis Axis{
			 Data => <-.DVsurf_plot.out;
		  };
	   };
	   macro Volume_Render<NEportLevels={0,1},instanced=0> {
		  UIshell Vortex_Rendering {
			 x = 333;
			 y = 347;
			 width = 745;
			 height = 581;
			 showStatusBar = 0;
			 visible<NEportLevels={3,0}> => <-.<-.User_Interface.Iconbar.toggle#1.output;
		  };
		  UIrenderView UIrenderView{
			 parent => <-.Vortex_Rendering;
			 y = 0;
			 x => <-.Control_Panel.width;
			 width => (parent.clientWidth - .x);
			 height => parent.clientHeight;
		  };
		  GDM.Mscene3D Mscene3D{
			 Top {
				child_objs => {
				   <-.<-.DataObject.obj,<-.<-.DataObject#1.obj,<-.<-.DataObject#2.obj,
				   <-.<-.Axis.DataObject.obj};
				Xform {
				   mat = {
					  -0.00676982,0.00796155,0.00616734,0.,
					  0.0100474,0.00483107,0.00479226,0.,
					  0.000688794,0.00777983,-0.00928729,0.,
					  0.,0.,0.,1.
				   };
				   xlate = {-0.904821,-1.80665,-1.15202};
				   dcenter = {0.808618,1.77301,1.19911};
				   ocenter = {116.5,146.,-4.};
				};
			 };
			 View {
				View{
				   handle => <-.<-.<-.UIrenderView.handle;
				   renderer => "Software";

	/*** IC June 26, 2000 */

				   blend_mode = "Saturate";

				};
			 };
			 Camera {
				Camera<NEportLevels={0,3}>;
			 };
		  };
		  macro DataObject<locked=0> {
			 group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255> {
				GDxform_templ &xform;
				method render;
			 } => <-.input;
			 Node_Data &texture_in<NEportLevels={2,1}>;
			 Node_Data &texture_col_in<NEportLevels={2,1}>;
			 imlink child_objs;
			 DefaultMinMax MinMax<export_all=1> {
				input => <-.in;
			 };
			 DefaultProps Props<export_all=1> {
				fat_ray<NEportLevels={3,0}> => <-.<-.fat_ray;
			 };
			 DefaultModes Modes<export_all=1> {
				mode = {0,0,0,3,0};
			 };
			 DefaultTexture Texture<export_all=1> {
				data => <-.texture_in;
				colormap => <-.texture_col_in;
			 };
			 DefaultPickInfo PickInfo<export_all=1>;
			 AltObject AltObject<export_all=1> {
				alt_in => <-.in;
			 };
			 DefaultObject Obj<NEportLevels={0,1},export_all=2> {
				input => <-.in;
				dmap => <-.VolumeRender;
				xform => in.xform;
				props => <-.Props;
				modes => <-.Modes;
				texture => <-.Texture;
				objects => <-.child_objs;
				altobj => <-.AltObject.obj;
				name => name_of(<-.<-);
				pick_info => <-.PickInfo;
				use_altobj<NEportLevels={3,0}> => <-.<-.Alternate_Object.set;
			 };
			 olink obj => .Obj;
			 VolumeRender VolumeRender{
				dataMin => <-.MinMax.min_value;
				dataMax => <-.MinMax.max_value;
				ratio;
				min_array={0,0,0};
				DataRange = {
				   {
					  UIMaxValue=><-.threshold,,,,,,,,,,,,,,,,,,,,,,,
				   },
				   };
				DatamapValue = {
				   {
					  v4=0.,v3=0.,v2=0.,,
				   },
				   {
					  v4=0.,v3=0.,v2=0.,
				   },
				   {
					  v4=0.,v3=0.,v2=0.5,
				   },
				   {
					  v4=0.,v3=0.,v2=1.,
				   }};
				currentColorModel = 1;
				float threshold<NEportLevels={3,1}> => <-.<-.Red_Threshold.value;
			 };
		  };
		  UItwoPoint UItwoPoint {
			 view => <-.UIrenderView;
			 x<NEportLevels={0,2}>;
			 y<NEportLevels={0,2}>;
			 time<NEportLevels={0,2}>;
			 state<NEportLevels={0,2}>;
		  };
		  GDtrack_edit GDtrack_edit {
			 camera => <-.Mscene3D.Camera.Camera;
			 obj => <-.Mscene3D.View.View.picked_obj;
			 event => <-.UItwoPoint.state;
			 x => <-.UItwoPoint.x;
			 y => <-.UItwoPoint.y;
			 view => <-.Mscene3D.View.View;
			 time<NEportLevels={2,0}> => <-.UItwoPoint.time;
			 mode<NEportLevels={2,0}> => <-.Edit_Options.Edit_Mode.selectedItem;
		  };
		  ilink input => <-.Data_Reader.channel_1;
		  macro DataObject#1<locked=0> {
			 group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255> {
				GDxform_templ &xform;
				method render;
			 } => <-.input#1;
			 Node_Data &texture_in<NEportLevels={2,1}>;
			 Node_Data &texture_col_in<NEportLevels={2,1}>;
			 imlink child_objs;
			 DefaultMinMax MinMax<export_all=1> {
				input => <-.in;
			 };
			 DefaultProps Props<export_all=1> {
				fat_ray<NEportLevels={3,0}> => <-.<-.fat_ray;
			 };
			 DefaultModes Modes<export_all=1> {
				mode = {0,0,0,3,0};
			 };
			 DefaultTexture Texture<export_all=1> {
				data => <-.texture_in;
				colormap => <-.texture_col_in;
			 };
			 DefaultPickInfo PickInfo<export_all=1>;
			 AltObject AltObject<export_all=1> {
				alt_in => <-.in;
			 };
			 DefaultObject+OPort Obj<export_all=2> {
				input => <-.in;
				dmap => <-.VolumeRender;
				xform => in.xform;
				props => <-.Props;
				modes => <-.Modes;
				texture => <-.Texture;
				objects => <-.child_objs;
				altobj => <-.AltObject.obj;
				name => name_of(<-.<-);
				pick_info => <-.PickInfo;
				use_altobj<NEportLevels={3,0}> => <-.<-.Alternate_Object.set;
			 };
			 olink obj => .Obj;
			 VolumeRender VolumeRender{
				dataMin => <-.MinMax.min_value;
				dataMax => <-.MinMax.max_value;
				min_array={0,0,0};
				ratio;
				DataRange = {
				   {
					  UIMaxValue=><-.threshold,,,,,,,,,,,,,,,,,,,,,,,
				   },
				   };
				DatamapValue = {
				   {
					  v4=0.,v3=0.,v2=0.,,
				   },
				   {
					  v4=0.,v3=0.,,
				   },
				   {
					  v4=0.,v3=0.5,,
				   },
				   {
					  v4=0.,,,
				   }};
				currentColorModel = 1;
				float threshold<NEportLevels={3,1}> => <-.<-.Green_Threshold.value;
			 };
		  };
		  ilink input#1 => <-.Data_Reader.channel_2;
		  macro DataObject#2<locked=0> {
			 group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255> {
				GDxform_templ &xform;
				method render;
			 } => <-.input#2;
			 Node_Data &texture_in<NEportLevels={2,1}>;
			 Node_Data &texture_col_in<NEportLevels={2,1}>;
			 imlink child_objs;
			 DefaultMinMax MinMax<export_all=1> {
				input => <-.in;
			 };
			 DefaultProps Props<export_all=1> {
				fat_ray<NEportLevels={3,0}> => <-.<-.fat_ray;
			 };
			 DefaultModes Modes<export_all=1> {
				mode = {0,0,0,3,0};
			 };
			 DefaultTexture Texture<export_all=1> {
				data => <-.texture_in;
				colormap => <-.texture_col_in;
			 };
			 DefaultPickInfo PickInfo<export_all=1>;
			 AltObject AltObject<export_all=1> {
				alt_in => <-.in;
			 };
			 DefaultObject+OPort Obj<export_all=2> {
				input => <-.in;
				dmap => <-.VolumeRender;
				xform => in.xform;
				props => <-.Props;
				modes => <-.Modes;
				texture => <-.Texture;
				objects => <-.child_objs;
				altobj => <-.AltObject.obj;
				name => name_of(<-.<-);
				pick_info => <-.PickInfo;
				use_altobj<NEportLevels={3,0}> => <-.<-.Alternate_Object.set;
			 };
			 olink obj => .Obj;
			 VolumeRender VolumeRender {
				dataMin => <-.MinMax.min_value;
				dataMax => <-.MinMax.max_value;
				min_array={0,0,0};
				DataRange = {
				   {
					  UIMaxValue=><-.threshold,,,,,,,,,,,,,,,,,,,,,,,
				   },
				   };
				DatamapValue = {
				   {
					  v4=0.,v3=0.,v2=0.,,
				   },
				   {
					  v4=0.,v3=0.,,
				   },
				   {
					  v4=0.5,v3=0.,,
				   },
				   {
					  v3=0.,,
				   }};
				currentColorModel = 1;
				float threshold<NEportLevels={3,1}> => <-.<-.Blue_Threshold.value;
			 };
		  };
		  ilink input#2 => <-.Data_Reader.channel_3;
		  int+Port fat_ray => .Decimate_Ray.set;
		  UIframe Control_Panel {
			 parent => <-.Vortex_Rendering;
			 y = 0;
			 width = 250;
			 height => parent.clientHeight;
		  };
		  UItoggle Decimate_Ray {
			 parent => <-.UIframe;
			 y = 35;
			 x = 5;
			 width => (parent.clientWidth - 10);
			 set = 0;
		  };
		  UIslider Red_Threshold {
			 parent => <-.UIframe;
			 value = 30.1;
			 y = 60;
			 x = 5;
			 width => (parent.clientWidth - 10);
			 min = 0.;
			 max+nres => input.node_data[0].max;
			 decimalPoints = 1;
		  };
		  UIslider Green_Threshold {
			 parent => <-.UIframe;
			 value = 96.3;
			 y = 120;
			 x = 5;
			 width => (parent.clientWidth - 10);
			 min = 0.;
			 max+nres => input#1.node_data[0].max;
			 decimalPoints = 1;
		  };
		  UIslider Blue_Threshold {
			 parent => <-.UIframe;
			 value = 32.49;
			 y = 180;
			 x = 5;
			 width => (parent.clientWidth - 10);
			 min = 0.;
			 max+nres => input#2.node_data[0].max;
		  };
		  UIframe UIframe {
			 parent => <-.Control_Panel;
			 y = 100;
			 width => parent.clientWidth;
			 height = 275;
		  };
		  UIframe UIframe#3 {
			 parent => <-.Control_Panel;
			 y = 510;
			 width => parent.clientWidth;
			 height = 60;
		  };
		  UIpanel UIpanel {
			 parent => <-.UIframe#3;
			 y = 25;
			 x = 5;
			 width => (parent.clientWidth - 10);
			 height => ((parent.clientHeight - .y) - 2);
			 color {
				backgroundColor = "red";
			 };
		  };
		  UIlabel Percent_Complete {
			 parent => <-.UIframe#3;
			 y = 0;
			 x = 5;
			 width => (parent.clientWidth - 10);
			 alignment = "left";
		  };
		  UIpanel UIpanel#1 {
			 parent => <-.UIpanel;
			 y = 0;
			 width => ((<-.<-.<-.<-.Scheduler.percent_done * parent.clientWidth) / 100);
			 height => parent.clientHeight;
			 color {
				backgroundColor = "green";
			 };
		  };
		  macro Edit_Options {
			 UIoption Rotate {
				set = 1;
			 };
			 UIoption Scale;
			 UIoption Translate;
			 UIoption Z_Translate;
			 UIoptionMenu Edit_Mode {
				parent<NEportLevels={3,0}> => <-.<-.UIframe#1;
				cmdList => {<-.Rotate,<-.Scale,
				   <-.Translate,<-.Z_Translate};
				selectedItem<NEportLevels={0,3}> = 0;
				x = 10;
				y = 55;
			 };
		  };
		  UIframe UIframe#1 {
			 parent => <-.Control_Panel;
			 y = 0;
			 width => parent.clientWidth;
		  };
		  UIlabel Display_Options {
			 parent => <-.UIframe#1;
			 y = 0;
			 width => parent.clientWidth;
		  };
		  UItoggle Alternate_Object {
			 parent => <-.UIframe#1;
			 set = 1;
			 y = 25;
			 x = 5;
			 width => (parent.clientWidth - 10);
		  };
		  UIlabel Rendering_Options {
			 parent => <-.UIframe;
			 y = 0;
			 width => parent.clientWidth;
		  };
		  XP_COMM_PROJ.Confocal.ConfocalMacs.Axis Axis {
			 Data => <-.input;
		  };
	   };
	   macro User_Interface {
		  UIapp Confocal_Microscopy_Demo {
			 menu => <-.Menubar;
			 x = 0;
			 y = 0;
			 width = 835;
			 height = 881;
			 iconName = "Confocal Demo";
		  };
		  UIcmdList Menubar {
			 cmdList => {<-.File_Menu.File};
		  };
		  macro File_Menu {
			 UIcmdList+OPort2 File {
				cmdList => {
				   <-.Load_Channel_1,<-.Load_Channel_2,<-.Load_Channel_3,
				   <-.UImenuSeparator,<-.Unset_Channel,<-.UImenuSeparator#1,<-.Exit
				};
			 };
			 UIcmd Load_Channel_1 {
				message = "Load data into channel 1";
			 };
			 UIcmd Load_Channel_2 {
				message = "Load data into channel 2";
			 };
			 UIcmd Load_Channel_3 {
				message = "Load data into channel 3";
			 };
			 UIcmd Exit {
				message = "Exit Application";
			 };
			 UIfileDialog Select_File_for_Channel_1 {
				visible => <-.Load_Channel_1.do;
				parent => <-.parent;
				searchPattern = "*.ncdf";
				x = 268;
				y = 212;
				width = 300;
				height = 390;
				cancel = 1;
				ok = 1;
				filename = "./xp_comm_proj/confocal/test.ncdf";
				dirMaskCache = "./xp_comm_proj/confocal/*.ncdf";
			 };
			 UIfileDialog Select_File_for_Channel_2 {
				visible => <-.Load_Channel_2.do;
				parent => <-.parent;
				searchPattern = "*.ncdf";
				x = 268;
				y = 212;
				width = 300;
				height = 390;
				cancel = 1;
				ok = 1;
				filename = "./xp_comm_proj/confocal/test.ncdf";
				dirMaskCache = "./xp_comm_proj/confocal/*.ncdf";
			 };
			 UIfileDialog Select_File_for_Channel_3 {
				visible => <-.Load_Channel_3.do;
				parent => <-.parent;
				searchPattern = "*.ncdf";
				x = 268;
				y = 212;
				width = 300;
				height = 390;
				cancel = 1;
				ok = 1;
				filename = "./xp_comm_proj/confocal/test.ncdf";
				dirMaskCache = "./xp_comm_proj/confocal/*.ncdf";
			 };
			 ilink parent => <-.Confocal_Microscopy_Demo;
			 UIquestionDialog Confirmation_Dialog {
				visible => <-.Exit.do;
				message = "Are you sure you want to exit this demo?";
				isModal = 1;
				ok<NEportLevels={1,2}>;
				okLabelString = "Yes";
				cancelLabelString = "No";
			 };
			 UImenuSeparator UImenuSeparator;
			 UImenuSeparator UImenuSeparator#1;
			 UIcmdList Unset_Channel {
				cmdList => {<-._1,<-._2,<-._3};
				label = "Unset Channel";
			 };
			 UIcmd _1;
			 UIcmd _2;
			 UIcmd _3;
			 GMOD.parse_v parse_v_1 {
				v_commands = "filename=;";
				trigger => <-._1.do;
				relative => <-.Select_File_for_Channel_1;
				on_inst=0;
			 };
			 GMOD.parse_v parse_v_2 {
				v_commands = "filename=;";
				trigger => <-._2.do;
				relative => <-.Select_File_for_Channel_2;
				on_inst=0;
			 };
			 GMOD.parse_v parse_v_3 {
				v_commands = "filename=;";
				trigger => <-._3.do;
				relative => <-.Select_File_for_Channel_3;
				on_inst=0;
			 };
			 GMOD.exit_process exit_process {
				do_exit => <-.Confirmation_Dialog.ok;
				status = 0;
			 };
		  };
		  group+OPort2 Params {
			 string+IPort2 filename_1 => <-.File_Menu.Select_File_for_Channel_1.filename;
			 string+IPort2 filename_2 => <-.File_Menu.Select_File_for_Channel_2.filename;
			 string+IPort2 filename_3 => <-.File_Menu.Select_File_for_Channel_3.filename;
			 int+IPort2 slice => <-.Control_Panel.slice;
			 int+IPort2 selected_channel => <-.Control_Panel.select_channel.selected;
			 int+IPort2 oe_visible => <-.Control_Panel.oe_visible;
		  };
		  macro Control_Panel {
			 UIpanel Control {
				parent<NEportLevels={3,0}> => <-.<-.Confocal_Microscopy_Demo;
				y = 40;
				width = 150;
				height => (parent.clientHeight - .y);
			 };
			 UIbutton Channel_1 {
				parent => <-.Control;
				height => .width;
				width => (parent.clientWidth / 2);
				y = 0;
				x => (parent.clientWidth / 2);
				message = "Enable/Disable Channel 1";
				active => is_valid(<-.<-.Params.filename_1);
			 };
			 UIbutton Channel_2 {
				parent => <-.Control;
				height => .width;
				width => (parent.clientWidth / 2);
				y => (parent.clientWidth / 2);
				x => .width;
				message = "Enable/Disable Channel 2";
				active => is_valid(<-.<-.Params.filename_2);
			 };
			 UIbutton Channel_3 {
				parent => <-.Control;
				height => .width;
				width => (parent.clientWidth / 2);
				y => parent.clientWidth;
				x => (parent.clientWidth / 2);
				message = "Enable/Disable Channel 3";
				active => is_valid(<-.<-.Params.filename_3);
			 };
			 UIbutton E_Vertex {
				parent => <-.Control;
				height => .width;
				width => (parent.clientWidth / 2);
				y = 75;
				message = "Edit the selected channel Vertex object (Vertex viewer must be on)";
				active = 1;
			 };
			 UIbutton Crop {
				parent => <-.Control;
				height => .width;
				width => (parent.clientWidth / 2);
				y = 0;
				active = 1;
				message = "Set i,j,k limits";
			 };
			 UIbutton Volume {
				parent => <-.Control;
				height => .width;
				width => (parent.clientWidth / 2);
				y => parent.clientWidth;
				active = 1;
				message = "Volume calculation";
			 };
			 UIdial Slice {
				parent => <-.Control;
				value<NEportLevels={2,2}> = 7.;
				x = 8;
				y = 246;
				width = 127;
				height = 119;
				min<NEportLevels={2,0}> => <-.CropParam.min[2];
				max<NEportLevels={2,0}> => <-.CropParam.max[2];
				mode = "integer";
				decimalPoints = 0;
				immediateMode = 1;
				showValue = 0;
			 };
			 group select_channel {
				int one<NEportLevels={2,1}> => <-.Channel_1.do;
				int two<NEportLevels={2,1}> => <-.Channel_2.do;
				int three<NEportLevels={2,1}> => <-.Channel_3.do;
				int selected<NEportLevels={1,3}> = 0;
				GMOD.parse_v parse_v {
				   v_commands = "selected = 0;";
				   trigger => <-.one;
				   on_inst = 0;
				   relative => <-.<-.select_channel;
				};
				GMOD.parse_v parse_v#1 {
				   v_commands = "selected = 1;";
				   trigger => <-.two;
				   on_inst = 0;
				   relative => <-.<-.select_channel;
				};
				GMOD.parse_v parse_v#2 {
				   v_commands = "selected = 2;";
				   trigger => <-.three;
				   on_inst = 0;
				   relative => <-.<-.select_channel;
				};
			 };
			 UIframe UIframe {
				parent => <-.Control;
				y = 375;
				x => <-.Slice.x;
				width => <-.Slice.width;
				height = 400;
				active = 1;
				message = "Crop values";
				visible => <-.toggle.output;
			 };
			 macro CropUI<instanced=1> {
				DV_Param_crop+IPort2 &param => <-.CropParam;
				ilink panel => <-.UIframe;
				UIslider min0_slider {
				   parent => <-.panel;
				   title => "I min";
				   max+nres => (<-.param.dims[0] - 1);
				   value+nres<NEportLevels={2,0}> => param.min[0];
				   mode = "integer";
				   y = 0;
				   width => (<-.panel.width - 20);
				   x = 10;
				};
				UIslider max0_slider {
				   parent => <-.panel;
				   title => "I max";
				   max+nres => (<-.param.dims[0] - 1);
				   value+nres<NEportLevels={2,0}> => param.max[0];
				   mode = "integer";
				   width => (<-.panel.width - 20);
				   x = 10;
				   y = 60;
				};
				UIslider min1_slider {
				   parent => <-.panel;
				   title => "J min";
				   max+nres => (<-.param.dims[1] - 1);
				   value+nres<NEportLevels={2,0}> => param.min[1];
				   mode = "integer";
				   width => (<-.panel.width - 20);
				   visible+nres => panel.visible;
				   x = 10;
				   y = 120;
				};
				UIslider max1_slider {
				   parent => <-.panel;
				   title => "J max";
				   max+nres => (<-.param.dims[1] - 1);
				   value+nres<NEportLevels={2,0}> => param.max[1];
				   mode = "integer";
				   width => (<-.panel.width - 20);
				   visible+nres => panel.visible;
				   x = 10;
				   y = 180;
				};
				UIslider min2_slider {
				   parent => <-.panel;
				   title => "K min";
				   max+nres => (<-.param.dims[2] - 1);
				   value+nres<NEportLevels={2,0}> => param.min[2];
				   mode = "integer";
				   width => (<-.panel.width - 20);
				   visible+nres => panel.visible;
				   x = 10;
				   min => <-.param.min[2];
				   y = 240;
				};
				UIslider max2_slider {
				   parent => <-.panel;
				   title => "K max";
				   max+nres => (<-.param.dims[2] - 1);
				   value+nres<NEportLevels={2,0}> => param.max[2];
				   mode = "integer";
				   y => ((<-.min2_slider.y + <-.min2_slider.height) + 4);
				   width => (<-.panel.width - 20);
				   visible+nres => panel.visible;
				   x = 10;
				};
			 };
			 DV_Param_crop &CropParam<export_all=2,NEportLevels={2,1}> {
				min+nres<NEportLevels={2,2}>;
				max+nres<NEportLevels={2,2}>;
			 } => <-.CropParam;
			 int slice<NEportLevels={1,2}> => (.Slice.value - .Slice.min);
			 group toggle {
				int trigger<NEportLevels={2,0}> => <-.Crop.do;
				int output<NEportLevels={0,2}> = 0;
				GMOD.parse_v parse_v {
				   v_commands = "output = !output;";
				   trigger => <-.trigger;
				   on_inst = 0;
				   relative => <-.<-.toggle;
				};
			 };
			 group toggle#1 {
				int trigger<NEportLevels={2,0}> => <-.E_Vertex.do;
				int output<NEportLevels={0,2}> = 0;
				GMOD.parse_v parse_v {
				   v_commands = "output = !output;";
				   trigger => <-.trigger;
				   on_inst = 0;
				   relative => <-.<-.toggle#1;
				};
			 };
			 olink oe_visible => .toggle#1.output;
			 UIframe UIframe#1 {
				parent => <-.Control;
				y = 375;
				x => <-.Slice.x;
				width => <-.Slice.width;
				height = 200;
				active = 1;
				message = "Volume integral";
				visible => <-.toggle#2.output;
			 };
			 macro VolIntUI {
				UIlabel Volume_Claculation {
				   parent => <-.parent;
				   y = 0;
				   width => parent.clientWidth;
				   alignment = "center";
				};
				UIlabel Volume {
				   parent => <-.parent;
				   y = 50;
				   width => parent.clientWidth;
				   alignment = "center";
				};
				UIfield UIfield {
				   parent => <-.parent;
				   value<NEportLevels={2,2}> => <-.VolIntrParam.volume;
				   y = 74;
				   x = 10;
				   width => (parent.clientWidth - 20);
				};
				UIlabel Volume_Integral {
				   parent => <-.parent;
				   y = 125;
				   width => parent.clientWidth;
				   alignment = "center";
				   active = 0;
				};
				UIfield UIfield#1 {
				   parent => <-.parent;
				   y = 149;
				   x = 10;
				   width => (parent.clientWidth - 20);
				   value<NEportLevels={2,2}> => <-.VolIntrParam.volume_intr;
				   active = 0;
				};
				group+IPort &VolIntrParam {
				   float volume<NEportLevels={2,2}>;
				   float volume_intr<NEportLevels={2,2}>;
				} => <-.VolIntrParam;
				ilink parent => <-.UIframe#1;
			 };
			 group toggle#2 {
				int trigger<NEportLevels={2,0}> => <-.Volume.do;
				int output<NEportLevels={0,2}> = 0;
				GMOD.parse_v parse_v {
				   v_commands = "output = !output;";
				   trigger => <-.trigger;
				   on_inst = 0;
				   relative => <-.<-.toggle#2;
				};
			 };
			 group &VolIntrParam<NEportLevels={2,1}> {
				float volume<NEportLevels={2,2}>;
				float volume_intr<NEportLevels={2,2}>;
			 } => <-.VolIntrParam;
		  };
		  UIpanel+OPort2 Viewer_Panel {
			 parent => <-.Confocal_Microscopy_Demo;
			 y = 40;
			 x = 150;
			 width => (parent.clientWidth - .x);
			 height => (parent.clientHeight - .y);
		  };
		  macro Iconbar {
			 UIframe Iconbar {
				parent<NEportLevels={3,0}> => <-.<-.Confocal_Microscopy_Demo;
				y = 0;
				width => parent.clientWidth;
				height = 40;
				shadowType = "shadow_out";
				shadowThickness = 2;
			 };
			 UIbutton Surface {
				parent => <-.Iconbar;
				height => parent.clientHeight;
				label => "SU";
				y = 0;
				width => .height;
				message = "Display Surface Plot of selected channel";
			 };
			 UIbutton three_d {
				parent => <-.Iconbar;
				height => parent.clientHeight;
				label => "VO";
				y = 0;
				width => .height;
				message = "Display Vortex Rendering";
				x => .width;
			 };
			 group toggle {
				int trigger<NEportLevels={2,0}> => <-.Surface.do;
				int output<NEportLevels={0,4}> = 0;
				GMOD.parse_v parse_v {
				   v_commands = "output = !output;";
				   trigger => <-.trigger;
				   on_inst = 0;
				   relative => <-.<-.toggle;
				};
			 };
			 group toggle#1 {
				int trigger<NEportLevels={2,0}> => <-.three_d.do;
				int output<NEportLevels={0,4}> = 0;
				GMOD.parse_v parse_v {
				   v_commands = "output = !output;";
				   trigger => <-.trigger;
				   on_inst = 0;
				   relative => <-.<-.toggle#1;
				};
			 };
			 UIbutton iso {
				parent => <-.Iconbar;
				height => parent.clientHeight;
				label => "VE";
				y = 0;
				width => .height;
				message = "Display Vertex Rendering";
				x => (.width * 2);
			 };
			 group toggle#2 {
				int trigger<NEportLevels={2,0}> => <-.iso.do;
				int output<NEportLevels={0,4}> = 0;
				GMOD.parse_v parse_v {
				   v_commands = "output = !output;";
				   trigger => <-.trigger;
				   on_inst = 0;
				   relative => <-.<-.toggle#2;
				};
			 };
		  };
		  DV_Param_crop &CropParam<export_all=2,NEportLevels={2,1}> {
			 min+nres<NEportLevels={2,2}>;
			 max+nres<NEportLevels={2,2}>;
		  } => <-.Data_Reader.CropParam;
		  group &VolIntrParam<NEportLevels={2,1}> {
			 float volume<NEportLevels={2,2}>;
			 float volume_intr<NEportLevels={2,2}>;
		  } => <-.Isosurfaces.VolIntrParam;
	   };
	   macro Data_Reader {
		  group+IPort2 &params => <-.User_Interface.Params;
		  olink channel_1 => .my_crop.out_fld;
		  olink channel_2 => .my_crop#1.out_fld;
		  olink channel_3 => .my_crop#2.out_fld;
		  DVread_net_cdf DVread_net_cdf {
			 filename => params.filename_1;
			 Mesh_Unif+Node_Data+write net_cdf_out {
				&node_data+nres[];
				Data_Array node_data_ref_0 {
				   byte values[nvals][veclen];
				};
			 };
		  };
		  DVread_net_cdf DVread_net_cdf#1 {
			 filename => params.filename_2;
			 Mesh_Unif+Node_Data+write net_cdf_out {
				&node_data+nres[];
				Data_Array node_data_ref_0 {
				   byte values[nvals][veclen];
				};
			 };
		  };
		  DVread_net_cdf DVread_net_cdf#2 {
			 filename => params.filename_3;
			 Mesh_Unif+Node_Data+write net_cdf_out {
				&node_data+nres[];
				Data_Array node_data_ref_0 {
				   byte values[nvals][veclen];
				};
			 };
		  };
		  DV_Param_crop &CropParam<export_all=2,NEportLevels={1,2}> {
			 min+nres<NEportLevels={2,2}>;
			 max+nres<NEportLevels={2,2}>;
		  } => .my_crop.CropParam;
		  macro my_crop#1 {
			 ilink in_field => <-.DVread_net_cdf#1.out;
			 DV_Param_crop &CropParam<export_all=2,NEportLevels={2,1}> {
				min+nres<NEportLevels={2,2}> => init_array(in_field.ndim,0,0);
				max+nres<NEportLevels={2,2}> => (in_field.dims - 1);
			 } => <-.CropParam;
			 DVcrop DVcrop {
				in => <-.in_field;
				min => param.min;
				max => param.max;
				DV_Param_crop &param<NEportLevels={2,0}> => <-.CropParam;
			 };
			 olink out_fld => .DVcrop.out;
		  };
		  macro my_crop#2 {
			 ilink in_field => <-.DVread_net_cdf#2.out;
			 DV_Param_crop &CropParam<export_all=2,NEportLevels={2,1}> {
				min+nres<NEportLevels={2,2}> => init_array(in_field.ndim,0,0);
				max+nres<NEportLevels={2,2}> => (in_field.dims - 1);
			 } => <-.CropParam;
			 DVcrop DVcrop {
				in => <-.in_field;
				min => param.min;
				max => param.max;
				DV_Param_crop &param<NEportLevels={2,0}> => <-.CropParam;
			 };
			 olink out_fld => .DVcrop.out;
		  };
		  XP_COMM_PROJ.Confocal.ConfocalMacs.my_crop my_crop {
			 in_field => <-.DVread_net_cdf.out;
			 CropParam {
				int ndim<NEportLevels=1> => <-.in_field.ndim;
				int dims<NEportLevels=1>[] => <-.in_field.dims;
			 };
		  };
	   };
	   GMOD.instancer instancer {
		  Value => <-.User_Interface.Iconbar.toggle.output;
		  Group => <-.Surface_Plot;
	   };
	   GMOD.instancer instancer#1 {
		  Value => <-.User_Interface.Iconbar.toggle#1.output;
		  Group => <-.Volume_Render;
	   };
	   GMOD.instancer instancer#2 {
		  Value => <-.User_Interface.Iconbar.toggle#2.output;
		  Group => <-.Isosurfaces;
	   };
	   macro+OPort Isosurfaces {
		  UIshell Vertex_Rendering {
			 x = 348;
			 y = 379;
			 width = 745;
			 height = 581;
			 showStatusBar = 0;
			 visible<NEportLevels={3,0}> => <-.<-.User_Interface.Iconbar.toggle#2.output;
		  };
		  UIrenderView UIrenderView {
			 parent => <-.Vertex_Rendering;
			 y = 0;
			 x => <-.Control_Panel.width;
			 width => (parent.clientWidth - .x);
			 height => parent.clientHeight;
		  };
		  GDM.Mscene3D Mscene3D {
			 Top {
				child_objs => {
				   <-.<-.DataObject.obj,<-.<-.DataObject#1.obj,<-.<-.DataObject#2.obj,
				   <-.<-.Axis.DataObject.obj};
				Xform {
				   mat = {
					  -0.00503583,0.00667651,0.00638175,0.,0.00922442,0.00327147,0.00385633,
	0.,0.000462796,0.00744196,-0.00742067,0.,0.,0.,0.,1.
				   };
				   xlate = {-0.880187,-2.27087,-0.937586};
				   dcenter = {-0.0852832,0.312792,-0.47801};
				};
				Datamap {
				   currentColorModel = 1;
				   DatamapValue = {
					  {
						 v4=0.,v3=0.,v2=0.,
					  },
					  {
						 v4=0.,v3=0.,v2=1.,
					  }};
				};
			 };
			 View {
				View {
				   handle => <-.<-.<-.UIrenderView.handle;
				   renderer => "OpenGL";
				   trigger = 2;
				};
			 };
			 Camera {
				Camera<NEportLevels={0,3}>;
			 };
			 Lights {
				LightInfo;
				Lights = {
				   {
					  type="BiDirectional"
				   },,,};
			 };
		  };
		  UItwoPoint UItwoPoint {
			 view => <-.UIrenderView;
			 x<NEportLevels={0,2}>;
			 y<NEportLevels={0,2}>;
			 time<NEportLevels={0,2}>;
			 state<NEportLevels={0,2}>;
		  };
		  GDtrack_edit GDtrack_edit {
			 camera => <-.Mscene3D.Camera.Camera;
			 obj => <-.Mscene3D.View.View.picked_obj;
			 event => <-.UItwoPoint.state;
			 x => <-.UItwoPoint.x;
			 y => <-.UItwoPoint.y;
			 view => <-.Mscene3D.View.View;
			 time<NEportLevels={2,0}> => <-.UItwoPoint.time;
			 mode<NEportLevels={2,0}> => <-.Edit_Options.Edit_Mode.selectedItem;
		  };
		  ilink input => <-.Data_Reader.channel_1;
		  ilink input#1 => <-.Data_Reader.channel_2;
		  ilink input#2 => <-.Data_Reader.channel_3;
		  UIframe Control_Panel {
			 parent => <-.Vertex_Rendering;
			 y = 0;
			 width = 250;
			 height => parent.clientHeight;
		  };
		  UIslider Red_Threshold {
			 parent => <-.UIframe;
			 value<NEportLevels={2,2}> = 30.1;
			 y = 60;
			 x = 5;
			 width => (parent.clientWidth - 10);
			 min = 0.;
			 max+nres => input.node_data[0].max;
			 decimalPoints = 1;
		  };
		  UIslider Green_Threshold {
			 parent => <-.UIframe;
			 value<NEportLevels={2,2}> = 96.3;
			 y = 120;
			 x = 5;
			 width => (parent.clientWidth - 10);
			 min = 0.;
			 max+nres => input#1.node_data[0].max;
			 decimalPoints = 1;
		  };
		  UIslider Blue_Threshold {
			 parent => <-.UIframe;
			 value = 32.49;
			 y = 180;
			 x = 5;
			 width => (parent.clientWidth - 10);
			 min = 0.;
			 max+nres => input#2.node_data[0].max;
		  };
		  UIframe UIframe {
			 parent => <-.Control_Panel;
			 y = 100;
			 width => parent.clientWidth;
			 height = 275;
		  };
		  UIframe UIframe#3 {
			 parent => <-.Control_Panel;
			 y = 510;
			 width => parent.clientWidth;
			 height = 60;
		  };
		  UIpanel UIpanel {
			 parent => <-.UIframe#3;
			 y = 25;
			 x = 5;
			 width => (parent.clientWidth - 10);
			 height => ((parent.clientHeight - .y) - 2);
			 color {
				backgroundColor = "red";
			 };
		  };
		  UIlabel Percent_Complete {
			 parent => <-.UIframe#3;
			 y = 0;
			 x = 5;
			 width => (parent.clientWidth - 10);
			 alignment = "left";
		  };
		  UIpanel UIpanel#1 {
			 parent => <-.UIpanel;
			 y = 0;
			 width => ((<-.<-.<-.<-.Scheduler.percent_done * parent.clientWidth) / 100);
			 height => parent.clientHeight;
			 color {
				backgroundColor = "green";
			 };
		  };
		  macro Edit_Options {
			 UIoption Rotate {
				set = 1;
			 };
			 UIoption Scale;
			 UIoption Translate;
			 UIoption Z_Translate;
			 UIoptionMenu Edit_Mode {
				parent<NEportLevels={3,0}> => <-.<-.UIframe#1;
				cmdList => {<-.Rotate,<-.Scale,
				   <-.Translate,<-.Z_Translate};
				selectedItem<NEportLevels={0,3}> = 0;
				x = 10;
				y = 55;
			 };
		  };
		  UIframe UIframe#1 {
			 parent => <-.Control_Panel;
			 y = 0;
			 width => parent.clientWidth;
		  };
		  UIlabel Display_Options {
			 parent => <-.UIframe#1;
			 y = 0;
			 width => parent.clientWidth;
		  };
		  UItoggle Alternate_Object {
			 parent => <-.UIframe#1;
			 set = 1;
			 y = 25;
			 x = 5;
			 width => (parent.clientWidth - 10);
		  };
		  UIlabel Rendering_Options {
			 parent => <-.UIframe;
			 y = 0;
			 width => parent.clientWidth;
		  };
		  GDM.DataObject DataObject#1 {
			 in => <-.DViso#1.out;
			 Datamap {
				currentColorModel = 1;
				DatamapValue = {
				   {
					  v4=0.,v3=0.,v2=0.,
				   },
				   {
					  v4=0.,,,
				   }};
			 };
			 Props {
				col = {0.02,1.,0.};
				inherit = 0;
			 };
			 Obj {
				use_altobj<NEportLevels={2,0}> => <-.use_altobj;
				cache_size = 16;
			 };
			 ilink use_altobj => <-.Alternate_Object.set;
		  };
		  GDM.DataObject DataObject#2 {
			 in => <-.DViso#2.out;
			 Datamap {
				currentColorModel = 1;
				DatamapValue = {
				   {
					  v4=0.,v3=0.,v2=0.,
				   },
				   {
					  v3=0.,,
				   }};
			 };
			 Props {
				col = {0.,0.04,1.};
				inherit = 0;
			 };
			 Obj {
				use_altobj<NEportLevels={2,0}> => <-.use_altobj;
				cache_size = 16;
			 };
			 ilink use_altobj => <-.Alternate_Object.set;
		  };
		  GDM.DataObject DataObject {
			 in => <-.DViso.out;
			 Datamap {
				currentColorModel = 1;
				DatamapValue = {
				   {
					  v4=0.,v3=0.,v2=0.,
				   },
				   {
					  v4=0.,,,
				   }};
			 };
			 Props {
				col = {1.,0.,0.};
				inherit = 0;
			 };
			 Obj {
				use_altobj<NEportLevels={2,0}> => <-.use_altobj;
				cache_size = 16;
			 };
			 ilink use_altobj => <-.Alternate_Object.set;
		  };
		  DViso DViso {
			 in => <-.input;
			 level => <-.Red_Threshold.value;
		  };
		  DViso DViso#1 {
			 in => <-.input#1;
			 level => <-.Green_Threshold.value;
		  };
		  DViso DViso#2 {
			 in => <-.input#2;
			 level => <-.Blue_Threshold.value;
		  };
		  XP_COMM_PROJ.Confocal.ConfocalMacs.Axis Axis {
			 Data => <-.input;
		  };
		  GDM.Uobject_editor Uobject_editor {
			 cur_obj => <-.DVswitch.out;
			 ilink visible => <-.params.oe_visible;
			 IUI {
				panel {
				   visible<NEportLevels={3,0}> => <-.<-.visible;
				   defaultX = 312;
				   defaultY = 32;
				};
				optionList {
				   selectedItem = 0;
				};
				General {
				   IUI {
					  panel {
						 visible = 1;
					  };
					  AltSpace {
						 OPcmdList = {
							{
							   set=1,,,,,,,,,,,,,,,,,,,,
							},
							};
					  };
					  TransformMode {
						 OPcmdList = {
							{
							   set=1,,,,,,,,,,,,,,,,,,,,
							},
							,,};
					  };
				   };
				};
			 };
			 in_view => <-.Mscene3D.View.View;
			 GDobj_edit {
				xform_mode = "Normal";
			 };
		  };
		  DVM.DVswitch DVswitch {
			 in => {<-.DataObject.obj,
				<-.DataObject#1.obj,<-.DataObject#2.obj};
			 index => <-.params.selected_channel;
		  };
		  group+IPort2 &params => <-.User_Interface.Params;
		  DVintegr_vol DVintegr_vol<NEx=1155.,NEy=374.> {
			 dim = 2;
			 in => <-.DVswitch#1.out;
		  };
		  group+OPort2 VolIntrParam {
			 float+Port2 volume => <-.DVintegr_vol.volume;
			 float+Port2 volume_intr => <-.DVintegr_vol.mass;
		  };
		  DVswitch DVswitch#1 {
			 in => {<-.DViso.out,
				<-.DViso#1.out,<-.DViso#2.out};
			 index => <-.params.selected_channel;
		  };
	   };
	};
};
