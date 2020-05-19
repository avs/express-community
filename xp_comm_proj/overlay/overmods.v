flibrary OverlayMods <build_dir="xp_comm_proj/overlay",
                       libdeps="GD FLD",
                       cxx_hdr_files="fld/Xfld.h",
                       out_hdr_file="gen.h",
                       out_src_file="gen.cxx"> {

  group OverlayParams{
    byte maskA<NEportLevels={2,2}>;
    byte maskR<NEportLevels={2,2}>;
    byte maskG<NEportLevels={2,2}>;
    byte maskB<NEportLevels={2,2}>;
    byte bkgndA<NEportLevels={2,2}>;
    byte bkgndR<NEportLevels={2,2}>;
    byte bkgndG<NEportLevels={2,2}>;
    byte bkgndB<NEportLevels={2,2}>;
  };
    
  module OverlayNodeData<src_file="over_img.cxx"> {
    Mesh_Unif+Node_Data &image1<NEportLevels={2,0}>;
    Mesh_Unif+Node_Data &image2<NEportLevels={2,0}>;
    OverlayParams &OverlayParams<NEportLevels={2,0}>;
    byte mask[4] => {OverlayParams.maskA,
                     OverlayParams.maskR,
                     OverlayParams.maskG,
                     OverlayParams.maskB};
    byte bkgnd[4] => {OverlayParams.bkgndA,
                      OverlayParams.bkgndR,
                      OverlayParams.bkgndG,
                      OverlayParams.bkgndB};
    Mesh_Unif+Node_Data compos<NEportLevels={0,2}>;

    cxxmethod+notify_inst+req overlay(
      mask+notify+read+req,
      bkgnd+notify+read+req,
      image1+notify+read+req,
      image2+notify+read+req,
      compos+write);
  };

};
