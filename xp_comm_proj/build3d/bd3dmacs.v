
flibrary Build3DMacs {

   XP_COMM_PROJ.Common.UIWidgets.IAC_StandardUI Build3DUI {

      XP_COMM_PROJ.Common.IOWidgets.IndexedFileParams &IndexedFileParams<NEportLevels={2,1}>;
      XP_COMM_PROJ.Build3D.Build3DMods.Build3DImageParams &Build3DImageParams<NEportLevels={2,1}>;

      panel {
         message = "Select Build3D control panel.";
      };

      mainTitleLabel {
         label => "Build3D control panel.";
      };


      XP_COMM_PROJ.Common.UIWidgets.SmartTextTypein DirectoryTextTypein {
         UIparent => <-.panel;
         slabel = "Directory";
         stext => <-.IndexedFileParams.dir;
         panel {
            x => <-.<-.mainTitleLabel.x;
            y => <-.<-.mainTitleLabel.y + <-.<-.mainTitleLabel.height + 10;
         };
      };
      XP_COMM_PROJ.Common.UIWidgets.SmartTextTypein FileStubTextTypein {
         UIparent => <-.panel;
         slabel = "Filename Stub";
         stext => <-.IndexedFileParams.filename_stub;
         panel {
            x => <-.<-.mainTitleLabel.x;
            y => <-.<-.DirectoryTextTypein.panel.y + <-.<-.DirectoryTextTypein.panel.height + 5;
         };
      };
      XP_COMM_PROJ.Common.UIWidgets.SmartTextTypein ExtensionTextTypein {
         UIparent => <-.panel;
         slabel = "File Extension";
         stext => <-.IndexedFileParams.extension;
         panel {
            x => <-.<-.mainTitleLabel.x;
            y => <-.<-.FileStubTextTypein.panel.y + <-.<-.FileStubTextTypein.panel.height + 5;
         };
      };
      UIslider IndexWidthSlider {
         parent => <-.panel;
         x => <-.mainTitleLabel.x;
         y => <-.ExtensionTextTypein.panel.y + <-.ExtensionTextTypein.panel.height + 5;
         min = 0.;
         max = 4.;
         value+Port2 => <-.IndexedFileParams.index_width;
         mode = "integer";
         title => "Index Width";
         decimalPoints = 0;
         increment = 1.;
      };


      XP_COMM_PROJ.Common.UIWidgets.SmartFieldTypein StartLayerFieldTypein {
         UIparent => <-.panel;
         flabel = "First Layer";
         fval => <-.Build3DImageParams.start_layer;
         fmax => <-.Build3DImageParams.end_layer;
         field {
            min => 0.;
            mode = "integer";
         };
         panel {
            x => <-.<-.mainTitleLabel.x;
            y => <-.<-.IndexWidthSlider.y + <-.<-.IndexWidthSlider.height + 10;
         };
      };
      XP_COMM_PROJ.Common.UIWidgets.SmartFieldTypein EndLayerFieldTypein {
         UIparent => <-.panel;
         flabel = "Last Layer";
         fval => <-.Build3DImageParams.end_layer;
         fmin => <-.Build3DImageParams.start_layer;
         field {
            mode = "integer";
         };
         panel {
            x => <-.<-.mainTitleLabel.x;
            y => <-.<-.StartLayerFieldTypein.panel.y + <-.<-.StartLayerFieldTypein.panel.height + 5;
         };
      };

      UIradioBoxLabel ColorChannelRadioBox {
         parent => <-.panel;
         x => <-.mainTitleLabel.x;
         y => <-.EndLayerFieldTypein.panel.y + <-.EndLayerFieldTypein.panel.height + 5;
         labels => {"alpha", "red", "green", "blue"};
         selectedItem => <-.Build3DImageParams.component;
         title => "Colour Channel to extract...";
      };


      UIbutton StartUIbutton {
         parent => <-.panel;
         label => "Build 3D Layers";
         do+Port2 => <-.Build3DImageParams.start;
         x => (parent.width / 2) - (width / 2);
         y => <-.ColorChannelRadioBox.y + <-.ColorChannelRadioBox.height + 20;
      };


      UIlabel CurrentLayerLabel {
         parent => <-.panel;
         alignment = "left";
         width => parent.width;
         label => "Current Layer: " + <-.Build3DImageParams.count;
         x => <-.StartUIbutton.x;
         y => <-.StartUIbutton.y + <-.StartUIbutton.height + 5;
         width => <-.StartUIbutton.width;
      };
   };



   // Build3DFunc
   //   Functional Macro (Build3DAssemble functional macro, Build3DCreateFilename, Build3DFileParams and Build3DImageParams)
   //   Loops through a number of files and uses the Build3DAssemble macro to create a complete data set.

   macro Build3DFunc {

      XP_COMM_PROJ.Common.IOWidgets.IndexedFileParams &FileParams<NEportLevels={2,1}>;
      XP_COMM_PROJ.Build3D.Build3DMods.Build3DImageParams &ImageParams<NEportLevels={2,1}>;

      GMOD.loop loop {
         reset = 0;
         cycle = 0;
         start_val => <-.ImageParams.start_layer;
         end_val => <-.ImageParams.end_layer;
         incr = 1;
         run+Port2 => <-.ImageParams.start;
         done = 1;
         count+Port2 => <-.ImageParams.count;
      };

      XP_COMM_PROJ.Common.IOWidgets.CreateIndexedFilename CreateIndexedFilename {
         &FileParams => <-.FileParams;
         index => <-.loop.count;
      };

      XP_COMM_PROJ.Build3D.Build3DMods.Build3DAssemble Build3DAssemble {
         &ImageParams => <-.ImageParams;
         filename => <-.CreateIndexedFilename.filename;
         index => (<-.loop.count - <-.ImageParams.start_layer);
      };

      XP_COMM_PROJ.Common.NetworkWidgets.BreakableLink BreakableLink {
         in => <-.Build3DAssemble.out;
         connect => <-.loop.done;
         disconnect => <-.loop.run;
         BreakLinkCore {
            BKL_update+notify_inst;
         };
      };

      Mesh_Unif+Node_Data+Port &built_fld => .BreakableLink.out;

      XP_COMM_PROJ.Build3D.Build3DMods.Build3DExtractScalar extract_scalar {
         in_field => <-.built_fld;
         component => <-.ImageParams.component;
      };

      olink out => .extract_scalar.out_fld;

   };



   // build3D
   //   User Macro (Build3D functional macro, Build3DUI, Build3DFileParams and Build3DImageParams)
   //   Automatically gets the size of the first image and sizes the array according to this

   macro build3D {

      XP_COMM_PROJ.Common.IOWidgets.IndexedFileParams FileParams<NEx=22.,NEy=77.> {
         extension = "tif";
         index_width = 0;
      };


      XP_COMM_PROJ.Common.IOWidgets.CreateIndexedFilename getDimsFilename<NEx=22.,NEy=143.> {
         FileParams => <-.FileParams;
         index => <-.ImageParams.start_layer;
      };

      DVM.DVread_image getDims<NEx=22.,NEy=209.> {
         filename => <-.getDimsFilename.filename;
         flip = 0;
      };


      XP_COMM_PROJ.Build3D.Build3DMods.Build3DImageParams UIImageParams<NEx=286.,NEy=77.> {
         resx => <-.getDims.out.dims[0];
         resy => <-.getDims.out.dims[1];
         count => <-.ImageParams.count;
         start = 0;
         count = 0;
      };
      XP_COMM_PROJ.Common.NetworkWidgets.SortTwoValues SortTwoValues<NEx=286., NEy=132> {
         value1 => <-.UIImageParams.start_layer;
         value2 => <-.UIImageParams.end_layer;
      };
      GMOD.copy_on_change copy_on_change<NEx=396.,NEy=198.> {
         trigger => <-.SortTwoValues.high;
         input => <-.SortTwoValues.low;
      };
      XP_COMM_PROJ.Build3D.Build3DMods.Build3DImageParams ImageParams<NEx=286.,NEy=253.> {
         resx => <-.UIImageParams.resx;
         resy => <-.UIImageParams.resy;
         start_layer => <-.SortTwoValues.low;
         end_layer => <-.SortTwoValues.high;
         start => <-.UIImageParams.start;
         component => <-.UIImageParams.component;
         count => <-.copy_on_change.output;
      };


      XP_COMM_PROJ.Build3D.Build3DMacs.Build3DUI Build3DUI<NEx=209.,NEy=33.> {
         IndexedFileParams => <-.FileParams;
         Build3DImageParams => <-.UIImageParams;
      };
      XP_COMM_PROJ.Build3D.Build3DMacs.Build3DFunc Build3DFunc<NEx=242.,NEy=319.> {
         &ImageParams => <-.ImageParams;
         &FileParams => <-.FileParams;
      };

      olink out<NEx=154.,NEy=385.> => .Build3DFunc.out;
   };


};

