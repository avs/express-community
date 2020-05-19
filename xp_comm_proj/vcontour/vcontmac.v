
flibrary VolumeToContourMacs{

  // VolumeToContourUI
  // User Macro 

  macro VolumeToContourUI{

    ilink in;
    XP_COMM_PROJ.VolumeToContour.VolumeToContourMods.VolumeToContourParams+IPort2 &Params;

    UI.UImod_panel UImod_panel{
	  title = "VolumeToContour";
    };
    UIslider Axis{
      parent => <-.UImod_panel;
	  value => <-.Params.axis;
	  width => parent.clientWidth;
	  mode = "integer";
	  min = 0;
	  max = 2;
      y = 10;
    };
    UIslider First_Slice{
      parent => <-.UImod_panel;
	  value => <-.Params.first_slice;
 	  width => parent.clientWidth;
      y => <-.Axis.y + <-.Axis.height + 10;
	  mode = "integer";
      min = 0;
      max => (<-.Params.nslices - 1);
    };

    UIslider Last_Slice{
      parent => <-.UImod_panel;
	  value => <-.Params.last_slice;
 	  width => parent.clientWidth;
      y => <-.First_Slice.y + <-.First_Slice.height + 10;
	  mode = "integer";
      min = 0;
      max => (<-.Params.nslices - 1);
    };

    UIslider Slice_Spacing{
      parent => <-.UImod_panel;
	  value => <-.Params.slice_spacing;
 	  width => parent.clientWidth;
      y => <-.Last_Slice.y + <-.Last_Slice.height + 10;
	  mode = "integer";
      min = 1;
      max => (<-.Params.nslices - 1);
    };

    UIslider NContours{
      parent => <-.UImod_panel;
	  value => <-.Params.ncontours;
 	  width => parent.clientWidth;
      y => <-.Slice_Spacing.y + <-.Slice_Spacing.height + 10;
	  mode = "integer";
      min = 1;
      max = 20;
    };

    UIslider Level_min{
      parent => <-.UImod_panel;
	  value => <-.Params.level_min;
 	  width => parent.clientWidth;
      y => <-.NContours.y + <-.NContours.height + 10;
      min => <-.Params.data_min;
      max => <-.Params.data_max;
    };

    UIslider Level_max{
      parent => <-.UImod_panel;
	  value => <-.Params.level_max;
  	  width => parent.clientWidth;
      y => <-.Level_min.y + <-.Level_min.height + 10;
      min => <-.Params.data_min;
      max => <-.Params.data_max;
    };

    UItoggle Color{
      parent => <-.UImod_panel;
  	  width => parent.clientWidth;
	  set => <-.Params.color;
      y => <-.Level_max.y + <-.Level_max.height + 10;
      x = 10;
    };
 
  };


 // VolumeToContourFunc
 // Funcitonal macro
 macro VolumeToContourFunc{
    ilink in;
    XP_COMM_PROJ.VolumeToContour.VolumeToContourMods.VolumeToContourParams+IPort2 &Params;
	XP_COMM_PROJ.VolumeToContour.VolumeToContourMods.VolumeToContour VolumeToContour{
	  Input_Field => <-.in;
      Params => <-.Params;
    };
	GDM.DataObject DataObject {
      in => <-.VolumeToContour.Contours;
	};	
	olink out => VolumeToContour.Contours;
	olink obj => DataObject.obj;
 };

 // Volume_To_Contour
 // User Macro 
 macro Volume_To_Contour{

   ilink in;
   XP_COMM_PROJ.VolumeToContour.VolumeToContourMods.VolumeToContourParams+IPort2 Params
   {
      axis=0;
	  color=1;
	  slice_spacing=2;
	  ncontours=5;
      first_slice=0;
	  nslices+nres => <-.in.dims[.axis];
	  last_slice+nres => (nslices-1);
	  data_min+nres => cache(<-.in.node_data[0].min);
	  level_min+nres => cache(data_min);
	  data_max+nres => cache(<-.in.node_data[0].max);
	  level_max+nres => cache(data_max);
   };

   XP_COMM_PROJ.VolumeToContour.VolumeToContourMacs.VolumeToContourFunc VolumeToContourFunc{
     &Params => <-.Params;
	 in => <-. in;
   };

   XP_COMM_PROJ.VolumeToContour.VolumeToContourMacs.VolumeToContourUI VolumeToContourUI{
      &Params => <-.Params;
      in => <-.in;
      UImod_panel{
	    title => name_of(<-.<-.<-);
		};
   };
   olink out => VolumeToContourFunc.out;
   olink obj => VolumeToContourFunc.obj;

 };

};
