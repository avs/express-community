flibrary HiVisMods {
  group HiVis_Grid_Interp<NEportLevels={0,2},NEnumColors=2,NEcolor0=0xff0000,NEcolor1=0x000000>
    {
      string id<NEportLevels=1>;
    };
  
  group HiVis_Attr_Comb<NEportLevels={0,2},NEnumColors=2,NEcolor0=0x00ff00,NEcolor1=0x000000>
    {
      enum id<NEportLevels=1> {
	choices = { "AttrCombDefault", "AttrCombEpsilon" };
      };
      double epsilon;
      int attribute;
      enum epsCalculateBy {
	choices = { "scalar difference", "vector difference" };
      };
      enum epsApplyOn {
	choices = { "maximum difference", "average difference" };
      };
    };
  
  HiVis_Attr_Comb HiVis_Attr_Comb_Default
    {
      id = "AttrCombDefault";
    };

  HiVis_Attr_Comb HiVis_Attr_Comb_Epsilon
    {
      id = "AttrCombEpsilon";
    };

  module HiVis_Grid
    <NEnumColors=3,NEcolor0=0xffffb0,NEcolor1=0x000000,NEcolor2=0xffffb0,
    build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp",
    cxx_class="visGridLink",
    cxx_class_args="this",
    cxx_hdr_files="VisApi/VisApi.h",
    cxx_src_files="VisApi/VisApi.cpp",
    libdeps="GD">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .hasChanged+notify+read+req,
				    .dim+write,
				    .size+write,
				    .attributeCount+write,
				    .cells+write,
				    .nodes+write,
				    .attributeName+write,
				    .minScalarValue+write,
				    .maxScalarValue+write
				    );
      int hasChanged;
      //int type;
      int dim;
      int size[.dim];
      int attributeCount;
      int attributeDim[.attributeCount];
      int cells;
      int nodes;
      string attributeName[.attributeCount];
      double minScalarValue[.attributeCount];
      double maxScalarValue[.attributeCount];
      int attributeDataID[.attributeCount];
      DefaultXform xform;
    };
    
  // Conversion low-level modules
    
  module Field_To_HiVis
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in+notify+read+req,
				    .assoc+notify+read,
				    .out+write
				    );
      Mesh_Struct+Node_Data &in<export=2,NEportLevels={2,0}>;
      int &assoc<export=2,NEportLevels={2,0}>;
      HiVis_Grid out<export=2,NEportLevels={0,2}> {
	xform {
	  mat => <-.<-.in.xform.mat;
	  xlate => <-.<-.in.xform.xlate;
	  center => <-.<-.in.xform.center;
	};
      };
    };
    
  module HiVis_To_Field
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .adjust_Min_Max+notify+read+req,
				    .out+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      boolean &adjust_Min_Max<export=2,NEportLevels={2,0}>;
      Mesh_Unif+Node_Data out<export=2,NEportLevels={0,2}> {
	xform {
	  mat => <-.<-.in.xform.mat;
	  xlate => <-.<-.in.xform.xlate;
	  center => <-.<-.in.xform.center;
	};
      };
    };
    
  module AG_Cell2Node
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .gridInterp+notify+read+req,
				    .out+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      HiVis_Grid_Interp &gridInterp<export=2,NEportLevels={2,0}>;
      HiVis_Grid out<export=2,NEportLevels={0,2}> {
	attributeDataID => <-.in.attributeDataID;
	xform {
	  mat => <-.<-.in.xform.mat;
	  xlate => <-.<-.in.xform.xlate;
	  center => <-.<-.in.xform.center;
	};
      };
    };
    
  module HiVis_Read_Octree
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp", 
    cxx_class="visOctree">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .fileName+notify+read+req,
				    .out+write
				    .maxLevel+write
				    );
      string &fileName<export=2,NEportLevels={2,0}>;
      HiVis_Grid out<export=2,NEportLevels={0,2}>;
      int maxLevel<export=2,NEportLevels={0,2}>;
    };
    
  module HiVis_Read_Field
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .fileName+notify+read+req,
				    .out+write
				    .maxLevel+write
				    );
      string &fileName<export=2,NEportLevels={2,0}>;
      HiVis_Grid out<export=2,NEportLevels={0,2}>;
    };
    
  module AG_Octree
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp", 
    cxx_class="visOctree">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .level+notify+read,
				    .attr_Comb+notify+read,
				    .out+write
				    .maxLevel+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      int &level<export=2,NEportLevels={2,0}>;
      HiVis_Attr_Comb &attr_Comb<export=2,NEportLevels={2,0}>;
      HiVis_Grid out<export=2,NEportLevels={0,2}> {
	xform {
	  mat => <-.<-.in.xform.mat;
	  xlate => <-.<-.in.xform.xlate;
	  center => <-.<-.in.xform.center;
	};
	attributeDataID => <-.in.attributeDataID;
      };
      int maxLevel<export=2,NEportLevels={0,2}>;
    };
    

  module HiVis_Read_SparseGrid
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp", 
    cxx_class="visSparseGrid">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .fileName+notify+read+req,
				    .level+notify+read,
				    .attr_Comb+notify+read,
				    .maxLdScopeDiff+notify+read,
				    .out+write,
				    .maxLevel+write
				    );
      string &fileName<export=2,NEportLevels={2,0}>;
      int &level<export=2,NEportLevels={2,0}>;
      HiVis_Attr_Comb &attr_Comb<export=2,NEportLevels={2,0}>;
      int &maxLdScopeDiff<export=2,NEportLevels={2,0}>;
      HiVis_Grid out<export=2,NEportLevels={0,2}>;
      int maxLevel<export=2,NEportLevels={0,2}>;
    };
    
  module AG_SparseGrid
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp", 
    cxx_class="visSparseGrid">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .level+notify+read,
				    .attr_Comb+notify+read,
				    .maxLdScopeDiff+notify+read,
				    .out+write,
				    .maxLevel+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      int &level<export=2,NEportLevels={2,0}>;
      HiVis_Attr_Comb &attr_Comb<export=2,NEportLevels={2,0}>;
      int &maxLdScopeDiff<export=2,NEportLevels={2,0}>;
      HiVis_Grid out<export=2,NEportLevels={0,2}> {
	xform {
	  mat => <-.<-.in.xform.mat;
	  xlate => <-.<-.in.xform.xlate;
	  center => <-.<-.in.xform.center;
	};
	attributeDataID => <-.in.attributeDataID;
      };
      int maxLevel<export=2,NEportLevels={0,2}>;
    };
    
  // Render low-level modules

  module HiVis_Iso
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .attribute+notify+read+req,
				    .colorattribute+notify+read+req,
				    .isoval+notify+read+req,
				    .invertfaces+notify+read+req,
				    .out+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      int &attribute<export=2,NEportLevels={2,0}>;
      int &colorattribute<export=2,NEportLevels={2,0}>;
      double &isoval<export=2,NEportLevels={2,0}>;
      boolean &invertfaces<export=2,NEportLevels={2,0}>;
      Mesh+Node_Data out<export=2,NEportLevels={0,2}> {
	xform {
	  mat => <-.<-.in.xform.mat;
	  xlate => <-.<-.in.xform.xlate;
	  center => <-.<-.in.xform.center;
	};
      };
    };
    
  module HiVis_Render_Cells
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .attribute+notify+read+req,
				    .assoc+notify+read+req,
				    .ipad+notify+read+req,
				    .renderType+notify+read,
				    .rangeAttribute+notify+read,
				    .ranges+notify+read,
				    .rangeMin+notify+read,
				    .rangeMax+notify+read,
				    .cropBorder+notify+read,
				    .out+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      int &attribute<export=2,NEportLevels={2,0}>;
      int &assoc<export=2,NEportLevels={2,0}>;
      float &ipad<export=2,NEportLevels={2,0}>;
      int &renderType<export=2,NEportLevels={2,0}>;
      int &rangeAttribute<export=2,NEportLevels={2,0}>;
      int &ranges<export=2,NEportLevels={2,0}>;
      double &rangeMin<export=2,NEportLevels={2,0}>[20];
      double &rangeMax<export=2,NEportLevels={2,0}>[20];
      boolean &cropBorder<export=2,NEportLevels={2,0}>;
      Mesh+Node_Data out<export=2,NEportLevels={0,2}> {
	xform {
	  mat => <-.<-.in.xform.mat;
	  xlate => <-.<-.in.xform.xlate;
	  center => <-.<-.in.xform.center;
	};
      };
    };

  module HiVis_Render_SparseGrid
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .attribute+notify+read+req,
				    .ldScale+notify+read+req,
				    .renderType+notify+read,
				    .pointShape+notify+read,
				    .out+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      int &attribute<export=2,NEportLevels={2,0}>;
      int &ldScale<export=2,NEportLevels={2,0}>;
      int &renderType<export=2,NEportLevels={2,0}>;
      Mesh &pointShape<export=2,NEportLevels={2,0}>;
      Mesh+Node_Data out<export=2,NEportLevels={0,2}> {
	xform {
	  mat => <-.<-.in.xform.mat;
	  xlate => <-.<-.in.xform.xlate;
	  center => <-.<-.in.xform.center;
	};
      };
    };

  module HiVis_Bounds
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .attribute+notify+read+req,
				    .renderType+notify+read,
				    .cropBorder+notify+read,
				    .out+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      int &attribute<export=2,NEportLevels={2,0}>;
      int &renderType<export=2,NEportLevels={2,0}>;
      boolean &cropBorder<export=2,NEportLevels={2,0}>;
      Mesh+Node_Data out<export=2,NEportLevels={0,2}> {
	xform {
	  mat => <-.<-.in.xform.mat;
	  xlate => <-.<-.in.xform.xlate;
	  center => <-.<-.in.xform.center;
	};
      };
    };

  module HiVis_Render_Streamline
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .probe+notify+read+req,
				    .particle_Shape+notify+read,
				    .comp_Vector+notify+read+req,
				    .max_Steps+notify+read+req,
				    .order+notify+read+req,
				    .steps_Per_Cell+notify+read+req,
				    .stream_Go_Backward+notify+read+req,
				    .vis_Type+notify+read+req,
				    .scale_Ribbon+notify+read+req,
				    .scale_Object+notify+read+req,
				    .scale_Stripe+notify+read+req,
				    .comp_Stripe+notify+read+req,
				    .comp_Color+notify+read+req,
				    .comp_Scale+notify+read+req,
				    .comp_ScaleX+notify+read+req,
				    .ribbon_Inverse_Faces+notify+read+req,
				    .adjust_Min_Max+notify+read+req,
				    .manual_Update+notify+read,
				    .do_Update+notify+read,
				    .out+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      Grid+Xform &probe<export=2,NEportLevels={2,0}>;
      Mesh &particle_Shape<export=2,NEportLevels={2,0}>;
      int &comp_Vector<export=2,NEportLevels={2,0}>;
      int &max_Steps<export=2,NEportLevels={2,0}>;
      int &order<export=2,NEportLevels={2,0}>;
      int &steps_Per_Cell<export=2,NEportLevels={2,0}>;
      boolean &stream_Go_Backward<export=2,NEportLevels={2,0}>;
      int &vis_Type<export=2,NEportLevels={2,0}>;
      float &scale_Ribbon<export=2,NEportLevels={2,0}>;
      float &scale_Object<export=2,NEportLevels={2,0}>;
      float &scale_Stripe<export=2,NEportLevels={2,0}>;
      int &comp_Stripe<export=2,NEportLevels={2,0}>;
      int &comp_Color<export=2,NEportLevels={2,0}>;
      int &comp_Scale<export=2,NEportLevels={2,0}>;
      int &comp_ScaleX<export=2,NEportLevels={2,0}>;
      boolean &ribbon_Inverse_Faces<export=2,NEportLevels={2,0}>;
      boolean &adjust_Min_Max<export=2,NEportLevels={2,0}>;
      boolean &manual_Update<export=2,NEportLevels={2,0}>;
      int &do_Update<export=2,NEportLevels={2,0}>;
      Mesh+Node_Data out<export=2,NEportLevels={0,2}> {
	xform {
	  xlate => <-.<-.in.xform.xlate;
	  center => <-.<-.in.xform.center;
	};
      };
    };

  module HiVis_Render_Field
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .ipad+notify+read,
				    .attribute+notify+read+req,
				    .renderType+notify+read,
				    .rangeAttribute+notify+read,
				    .ranges+notify+read,
				    .rangeMin+notify+read,
				    .rangeMax+notify+read,
				    .out+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      int &attribute<export=2,NEportLevels={2,0}>;
      float &ipad<export=2,NEportLevels={2,0}>;
      int &renderType<export=2,NEportLevels={2,0}>;
      int &rangeAttribute<export=2,NEportLevels={2,0}>;
      int &ranges<export=2,NEportLevels={2,0}>;
      double &rangeMin<export=2,NEportLevels={2,0}>[20];
      double &rangeMax<export=2,NEportLevels={2,0}>[20];
      Mesh+Node_Data out<export=2,NEportLevels={0,2}> {
	xform {
	  mat => <-.<-.in.xform.mat;
	  xlate => <-.<-.in.xform.xlate;
	  center => <-.<-.in.xform.center;
	};
      };
    };

  module AG_OrthoSlice
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp", 
    cxx_class="visAdaptiveGridSlice">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .axis+notify+read+req,
				    .pos+notify+read+req,
				    .adjust_Min_Max+notify+read+req,
				    .out+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      int &axis<export=2,NEportLevels={2,0}>;
      int &pos<export=2,NEportLevels={2,0}>;
      boolean &adjust_Min_Max<export=2,NEportLevels={2,0}>;
      HiVis_Grid out<export=2,NEportLevels={0,2}> {
	attributeDataID => <-.in.attributeDataID;
      };
    };

  module HiVis_Debug
    <build_dir="xp_comm_proj/HiVis",
    src_file="VisApi/AVS/modules.cpp">
    {
      cxxmethod+notify_inst+req upd<status=1>(
				    .in.hasChanged+notify+read+req,
				    .probe_Attribute+notify+read+req,
				    .probe_Pos+notify+read+req,
				    .cell_No_Overlap+write,
				    .cell_Complete+write,
				    .field_Boundary_Ok+write,
				    .field_NumberOfCells_Ok+write
				    );
      HiVis_Grid &in<export=2,NEportLevels={2,0}>;
      int &probe_Attribute<export=2,NEportLevels={2,0}>;
      int &probe_Pos<export=2,NEportLevels={2,0}>[.in.dim];
      boolean cell_No_Overlap<export=2,NEportLevels={0,2}>;
      boolean cell_Complete<export=2,NEportLevels={0,2}>;
      boolean good_NumberOfCells<export=2,NEportLevels={0,2}>;
    };
};
