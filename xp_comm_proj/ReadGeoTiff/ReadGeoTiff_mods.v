
flibrary ReadGeoTiffMods <out_hdr_file="gen.h",
		          out_src_file="gen.cxx"> {

  // define the parameter block group
  group+OPort ReadGeoTiffParams  {
     string  Filename;
	 enum    InputMode {
                    choices = {"Row/Column", "Lat/Lon"};
                    values = {0, 1};
                  } = "Row/Column";
     int     ChipStartRow = 0;
     int     ChipStartColumn = 0;
     int     ChipNumberOfRows;
     int     ChipNumberOfColumns;
     double  ChipSouthLatitude;
     double  ChipWestLongitude;
     double  ChipNorthLatitude;
     double  ChipEastLongitude;
     double  FileSouthLatitude;
     double  FileWestLongitude;
     double  FileNorthLatitude;
     double  FileEastLongitude;
     double  FileLongitudeResolution;
     double  FileLatitudeResolution;
   };

  module readgeotiff< src_file="GISread_gtiff.cxx",
                  cxx_src_files="gsgtiff.cxx",
                  c_src_files="geo_tiffp.c xtiff.c",
		      hdr_dirs="libtiff",
		      cxx_hdr_files="gsgtiff.h fld/Xfld.h"
			>
 {
   ReadGeoTiffParams+IPort2 &Params;

   cxxmethod+req+notify_inst read( Filename+notify+read+req,
                                   InputMode+notify+read,
                                   ChipStartRow+notify+read,
                                   ChipStartColumn+notify+read,
                                   ChipNumberOfRows+notify+read,
                                   ChipNumberOfColumns+notify+read,
                                   ChipSouthLatitude+notify+read,
                                   ChipWestLongitude+notify+read,
                                   ChipNorthLatitude+notify+read,
                                   ChipEastLongitude+notify+read,
                                   FileSouthLatitude+write,
                                   FileWestLongitude+write,
                                   FileNorthLatitude+write,
                                   FileEastLongitude+write,
                                   FileLongitudeResolution+write,
                                   FileLatitudeResolution+write,
                                   OutField+write
                                  );


   string  Filename => Params.Filename;
   enum    InputMode {
                    choices = {"Row/Column", "Lat/Lon"};
                    values = {0, 1};
                  } => Params.InputMode;
   int     ChipStartRow => Params.ChipStartRow;
   int     ChipStartColumn => Params.ChipStartColumn;
   int     ChipNumberOfRows => Params.ChipNumberOfRows;
   int     ChipNumberOfColumns => Params.ChipNumberOfColumns;
   double  ChipSouthLatitude => Params.ChipSouthLatitude;
   double  ChipWestLongitude => Params.ChipWestLongitude;
   double  ChipNorthLatitude => Params.ChipNorthLatitude;
   double  ChipEastLongitude => Params.ChipEastLongitude;
   double  FileSouthLatitude => Params.FileSouthLatitude;
   double  FileWestLongitude => Params.FileWestLongitude;
   double  FileNorthLatitude => Params.FileNorthLatitude;
   double  FileEastLongitude => Params.FileEastLongitude;
   double  FileLongitudeResolution => Params.FileLongitudeResolution;
   double  FileLatitudeResolution => Params.FileLatitudeResolution;
   Mesh_Unif+Dim2+Space2+Node_Data+OPort2 OutField;
  };

};
