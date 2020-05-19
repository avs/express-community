
flibrary RegionGrowMacs {


//*********************************************************************
  
   macro RegionGrowUI {
      ilink in_field;
      XP_COMM_PROJ.RegionGrow.RegionGrowMods.RegionGrowParams &RegionGrowParams<NEportLevels={2,1}>;

      UImod_panel UImod_panel {
         title => "Region Growing";
         message = "This module segments data by region growing";
      };
      UIframe UIframe_GAUS {
         parent => <-.UImod_panel;
         x = 0;
         y = 20;
         width => (<-.UImod_panel.width - 10);
         height = 220;
      };
      UIlabel UIlabel {
         parent => <-.UIframe_GAUS;
         label => "Gaussian Parameters";
         x => ((<-.UIframe_GAUS.width / 2) - (width / 2));
         y = 0;
         width => <-.UIframe_GAUS.width - 20;
      };
      UIslider UIslider_MIN_IN {
         parent => <-.UIframe_GAUS;
         value => <-.RegionGrowParams.mean_min;
         title => "Minimum ideal mean";
         x = 10;
         y = 35;
         max+nres =>  <-.in_field.node_data[0].max;
         min+nres => (<-.in_field.node_data[0].max * -1);
         width => (<-.UIframe_GAUS.width - 30);
         message = "The lower value for the peak of the object's threshold identity curve";
      };
      UIslider UIslider_MAX_IN {
         parent => <-.UIframe_GAUS;
         value => <-.RegionGrowParams.mean_max;
         title => "Maximum ideal mean";
         x = 10;
         y = 95;
         max+nres =>  <-.in_field.node_data[0].max;
         min+nres => (<-.in_field.node_data[0].max * -1);
         width => (<-.UIframe_GAUS.width - 30);
         message = "The higher value for the peak of the object's threshold identity curve";
      };
      UIslider UIslider_STDDEV {
         parent => <-.UIframe_GAUS;
         value => <-.RegionGrowParams.stddev;
         title => "Ideal standard deviation";
         x = 10;
         y = 150;
         max+nres => <-.in_field.node_data[0].max;
         min = 0.;
         width => (<-.UIframe_GAUS.width - 30);
         message = "Estimated value of the object's threshold curve standard deviation, lower values give steeper curves and larger values give a wider spread";
      };



      UIslider UIslider_REG_TH {
         parent => <-.UImod_panel;
         value => <-.RegionGrowParams.reg_threshold;
         title => "Region Growing Threshold";
         x = 10;
         y => <-.UIframe_GAUS.y + <-.UIframe_GAUS.height + 50;
         min = 0;
         max = 255;
         width => (<-.UImod_panel.width - 10);
         message = "Threshold below which region growing is stopped";
      };


      UIlabel UIlabel_RG {
         parent => <-.UImod_panel;
         label => "Start The Region Growing Process";
         x => ((<-.UImod_panel.width / 2) - (.width / 2));
         y => <-.UIslider_REG_TH.y + <-.UIslider_REG_TH.height + 30;
         width => (<-.UImod_panel.width - 10);
      };
      UItoggle UItoggle {
         parent => <-.UImod_panel;
         label => "Go";
         set => <-.RegionGrowParams.go;
         x => ((<-.UImod_panel.width / 2) - (.width / 2));
         y => <-.UIlabel_RG.y + <-.UIlabel_RG.height + 10;
      };

   };

//**************************************************************************


   macro RegionGrow {
      ilink in_field;
      ilink in_probe;

      XP_COMM_PROJ.RegionGrow.RegionGrowMods.RegionGrowParams &RegionGrowParams<NEportLevels={2,1}>;

      XP_COMM_PROJ.RegionGrow.RegionGrowMods.GenerateGuassianProbs GenerateGuassianProbs {
         mean_min => <-.RegionGrowParams.mean_min;
         mean_max => <-.RegionGrowParams.mean_max;
         stddev   => <-.RegionGrowParams.stddev;
         max      => <-.RegionGrowParams.max;
      };
      XP_COMM_PROJ.RegionGrow.RegionGrowMods.RegionGrowCore RegionGrowCore {
         in            => <-.in_field;
         probe_in      => <-.in_probe;
         coherance     => <-.GenerateGuassianProbs.probs;
         reg_threshold => <-.RegionGrowParams.reg_threshold;
         go            => <-.RegionGrowParams.go;
      };

      olink out_fld => .RegionGrowCore.out;
      olink out_probs => .GenerateGuassianProbs.probs;
   };

//***************************************************************************

   macro region_grow {
      ilink in_field;
      ilink in_probe;

      XP_COMM_PROJ.RegionGrow.RegionGrowMods.RegionGrowParams RegionGrowParams {
         max+nres => <-.in_field.node_data[0].max;
      };

      RegionGrow RegionGrow {
         in_field => <-.in_field;
         in_probe => <-.in_probe;
         RegionGrowParams => <-.RegionGrowParams;
      };
      RegionGrowUI RegionGrowUI {
         in_field => <-.in_field;
         RegionGrowParams => <-.RegionGrowParams;
      };

      GDM.DataObject DataObject {
         in => <-.RegionGrow.out_fld;
      };

      olink out_fld   => .RegionGrow.out_fld;
      olink out_obj   => .DataObject.obj;
      olink out_probs => .RegionGrow.out_probs;
   };


//***************************************************************************

   APPS.MultiWindowApp RegionGrowEg {
      GDM.Uviewer3D Uviewer3D<NEx=308.,NEy=374.> {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Probe.out_obj,
                  <-.<-.<-.Probe.out_anno,
                  <-.<-.<-.Probe.out_title,
                  <-.<-.<-.threshold.out_obj,
                  <-.<-.<-.bounds.out_obj
               };
            };
         };
         Scene_Selector {
            input_views => {<-.Scene.View.View,<-.RegionGrowScene.View.View};
         };
         GDM.Uscene3D RegionGrowScene {
            Top {
               child_objs<NEportLevels={4,1}> => {
                  <-.<-.<-.RG_orthoslice.out_obj,
                  <-.<-.<-.RG_isosurface.out_obj,
                  <-.<-.<-.RG_bounds.out_obj
               };
            };
         };
      };

      GDM.Uviewer2D Uviewer2D<NEx=539.,NEy=374.> {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.General_Graph.GraphViewport.obj
               };
            };
         };
      };



      MODS.Read_Field Read_Field<NEx=66.,NEy=33.> {
         read_field_ui {
            file_browser {
               dirMaskCache = "$XP_ROOT/data/field/*.fld";
            };
            filename = "$XP_ROOT/data/field/lobster.fld";
         };
      };

      MODS.bounds bounds<NEx=22.,NEy=198.> {
         in_field => <-.Read_Field.field;
      };
      MODS.orthoslice orthoslice<NEx=22.,NEy=253.> {
         in_field => <-.Read_Field.field;
         OrthoSliceParam {
            axis = 2;
         };
      };
      MODS.threshold threshold<NEx=22.,NEy=319.> {
         in_field => <-.orthoslice.out_fld;
         ThreshParam {
            above = 1;
            min_value => <-.<-.region_grow.RegionGrowParams.mean_min;
            max_value => <-.<-.region_grow.RegionGrowParams.mean_max;
         };
      };

      HLM.Probe Probe<NEx=187.,NEy=132.> {
         in_field => <-.Read_Field.field;
         in_pick => <-.Uviewer3D.Scene.View.View.picked_obj;
      };


      XP_COMM_PROJ.RegionGrow.RegionGrowMacs.region_grow region_grow<NEx=429.,NEy=154.> {
         RegionGrowParams {
            mean_min = 30.0;
            mean_max = 180.0;
            stddev = 8.39;
            reg_threshold = 90;
         };
         in_field => <-.Read_Field.field;
         in_probe => <-.Probe.out_fld; 
      };


      MODS.bounds RG_bounds<NEx=528.,NEy=220.> {
         in_field => <-.region_grow.out_fld;
      };
      MODS.orthoslice RG_orthoslice<NEx=429.,NEy=253.> {
         in_field => <-.region_grow.out_fld;
         OrthoSliceParam {
            axis = 2;
         };
      };
      MODS.isosurface RG_isosurface<NEx=374.,NEy=308.> {
         in_field => <-.region_grow.out_fld;
      };

      AGHLM.General_Graph General_Graph<NEx=649.,NEy=187.> {
         ValuesY1 => <-.region_grow.out_probs;

         Graph#1 {
            Gparams {
               Graph_params1 {
                  graphtype = 2;
                  line_color = 9;
               };
            };
         };
         Graph#2 {
            Gparams {
               Graph_params2 {
                  Graph_on_off = 0;
               };
            };
         };
         Graph#3 {
            Gparams {
               Graph_params3 {
                  Graph_on_off = 0;
               };
            };
         };
         Graph#4 {
            Gparams {
               Graph_params4 {
                  Graph_on_off = 0;
               };
            };
         };
         Graph#5 {
            Gparams {
               Graph_params5 {
                  Graph_on_off = 0;
               };
            };
         };
         GraphLegend {
            Legend_params {
               Legend_on_off = 0;
            };
         };
         XYAxis {
            X_Axis_params {
               X_Axis_text = "Input Value";
            };
            Y_Axis_params {
               label_Decimals = 2.;
               Y_Axis_text = "Probability";
            };
         };
      };

   };


