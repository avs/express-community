
library ReadDICOMMods<build_dir="xp_comm_proj/rd_dicom", cxx_hdr_files="fld/Xfld.h", out_src_file="gen.cxx", out_hdr_file="gen.h",
   hdr_dirs="$(DCMTK_ROOT)/include",
#ifdef MSDOS
      link_files="netapi32.lib wsock32.lib $(DCMTK_ROOT)/lib/ofstd.lib $(DCMTK_ROOT)/lib/dcmdata.lib $(DCMTK_ROOT)/lib/dcmimgle.lib $(DCMTK_ROOT)/lib/dcmimage.lib"
#else
      link_files="-L$(DCMTK_ROOT)/lib -ldcmimage -ldcmimgle -ldcmdata -loflog -lofstd -lrt -lpthread -lnsl"
#endif
> {
   // read_DCMLIST reads/creates/modifies a list of DICOM series'
   // present in the a the local directory
   module read_DCMLIST <cxx_hdr_files="read_DCMLIST.h", src_file="read_DCMLIST.cxx",
      cxx_members="
      int read_DCMLIST(int &npats, int &_nseries, int &_nfiles, dicom_pat_info **pats, dicom_file_info **dfiles);
      int rescan_dir(int &npats, int &_nseries, int &_nfiles, dicom_pat_info **pats, dicom_file_info **dfiles);
      int write_DCMLIST(int npats, int _nseries, int _nfiles, dicom_pat_info *pats, dicom_file_info *dfiles);
      int output_infos(int npats, int _nseries, int _nfiles, dicom_pat_info *pats, dicom_file_info *dfiles);
      ">
   {
      string dirname<NEportLevels={2,1}>;
      int rescan<NEportLevels={2,0}>;
      int delete<NEportLevels={2,0}>;
      int nfiles<NEportLevels={0,2}>;
      string filenames<NEportLevels={0,2}>[nfiles];
      int nseries<NEportLevels={0,2}>;
      string series<NEportLevels={0,2}>[nseries];
      int series_st_en<NEportLevels={0,2}>[nseries][2];
      int selected_series<NEportLevels={2,0}>;
      cxxmethod+notify_inst+req read<status=1> (
	 dirname+read+req+notify,
	 rescan+read+notify,
	 nfiles+write,
	 filenames+write,
	 nseries+write,
	 series+write,
	 series_st_en+write
      );
      cxxmethod+req remove (
	 delete+read+req+notify,
	 dirname+read+req,
	 selected_series+read+req,
	 nfiles+write,
	 filenames+write,
	 nseries+write,
	 series+write,
	 series_st_en+write
      );
   };

   // read_dicom_files reads in the given array of DICOM filenames
   // reconstructing a volume of data. The image are now transposed correctly
   module read_dicom_files <src_file="read_dicom_files.cxx", cxx_hdr_files="dcmtk/config/osconfig.h dcmtk/dcmdata/dctk.h",
      cxx_members="
      void read_PatientInfo(DcmDataset *dataset);
      void read_GenStudyInfo(DcmDataset *dataset);
      void read_GenSeriesInfo(DcmDataset *dataset);
      void read_GenEquipInfo(DcmDataset *dataset);
      void read_ImageSeries(DcmDataset *dataset);
      ">
   {
      // Input: DICOM filename
      string filenames<NEportLevels={2,0}>[];
      int trigger<NEportLevels={2,0}>;
      int flip<NEportLevels={2,0}>;
      // Outputs: Currently busy, header information & image data
      int busy<NEportLevels={0,2}> = 0;
      int done<NEportLevels={0,2}> = 0;
      XP_COMM_PROJ.ReadDICOM.ReadDICOMData.DICOM_Header header<NEportLevels={0,2}>;
      XP_COMM_PROJ.ReadDICOM.ReadDICOMData.DICOM_Volume volume<NEportLevels={0,2}>;
      cxxmethod+req read<status=1> (
	 filenames+read+req,
	 trigger+notify,
	 flip+read,
	 busy+write+nonotify,
	 done+write+nonotify,
	 header+write+nonotify,
	 volume+write+nonotify
      );
   };
   
};
