
flibrary ReadDbaseMods <build_dir="xp_comm_proj/rd_dbase"> {

   module ReadDBFDate < src_file="dbfrddat.cxx",
                        cxx_src_files="dbf.cxx 
                                       dbfhdr.cxx
                                       dbffldds.cxx
                                       gsbyteu.cxx",
                        out_src_file="dat_gen.cxx",
                        out_hdr_file="dat_gen.h" > {

      string+IPort2 DBFFileName;
      string+IPort2 ColumnName;
      int+IPort2    ColumnNumber;

      cxxmethod+notify_inst+req ReadDBFDate (
         .DBFFileName+read+notify+req,
         .ColumnName+read+notify,
         .ColumnNumber+read+notify,
         .ColumnValues+write
      );

      int NumberOfValues;
      string+OPort2 ColumnValues[NumberOfValues];
   };


   module ReadDBFFloat < src_file="dbfrdflt.cxx",
                         cxx_src_files="dbf.cxx 
                                        dbfhdr.cxx
                                        dbffldds.cxx
                                        gsbyteu.cxx",
                         out_src_file="flt_gen.cxx",
                         out_hdr_file="flt_gen.h" > {

      string+IPort2 DBFFileName;
      string+IPort2 ColumnName;
      int+IPort2    ColumnNumber;

      cxxmethod+notify_inst+req ReadDBFFloat(
         .DBFFileName+read+notify+req,
         .ColumnName+read+notify,
         .ColumnNumber+read+notify,
         .ColumnValues+write
      );

      int NumberOfValues;
      float+OPort2 ColumnValues[NumberOfValues];
   };


   module ReadDBFInteger < src_file="dbfrdint.cxx",
                           cxx_src_files="dbf.cxx 
                                          dbfhdr.cxx
                                          dbffldds.cxx
                                          gsbyteu.cxx",
                           out_src_file="int_gen.cxx",
                           out_hdr_file="int_gen.h" > {

      string+IPort2 DBFFileName;
      string+IPort2 ColumnName;
      int+IPort2    ColumnNumber;

      cxxmethod+notify_inst+req ReadDBFInteger (
         .DBFFileName+read+notify+req,
         .ColumnName+read+notify,
         .ColumnNumber+read+notify,
         .ColumnValues+write
      );

      int NumberOfValues;
      int+OPort2 ColumnValues[NumberOfValues];
   };


   module ReadDBFLogical < src_file="dbfrdlog.cxx",
                           cxx_src_files="dbf.cxx 
                                          dbfhdr.cxx
                                          dbffldds.cxx
                                          gsbyteu.cxx",
                           out_src_file="log_gen.cxx",
                           out_hdr_file="log_gen.h" > {

      string+IPort2 DBFFileName;
      string+IPort2 ColumnName;
      int+IPort2    ColumnNumber;

      cxxmethod+notify_inst+req ReadDBFLogical (
         .DBFFileName+read+notify+req,
         .ColumnName+read+notify,
         .ColumnNumber+read+notify,
         .ColumnValues+write
      );

      int NumberOfValues;
      char+OPort2 ColumnValues[NumberOfValues];
   };


   module ReadDBFString < src_file="dbfrdstr.cxx",
                          cxx_src_files="dbf.cxx 
                                         dbfhdr.cxx
                                         dbffldds.cxx
                                         gsbyteu.cxx",
                          out_src_file="str_gen.cxx",
                          out_hdr_file="str_gen.h" > {

      string+IPort2 DBFFileName;
      string+IPort2 ColumnName;
      int+IPort2    ColumnNumber;

      cxxmethod+notify_inst+req ReadDBFString (
         .DBFFileName+read+notify+req,
         .ColumnName+read+notify,
         .ColumnNumber+read+notify,
         .ColumnValues+write
      );

      int NumberOfValues;
      string+OPort2 ColumnValues[NumberOfValues];
   };


   module ReadDBFType < src_file="dbfrdtyp.cxx",
                        cxx_src_files="dbf.cxx 
                                       dbfhdr.cxx
                                       dbffldds.cxx
                                       gsbyteu.cxx",
                        out_src_file="typ_gen.cxx",
                        out_hdr_file="typ_gen.h" > {

      string+IPort2 DBFFileName;
      string+IPort2 ColumnName;
      int+IPort2    ColumnNumber;

      cxxmethod+notify_inst+req ReadDBFType (
         .DBFFileName+read+notify+req,
         .ColumnName+read+notify,
         .ColumnNumber+read+notify,
         .ColumnType+write
      );

      int+OPort2 ColumnType;
   };

};

