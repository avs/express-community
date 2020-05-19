
flibrary IsoObjectLabelMacs <compile_subs=0> {

   macro IsoObjectLabelUI {
      UImod_panel UImod_panel {
         title => "IsoObjectLabel";
      };
      XP_COMM_PROJ.IsoObjectLabel.IsoObjectLabelMods.IsoObjectLabelParams+IPort2 &params;
      UIslider contourFrequencySlider {
         parent => <-.UImod_panel;
         value => <-.params.ContourFrequency;
         title => "Contour Frequency of Labels";
         y=0;
         min = 1.;
         max = 10.;
         mode = "integer";
         width => parent.clientWidth;
      };
      UIslider nodeFrequencySlider {
         parent => <-.UImod_panel;
         value => <-.params.NodeFrequency;
         title => "Node Frequency of Labels";
         min = 1.;
         max = 250.;
         mode = "integer";
         y => <-.contourFrequencySlider.y + <-.contourFrequencySlider.height + 10;
         width => parent.clientWidth;
      };
	  UItoggle isolabelToggle{
         parent => <-.UImod_panel;
         set => <-.params.LineIndependent;
         label = "Independent from number of isolines";
         y => <-.nodeFrequencySlider.y + <-.nodeFrequencySlider.height + 10;
         width => parent.clientWidth;
      };
      UIslider num_labels_slider {
         parent => <-.UImod_panel;
         value => <-.params.NumberOfLabels;
         title => "Number of Labels";
         min = 0.;
         max = 50.;
         mode = "integer";
         active => <-.params.LineIndependent;
         y => <-.isolabelToggle.y + <-.isolabelToggle.height+10;
         width => parent.clientWidth;
      };
      UIslider decimalsSlider {
         parent => <-.UImod_panel;
         value => <-.params.Decimals;
         title => "Number of Decimal Points";
         min = 0.;
         max = 10.;
         mode = "integer";
         y => <-.num_labels_slider.y + <-.num_labels_slider.height + 10;
         width => parent.clientWidth;
      };
   };

   macro IsoObjectLabelFunc {
      ilink in_fld;
      XP_COMM_PROJ.IsoObjectLabel.IsoObjectLabelMods.IsoObjectLabelParams &params<NEportLevels={2,1}>;
      XP_COMM_PROJ.IsoObjectLabel.IsoObjectLabelMods.IsoObjectLabelCore IsoObjectLabelCore {
         in_fld => <-.in_fld;
         params => <-.params;
      };
      link+Port   out_vals => IsoObjectLabelCore.out.node_data[0].values;
      string+Port string_vals[] => str_format(params.Format, out_vals);
      olink out_fld  => IsoObjectLabelCore.out;
      olink out_text => string_vals;
   };

   macro IsoObjectLabel {
      Mesh+Node_Data+IPort2& in_fld;
      XP_COMM_PROJ.IsoObjectLabel.IsoObjectLabelMods.IsoObjectLabelParams params {
         Active = 1;
         ContourFrequency = 1;
         NodeFrequency = 25;
         LineIndependent = 0;
         NumberOfLabels = 0;
         Decimals = 2;
         Format => "%." + Decimals + "f";
      };
      XP_COMM_PROJ.IsoObjectLabel.IsoObjectLabelMacs.IsoObjectLabelFunc IsoObjectLabelFunc {
         in_fld => <-.in_fld;
         params => <-.params;
      };
      XP_COMM_PROJ.IsoObjectLabel.IsoObjectLabelMods.MaxFrequency MaxFrequency{
         in_fld => <-.in_fld;
      };
      XP_COMM_PROJ.IsoObjectLabel.IsoObjectLabelMacs.IsoObjectLabelUI IsoObjectLabelUI {
         params => <-.params;
         contourFrequencySlider.max => <-.<-.MaxFrequency.contourFreqMax;
         nodeFrequencySlider.max => <-.<-.MaxFrequency.nodeFreqMax;
      };
      MODS.text_glyph text_glyph {
         in_mesh => <-.IsoObjectLabelFunc.out_fld;
         in_text => <-.IsoObjectLabelFunc.out_text;
         UIlabel label{
            parent => IsoObjectLabelUI.UImod_panel;
            y => <-.<-.IsoObjectLabelUI.decimalsSlider.y + <-.<-.IsoObjectLabelUI.decimalsSlider.height + 10;
            width => parent.clientWidth;
            label = "Text glyph parameters";
         };
         UIframe UImod_panel{
            parent => IsoObjectLabelUI.UImod_panel;
            x = 0;
            y => <-.label.y + <-.label.height;
            width => parent.clientWidth;
            height => ((<-.TextUI.StrokeText.y + <-.TextUI.StrokeText.height) + 6);
         };
         TextUI<NEdisplayMode="maximized"> {
            Font {
              text = "-adobe-times-bold-r-*-*-16-*-*-*-*-*-*-*";
              width => (parent.clientWidth - x);
            };
         };
      };
      olink out_fld  => IsoObjectLabelFunc.out_fld;
      olink out_text => IsoObjectLabelFunc.out_text;
      olink out_obj  => text_glyph.out_obj;
   };

}; // end of flibrary