//**************************************************************************

   macro AccumulateUI {
      XP_COMM_PROJ.RegionGrow.RegionGrowMods.AccumulateParams &AccumulateParams<NEportLevels={2,1}>; 

      UImod_panel UImod_panel {
         title = "Accumulate Regions";
      };
      UIlabel UIlabel_l1 {
         parent => <-.UImod_panel;
         label = "Accumulate Regions Created By";
         x = 5;
         y = 0;
         width => (<-.UImod_panel.width - 10);
      };
      UIlabel UIlabel_l2 {
         parent => <-.UImod_panel;
         label = "Region Grow Module";
         x = 5;
         y => (<-.UIlabel_l1.y + <-.UIlabel_l1.height + 5);
         width => (<-.UImod_panel.width - 10);
      };

      UItoggle UItoggleGo {
         parent => <-.UImod_panel;
         label = "Go";
         x = 5;
         y => (<-.UIlabel_l2.y + <-.UIlabel_l2.height + 25);
         message = "Press this to add the region.";
         set => <-.AccumulateParams.go;
      };
      UItoggle UItoggleReset {
         parent => <-.UImod_panel;
         label = "Reset";
         x = 5;
         y => (<-.UItoggleGo.y + <-.UItoggleGo.height + 10);
         message = "Press this to set the volume to zero values";
         set => <-.AccumulateParams.reset;
      };
      UItoggle UItoggleChoice {
         parent => <-.UImod_panel;
         label => "Set your own dimensions";
         x = 5;
         y => (<-.UItoggleReset.y + <-.UItoggleReset.height + 10);
         message = "Press this so you can put your own dimensions in.";
         set => <-.AccumulateParams.dims_choice;
      };


      UIframe UIframe {
         visible => <-.UItoggleChoice.set;
         parent => <-.UImod_panel;
         y => (<-.UItoggleChoice.y + <-.UItoggleChoice.height + 20);
         width => (<-.UImod_panel.width - 10);
         height = 200;
      };
      UIlabel UIlabelx {
         parent => <-.UIframe;
         label = "x:";
         y = 20;
         width = 30;
      };
      UIfield UIfieldx {
         parent => <-.UIframe;
         x => <-.UIlabelx.x + <-.UIlabelx.height + 10;
         y => <-.UIlabelx.y;
         width => (<-.UIframe.width - x - 10);
         message = "The value for the x dimension.";
         value => <-.AccumulateParams.dims[0];
      };
      UIlabel UIlabely {
         parent => <-.UIframe;
         label = "y:";
         y => (<-.UIfieldx.y + <-.UIfieldx.height + 10);
         width = 30;
      };
      UIfield UIfieldy {
         parent => <-.UIframe;
         x => <-.UIlabely.x + <-.UIlabely.height + 10;
         y => <-.UIlabely.y;
         width => (<-.UIframe.width - x - 10);
         message = "The value for the y dimension.";
         value => <-.AccumulateParams.dims[1];
      };
      UIlabel UIlabelz {
         parent => <-.UIframe;
         label = "z:";
         y => (<-.UIfieldy.y + <-.UIfieldy.height + 10);
         width = 30;
      };
      UIfield UIfieldz {
         parent => <-.UIframe;
         x => <-.UIlabelz.x + <-.UIlabelz.height + 10;
         y => <-.UIlabelz.y;
         width => (<-.UIframe.width - x - 10);
         message = "The value for the z dimension.";
         value => <-.AccumulateParams.dims[2];
      };

   };


