
//
// list of colors for markers
//                        R     G      B
#define MK_RED        { 0.99,  0.0,  0.0 }
#define MK_BLUE       { 0.0,   0.0,  0.99 }
#define MK_GREEN      { 0.0,   0.99, 0.0 }
#define MK_WHITE      { 0.99,  0.99, 0.99 }
#define MK_PURPLE     { 0.99,  0.0,  0.7 }
#define MK_YELLOW     { 0.99,  0.99, 0.0 }
#define MK_ORANGE     { 0.99,  0.5,  0.0 }
#define MK_NAVY       { 0.0,   0.5,  0.99 }
#define MK_DARK_GRAY  { 0.2,   0.2,  0.3 }
#define MK_DARK_RED   { 0.5,   0.1,  0.1 }
#define MK_LIGHT_GRAY { 0.5,   0.5,  0.7 }

#define MK_PI 3.1415927

flibrary Simple2D <compile_subs=0> {

  // Arrow
  macro mk_arrow {
        Quad  set1 {
              ncells = 1;
              node_connect_list = {0,1,2,3};
        };
        Tri set2 {
              ncells = 1;
              node_connect_list = {4,5,6};
        };
        Mesh  glyph2 {
                int nnodes = 7;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {0,-0.1,0},
                        {0.7,-0.1,0},
                        {0.7,0.1,0},
                        {0,0.1,0},
                        {0.7,-0.17,0},
                        {1,0,0},
                        {0.7,0.17,0}
                    };
                };
                int ncell_sets = 2;
                cell_set[ncell_sets] => {set1, set2 };
        };

        // std object output configuration
        GDM.DataObject obj {
	    in => glyph2;
	    Props {
	       col = MK_BLUE;
	       inherit = 0;
	    };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };


  // Diamond
  macro mk_diamond {
        Quad  set1 {
              ncells = 1;
              node_connect_list = {0,1,2,3};
        };
        Mesh  glyph2 {
                int nnodes = 4;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {0,-1,0},
                        {1,0,0},
                        {0,1,0},
                        {-1,0,0}
			};
                };
                int ncell_sets = 1;
                cell_set[ncell_sets] => {set1};
        };

        // std object output configuration
        GDM.DataObject obj {
	    in => glyph2;
	    Props {
	       col = MK_RED;
	       inherit = 0;
	    };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };



  // Square
  macro mk_square {
        Quad  set1 {
              ncells = 1;
              node_connect_list = {0,1,2,3};
        };
        Mesh  glyph2 {
                int nnodes = 4;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {1,-1,0},
                        {1,1,0},
                        {-1,1,0},
                        {-1,-1,0} };
		};
                int ncell_sets = 1;
                cell_set[ncell_sets] => {set1};
        };

        // std object output configuration
        GDM.DataObject obj {
	    in => glyph2;
	    Props {
	       col = MK_GREEN;
	       inherit = 0;
	    };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };


  // CrossX
  macro mk_crossx {
        Quad  set1 {
              ncells = 2;
              node_connect_list = {0,1,2,3,
                                   4,5,6,7};
        };
        Mesh  glyph2 {
                int nnodes = 8;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {1,-1.5,0},
                        {1.5,-1,0},
                        {-1,1.5,0},
                        {-1.5,1,0},
                        {1,1.5,0},
                        {1.5,1,0},
                        {-1,-1.5,0},
                        {-1.5,-1,0} };
		};
                int ncell_sets = 1;
                cell_set[ncell_sets] => {set1};
        };

        // std object output configuration
        GDM.DataObject obj {
	    in => glyph2;
	    Props {
	       col = MK_PURPLE;
	       inherit = 0;
	    };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };


  // Hexagon
  macro mk_hexagon {
        Quad  set1 {
              ncells = 2;
              node_connect_list = {0,1,2,3,
                                   4,5,6,7};
        };
        Mesh  glyph2 {
                int nnodes = 8;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {1,0,0},
                        {0.5,0.72,0},
                        {-0.5,0.72,0},
                        {-1,0,0},
                        {-1,0,0},
                        {-0.5,-0.72,0},
                        {0.5,-0.72,0},
                        {1,0,0} };
		};
                int ncell_sets = 1;
                cell_set[ncell_sets] => {set1};
        };

        // std object output configuration
        GDM.DataObject obj {
	    in => glyph2;
	    Props {
	       col = MK_YELLOW;
	       inherit = 0;
	    };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };




  // Triangle
  macro mk_triangle {
        Tri  set1 {
              ncells = 1;
              node_connect_list = {0,1,2};
        };
        Mesh  glyph2 {
                int nnodes = 3;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {0.7,-0.22,0},
                        {0,1,0},
                        {-0.7,-0.22,0} };
		};
                int ncell_sets = 1;
                cell_set[ncell_sets] => {set1};
        };

        // std object output configuration
        GDM.DataObject obj {
	    in => glyph2;
	    Props {
	       col = MK_ORANGE;
	       inherit = 0;
	    };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };



  // Tank
  macro mk_tank {
        Quad  set1 {
              ncells = 3;
              node_connect_list = {0,1,2,3,
                                   4,5,6,7,
                                   8,9,10,11};
        };
        Mesh  glyph2 {
                int nnodes = 12;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {1,-1,0},
                        {2,0,0},
                        {-2,0,0},
                        {-1,-1,0},

                        {1,0,0},
                        {0.5,0.8,0},
                        {-0.5,0.8,0},
                        {-1,0,0 },

                        {0.5,0.4,0},
                        {2,0.4,0},
                        {2,0.6,0},
                        {0.5,0.6,0} };

		};
                int ncell_sets = 1;
                cell_set[ncell_sets] => {set1};
        };

        // std object output configuration
        GDM.DataObject obj {
	    in => glyph2;
	    Props {
	       col = MK_DARK_RED;
	       inherit = 0;
	    };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };


  // Radar
  macro mk_radar {
        Quad  set1 {
              ncells = 4;
              node_connect_list = {0,1,2,3,
                                   4,5,6,7,
                                   8,9,10,11,
				   12,13,14,15};
        };
        Tri set2 {
              ncells = 1;
              node_connect_list = {16,17,18};
        };
        Mesh  glyph2 {
                int nnodes = 19;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {0.5,0.2,0},
                        {1,0.5,0},
                        {-0.5,2,0},
                        {-0.8,1.5,0},

                        {0.5,-1,0},
                        {0.5,0,0},
                        {-0.5,0,0},
                        {-0.5,-1,0},

                        {0,0,0},
                        {0.5,0.2,0},
                        {-0.8,1.5,0},
                        {-1,1,0},

                        {0.2,1,0},
                        {1,1.8,0},
                        {0.8,2,0},
                        {0,1.2,0},

                        {0,0,0},
                        {-1,1,0},
                        {-0.5,0.25,0} };

		};
                int ncell_sets = 2;
                cell_set[ncell_sets] => {set1, set2};
        };

        // std object output configuration
        GDM.DataObject obj {
	    in => glyph2;
	    Props {
	       col = MK_ORANGE;
	       inherit = 0;
	    };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };


  //
  // Plane  - 2D, very simple flat shape with 5 triangles
  //
  macro mk_plane {
        Tri  set1 {
              ncells = 5;
              node_connect_list = {0,1,2,
				   3,4,5,
				   6,7,8,
				   9,10,11,
				   12,13,14};
        };
        Mesh  glyph2 {
                int nnodes = 15;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {1,-1,0},
                        {3,0,0},
                        {1,1,0},

                        {0,0,0},
                        {2,0,0},
                        {-1,3,0},

                        {0,0,0},
                        {-1,-3,0},
                        {2,0,0},

                        {0,-1,0},
                        {0,1,0},
                        {-3,0,0},

                        {-2,0,0},
                        {-3,1,0},
                        {-3,-1,0}
                    };
                };
                int ncell_sets = 1;
                cell_set[ncell_sets] => {set1};
        };

        // std object output configuration
        GDM.DataObject obj {
	    in => glyph2;
	    Props {
	       col = MK_NAVY;
	       inherit = 0;
	    };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };


  // Triangle with colored verts
  macro mk_tri_rgb {
        Tri  set1 {
              ncells = 1;
              node_connect_list = {0,1,2};
        };
        Mesh+Node_Data+Vector+Float+Space3 tri_rgb<NEportLevels={0,1}> {
                nnodes = 3;

                coordinates {
                    values[nvals][veclen] = {
                        {1,0,0},
                        {0,3,0},
                        {-1,0,0} };
		};

                ncell_sets = 1;
                cell_set[ncell_sets] => {set1};

                nnode_data = 1;
		node_data[nnode_data];

                !node_data[0] {
                    values[nvals][veclen] = {
			    {0.9,0,0.5},
			    {0.9,0.9,0},
			    {0.2,0.2,0.6} };
		    nvals => <-.nnodes;
		    veclen = 3;
                    id = 667;
                };
        };

        // std object output configuration


        GDM.DataObject DataObject<NEx=418,NEy=209> {
            in => <-.tri_rgb;
        };

        olink out_fld => tri_rgb;
        olink out_obj<NEx=462,NEy=330> => .DataObject.obj;
  };



  // Needle and center dot with colored verts : PARAMS
  macro mk_needle {
        Tri  set1 {
              ncells = 16;
              node_connect_list = {2,0,1,
				   3,6,2,
				   4,7,3,
				   5,8,4,
				   

				   9,10,11,
				   9,11,12,
				   9,12,13,
				   9,13,14,
				   9,14,15,
				   9,15,16,
				   9,16,17,
				   9,17,18,
				   9,18,19,
				   9,19,20,
				   9,20,21,
				   9,21,10
				   
				   };
        };
        Mesh+Node_Data+Vector+Float+Space3 tri_rgb<NEportLevels={0,1}> {

                nnodes = 22;

                coordinates {

		    float sc = 0.22;

                    values[nvals][veclen] = {
		      // needle
                        {0,6,0},
                        {-1,0,0},
                        {-0.5,0.7,0},
                        {0,0.9,0},
                        {0.5,0.7,0},
                        {1,0,0},
                        {0,6.02,0},
                        {0,6.03,0},
                        {0,6.04,0},
		      // circle
                        {0,0,0},
                        {sc*4,sc*0,0},
                        {sc*3.4,sc*2,0},
                        {sc*2,sc*3.4,0},
                        {sc*0,sc*4,0},
                        {sc*(-2),sc*3.4,0},
                        {sc*(-3.4),sc*2,0},
                        {sc*(-4),sc*0,0},
                        {sc*(-3.4),sc*(-2),0},
                        {sc*(-2),sc*(-3.4),0},
                        {sc*0,sc*(-4),0},
                        {sc*2,sc*(-3.4),0},
                        {sc*3.4,sc*(-2),0}

		    };
		};

                ncell_sets = 1;
                cell_set[ncell_sets] => {set1};

                nnode_data = 1;
		node_data[nnode_data];

                !node_data[0] {
                    float lr = 0.99;
                    float lg = 0.40;
                    float lb = 0.88;

                    values[nvals][veclen] = {
			  // needle
                          { 0.25 * lr, 0.25 * lg, 0.25 * lb },
                          { 0.25 * lr, 0.25 * lg, 0.25 * lb },
                          { 0.25 * lr, 0.25 * lg, 0.25 * lb },
                          { 1.00 * lr, 1.00 * lg, 1.00 * lb },
                          { 0.50 * lr, 0.50 * lg, 0.50 * lb },
                          { 0.25 * lr, 0.25 * lg, 0.25 * lb },
                          { 0.25 * lr, 0.25 * lg, 0.25 * lb },
                          { 1.00 * lr, 1.00 * lg, 1.00 * lb },
                          { 0.75 * lr, 0.75 * lg, 0.75 * lb },
			  // center circle
			  { 0.9, 0.9, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 }


		    };

		    nvals => <-.nnodes;
		    veclen = 3;
                    id = 667;
                };
        };

        // std object output configuration


        GDM.DataObject DataObject<NEx=418,NEy=209> {
            in => <-.tri_rgb;
        };

        olink out_fld => tri_rgb;
        olink out_obj<NEx=462,NEy=330> => .DataObject.obj;
  };

  //
  // Smooth Dot with colored verts
  // 12-sided circle, with shading : PARAMS
  //
  macro mk_smdot {
        Tri  set1 {
              ncells = 12;
              node_connect_list = {
                                   0,1,2,
				   0,2,3,
				   0,3,4,
				   0,4,5,
				   0,5,6,
				   0,6,7,
				   0,7,8,
				   0,8,9,
				   0,9,10,
				   0,10,11,
				   0,11,12,
				   0,12,1

				   };
        };
        Mesh+Node_Data+Vector+Float+Space3 tri_rgb<NEportLevels={0,1}> {

                nnodes = 13;

                coordinates {

                    // size control, set to radius of 1
		    float sc = 0.20;

                    values[nvals][veclen] = {
		      // circle
                        {0,0,0},
                        {sc*4,sc*0,0},
                        {sc*3.4,sc*2,0},
                        {sc*2,sc*3.4,0},
                        {sc*0,sc*4,0},
                        {sc*(-2),sc*3.4,0},
                        {sc*(-3.4),sc*2,0},
                        {sc*(-4),sc*0,0},
                        {sc*(-3.4),sc*(-2),0},
                        {sc*(-2),sc*(-3.4),0},
                        {sc*0,sc*(-4),0},
                        {sc*2,sc*(-3.4),0},
                        {sc*3.4,sc*(-2),0}
		    };
		};

                ncell_sets = 1;
                cell_set[ncell_sets] => {set1};

                nnode_data = 1;
		node_data[nnode_data];

                !node_data[0] {

                    values[nvals][veclen] = {
			  // center circle
			  { 0.9, 0.9, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 },
			  { 0.2, 0.2, 0.99 }
		    };

		    nvals => <-.nnodes;
		    veclen = 3;
                    id = 667;
                };
        };

        // std object output configuration

        GDM.DataObject DataObject<NEx=418,NEy=209> {
            in => <-.tri_rgb;
        };

        olink out_fld => tri_rgb;
        olink out_obj<NEx=462,NEy=330> => .DataObject.obj;
  };


  // Radial Tic Lines with colored verts
  // 12-tris, with shading, decorative background : PARAMS
  macro mk_radtics {
        Tri  set1 {
              ncells = 12;
              node_connect_list = {
                                   13,1,2,
				   14,2,3,
				   15,3,4,
				   16,4,5,
				   17,5,6,
				   18,6,7,
				   19,7,8,
				   20,8,9,
				   21,9,10,
				   22,10,11,
				   23,11,12,
				   24,12,1

				   };
        };
        Mesh+Node_Data+Vector+Float+Space3 tri_rgb<NEportLevels={0,1}> {

                nnodes = 25;

                coordinates {

                    // size control, set to radius of 1
		    float sc = 0.20;
		    float sc2 = 0.10;

                    values[nvals][veclen] = {
		      // circle
                        {0,0,0},

                        {sc*4,sc*0,0},
                        {sc*3.4,sc*2,0},
                        {sc*2,sc*3.4,0},
                        {sc*0,sc*4,0},
                        {sc*(-2),sc*3.4,0},
                        {sc*(-3.4),sc*2,0},
                        {sc*(-4),sc*0,0},
                        {sc*(-3.4),sc*(-2),0},
                        {sc*(-2),sc*(-3.4),0},
                        {sc*0,sc*(-4),0},
                        {sc*2,sc*(-3.4),0},
                        {sc*3.4,sc*(-2),0},

                        {sc2*3.75,sc2*1,0},
                        {sc2*2.8,sc2*2.8,0},
                        {sc2*1,sc2*3.75,0},

                        {sc2*(-1),sc2*3.75,0},
                        {sc2*(-2.8),sc2*2.8,0},
                        {sc2*(-3.75),sc2*1,0},

                        {sc2*(-3.75),sc2*(-1),0},
                        {sc2*(-2.8),sc2*(-2.8),0},
                        {sc2*(-1),sc2*(-3.75),0},

                        {sc2*1,sc2*(-3.75),0},
                        {sc2*2.8,sc2*(-2.8),0},
                        {sc2*3.75,sc2*(-1),0}
		    };
		};

                ncell_sets = 1;
                cell_set[ncell_sets] => {set1};

                nnode_data = 1;
		node_data[nnode_data];

                !node_data[0] {

                    float lr = 0.22;
                    float lg = 0.22;
                    float lb = 0.99;

                    float lr2 = 0.82;
                    float lg2 = 0.22;
                    float lb2 = 0.99;

                    values[nvals][veclen] = {
			  // center circle
			  { 0.99, 0.99, 0.99 },

                          { lr, lg, lb },
                          { lr, lg, lb },
                          { lr, lg, lb },
                          { lr, lg, lb },
                          { lr, lg, lb },
                          { lr, lg, lb },
                          { lr, lg, lb },
                          { lr, lg, lb },
                          { lr, lg, lb },
                          { lr, lg, lb },
                          { lr, lg, lb },
                          { lr, lg, lb },

                          { lr2, lg2, lb2 },
                          { lr2, lg2, lb2 },
                          { lr2, lg2, lb2 },
                          { lr2, lg2, lb2 },
                          { lr2, lg2, lb2 },
                          { lr2, lg2, lb2 },
                          { lr2, lg2, lb2 },
                          { lr2, lg2, lb2 },
                          { lr2, lg2, lb2 },
                          { lr2, lg2, lb2 },
                          { lr2, lg2, lb2 },
                          { lr2, lg2, lb2 }

		    };

		    nvals => <-.nnodes;
		    veclen = 3;
                    id = 667;
                };
        };

        // std object output configuration

        GDM.DataObject DataObject<NEx=418,NEy=209> {
            in => <-.tri_rgb;
        };

        olink out_fld => tri_rgb;
        olink out_obj<NEx=462,NEy=330> => .DataObject.obj;
  };



  // Dash Tic Blocks, RGB node data
  // NB: MORE WORK TO DO ON THIS???
  macro mk_dashtic {
        Quad  set1 {
              ncells = 5;
              node_connect_list = {0,1,2,3,
				   4,5,6,7,
				   8,9,10,11,
				   12,13,14,15,
				   16,17,18,19};
        };
        Mesh+Node_Data+Vector+Float  glyph2 {
                int nnodes = 20;
                int nspace = 3;
                coordinates {

		    float sc = 0.35; // glyph scale factor

                    float values[nvals][veclen] = {
                        {sc * 6,sc * -1,0},  // 1
                        {sc * 6,sc * 1,0},
                        {sc * 12,sc * 1,0},
                        {sc * 12,sc * -1,0},

                        {sc * 5.2,sc * 3.8,0},  // 2
                        {sc * 3.8,sc * 5.2,0},
                        {sc * 7.5,sc * 8.8,0},
                        {sc * 8.8,sc * 7.5,0},

                        {sc * -1,sc * 6,0},  // 3
                        {sc * -1,sc * 12,0},
                        {sc * 1,sc * 12,0},
                        {sc * 1,sc * 6,0},

                        {sc * -5.2,sc * 3.8,0},  // 4
                        {sc * -3.8,sc * 5.2,0},
                        {sc * -7.5,sc * 8.8,0},
                        {sc * -8.8,sc * 7.5,0},

                        {sc * -6,sc * -1,0},  // 5
                        {sc * -6,sc * 1,0},
                        {sc * -12,sc * 1,0},
                        {sc * -12,sc * -1,0}


			};
                };
                int ncell_sets = 1;
                cell_set[ncell_sets] => {set1};

#define DASHTIC_RED    {1,0,0}
#define DASHTIC_PINK   {0.7,0.4,0.4}
#define DASHTIC_WHITE  {1,1,1}
#define DASHTIC_LTGRN  {0.4,0.7,0.4}
#define DASHTIC_GREEN  {0,1,0}
#define DASHTIC_BLACK  {0.1,0.1,0.1}

                nnode_data = 1;
		node_data[nnode_data];

                !node_data[0] {
                    values[nvals][veclen] = {
			    DASHTIC_RED,
			    DASHTIC_RED,
			    DASHTIC_RED,
			    DASHTIC_RED,
			    DASHTIC_PINK,
			    DASHTIC_PINK,
			    DASHTIC_PINK,
			    DASHTIC_PINK,
			    DASHTIC_WHITE,
			    DASHTIC_WHITE,
			    DASHTIC_WHITE,
			    DASHTIC_WHITE,
			    DASHTIC_LTGRN,
			    DASHTIC_LTGRN,
			    DASHTIC_LTGRN,
			    DASHTIC_LTGRN,
			    DASHTIC_GREEN,
			    DASHTIC_GREEN,
			    DASHTIC_GREEN,
			    DASHTIC_GREEN };
		    nvals => <-.nnodes;
		    veclen = 3;
                    id = 667;
                };
        };

        // std object output configuration
        GDM.DataObject obj {
	    in => glyph2;
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };



  //
  // labels on dashboard object
  //
  macro mk_dashtext {

    Point  set1 {
        ncells = 5;
        node_connect_list = {0,1,2,3,4};
    };

    Mesh mesh {
        int nnodes = 5;
                int nspace = 2;
                coordinates {
                float values[nvals][veclen] = {
                        {4.2,0},
                        {3.2,3.2},
                        {0,4.3},
                        {-3.2,3.2},
                        {-4.2,0}
                        };
                };
                int ncell_sets = 1;
                cell_set[ncell_sets] => {set1};
    };
    string in_text[] = {"2","1","0","-1","-2"};

    TextValues TextValues {
        text_values => <-.in_text;
        align_horiz<NEportLevels={2,0}> = 0;
        align_vert<NEportLevels={2,0}> = 0;
        drop_shadow<NEportLevels={2,0}> = 0;
        bounds<NEportLevels={2,0}> = 0;
        underline<NEportLevels={2,0}> =0;
        background<NEportLevels={2,0}> =0;
        lead_line<NEportLevels={2,0}> =0;
        radial<NEportLevels={2,0}> =0;
        do_offset<NEportLevels={2,0}> =0;
        offset = { 0.05, 0.05, 0.0 };
    };

    TextField &TextField => merge(TextValues, mesh);

    GDM.DataObject obj {
        in => TextField;
	Props.col = DASHTIC_BLACK;
    };

    olink out_obj => obj.obj;

  };


    //
    // polygon object, with N sides
    //  house shape, using mesh mappers
    //
    macro mk_house {
    
      int+Port   nnodes = 5;
      int+Port   nspace = 3;
      float+Port verts[nnodes][nspace] => {
    		   {0, 0, 0},
    		   {2, 0, 0},
    		   {2, 2, 0},
    		   {1, 3, 0},
    		   {0, 2, 0}
                     };
    
      int+Port   connect[nnodes] => init_array(nnodes,0,nnodes-1);
    
      int+Port   poly_nodes[1]   =>{nnodes};
    
      FLD_MAP.Mesh_Mappers.polyhedron_mesh  polygon{
            coord      => <-.verts;
            connect    => <-.connect;
            poly_nodes => <-.poly_nodes;
      };

      olink out => polygon.out;
      olink obj => polygon.obj;   
    };


   //
   // simple large 2D square
   // (from tcsvw25)
   //
   macro mk_bigsquare {
      //
      // simple square in 2D
      //
      Mesh_Unif+OPort2 Square {
                dims    => {2,2};
                nspace  =  2;
                ndim    =  2;
                points  => {{-4, -4}, {4, 4}};
      };

       GDM.DataObject obj {
            in => <-.Square;
            Modes {
               mode = {0,0,GD_SURF_FLAT,0,0};
               normals = GD_NO_NORMALS;
            };
        };
        olink out_obj => obj.obj;
   };

  //
  // simple wire frame box, made from disjoint lines
  // with node data for line interpolated color
  //
  macro mk_wirebox2D {
    Field+Space3+Scalar+Float my_field {
       nnodes = 4;
       !node_data[0] {
          values = {0,1,2,3};
       };
       coordinates {
          values = {0,0,0,
                    1,0,0,
                    1,1,0,
                    0,1,0};
       };
       ncell_sets = 1;
       Line cell_set[ncell_sets];
       !cell_set[0] {
          ncells=4;
          node_connect_list = {0,1,1,2,2,3,3,0};
       };
    };
    GDM.DataObject DataObject {
        in => <-.my_field;
    };

    olink out => my_field;
    olink obj => DataObject.obj;
  };



  //
  // wireframe Eiffel Tower, Paris, with single polyline cell set
  // David Curington, Age 9
  //
  macro mk_eiffel2D {
   FLD_MAP.polyline_mesh polyline_mesh {
      coord => <-.coordinates;
      connect => <-.dims;
   };
   int dims[] = {0,9,10,13,14,16,17,21};
   float coordinates[22][3] => {

      2,1,0,
      4,1,0,
      5,3,0,
      7,4,0,
      9,3,0,
      10,1,0,
      12,1,0,
      9,5,0,
      5,5,0,
      2,1,0,

      5,5,0,
      6,11,0,
      8,11,0,
      9,5,0,

      6,6,0,
      7,10,0,
      8,6,0,

      6,11,0,
      6.5,20,0,
      7,21,0,
      7.5,20,0,
      8,11,0

   };
   link fld<NEportLevels={1,2},NEx=44.,NEy=165.> => .polyline_mesh.out;
   link obj<NEportLevels={1,2},NEx=242.,NEy=165.> => .polyline_mesh.obj;
  };


  //
  // David Curington's Rocket, made from single polyline cell set
  // Age 9
  //
  macro mk_rocket2D {
   FLD_MAP.polyline_mesh polyline_mesh {
      coord => <-.coordinates;
      connect => <-.dims;
   };
   int dims[] => {0,27,28,36,37,38,39,40,41,42,43,47,48,52 };
   float coordinates[53][3] => {
        4,1,0,
        4,4,0,
        3,4,0,
        3,9,0,
        4,12,0,
        2,10,0,
        4,15,0,
        5,20,0,
        4,20,0,
        5,21,0,
        5.5,22,0,
        5.5,25,0,
        5.5,22,0,
        6,21,0,
        7,20,0,
        6,20,0,
        7,15,0,
        9,10,0,
        7,12,0,
        8,9,0,
        8,4,0,
        7,4,0,
        7,1,0,
        6,1,0,
        6,2,0,
        5,2,0,
        5,1,0,
        4,1,0,

        4,4,0,
        5,5.5,0,
        6,5.5,0,
        7,4,0,
        6,4,0,
        6,5,0,
        5,5,0,
        5,4,0,
        4,4,0,

        5,4,0,
        5,2,0,

        6,4,0,
        6,2,0,

        3,9,0,
        8,9,0,

        4,12,0,
        7,12,0,
        7,15,0,
        4,15,0,
        4,12,0,

        5,20,0,
        5,21,0,
        6,21,0,
        6,20,0,
        5,20,0
           };
   link fld<NEportLevels={1,2},NEx=44.,NEy=165.> => .polyline_mesh.out;
   link obj<NEportLevels={1,2},NEx=242.,NEy=165.> => .polyline_mesh.obj;
  };


};
