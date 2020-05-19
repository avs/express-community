
flibrary ReadSpotMacs<compile_subs=0> {

   macro ReadSpotUI {
      XP_COMM_PROJ.ReadSpot.ReadSpotMods.ReadSpotInputParams &input_params<NEportLevels={2,1}>;
      XP_COMM_PROJ.ReadSpot.ReadSpotMods.ReadSpotOutputParams &output_params<NEportLevels={2,1}>;

      UImod_panel parent {
         title => name_of(<-.<-.<-);
         parent<NEportLevels={4,0}>;
      };

      UIlabel UIlabelReadSpotTitle {
         parent => <-.parent;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Read SPOT";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

      UIlabel UIlabelHeaderFileName {
         parent => <-.parent;
         label => "Header File Name";
         y => <-.UIlabelReadSpotTitle.y + <-.UIlabelReadSpotTitle.height + 10;
      };
      UItext HeaderFileName {
         parent => <-.parent;
         text => <-.input_params.HeaderFileName;
         x => <-.UIlabelHeaderFileName.x + <-.UIlabelHeaderFileName.width + 10;
         y => <-.UIlabelHeaderFileName.y;
      };
      UIbutton UIbuttonHeaderFileName {
         parent => <-.parent;
         label => "...";
         x => <-.HeaderFileName.x + <-.HeaderFileName.width + 10;
         y => <-.HeaderFileName.y;
         width = 30;
      };
      UIfileDialog HeaderFileNameDialog {
         visible => <-.UIbuttonHeaderFileName.do;
         filename => <-.input_params.HeaderFileName;
         parent => <-.parent;
         title => "SPOT Header File";
         searchPattern = "*.dat";
         dirMaskCache = "*.dat";
      };

      UIlabel UIlabelImageFileName {
         parent => <-.parent;
         label => "Image File Name";
         y => <-.HeaderFileName.y + <-.HeaderFileName.height + 5;
      };
      UItext ImageFileName {
         parent => <-.parent;
         text => <-.input_params.ImageFileName;
         x => <-.UIlabelImageFileName.x + <-.UIlabelImageFileName.width + 10;
         y => <-.UIlabelImageFileName.y;
      };
      UIbutton UIbuttonImageFileName {
         parent => <-.parent;
         label => "...";
         x => <-.ImageFileName.x + <-.ImageFileName.width + 10;
         y => <-.ImageFileName.y;
         width = 30;
      };
      UIfileDialog ImageFileNameDialog {
         visible => <-.UIbuttonImageFileName.do;
         filename => <-.input_params.ImageFileName;
         parent => <-.parent;
         title => "SPOT Image File";
         searchPattern = "*.dat";
         dirMaskCache = "*.dat";
      };


      UIlabel UIlabelReadImage {
         parent => <-.parent;
         label => "Update Image";
         y => <-.ImageFileName.y + <-.ImageFileName.height + 15;
      };
      UItoggle UItoggleReadImage {
         parent => <-.parent;
         label => " ";
         set => <-.input_params.ActivateFlag;
         x => <-.UIlabelReadImage.x + <-.UIlabelReadImage.width + 10;
         y => <-.UIlabelReadImage.y;
      };


      UIlabel UIlabelProcessingLevel {
         parent => <-.parent;
         label => "Processing Level";
         y => <-.UIlabelReadImage.y + <-.UIlabelReadImage.height + 30;
      };
      UItext ProcessingLevel {
         parent => <-.parent;
         text => <-.output_params.ProcessingLevel;
         x => <-.UIlabelProcessingLevel.width + 10;
         y => <-.UIlabelProcessingLevel.y;
         active = 0;
      };

      UIlabel UIlabelNumberOfBands {
         parent => <-.parent;
         label => "Number Of Bands";
         y => <-.ProcessingLevel.y + <-.ProcessingLevel.height + 2;
      };
      UIfield NumberOfBands {
         parent => <-.parent;
         value => <-.output_params.NumberOfBands;
         x => <-.UIlabelNumberOfBands.width + 10;
         y => <-.UIlabelNumberOfBands.y;
         active = 0;
      };

      UIlabel UIlabelTotalNumberOfRows {
         parent => <-.parent;
         label => "Total Num. Of Rows";
         y => <-.NumberOfBands.y + <-.NumberOfBands.height + 2;
      };
      UIfield TotalNumberOfRows {
         parent => <-.parent;
         value => <-.output_params.NumberOfRows;
         x => <-.UIlabelTotalNumberOfRows.width + 10;
         y => <-.UIlabelTotalNumberOfRows.y;
         active = 0;
      };

      UIlabel UIlabelTotalNumberOfColumns {
         parent => <-.parent;
         label => "Total Num. Of Cols.";
         y => <-.TotalNumberOfRows.y + <-.TotalNumberOfRows.height + 2;
      };
      UIfield TotalNumberOfColumns {
         parent => <-.parent;
         value => <-.output_params.NumberOfColumns;
         x => <-.UIlabelTotalNumberOfColumns.width + 10;
         y => <-.UIlabelTotalNumberOfColumns.y;
         active = 0;
      };

      UIlabel UIlabelDate {
         parent => <-.parent;
         label => "Date";
         y => <-.TotalNumberOfColumns.y + <-.TotalNumberOfColumns.height + 2;
      };
      UItext Date {
         parent => <-.parent;
         text => str_format("%02d/%02d/%04d", <-.output_params.Day,
                                              <-.output_params.Month,
                                              <-.output_params.Year);
         x => <-.UIlabelDate.width + 10;
         y => <-.UIlabelDate.y;
         active = 0;
      };

      UIlabel UIlabelTime {
         parent => <-.parent;
         label => "Time";
         y => <-.Date.y + <-.Date.height + 2;
      };
      UItext Time {
         parent => <-.parent;
         text => str_format("%02d/%02d/%02.f", <-.output_params.Hour,
                                               <-.output_params.minute,
                                               <-.output_params.Second);
         x => <-.UIlabelTime.width + 10;
         y => <-.UIlabelTime.y;
         active = 0;
      };




      UIlabel UIlabelBandNumberToRead {
         parent => <-.parent;
         label => "Band Num To Read";
         y => <-.Time.y + <-.Time.height + 15;
      };
      UIfield BandNumberToRead {
         parent => <-.parent;
         value => <-.input_params.BandNumberToRead;
         x => <-.UIlabelBandNumberToRead.width + 10;
         y => <-.UIlabelBandNumberToRead.y;
         min = 1.;
         decimalPoints = 0;
      };

      UIlabel UIlabelStartRow {
         parent => <-.parent;
         label => "Start Row";
         y => <-.BandNumberToRead.y + <-.BandNumberToRead.height + 2;
      };
      UIfield StartRow {
         parent => <-.parent;
         value => <-.input_params.StartRow;
         x => <-.UIlabelStartRow.width + 10;
         y => <-.UIlabelStartRow.y;
         min = 0.;
         decimalPoints = 0;
      };

      UIlabel UIlabelStartColumn {
         parent => <-.parent;
         label => "Start Column";
         y => <-.StartRow.y + <-.StartRow.height + 2;
      };
      UIfield StartColumn {
         parent => <-.parent;
         value => <-.input_params.StartColumn;
         x => <-.UIlabelStartColumn.width + 10;
         y => <-.UIlabelStartColumn.y;
         min = 0.;
         decimalPoints = 0;
      };

      UIlabel UIlabelNumberOfRows {
         parent => <-.parent;
         label => "Num. Output Rows";
         y => <-.StartColumn.y + <-.StartColumn.height + 2;
      };
      UIfield NumberOfRows {
         parent => <-.parent;
         value => <-.input_params.NumberOfRows;
         x => <-.UIlabelNumberOfRows.width + 10;
         y => <-.UIlabelNumberOfRows.y;
         min = 0.;
         decimalPoints = 0;
      };

      UIlabel UIlabelNumberOfColumns {
         parent => <-.parent;
         label => "Num. Output Cols.";
         y => <-.NumberOfRows.y + <-.NumberOfRows.height + 2;
      };
      UIfield NumberOfColumns {
         parent => <-.parent;
         value => <-.input_params.NumberOfColumns;
         x => <-.UIlabelNumberOfColumns.width + 10;
         y => <-.UIlabelNumberOfColumns.y;
         min = 0.;
         decimalPoints = 0;
      };

      UIlabel UIlabelXScaleFactor {
         parent => <-.parent;
         label => "X Scale Factor";
         y => <-.NumberOfColumns.y + <-.NumberOfColumns.height + 2;
      };
      UIfield XScaleFactor {
         parent => <-.parent;
         value => <-.input_params.XScaleFactor;
         x => <-.UIlabelXScaleFactor.width + 10;
         y => <-.UIlabelXScaleFactor.y;
         min = 0.;
         decimalPoints = 4;
      };

      UIlabel UIlabelYScaleFactor {
         parent => <-.parent;
         label => "Y Scale Factor";
         y => <-.XScaleFactor.y + <-.XScaleFactor.height + 2;
      };
      UIfield YScaleFactor {
         parent => <-.parent;
         value => <-.input_params.YScaleFactor;
         x => <-.UIlabelYScaleFactor.width + 10;
         y => <-.UIlabelYScaleFactor.y;
         min = 0.;
         decimalPoints = 4;
      };
   };


   //
   // Modified User interface macro for alternative Read Dted module
   //
   ReadSpotUI ReadSpotUIAlt {
      UIlabelReadSpotTitle {
         label => "Alternative Read SPOT";
      };
      UIlabelNumberOfRows {
         label => "Num. Input Rows";
      };
      UIlabelNumberOfColumns {
         label => "Num. Input Cols.";
      };
   };



   macro ReadSpotImageFunc {
      XP_COMM_PROJ.ReadSpot.ReadSpotMods.ReadSpotInputParams &input_params<NEportLevels={2,1}>;

      XP_COMM_PROJ.ReadSpot.ReadSpotMods.ReadSpotImage ReadSpotImage {
         input_params => <-.input_params;
      };


#ifdef VIZ_EXPRESS
      GDM.DataObject DataObject {
         in => <-.ReadSpotImage.Image;
         Obj {
            name => name_of(<-.<-.<-);
         };
      };
#else
      DefaultMinMax MinMax {
         input => <-.ReadSpotImage.Image;
      };
      GreyScale GreyScale {
         dataMin => <-.MinMax.min_value;
         dataMax => <-.MinMax.max_value;
      };

      DataObjectLite DataObjectLite {
         in => <-.ReadSpotImage.Image;
         Obj {
            dmap+IPort3 => <-.<-.GreyScale;
            name => name_of(<-.<-.<-);
         };
      };
#endif

      olink out_image => ReadSpotImage.Image;
#ifdef VIZ_EXPRESS
      olink out_obj => .DataObject.obj;
#else
      olink out_obj => DataObjectLite.obj;
#endif
   };



   ReadSpotImageFunc ReadSpotImageFuncAlt {
      -ReadSpotImage;

      XP_COMM_PROJ.ReadSpot.ReadSpotMods.ReadSpotImageAlt ReadSpotImageAlt {
         input_params => <-.input_params;
      };

#ifdef VIZ_EXPRESS
      DataObject {
         in => <-.ReadSpotImageAlt.Image;
      };
#else
      MinMax {
         input => <-.ReadSpotImageAlt.Image;
      };
      DataObjectLite {
         in => <-.ReadSpotImageAlt.Image;
      };
#endif

      out_image => ReadSpotImageAlt.Image;
   };



   macro read_spot {
      XP_COMM_PROJ.ReadSpot.ReadSpotMods.ReadSpotInputParams+OPort input_params {
         ActivateFlag = 1;
      };

      XP_COMM_PROJ.ReadSpot.ReadSpotMods.ReadSpotHeader ReadSpotHeader {
         input_params => <-.input_params;
      };

      XP_COMM_PROJ.ReadSpot.ReadSpotMacs.ReadSpotUI ReadSpotUI {
         input_params => <-.input_params;
         output_params => <-.ReadSpotHeader.output_params;
      };

      XP_COMM_PROJ.ReadSpot.ReadSpotMacs.ReadSpotImageFunc ReadSpotImageFunc {
         input_params => <-.input_params;
         out_obj {
            name => name_of(<-.<-.<-.<-);
         };
      };

      olink out_image => ReadSpotImageFunc.out_image;
      olink out_obj => ReadSpotImageFunc.out_obj;
   };



   read_spot read_spot_alt {
      -ReadSpotUI;
      -ReadSpotImageFunc;

      XP_COMM_PROJ.ReadSpot.ReadSpotMacs.ReadSpotUIAlt ReadSpotUIAlt {
         input_params => <-.input_params;
         output_params => <-.ReadSpotHeader.output_params;
      };

      XP_COMM_PROJ.ReadSpot.ReadSpotMacs.ReadSpotImageFuncAlt ReadSpotImageFuncAlt {
         input_params => <-.input_params;
         out_obj {
            name => name_of(<-.<-.<-.<-);
         };
      };

      out_image => ReadSpotImageFuncAlt.out_image;
      out_obj => ReadSpotImageFuncAlt.out_obj;
   };

};

