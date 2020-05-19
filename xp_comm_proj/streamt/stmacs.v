
flibrary StreamTimeMacs <compile_subs=0> {

   // UI macro
   macro StreamTimeUI {
      UImod_panel UImod_panel<NEx=11.,NEy=11.> {
         title => name_of(<-.<-.<-);
      };

      XP_COMM_PROJ.StreamTime.StreamTimeMods.StreamTimeParams &StreamTimeParams<NEx=550.,NEy=55.,NEportLevels={2,1}>;

      UIslider axisSlider<NEx=198.,NEy=11.> {
         parent => <-.UImod_panel;
         value = 2.;
         title => "2D projection axis";
         y = 8;
         width => w;
         min = 0.;
         max = 2.;
         mode = "integer";
         decimalPoints = 0;
         immediateMode = 0;
         value => StreamTimeParams.axis;
      };
      UItoggle xformToggle<NEx=198.,NEy=44.> {
         parent => <-.UImod_panel;
         label => "Do xform on 2D datasets";
         y => ((<-.axisSlider.y + <-.axisSlider.height) + axisSlider.y);
         width => <-.w;
         set => StreamTimeParams.do_transform;
      };
      int w<NEportLevels=1,NEx=11.,NEy=44.> => UImod_panel.clientWidth;
      UIlabel zsLabel<NEx=198.,NEy=77.> {
         parent => <-.UImod_panel;
         label => "Z Scale for 2D transform";
         y => ((xformToggle.y + xformToggle.height) + axisSlider.y);
         width = 160;
         alignment = "left";
      };
      UIfield zsField<NEx=341.,NEy=77.> {
         parent => <-.UImod_panel;
         value = 1.;
         y => (zsLabel.y - (height / 5));
         x => (zsLabel.x + zsLabel.width);
         width => (w - x);
         value => StreamTimeParams.z_scale;
      };
      UIslider resSlider<NEx=198.,NEy=110.> {
         parent => <-.UImod_panel;
         value = 3.;
         title => "Number of facets on tubes";
         y => ((zsField.y + zsField.height) + axisSlider.y);
         min = 3.;
         max = 36.;
         mode = "integer";
         decimalPoints = 0;
         immediateMode = 0;
         value => StreamTimeParams.resolution;
      };
      UItoggle normToggle<NEx=198.,NEy=143.> {
         parent => <-.UImod_panel;
         label => "Normalise tube radii to Radius Scale";
         y => ((resSlider.y + resSlider.height) + axisSlider.y);
         width => w;
         label => "Normalise tubes to Radius Scale";
         set => StreamTimeParams.normalise;
      };
      UIlabel scaleLabel<NEx=198.,NEy=176.> {
         parent => <-.UImod_panel;
         label => "Radius Scale";
         y => ((normToggle.y + normToggle.height) + axisSlider.y);
         width = 160;
         alignment = "left";
      };
      UIfield scaleField<NEx=341.,NEy=176.> {
         parent => <-.UImod_panel;
         value = 1.;
         y => (scaleLabel.y - (height / 5));
         x => (scaleLabel.x + scaleLabel.width);
         width => (w - x);
         value => <-.StreamTimeParams.radius_scale;
         decimalPoints = 6;
      };
      UItoggle ncToggle<NEx=198.,NEy=209.> {
         parent => <-.UImod_panel;
         label => "Select node data (off) or cell data (on)";
         y => ((scaleField.y + scaleField.height) + axisSlider.y);
         width => w;
         set => StreamTimeParams.node_cell;
      };
      UIlabel ndLabel<NEx=198.,NEy=242.> {
         parent => <-.UImod_panel;
         label => "Node data component";
         y => ((ncToggle.y + ncToggle.height) + axisSlider.y);
         width = 160;
         active => (!ncToggle.set);
         alignment = "left";
      };
      UIlabel cdLabel<NEx=198.,NEy=275.> {
         parent => <-.UImod_panel;
         label => "Cell data component";
         y => ((ndField.y + ndField.height) + axisSlider.y);
         width = 160;
         active => (!ndLabel.active);
         alignment = "left";
      };
      UIfield ndField<NEx=341.,NEy=242.> {
         parent => <-.UImod_panel;
         value = 0.;
         y => (ndLabel.y - (height / 5));
         x => (ndLabel.x + ndLabel.width);
         width => (w - x);
         active => ndLabel.active;
         min = 0.;
         mode = "integer";
         decimalPoints = 0;
         value => <-.StreamTimeParams.node_data_comp;
      };
      UIfield cdField<NEx=341.,NEy=275.> {
         parent => <-.UImod_panel;
         value = -1.;
         y => (cdLabel.y - (height / 5));
         x => (colLabel.x + colLabel.width);
         width => (w - x);
         active => cdLabel.active;
         min = 0.;
         mode = "integer";
         decimalPoints = 0;
         value => <-.StreamTimeParams.cell_data_comp;
      };
      UIlabel colLabel<NEx=198.,NEy=308.> {
         parent => <-.UImod_panel;
         label => "Colour component";
         y => ((cdField.y + cdField.height) + axisSlider.y);
         width = 160;
         alignment = "left";
      };
      UIfield colField<NEx=341.,NEy=308.> {
         parent => <-.UImod_panel;
         value = 0.;
         y => (colLabel.y - (height / 5));
         x => (colLabel.x + colLabel.width);
         width => (w - x);
         min = 0.;
         mode = "integer";
         decimalPoints = 0;
         value => <-.StreamTimeParams.col_data_comp;
      };
   };


   // Higher level stream_time macro, containing params block an I/O
   macro StreamTimeFunc {
      ilink in;

      XP_COMM_PROJ.StreamTime.StreamTimeMods.StreamTimeParams &StreamTimeParams<NEportLevels={2,1}>;

      XP_COMM_PROJ.StreamTime.StreamTimeMods.StreamTimeCore StreamTimeCore {
         in => <-.in;
         axis => <-.StreamTimeParams.axis;
         xform => <-.StreamTimeParams.do_transform;
         zscale => <-.StreamTimeParams.z_scale;
         resolution => <-.StreamTimeParams.resolution;
         normalise => <-.StreamTimeParams.normalise;
         scale => <-.StreamTimeParams.radius_scale;
         node_cell => <-.StreamTimeParams.node_cell;
         node_data_comp => <-.StreamTimeParams.node_data_comp;
         cell_data_comp => <-.StreamTimeParams.cell_data_comp;
         col_data_comp => <-.StreamTimeParams.col_data_comp;
      };

      GDM.DataObject out_GD_obj {
         in => StreamTimeCore.out;
         Obj.name => name_of(<-.<-.<-);
      };

      olink out_fld => .StreamTimeCore.out;
      olink out_obj => out_GD_obj.obj;
   };



   // user macro
   macro stream_time {
      ilink in_fld;

      XP_COMM_PROJ.StreamTime.StreamTimeMods.StreamTimeParams StreamTimeParams {
         axis = 2;
         do_transform = 1;
         z_scale = 1.;
         resolution = 3;
         normalise = 0;
         radius_scale = 1.;
         node_cell = 0;
         node_data_comp = 0;
         cell_data_comp = 0;
         col_data_comp = 0;
      };

      XP_COMM_PROJ.StreamTime.StreamTimeMacs.StreamTimeUI StreamTimeUI {
         StreamTimeParams => <-.StreamTimeParams;
      };

      XP_COMM_PROJ.StreamTime.StreamTimeMacs.StreamTimeFunc StreamTimeFunc {
         StreamTimeParams => <-.StreamTimeParams;
         in => <-.in_fld;
         out_GD_obj.Obj.name => name_of(<-.<-.<-.<-);
      };

      olink out_fld => StreamTimeFunc.out_fld;
      olink out_obj => StreamTimeFunc.out_obj;
   };

};

