library DICOMOrthoMods<build_dir="xp_comm_proj/dcmortho",out_src_file="gen.cxx",out_hdr_file="gen.h", cxx_hdr_files="fld/Xfld.h"> {
   // MPR orthoslice presumes the patient is orientated int he dataset in the following manor
   // z 0:N slices 0 being the feet end and facing in +ve y
   // slices from axial(xy) and coronal (xz) planes will be swapped left to right
   // this is so the patient Left appears on the users Right
   module DICOM_orthoslice_unif<src_file="dcmounif.cxx"> {
	Mesh_Unif+Node_Data+Iparam &in;
	int+Iparam axis;
	int+Iparam plane;
	Mesh_Unif+Node_Data+Oparam out;
        cxxmethod+notify_inst orthoslice (
	   in+read+req+notify,
	   axis+read+req+notify,
	   plane+read+req+notify,
	   out+write
	);	 
      }; 

};
