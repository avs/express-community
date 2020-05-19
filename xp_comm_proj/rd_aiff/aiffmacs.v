
flibrary ReadAiffMacs {

   // ReadAiffUI
   //   UI Macro
   //   UImod_panel containing 4 UIfieldTypein macros and associated strings

   macro ReadAiffUI {
      XP_COMM_PROJ.ReadAiff.ReadAiffMods.ReadAiffParams &parameters <NEportLevels={2,1},NEx=121.,NEy=22.>;

      UImod_panel readAiff_panel<NEx=352.,NEy=407.> {
         title = "ReadAiff";
      };
      UIfieldTypein NumChanUIfieldTypein<NEx=198.,NEy=154.> {
         UIparent => <-.readAiff_panel;
         flabel => <-.NumChanString;
         fval => <-.parameters.Num_channels;
         field {
            decimalPoints = 0;
            outputOnly = 1;
         };
         y = 0;
      };
      UIfieldTypein SampleSizeUIfieldTypein<NEx=209.,NEy=209.> {
         UIparent => <-.readAiff_panel;
         flabel => <-.SampleSizeString;
         fval => <-.parameters.Sample_size;
         field {
            decimalPoints = 0;
            outputOnly = 1;
         };
         y = 40;
      };
      UIfieldTypein NumSamplesUIfieldTypein<NEx=220.,NEy=264.> {
         UIparent => <-.readAiff_panel;
         flabel => <-.NumSamplesString;
         fval => <-.parameters.Num_samples;
         field {
            decimalPoints = 0;
            outputOnly = 1;
         };
         y = 80;
      };
      UIfieldTypein SampleRateUIfieldTypein<NEx=231.,NEy=319.> {
         UIparent => <-.readAiff_panel;
         flabel => <-.SampleRateString;
         fval => <-.parameters.Sample_rate;
         field {
            outputOnly = 1;
         };
         y = 120;
      };
      string NumChanString<NEportLevels=1,NEx=11.,NEy=154.> = "Number of Channels";
      string SampleSizeString<NEportLevels=1,NEx=22.,NEy=209.> = "Sample Size";
      string NumSamplesString<NEportLevels=1,NEx=33.,NEy=264.> = "Number of Samples";
      string SampleRateString<NEportLevels=1,NEx=44.,NEy=319.> = "Sample Rate";

      UIfileDialog AiffUIfileDialog<NEx=550.,NEy=121.> {
         visible => <-.OpenFileUIButton.do;
         parent => <-.readAiff_panel;
         title => <-.DialogNameString;
         filename<NEportLevels={2,2}> => <-.parameters.file_name;
         searchPattern = "*.aiff";
      };
      string DialogNameString<NEportLevels=1,NEx=759.,NEy=121.> = "Open AIFF File...";

      UIlabel AIFF_Filename<NEx=605.,NEy=286.> {
         parent => <-.readAiff_panel;
         label<NEdisplayMode="open">;
         y = 180;
      };
      UItext UItext<NEx=594.,NEy=209.> {
         parent => <-.readAiff_panel;
         text<NEportLevels={2,2}> => <-.AiffUIfileDialog.filename;
         y = 210;
         width = 175;
      };
      UIbutton OpenFileUIButton<NEx=550.,NEy=22.> {
         parent => <-.readAiff_panel;
         label => <-.OpenFileString;
         x = 185;
         y = 210;
         width = 60;
      };
      string OpenFileString<NEportLevels=1,NEx=748.,NEy=22.> = "Browse...";

   };


   // ReadAiff
   //   User Macro (ReadAiffData C module, reference to ReadAiffParams and a link out)

   macro ReadAiff {
      XP_COMM_PROJ.ReadAiff.ReadAiffMods.ReadAiffParams &ReadAiffParams<NEportLevels={2,1}>;

      XP_COMM_PROJ.ReadAiff.ReadAiffMods.ReadAiffData ReadAiffData {
         parameters => <-.ReadAiffParams;
      };

      olink out_data => .ReadAiffData.out_data;
   };


   // readAiff
   //   User Macro (ReadAiff user macro, ReadAiffParams, and ReadAiffUI)
   macro readAiff {
      XP_COMM_PROJ.ReadAiff.ReadAiffMods.ReadAiffParams ReadAiffParams;

      XP_COMM_PROJ.ReadAiff.ReadAiffMacs.ReadAiffUI ReadAiffUI {
         parameters => <-.ReadAiffParams;
      };

      XP_COMM_PROJ.ReadAiff.ReadAiffMacs.ReadAiff ReadAiff {
         ReadAiffParams => <-.ReadAiffParams;
      };

      olink out_data => .ReadAiff.out_data;
   };


   APPS.SingleWindowApp ReadAiffEg {
      GDM.Uviewer2D Uviewer2D<NEx=198.,NEy=231.> {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.AGDataObject.obj};
            };
            Camera {
               Camera {
                  auto_norm = "None";
               };
            };
         };
      };

      AGDataObject AGDataObject<NEx=440.,NEy=319.> {
         in => <-.AGGraphViewport;
      };
      AGGraphViewport AGGraphViewport<NEx=440.,NEy=253.,NEwidth=385,NEheight=220> {
         children => {<-.AGGraphWorld};
      };
      AGGraphWorld AGGraphWorld<NEx=440.,NEy=176.,NEwidth=187,NEheight=220> {
         children => {
            <-.AGGraph,<-.AGYAxis,<-.AGYTicklines,<-.AGXAxis,
            <-.AGXTicklines
         };
         niceLimits = 0;
         textColor = "cyan";
         majorTickColor = "white";
         majorTickLineStyle = "dashed";
      };
      AGGraph AGGraph<NEx=143.,NEy=110.> {
         valuesY => readAiff.out_data;
         priority = 0;
         color = "magenta";
      };
      AGYAxis AGYAxis<NEx=550,NEy=33> {
         textColor = "yellow";
         text = "amplitude";
      };
      AGYTicklines AGYTicklines<NEx=550,NEy=66> {
         priority = 0;
      };
      AGXAxis AGXAxis<NEx=374,NEy=33,NEwidth=209,NEheight=242> {
         textColor = "yellow";
         text = "samples";
      };
      AGXTicklines AGXTicklines<NEx=374,NEy=66,NEwidth=198,NEheight=231> {
         priority = 0;
         majorTickLineStyle = "dotted";
         majorTickOn = 1;
      };

      XP_COMM_PROJ.ReadAiff.ReadAiffMacs.readAiff readAiff<NEx=132.,NEy=44.>;
   };


};

