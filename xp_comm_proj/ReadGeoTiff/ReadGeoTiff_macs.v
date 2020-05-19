
flibrary ReadGeoTiffMacros{

  //ReadGeoTiffUI
  macro ReadGeoTiffUI{

      XP_COMM_PROJ.ReadGeoTiff.ReadGeoTiffMods.ReadGeoTiffParams &Params <NEportLevels={2,1}>;

   	  UI.UImod_panel UImod_panel{
	    title = "ReadGeoTiff";
	  };
      UIbutton BrowseButton {
         parent => UImod_panel;
         label => "Browse...";
         y = 0;
         width => parent.clientWidth;
      };
      UIfileDialog FileDialog {
         searchPattern = "*.tif";
         filename => <-.Params.Filename;
         visible => <-.BrowseButton.do;
      };
      UItext FilenameText {
         parent => <-.UImod_panel;
         y => ((BrowseButton.y + BrowseButton.height) + 3);
         width => parent.clientWidth;
         text => FileDialog.filename;
      };
      UIoption RowColOption {
         label => "Row/Column";
      };
      UIoption LatLonOption {
         label => "Lat/Lon";
      };
      UIoptionMenu InputModeOptionMenu {
         cmdList => {RowColOption,
            LatLonOption};
         selectedItem => <-.Params.InputMode;
         parent => <-.UImod_panel;
         label => "InputMode:";
         x = 0;
         y => ((FilenameText.y + FilenameText.height) + 10);
         width => parent.clientWidth;
      };
      UIframe ChipRowColFrame {
         parent => <-.UImod_panel;
         y => ((InputModeOptionMenu.y + InputModeOptionMenu.height) + 10);
         width => parent.clientWidth;
         height => ((ChipRowNumField.y + ChipRowNumField.height) + 5);
      };
      UIlabel ChipRowStartLabel {
         parent => <-.ChipRowColFrame;
         label => "Chip Start Row:";
         y = 0;
         width => (parent.clientWidth / 2);
         x => (parent.clientWidth / 4);
         alignment = "left";
      };
      UIfield ChipRowStartField {
         parent => <-.ChipRowColFrame;
         value => <-.Params.ChipStartRow;
         y => (ChipRowStartLabel.y + ChipRowStartLabel.height);
         x => (parent.clientWidth / 4);
         width => (parent.clientWidth / 2);
      };
      UIlabel ChipColStartLabel {
         parent => <-.ChipRowColFrame;
         label => "Chip Column Start:";
         y => ((ChipRowStartField.y + ChipRowStartField.height) + 5);
         width => (parent.clientWidth * 0.45);
         alignment = "left";
      };
      UIfield ChipColStartField {
         parent => <-.ChipRowColFrame;
         value => <-.Params.ChipStartColumn;
         y => (ChipColStartLabel.y + ChipColStartLabel.height);
         width => (parent.clientWidth * 0.45);
      };
      UIlabel ChipColNumLabel {
         parent => <-.ChipRowColFrame;
         label => "Chip Columns:";
         y => ChipColStartLabel.y;
         x => (parent.clientWidth / 2);
         width => ChipColStartLabel.width;
         alignment = "left";
      };
      UIfield ChipColNumField {
         parent => <-.ChipRowColFrame;
         value => <-.Params.ChipNumberOfColumns;
         y => ChipColStartField.y;
         width => ChipColStartField.width;
         x => ChipColNumLabel.x;
      };
      UIlabel ChipRowNumLabel {
         parent => <-.ChipRowColFrame;
         label => "Chip Rows:";
         y => ((ChipColStartField.y + ChipColStartField.height) + 5);
         x => ChipRowStartLabel.x;
         width => ChipRowStartLabel.width;
         alignment = "left";
      };
      UIfield ChipRowNumField {
         parent => <-.ChipRowColFrame;
         value => <-.Params.ChipNumberOfRows;
         y => (ChipRowNumLabel.y + ChipRowNumLabel.height);
         width => ChipRowStartField.width;
         x => ChipRowNumLabel.x;
      };
      UIframe ChipLocationFrame {
         parent => <-.UImod_panel;
         y => ((ChipRowColFrame.y + ChipRowColFrame.height) + 10);
         width => parent.clientWidth;
         height => ((ChipSouthLatField.y + ChipSouthLatField.height) + 5);
      };
      UIlabel ChipNorthLatLabel {
         parent => <-.ChipLocationFrame;
         label => "Chip North Lat:";
         y = 0;
         width => (parent.clientWidth / 2);
         x => (parent.clientWidth / 4);
         alignment = "left";
      };
      UIfield ChipNorthLatField {
         parent => <-.ChipLocationFrame;
         value => <-.Params.ChipNorthLatitude;
         y => (ChipNorthLatLabel.y + ChipNorthLatLabel.height);
         x => (parent.clientWidth / 4);
         width => (parent.clientWidth / 2);
      };
      UIlabel ChipWestLonLabel {
         parent => <-.ChipLocationFrame;
         label => "Chip West Lon:";
         y => ((ChipNorthLatField.y + ChipNorthLatField.height) + 5);
         width => (parent.clientWidth * 0.45);
         alignment = "left";
      };
      UIfield ChipWestLonField {
         parent => <-.ChipLocationFrame;
         value => <-.Params.ChipWestLongitude;
         y => (ChipWestLonLabel.y + ChipWestLonLabel.height);
         width => (parent.clientWidth * 0.45);
      };
      UIlabel ChipEastLonLabel {
         parent => <-.ChipLocationFrame;
         label => "Chip EastLon:";
         y => ChipWestLonLabel.y;
         x => (parent.clientWidth / 2);
         width => ChipWestLonLabel.width;
         alignment = "left";
      };
      UIfield ChipEastLonField {
         parent => <-.ChipLocationFrame;
         value => <-.Params.ChipEastLongitude;
         y => ChipWestLonField.y;
         width => ChipWestLonField.width;
         x => ChipEastLonLabel.x;
      };
      UIlabel ChipSouthLatLabel {
         parent => <-.ChipLocationFrame;
         label => "Chip South Lat:";
         y => ((ChipWestLonField.y + ChipWestLonField.height) + 5);
         x => ChipNorthLatLabel.x;
         width => ChipNorthLatLabel.width;
         alignment = "left";
      };
      UIfield ChipSouthLatField {
         parent => <-.ChipLocationFrame;
         value => <-.Params.ChipSouthLatitude;
         y => (ChipSouthLatLabel.y + ChipSouthLatLabel.height);
         width => ChipNorthLatField.width;
         x => ChipSouthLatLabel.x;
      };
      UIframe FileLocationFrame {
         parent => <-.UImod_panel;
         y => ((ChipLocationFrame.y + ChipLocationFrame.height) + 10);
         width => parent.clientWidth;
         height => ((FileSouthLatField.y + FileSouthLatField.height) + 5);
      };
      UIlabel FileNorthLatLabel {
         parent => <-.FileLocationFrame;
         label => "File North Lat:";
         y = 0;
         width => (parent.clientWidth / 2);
         x => (parent.clientWidth / 4);
         alignment = "left";
      };
      UIfield FileNorthLatField {
         parent => <-.FileLocationFrame;
         value => <-.Params.FileNorthLatitude;
         y => (FileNorthLatLabel.y + FileNorthLatLabel.height);
         x => (parent.clientWidth / 4);
         width => (parent.clientWidth / 2);
         outputOnly = 1;
      };
      UIlabel FileWestLonLabel {
         parent => <-.FileLocationFrame;
         label => "File West Lon:";
         y => ((FileNorthLatField.y + FileNorthLatField.height) + 5);
         width => (parent.clientWidth * 0.45);
         alignment = "left";
      };
      UIfield FileWestLonField {
         parent => <-.FileLocationFrame;
         value => <-.Params.FileWestLongitude;
         y => (FileWestLonLabel.y + FileWestLonLabel.height);
         width => (parent.clientWidth * 0.45);
         outputOnly = 1;
      };
      UIlabel FileEastLonLabel {
         parent => <-.FileLocationFrame;
         label => "File East Lon:";
         y => FileWestLonLabel.y;
         x => (parent.clientWidth / 2);
         width => FileWestLonLabel.width;
         alignment = "left";
      };
      UIfield FileEastLonField {
         parent => <-.FileLocationFrame;
         value => <-.Params.FileEastLongitude;
         y => FileWestLonField.y;
         width => FileWestLonField.width;
         x => FileEastLonLabel.x;
         outputOnly = 1;
      };
      UIlabel FileSouthLatLabel {
         parent => <-.FileLocationFrame;
         label => "File South Lat:";
         y => ((FileWestLonField.y + FileWestLonField.height) + 5);
         x => FileNorthLatLabel.x;
         width => FileNorthLatLabel.width;
         alignment = "left";
      };
      UIfield FileSouthLatField {
         parent => <-.FileLocationFrame;
         value => <-.Params.FileSouthLatitude;
         y => (FileSouthLatLabel.y + FileSouthLatLabel.height);
         width => FileNorthLatField.width;
         x => FileSouthLatLabel.x;
         outputOnly = 1;
      };
      UIframe FileResolutionFrame {
         parent => <-.UImod_panel;
         y => FileLocationFrame.y + FileLocationFrame.height + 10;
         width => parent.clientWidth;
         height => ((FileLonResField.y + FileLonResField.height) + 5);
      };
      UIlabel FileLatResLabel {
         parent => <-.FileResolutionFrame;
         label => "File LatitudeResolution:";
         y = 0;
         width => parent.clientWidth;
         alignment = "left";
      };
      UIfield FileLatResField {
         parent => <-.FileResolutionFrame;
         value => <-.Params.FileLatitudeResolution;
         y => ((FileLatResLabel.y + FileLatResLabel.height) + 5);
         width => parent.clientWidth;
         decimalPoints = 6;
         outputOnly = 1;
      };
      UIlabel FileLonResLabel {
         parent => <-.FileResolutionFrame;
         label => "File Longitude Resoution:";
         y => ((FileLatResField.y + FileLatResField.height) + 5);
         width => parent.clientWidth;
         alignment = "left";
      };
      UIfield FileLonResField {
         parent => <-.FileResolutionFrame;
         value => <-.Params.FileLongitudeResolution;
         y => ((FileLonResLabel.y + FileLonResLabel.height) + 5);
         width => parent.clientWidth;
         decimalPoints = 6;
         outputOnly = 1;
      };
  };


 //ReadGeoTiffFunc
 // Funcitonal macro
 macro ReadGeoTiffFunc{
    XP_COMM_PROJ.ReadGeoTiff.ReadGeoTiffMods.ReadGeoTiffParams &ReadGeoTiffParams <NEportLevels={2,1}>;

	XP_COMM_PROJ.ReadGeoTiff.ReadGeoTiffMods.readgeotiff readgeotiff{
      Params =><-.ReadGeoTiffParams;
    };

	olink out => readgeotiff.OutField;
 };

 //ReadGeoTiff
 // User Macro 
 macro ReadGeoTiff{

   XP_COMM_PROJ.ReadGeoTiff.ReadGeoTiffMods.ReadGeoTiffParams ReadGeoTiffParams;

   XP_COMM_PROJ.ReadGeoTiff.ReadGeoTiffMacs.ReadGeoTiffFunc ReadGeoTiffFunc{
     &ReadGeoTiffParams => <-.ReadGeoTiffParams;
   };

   XP_COMM_PROJ.ReadGeoTiff.ReadGeoTiffMacs.ReadGeoTiffUI ReadGeoTiffUI{
      &Params => <-.ReadGeoTiffParams;
      UImod_panel{
	    title => name_of(<-.<-.<-);
		};
   };

   olink out => ReadGeoTiffFunc.out;
	
   GDM.DataObject DataObject {
      in => <-.ReadGeoTiffFunc.out;
   };
   olink obj => DataObject.obj;

 };

};
