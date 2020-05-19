
flibrary ReadSpotMods <build_dir="xp_comm_proj/rd_spot",
                       out_src_file="gen.cxx",
                       out_hdr_file="gen.hxx">
{

   group+OPort ReadSpotInputParams {
      string+IPort2 HeaderFileName;
      string+IPort2 ImageFileName;

      int+IPort2   BandNumberToRead;
      int+IPort2   StartRow;
      int+IPort2   StartColumn;
      int+IPort2   NumberOfRows;
      int+IPort2   NumberOfColumns;
      float+IPort2 XScaleFactor;
      float+IPort2 YScaleFactor;
      int+IPort2   ActivateFlag;
   };

   group ReadSpotOutputParams {
      string+OPort2 ProcessingLevel = "unknown";
      int+OPort2    NumberOfBands = 0;
      int+OPort2    NumberOfRows = 0;
      int+OPort2    NumberOfColumns = 0;
      int+OPort2    Year = 0;
      int+OPort2    Month = 0;
      int+OPort2    Day = 0;
      int+OPort2    Hour = 0;
      int+OPort2    Minute = 0;
      float+OPort2  Second = 0;
   };



   module ReadSpotHeader <src_file="rspt_hdr.cxx",
                          cxx_src_files="gsspot.cxx">
   {
      ReadSpotInputParams+IPort2 &input_params;

      cxxmethod+notify_inst+req update(
         .input_params+read+notify+req,
         .input_params.HeaderFileName+read+notify+req,
         .output_params+write,
         .output_params.ProcessingLevel+write,
         .output_params.NumberOfBands+write,
         .output_params.NumberOfRows+write,
         .output_params.NumberOfColumns+write,
         .output_params.Year+write,
         .output_params.Month+write,
         .output_params.Day+write,
         .output_params.Hour+write,
         .output_params.Minute+write,
         .output_params.Second+write
      );

      ReadSpotOutputParams+OPort2 output_params;
   };



   module ReadSpotImage <src_file="rspt_img.cxx",
                         cxx_src_files="gsspot.cxx",
                         cxx_hdr_files="fld/Xfld.h">
   {
      ReadSpotInputParams+IPort2 &input_params;

      int NumberOfOutputRows => input_params.NumberOfRows;
      int NumberOfOutputColumns => input_params.NumberOfColumns;

      cxxmethod+notify_inst+req update(
         .input_params+read+notify,
         .input_params.HeaderFileName+read+notify+req,
         .input_params.ImageFileName+read+notify+req,
         .input_params.BandNumberToRead+read+notify,
         .input_params.StartRow+read+notify,
         .input_params.StartColumn+read+notify,
         .NumberOfOutputRows+read+notify,
         .NumberOfOutputColumns+read+notify,
         .input_params.XScaleFactor+read+notify,
         .input_params.YScaleFactor+read+notify,
         .input_params.ActivateFlag+read+notify,
         .Image+write
      );

      Field_Unif+Dim2+Byte+Space2+Node_Data+OPort2 Image;
   };


   //
   // Alternative ReadSpotImage module.
   // Modified from standard ReadSpotImage module by using V statements to
   // convert input dimensions into output dimensions.
   //
   ReadSpotImage ReadSpotImageAlt {
      NumberOfOutputRows => input_params.NumberOfRows * input_params.YScaleFactor;
      NumberOfOutputColumns => input_params.NumberOfColumns * input_params.XScaleFactor;
   };

};

