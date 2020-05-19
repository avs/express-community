
flibrary PrintFieldMods <build_dir="xp_comm_proj/printfld",
		         out_hdr_file="gen.h",
		         out_src_file="gen.cxx"> {

  // define the parameter block group

  group PrintFieldParams <NEportLevels={0,1}> {
    string filename <NEportLevels={1,2}>;
    int gridout <NEportLevels={1,2}>;
    int cellout <NEportLevels={1,2}>;
    int xformout <NEportLevels={1,2}>;
    int nodesout <NEportLevels={1,2}>;
    int htmlout <NEportLevels={1,2}>;
    int allout <NEportLevels={1,2}>;
    int trigger <NEportLevels={1,2}>;
    string message <NEportLevels={1,2}>;
    string notes <NEportLevels={1,2}>;
  };

  // define the low-level module

  module PrintField< src_file="printfld.c"> {
    PrintFieldParams &PrintFieldParams <NEportLevels={2,1}>;

    string filename <NEportLevels={1,1}> => PrintFieldParams.filename;
    int gridout <NEportLevels={1,1}> => PrintFieldParams.gridout;
    int cellout <NEportLevels={1,1}> => PrintFieldParams.cellout;
    int xformout <NEportLevels={1,1}> => PrintFieldParams.xformout;
    int nodesout <NEportLevels={1,1}> => PrintFieldParams.nodesout;
    int htmlout <NEportLevels={1,1}> => PrintFieldParams.htmlout;
    int allout <NEportLevels={1,1}> => PrintFieldParams.allout;
    int trigger <NEportLevels={1,1}> => PrintFieldParams.trigger;
    string message <NEportLevels={1,1}> => PrintFieldParams.message;
    string notes <NEportLevels={1,1}> => PrintFieldParams.notes;

    Mesh+Node_Data &in <NEportLevels={2,0}>;

    omethod+req print_fld(
      filename+read+notify+req,
      gridout+read+notify+req,
      cellout+read+notify+req,
      xformout+read+notify+req,
      nodesout+read+notify+req,
      htmlout+read+notify+req,
      allout+read+notify+req,
      trigger+read+notify+req,
      message+write,
      notes+write,
      in+read+notify+req ) = "print_field";

  };
};