//**************************************************************************

   macro Accumulate {
      ilink in_field;
      XP_COMM_PROJ.RegionGrow.RegionGrowMods.AccumulateParams &AccumulateParams<NEportLevels={2,1}>;

      XP_COMM_PROJ.RegionGrow.RegionGrowMods.AccumulateCore AccumulateCore {
         in_fld => <-.in_field;
         params => <-.AccumulateParams;
         dtype = 6;
      };

      olink out_fld => AccumulateCore.out_fld;
   };

//**************************************************************************

   macro accumulate_regions {
      ilink in_field;

      XP_COMM_PROJ.RegionGrow.RegionGrowMods.AccumulateParams AccumulateParams {
         go          = 0;
         reset       = 1;
         dims_choice = 0;
         dims        = {0,0,0};
      }; 

      AccumulateUI AccumulateUI {
         AccumulateParams => <-.AccumulateParams;
      };
      Accumulate Accumulate {
         in_field => <-.in_field;
         AccumulateParams => <-.AccumulateParams;
      };

      GDM.DataObject DataObject {
         in => <-.Accumulate.out_fld;
      };

      olink out_fld => .Accumulate.out_fld;
      olink out_obj => .DataObject.obj;
   };


//**************************************************************************

   APPS.MultiWindowApp AccumulateRegionsEg {
      GDM.Uviewer3D Uviewer3D<NEx=308.,NEy=374.> {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Probe.out_obj,
                  <-.<-.<-.Probe.out_anno,
                  <-.<-.<-.Probe.out_title,
                  <-.<-.<-.threshold.out_obj,
                  <-.<-.<-.bounds.out_obj
               };
            };
         };
         Scene_Selector {
            input_views => {<-.Scene.View.View,<-.AccumulateRegionsScene.View.View};
         };
         GDM.Uscene3D AccumulateRegionsScene {
            Top {
               child_objs<NEportLevels={4,1}> => {
                  <-.<-.<-.ACC_orthoslice.out_obj,
                  <-.<-.<-.ACC_isosurface.out_obj,
                  <-.<-.<-.ACC_bounds.out_obj,
                  <-.<-.<-.RG_isosurface.out_obj
               };
            };
         };
      };

      GDM.Uviewer2D Uviewer2D<NEx=539.,NEy=374.> {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.General_Graph.GraphViewport.obj
               };
            };
         };
      };


      MODS.Read_Field Read_Field<NEx=66.,NEy=33.> {
         read_field_ui {
            file_browser {
               dirMaskCache = "$XP_ROOT/data/field/*.fld";
            };
            filename = "$XP_ROOT/data/field/lobster.fld";
         };
      };

      MODS.bounds bounds<NEx=22.,NEy=198.> {
         in_field => <-.Read_Field.field;
      };
      MODS.orthoslice orthoslice<NEx=22.,NEy=253.> {
         in_field => <-.Read_Field.field;
         OrthoSliceParam {
            axis = 2;
         };
      };
      MODS.threshold threshold<NEx=22.,NEy=319.> {
         in_field => <-.orthoslice.out_fld;
         ThreshParam {
            above = 1;
            min_value => <-.<-.region_grow.RegionGrowParams.mean_min;
            max_value => <-.<-.region_grow.RegionGrowParams.mean_max;
         };
      };


      HLM.Probe Probe<NEx=187.,NEy=132.> {
         in_field => <-.Read_Field.field;
         in_pick => <-.Uviewer3D.Scene.View.View.picked_obj;
      };

      XP_COMM_PROJ.RegionGrow.RegionGrowMacs.region_grow region_grow<NEx=429.,NEy=130.> {
         RegionGrowParams {
            mean_min = 30.0;
            mean_max = 180.0;
            stddev = 8.39;
            reg_threshold = 90;
         };
         in_field => <-.Read_Field.field;
         in_probe => <-.Probe.out_fld;
      };

      XP_COMM_PROJ.RegionGrow.RegionGrowMacs.accumulate_regions accumulate_regions<NEx=374.,NEy=190.> {
         in_field => <-.region_grow.out_fld; 
      };

      MODS.isosurface RG_isosurface<NEx=594.,NEy=242.> {
         in_field => <-.region_grow.out_fld;
         obj {
            Datamap {
               DataRange[3] = {
                  {
                     DataMaxValue=>(<-.dataMin + ((<-.dataMax - <-.dataMin) * 0.5)),
                     UIMaxValue=127.5,
                     size=128,,,
                     selectAlphaRange=0,
                     ,,,,,,,,,,
                  },
                  {
                     DataMinValue=>DataRange[0].DataMaxValue,UIMinValue=127.5,,,,
                     DataMaxValue=>(<-.dataMin + ((<-.dataMax - <-.dataMin) * 0.75)),
                     UIMaxValue=191.25,
                     size=64,,,,,,,,,,
                     controlPoints=>{<-.DatamapValue[1],<-.DatamapValue[2]},,,,
                  },
                  {
                     DataMinValue=>DataRange[1].DataMaxValue,
                     UIMinValue=191.25,,,,
                     DataMaxValue=><-.dataMax,,
                     size=64,,,,,,,,,,
                     controlPoints=>{<-.DatamapValue[2],<-.DatamapValue[3]},,,,
                  }
               };
               currentColorModel = 1;
               DatamapValue[4] = {
                  {
                     v2=0.,v3=0.,v4=0.,v1=1.,
                  },
                  {
                     v2=1.,v3=0.,v4=0.,
                  },
                  {
                     v1=1.,v2=1.,v3=0.5,v4=0.,
                  },
                  {
                     v1=1.,v2=1.,v3=1.,v4=1.,
                  }
               };
            };
         };
      };

      MODS.bounds ACC_bounds<NEx=440.,NEy=242.> {
         in_field => <-.accumulate_regions.out_fld;
      };
      MODS.orthoslice ACC_orthoslice<NEx=407.,NEy=275.> {
         in_field => <-.accumulate_regions.out_fld;
         OrthoSliceParam {
            axis = 2;
         };
      };
      MODS.isosurface ACC_isosurface<NEx=374.,NEy=308.> {
         in_field => <-.accumulate_regions.out_fld;
      };

      AGHLM.General_Graph General_Graph<NEx=649.,NEy=160.> {
         ValuesY1 => <-.region_grow.out_probs;

         Graph#1 {
            Gparams {
               Graph_params1 {
                  graphtype = 2;
                  line_color = 9;
               };
            };
         };
         Graph#2 {
            Gparams {
               Graph_params2 {
                  Graph_on_off = 0;
               };
            };
         };
         Graph#3 {
            Gparams {
               Graph_params3 {
                  Graph_on_off = 0;
               };
            };
         };
         Graph#4 {
            Gparams {
               Graph_params4 {
                  Graph_on_off = 0;
               };
            };
         };
         Graph#5 {
            Gparams {
               Graph_params5 {
                  Graph_on_off = 0;
               };
            };
         };
         GraphLegend {
            Legend_params {
               Legend_on_off = 0;
            };
         };
         XYAxis {
            X_Axis_params {
               X_Axis_text = "Input Value";
            };
            Y_Axis_params {
               label_Decimals = 2.;
               Y_Axis_text = "Probability";
            };
         };
      };


   };

};

