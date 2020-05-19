
flibrary ReadNastranMods <build_dir="xp_comm_proj/rd_nastr",
                         out_hdr_file="gen.h",
                         out_src_file="gen.cxx"> {

   group ReadNastranParams {
      string filename;

	enum WireframeMode { choices = {"none", "regular"}; } = 0;
	enum SurfaceMode { choices = {"None","No Lighting","Flat Shading","Gourad Shading"}; } = 3;
	enum ColorMode { choices = {"By Node","By Material"}; } = 1;

	int NormalGenerationMode => (SurfaceMode==4) + 1; //Gourad ==> Vertex
	int NodeDataOn => (ColorMode==0);
	int AlternateOn = 1;
	int CacheSize = 64;
   };


    group nastran_output {
	    int Size=0;
	    int IDS[Size];
	    float Coords[Size][3];

	    int CTRIAX6_Size=0;
	    int CTRIAX6[CTRIAX6_Size*6];
	    int CTRIAX6IDS[CTRIAX6_Size];
	    int CTRIAX6MATERIALS[CTRIAX6_Size];

	    int CHEXA8_Size=0;
	    int CHEXA8[CHEXA8_Size*8];
	    int CHEXA8IDS[CHEXA8_Size];
	    int CHEXA8MATERIALS[CHEXA8_Size];

	    int CHEXA20_Size=0;
	    int CHEXA20[CHEXA20_Size*20];
	    int CHEXA20IDS[CHEXA20_Size];
	    int CHEXA20MATERIALS[CHEXA20_Size];

	    int CTETRA4_Size=0;
	    int CTETRA4[CTETRA4_Size*4];
	    int CTETRA4IDS[CTETRA4_Size];
	    int CTETRA4MATERIALS[CTETRA4_Size];

	    int CTETRA10_Size=0;
	    int CTETRA10[CTETRA10_Size*10];
	    int CTETRA10IDS[CTETRA10_Size];
	    int CTETRA10MATERIALS[CTETRA10_Size];

	    int CPENTA6_Size=0;
	    int CPENTA6[CPENTA6_Size*6];
	    int CPENTA6IDS[CPENTA6_Size];
	    int CPENTA6MATERIALS[CPENTA6_Size];

	    int CPENTA15_Size=0;
	    int CPENTA15[CPENTA15_Size*15];
	    int CPENTA15IDS[CPENTA15_Size];
	    int CPENTA15MATERIALS[CPENTA15_Size];


	    int CurrentNode=0;//?
	    int CurrentCellSet=0;//?
	    int CurrentElement=0;//?

    };

	module ReadNastranCore <src_file="rd_nastr.cxx",
		cxx_src_files="Indexable.cxx GridClasses.cxx DBFparserClasses.cxx WP1_MeshClasses.cxx MaterialsClasses.cxx ConnectivityClasses.cxx Utils.cxx"
	> {
		ReadNastranParams& ReadNastranParams<NEportLevels={2,1}>;
		nastran_output& nastran_output<NEportLevels={2,1}>;

		cxxmethod Import(
			ReadNastranParams.filename+notify+req
		);
	};


}; // end of library

