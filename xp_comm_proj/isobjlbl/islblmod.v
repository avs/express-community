
flibrary IsoObjectLabelMods <build_dir="xp_comm_proj/isobjlbl",
                        out_src_file="gen.cxx",
                        out_hdr_file="gen.hxx"> {

   group+OPort IsoObjectLabelParams {
      int+Port2    Active;
      int+Port2    ContourFrequency;
      int+Port2    NodeFrequency;
      int+Port2    LineIndependent;
      int+Port2    NumberOfLabels;
      int+Port2    Decimals;
      string+Port2 Format;
   };

   module MaxFrequency{
      Mesh+Node_Data &in_fld<NEportLevels={2,1}>;
      int contourFreqMax<NEportLevels={1,2}>;
      int nodeFreqMax<NEportLevels={1,2}>;
	  int nnodesPerCellSet[in_fld.ncell_sets] => in_fld.cell_set.ncells;
	  nodeFreqMax => max_array(nnodesPerCellSet);
      contourFreqMax => in_fld.ncell_sets;
   };

   module IsoObjectLabelCore <src_file="isobjlbl.cxx",
                         libdeps="FLD",
                         cxx_hdr_files="fld/Xfld.h",
						 cxx_members="\tprivate: void SearchNext(int* Back, int* Forw, int* VisitFlag, int& JJ, int& GoOn);\n\t\tvoid MarkAndPost (int JJ, int* VisitFlag, int FreqCounter, int Frequency, int markNodes, int* OutFlag, int& Nnodes_out);\n"> {

      Mesh+Node_Data &in_fld<NEportLevels={2,1}>;
      IsoObjectLabelParams &params<NEportLevels={2,1}>;

      int Active => params.Active;
      int ContourFrequency => params.ContourFrequency;
      int NodeFrequency => params.NodeFrequency;
      int LineIndependent => params.LineIndependent;
      int NumberOfLabels => params.NumberOfLabels;

      cxxmethod update (
         in_fld+read+notify+req,
         Active+read+notify+req,
         ContourFrequency+read+notify+req,
         NodeFrequency+read+notify+req,
		 LineIndependent+read+notify+req,
		 NumberOfLabels+read+notify+req,
         out+write
      );

      Mesh+Node_Data out<NEportLevels={1,2}> {
         &xform => in_fld.xform;
      };
   };

};

