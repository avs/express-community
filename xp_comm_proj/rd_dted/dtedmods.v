
flibrary ReadDtedMods <build_dir="xp_comm_proj/rd_dted",
                       out_src_file="gen.cxx",
                       out_hdr_file="gen.hxx">
{

   //
   // Parameter block used by both standard and alternative modules
   //
   group+OPort ReadDtedParams {
      string+Port2 Filename;
      int+Port2    StartRow;
      int+Port2    StartColumn;
      int+Port2    NumberOfRows;
      int+Port2    NumberOfColumns;
      float+Port2  XScaleFactor;
      float+Port2  YScaleFactor;
      int+Port2    ActivateFlag;
   };


   //
   // Standard Read Dted module.  Contains all references to program
   // code used by this project.
   //
   module ReadDtedCore <src_file="rd_dted.cxx",
                        cxx_src_files="gsdted.cxx
                                       gs252.cxx
                                       gsacc.cxx
                                       gsdsi.cxx
                                       gsuhl.cxx
                                       gsbyteu.cxx",
                        cxx_hdr_files="fld/Xfld.h",
                        libdeps="FLD">
   {
      ReadDtedParams+IPort2 &params;

      string FileName => params.Filename;
      int    StartRow => params.StartRow;
      int    StartColumn => params.StartColumn;
      int    NumberOfOutputRows => params.NumberOfRows;
      int    NumberOfOutputColumns => params.NumberOfColumns;
      float  XScaleFactor => params.XScaleFactor;
      float  YScaleFactor => params.YScaleFactor;
      int    ActivateFlag => params.ActivateFlag;

      cxxmethod+notify_inst+req update(
         .FileName+read+notify+req,
         .StartRow+read+notify,
         .StartColumn+read+notify,
         .NumberOfOutputRows+read+notify,
         .NumberOfOutputColumns+read+notify,
         .XScaleFactor+read+notify,
         .YScaleFactor+read+notify,
         .ActivateFlag+read+notify,
         .Data+write
      );

      Field_Unif+Dim2+Short+Space2+Node_Data+OPort2 Data;
   };


   //
   // Alternative Read Dted module.
   // Modified from standard Read Dted module by used V statements to
   // convert input dimensions into output dimensions.
   //
   ReadDtedCore ReadDtedCoreAlt {
      NumberOfOutputRows => params.NumberOfRows * params.YScaleFactor;
      NumberOfOutputColumns => params.NumberOfColumns * params.XScaleFactor;
   };


};

