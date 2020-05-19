
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

flibrary Simple3D <compile_subs=0> {

  //
  // Plane 3D, 7 triangle 3D airplane glyph
  //
  macro mk_plane3D {
        Tri  set1 {
              ncells = 7;
              node_connect_list = {0,1,2,
				   3,4,5,
				   6,7,8,
				   9,10,11,
				   12,13,14,
				   1,15,12,
				   11,12,16};
        };
        Mesh  glyph2 {
                int nnodes = 17;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {0.7,-0.7,0},
                        {3,0,0},
                        {0.7,0.7,0},

                        {0,0,0},
                        {1.5,0,0},
                        {-1,3,0},

                        {0,0,0},
                        {-1,-3,0},
                        {1.5,0,0},

                        {0,-0.7,0},
                        {0,0.7,0},
                        {-3,0,0},

                        {-2,0,0},
                        {-3,1,0},
                        {-3,-1,0},

                        {1,0,1.2},
                        {-3,0,1.2}
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
            Modes {
               mode = {0,0,GD_SURF_FLAT,0,0};
               normals = GD_NO_NORMALS;
            };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };


  //
  // Ship 3D - simple geometric icon to look like
  // a small battle ship
  //
  macro mk_ship3d {
        Hex  set1 {
              ncells = 4;
              node_connect_list = {0,1,2,3,4,5,6,7,
                                   8,9,10,11,12,13,14,15,
                                   16,17,18,19,20,21,22,23,
				   24,25,26,27,28,29,30,31};
        };
        Mesh  glyph2 {
                int nnodes = 32;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                     // hull
                        {-3,0.,0},
                        {0,0.7,0},
                        {3,0,0},
                        {0,-0.7,0},
                        {-4,0.,1},
                        {0,1.2,1},
                        {4,0,1},
                        {0,-1.2,1},
                     // deck box
                        {-2,-0.5,1},
                        {-2, 0.5,1},
                        { 2, 0.5,1},
                        { 2,-0.5,1},
                        {-2,-0.5,1.2},
                        {-2, 0.5,1.2},
                        { 2, 0.5,1.2},
                        { 2,-0.5,1.2},
                     // tower
                        {-1,-0.2,1},
                        {-1, 0.2,1},
                        {-0.6, 0.2,1},
                        {-0.6,-0.2,1},
                        {-1,-0.2,2},
                        {-1, 0.2,2},
                        {-0.6, 0.2,2},
                        {-0.6,-0.2,2},
                     // gun
                        {0,-0.1,1},
                        {0, 0.1,1},
                        {3, 0.1,1.5},
                        {3,-0.1,1.5},
                        {0,-0.1,1.1},
                        {0, 0.1,1.1},
                        {3, 0.1,1.6},
                        {3,-0.1,1.6}
                    };
                };
                int ncell_sets = 1;
                cell_set[ncell_sets] => {set1};
        };

        // std object output configuration
        GDM.DataObject obj {
	    in => glyph2;
	    Props {
	       col = MK_LIGHT_GRAY;
	       inherit = 0;
	    };
            Modes {
               mode = {0,0,GD_SURF_FLAT,0,0};
               normals = GD_NO_NORMALS;
            };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
  };


  // 3D Cone with colored verts
  // 12-tris, with shading : PARAMS
  macro mk_cone3d_rgb {
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
		    float sc = 0.125;
		    float sc2 = 0.0;
		    float z1 = 0.0;
		    float z2 = 1.0;

                    values[nvals][veclen] = {
		      // circle
                        {0,0,0},

                        {sc*4,sc*0,z1},
                        {sc*3.4,sc*2,z1},
                        {sc*2,sc*3.4,z1},
                        {sc*0,sc*4,z1},
                        {sc*(-2),sc*3.4,z1},
                        {sc*(-3.4),sc*2,z1},
                        {sc*(-4),sc*0,z1},
                        {sc*(-3.4),sc*(-2),z1},
                        {sc*(-2),sc*(-3.4),z1},
                        {sc*0,sc*(-4),z1},
                        {sc*2,sc*(-3.4),z1},
                        {sc*3.4,sc*(-2),z1},

                        {sc2*3.75,sc2*1,z2},
                        {sc2*2.8,sc2*2.8,z2},
                        {sc2*1,sc2*3.75,z2},

                        {sc2*(-1),sc2*3.75,z2},
                        {sc2*(-2.8),sc2*2.8,z2},
                        {sc2*(-3.75),sc2*1,z2},

                        {sc2*(-3.75),sc2*(-1),z2},
                        {sc2*(-2.8),sc2*(-2.8),z2},
                        {sc2*(-1),sc2*(-3.75),z2},

                        {sc2*1,sc2*(-3.75),z2},
                        {sc2*2.8,sc2*(-2.8),z2},
                        {sc2*3.75,sc2*(-1),z2}
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


  // 3D Cone geometry only, no node data : PARAMS
  // 12-tris
  macro mk_cone3d_mesh {

       // size control, set to radius of 1
       float sc = 0.125;
       float sc2 = 0.0;
       float z1 = 0.0;
       float z2 = 1.0;

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
        Mesh+Vector+Float+Space3 tri_mesh<NEportLevels={0,1}> {

                nnodes = 25;

                coordinates {

                    // size controls
		    float sc =>  <-.<-.sc;
		    float sc2 => <-.<-.sc2;
		    float z1 =>  <-.<-.z1;
		    float z2 =>  <-.<-.z2;

                    values[nvals][veclen] = {
		      // circle
                        {0,0,0},

                        {sc*4,sc*0,z1},
                        {sc*3.4,sc*2,z1},
                        {sc*2,sc*3.4,z1},
                        {sc*0,sc*4,z1},
                        {sc*(-2),sc*3.4,z1},
                        {sc*(-3.4),sc*2,z1},
                        {sc*(-4),sc*0,z1},
                        {sc*(-3.4),sc*(-2),z1},
                        {sc*(-2),sc*(-3.4),z1},
                        {sc*0,sc*(-4),z1},
                        {sc*2,sc*(-3.4),z1},
                        {sc*3.4,sc*(-2),z1},

                        {sc2*3.75,sc2*1,z2},
                        {sc2*2.8,sc2*2.8,z2},
                        {sc2*1,sc2*3.75,z2},

                        {sc2*(-1),sc2*3.75,z2},
                        {sc2*(-2.8),sc2*2.8,z2},
                        {sc2*(-3.75),sc2*1,z2},

                        {sc2*(-3.75),sc2*(-1),z2},
                        {sc2*(-2.8),sc2*(-2.8),z2},
                        {sc2*(-1),sc2*(-3.75),z2},

                        {sc2*1,sc2*(-3.75),z2},
                        {sc2*2.8,sc2*(-2.8),z2},
                        {sc2*3.75,sc2*(-1),z2}
		    };
		};

                ncell_sets = 1;
                cell_set[ncell_sets] => {set1};

        };

        // std object output configuration

        GDM.DataObject DataObject<NEx=418,NEy=209> {
            in => <-.tri_mesh;
        };

        olink out_fld => tri_mesh;
        olink out_obj<NEx=462,NEy=330> => .DataObject.obj;
  };


  // 3D Cylinder geometry only, no node data : PARAMS
  // 12-quads
  macro mk_cylinder {

       // size control, set to radius of 1
       float sc = 0.125;
       float sc2 = 0.125;
       float z1 = 0.0;
       float z2 = 1.0;

        Quad  set1 {
              ncells = 12;
              node_connect_list = {
                                   13,1,2,14,
				   14,2,3,15,
				   15,3,4,16,
				   16,4,5,17,
				   17,5,6,18,
				   18,6,7,19,
				   19,7,8,20,
				   20,8,9,21,
				   21,9,10,22,
				   22,10,11,23,
				   23,11,12,24,
				   24,12,1,13
				   };
        };
        Mesh+Vector+Float+Space3 tri_mesh<NEportLevels={0,1}> {

                nnodes = 25;

                coordinates {

                    // size controls
		    float sc =>  <-.<-.sc;
		    float sc2 => <-.<-.sc2;
		    float z1 =>  <-.<-.z1;
		    float z2 =>  <-.<-.z2;

                    values[nvals][veclen] = {
		      // two circles
                        {0,0,0},   // first point not used

                        {sc*4,sc*0,z1},
                        {sc*3.4,sc*2,z1},
                        {sc*2,sc*3.4,z1},
                        {sc*0,sc*4,z1},
                        {sc*(-2),sc*3.4,z1},
                        {sc*(-3.4),sc*2,z1},
                        {sc*(-4),sc*0,z1},
                        {sc*(-3.4),sc*(-2),z1},
                        {sc*(-2),sc*(-3.4),z1},
                        {sc*0,sc*(-4),z1},
                        {sc*2,sc*(-3.4),z1},
                        {sc*3.4,sc*(-2),z1},

                        {sc2*4,sc*0,z2},
                        {sc2*3.4,sc*2,z2},
                        {sc2*2,sc*3.4,z2},
                        {sc2*0,sc*4,z2},
                        {sc2*(-2),sc*3.4,z2},
                        {sc2*(-3.4),sc*2,z2},
                        {sc2*(-4),sc*0,z2},
                        {sc2*(-3.4),sc*(-2),z2},
                        {sc2*(-2),sc*(-3.4),z2},
                        {sc2*0,sc*(-4),z2},
                        {sc2*2,sc*(-3.4),z2},
                        {sc2*3.4,sc*(-2),z2}

		    };
		};

                ncell_sets = 1;
                cell_set[ncell_sets] => {set1};

        };

        // std object output configuration

        GDM.DataObject DataObject<NEx=418,NEy=209> {
            in => <-.tri_mesh;
        };

        olink out_fld => tri_mesh;
        olink out_obj<NEx=462,NEy=330> => .DataObject.obj;
  };




  //
  // 3D Solid Arrow geometry only, no node data
  // (cone + cylinder)
  // special version with reversed connectivity from above,
  // such that normals are correct, and orientation is reversed,
  // Arrow points along X axis.
  // PARAMS / Build from simpler objects??

  macro mk_solid_arrow {

       // size control, set to radius of 1
       float cone_sc = 0.0625;
       float cone_sc2 = 0.0;
       float cone_z1 = 0.5;
       float cone_z2 = 1.0;

       float cyl_sc = 0.03;
       float cyl_sc2 = 0.03;
       float cyl_z1 = 0.0;
       float cyl_z2 = 0.5;

        Tri  set1 {
              ncells = 12;
              node_connect_list = {
                                   13,2,1,
				   14,3,2,
				   15,4,3,
				   16,5,4,
				   17,6,5,
				   18,7,6,
				   19,8,7,
				   20,9,8,
				   21,10,9,
				   22,11,10,
				   23,12,11,
				   24,1,12
				   };
        };


        Quad  set2 {
              ncells = 12;
              node_connect_list = {
                                   37,38,26,25,
				   38,39,27,26,
				   39,40,28,27,
				   40,41,29,28,
				   41,42,30,29,
				   42,43,31,30,
				   43,44,32,31,
				   44,45,33,32,
				   45,46,34,33,
				   46,47,35,34,
				   47,48,36,35,
				   48,37,25,36
				   };
        };

        Mesh+Vector+Float+Space3 arrow_mesh<NEportLevels={0,1}> {

                nnodes = 49;

                coordinates {

                    // size controls
		    float sc =>  <-.<-.cone_sc;
		    float sc2 => <-.<-.cone_sc2;
		    float z1 =>  <-.<-.cone_z1;
		    float z2 =>  <-.<-.cone_z2;

		    float cyl_sc =>  <-.<-.cyl_sc;
		    float cyl_sc2 => <-.<-.cyl_sc2;
		    float cyl_z1 =>  <-.<-.cyl_z1;
		    float cyl_z2 =>  <-.<-.cyl_z2;

                    values[nvals][veclen] = {
		      // circle
                        {0,0,0},

                        // Cone nodes

                        {z1,sc*0,sc*4},
                        {z1,sc*2,sc*3.4},
                        {z1,sc*3.4,sc*2},
                        {z1,sc*4,0},
                        {z1,sc*3.4,sc*(-2)},
                        {z1,sc*2,sc*(-3.4)},
                        {z1,sc*0,sc*(-4)},
                        {z1,sc*(-2),sc*(-3.4)},
                        {z1,sc*(-3.4),sc*(-2)},
                        {z1,sc*(-4),0},
                        {z1,sc*(-3.4),sc*2},
                        {z1,sc*(-2),sc*3.4},

                        {z2,sc2*1,sc2*3.75},
                        {z2,sc2*2.8,sc2*2.8},
                        {z2,sc2*3.75,sc2*1},

                        {z2,sc2*3.75,sc2*(-1)},
                        {z2,sc2*2.8,sc2*(-2.8)},
                        {z2,sc2*1,sc2*(-3.75)},

                        {z2,sc2*(-1),sc2*(-3.75)},
                        {z2,sc2*(-2.8),sc2*(-2.8)},
                        {z2,sc2*(-3.75),sc2*(-1)},

                        {z2,sc2*(-3.75),sc2*1},
                        {z2,sc2*(-2.8),sc2*2.8},
                        {z2,sc2*(-1),sc2*3.75},

                        // Cylinder nodes

                        {cyl_z1,cyl_sc*0,cyl_sc*4},
                        {cyl_z1,cyl_sc*2,cyl_sc*3.4},
                        {cyl_z1,cyl_sc*3.4,cyl_sc*2},
                        {cyl_z1,cyl_sc*4,0},
                        {cyl_z1,cyl_sc*3.4,cyl_sc*(-2)},
                        {cyl_z1,cyl_sc*2,cyl_sc*(-3.4)},
                        {cyl_z1,cyl_sc*0,cyl_sc*(-4)},
                        {cyl_z1,cyl_sc*(-2),cyl_sc*(-3.4)},
                        {cyl_z1,cyl_sc*(-3.4),cyl_sc*(-2)},
                        {cyl_z1,cyl_sc*(-4),0},
                        {cyl_z1,cyl_sc*(-3.4),cyl_sc*2},
                        {cyl_z1,cyl_sc*(-2),cyl_sc*3.4},

                        {cyl_z2,cyl_sc2*0,cyl_sc2*4},
                        {cyl_z2,cyl_sc2*2,cyl_sc2*3.4},
                        {cyl_z2,cyl_sc2*3.4,cyl_sc2*2},
                        {cyl_z2,cyl_sc2*4,0},
                        {cyl_z2,cyl_sc2*3.4,cyl_sc2*(-2)},
                        {cyl_z2,cyl_sc2*2,cyl_sc2*(-3.4)},
                        {cyl_z2,cyl_sc2*0,cyl_sc2*(-4)},
                        {cyl_z2,cyl_sc2*(-2),cyl_sc2*(-3.4)},
                        {cyl_z2,cyl_sc2*(-3.4),cyl_sc2*(-2)},
                        {cyl_z2,cyl_sc2*(-4),0},
                        {cyl_z2,cyl_sc2*(-3.4),cyl_sc2*2},
                        {cyl_z2,cyl_sc2*(-2),cyl_sc2*3.4}
		    };
		};

                ncell_sets = 2;
                cell_set[ncell_sets] => {set1,set2};

        };

        // std object output configuration

        GDM.DataObject DataObject<NEx=418,NEy=209> {
            in => <-.arrow_mesh;
        };

        olink out_fld => arrow_mesh;
        olink out_obj<NEx=462,NEy=330> => .DataObject.obj;
  };


    //
    // AVS pyramid logo shape
    //
    macro mk_avslogo {


    #define MK_LOGOBLUE { 0.2, 1.0, 0.6 }


        Tet  set1 {
              ncells = 1;
              node_connect_list = {0,3,2,1};
        };
        Prism set2 {
              ncells = 2;
              node_connect_list = {4,6,5,7,9,8,
				   10,12,11,13,15,14};
        };
        Mesh+Port  glyph2 {
                int nnodes = 16;
                int nspace = 3;
                coordinates {


   float low_gap_pos  = 0.23;
   float low_gap_width= 0.135;
   float up_gap_pos   = 0.63;
   float up_gap_width = 0.155;
   float asp          = 1.0;  // aspect

   float h0 = 0;
   float h1 => low_gap_pos;
   float h2 => low_gap_pos + low_gap_width;
   float h3 => up_gap_pos;
   float h4 => up_gap_pos + up_gap_width;
   float h5 = 1.0;

   float xx => (120.0 * (MK_PI / 180.0));
   float pos0 = 0.0;
   float pos1 => xx;
   float pos2 => 2.0 * xx;


                    float values[nvals][veclen] => {

                      /* bottom tetrahedron */
    {(cos(pos0)*asp*(h0)),(sin(pos0)*asp*(h0)),(h0)},
    {(cos(pos0)*asp*(h1)),(sin(pos0)*asp*(h1)),(h1)},
    {(cos(pos1)*asp*(h1)),(sin(pos1)*asp*(h1)),(h1)},
    {(cos(pos2)*asp*(h1)),(sin(pos2)*asp*(h1)),(h1)},

                      /* middle section prism */
    {(cos(pos0)*asp*(h2)),(sin(pos0)*asp*(h2)),(h2)},
    {(cos(pos1)*asp*(h2)),(sin(pos1)*asp*(h2)),(h2)},
    {(cos(pos2)*asp*(h2)),(sin(pos2)*asp*(h2)),(h2)},
    {(cos(pos0)*asp*(h3)),(sin(pos0)*asp*(h3)),(h3)},
    {(cos(pos1)*asp*(h3)),(sin(pos1)*asp*(h3)),(h3)},
    {(cos(pos2)*asp*(h3)),(sin(pos2)*asp*(h3)),(h3)},

                      /* top section prism */
    {(cos(pos0)*asp*(h4)),(sin(pos0)*asp*(h4)),(h4)},
    {(cos(pos1)*asp*(h4)),(sin(pos1)*asp*(h4)),(h4)},
    {(cos(pos2)*asp*(h4)),(sin(pos2)*asp*(h4)),(h4)},
    {(cos(pos0)*asp*(h5)),(sin(pos0)*asp*(h5)),(h5)},
    {(cos(pos1)*asp*(h5)),(sin(pos1)*asp*(h5)),(h5)},
    {(cos(pos2)*asp*(h5)),(sin(pos2)*asp*(h5)),(h5)}

                    };
                };
                int ncell_sets = 2;
                cell_set[ncell_sets] => {set1, set2 };
        };

        // std object output configuration
        GDM.DataObject obj {
            in => glyph2;
            Props {
               col = MK_LOGOBLUE;
               inherit = 0;
            };
            Modes {
               mode = {0,0,GD_SURF_FLAT,0,0};
               normals = GD_NO_NORMALS;
            };
        };
        olink out_fld => glyph2;
        olink out_obj => obj.obj;
   };


   //
   // Dr. Sergio Punzi contributed objects
   //
   macro mk_crystal {
       Mesh_Spher_Unif+Dim3+Space3 crystal<NEportLevels={0,2}> {
          dims = {2,4,4};
          points = {0.,0.,0.,1., 2*acos(-1),2*acos(-1)};
       };
       GDM.DataObject obj {
            in => <-.crystal;
            Modes {
              // mode = {0,0,GD_SURF_FLAT,0,0};
               normals = GD_NO_NORMALS;
            };
        };
        olink out_obj => obj.obj;
   };
   macro mk_cucumber {
       Mesh_Spher_Unif+Dim3+Space3 cucumber<NEportLevels={0,2}> {
          dims = {2,8,8};
          points = {0.,0.,0.,1., 2*acos(-1),2*acos(-1)};
       };
       GDM.DataObject obj {
            in => <-.cucumber;
            Modes {
               // mode = {0,0,GD_SURF_FLAT,0,0};
               normals = GD_NO_NORMALS;
            };
        };
        olink out_obj => obj.obj;
   };
   macro mk_cylinder2 {
       Mesh_Cyl_Unif+Space3+Dim3 cylinder<NEportLevels={0,2}> {
          dims = {2,8,8};
          points = {0.,0.,0.,1., 2*acos(-1),1.};
       };
       GDM.DataObject obj {
            in => <-.cylinder;
            Modes {
               mode = {0,0,GD_SURF_FLAT,0,0};
               normals = GD_NO_NORMALS;
            };
        };
        olink out_obj => obj.obj;
   };
   macro mk_pipe {
       Mesh_Cyl_Unif+Space3+Dim3 pipe<NEportLevels={0,2}> {
          dims = {2,8,8};
          points = {1.,0.,0.,1., 2*acos(-1),1.};
       };
       GDM.DataObject obj {
            in => <-.pipe;
            Modes {
               mode = {0,0,GD_SURF_FLAT,0,0};
               normals = GD_NO_NORMALS;
            };
        };
        olink out_obj => obj.obj;
   };


   //
   // mk_simple_hexprism
   // reduced number of separate cells from 30 to 18
   //
   macro mk_simple_hexprism {

      // size control
      float width = 0.5;
      float length = 1.0;

      Tri  set1 {
         ncells = 12;
         node_connect_list = {
            13, 7, 8,
            13, 8, 9,
            13, 9,10,
            13,10,11,
            13,11,12,
            13,12, 7,
			    0, 1, 2,
             0, 2, 3,
             0, 3, 4,
             0, 4, 5,
             0, 5, 6,
             0, 6, 1
         };
      };

      Quad  set2 {
         ncells = 6;
         node_connect_list = {
             1, 2, 8, 7,
             2, 3, 9, 8,
             3, 4,10, 9,
             4, 5,11,10,
             5, 6,12,11,
             6, 1, 7,12
         };
      };

      Mesh+Space3 hex_mesh {
         nnodes = 14;
         nspace = 3;
         float width => <-.width;
         coordinates {
	         float r1 => 1.7320508 * (<-.width)/2.;
	         float r2 => 0.5000000 * (<-.width);
	         float r3 => 1.0000000 * (<-.width);
            float len => (<-.<-.length) / 2;
	         float values[nnodes][nspace] =>
             {   0,  0,  0,  
			        0,-r1, r2,   0, 0, r3,   0, r1, r2, 
					  0, r1,-r2,   0, 0,-r3,   0,-r1,-r2,
					len,-r1, r2, len, 0, r3, len, r1, r2, 
					len, r1,-r2, len, 0,-r3, len,-r1,-r2,
					len,  0,  0};
         };

         ncell_sets = 2;
         cell_set[ncell_sets] => {set1,set2};
      };

      // std object output configuration

      GDM.DataObject DataObject {
         in => <-.hex_mesh;
         Props.col = {0.7,0.7,0.7};
      };

      olink out_fld => hex_mesh;
      olink out_obj => .DataObject.obj;
   };


   //
   //
   // mk_hexprism. Include PARAMS
   //
   macro mk_hexprism {

      // size control
      float width = 1.2;

      Tri  set1 {
         ncells = 12;
         node_connect_list = {
            25,19,20,
            25,20,21,
            25,21,22,
            25,22,23,
            25,23,24,
            25,24,19,
			    0, 1, 2,
             0, 2, 3,
             0, 3, 4,
             0, 4, 5,
             0, 5, 6,
             0, 6, 1
         };
      };

      Quad  set2 {
         ncells = 18;
         node_connect_list = {
             1, 2, 8, 7,
             2, 3, 9, 8,
             3, 4,10, 9,
             4, 5,11,10,
             5, 6,12,11,
             6, 1, 7,12,
			    7, 8,14,13,
             8, 9,15,14,
             9,10,16,15,
            10,11,17,16,
            11,12,18,17,
            12, 7,13,18,
			   13,14,20,19,
            14,15,21,20,
            15,16,22,21,
            16,17,23,22,
            17,18,24,23,
            18,13,19,24
         };
      };

      Mesh+Space3 hex_mesh {
         nnodes = 26;
         nspace = 3;
         float width => <-.width;
         coordinates {
	         float r1 => 1.7320508 * (<-.width)/2.;
	         float r2 => 0.5000000 * (<-.width);
	         float r3 => 1.0000000 * (<-.width);
	         float values[nnodes][nspace] =>
             { 0,  0,  0,  
			      0,-r1, r2, 0, 0, r3, 0, r1, r2, 
					0, r1,-r2, 0, 0,-r3, 0,-r1,-r2,
				  	1,-r1, r2, 1, 0, r3, 1, r1, r2, 
			   	1, r1,-r2, 1, 0,-r3, 1,-r1,-r2,
					2,-r1, r2, 2, 0, r3, 2, r1, r2, 
					2, r1,-r2, 2, 0,-r3, 2,-r1,-r2,
					3,-r1, r2, 3, 0, r3, 3, r1, r2, 
					3, r1,-r2, 3, 0,-r3, 3,-r1,-r2,
					3,  0,  0};
         };

         ncell_sets = 2;
         cell_set[ncell_sets] => {set1,set2};
      };

      // std object output configuration

      GDM.DataObject DataObject {
         in => <-.hex_mesh;
         Props.col = {0.7,0.7,0.7};
      };

      olink out_fld => hex_mesh;
      olink out_obj => .DataObject.obj;
   };


   //
   //
   // apple.v
   //
   mk_hexprism mk_apple {

      // size control
      width = 0.4;
      float l0 = 0.0;
      float l1 = 0.5;
      float l2 = 0.7;
      float l3 = 0.8;
      float rr0 = 3;

      hex_mesh {
         coordinates {
            float l0 => <-.<-.l0;
            float l1 => <-.<-.l1;
            float l2 => <-.<-.l2;
            float l3 => <-.<-.l3;
            float rr0 => <-.<-.rr0;
            float values[nnodes][nspace] => { 0,  0,  0,
             -r1/rr0, r2/rr0, l0, 0, r3/rr0, l0, r1/rr0, r2/rr0, l0,
              r1/rr0,-r2/rr0, l0, 0,-r3/rr0, l0,-r1/rr0,-r2/rr0, l0,
             -r1, r2, l1, 0, r3, l1, r1, r2,l1,
              r1,-r2, l1, 0,-r3, l1,-r1,-r2,l1,
             -r1, r2, l2, 0, r3, l2, r1, r2, l2,
              r1,-r2, l2, 0,-r3, l2,-r1,-r2,l2,
             -r1/rr0, r2/rr0, l3, 0, r3/rr0, l3, r1/rr0, r2/rr0, l3,
              r1/rr0,-r2/rr0, l3, 0,-r3/rr0, l3,-r1/rr0,-r2/rr0,l3,
              0,0,l3};
         };
      };

      DataObject {
         Props.col = {1.0, 0., 0.};
      };

   }; // end of apple.v


  //  Octahedron
  macro mk_octahedron {
        Tri  set1 {
              ncells = 8;
              node_connect_list = {4,0,1,
				   4,1,2,
				   4,2,3,
				   4,3,0,
				   5,2,1,
				   5,3,2,
				   5,0,3,
				   5,1,0};
        };
        Mesh  glyph2 {
                int nnodes = 6;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {-1,-1,0},
                        { 1,-1,0},
                        { 1, 1,0},
                        {-1, 1,0},
                        { 0, 0, 1.2},
                        { 0, 0, -1.2} };
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



  //  Hyper-Cube
  macro mk_hypercube {
        Quad  set1 {
              ncells = 12;
              node_connect_list = {0,1,9,8,
				   1,2,10,9,
				   2,3,11,10,
				   3,0,8,11,
				   4,5,13,12,
				   5,6,14,13,
				   6,7,15,14,
				   7,4,12,15,
				   1,5,13,9,
				   2,6,14,10,
				   3,7,15,11,
				   0,4,12,8 };
        };
        Mesh  glyph2 {
                int nnodes = 16;
                int nspace = 3;
                coordinates {
                    float values[nvals][veclen] = {
                        {-0.5, -0.5, -0.5},
                        { 0.5, -0.5, -0.5},
                        { 0.5,  0.5, -0.5},
                        {-0.5,  0.5, -0.5},
                        {-0.5, -0.5,  0.5},
                        { 0.5, -0.5,  0.5},
                        { 0.5,  0.5,  0.5},
                        {-0.5,  0.5,  0.5},

                        {-0.4, -0.4, -0.4},
                        { 0.4, -0.4, -0.4},
                        { 0.4,  0.4, -0.4},
                        {-0.4,  0.4, -0.4},
                        {-0.4, -0.4,  0.4},
                        { 0.4, -0.4,  0.4},
                        { 0.4,  0.4,  0.4},
                        {-0.4,  0.4,  0.4} };
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



  macro mk_CubeOctahedron {
	Tri  set1 {
	   	ncells = 8;
	   	node_connect_list =
                  {2,5,0,
                   2,1,5,
                   2,4,1,
                   2,0,4,
                   3,0,5,
                   3,5,1,
                   3,1,4,
                   3,4,0};
	};
   Quad set2 {
         ncells = 6;
         node_connect_list =
                { 6, 9, 8, 7,
                  10, 11, 12, 13,
                  6, 7, 11, 10,
                  7, 8, 12, 11,
                  8, 9, 13, 12,
                  9, 6, 10, 13 };
   };
                  

	Mesh  diamond {
           	int nnodes = 14;
	      	int nspace = 3;
   	      coordinates {
               float dia_sz = 0.5;
               float cube_sz = 0.25;
		         float values[nvals][veclen] = {
		                  {-dia_sz,0,0},
                         {dia_sz,0,0},
                         {0,-dia_sz,0},
                         {0,dia_sz,0},
                         {0,0,-dia_sz},
                         {0,0,dia_sz},

                         {-cube_sz, -cube_sz, -cube_sz },
                         {-cube_sz,  cube_sz, -cube_sz },
                         { cube_sz,  cube_sz, -cube_sz },
                         { cube_sz, -cube_sz, -cube_sz },
                         {-cube_sz, -cube_sz,  cube_sz },
                         {-cube_sz,  cube_sz,  cube_sz },
                         { cube_sz,  cube_sz,  cube_sz },
                         { cube_sz, -cube_sz,  cube_sz }
	      	   };
	      	};
	      	int ncell_sets = 2;
            cell_set[ncell_sets] => {set1, set2};    
	};
	GDM.DataObject obj {
      in => diamond;
		Obj {
			name => name_of(<-.<-.<-);
		};
      Modes {
         mode = {0,0,GD_SURF_FLAT,0,0};
         normals = GD_NO_NORMALS;
      };
	};

	olink out_fld => diamond;
	olink out_obj => obj.obj;
  };


  macro mk_Tet {
	Tri  set1 {
	   	ncells = 4;
	   	node_connect_list =
                  {1,2,0,
                   2,1,3,
                   2,3,0,
                   3,1,0 };
	};
	Mesh  diamond {
  	      	int nnodes = 4;
	      	int nspace = 3;
  	      	coordinates {
               float sz = 1.7321 / 2.0;
	   	      float values[nvals][veclen] = {
		                   {1,-0.5,0.5},
                         {0,sz, 0.5},
                         {-1,-0.5,0.5},
                         {0,0,-sz}
	      	   };
	      	};
	      	int ncell_sets = 1;
            cell_set[ncell_sets] => {set1};    
	};
	GDM.DataObject obj {
     	in => diamond;
		Obj {
			name => name_of(<-.<-.<-);
		};
	};

	olink out_fld => diamond;
	olink out_obj => obj.obj;
  };


  macro mk_TetDual {
	Tri  set1 {
	   	ncells = 8;
	   	node_connect_list =
                  {1,2,0,
                   2,1,3,
                   2,3,0,
                   3,1,0,
                   5,6,4,
                   6,5,7,
                   6,7,4,
                   7,5,4 };
	};
	Mesh  diamond {
  	      	int nnodes = 8;
	      	int nspace = 3;
  	      	coordinates {
               float sz = 1.7321 / 2.0;
	   	      float values[nvals][veclen] = {
		                   {sz,-0.5,0.5},
                         {0,sz, 0.5},
                         {-sz,-0.5,0.5},
                         {0,0,-1},

           		          {sz, 0.5,-0.5},
                         {0,-sz, -0.5},
                         {-sz, 0.5,-0.5},
                         {0,0,1}
	      	      };
	      	};
	      	int ncell_sets = 1;
           	cell_set[ncell_sets] => {set1};    
	};
	GDM.DataObject obj {
     	in => diamond;
		Obj {
			name => name_of(<-.<-.<-);
		};
	};

	olink out_fld => diamond;
	olink out_obj => obj.obj;
  };


  macro mk_TetMulti {
	Tri  set1 {
	   	ncells = 16;
	   	node_connect_list =
                  {1,2,0,
                   2,1,3,
                   2,3,0,
                   3,1,0,
                   5,6,4,
                   6,5,7,
                   6,7,4,
                   7,5,4,

                   9,10,8,
                   10,9,11,
                   10,11,8,
                   11,9,8,
                   13,14,12,
                   14,13,15,
                   14,15,12,
                   14,13,12 };
	};
	Mesh  diamond {
  	      	int nnodes = 16;
	      	int nspace = 3;
  	      	coordinates {
               float sz = 1.7321 / 2.0;
	   	      float values[nvals][veclen] = {
		                   {sz,-0.5,0.5},
                         {0,sz, 0.5},
                         {-sz,-0.5,0.5},
                         {0,0,-1},

		                   {sz, -0.5,-0.5},
                         {0,sz, -0.5},
                         {-sz, -0.5, -0.5},
                         {0,0,1},

                         {-0.5, sz,0.5},
                         {sz, 0, 0.5},
                         {-0.5,-sz,0.5},
                         {0,0, -1},

		                   {-0.5,sz, -0.5},
                         {sz,0, -0.5},
                         {-0.5,-sz,-0.5},
                         {0,0,      1}
	      	      };
	      	};
	      	int ncell_sets = 1;
           	cell_set[ncell_sets] => {set1};    
	};
	GDM.DataObject obj {
     	in => diamond;
		Obj {
			name => name_of(<-.<-.<-);
		};
	};

	olink out_fld => diamond;
	olink out_obj => obj.obj;
  };


 macro mk_dodecahedron {

  olink out_fld => mesh;
  olink out_obj => obj.obj;

  GDM.DataObject obj {
     in => <-.mesh;
  };

  Polytri set1 {
      npolys=1;
      poly_connect_list[2] = {0,1076};
  };

  Mesh  mesh {
   nnodes = 2157;
   nspace = 3;
   ncell_sets = 1;
   cell_set[ncell_sets] => {<-.set1};

   coordinates {
      units = "inches";
      values = {

         0.738053,-0.0241304,0.42226,0.858927,0.239963,0.226082,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.858927,0.239963,0.226082,0.738053,-0.0241304,
0.42226,0.738053,-0.0241304,0.42226,0.738053,-0.0241304,0.42226,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,0.858927,0.239963,0.226082,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.738053,-0.0241304,
0.42226,0.738053,-0.0241304,0.42226,0.894792,-0.161465,0.140456,0.890152,-0.581387,
0.124566,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,0.894792,-0.161465,0.140456,0.738053,-0.0241304,
0.42226,0.738053,-0.0241304,0.42226,0.738053,-0.0241304,0.42226,0.890152,-0.581387,
0.124566,0.890152,-0.581387,0.124566,0.894792,-0.161465,0.140456,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.738053,-0.0241304,
0.42226,0.738053,-0.0241304,0.42226,0.714049,-0.373101,0.444273,0.513941,-0.513786,
0.785994,0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,0.890152,-0.581387,
0.124566,0.890152,-0.581387,0.124566,0.714049,-0.373101,0.444273,0.738053,-0.0241304,
0.42226,0.738053,-0.0241304,0.42226,0.738053,-0.0241304,0.42226,0.513941,-0.513786,
0.785994,0.513941,-0.513786,0.785994,0.714049,-0.373101,0.444273,0.890152,-0.581387,
0.124566,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.738053,-0.0241304,
0.42226,0.738053,-0.0241304,0.42226,0.566479,-0.102471,0.717669,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,0.513941,-0.513786,
0.785994,0.513941,-0.513786,0.785994,0.566479,-0.102471,0.717669,0.738053,-0.0241304,
0.42226,0.738053,-0.0241304,0.42226,0.738053,-0.0241304,0.42226,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.566479,-0.102471,0.717669,0.513941,-0.513786,
0.785994,0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,0.738053,-0.0241304,
0.42226,0.738053,-0.0241304,0.42226,0.656018,0.276423,0.58282,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.656018,0.276423,0.58282,0.738053,-0.0241304,
0.42226,0.738053,-0.0241304,0.42226,0.738053,-0.0241304,0.42226,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.656018,0.276423,0.58282,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,0.151985,0.641657,
0.537387,0.151985,0.641657,0.537387,-0.00736853,0.863411,0.317676,0.246242,1.04122,
0.0336426,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,-0.00736853,0.863411,0.317676,0.151985,0.641657,
0.537387,0.151985,0.641657,0.537387,0.151985,0.641657,0.537387,0.246242,1.04122,
0.0336426,0.246242,1.04122,0.0336426,-0.00736853,0.863411,0.317676,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,0.151985,0.641657,
0.537387,0.151985,0.641657,0.537387,0.393595,0.768591,0.317492,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.246242,1.04122,
0.0336426,0.246242,1.04122,0.0336426,0.393595,0.768591,0.317492,0.151985,0.641657,
0.537387,0.151985,0.641657,0.537387,0.151985,0.641657,0.537387,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.393595,0.768591,0.317492,0.246242,1.04122,
0.0336426,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.151985,0.641657,
0.537387,0.151985,0.641657,0.537387,0.460662,0.498352,0.621195,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.460662,0.498352,0.621195,0.151985,0.641657,
0.537387,0.151985,0.641657,0.537387,0.151985,0.641657,0.537387,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.460662,0.498352,0.621195,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.151985,0.641657,
0.537387,0.151985,0.641657,0.537387,0.101148,0.426156,0.809079,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.295984,0.406308,0.945096,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.101148,0.426156,0.809079,0.151985,0.641657,
0.537387,0.151985,0.641657,0.537387,0.151985,0.641657,0.537387,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,0.101148,0.426156,0.809079,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,0.151985,0.641657,
0.537387,0.151985,0.641657,0.537387,-0.188111,0.651774,0.621494,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.188111,0.651774,0.621494,0.151985,0.641657,
0.537387,0.151985,0.641657,0.537387,0.151985,0.641657,0.537387,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,-0.188111,0.651774,0.621494,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.295984,0.406308,0.945096,0.592462,0.562508,
-0.237027,0.592462,0.562508,-0.237027,0.489934,0.529937,-0.570596,0.68895,0.229327,
-0.786546,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.188389,0.797976,
-0.688216,0.188389,0.797976,-0.688216,0.489934,0.529937,-0.570596,0.592462,0.562508,
-0.237027,0.592462,0.562508,-0.237027,0.592462,0.562508,-0.237027,0.68895,0.229327,
-0.786546,0.68895,0.229327,-0.786546,0.489934,0.529937,-0.570596,0.188389,0.797976,
-0.688216,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.592462,0.562508,
-0.237027,0.592462,0.562508,-0.237027,0.779194,0.304319,-0.383011,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.68895,0.229327,
-0.786546,0.68895,0.229327,-0.786546,0.779194,0.304319,-0.383011,0.592462,0.562508,
-0.237027,0.592462,0.562508,-0.237027,0.592462,0.562508,-0.237027,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,0.779194,0.304319,-0.383011,0.68895,0.229327,
-0.786546,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.592462,0.562508,
-0.237027,0.592462,0.562508,-0.237027,0.810397,0.435509,0.00631977,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,0.810397,0.435509,0.00631977,0.592462,0.562508,
-0.237027,0.592462,0.562508,-0.237027,0.592462,0.562508,-0.237027,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.810397,0.435509,0.00631977,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.592462,0.562508,
-0.237027,0.592462,0.562508,-0.237027,0.540421,0.742208,0.0593536,0.246242,1.04122,
0.0336426,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.540421,0.742208,0.0593536,0.592462,0.562508,
-0.237027,0.592462,0.562508,-0.237027,0.592462,0.562508,-0.237027,0.246242,1.04122,
0.0336426,0.246242,1.04122,0.0336426,0.540421,0.742208,0.0593536,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.592462,0.562508,
-0.237027,0.592462,0.562508,-0.237027,0.342364,0.800568,-0.2972,0.188389,0.797976,
-0.688216,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.246242,1.04122,
0.0336426,0.246242,1.04122,0.0336426,0.342364,0.800568,-0.2972,0.592462,0.562508,
-0.237027,0.592462,0.562508,-0.237027,0.592462,0.562508,-0.237027,0.188389,0.797976,
-0.688216,0.188389,0.797976,-0.688216,0.342364,0.800568,-0.2972,0.246242,1.04122,
0.0336426,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.670318,-0.308921,
-0.422907,0.670318,-0.308921,-0.422907,0.462212,-0.540862,-0.583334,0.420333,
-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,0.462212,-0.540862,-0.583334,
0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,
0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.462212,-0.540862,-0.583334,
0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,
0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,0.660268,-0.599222,-0.22678,
0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,
0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.660268,-0.599222,-0.22678,
0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,
0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.660268,-0.599222,-0.22678,
0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,0.872213,-0.256395,-0.141267,
1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,
0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.872213,-0.256395,-0.141267,
0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,
1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.872213,-0.256395,-0.141267,
0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,
0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,0.805146,0.0138428,-0.444971,
0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,
1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.805146,0.0138428,-0.444971,
0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,
0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.805146,0.0138428,-0.444971,
1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,
0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,0.551751,-0.161967,-0.718183,
0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,
0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.551751,-0.161967,-0.718183,
0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,
0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,0.551751,-0.161967,-0.718183,
0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,
0.277959,-0.768345,0.236627,0.277959,-0.768345,0.236627,-0.052224,-0.86918,0.297067,
-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,-0.052224,
-0.86918,0.297067,0.277959,-0.768345,0.236627,0.277959,-0.768345,0.236627,0.277959,
-0.768345,0.236627,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,
-0.052224,-0.86918,0.297067,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,
-0.0336426,-0.246242,-1.04122,-0.0336426,0.277959,-0.768345,0.236627,0.277959,
-0.768345,0.236627,0.20117,-0.693369,0.570279,0.513941,-0.513786,0.785994,0.513941,
-0.513786,0.785994,0.513941,-0.513786,0.785994,-0.188389,-0.797976,0.688216,-0.188389,
-0.797976,0.688216,0.20117,-0.693369,0.570279,0.277959,-0.768345,0.236627,0.277959,
-0.768345,0.236627,0.277959,-0.768345,0.236627,0.513941,-0.513786,0.785994,0.513941,
-0.513786,0.785994,0.20117,-0.693369,0.570279,-0.188389,-0.797976,0.688216,-0.188389,
-0.797976,0.688216,-0.188389,-0.797976,0.688216,0.277959,-0.768345,0.236627,0.277959,
-0.768345,0.236627,0.560684,-0.621173,0.382396,0.890152,-0.581387,0.124566,0.890152,
-0.581387,0.124566,0.890152,-0.581387,0.124566,0.513941,-0.513786,0.785994,0.513941,
-0.513786,0.785994,0.560684,-0.621173,0.382396,0.277959,-0.768345,0.236627,0.277959,
-0.768345,0.236627,0.277959,-0.768345,0.236627,0.890152,-0.581387,0.124566,0.890152,
-0.581387,0.124566,0.560684,-0.621173,0.382396,0.513941,-0.513786,0.785994,0.513941,
-0.513786,0.785994,0.513941,-0.513786,0.785994,0.277959,-0.768345,0.236627,0.277959,
-0.768345,0.236627,0.529481,-0.752363,-0.00693498,0.420333,-0.907357,-0.381998,
0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.890152,-0.581387,0.124566,
0.890152,-0.581387,0.124566,0.529481,-0.752363,-0.00693498,0.277959,-0.768345,
0.236627,0.277959,-0.768345,0.236627,0.277959,-0.768345,0.236627,0.420333,-0.907357,
-0.381998,0.420333,-0.907357,-0.381998,0.529481,-0.752363,-0.00693498,0.890152,
-0.581387,0.124566,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.277959,
-0.768345,0.236627,0.277959,-0.768345,0.236627,0.150683,-0.905641,-0.0596712,-0.246242,
-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,
0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.150683,-0.905641,-0.0596712,
0.277959,-0.768345,0.236627,0.277959,-0.768345,0.236627,0.277959,-0.768345,0.236627,
-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,0.150683,-0.905641,
-0.0596712,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.420333,
-0.907357,-0.381998,-0.0423876,-0.180856,0.830121,-0.0423876,-0.180856,0.830121,
-0.342441,-0.0012916,0.853921,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.342441,-0.0012916,0.853921,-0.0423876,-0.180856,0.830121,-0.0423876,
-0.180856,0.830121,-0.0423876,-0.180856,0.830121,-0.295984,0.406308,0.945096,
-0.295984,0.406308,0.945096,-0.342441,-0.0012916,0.853921,-0.68895,-0.229327,0.786546,
-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,-0.0423876,-0.180856,0.830121,
-0.0423876,-0.180856,0.830121,0.0363575,0.151985,0.906657,0.447444,0.230502,0.944754,
0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,-0.295984,0.406308,0.945096,
-0.295984,0.406308,0.945096,0.0363575,0.151985,0.906657,-0.0423876,-0.180856,0.830121,
-0.0423876,-0.180856,0.830121,-0.0423876,-0.180856,0.830121,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.0363575,0.151985,0.906657,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.295984,0.406308,0.945096,-0.0423876,-0.180856,
0.830121,-0.0423876,-0.180856,0.830121,0.306332,-0.154713,0.853623,0.513941,-0.513786,
0.785994,0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.306332,-0.154713,0.853623,-0.0423876,-0.180856,
0.830121,-0.0423876,-0.180856,0.830121,-0.0423876,-0.180856,0.830121,0.513941,
-0.513786,0.785994,0.513941,-0.513786,0.785994,0.306332,-0.154713,0.853623,0.447444,
0.230502,0.944754,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,-0.0423876,
-0.180856,0.830121,-0.0423876,-0.180856,0.830121,0.0943881,-0.497539,0.76811,
-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,0.0943881,-0.497539,
0.76811,-0.0423876,-0.180856,0.830121,-0.0423876,-0.180856,0.830121,-0.0423876,
-0.180856,0.830121,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,
0.0943881,-0.497539,0.76811,0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,
0.513941,-0.513786,0.785994,-0.0423876,-0.180856,0.830121,-0.0423876,-0.180856,
0.830121,-0.306576,-0.40272,0.768294,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.68895,-0.229327,0.786546,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,-0.306576,-0.40272,0.768294,-0.0423876,-0.180856,0.830121,-0.0423876,-0.180856,
0.830121,-0.0423876,-0.180856,0.830121,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.306576,-0.40272,0.768294,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,-0.188389,-0.797976,0.688216,-0.670318,0.308921,0.422907,-0.670318,0.308921,
0.422907,-0.551751,0.161967,0.718183,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.68895,-0.229327,0.786546,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.551751,0.161967,0.718183,-0.670318,0.308921,0.422907,-0.670318,0.308921,
0.422907,-0.670318,0.308921,0.422907,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.551751,0.161967,0.718183,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.670318,0.308921,0.422907,-0.670318,0.308921,
0.422907,-0.805146,-0.0138428,0.444971,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-1.05617,-0.121122,0.125459,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.805146,-0.0138428,0.444971,-0.670318,0.308921,0.422907,-0.670318,0.308921,
0.422907,-0.670318,0.308921,0.422907,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-0.805146,-0.0138428,0.444971,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.68895,-0.229327,0.786546,-0.670318,0.308921,0.422907,-0.670318,0.308921,
0.422907,-0.872213,0.256395,0.141267,-0.890152,0.581387,-0.124566,-0.890152,0.581387,
-0.124566,-0.890152,0.581387,-0.124566,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-0.872213,0.256395,0.141267,-0.670318,0.308921,0.422907,-0.670318,0.308921,
0.422907,-0.670318,0.308921,0.422907,-0.890152,0.581387,-0.124566,-0.890152,0.581387,
-0.124566,-0.872213,0.256395,0.141267,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-1.05617,-0.121122,0.125459,-0.670318,0.308921,0.422907,-0.670318,0.308921,
0.422907,-0.660268,0.599222,0.22678,-0.420333,0.907357,0.381998,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,-0.890152,0.581387,-0.124566,-0.890152,0.581387,
-0.124566,-0.660268,0.599222,0.22678,-0.670318,0.308921,0.422907,-0.670318,0.308921,
0.422907,-0.670318,0.308921,0.422907,-0.420333,0.907357,0.381998,-0.420333,0.907357,
0.381998,-0.660268,0.599222,0.22678,-0.890152,0.581387,-0.124566,-0.890152,0.581387,
-0.124566,-0.890152,0.581387,-0.124566,-0.670318,0.308921,0.422907,-0.670318,0.308921,
0.422907,-0.462212,0.540862,0.583334,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.420333,0.907357,0.381998,-0.420333,0.907357,
0.381998,-0.462212,0.540862,0.583334,-0.670318,0.308921,0.422907,-0.670318,0.308921,
0.422907,-0.670318,0.308921,0.422907,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.462212,0.540862,0.583334,-0.420333,0.907357,0.381998,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,-0.592462,-0.562508,0.237027,-0.592462,-0.562508,
0.237027,-0.342364,-0.800568,0.2972,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,
-0.0336426,-0.246242,-1.04122,-0.0336426,-0.188389,-0.797976,0.688216,-0.188389,
-0.797976,0.688216,-0.342364,-0.800568,0.2972,-0.592462,-0.562508,0.237027,-0.592462,
-0.562508,0.237027,-0.592462,-0.562508,0.237027,-0.246242,-1.04122,-0.0336426,-0.246242,
-1.04122,-0.0336426,-0.342364,-0.800568,0.2972,-0.188389,-0.797976,0.688216,-0.188389,
-0.797976,0.688216,-0.188389,-0.797976,0.688216,-0.592462,-0.562508,0.237027,
-0.592462,-0.562508,0.237027,-0.540421,-0.742208,-0.0593536,-0.782558,-0.622897,
-0.381445,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.246242,
-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,-0.540421,-0.742208,-0.0593536,
-0.592462,-0.562508,0.237027,-0.592462,-0.562508,0.237027,-0.592462,-0.562508,
0.237027,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.540421,
-0.742208,-0.0593536,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,
-0.246242,-1.04122,-0.0336426,-0.592462,-0.562508,0.237027,-0.592462,-0.562508,
0.237027,-0.810397,-0.435509,-0.00631978,-1.05617,-0.121122,0.125459,-1.05617,
-0.121122,0.125459,-1.05617,-0.121122,0.125459,-0.782558,-0.622897,-0.381445,-0.782558,
-0.622897,-0.381445,-0.810397,-0.435509,-0.00631978,-0.592462,-0.562508,0.237027,
-0.592462,-0.562508,0.237027,-0.592462,-0.562508,0.237027,-1.05617,-0.121122,0.125459,
-1.05617,-0.121122,0.125459,-0.810397,-0.435509,-0.00631978,-0.782558,-0.622897,
-0.381445,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.592462,
-0.562508,0.237027,-0.592462,-0.562508,0.237027,-0.779194,-0.304319,0.383011,
-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,
-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,-0.779194,-0.304319,0.383011,
-0.592462,-0.562508,0.237027,-0.592462,-0.562508,0.237027,-0.592462,-0.562508,
0.237027,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,-0.779194,-0.304319,
0.383011,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-0.592462,-0.562508,0.237027,-0.592462,-0.562508,0.237027,-0.489934,-0.529937,
0.570596,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,-0.489934,-0.529937,
0.570596,-0.592462,-0.562508,0.237027,-0.592462,-0.562508,0.237027,-0.592462,-0.562508,
0.237027,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,-0.489934,-0.529937,
0.570596,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.151985,-0.641657,-0.537387,-0.151985,-0.641657,-0.537387,0.188111,
-0.651774,-0.621494,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,
0.295984,-0.406308,-0.945096,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
0.188111,-0.651774,-0.621494,-0.151985,-0.641657,-0.537387,-0.151985,-0.641657,
-0.537387,-0.151985,-0.641657,-0.537387,0.295984,-0.406308,-0.945096,0.295984,
-0.406308,-0.945096,0.188111,-0.651774,-0.621494,0.420333,-0.907357,-0.381998,
0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,-0.151985,-0.641657,
-0.537387,-0.151985,-0.641657,-0.537387,-0.101148,-0.426156,-0.809079,-0.447444,
-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,
0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,-0.101148,-0.426156,
-0.809079,-0.151985,-0.641657,-0.537387,-0.151985,-0.641657,-0.537387,-0.151985,
-0.641657,-0.537387,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,
-0.101148,-0.426156,-0.809079,0.295984,-0.406308,-0.945096,0.295984,-0.406308,
-0.945096,0.295984,-0.406308,-0.945096,-0.151985,-0.641657,-0.537387,-0.151985,
-0.641657,-0.537387,-0.460662,-0.498352,-0.621195,-0.782558,-0.622897,-0.381445,
-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.447444,-0.230502,
-0.944754,-0.447444,-0.230502,-0.944754,-0.460662,-0.498352,-0.621195,-0.151985,
-0.641657,-0.537387,-0.151985,-0.641657,-0.537387,-0.151985,-0.641657,-0.537387,
-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.460662,-0.498352,
-0.621195,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.447444,
-0.230502,-0.944754,-0.151985,-0.641657,-0.537387,-0.151985,-0.641657,-0.537387,
-0.393595,-0.768591,-0.317492,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,
-0.0336426,-0.246242,-1.04122,-0.0336426,-0.782558,-0.622897,-0.381445,-0.782558,
-0.622897,-0.381445,-0.393595,-0.768591,-0.317492,-0.151985,-0.641657,-0.537387,
-0.151985,-0.641657,-0.537387,-0.151985,-0.641657,-0.537387,-0.246242,-1.04122,
-0.0336426,-0.246242,-1.04122,-0.0336426,-0.393595,-0.768591,-0.317492,-0.782558,
-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,
-0.151985,-0.641657,-0.537387,-0.151985,-0.641657,-0.537387,0.00736853,-0.863411,
-0.317676,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.420333,
-0.907357,-0.381998,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,
0.00736853,-0.863411,-0.317676,-0.151985,-0.641657,-0.537387,-0.151985,-0.641657,
-0.537387,-0.151985,-0.641657,-0.537387,0.420333,-0.907357,-0.381998,0.420333,
-0.907357,-0.381998,0.00736853,-0.863411,-0.317676,-0.246242,-1.04122,-0.0336426,
-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,0.0423876,0.180856,
-0.830121,0.0423876,0.180856,-0.830121,0.306576,0.40272,-0.768294,0.188389,0.797976,
-0.688216,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.68895,0.229327,
-0.786546,0.68895,0.229327,-0.786546,0.306576,0.40272,-0.768294,0.0423876,0.180856,
-0.830121,0.0423876,0.180856,-0.830121,0.0423876,0.180856,-0.830121,0.188389,
0.797976,-0.688216,0.188389,0.797976,-0.688216,0.306576,0.40272,-0.768294,0.68895,
0.229327,-0.786546,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.0423876,
0.180856,-0.830121,0.0423876,0.180856,-0.830121,-0.0943881,0.497539,-0.76811,
-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.513941,0.513786,
-0.785994,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,-0.0943881,0.497539,
-0.76811,0.0423876,0.180856,-0.830121,0.0423876,0.180856,-0.830121,0.0423876,0.180856,
-0.830121,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.0943881,
0.497539,-0.76811,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.188389,
0.797976,-0.688216,0.0423876,0.180856,-0.830121,0.0423876,0.180856,-0.830121,-0.306332,
0.154713,-0.853623,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,
-0.447444,-0.230502,-0.944754,-0.513941,0.513786,-0.785994,-0.513941,0.513786,
-0.785994,-0.306332,0.154713,-0.853623,0.0423876,0.180856,-0.830121,0.0423876,
0.180856,-0.830121,0.0423876,0.180856,-0.830121,-0.447444,-0.230502,-0.944754,
-0.447444,-0.230502,-0.944754,-0.306332,0.154713,-0.853623,-0.513941,0.513786,
-0.785994,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,0.0423876,
0.180856,-0.830121,0.0423876,0.180856,-0.830121,-0.0363575,-0.151985,-0.906657,
0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,
-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.0363575,-0.151985,
-0.906657,0.0423876,0.180856,-0.830121,0.0423876,0.180856,-0.830121,0.0423876,
0.180856,-0.830121,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,-0.0363575,
-0.151985,-0.906657,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,
-0.447444,-0.230502,-0.944754,0.0423876,0.180856,-0.830121,0.0423876,0.180856,
-0.830121,0.342441,0.0012916,-0.853921,0.68895,0.229327,-0.786546,0.68895,0.229327,
-0.786546,0.68895,0.229327,-0.786546,0.295984,-0.406308,-0.945096,0.295984,-0.406308,
-0.945096,0.342441,0.0012916,-0.853921,0.0423876,0.180856,-0.830121,0.0423876,
0.180856,-0.830121,0.0423876,0.180856,-0.830121,0.68895,0.229327,-0.786546,0.68895,
0.229327,-0.786546,0.342441,0.0012916,-0.853921,0.295984,-0.406308,-0.945096,0.295984,
-0.406308,-0.945096,0.295984,-0.406308,-0.945096,-0.277959,0.768345,-0.236627,
-0.277959,0.768345,-0.236627,-0.150683,0.905641,0.0596712,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,0.246242,1.04122,
0.0336426,0.246242,1.04122,0.0336426,-0.150683,0.905641,0.0596712,-0.277959,0.768345,
-0.236627,-0.277959,0.768345,-0.236627,-0.277959,0.768345,-0.236627,-0.420333,
0.907357,0.381998,-0.420333,0.907357,0.381998,-0.150683,0.905641,0.0596712,0.246242,
1.04122,0.0336426,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,-0.277959,
0.768345,-0.236627,-0.277959,0.768345,-0.236627,-0.529481,0.752363,0.00693498,-0.890152,
0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.420333,
0.907357,0.381998,-0.420333,0.907357,0.381998,-0.529481,0.752363,0.00693498,-0.277959,
0.768345,-0.236627,-0.277959,0.768345,-0.236627,-0.277959,0.768345,-0.236627,-0.890152,
0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.529481,0.752363,0.00693498,-0.420333,
0.907357,0.381998,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,-0.277959,
0.768345,-0.236627,-0.277959,0.768345,-0.236627,-0.560684,0.621173,-0.382396,-0.513941,
0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.890152,
0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.560684,0.621173,-0.382396,-0.277959,
0.768345,-0.236627,-0.277959,0.768345,-0.236627,-0.277959,0.768345,-0.236627,-0.513941,
0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.560684,0.621173,-0.382396,-0.890152,
0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.277959,
0.768345,-0.236627,-0.277959,0.768345,-0.236627,-0.20117,0.693369,-0.570279,0.188389,
0.797976,-0.688216,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,-0.513941,
0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.20117,0.693369,-0.570279,-0.277959,
0.768345,-0.236627,-0.277959,0.768345,-0.236627,-0.277959,0.768345,-0.236627,0.188389,
0.797976,-0.688216,0.188389,0.797976,-0.688216,-0.20117,0.693369,-0.570279,-0.513941,
0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.277959,
0.768345,-0.236627,-0.277959,0.768345,-0.236627,0.052224,0.86918,-0.297067,0.246242,
1.04122,0.0336426,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.188389,
0.797976,-0.688216,0.188389,0.797976,-0.688216,0.052224,0.86918,-0.297067,-0.277959,
0.768345,-0.236627,-0.277959,0.768345,-0.236627,-0.277959,0.768345,-0.236627,0.246242,
1.04122,0.0336426,0.246242,1.04122,0.0336426,0.052224,0.86918,-0.297067,0.188389,
0.797976,-0.688216,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,-0.738053,
0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,-0.656018,-0.276423,-0.58282,
-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,
-0.944754,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.656018,
-0.276423,-0.58282,-0.738053,0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,
-0.738053,0.0241304,-0.42226,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,
-0.944754,-0.656018,-0.276423,-0.58282,-0.782558,-0.622897,-0.381445,-0.782558,
-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.738053,0.0241304,-0.42226,
-0.738053,0.0241304,-0.42226,-0.566479,0.102471,-0.717669,-0.513941,0.513786,
-0.785994,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.447444,
-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.566479,0.102471,-0.717669,
-0.738053,0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,-0.738053,0.0241304,
-0.42226,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.566479,0.102471,
-0.717669,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.447444,
-0.230502,-0.944754,-0.738053,0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,
-0.714049,0.373101,-0.444273,-0.890152,0.581387,-0.124566,-0.890152,0.581387,
-0.124566,-0.890152,0.581387,-0.124566,-0.513941,0.513786,-0.785994,-0.513941,
0.513786,-0.785994,-0.714049,0.373101,-0.444273,-0.738053,0.0241304,-0.42226,
-0.738053,0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,-0.890152,0.581387,
-0.124566,-0.890152,0.581387,-0.124566,-0.714049,0.373101,-0.444273,-0.513941,
0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.738053,
0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,-0.894792,0.161465,-0.140456,-1.05617,
-0.121122,0.125459,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,-0.890152,
0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.894792,0.161465,-0.140456,-0.738053,
0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,
-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,-0.894792,0.161465,-0.140456,
-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.890152,0.581387,
-0.124566,-0.738053,0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,-0.858927,
-0.239963,-0.226082,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,
-0.782558,-0.622897,-0.381445,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,
-0.858927,-0.239963,-0.226082,-0.738053,0.0241304,-0.42226,-0.738053,0.0241304,
-0.42226,-0.738053,0.0241304,-0.42226,-0.782558,-0.622897,-0.381445,-0.782558,
-0.622897,-0.381445,-0.858927,-0.239963,-0.226082,-1.05617,-0.121122,0.125459,
0.738053,-0.0241304,0.42226,0.858927,0.239963,0.226082,0.858927,0.239963,0.226082,
1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.738053,-0.0241304,0.42226,
0.782558,0.622897,0.381445,0.858927,0.239963,0.226082,0.858927,0.239963,0.226082,
0.738053,-0.0241304,0.42226,0.738053,-0.0241304,0.42226,0.782558,0.622897,0.381445,
1.05617,0.121122,-0.125459,0.858927,0.239963,0.226082,0.858927,0.239963,0.226082,
0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,1.05617,0.121122,-0.125459,
0.738053,-0.0241304,0.42226,0.894792,-0.161465,0.140456,0.894792,-0.161465,0.140456,
0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.738053,-0.0241304,0.42226,
1.05617,0.121122,-0.125459,0.894792,-0.161465,0.140456,0.894792,-0.161465,0.140456,
0.738053,-0.0241304,0.42226,0.738053,-0.0241304,0.42226,1.05617,0.121122,-0.125459,
0.890152,-0.581387,0.124566,0.894792,-0.161465,0.140456,0.894792,-0.161465,0.140456,
1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.890152,-0.581387,0.124566,
0.738053,-0.0241304,0.42226,0.714049,-0.373101,0.444273,0.714049,-0.373101,0.444273,
0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,0.738053,-0.0241304,0.42226,
0.890152,-0.581387,0.124566,0.714049,-0.373101,0.444273,0.714049,-0.373101,0.444273,
0.738053,-0.0241304,0.42226,0.738053,-0.0241304,0.42226,0.890152,-0.581387,0.124566,
0.513941,-0.513786,0.785994,0.714049,-0.373101,0.444273,0.714049,-0.373101,0.444273,
0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.513941,-0.513786,0.785994,
0.738053,-0.0241304,0.42226,0.566479,-0.102471,0.717669,0.566479,-0.102471,0.717669,
0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,0.738053,-0.0241304,0.42226,
0.513941,-0.513786,0.785994,0.566479,-0.102471,0.717669,0.566479,-0.102471,0.717669,
0.738053,-0.0241304,0.42226,0.738053,-0.0241304,0.42226,0.513941,-0.513786,0.785994,
0.447444,0.230502,0.944754,0.566479,-0.102471,0.717669,0.566479,-0.102471,0.717669,
0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,0.447444,0.230502,0.944754,
0.738053,-0.0241304,0.42226,0.656018,0.276423,0.58282,0.656018,0.276423,0.58282,
0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.738053,-0.0241304,0.42226,
0.447444,0.230502,0.944754,0.656018,0.276423,0.58282,0.656018,0.276423,0.58282,
0.738053,-0.0241304,0.42226,0.738053,-0.0241304,0.42226,0.447444,0.230502,0.944754,
0.782558,0.622897,0.381445,0.656018,0.276423,0.58282,0.656018,0.276423,0.58282,
0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,0.782558,0.622897,0.381445,
0.151985,0.641657,0.537387,-0.00736853,0.863411,0.317676,-0.00736853,0.863411,
0.317676,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.151985,0.641657,
0.537387,-0.420333,0.907357,0.381998,-0.00736853,0.863411,0.317676,-0.00736853,0.863411,
0.317676,0.151985,0.641657,0.537387,0.151985,0.641657,0.537387,-0.420333,0.907357,
0.381998,0.246242,1.04122,0.0336426,-0.00736853,0.863411,0.317676,-0.00736853,0.863411,
0.317676,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,0.246242,1.04122,
0.0336426,0.151985,0.641657,0.537387,0.393595,0.768591,0.317492,0.393595,0.768591,
0.317492,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.151985,0.641657,
0.537387,0.246242,1.04122,0.0336426,0.393595,0.768591,0.317492,0.393595,0.768591,
0.317492,0.151985,0.641657,0.537387,0.151985,0.641657,0.537387,0.246242,1.04122,
0.0336426,0.782558,0.622897,0.381445,0.393595,0.768591,0.317492,0.393595,0.768591,
0.317492,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.782558,0.622897,
0.381445,0.151985,0.641657,0.537387,0.460662,0.498352,0.621195,0.460662,0.498352,
0.621195,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,0.151985,0.641657,
0.537387,0.782558,0.622897,0.381445,0.460662,0.498352,0.621195,0.460662,0.498352,
0.621195,0.151985,0.641657,0.537387,0.151985,0.641657,0.537387,0.782558,0.622897,
0.381445,0.447444,0.230502,0.944754,0.460662,0.498352,0.621195,0.460662,0.498352,
0.621195,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.447444,0.230502,
0.944754,0.151985,0.641657,0.537387,0.101148,0.426156,0.809079,0.101148,0.426156,
0.809079,-0.295984,0.406308,0.945096,-0.295984,0.406308,0.945096,0.151985,0.641657,
0.537387,0.447444,0.230502,0.944754,0.101148,0.426156,0.809079,0.101148,0.426156,
0.809079,0.151985,0.641657,0.537387,0.151985,0.641657,0.537387,0.447444,0.230502,
0.944754,-0.295984,0.406308,0.945096,0.101148,0.426156,0.809079,0.101148,0.426156,
0.809079,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,-0.295984,0.406308,
0.945096,0.151985,0.641657,0.537387,-0.188111,0.651774,0.621494,-0.188111,0.651774,
0.621494,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,0.151985,0.641657,
0.537387,-0.295984,0.406308,0.945096,-0.188111,0.651774,0.621494,-0.188111,0.651774,
0.621494,0.151985,0.641657,0.537387,0.151985,0.641657,0.537387,-0.295984,0.406308,
0.945096,-0.420333,0.907357,0.381998,-0.188111,0.651774,0.621494,-0.188111,0.651774,
0.621494,-0.295984,0.406308,0.945096,-0.295984,0.406308,0.945096,-0.420333,0.907357,
0.381998,0.592462,0.562508,-0.237027,0.489934,0.529937,-0.570596,0.489934,0.529937,
-0.570596,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.592462,0.562508,
-0.237027,0.188389,0.797976,-0.688216,0.489934,0.529937,-0.570596,0.489934,0.529937,
-0.570596,0.592462,0.562508,-0.237027,0.592462,0.562508,-0.237027,0.188389,0.797976,
-0.688216,0.68895,0.229327,-0.786546,0.489934,0.529937,-0.570596,0.489934,0.529937,
-0.570596,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.68895,0.229327,
-0.786546,0.592462,0.562508,-0.237027,0.779194,0.304319,-0.383011,0.779194,0.304319,
-0.383011,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.592462,0.562508,
-0.237027,0.68895,0.229327,-0.786546,0.779194,0.304319,-0.383011,0.779194,0.304319,
-0.383011,0.592462,0.562508,-0.237027,0.592462,0.562508,-0.237027,0.68895,0.229327,
-0.786546,1.05617,0.121122,-0.125459,0.779194,0.304319,-0.383011,0.779194,0.304319,
-0.383011,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,1.05617,0.121122,
-0.125459,0.592462,0.562508,-0.237027,0.810397,0.435509,0.00631977,0.810397,0.435509,
0.00631977,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.592462,0.562508,
-0.237027,1.05617,0.121122,-0.125459,0.810397,0.435509,0.00631977,0.810397,0.435509,
0.00631977,0.592462,0.562508,-0.237027,0.592462,0.562508,-0.237027,1.05617,0.121122,
-0.125459,0.782558,0.622897,0.381445,0.810397,0.435509,0.00631977,0.810397,0.435509,
0.00631977,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.782558,0.622897,
0.381445,0.592462,0.562508,-0.237027,0.540421,0.742208,0.0593536,0.540421,0.742208,
0.0593536,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.592462,0.562508,
-0.237027,0.782558,0.622897,0.381445,0.540421,0.742208,0.0593536,0.540421,0.742208,
0.0593536,0.592462,0.562508,-0.237027,0.592462,0.562508,-0.237027,0.782558,0.622897,
0.381445,0.246242,1.04122,0.0336426,0.540421,0.742208,0.0593536,0.540421,0.742208,
0.0593536,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.246242,1.04122,
0.0336426,0.592462,0.562508,-0.237027,0.342364,0.800568,-0.2972,0.342364,0.800568,
-0.2972,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.592462,0.562508,
-0.237027,0.246242,1.04122,0.0336426,0.342364,0.800568,-0.2972,0.342364,0.800568,
-0.2972,0.592462,0.562508,-0.237027,0.592462,0.562508,-0.237027,0.246242,1.04122,
0.0336426,0.188389,0.797976,-0.688216,0.342364,0.800568,-0.2972,0.342364,0.800568,
-0.2972,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.188389,0.797976,
-0.688216,0.670318,-0.308921,-0.422907,0.462212,-0.540862,-0.583334,0.462212,
-0.540862,-0.583334,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
0.670318,-0.308921,-0.422907,0.295984,-0.406308,-0.945096,0.462212,-0.540862,-0.583334,
0.462212,-0.540862,-0.583334,0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,
0.295984,-0.406308,-0.945096,0.420333,-0.907357,-0.381998,0.462212,-0.540862,-0.583334,
0.462212,-0.540862,-0.583334,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,
0.420333,-0.907357,-0.381998,0.670318,-0.308921,-0.422907,0.660268,-0.599222,-0.22678,
0.660268,-0.599222,-0.22678,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,
0.670318,-0.308921,-0.422907,0.420333,-0.907357,-0.381998,0.660268,-0.599222,-0.22678,
0.660268,-0.599222,-0.22678,0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,
0.420333,-0.907357,-0.381998,0.890152,-0.581387,0.124566,0.660268,-0.599222,-0.22678,
0.660268,-0.599222,-0.22678,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
0.890152,-0.581387,0.124566,0.670318,-0.308921,-0.422907,0.872213,-0.256395,-0.141267,
0.872213,-0.256395,-0.141267,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,
0.670318,-0.308921,-0.422907,0.890152,-0.581387,0.124566,0.872213,-0.256395,-0.141267,
0.872213,-0.256395,-0.141267,0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,
0.890152,-0.581387,0.124566,1.05617,0.121122,-0.125459,0.872213,-0.256395,-0.141267,
0.872213,-0.256395,-0.141267,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,
1.05617,0.121122,-0.125459,0.670318,-0.308921,-0.422907,0.805146,0.0138428,-0.444971,
0.805146,0.0138428,-0.444971,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,
0.670318,-0.308921,-0.422907,1.05617,0.121122,-0.125459,0.805146,0.0138428,-0.444971,
0.805146,0.0138428,-0.444971,0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,
1.05617,0.121122,-0.125459,0.68895,0.229327,-0.786546,0.805146,0.0138428,-0.444971,
0.805146,0.0138428,-0.444971,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,
0.68895,0.229327,-0.786546,0.670318,-0.308921,-0.422907,0.551751,-0.161967,-0.718183,
0.551751,-0.161967,-0.718183,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,
0.670318,-0.308921,-0.422907,0.68895,0.229327,-0.786546,0.551751,-0.161967,-0.718183,
0.551751,-0.161967,-0.718183,0.670318,-0.308921,-0.422907,0.670318,-0.308921,-0.422907,
0.68895,0.229327,-0.786546,0.295984,-0.406308,-0.945096,0.551751,-0.161967,-0.718183,
0.551751,-0.161967,-0.718183,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,
0.295984,-0.406308,-0.945096,0.277959,-0.768345,0.236627,-0.052224,-0.86918,0.297067,
-0.052224,-0.86918,0.297067,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,
0.277959,-0.768345,0.236627,-0.246242,-1.04122,-0.0336426,-0.052224,-0.86918,
0.297067,-0.052224,-0.86918,0.297067,0.277959,-0.768345,0.236627,0.277959,-0.768345,
0.236627,-0.246242,-1.04122,-0.0336426,-0.188389,-0.797976,0.688216,-0.052224,
-0.86918,0.297067,-0.052224,-0.86918,0.297067,-0.246242,-1.04122,-0.0336426,-0.246242,
-1.04122,-0.0336426,-0.188389,-0.797976,0.688216,0.277959,-0.768345,0.236627,
0.20117,-0.693369,0.570279,0.20117,-0.693369,0.570279,0.513941,-0.513786,0.785994,
0.513941,-0.513786,0.785994,0.277959,-0.768345,0.236627,-0.188389,-0.797976,0.688216,
0.20117,-0.693369,0.570279,0.20117,-0.693369,0.570279,0.277959,-0.768345,0.236627,
0.277959,-0.768345,0.236627,-0.188389,-0.797976,0.688216,0.513941,-0.513786,0.785994,
0.20117,-0.693369,0.570279,0.20117,-0.693369,0.570279,-0.188389,-0.797976,0.688216,
-0.188389,-0.797976,0.688216,0.513941,-0.513786,0.785994,0.277959,-0.768345,0.236627,
0.560684,-0.621173,0.382396,0.560684,-0.621173,0.382396,0.890152,-0.581387,0.124566,
0.890152,-0.581387,0.124566,0.277959,-0.768345,0.236627,0.513941,-0.513786,0.785994,
0.560684,-0.621173,0.382396,0.560684,-0.621173,0.382396,0.277959,-0.768345,0.236627,
0.277959,-0.768345,0.236627,0.513941,-0.513786,0.785994,0.890152,-0.581387,0.124566,
0.560684,-0.621173,0.382396,0.560684,-0.621173,0.382396,0.513941,-0.513786,0.785994,
0.513941,-0.513786,0.785994,0.890152,-0.581387,0.124566,0.277959,-0.768345,0.236627,
0.529481,-0.752363,-0.00693498,0.529481,-0.752363,-0.00693498,0.420333,-0.907357,
-0.381998,0.420333,-0.907357,-0.381998,0.277959,-0.768345,0.236627,0.890152,-0.581387,
0.124566,0.529481,-0.752363,-0.00693498,0.529481,-0.752363,-0.00693498,0.277959,
-0.768345,0.236627,0.277959,-0.768345,0.236627,0.890152,-0.581387,0.124566,0.420333,
-0.907357,-0.381998,0.529481,-0.752363,-0.00693498,0.529481,-0.752363,-0.00693498,
0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.420333,-0.907357,-0.381998,
0.277959,-0.768345,0.236627,0.150683,-0.905641,-0.0596712,0.150683,-0.905641,-0.0596712,
-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,0.277959,-0.768345,
0.236627,0.420333,-0.907357,-0.381998,0.150683,-0.905641,-0.0596712,0.150683,
-0.905641,-0.0596712,0.277959,-0.768345,0.236627,0.277959,-0.768345,0.236627,
0.420333,-0.907357,-0.381998,-0.246242,-1.04122,-0.0336426,0.150683,-0.905641,
-0.0596712,0.150683,-0.905641,-0.0596712,0.420333,-0.907357,-0.381998,0.420333,
-0.907357,-0.381998,-0.246242,-1.04122,-0.0336426,-0.0423876,-0.180856,0.830121,
-0.342441,-0.0012916,0.853921,-0.342441,-0.0012916,0.853921,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.0423876,-0.180856,0.830121,-0.68895,-0.229327,
0.786546,-0.342441,-0.0012916,0.853921,-0.342441,-0.0012916,0.853921,-0.0423876,
-0.180856,0.830121,-0.0423876,-0.180856,0.830121,-0.68895,-0.229327,0.786546,
-0.295984,0.406308,0.945096,-0.342441,-0.0012916,0.853921,-0.342441,-0.0012916,
0.853921,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,-0.295984,0.406308,
0.945096,-0.0423876,-0.180856,0.830121,0.0363575,0.151985,0.906657,0.0363575,0.151985,
0.906657,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,-0.0423876,-0.180856,
0.830121,-0.295984,0.406308,0.945096,0.0363575,0.151985,0.906657,0.0363575,0.151985,
0.906657,-0.0423876,-0.180856,0.830121,-0.0423876,-0.180856,0.830121,-0.295984,
0.406308,0.945096,0.447444,0.230502,0.944754,0.0363575,0.151985,0.906657,0.0363575,
0.151985,0.906657,-0.295984,0.406308,0.945096,-0.295984,0.406308,0.945096,0.447444,
0.230502,0.944754,-0.0423876,-0.180856,0.830121,0.306332,-0.154713,0.853623,0.306332,
-0.154713,0.853623,0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,-0.0423876,
-0.180856,0.830121,0.447444,0.230502,0.944754,0.306332,-0.154713,0.853623,0.306332,
-0.154713,0.853623,-0.0423876,-0.180856,0.830121,-0.0423876,-0.180856,0.830121,
0.447444,0.230502,0.944754,0.513941,-0.513786,0.785994,0.306332,-0.154713,0.853623,
0.306332,-0.154713,0.853623,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,
0.513941,-0.513786,0.785994,-0.0423876,-0.180856,0.830121,0.0943881,-0.497539,
0.76811,0.0943881,-0.497539,0.76811,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,-0.0423876,-0.180856,0.830121,0.513941,-0.513786,0.785994,0.0943881,-0.497539,
0.76811,0.0943881,-0.497539,0.76811,-0.0423876,-0.180856,0.830121,-0.0423876,-0.180856,
0.830121,0.513941,-0.513786,0.785994,-0.188389,-0.797976,0.688216,0.0943881,-0.497539,
0.76811,0.0943881,-0.497539,0.76811,0.513941,-0.513786,0.785994,0.513941,-0.513786,
0.785994,-0.188389,-0.797976,0.688216,-0.0423876,-0.180856,0.830121,-0.306576,
-0.40272,0.768294,-0.306576,-0.40272,0.768294,-0.68895,-0.229327,0.786546,-0.68895,
-0.229327,0.786546,-0.0423876,-0.180856,0.830121,-0.188389,-0.797976,0.688216,
-0.306576,-0.40272,0.768294,-0.306576,-0.40272,0.768294,-0.0423876,-0.180856,0.830121,
-0.0423876,-0.180856,0.830121,-0.188389,-0.797976,0.688216,-0.68895,-0.229327,
0.786546,-0.306576,-0.40272,0.768294,-0.306576,-0.40272,0.768294,-0.188389,-0.797976,
0.688216,-0.188389,-0.797976,0.688216,-0.68895,-0.229327,0.786546,-0.670318,0.308921,
0.422907,-0.551751,0.161967,0.718183,-0.551751,0.161967,0.718183,-0.68895,-0.229327,
0.786546,-0.68895,-0.229327,0.786546,-0.670318,0.308921,0.422907,-0.295984,0.406308,
0.945096,-0.551751,0.161967,0.718183,-0.551751,0.161967,0.718183,-0.670318,0.308921,
0.422907,-0.670318,0.308921,0.422907,-0.295984,0.406308,0.945096,-0.68895,-0.229327,
0.786546,-0.551751,0.161967,0.718183,-0.551751,0.161967,0.718183,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.68895,-0.229327,0.786546,-0.670318,0.308921,
0.422907,-0.805146,-0.0138428,0.444971,-0.805146,-0.0138428,0.444971,-1.05617,
-0.121122,0.125459,-1.05617,-0.121122,0.125459,-0.670318,0.308921,0.422907,-0.68895,
-0.229327,0.786546,-0.805146,-0.0138428,0.444971,-0.805146,-0.0138428,0.444971,
-0.670318,0.308921,0.422907,-0.670318,0.308921,0.422907,-0.68895,-0.229327,0.786546,
-1.05617,-0.121122,0.125459,-0.805146,-0.0138428,0.444971,-0.805146,-0.0138428,
0.444971,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,-1.05617,-0.121122,
0.125459,-0.670318,0.308921,0.422907,-0.872213,0.256395,0.141267,-0.872213,0.256395,
0.141267,-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.670318,0.308921,
0.422907,-1.05617,-0.121122,0.125459,-0.872213,0.256395,0.141267,-0.872213,0.256395,
0.141267,-0.670318,0.308921,0.422907,-0.670318,0.308921,0.422907,-1.05617,-0.121122,
0.125459,-0.890152,0.581387,-0.124566,-0.872213,0.256395,0.141267,-0.872213,0.256395,
0.141267,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,-0.890152,0.581387,
-0.124566,-0.670318,0.308921,0.422907,-0.660268,0.599222,0.22678,-0.660268,0.599222,
0.22678,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,-0.670318,0.308921,
0.422907,-0.890152,0.581387,-0.124566,-0.660268,0.599222,0.22678,-0.660268,0.599222,
0.22678,-0.670318,0.308921,0.422907,-0.670318,0.308921,0.422907,-0.890152,0.581387,
-0.124566,-0.420333,0.907357,0.381998,-0.660268,0.599222,0.22678,-0.660268,0.599222,
0.22678,-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.420333,0.907357,
0.381998,-0.670318,0.308921,0.422907,-0.462212,0.540862,0.583334,-0.462212,0.540862,
0.583334,-0.295984,0.406308,0.945096,-0.295984,0.406308,0.945096,-0.670318,0.308921,
0.422907,-0.420333,0.907357,0.381998,-0.462212,0.540862,0.583334,-0.462212,0.540862,
0.583334,-0.670318,0.308921,0.422907,-0.670318,0.308921,0.422907,-0.420333,0.907357,
0.381998,-0.295984,0.406308,0.945096,-0.462212,0.540862,0.583334,-0.462212,0.540862,
0.583334,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,-0.295984,0.406308,
0.945096,-0.592462,-0.562508,0.237027,-0.342364,-0.800568,0.2972,-0.342364,-0.800568,
0.2972,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,-0.592462,-0.562508,
0.237027,-0.188389,-0.797976,0.688216,-0.342364,-0.800568,0.2972,-0.342364,-0.800568,
0.2972,-0.592462,-0.562508,0.237027,-0.592462,-0.562508,0.237027,-0.188389,-0.797976,
0.688216,-0.246242,-1.04122,-0.0336426,-0.342364,-0.800568,0.2972,-0.342364,-0.800568,
0.2972,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,-0.246242,-1.04122,
-0.0336426,-0.592462,-0.562508,0.237027,-0.540421,-0.742208,-0.0593536,-0.540421,
-0.742208,-0.0593536,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,
-0.592462,-0.562508,0.237027,-0.246242,-1.04122,-0.0336426,-0.540421,-0.742208,
-0.0593536,-0.540421,-0.742208,-0.0593536,-0.592462,-0.562508,0.237027,-0.592462,
-0.562508,0.237027,-0.246242,-1.04122,-0.0336426,-0.782558,-0.622897,-0.381445,
-0.540421,-0.742208,-0.0593536,-0.540421,-0.742208,-0.0593536,-0.246242,-1.04122,
-0.0336426,-0.246242,-1.04122,-0.0336426,-0.782558,-0.622897,-0.381445,-0.592462,
-0.562508,0.237027,-0.810397,-0.435509,-0.00631978,-0.810397,-0.435509,-0.00631978,
-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,-0.592462,-0.562508,0.237027,
-0.782558,-0.622897,-0.381445,-0.810397,-0.435509,-0.00631978,-0.810397,-0.435509,
-0.00631978,-0.592462,-0.562508,0.237027,-0.592462,-0.562508,0.237027,-0.782558,
-0.622897,-0.381445,-1.05617,-0.121122,0.125459,-0.810397,-0.435509,-0.00631978,
-0.810397,-0.435509,-0.00631978,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,
-0.381445,-1.05617,-0.121122,0.125459,-0.592462,-0.562508,0.237027,-0.779194,-0.304319,
0.383011,-0.779194,-0.304319,0.383011,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.592462,-0.562508,0.237027,-1.05617,-0.121122,0.125459,-0.779194,-0.304319,
0.383011,-0.779194,-0.304319,0.383011,-0.592462,-0.562508,0.237027,-0.592462,-0.562508,
0.237027,-1.05617,-0.121122,0.125459,-0.68895,-0.229327,0.786546,-0.779194,-0.304319,
0.383011,-0.779194,-0.304319,0.383011,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-0.68895,-0.229327,0.786546,-0.592462,-0.562508,0.237027,-0.489934,-0.529937,
0.570596,-0.489934,-0.529937,0.570596,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,-0.592462,-0.562508,0.237027,-0.68895,-0.229327,0.786546,-0.489934,-0.529937,
0.570596,-0.489934,-0.529937,0.570596,-0.592462,-0.562508,0.237027,-0.592462,-0.562508,
0.237027,-0.68895,-0.229327,0.786546,-0.188389,-0.797976,0.688216,-0.489934,-0.529937,
0.570596,-0.489934,-0.529937,0.570596,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.188389,-0.797976,0.688216,-0.151985,-0.641657,-0.537387,0.188111,
-0.651774,-0.621494,0.188111,-0.651774,-0.621494,0.295984,-0.406308,-0.945096,
0.295984,-0.406308,-0.945096,-0.151985,-0.641657,-0.537387,0.420333,-0.907357,
-0.381998,0.188111,-0.651774,-0.621494,0.188111,-0.651774,-0.621494,-0.151985,
-0.641657,-0.537387,-0.151985,-0.641657,-0.537387,0.420333,-0.907357,-0.381998,
0.295984,-0.406308,-0.945096,0.188111,-0.651774,-0.621494,0.188111,-0.651774,-0.621494,
0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.295984,-0.406308,-0.945096,
-0.151985,-0.641657,-0.537387,-0.101148,-0.426156,-0.809079,-0.101148,-0.426156,
-0.809079,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.151985,
-0.641657,-0.537387,0.295984,-0.406308,-0.945096,-0.101148,-0.426156,-0.809079,
-0.101148,-0.426156,-0.809079,-0.151985,-0.641657,-0.537387,-0.151985,-0.641657,
-0.537387,0.295984,-0.406308,-0.945096,-0.447444,-0.230502,-0.944754,-0.101148,
-0.426156,-0.809079,-0.101148,-0.426156,-0.809079,0.295984,-0.406308,-0.945096,
0.295984,-0.406308,-0.945096,-0.447444,-0.230502,-0.944754,-0.151985,-0.641657,
-0.537387,-0.460662,-0.498352,-0.621195,-0.460662,-0.498352,-0.621195,-0.782558,
-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.151985,-0.641657,-0.537387,
-0.447444,-0.230502,-0.944754,-0.460662,-0.498352,-0.621195,-0.460662,-0.498352,
-0.621195,-0.151985,-0.641657,-0.537387,-0.151985,-0.641657,-0.537387,-0.447444,
-0.230502,-0.944754,-0.782558,-0.622897,-0.381445,-0.460662,-0.498352,-0.621195,
-0.460662,-0.498352,-0.621195,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,
-0.944754,-0.782558,-0.622897,-0.381445,-0.151985,-0.641657,-0.537387,-0.393595,
-0.768591,-0.317492,-0.393595,-0.768591,-0.317492,-0.246242,-1.04122,-0.0336426,
-0.246242,-1.04122,-0.0336426,-0.151985,-0.641657,-0.537387,-0.782558,-0.622897,
-0.381445,-0.393595,-0.768591,-0.317492,-0.393595,-0.768591,-0.317492,-0.151985,
-0.641657,-0.537387,-0.151985,-0.641657,-0.537387,-0.782558,-0.622897,-0.381445,
-0.246242,-1.04122,-0.0336426,-0.393595,-0.768591,-0.317492,-0.393595,-0.768591,
-0.317492,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.246242,
-1.04122,-0.0336426,-0.151985,-0.641657,-0.537387,0.00736853,-0.863411,-0.317676,
0.00736853,-0.863411,-0.317676,0.420333,-0.907357,-0.381998,0.420333,-0.907357,
-0.381998,-0.151985,-0.641657,-0.537387,-0.246242,-1.04122,-0.0336426,0.00736853,
-0.863411,-0.317676,0.00736853,-0.863411,-0.317676,-0.151985,-0.641657,-0.537387,
-0.151985,-0.641657,-0.537387,-0.246242,-1.04122,-0.0336426,0.420333,-0.907357,
-0.381998,0.00736853,-0.863411,-0.317676,0.00736853,-0.863411,-0.317676,-0.246242,
-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,0.420333,-0.907357,-0.381998,
0.0423876,0.180856,-0.830121,0.306576,0.40272,-0.768294,0.306576,0.40272,-0.768294,
0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.0423876,0.180856,-0.830121,
0.68895,0.229327,-0.786546,0.306576,0.40272,-0.768294,0.306576,0.40272,-0.768294,
0.0423876,0.180856,-0.830121,0.0423876,0.180856,-0.830121,0.68895,0.229327,-0.786546,
0.188389,0.797976,-0.688216,0.306576,0.40272,-0.768294,0.306576,0.40272,-0.768294,
0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.188389,0.797976,-0.688216,
0.0423876,0.180856,-0.830121,-0.0943881,0.497539,-0.76811,-0.0943881,0.497539,
-0.76811,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,0.0423876,0.180856,
-0.830121,0.188389,0.797976,-0.688216,-0.0943881,0.497539,-0.76811,-0.0943881,0.497539,
-0.76811,0.0423876,0.180856,-0.830121,0.0423876,0.180856,-0.830121,0.188389,0.797976,
-0.688216,-0.513941,0.513786,-0.785994,-0.0943881,0.497539,-0.76811,-0.0943881,
0.497539,-0.76811,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,-0.513941,
0.513786,-0.785994,0.0423876,0.180856,-0.830121,-0.306332,0.154713,-0.853623,-0.306332,
0.154713,-0.853623,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,
0.0423876,0.180856,-0.830121,-0.513941,0.513786,-0.785994,-0.306332,0.154713,
-0.853623,-0.306332,0.154713,-0.853623,0.0423876,0.180856,-0.830121,0.0423876,
0.180856,-0.830121,-0.513941,0.513786,-0.785994,-0.447444,-0.230502,-0.944754,
-0.306332,0.154713,-0.853623,-0.306332,0.154713,-0.853623,-0.513941,0.513786,
-0.785994,-0.513941,0.513786,-0.785994,-0.447444,-0.230502,-0.944754,0.0423876,
0.180856,-0.830121,-0.0363575,-0.151985,-0.906657,-0.0363575,-0.151985,-0.906657,
0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,0.0423876,0.180856,
-0.830121,-0.447444,-0.230502,-0.944754,-0.0363575,-0.151985,-0.906657,-0.0363575,
-0.151985,-0.906657,0.0423876,0.180856,-0.830121,0.0423876,0.180856,-0.830121,
-0.447444,-0.230502,-0.944754,0.295984,-0.406308,-0.945096,-0.0363575,-0.151985,
-0.906657,-0.0363575,-0.151985,-0.906657,-0.447444,-0.230502,-0.944754,-0.447444,
-0.230502,-0.944754,0.295984,-0.406308,-0.945096,0.0423876,0.180856,-0.830121,
0.342441,0.0012916,-0.853921,0.342441,0.0012916,-0.853921,0.68895,0.229327,-0.786546,
0.68895,0.229327,-0.786546,0.0423876,0.180856,-0.830121,0.295984,-0.406308,-0.945096,
0.342441,0.0012916,-0.853921,0.342441,0.0012916,-0.853921,0.0423876,0.180856,
-0.830121,0.0423876,0.180856,-0.830121,0.295984,-0.406308,-0.945096,0.68895,
0.229327,-0.786546,0.342441,0.0012916,-0.853921,0.342441,0.0012916,-0.853921,0.295984,
-0.406308,-0.945096,0.295984,-0.406308,-0.945096,0.68895,0.229327,-0.786546,-0.277959,
0.768345,-0.236627,-0.150683,0.905641,0.0596712,-0.150683,0.905641,0.0596712,-0.420333,
0.907357,0.381998,-0.420333,0.907357,0.381998,-0.277959,0.768345,-0.236627,0.246242,
1.04122,0.0336426,-0.150683,0.905641,0.0596712,-0.150683,0.905641,0.0596712,-0.277959,
0.768345,-0.236627,-0.277959,0.768345,-0.236627,0.246242,1.04122,0.0336426,-0.420333,
0.907357,0.381998,-0.150683,0.905641,0.0596712,-0.150683,0.905641,0.0596712,0.246242,
1.04122,0.0336426,0.246242,1.04122,0.0336426,-0.420333,0.907357,0.381998,-0.277959,
0.768345,-0.236627,-0.529481,0.752363,0.00693498,-0.529481,0.752363,0.00693498,
-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.277959,0.768345,
-0.236627,-0.420333,0.907357,0.381998,-0.529481,0.752363,0.00693498,-0.529481,
0.752363,0.00693498,-0.277959,0.768345,-0.236627,-0.277959,0.768345,-0.236627,
-0.420333,0.907357,0.381998,-0.890152,0.581387,-0.124566,-0.529481,0.752363,0.00693498,
-0.529481,0.752363,0.00693498,-0.420333,0.907357,0.381998,-0.420333,0.907357,
0.381998,-0.890152,0.581387,-0.124566,-0.277959,0.768345,-0.236627,-0.560684,0.621173,
-0.382396,-0.560684,0.621173,-0.382396,-0.513941,0.513786,-0.785994,-0.513941,
0.513786,-0.785994,-0.277959,0.768345,-0.236627,-0.890152,0.581387,-0.124566,-0.560684,
0.621173,-0.382396,-0.560684,0.621173,-0.382396,-0.277959,0.768345,-0.236627,-0.277959,
0.768345,-0.236627,-0.890152,0.581387,-0.124566,-0.513941,0.513786,-0.785994,-0.560684,
0.621173,-0.382396,-0.560684,0.621173,-0.382396,-0.890152,0.581387,-0.124566,-0.890152,
0.581387,-0.124566,-0.513941,0.513786,-0.785994,-0.277959,0.768345,-0.236627,-0.20117,
0.693369,-0.570279,-0.20117,0.693369,-0.570279,0.188389,0.797976,-0.688216,0.188389,
0.797976,-0.688216,-0.277959,0.768345,-0.236627,-0.513941,0.513786,-0.785994,-0.20117,
0.693369,-0.570279,-0.20117,0.693369,-0.570279,-0.277959,0.768345,-0.236627,-0.277959,
0.768345,-0.236627,-0.513941,0.513786,-0.785994,0.188389,0.797976,-0.688216,-0.20117,
0.693369,-0.570279,-0.20117,0.693369,-0.570279,-0.513941,0.513786,-0.785994,-0.513941,
0.513786,-0.785994,0.188389,0.797976,-0.688216,-0.277959,0.768345,-0.236627,0.052224,
0.86918,-0.297067,0.052224,0.86918,-0.297067,0.246242,1.04122,0.0336426,0.246242,
1.04122,0.0336426,-0.277959,0.768345,-0.236627,0.188389,0.797976,-0.688216,0.052224,
0.86918,-0.297067,0.052224,0.86918,-0.297067,-0.277959,0.768345,-0.236627,-0.277959,
0.768345,-0.236627,0.188389,0.797976,-0.688216,0.246242,1.04122,0.0336426,0.052224,
0.86918,-0.297067,0.052224,0.86918,-0.297067,0.188389,0.797976,-0.688216,0.188389,
0.797976,-0.688216,0.246242,1.04122,0.0336426,-0.738053,0.0241304,-0.42226,-0.656018,
-0.276423,-0.58282,-0.656018,-0.276423,-0.58282,-0.447444,-0.230502,-0.944754,
-0.447444,-0.230502,-0.944754,-0.738053,0.0241304,-0.42226,-0.782558,-0.622897,
-0.381445,-0.656018,-0.276423,-0.58282,-0.656018,-0.276423,-0.58282,-0.738053,
0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,-0.782558,-0.622897,-0.381445,
-0.447444,-0.230502,-0.944754,-0.656018,-0.276423,-0.58282,-0.656018,-0.276423,
-0.58282,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.447444,
-0.230502,-0.944754,-0.738053,0.0241304,-0.42226,-0.566479,0.102471,-0.717669,
-0.566479,0.102471,-0.717669,-0.513941,0.513786,-0.785994,-0.513941,0.513786,
-0.785994,-0.738053,0.0241304,-0.42226,-0.447444,-0.230502,-0.944754,-0.566479,
0.102471,-0.717669,-0.566479,0.102471,-0.717669,-0.738053,0.0241304,-0.42226,
-0.738053,0.0241304,-0.42226,-0.447444,-0.230502,-0.944754,-0.513941,0.513786,
-0.785994,-0.566479,0.102471,-0.717669,-0.566479,0.102471,-0.717669,-0.447444,
-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.513941,0.513786,-0.785994,
-0.738053,0.0241304,-0.42226,-0.714049,0.373101,-0.444273,-0.714049,0.373101,
-0.444273,-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.738053,
0.0241304,-0.42226,-0.513941,0.513786,-0.785994,-0.714049,0.373101,-0.444273,-0.714049,
0.373101,-0.444273,-0.738053,0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,
-0.513941,0.513786,-0.785994,-0.890152,0.581387,-0.124566,-0.714049,0.373101,
-0.444273,-0.714049,0.373101,-0.444273,-0.513941,0.513786,-0.785994,-0.513941,
0.513786,-0.785994,-0.890152,0.581387,-0.124566,-0.738053,0.0241304,-0.42226,
-0.894792,0.161465,-0.140456,-0.894792,0.161465,-0.140456,-1.05617,-0.121122,0.125459,
-1.05617,-0.121122,0.125459,-0.738053,0.0241304,-0.42226,-0.890152,0.581387,-0.124566,
-0.894792,0.161465,-0.140456,-0.894792,0.161465,-0.140456,-0.738053,0.0241304,
-0.42226,-0.738053,0.0241304,-0.42226,-0.890152,0.581387,-0.124566,-1.05617,-0.121122,
0.125459,-0.894792,0.161465,-0.140456,-0.894792,0.161465,-0.140456,-0.890152,0.581387,
-0.124566,-0.890152,0.581387,-0.124566,-1.05617,-0.121122,0.125459,-0.738053,0.0241304,
-0.42226,-0.858927,-0.239963,-0.226082,-0.858927,-0.239963,-0.226082,-0.782558,
-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.738053,0.0241304,-0.42226,
-1.05617,-0.121122,0.125459,-0.858927,-0.239963,-0.226082,-0.858927,-0.239963,
-0.226082,-0.738053,0.0241304,-0.42226,-0.738053,0.0241304,-0.42226,-1.05617,
-0.121122,0.125459,-0.782558,-0.622897,-0.381445,-0.858927,-0.239963,-0.226082,
-0.858927,-0.239963,-0.226082,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,
-0.782558,-0.622897,-0.381445
      };
    };
  };
 };


 macro mk_dodec_stellated {

  olink out_fld => mesh;
  olink out_obj => obj.obj;

  GDM.DataObject obj {
     in => <-.mesh;
  };

  Polytri set1 {
      npolys=1;
      poly_connect_list[2] = {0,1076};
  };

  Mesh  mesh {
   nnodes = 2157;
   nspace = 3;
   ncell_sets = 1;
   cell_set[ncell_sets] => {<-.set1};

   coordinates {
      units = "inches";
      values = {
         1.60569,-0.0524971,0.918657,1.14814,0.230507,0.391548,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,1.14814,0.230507,0.391548,1.60569,-0.0524971,
0.918657,1.60569,-0.0524971,0.918657,1.60569,-0.0524971,0.918657,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,1.14814,0.230507,0.391548,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,1.60569,-0.0524971,
0.918657,1.60569,-0.0524971,0.918657,1.184,-0.170921,0.305921,0.890152,-0.581387,
0.124566,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,1.184,-0.170921,0.305921,1.60569,-0.0524971,
0.918657,1.60569,-0.0524971,0.918657,1.60569,-0.0524971,0.918657,0.890152,-0.581387,
0.124566,0.890152,-0.581387,0.124566,1.184,-0.170921,0.305921,1.05617,0.121122,
-0.125459,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,1.60569,-0.0524971,
0.918657,1.60569,-0.0524971,0.918657,1.00326,-0.382557,0.609739,0.513941,-0.513786,
0.785994,0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,0.890152,-0.581387,
0.124566,0.890152,-0.581387,0.124566,1.00326,-0.382557,0.609739,1.60569,-0.0524971,
0.918657,1.60569,-0.0524971,0.918657,1.60569,-0.0524971,0.918657,0.513941,-0.513786,
0.785994,0.513941,-0.513786,0.785994,1.00326,-0.382557,0.609739,0.890152,-0.581387,
0.124566,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,1.60569,-0.0524971,
0.918657,1.60569,-0.0524971,0.918657,0.85569,-0.111927,0.883135,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,0.513941,-0.513786,
0.785994,0.513941,-0.513786,0.785994,0.85569,-0.111927,0.883135,1.60569,-0.0524971,
0.918657,1.60569,-0.0524971,0.918657,1.60569,-0.0524971,0.918657,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.85569,-0.111927,0.883135,0.513941,-0.513786,
0.785994,0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,1.60569,-0.0524971,
0.918657,1.60569,-0.0524971,0.918657,0.945229,0.266967,0.748285,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.945229,0.266967,0.748285,1.60569,-0.0524971,
0.918657,1.60569,-0.0524971,0.918657,1.60569,-0.0524971,0.918657,0.782558,0.622897,
0.381445,0.782558,0.622897,0.381445,0.945229,0.266967,0.748285,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,0.330656,1.39597,
1.16912,0.330656,1.39597,1.16912,0.0521883,1.11485,0.528255,0.246242,1.04122,
0.0336426,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,0.0521883,1.11485,0.528255,0.330656,1.39597,
1.16912,0.330656,1.39597,1.16912,0.330656,1.39597,1.16912,0.246242,1.04122,0.0336426,
0.246242,1.04122,0.0336426,0.0521883,1.11485,0.528255,-0.420333,0.907357,0.381998,
-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,0.330656,1.39597,1.16912,
0.330656,1.39597,1.16912,0.453152,1.02003,0.52807,0.782558,0.622897,0.381445,
0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.246242,1.04122,0.0336426,
0.246242,1.04122,0.0336426,0.453152,1.02003,0.52807,0.330656,1.39597,1.16912,
0.330656,1.39597,1.16912,0.330656,1.39597,1.16912,0.782558,0.622897,0.381445,
0.782558,0.622897,0.381445,0.453152,1.02003,0.52807,0.246242,1.04122,0.0336426,
0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.330656,1.39597,1.16912,
0.330656,1.39597,1.16912,0.520219,0.749789,0.831774,0.447444,0.230502,0.944754,
0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,0.782558,0.622897,0.381445,
0.782558,0.622897,0.381445,0.520219,0.749789,0.831774,0.330656,1.39597,1.16912,
0.330656,1.39597,1.16912,0.330656,1.39597,1.16912,0.447444,0.230502,0.944754,
0.447444,0.230502,0.944754,0.520219,0.749789,0.831774,0.782558,0.622897,0.381445,
0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,0.330656,1.39597,1.16912,
0.330656,1.39597,1.16912,0.160705,0.677593,1.01966,-0.295984,0.406308,0.945096,
-0.295984,0.406308,0.945096,-0.295984,0.406308,0.945096,0.447444,0.230502,0.944754,
0.447444,0.230502,0.944754,0.160705,0.677593,1.01966,0.330656,1.39597,1.16912,
0.330656,1.39597,1.16912,0.330656,1.39597,1.16912,-0.295984,0.406308,0.945096,
-0.295984,0.406308,0.945096,0.160705,0.677593,1.01966,0.447444,0.230502,0.944754,
0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,0.330656,1.39597,1.16912,
0.330656,1.39597,1.16912,-0.128554,0.903211,0.832073,-0.420333,0.907357,0.381998,
-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,-0.295984,0.406308,0.945096,
-0.295984,0.406308,0.945096,-0.128554,0.903211,0.832073,0.330656,1.39597,1.16912,
0.330656,1.39597,1.16912,0.330656,1.39597,1.16912,-0.420333,0.907357,0.381998,
-0.420333,0.907357,0.381998,-0.128554,0.903211,0.832073,-0.295984,0.406308,0.945096,
-0.295984,0.406308,0.945096,-0.295984,0.406308,0.945096,1.28894,1.22377,-0.515671,
1.28894,1.22377,-0.515671,0.722094,0.750358,-0.663478,0.68895,0.229327,-0.786546,
0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.188389,0.797976,-0.688216,
0.188389,0.797976,-0.688216,0.722094,0.750358,-0.663478,1.28894,1.22377,-0.515671,
1.28894,1.22377,-0.515671,1.28894,1.22377,-0.515671,0.68895,0.229327,-0.786546,
0.68895,0.229327,-0.786546,0.722094,0.750358,-0.663478,0.188389,0.797976,-0.688216,
0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,1.28894,1.22377,-0.515671,
1.28894,1.22377,-0.515671,1.01135,0.52474,-0.475892,1.05617,0.121122,-0.125459,
1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.68895,0.229327,-0.786546,
0.68895,0.229327,-0.786546,1.01135,0.52474,-0.475892,1.28894,1.22377,-0.515671,
1.28894,1.22377,-0.515671,1.28894,1.22377,-0.515671,1.05617,0.121122,-0.125459,
1.05617,0.121122,-0.125459,1.01135,0.52474,-0.475892,0.68895,0.229327,-0.786546,
0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,1.28894,1.22377,-0.515671,
1.28894,1.22377,-0.515671,1.04256,0.65593,-0.0865617,0.782558,0.622897,0.381445,
0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,1.05617,0.121122,-0.125459,
1.05617,0.121122,-0.125459,1.04256,0.65593,-0.0865617,1.28894,1.22377,-0.515671,
1.28894,1.22377,-0.515671,1.28894,1.22377,-0.515671,0.782558,0.622897,0.381445,
0.782558,0.622897,0.381445,1.04256,0.65593,-0.0865617,1.05617,0.121122,-0.125459,
1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,1.28894,1.22377,-0.515671,
1.28894,1.22377,-0.515671,0.772582,0.962629,-0.0335278,0.246242,1.04122,0.0336426,
0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.782558,0.622897,0.381445,
0.782558,0.622897,0.381445,0.772582,0.962629,-0.0335278,1.28894,1.22377,-0.515671,
1.28894,1.22377,-0.515671,1.28894,1.22377,-0.515671,0.246242,1.04122,0.0336426,
0.246242,1.04122,0.0336426,0.772582,0.962629,-0.0335278,0.782558,0.622897,0.381445,
0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,1.28894,1.22377,-0.515671,
1.28894,1.22377,-0.515671,0.574525,1.02099,-0.390081,0.188389,0.797976,-0.688216,
0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.246242,1.04122,0.0336426,
0.246242,1.04122,0.0336426,0.574525,1.02099,-0.390081,1.28894,1.22377,-0.515671,
1.28894,1.22377,-0.515671,1.28894,1.22377,-0.515671,0.188389,0.797976,-0.688216,
0.188389,0.797976,-0.688216,0.574525,1.02099,-0.390081,0.246242,1.04122,0.0336426,
0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,1.45832,-0.672076,-0.920067,
1.45832,-0.672076,-0.920067,0.72488,-0.661914,-0.749054,0.420333,-0.907357,-0.381998,
0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.295984,-0.406308,-0.945096,
0.295984,-0.406308,-0.945096,0.72488,-0.661914,-0.749054,1.45832,-0.672076,-0.920067,
1.45832,-0.672076,-0.920067,1.45832,-0.672076,-0.920067,0.420333,-0.907357,-0.381998,
0.420333,-0.907357,-0.381998,0.72488,-0.661914,-0.749054,0.295984,-0.406308,-0.945096,
0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,1.45832,-0.672076,-0.920067,
1.45832,-0.672076,-0.920067,0.922936,-0.720273,-0.3925,0.890152,-0.581387,0.124566,
0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.420333,-0.907357,-0.381998,
0.420333,-0.907357,-0.381998,0.922936,-0.720273,-0.3925,1.45832,-0.672076,-0.920067,
1.45832,-0.672076,-0.920067,1.45832,-0.672076,-0.920067,0.890152,-0.581387,0.124566,
0.890152,-0.581387,0.124566,0.922936,-0.720273,-0.3925,0.420333,-0.907357,-0.381998,
0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,1.45832,-0.672076,-0.920067,
1.45832,-0.672076,-0.920067,1.13488,-0.377447,-0.306987,1.05617,0.121122,-0.125459,
1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,0.890152,-0.581387,0.124566,
0.890152,-0.581387,0.124566,1.13488,-0.377447,-0.306987,1.45832,-0.672076,-0.920067,
1.45832,-0.672076,-0.920067,1.45832,-0.672076,-0.920067,1.05617,0.121122,-0.125459,
1.05617,0.121122,-0.125459,1.13488,-0.377447,-0.306987,0.890152,-0.581387,0.124566,
0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,1.45832,-0.672076,-0.920067,
1.45832,-0.672076,-0.920067,1.06781,-0.107209,-0.610691,0.68895,0.229327,-0.786546,
0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,1.05617,0.121122,-0.125459,
1.05617,0.121122,-0.125459,1.06781,-0.107209,-0.610691,1.45832,-0.672076,-0.920067,
1.45832,-0.672076,-0.920067,1.45832,-0.672076,-0.920067,0.68895,0.229327,-0.786546,
0.68895,0.229327,-0.786546,1.06781,-0.107209,-0.610691,1.05617,0.121122,-0.125459,
1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,1.45832,-0.672076,-0.920067,
1.45832,-0.672076,-0.920067,0.814419,-0.283019,-0.883903,0.295984,-0.406308,-0.945096,
0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,0.68895,0.229327,-0.786546,
0.68895,0.229327,-0.786546,0.814419,-0.283019,-0.883903,1.45832,-0.672076,-0.920067,
1.45832,-0.672076,-0.920067,1.45832,-0.672076,-0.920067,0.295984,-0.406308,-0.945096,
0.295984,-0.406308,-0.945096,0.814419,-0.283019,-0.883903,0.68895,0.229327,-0.786546,
0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.604722,-1.67159,0.514796,
0.604722,-1.67159,0.514796,0.0566972,-1.17026,0.38979,-0.188389,-0.797976,0.688216,
-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,-0.246242,-1.04122,
-0.0336426,-0.246242,-1.04122,-0.0336426,0.0566972,-1.17026,0.38979,0.604722,
-1.67159,0.514796,0.604722,-1.67159,0.514796,0.604722,-1.67159,0.514796,-0.188389,
-0.797976,0.688216,-0.188389,-0.797976,0.688216,0.0566972,-1.17026,0.38979,-0.246242,
-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,
0.604722,-1.67159,0.514796,0.604722,-1.67159,0.514796,0.310091,-0.99445,0.663002,
0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,
-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,0.310091,-0.99445,0.663002,
0.604722,-1.67159,0.514796,0.604722,-1.67159,0.514796,0.604722,-1.67159,0.514796,
0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,0.310091,-0.99445,0.663002,
-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,0.604722,-1.67159,0.514796,0.604722,-1.67159,0.514796,0.669605,-0.922254,
0.475119,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.890152,-0.581387,
0.124566,0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,0.669605,-0.922254,
0.475119,0.604722,-1.67159,0.514796,0.604722,-1.67159,0.514796,0.604722,-1.67159,
0.514796,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.669605,-0.922254,
0.475119,0.513941,-0.513786,0.785994,0.513941,-0.513786,0.785994,0.513941,-0.513786,
0.785994,0.604722,-1.67159,0.514796,0.604722,-1.67159,0.514796,0.638402,-1.05344,
0.085788,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.420333,-0.907357,
-0.381998,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.638402,-1.05344,
0.085788,0.604722,-1.67159,0.514796,0.604722,-1.67159,0.514796,0.604722,-1.67159,
0.514796,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.638402,-1.05344,
0.085788,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,0.890152,-0.581387,
0.124566,0.604722,-1.67159,0.514796,0.604722,-1.67159,0.514796,0.259605,-1.20672,
0.0330518,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,-0.246242,
-1.04122,-0.0336426,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
0.259605,-1.20672,0.0330518,0.604722,-1.67159,0.514796,0.604722,-1.67159,0.514796,
0.604722,-1.67159,0.514796,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,
0.259605,-1.20672,0.0330518,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
0.420333,-0.907357,-0.381998,-0.0922174,-0.393465,1.80599,-0.0922174,-0.393465,
1.80599,-0.35905,-0.0721612,1.17921,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.35905,-0.0721612,1.17921,-0.0922174,-0.393465,1.80599,-0.0922174,-0.393465,
1.80599,-0.0922174,-0.393465,1.80599,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.35905,-0.0721612,1.17921,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.68895,-0.229327,0.786546,-0.0922174,-0.393465,1.80599,-0.0922174,-0.393465,
1.80599,0.0197475,0.0811151,1.23195,0.447444,0.230502,0.944754,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,0.0197475,0.0811151,1.23195,-0.0922174,-0.393465,1.80599,-0.0922174,-0.393465,
1.80599,-0.0922174,-0.393465,1.80599,0.447444,0.230502,0.944754,0.447444,0.230502,
0.944754,0.0197475,0.0811151,1.23195,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.0922174,-0.393465,1.80599,-0.0922174,-0.393465,
1.80599,0.289723,-0.225583,1.17891,0.513941,-0.513786,0.785994,0.513941,-0.513786,
0.785994,0.513941,-0.513786,0.785994,0.447444,0.230502,0.944754,0.447444,0.230502,
0.944754,0.289723,-0.225583,1.17891,-0.0922174,-0.393465,1.80599,-0.0922174,-0.393465,
1.80599,-0.0922174,-0.393465,1.80599,0.513941,-0.513786,0.785994,0.513941,-0.513786,
0.785994,0.289723,-0.225583,1.17891,0.447444,0.230502,0.944754,0.447444,0.230502,
0.944754,0.447444,0.230502,0.944754,-0.0922174,-0.393465,1.80599,-0.0922174,-0.393465,
1.80599,0.0777782,-0.568409,1.0934,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,-0.188389,-0.797976,0.688216,0.513941,-0.513786,0.785994,0.513941,-0.513786,
0.785994,0.0777782,-0.568409,1.0934,-0.0922174,-0.393465,1.80599,-0.0922174,-0.393465,
1.80599,-0.0922174,-0.393465,1.80599,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,0.0777782,-0.568409,1.0934,0.513941,-0.513786,0.785994,0.513941,-0.513786,
0.785994,0.513941,-0.513786,0.785994,-0.0922174,-0.393465,1.80599,-0.0922174,-0.393465,
1.80599,-0.323185,-0.473589,1.09358,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.68895,-0.229327,0.786546,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,-0.323185,-0.473589,1.09358,-0.0922174,-0.393465,1.80599,-0.0922174,-0.393465,
1.80599,-0.0922174,-0.393465,1.80599,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.323185,-0.473589,1.09358,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,-0.188389,-0.797976,0.688216,-1.45832,0.672078,0.920063,-1.45832,0.672078,
0.920063,-0.814419,0.28302,0.883902,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.68895,-0.229327,0.786546,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.814419,0.28302,0.883902,-1.45832,0.672078,0.920063,-1.45832,0.672078,
0.920063,-1.45832,0.672078,0.920063,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.814419,0.28302,0.883902,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-1.45832,0.672078,0.920063,-1.45832,0.672078,
0.920063,-1.06781,0.10721,0.610689,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-1.05617,-0.121122,0.125459,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-1.06781,0.10721,0.610689,-1.45832,0.672078,0.920063,-1.45832,0.672078,
0.920063,-1.45832,0.672078,0.920063,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-1.06781,0.10721,0.610689,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.68895,-0.229327,0.786546,-1.45832,0.672078,0.920063,-1.45832,0.672078,
0.920063,-1.13488,0.377448,0.306985,-0.890152,0.581387,-0.124566,-0.890152,0.581387,
-0.124566,-0.890152,0.581387,-0.124566,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-1.13488,0.377448,0.306985,-1.45832,0.672078,0.920063,-1.45832,0.672078,
0.920063,-1.45832,0.672078,0.920063,-0.890152,0.581387,-0.124566,-0.890152,0.581387,
-0.124566,-1.13488,0.377448,0.306985,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-1.05617,-0.121122,0.125459,-1.45832,0.672078,0.920063,-1.45832,0.672078,
0.920063,-0.922936,0.720274,0.392498,-0.420333,0.907357,0.381998,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,-0.890152,0.581387,-0.124566,-0.890152,0.581387,
-0.124566,-0.922936,0.720274,0.392498,-1.45832,0.672078,0.920063,-1.45832,0.672078,
0.920063,-1.45832,0.672078,0.920063,-0.420333,0.907357,0.381998,-0.420333,0.907357,
0.381998,-0.922936,0.720274,0.392498,-0.890152,0.581387,-0.124566,-0.890152,0.581387,
-0.124566,-0.890152,0.581387,-0.124566,-1.45832,0.672078,0.920063,-1.45832,0.672078,
0.920063,-0.72488,0.661914,0.749052,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.420333,0.907357,0.381998,-0.420333,0.907357,
0.381998,-0.72488,0.661914,0.749052,-1.45832,0.672078,0.920063,-1.45832,0.672078,
0.920063,-1.45832,0.672078,0.920063,-0.295984,0.406308,0.945096,-0.295984,0.406308,
0.945096,-0.72488,0.661914,0.749052,-0.420333,0.907357,0.381998,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,
0.515671,-0.574524,-1.02099,0.390082,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,
-0.0336426,-0.246242,-1.04122,-0.0336426,-0.188389,-0.797976,0.688216,-0.188389,
-0.797976,0.688216,-0.574524,-1.02099,0.390082,-1.28894,-1.22378,0.515671,-1.28894,
-1.22378,0.515671,-1.28894,-1.22378,0.515671,-0.246242,-1.04122,-0.0336426,-0.246242,
-1.04122,-0.0336426,-0.574524,-1.02099,0.390082,-0.188389,-0.797976,0.688216,
-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,-1.28894,-1.22378,0.515671,
-1.28894,-1.22378,0.515671,-0.77258,-0.962631,0.0335279,-0.782558,-0.622897,-0.381445,
-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.246242,-1.04122,
-0.0336426,-0.246242,-1.04122,-0.0336426,-0.77258,-0.962631,0.0335279,-1.28894,
-1.22378,0.515671,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,0.515671,-0.782558,
-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.77258,-0.962631,0.0335279,
-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,
-0.0336426,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,0.515671,-1.04256,-0.655932,
0.0865618,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-1.04256,
-0.655932,0.0865618,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,0.515671,-1.28894,
-1.22378,0.515671,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,-1.04256,
-0.655932,0.0865618,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,
-0.782558,-0.622897,-0.381445,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,0.515671,
-1.01135,-0.524742,0.475892,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,
-0.68895,-0.229327,0.786546,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,
-1.01135,-0.524742,0.475892,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,0.515671,
-1.28894,-1.22378,0.515671,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,
-1.01135,-0.524742,0.475892,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,
-1.05617,-0.121122,0.125459,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,0.515671,
-0.722093,-0.75036,0.663478,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,
-0.188389,-0.797976,0.688216,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,
-0.722093,-0.75036,0.663478,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,0.515671,
-1.28894,-1.22378,0.515671,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,
-0.722093,-0.75036,0.663478,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,0.786546,
-0.68895,-0.229327,0.786546,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,-1.16913,
0.128554,-0.90321,-0.832074,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,
0.295984,-0.406308,-0.945096,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
0.128554,-0.90321,-0.832074,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,-1.16913,
-0.330655,-1.39596,-1.16913,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,
0.128554,-0.90321,-0.832074,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
0.420333,-0.907357,-0.381998,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,-1.16913,
-0.160705,-0.677592,-1.01966,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,
-0.944754,-0.447444,-0.230502,-0.944754,0.295984,-0.406308,-0.945096,0.295984,
-0.406308,-0.945096,-0.160705,-0.677592,-1.01966,-0.330655,-1.39596,-1.16913,
-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,-1.16913,-0.447444,-0.230502,-0.944754,
-0.447444,-0.230502,-0.944754,-0.160705,-0.677592,-1.01966,0.295984,-0.406308,
-0.945096,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,-0.330655,
-1.39596,-1.16913,-0.330655,-1.39596,-1.16913,-0.520219,-0.749788,-0.831776,-0.782558,
-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,
-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.520219,-0.749788,
-0.831776,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,
-1.16913,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.520219,
-0.749788,-0.831776,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,
-0.447444,-0.230502,-0.944754,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,
-1.16913,-0.453152,-1.02003,-0.528072,-0.246242,-1.04122,-0.0336426,-0.246242,
-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,-0.782558,-0.622897,-0.381445,
-0.782558,-0.622897,-0.381445,-0.453152,-1.02003,-0.528072,-0.330655,-1.39596,
-1.16913,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,-1.16913,-0.246242,-1.04122,
-0.0336426,-0.246242,-1.04122,-0.0336426,-0.453152,-1.02003,-0.528072,-0.782558,
-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,
-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,-1.16913,-0.0521879,-1.11485,-0.528256,
0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,-0.0521879,-1.11485,
-0.528256,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,
-1.16913,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,-0.0521879,-1.11485,
-0.528256,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,-0.246242,
-1.04122,-0.0336426,0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,0.323185,
0.47359,-1.09358,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.188389,
0.797976,-0.688216,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.323185,
0.47359,-1.09358,0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,0.0922167,
0.393466,-1.80599,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.323185,
0.47359,-1.09358,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.68895,
0.229327,-0.786546,0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,-0.0777784,
0.568409,-1.0934,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.513941,
0.513786,-0.785994,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,-0.0777784,
0.568409,-1.0934,0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,0.0922167,
0.393466,-1.80599,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.0777784,
0.568409,-1.0934,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.188389,
0.797976,-0.688216,0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,-0.289723,
0.225583,-1.17891,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,
-0.447444,-0.230502,-0.944754,-0.513941,0.513786,-0.785994,-0.513941,0.513786,
-0.785994,-0.289723,0.225583,-1.17891,0.0922167,0.393466,-1.80599,0.0922167,0.393466,
-1.80599,0.0922167,0.393466,-1.80599,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,
-0.944754,-0.289723,0.225583,-1.17891,-0.513941,0.513786,-0.785994,-0.513941,0.513786,
-0.785994,-0.513941,0.513786,-0.785994,0.0922167,0.393466,-1.80599,0.0922167,0.393466,
-1.80599,-0.0197478,-0.0811147,-1.23195,0.295984,-0.406308,-0.945096,0.295984,
-0.406308,-0.945096,0.295984,-0.406308,-0.945096,-0.447444,-0.230502,-0.944754,
-0.447444,-0.230502,-0.944754,-0.0197478,-0.0811147,-1.23195,0.0922167,0.393466,
-1.80599,0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,0.295984,-0.406308,
-0.945096,0.295984,-0.406308,-0.945096,-0.0197478,-0.0811147,-1.23195,-0.447444,
-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,
0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,0.35905,0.0721616,-1.17921,
0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,
0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,0.35905,0.0721616,-1.17921,
0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,
0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.35905,0.0721616,-1.17921,
0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,
-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.259603,1.20672,-0.0330543,
-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,
0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,-0.259603,1.20672,-0.0330543,
-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,
-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,-0.259603,1.20672,-0.0330543,
0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,
-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.638401,1.05344,-0.0857906,
-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.890152,0.581387,
-0.124566,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,-0.638401,1.05344,
-0.0857906,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.604718,1.67159,
-0.514804,-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.638401,
1.05344,-0.0857906,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,-0.420333,
0.907357,0.381998,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.669604,
0.922253,-0.475121,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.513941,
0.513786,-0.785994,-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.669604,
0.922253,-0.475121,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.604718,
1.67159,-0.514804,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.669604,
0.922253,-0.475121,-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.890152,
0.581387,-0.124566,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.31009,
0.99445,-0.663005,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.188389,
0.797976,-0.688216,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.31009,
0.99445,-0.663005,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.604718,
1.67159,-0.514804,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,-0.31009,
0.99445,-0.663005,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.513941,
0.513786,-0.785994,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.0566957,
1.17026,-0.389792,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,0.246242,
1.04122,0.0336426,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,-0.0566957,
1.17026,-0.389792,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.604718,
1.67159,-0.514804,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,-0.0566957,
1.17026,-0.389792,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.188389,
0.797976,-0.688216,-1.60568,0.0524988,-0.918662,-1.60568,0.0524988,-0.918662,-0.945228,
-0.266967,-0.748287,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,
-0.447444,-0.230502,-0.944754,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,
-0.381445,-0.945228,-0.266967,-0.748287,-1.60568,0.0524988,-0.918662,-1.60568,
0.0524988,-0.918662,-1.60568,0.0524988,-0.918662,-0.447444,-0.230502,-0.944754,
-0.447444,-0.230502,-0.944754,-0.945228,-0.266967,-0.748287,-0.782558,-0.622897,
-0.381445,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-1.60568,
0.0524988,-0.918662,-1.60568,0.0524988,-0.918662,-0.855689,0.111928,-0.883137,
-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.513941,0.513786,
-0.785994,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.855689,
0.111928,-0.883137,-1.60568,0.0524988,-0.918662,-1.60568,0.0524988,-0.918662,-1.60568,
0.0524988,-0.918662,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,
-0.855689,0.111928,-0.883137,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,
-0.944754,-0.447444,-0.230502,-0.944754,-1.60568,0.0524988,-0.918662,-1.60568,
0.0524988,-0.918662,-1.00326,0.382557,-0.609741,-0.890152,0.581387,-0.124566,-0.890152,
0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.513941,0.513786,-0.785994,-0.513941,
0.513786,-0.785994,-1.00326,0.382557,-0.609741,-1.60568,0.0524988,-0.918662,-1.60568,
0.0524988,-0.918662,-1.60568,0.0524988,-0.918662,-0.890152,0.581387,-0.124566,
-0.890152,0.581387,-0.124566,-1.00326,0.382557,-0.609741,-0.513941,0.513786,-0.785994,
-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-1.60568,0.0524988,-0.918662,
-1.60568,0.0524988,-0.918662,-1.184,0.170921,-0.305923,-1.05617,-0.121122,0.125459,
-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,-0.890152,0.581387,-0.124566,
-0.890152,0.581387,-0.124566,-1.184,0.170921,-0.305923,-1.60568,0.0524988,-0.918662,
-1.60568,0.0524988,-0.918662,-1.60568,0.0524988,-0.918662,-1.05617,-0.121122,0.125459,
-1.05617,-0.121122,0.125459,-1.184,0.170921,-0.305923,-0.890152,0.581387,-0.124566,
-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-1.60568,0.0524988,-0.918662,
-1.60568,0.0524988,-0.918662,-1.14814,-0.230507,-0.391549,-0.782558,-0.622897,
-0.381445,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-1.05617,
-0.121122,0.125459,-1.05617,-0.121122,0.125459,-1.14814,-0.230507,-0.391549,-1.60568,
0.0524988,-0.918662,-1.60568,0.0524988,-0.918662,-1.60568,0.0524988,-0.918662,
-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-1.14814,-0.230507,
-0.391549,-1.05617,-0.121122,0.125459,1.60569,-0.0524971,0.918657,1.14814,0.230507,
0.391548,1.14814,0.230507,0.391548,1.05617,0.121122,-0.125459,1.05617,0.121122,
-0.125459,1.60569,-0.0524971,0.918657,0.782558,0.622897,0.381445,1.14814,0.230507,
0.391548,1.14814,0.230507,0.391548,1.60569,-0.0524971,0.918657,1.60569,-0.0524971,
0.918657,0.782558,0.622897,0.381445,1.05617,0.121122,-0.125459,1.14814,0.230507,
0.391548,1.14814,0.230507,0.391548,0.782558,0.622897,0.381445,0.782558,0.622897,
0.381445,1.05617,0.121122,-0.125459,1.60569,-0.0524971,0.918657,1.184,-0.170921,
0.305921,1.184,-0.170921,0.305921,0.890152,-0.581387,0.124566,0.890152,-0.581387,
0.124566,1.60569,-0.0524971,0.918657,1.05617,0.121122,-0.125459,1.184,-0.170921,
0.305921,1.184,-0.170921,0.305921,1.60569,-0.0524971,0.918657,1.60569,-0.0524971,
0.918657,1.05617,0.121122,-0.125459,0.890152,-0.581387,0.124566,1.184,-0.170921,
0.305921,1.184,-0.170921,0.305921,1.05617,0.121122,-0.125459,1.05617,0.121122,
-0.125459,0.890152,-0.581387,0.124566,1.60569,-0.0524971,0.918657,1.00326,-0.382557,
0.609739,1.00326,-0.382557,0.609739,0.513941,-0.513786,0.785994,0.513941,-0.513786,
0.785994,1.60569,-0.0524971,0.918657,0.890152,-0.581387,0.124566,1.00326,-0.382557,
0.609739,1.00326,-0.382557,0.609739,1.60569,-0.0524971,0.918657,1.60569,-0.0524971,
0.918657,0.890152,-0.581387,0.124566,0.513941,-0.513786,0.785994,1.00326,-0.382557,
0.609739,1.00326,-0.382557,0.609739,0.890152,-0.581387,0.124566,0.890152,-0.581387,
0.124566,0.513941,-0.513786,0.785994,1.60569,-0.0524971,0.918657,0.85569,-0.111927,
0.883135,0.85569,-0.111927,0.883135,0.447444,0.230502,0.944754,0.447444,0.230502,
0.944754,1.60569,-0.0524971,0.918657,0.513941,-0.513786,0.785994,0.85569,-0.111927,
0.883135,0.85569,-0.111927,0.883135,1.60569,-0.0524971,0.918657,1.60569,-0.0524971,
0.918657,0.513941,-0.513786,0.785994,0.447444,0.230502,0.944754,0.85569,-0.111927,
0.883135,0.85569,-0.111927,0.883135,0.513941,-0.513786,0.785994,0.513941,-0.513786,
0.785994,0.447444,0.230502,0.944754,1.60569,-0.0524971,0.918657,0.945229,0.266967,
0.748285,0.945229,0.266967,0.748285,0.782558,0.622897,0.381445,0.782558,0.622897,
0.381445,1.60569,-0.0524971,0.918657,0.447444,0.230502,0.944754,0.945229,0.266967,
0.748285,0.945229,0.266967,0.748285,1.60569,-0.0524971,0.918657,1.60569,-0.0524971,
0.918657,0.447444,0.230502,0.944754,0.782558,0.622897,0.381445,0.945229,0.266967,
0.748285,0.945229,0.266967,0.748285,0.447444,0.230502,0.944754,0.447444,0.230502,
0.944754,0.782558,0.622897,0.381445,0.330656,1.39597,1.16912,0.0521883,1.11485,
0.528255,0.0521883,1.11485,0.528255,0.246242,1.04122,0.0336426,0.246242,1.04122,
0.0336426,0.330656,1.39597,1.16912,-0.420333,0.907357,0.381998,0.0521883,1.11485,
0.528255,0.0521883,1.11485,0.528255,0.330656,1.39597,1.16912,0.330656,1.39597,
1.16912,-0.420333,0.907357,0.381998,0.246242,1.04122,0.0336426,0.0521883,1.11485,
0.528255,0.0521883,1.11485,0.528255,-0.420333,0.907357,0.381998,-0.420333,0.907357,
0.381998,0.246242,1.04122,0.0336426,0.330656,1.39597,1.16912,0.453152,1.02003,
0.52807,0.453152,1.02003,0.52807,0.782558,0.622897,0.381445,0.782558,0.622897,
0.381445,0.330656,1.39597,1.16912,0.246242,1.04122,0.0336426,0.453152,1.02003,
0.52807,0.453152,1.02003,0.52807,0.330656,1.39597,1.16912,0.330656,1.39597,1.16912,
0.246242,1.04122,0.0336426,0.782558,0.622897,0.381445,0.453152,1.02003,0.52807,
0.453152,1.02003,0.52807,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,
0.782558,0.622897,0.381445,0.330656,1.39597,1.16912,0.520219,0.749789,0.831774,
0.520219,0.749789,0.831774,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,
0.330656,1.39597,1.16912,0.782558,0.622897,0.381445,0.520219,0.749789,0.831774,
0.520219,0.749789,0.831774,0.330656,1.39597,1.16912,0.330656,1.39597,1.16912,
0.782558,0.622897,0.381445,0.447444,0.230502,0.944754,0.520219,0.749789,0.831774,
0.520219,0.749789,0.831774,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,
0.447444,0.230502,0.944754,0.330656,1.39597,1.16912,0.160705,0.677593,1.01966,
0.160705,0.677593,1.01966,-0.295984,0.406308,0.945096,-0.295984,0.406308,0.945096,
0.330656,1.39597,1.16912,0.447444,0.230502,0.944754,0.160705,0.677593,1.01966,
0.160705,0.677593,1.01966,0.330656,1.39597,1.16912,0.330656,1.39597,1.16912,
0.447444,0.230502,0.944754,-0.295984,0.406308,0.945096,0.160705,0.677593,1.01966,
0.160705,0.677593,1.01966,0.447444,0.230502,0.944754,0.447444,0.230502,0.944754,
-0.295984,0.406308,0.945096,0.330656,1.39597,1.16912,-0.128554,0.903211,0.832073,
-0.128554,0.903211,0.832073,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,
0.330656,1.39597,1.16912,-0.295984,0.406308,0.945096,-0.128554,0.903211,0.832073,
-0.128554,0.903211,0.832073,0.330656,1.39597,1.16912,0.330656,1.39597,1.16912,
-0.295984,0.406308,0.945096,-0.420333,0.907357,0.381998,-0.128554,0.903211,0.832073,
-0.128554,0.903211,0.832073,-0.295984,0.406308,0.945096,-0.295984,0.406308,0.945096,
-0.420333,0.907357,0.381998,1.28894,1.22377,-0.515671,0.722094,0.750358,-0.663478,
0.722094,0.750358,-0.663478,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,
1.28894,1.22377,-0.515671,0.188389,0.797976,-0.688216,0.722094,0.750358,-0.663478,
0.722094,0.750358,-0.663478,1.28894,1.22377,-0.515671,1.28894,1.22377,-0.515671,
0.188389,0.797976,-0.688216,0.68895,0.229327,-0.786546,0.722094,0.750358,-0.663478,
0.722094,0.750358,-0.663478,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,
0.68895,0.229327,-0.786546,1.28894,1.22377,-0.515671,1.01135,0.52474,-0.475892,
1.01135,0.52474,-0.475892,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,
1.28894,1.22377,-0.515671,0.68895,0.229327,-0.786546,1.01135,0.52474,-0.475892,
1.01135,0.52474,-0.475892,1.28894,1.22377,-0.515671,1.28894,1.22377,-0.515671,
0.68895,0.229327,-0.786546,1.05617,0.121122,-0.125459,1.01135,0.52474,-0.475892,
1.01135,0.52474,-0.475892,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,
1.05617,0.121122,-0.125459,1.28894,1.22377,-0.515671,1.04256,0.65593,-0.0865617,
1.04256,0.65593,-0.0865617,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,
1.28894,1.22377,-0.515671,1.05617,0.121122,-0.125459,1.04256,0.65593,-0.0865617,
1.04256,0.65593,-0.0865617,1.28894,1.22377,-0.515671,1.28894,1.22377,-0.515671,
1.05617,0.121122,-0.125459,0.782558,0.622897,0.381445,1.04256,0.65593,-0.0865617,
1.04256,0.65593,-0.0865617,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,
0.782558,0.622897,0.381445,1.28894,1.22377,-0.515671,0.772582,0.962629,-0.0335278,
0.772582,0.962629,-0.0335278,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,
1.28894,1.22377,-0.515671,0.782558,0.622897,0.381445,0.772582,0.962629,-0.0335278,
0.772582,0.962629,-0.0335278,1.28894,1.22377,-0.515671,1.28894,1.22377,-0.515671,
0.782558,0.622897,0.381445,0.246242,1.04122,0.0336426,0.772582,0.962629,-0.0335278,
0.772582,0.962629,-0.0335278,0.782558,0.622897,0.381445,0.782558,0.622897,0.381445,
0.246242,1.04122,0.0336426,1.28894,1.22377,-0.515671,0.574525,1.02099,-0.390081,
0.574525,1.02099,-0.390081,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,
1.28894,1.22377,-0.515671,0.246242,1.04122,0.0336426,0.574525,1.02099,-0.390081,
0.574525,1.02099,-0.390081,1.28894,1.22377,-0.515671,1.28894,1.22377,-0.515671,
0.246242,1.04122,0.0336426,0.188389,0.797976,-0.688216,0.574525,1.02099,-0.390081,
0.574525,1.02099,-0.390081,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,
0.188389,0.797976,-0.688216,1.45832,-0.672076,-0.920067,0.72488,-0.661914,-0.749054,
0.72488,-0.661914,-0.749054,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
1.45832,-0.672076,-0.920067,0.295984,-0.406308,-0.945096,0.72488,-0.661914,-0.749054,
0.72488,-0.661914,-0.749054,1.45832,-0.672076,-0.920067,1.45832,-0.672076,-0.920067,
0.295984,-0.406308,-0.945096,0.420333,-0.907357,-0.381998,0.72488,-0.661914,-0.749054,
0.72488,-0.661914,-0.749054,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,
0.420333,-0.907357,-0.381998,1.45832,-0.672076,-0.920067,0.922936,-0.720273,-0.3925,
0.922936,-0.720273,-0.3925,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,
1.45832,-0.672076,-0.920067,0.420333,-0.907357,-0.381998,0.922936,-0.720273,-0.3925,
0.922936,-0.720273,-0.3925,1.45832,-0.672076,-0.920067,1.45832,-0.672076,-0.920067,
0.420333,-0.907357,-0.381998,0.890152,-0.581387,0.124566,0.922936,-0.720273,-0.3925,
0.922936,-0.720273,-0.3925,0.420333,-0.907357,-0.381998,0.420333,-0.907357,-0.381998,
0.890152,-0.581387,0.124566,1.45832,-0.672076,-0.920067,1.13488,-0.377447,-0.306987,
1.13488,-0.377447,-0.306987,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,
1.45832,-0.672076,-0.920067,0.890152,-0.581387,0.124566,1.13488,-0.377447,-0.306987,
1.13488,-0.377447,-0.306987,1.45832,-0.672076,-0.920067,1.45832,-0.672076,-0.920067,
0.890152,-0.581387,0.124566,1.05617,0.121122,-0.125459,1.13488,-0.377447,-0.306987,
1.13488,-0.377447,-0.306987,0.890152,-0.581387,0.124566,0.890152,-0.581387,0.124566,
1.05617,0.121122,-0.125459,1.45832,-0.672076,-0.920067,1.06781,-0.107209,-0.610691,
1.06781,-0.107209,-0.610691,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,
1.45832,-0.672076,-0.920067,1.05617,0.121122,-0.125459,1.06781,-0.107209,-0.610691,
1.06781,-0.107209,-0.610691,1.45832,-0.672076,-0.920067,1.45832,-0.672076,-0.920067,
1.05617,0.121122,-0.125459,0.68895,0.229327,-0.786546,1.06781,-0.107209,-0.610691,
1.06781,-0.107209,-0.610691,1.05617,0.121122,-0.125459,1.05617,0.121122,-0.125459,
0.68895,0.229327,-0.786546,1.45832,-0.672076,-0.920067,0.814419,-0.283019,-0.883903,
0.814419,-0.283019,-0.883903,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,
1.45832,-0.672076,-0.920067,0.68895,0.229327,-0.786546,0.814419,-0.283019,-0.883903,
0.814419,-0.283019,-0.883903,1.45832,-0.672076,-0.920067,1.45832,-0.672076,-0.920067,
0.68895,0.229327,-0.786546,0.295984,-0.406308,-0.945096,0.814419,-0.283019,-0.883903,
0.814419,-0.283019,-0.883903,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,
0.295984,-0.406308,-0.945096,0.604722,-1.67159,0.514796,0.0566972,-1.17026,0.38979,
0.0566972,-1.17026,0.38979,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,0.688216,
0.604722,-1.67159,0.514796,-0.246242,-1.04122,-0.0336426,0.0566972,-1.17026,0.38979,
0.0566972,-1.17026,0.38979,0.604722,-1.67159,0.514796,0.604722,-1.67159,0.514796,
-0.246242,-1.04122,-0.0336426,-0.188389,-0.797976,0.688216,0.0566972,-1.17026,
0.38979,0.0566972,-1.17026,0.38979,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,
-0.0336426,-0.188389,-0.797976,0.688216,0.604722,-1.67159,0.514796,0.310091,-0.99445,
0.663002,0.310091,-0.99445,0.663002,0.513941,-0.513786,0.785994,0.513941,-0.513786,
0.785994,0.604722,-1.67159,0.514796,-0.188389,-0.797976,0.688216,0.310091,-0.99445,
0.663002,0.310091,-0.99445,0.663002,0.604722,-1.67159,0.514796,0.604722,-1.67159,
0.514796,-0.188389,-0.797976,0.688216,0.513941,-0.513786,0.785994,0.310091,-0.99445,
0.663002,0.310091,-0.99445,0.663002,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,0.513941,-0.513786,0.785994,0.604722,-1.67159,0.514796,0.669605,-0.922254,
0.475119,0.669605,-0.922254,0.475119,0.890152,-0.581387,0.124566,0.890152,-0.581387,
0.124566,0.604722,-1.67159,0.514796,0.513941,-0.513786,0.785994,0.669605,-0.922254,
0.475119,0.669605,-0.922254,0.475119,0.604722,-1.67159,0.514796,0.604722,-1.67159,
0.514796,0.513941,-0.513786,0.785994,0.890152,-0.581387,0.124566,0.669605,-0.922254,
0.475119,0.669605,-0.922254,0.475119,0.513941,-0.513786,0.785994,0.513941,-0.513786,
0.785994,0.890152,-0.581387,0.124566,0.604722,-1.67159,0.514796,0.638402,-1.05344,
0.085788,0.638402,-1.05344,0.085788,0.420333,-0.907357,-0.381998,0.420333,-0.907357,
-0.381998,0.604722,-1.67159,0.514796,0.890152,-0.581387,0.124566,0.638402,-1.05344,
0.085788,0.638402,-1.05344,0.085788,0.604722,-1.67159,0.514796,0.604722,-1.67159,
0.514796,0.890152,-0.581387,0.124566,0.420333,-0.907357,-0.381998,0.638402,-1.05344,
0.085788,0.638402,-1.05344,0.085788,0.890152,-0.581387,0.124566,0.890152,-0.581387,
0.124566,0.420333,-0.907357,-0.381998,0.604722,-1.67159,0.514796,0.259605,-1.20672,
0.0330518,0.259605,-1.20672,0.0330518,-0.246242,-1.04122,-0.0336426,-0.246242,
-1.04122,-0.0336426,0.604722,-1.67159,0.514796,0.420333,-0.907357,-0.381998,0.259605,
-1.20672,0.0330518,0.259605,-1.20672,0.0330518,0.604722,-1.67159,0.514796,0.604722,
-1.67159,0.514796,0.420333,-0.907357,-0.381998,-0.246242,-1.04122,-0.0336426,0.259605,
-1.20672,0.0330518,0.259605,-1.20672,0.0330518,0.420333,-0.907357,-0.381998,0.420333,
-0.907357,-0.381998,-0.246242,-1.04122,-0.0336426,-0.0922174,-0.393465,1.80599,
-0.35905,-0.0721612,1.17921,-0.35905,-0.0721612,1.17921,-0.295984,0.406308,0.945096,
-0.295984,0.406308,0.945096,-0.0922174,-0.393465,1.80599,-0.68895,-0.229327,0.786546,
-0.35905,-0.0721612,1.17921,-0.35905,-0.0721612,1.17921,-0.0922174,-0.393465,1.80599,
-0.0922174,-0.393465,1.80599,-0.68895,-0.229327,0.786546,-0.295984,0.406308,0.945096,
-0.35905,-0.0721612,1.17921,-0.35905,-0.0721612,1.17921,-0.68895,-0.229327,0.786546,
-0.68895,-0.229327,0.786546,-0.295984,0.406308,0.945096,-0.0922174,-0.393465,1.80599,
0.0197475,0.0811151,1.23195,0.0197475,0.0811151,1.23195,0.447444,0.230502,0.944754,
0.447444,0.230502,0.944754,-0.0922174,-0.393465,1.80599,-0.295984,0.406308,0.945096,
0.0197475,0.0811151,1.23195,0.0197475,0.0811151,1.23195,-0.0922174,-0.393465,1.80599,
-0.0922174,-0.393465,1.80599,-0.295984,0.406308,0.945096,0.447444,0.230502,0.944754,
0.0197475,0.0811151,1.23195,0.0197475,0.0811151,1.23195,-0.295984,0.406308,0.945096,
-0.295984,0.406308,0.945096,0.447444,0.230502,0.944754,-0.0922174,-0.393465,1.80599,
0.289723,-0.225583,1.17891,0.289723,-0.225583,1.17891,0.513941,-0.513786,0.785994,
0.513941,-0.513786,0.785994,-0.0922174,-0.393465,1.80599,0.447444,0.230502,0.944754,
0.289723,-0.225583,1.17891,0.289723,-0.225583,1.17891,-0.0922174,-0.393465,1.80599,
-0.0922174,-0.393465,1.80599,0.447444,0.230502,0.944754,0.513941,-0.513786,0.785994,
0.289723,-0.225583,1.17891,0.289723,-0.225583,1.17891,0.447444,0.230502,0.944754,
0.447444,0.230502,0.944754,0.513941,-0.513786,0.785994,-0.0922174,-0.393465,1.80599,
0.0777782,-0.568409,1.0934,0.0777782,-0.568409,1.0934,-0.188389,-0.797976,0.688216,
-0.188389,-0.797976,0.688216,-0.0922174,-0.393465,1.80599,0.513941,-0.513786,0.785994,
0.0777782,-0.568409,1.0934,0.0777782,-0.568409,1.0934,-0.0922174,-0.393465,1.80599,
-0.0922174,-0.393465,1.80599,0.513941,-0.513786,0.785994,-0.188389,-0.797976,0.688216,
0.0777782,-0.568409,1.0934,0.0777782,-0.568409,1.0934,0.513941,-0.513786,0.785994,
0.513941,-0.513786,0.785994,-0.188389,-0.797976,0.688216,-0.0922174,-0.393465,
1.80599,-0.323185,-0.473589,1.09358,-0.323185,-0.473589,1.09358,-0.68895,-0.229327,
0.786546,-0.68895,-0.229327,0.786546,-0.0922174,-0.393465,1.80599,-0.188389,-0.797976,
0.688216,-0.323185,-0.473589,1.09358,-0.323185,-0.473589,1.09358,-0.0922174,-0.393465,
1.80599,-0.0922174,-0.393465,1.80599,-0.188389,-0.797976,0.688216,-0.68895,-0.229327,
0.786546,-0.323185,-0.473589,1.09358,-0.323185,-0.473589,1.09358,-0.188389,-0.797976,
0.688216,-0.188389,-0.797976,0.688216,-0.68895,-0.229327,0.786546,-1.45832,0.672078,
0.920063,-0.814419,0.28302,0.883902,-0.814419,0.28302,0.883902,-0.68895,-0.229327,
0.786546,-0.68895,-0.229327,0.786546,-1.45832,0.672078,0.920063,-0.295984,0.406308,
0.945096,-0.814419,0.28302,0.883902,-0.814419,0.28302,0.883902,-1.45832,0.672078,
0.920063,-1.45832,0.672078,0.920063,-0.295984,0.406308,0.945096,-0.68895,-0.229327,
0.786546,-0.814419,0.28302,0.883902,-0.814419,0.28302,0.883902,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-0.68895,-0.229327,0.786546,-1.45832,0.672078,
0.920063,-1.06781,0.10721,0.610689,-1.06781,0.10721,0.610689,-1.05617,-0.121122,
0.125459,-1.05617,-0.121122,0.125459,-1.45832,0.672078,0.920063,-0.68895,-0.229327,
0.786546,-1.06781,0.10721,0.610689,-1.06781,0.10721,0.610689,-1.45832,0.672078,
0.920063,-1.45832,0.672078,0.920063,-0.68895,-0.229327,0.786546,-1.05617,-0.121122,
0.125459,-1.06781,0.10721,0.610689,-1.06781,0.10721,0.610689,-0.68895,-0.229327,
0.786546,-0.68895,-0.229327,0.786546,-1.05617,-0.121122,0.125459,-1.45832,0.672078,
0.920063,-1.13488,0.377448,0.306985,-1.13488,0.377448,0.306985,-0.890152,0.581387,
-0.124566,-0.890152,0.581387,-0.124566,-1.45832,0.672078,0.920063,-1.05617,-0.121122,
0.125459,-1.13488,0.377448,0.306985,-1.13488,0.377448,0.306985,-1.45832,0.672078,
0.920063,-1.45832,0.672078,0.920063,-1.05617,-0.121122,0.125459,-0.890152,0.581387,
-0.124566,-1.13488,0.377448,0.306985,-1.13488,0.377448,0.306985,-1.05617,-0.121122,
0.125459,-1.05617,-0.121122,0.125459,-0.890152,0.581387,-0.124566,-1.45832,0.672078,
0.920063,-0.922936,0.720274,0.392498,-0.922936,0.720274,0.392498,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,-1.45832,0.672078,0.920063,-0.890152,0.581387,
-0.124566,-0.922936,0.720274,0.392498,-0.922936,0.720274,0.392498,-1.45832,0.672078,
0.920063,-1.45832,0.672078,0.920063,-0.890152,0.581387,-0.124566,-0.420333,0.907357,
0.381998,-0.922936,0.720274,0.392498,-0.922936,0.720274,0.392498,-0.890152,0.581387,
-0.124566,-0.890152,0.581387,-0.124566,-0.420333,0.907357,0.381998,-1.45832,0.672078,
0.920063,-0.72488,0.661914,0.749052,-0.72488,0.661914,0.749052,-0.295984,0.406308,
0.945096,-0.295984,0.406308,0.945096,-1.45832,0.672078,0.920063,-0.420333,0.907357,
0.381998,-0.72488,0.661914,0.749052,-0.72488,0.661914,0.749052,-1.45832,0.672078,
0.920063,-1.45832,0.672078,0.920063,-0.420333,0.907357,0.381998,-0.295984,0.406308,
0.945096,-0.72488,0.661914,0.749052,-0.72488,0.661914,0.749052,-0.420333,0.907357,
0.381998,-0.420333,0.907357,0.381998,-0.295984,0.406308,0.945096,-1.28894,-1.22378,
0.515671,-0.574524,-1.02099,0.390082,-0.574524,-1.02099,0.390082,-0.246242,-1.04122,
-0.0336426,-0.246242,-1.04122,-0.0336426,-1.28894,-1.22378,0.515671,-0.188389,
-0.797976,0.688216,-0.574524,-1.02099,0.390082,-0.574524,-1.02099,0.390082,-1.28894,
-1.22378,0.515671,-1.28894,-1.22378,0.515671,-0.188389,-0.797976,0.688216,-0.246242,
-1.04122,-0.0336426,-0.574524,-1.02099,0.390082,-0.574524,-1.02099,0.390082,-0.188389,
-0.797976,0.688216,-0.188389,-0.797976,0.688216,-0.246242,-1.04122,-0.0336426,-1.28894,
-1.22378,0.515671,-0.77258,-0.962631,0.0335279,-0.77258,-0.962631,0.0335279,-0.782558,
-0.622897,-0.381445,-0.782558,-0.622897,-0.381445,-1.28894,-1.22378,0.515671,
-0.246242,-1.04122,-0.0336426,-0.77258,-0.962631,0.0335279,-0.77258,-0.962631,
0.0335279,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,0.515671,-0.246242,-1.04122,
-0.0336426,-0.782558,-0.622897,-0.381445,-0.77258,-0.962631,0.0335279,-0.77258,
-0.962631,0.0335279,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,
-0.782558,-0.622897,-0.381445,-1.28894,-1.22378,0.515671,-1.04256,-0.655932,0.0865618,
-1.04256,-0.655932,0.0865618,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,
-1.28894,-1.22378,0.515671,-0.782558,-0.622897,-0.381445,-1.04256,-0.655932,0.0865618,
-1.04256,-0.655932,0.0865618,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,0.515671,
-0.782558,-0.622897,-0.381445,-1.05617,-0.121122,0.125459,-1.04256,-0.655932,0.0865618,
-1.04256,-0.655932,0.0865618,-0.782558,-0.622897,-0.381445,-0.782558,-0.622897,
-0.381445,-1.05617,-0.121122,0.125459,-1.28894,-1.22378,0.515671,-1.01135,-0.524742,
0.475892,-1.01135,-0.524742,0.475892,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-1.28894,-1.22378,0.515671,-1.05617,-0.121122,0.125459,-1.01135,-0.524742,
0.475892,-1.01135,-0.524742,0.475892,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,
0.515671,-1.05617,-0.121122,0.125459,-0.68895,-0.229327,0.786546,-1.01135,-0.524742,
0.475892,-1.01135,-0.524742,0.475892,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,
0.125459,-0.68895,-0.229327,0.786546,-1.28894,-1.22378,0.515671,-0.722093,-0.75036,
0.663478,-0.722093,-0.75036,0.663478,-0.188389,-0.797976,0.688216,-0.188389,-0.797976,
0.688216,-1.28894,-1.22378,0.515671,-0.68895,-0.229327,0.786546,-0.722093,-0.75036,
0.663478,-0.722093,-0.75036,0.663478,-1.28894,-1.22378,0.515671,-1.28894,-1.22378,
0.515671,-0.68895,-0.229327,0.786546,-0.188389,-0.797976,0.688216,-0.722093,-0.75036,
0.663478,-0.722093,-0.75036,0.663478,-0.68895,-0.229327,0.786546,-0.68895,-0.229327,
0.786546,-0.188389,-0.797976,0.688216,-0.330655,-1.39596,-1.16913,0.128554,-0.90321,
-0.832074,0.128554,-0.90321,-0.832074,0.295984,-0.406308,-0.945096,0.295984,-0.406308,
-0.945096,-0.330655,-1.39596,-1.16913,0.420333,-0.907357,-0.381998,0.128554,-0.90321,
-0.832074,0.128554,-0.90321,-0.832074,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,
-1.16913,0.420333,-0.907357,-0.381998,0.295984,-0.406308,-0.945096,0.128554,-0.90321,
-0.832074,0.128554,-0.90321,-0.832074,0.420333,-0.907357,-0.381998,0.420333,-0.907357,
-0.381998,0.295984,-0.406308,-0.945096,-0.330655,-1.39596,-1.16913,-0.160705,-0.677592,
-1.01966,-0.160705,-0.677592,-1.01966,-0.447444,-0.230502,-0.944754,-0.447444,
-0.230502,-0.944754,-0.330655,-1.39596,-1.16913,0.295984,-0.406308,-0.945096,-0.160705,
-0.677592,-1.01966,-0.160705,-0.677592,-1.01966,-0.330655,-1.39596,-1.16913,-0.330655,
-1.39596,-1.16913,0.295984,-0.406308,-0.945096,-0.447444,-0.230502,-0.944754,-0.160705,
-0.677592,-1.01966,-0.160705,-0.677592,-1.01966,0.295984,-0.406308,-0.945096,0.295984,
-0.406308,-0.945096,-0.447444,-0.230502,-0.944754,-0.330655,-1.39596,-1.16913,
-0.520219,-0.749788,-0.831776,-0.520219,-0.749788,-0.831776,-0.782558,-0.622897,
-0.381445,-0.782558,-0.622897,-0.381445,-0.330655,-1.39596,-1.16913,-0.447444,
-0.230502,-0.944754,-0.520219,-0.749788,-0.831776,-0.520219,-0.749788,-0.831776,
-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,-1.16913,-0.447444,-0.230502,-0.944754,
-0.782558,-0.622897,-0.381445,-0.520219,-0.749788,-0.831776,-0.520219,-0.749788,
-0.831776,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,-0.782558,
-0.622897,-0.381445,-0.330655,-1.39596,-1.16913,-0.453152,-1.02003,-0.528072,-0.453152,
-1.02003,-0.528072,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,
-0.330655,-1.39596,-1.16913,-0.782558,-0.622897,-0.381445,-0.453152,-1.02003,
-0.528072,-0.453152,-1.02003,-0.528072,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,
-1.16913,-0.782558,-0.622897,-0.381445,-0.246242,-1.04122,-0.0336426,-0.453152,
-1.02003,-0.528072,-0.453152,-1.02003,-0.528072,-0.782558,-0.622897,-0.381445,
-0.782558,-0.622897,-0.381445,-0.246242,-1.04122,-0.0336426,-0.330655,-1.39596,
-1.16913,-0.0521879,-1.11485,-0.528256,-0.0521879,-1.11485,-0.528256,0.420333,
-0.907357,-0.381998,0.420333,-0.907357,-0.381998,-0.330655,-1.39596,-1.16913,
-0.246242,-1.04122,-0.0336426,-0.0521879,-1.11485,-0.528256,-0.0521879,-1.11485,
-0.528256,-0.330655,-1.39596,-1.16913,-0.330655,-1.39596,-1.16913,-0.246242,-1.04122,
-0.0336426,0.420333,-0.907357,-0.381998,-0.0521879,-1.11485,-0.528256,-0.0521879,
-1.11485,-0.528256,-0.246242,-1.04122,-0.0336426,-0.246242,-1.04122,-0.0336426,
0.420333,-0.907357,-0.381998,0.0922167,0.393466,-1.80599,0.323185,0.47359,-1.09358,
0.323185,0.47359,-1.09358,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,
0.0922167,0.393466,-1.80599,0.68895,0.229327,-0.786546,0.323185,0.47359,-1.09358,
0.323185,0.47359,-1.09358,0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,
0.68895,0.229327,-0.786546,0.188389,0.797976,-0.688216,0.323185,0.47359,-1.09358,
0.323185,0.47359,-1.09358,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,
0.188389,0.797976,-0.688216,0.0922167,0.393466,-1.80599,-0.0777784,0.568409,-1.0934,
-0.0777784,0.568409,-1.0934,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,
0.0922167,0.393466,-1.80599,0.188389,0.797976,-0.688216,-0.0777784,0.568409,-1.0934,
-0.0777784,0.568409,-1.0934,0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,
0.188389,0.797976,-0.688216,-0.513941,0.513786,-0.785994,-0.0777784,0.568409,
-1.0934,-0.0777784,0.568409,-1.0934,0.188389,0.797976,-0.688216,0.188389,0.797976,
-0.688216,-0.513941,0.513786,-0.785994,0.0922167,0.393466,-1.80599,-0.289723,0.225583,
-1.17891,-0.289723,0.225583,-1.17891,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,
-0.944754,0.0922167,0.393466,-1.80599,-0.513941,0.513786,-0.785994,-0.289723,0.225583,
-1.17891,-0.289723,0.225583,-1.17891,0.0922167,0.393466,-1.80599,0.0922167,0.393466,
-1.80599,-0.513941,0.513786,-0.785994,-0.447444,-0.230502,-0.944754,-0.289723,
0.225583,-1.17891,-0.289723,0.225583,-1.17891,-0.513941,0.513786,-0.785994,-0.513941,
0.513786,-0.785994,-0.447444,-0.230502,-0.944754,0.0922167,0.393466,-1.80599,
-0.0197478,-0.0811147,-1.23195,-0.0197478,-0.0811147,-1.23195,0.295984,-0.406308,
-0.945096,0.295984,-0.406308,-0.945096,0.0922167,0.393466,-1.80599,-0.447444,-0.230502,
-0.944754,-0.0197478,-0.0811147,-1.23195,-0.0197478,-0.0811147,-1.23195,0.0922167,
0.393466,-1.80599,0.0922167,0.393466,-1.80599,-0.447444,-0.230502,-0.944754,0.295984,
-0.406308,-0.945096,-0.0197478,-0.0811147,-1.23195,-0.0197478,-0.0811147,-1.23195,
-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,0.295984,-0.406308,
-0.945096,0.0922167,0.393466,-1.80599,0.35905,0.0721616,-1.17921,0.35905,0.0721616,
-1.17921,0.68895,0.229327,-0.786546,0.68895,0.229327,-0.786546,0.0922167,0.393466,
-1.80599,0.295984,-0.406308,-0.945096,0.35905,0.0721616,-1.17921,0.35905,0.0721616,
-1.17921,0.0922167,0.393466,-1.80599,0.0922167,0.393466,-1.80599,0.295984,-0.406308,
-0.945096,0.68895,0.229327,-0.786546,0.35905,0.0721616,-1.17921,0.35905,0.0721616,
-1.17921,0.295984,-0.406308,-0.945096,0.295984,-0.406308,-0.945096,0.68895,0.229327,
-0.786546,-0.604718,1.67159,-0.514804,-0.259603,1.20672,-0.0330543,-0.259603,1.20672,
-0.0330543,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,-0.604718,1.67159,
-0.514804,0.246242,1.04122,0.0336426,-0.259603,1.20672,-0.0330543,-0.259603,1.20672,
-0.0330543,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,0.246242,1.04122,
0.0336426,-0.420333,0.907357,0.381998,-0.259603,1.20672,-0.0330543,-0.259603,1.20672,
-0.0330543,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,-0.420333,0.907357,
0.381998,-0.604718,1.67159,-0.514804,-0.638401,1.05344,-0.0857906,-0.638401,1.05344,
-0.0857906,-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.604718,
1.67159,-0.514804,-0.420333,0.907357,0.381998,-0.638401,1.05344,-0.0857906,-0.638401,
1.05344,-0.0857906,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.420333,
0.907357,0.381998,-0.890152,0.581387,-0.124566,-0.638401,1.05344,-0.0857906,-0.638401,
1.05344,-0.0857906,-0.420333,0.907357,0.381998,-0.420333,0.907357,0.381998,-0.890152,
0.581387,-0.124566,-0.604718,1.67159,-0.514804,-0.669604,0.922253,-0.475121,-0.669604,
0.922253,-0.475121,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-0.604718,
1.67159,-0.514804,-0.890152,0.581387,-0.124566,-0.669604,0.922253,-0.475121,-0.669604,
0.922253,-0.475121,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.890152,
0.581387,-0.124566,-0.513941,0.513786,-0.785994,-0.669604,0.922253,-0.475121,-0.669604,
0.922253,-0.475121,-0.890152,0.581387,-0.124566,-0.890152,0.581387,-0.124566,-0.513941,
0.513786,-0.785994,-0.604718,1.67159,-0.514804,-0.31009,0.99445,-0.663005,-0.31009,
0.99445,-0.663005,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,-0.604718,
1.67159,-0.514804,-0.513941,0.513786,-0.785994,-0.31009,0.99445,-0.663005,-0.31009,
0.99445,-0.663005,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,-0.513941,
0.513786,-0.785994,0.188389,0.797976,-0.688216,-0.31009,0.99445,-0.663005,-0.31009,
0.99445,-0.663005,-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,0.188389,
0.797976,-0.688216,-0.604718,1.67159,-0.514804,-0.0566957,1.17026,-0.389792,-0.0566957,
1.17026,-0.389792,0.246242,1.04122,0.0336426,0.246242,1.04122,0.0336426,-0.604718,
1.67159,-0.514804,0.188389,0.797976,-0.688216,-0.0566957,1.17026,-0.389792,-0.0566957,
1.17026,-0.389792,-0.604718,1.67159,-0.514804,-0.604718,1.67159,-0.514804,0.188389,
0.797976,-0.688216,0.246242,1.04122,0.0336426,-0.0566957,1.17026,-0.389792,-0.0566957,
1.17026,-0.389792,0.188389,0.797976,-0.688216,0.188389,0.797976,-0.688216,0.246242,
1.04122,0.0336426,-1.60568,0.0524988,-0.918662,-0.945228,-0.266967,-0.748287,-0.945228,
-0.266967,-0.748287,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,-0.944754,
-1.60568,0.0524988,-0.918662,-0.782558,-0.622897,-0.381445,-0.945228,-0.266967,
-0.748287,-0.945228,-0.266967,-0.748287,-1.60568,0.0524988,-0.918662,-1.60568,
0.0524988,-0.918662,-0.782558,-0.622897,-0.381445,-0.447444,-0.230502,-0.944754,
-0.945228,-0.266967,-0.748287,-0.945228,-0.266967,-0.748287,-0.782558,-0.622897,
-0.381445,-0.782558,-0.622897,-0.381445,-0.447444,-0.230502,-0.944754,-1.60568,
0.0524988,-0.918662,-0.855689,0.111928,-0.883137,-0.855689,0.111928,-0.883137,
-0.513941,0.513786,-0.785994,-0.513941,0.513786,-0.785994,-1.60568,0.0524988,-0.918662,
-0.447444,-0.230502,-0.944754,-0.855689,0.111928,-0.883137,-0.855689,0.111928,
-0.883137,-1.60568,0.0524988,-0.918662,-1.60568,0.0524988,-0.918662,-0.447444,
-0.230502,-0.944754,-0.513941,0.513786,-0.785994,-0.855689,0.111928,-0.883137,
-0.855689,0.111928,-0.883137,-0.447444,-0.230502,-0.944754,-0.447444,-0.230502,
-0.944754,-0.513941,0.513786,-0.785994,-1.60568,0.0524988,-0.918662,-1.00326,
0.382557,-0.609741,-1.00326,0.382557,-0.609741,-0.890152,0.581387,-0.124566,-0.890152,
0.581387,-0.124566,-1.60568,0.0524988,-0.918662,-0.513941,0.513786,-0.785994,-1.00326,
0.382557,-0.609741,-1.00326,0.382557,-0.609741,-1.60568,0.0524988,-0.918662,-1.60568,
0.0524988,-0.918662,-0.513941,0.513786,-0.785994,-0.890152,0.581387,-0.124566,
-1.00326,0.382557,-0.609741,-1.00326,0.382557,-0.609741,-0.513941,0.513786,-0.785994,
-0.513941,0.513786,-0.785994,-0.890152,0.581387,-0.124566,-1.60568,0.0524988,-0.918662,
-1.184,0.170921,-0.305923,-1.184,0.170921,-0.305923,-1.05617,-0.121122,0.125459,
-1.05617,-0.121122,0.125459,-1.60568,0.0524988,-0.918662,-0.890152,0.581387,-0.124566,
-1.184,0.170921,-0.305923,-1.184,0.170921,-0.305923,-1.60568,0.0524988,-0.918662,
-1.60568,0.0524988,-0.918662,-0.890152,0.581387,-0.124566,-1.05617,-0.121122,0.125459,
-1.184,0.170921,-0.305923,-1.184,0.170921,-0.305923,-0.890152,0.581387,-0.124566,
-0.890152,0.581387,-0.124566,-1.05617,-0.121122,0.125459,-1.60568,0.0524988,-0.918662,
-1.14814,-0.230507,-0.391549,-1.14814,-0.230507,-0.391549,-0.782558,-0.622897,
-0.381445,-0.782558,-0.622897,-0.381445,-1.60568,0.0524988,-0.918662,-1.05617,
-0.121122,0.125459,-1.14814,-0.230507,-0.391549,-1.14814,-0.230507,-0.391549,-1.60568,
0.0524988,-0.918662,-1.60568,0.0524988,-0.918662,-1.05617,-0.121122,0.125459,
-0.782558,-0.622897,-0.381445,-1.14814,-0.230507,-0.391549,-1.14814,-0.230507,
-0.391549,-1.05617,-0.121122,0.125459,-1.05617,-0.121122,0.125459,-0.782558,-0.622897,
-0.381445
      };
    };
  };
 };


 macro mk_icosahedron {

  olink out_fld => mesh;
  olink out_obj => obj.obj;

  GDM.DataObject obj {
     in => <-.mesh;
  };

  Polytri set1 {
      npolys=1;
      poly_connect_list[2] = {0,1076};
  };

  Mesh  mesh {
   nnodes = 2157;
   nspace = 3;
   ncell_sets = 1;
   cell_set[ncell_sets] => {<-.set1};

   coordinates {
      units = "inches";
      values = {
   1.218,-0.627214,0.638571,1.36077,-0.277025,0.710219,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,1.36077,-0.277025,0.710219,1.218,-0.627214,0.638571,
1.218,-0.627214,0.638571,1.218,-0.627214,0.638571,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.36077,-0.277025,0.710219,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,1.218,-0.627214,0.638571,
1.218,-0.627214,0.638571,1.30049,-0.804542,0.307061,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.30049,-0.804542,0.307061,1.218,-0.627214,0.638571,
1.218,-0.627214,0.638571,1.218,-0.627214,0.638571,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,1.30049,-0.804542,0.307061,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.218,-0.627214,0.638571,
1.218,-0.627214,0.638571,0.992746,-0.800074,0.898433,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.992746,-0.800074,0.898433,1.218,-0.627214,0.638571,
1.218,-0.627214,0.638571,1.218,-0.627214,0.638571,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.992746,-0.800074,0.898433,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,1.1238,-0.931751,-0.391939,
1.1238,-0.931751,-0.391939,1.26908,-0.906055,-0.0364424,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,1.26908,-0.906055,-0.0364424,1.1238,-0.931751,-0.391939,
1.1238,-0.931751,-0.391939,1.1238,-0.931751,-0.391939,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.26908,-0.906055,-0.0364424,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,1.1238,-0.931751,-0.391939,
1.1238,-0.931751,-0.391939,1.23516,-0.683075,-0.663795,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.23516,-0.683075,-0.663795,1.1238,-0.931751,-0.391939,
1.1238,-0.931751,-0.391939,1.1238,-0.931751,-0.391939,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,1.23516,-0.683075,-0.663795,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.1238,-0.931751,-0.391939,
1.1238,-0.931751,-0.391939,0.86714,-1.20612,-0.475581,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.86714,-1.20612,-0.475581,1.1238,-0.931751,-0.391939,
1.1238,-0.931751,-0.391939,1.1238,-0.931751,-0.391939,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.86714,-1.20612,-0.475581,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,1.28161,0.449306,0.663539,
1.28161,0.449306,0.663539,1.3853,0.630818,0.340352,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,1.3853,0.630818,0.340352,1.28161,0.449306,0.663539,
1.28161,0.449306,0.663539,1.28161,0.449306,0.663539,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.3853,0.630818,0.340352,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,1.28161,0.449306,0.663539,
1.28161,0.449306,0.663539,1.38197,0.0818148,0.718541,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.38197,0.0818148,0.718541,1.28161,0.449306,0.663539,
1.28161,0.449306,0.663539,1.28161,0.449306,0.663539,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,1.38197,0.0818148,0.718541,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.28161,0.449306,0.663539,
1.28161,0.449306,0.663539,1.07756,0.635286,0.931724,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,1.07756,0.635286,0.931724,1.28161,0.449306,0.663539,
1.28161,0.449306,0.663539,1.28161,0.449306,0.663539,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,1.07756,0.635286,0.931724,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,1.12919,-0.043444,-1.00386,
1.12919,-0.043444,-1.00386,1.23696,-0.386972,-0.867769,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,1.23696,-0.386972,-0.867769,1.12919,-0.043444,-1.00386,
1.12919,-0.043444,-1.00386,1.12919,-0.043444,-1.00386,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.23696,-0.386972,-0.867769,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,1.12919,-0.043444,-1.00386,
1.12919,-0.043444,-1.00386,1.27627,0.278355,-0.852339,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.27627,0.278355,-0.852339,1.12919,-0.043444,-1.00386,
1.12919,-0.043444,-1.00386,1.12919,-0.043444,-1.00386,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,1.27627,0.278355,-0.852339,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.12919,-0.043444,-1.00386,
1.12919,-0.043444,-1.00386,0.874331,-0.0217143,-1.29148,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,0.874331,-0.0217143,-1.29148,1.12919,-0.043444,-1.00386,
1.12919,-0.043444,-1.00386,1.12919,-0.043444,-1.00386,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.874331,-0.0217143,-1.29148,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,1.22672,0.810096,-0.351541,
1.22672,0.810096,-0.351541,1.30879,0.562868,-0.634899,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,1.30879,0.562868,-0.634899,1.22672,0.810096,-0.351541,
1.22672,0.810096,-0.351541,1.22672,0.810096,-0.351541,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.30879,0.562868,-0.634899,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,1.22672,0.810096,-0.351541,
1.22672,0.810096,-0.351541,1.36701,0.751081,0.001992,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.36701,0.751081,0.001992,1.22672,0.810096,-0.351541,
1.22672,0.810096,-0.351541,1.22672,0.810096,-0.351541,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,1.36701,0.751081,0.001992,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.22672,0.810096,-0.351541,
1.22672,0.810096,-0.351541,1.00438,1.11634,-0.421716,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,1.00438,1.11634,-0.421716,1.22672,0.810096,-0.351541,
1.22672,0.810096,-0.351541,1.22672,0.810096,-0.351541,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,1.00438,1.11634,-0.421716,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.318121,-0.92005,1.15629,
0.318121,-0.92005,1.15629,0.692786,-0.897686,1.07101,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.692786,-0.897686,1.07101,0.318121,-0.92005,1.15629,
0.318121,-0.92005,1.15629,0.318121,-0.92005,1.15629,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.692786,-0.897686,1.07101,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.318121,-0.92005,1.15629,
0.318121,-0.92005,1.15629,0.100648,-1.19499,0.997352,-0.805874,-0.987141,1.41214,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.100648,-1.19499,0.997352,0.318121,-0.92005,1.15629,
0.318121,-0.92005,1.15629,0.318121,-0.92005,1.15629,-0.805874,-0.987141,1.41214,
-0.805874,-0.987141,1.41214,0.100648,-1.19499,0.997352,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.318121,-0.92005,1.15629,
0.318121,-0.92005,1.15629,0.16093,-0.667473,1.40051,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,-0.805874,-0.987141,1.41214,
-0.805874,-0.987141,1.41214,0.16093,-0.667473,1.40051,0.318121,-0.92005,1.15629,
0.318121,-0.92005,1.15629,0.318121,-0.92005,1.15629,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.16093,-0.667473,1.40051,-0.805874,-0.987141,1.41214,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,0.165696,-1.4128,-0.511112,
0.165696,-1.4128,-0.511112,-0.00838202,-1.54745,-0.195339,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,-0.980538,-1.55178,-0.498532,
-0.980538,-1.55178,-0.498532,-0.00838202,-1.54745,-0.195339,0.165696,-1.4128,
-0.511112,0.165696,-1.4128,-0.511112,0.165696,-1.4128,-0.511112,0.789696,-1.67778,
0.423627,0.789696,-1.67778,0.423627,-0.00838202,-1.54745,-0.195339,-0.980538,-1.55178,
-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,0.165696,
-1.4128,-0.511112,0.165696,-1.4128,-0.511112,0.547774,-1.36647,-0.515305,0.68793,
-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.789696,
-1.67778,0.423627,0.789696,-1.67778,0.423627,0.547774,-1.36647,-0.515305,0.165696,
-1.4128,-0.511112,0.165696,-1.4128,-0.511112,0.165696,-1.4128,-0.511112,0.68793,
-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.547774,-1.36647,-0.515305,0.789696,
-1.67778,0.423627,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.165696,
-1.4128,-0.511112,0.165696,-1.4128,-0.511112,-0.042304,-1.32447,-0.822691,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,0.68793,
-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.042304,-1.32447,-0.822691,0.165696,
-1.4128,-0.511112,0.165696,-1.4128,-0.511112,0.165696,-1.4128,-0.511112,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.042304,-1.32447,-0.822691,0.68793,
-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.332239,
-1.40557,0.445745,-0.332239,-1.40557,0.445745,-0.116139,-1.35683,0.760504,0.789696,
-1.67778,0.423627,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,-0.805874,
-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.116139,-1.35683,0.760504,-0.332239,
-1.40557,0.445745,-0.332239,-1.40557,0.445745,-0.332239,-1.40557,0.445745,0.789696,
-1.67778,0.423627,0.789696,-1.67778,0.423627,-0.116139,-1.35683,0.760504,-0.805874,
-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.332239,
-1.40557,0.445745,-0.332239,-1.40557,0.445745,-0.17436,-1.54504,0.123613,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,0.789696,
-1.67778,0.423627,0.789696,-1.67778,0.423627,-0.17436,-1.54504,0.123613,-0.332239,
-1.40557,0.445745,-0.332239,-1.40557,0.445745,-0.332239,-1.40557,0.445745,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.17436,-1.54504,0.123613,0.789696,
-1.67778,0.423627,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,-0.332239,
-1.40557,0.445745,-0.332239,-1.40557,0.445745,-0.706217,-1.31483,0.453118,-0.805874,
-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.706217,-1.31483,0.453118,-0.332239,
-1.40557,0.445745,-0.332239,-1.40557,0.445745,-0.332239,-1.40557,0.445745,-0.805874,
-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.706217,-1.31483,0.453118,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,0.42105,
0.821797,1.19669,0.42105,0.821797,1.19669,0.79071,0.759449,1.10944,0.970542,-0.0952288,
1.6331,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.980538,1.55178,
0.498532,0.980538,1.55178,0.498532,0.79071,0.759449,1.10944,0.42105,0.821797,
1.19669,0.42105,0.821797,1.19669,0.42105,0.821797,1.19669,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.79071,0.759449,1.10944,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.42105,0.821797,1.19669,
0.42105,0.821797,1.19669,0.234554,0.578469,1.42941,-0.68793,1.00884,1.45843,
-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.234554,0.578469,1.42941,0.42105,0.821797,1.19669,
0.42105,0.821797,1.19669,0.42105,0.821797,1.19669,-0.68793,1.00884,1.45843,-0.68793,
1.00884,1.45843,0.234554,0.578469,1.42941,0.970542,-0.0952288,1.6331,0.970542,
-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.42105,0.821797,1.19669,0.42105,
0.821797,1.19669,0.237886,1.12747,1.05122,0.980538,1.55178,0.498532,0.980538,
1.55178,0.498532,0.980538,1.55178,0.498532,-0.68793,1.00884,1.45843,-0.68793,
1.00884,1.45843,0.237886,1.12747,1.05122,0.42105,0.821797,1.19669,0.42105,0.821797,
1.19669,0.42105,0.821797,1.19669,0.980538,1.55178,0.498532,0.980538,1.55178,
0.498532,0.237886,1.12747,1.05122,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-0.174421,-0.0245099,1.50122,-0.174421,-0.0245099,
1.50122,0.0360638,0.296367,1.53092,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,
1.6331,0.970542,-0.0952288,1.6331,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,0.0360638,0.296367,1.53092,-0.174421,-0.0245099,1.50122,-0.174421,-0.0245099,
1.50122,-0.174421,-0.0245099,1.50122,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,
1.6331,0.0360638,0.296367,1.53092,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-0.174421,-0.0245099,1.50122,-0.174421,-0.0245099,
1.50122,-0.00325088,-0.36896,1.51549,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,
1.41214,-0.805874,-0.987141,1.41214,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,
1.6331,-0.00325088,-0.36896,1.51549,-0.174421,-0.0245099,1.50122,-0.174421,-0.0245099,
1.50122,-0.174421,-0.0245099,1.50122,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,
1.41214,-0.00325088,-0.36896,1.51549,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,
1.6331,0.970542,-0.0952288,1.6331,-0.174421,-0.0245099,1.50122,-0.174421,-0.0245099,
1.50122,-0.556075,-0.000936999,1.45726,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,
1.41214,-0.556075,-0.000936999,1.45726,-0.174421,-0.0245099,1.50122,-0.174421,
-0.0245099,1.50122,-0.174421,-0.0245099,1.50122,-0.68793,1.00884,1.45843,-0.68793,
1.00884,1.45843,-0.556075,-0.000936999,1.45726,-0.805874,-0.987141,1.41214,-0.805874,
-0.987141,1.41214,-0.805874,-0.987141,1.41214,0.174421,0.0245099,-1.50122,0.174421,
0.0245099,-1.50122,-0.0360638,-0.296367,-1.53092,0.68793,-1.00884,-1.45843,0.68793,
-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.970542,0.0952288,-1.6331,-0.970542,
0.0952288,-1.6331,-0.0360638,-0.296367,-1.53092,0.174421,0.0245099,-1.50122,0.174421,
0.0245099,-1.50122,0.174421,0.0245099,-1.50122,0.68793,-1.00884,-1.45843,0.68793,
-1.00884,-1.45843,-0.0360638,-0.296367,-1.53092,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,0.174421,0.0245099,-1.50122,
0.174421,0.0245099,-1.50122,0.556075,0.000936998,-1.45726,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.556075,0.000936998,-1.45726,0.174421,0.0245099,-1.50122,
0.174421,0.0245099,-1.50122,0.174421,0.0245099,-1.50122,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,0.556075,0.000936998,-1.45726,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.174421,0.0245099,-1.50122,
0.174421,0.0245099,-1.50122,0.00325088,0.36896,-1.51549,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,0.00325088,0.36896,-1.51549,0.174421,0.0245099,-1.50122,
0.174421,0.0245099,-1.50122,0.174421,0.0245099,-1.50122,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,0.00325088,0.36896,-1.51549,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,-0.42105,-0.821797,-1.19669,
-0.42105,-0.821797,-1.19669,-0.237886,-1.12747,-1.05122,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.980538,-1.55178,-0.498532,
-0.980538,-1.55178,-0.498532,-0.237886,-1.12747,-1.05122,-0.42105,-0.821797,-1.19669,
-0.42105,-0.821797,-1.19669,-0.42105,-0.821797,-1.19669,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,-0.237886,-1.12747,-1.05122,-0.980538,-1.55178,-0.498532,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.42105,-0.821797,-1.19669,
-0.42105,-0.821797,-1.19669,-0.234554,-0.578469,-1.42941,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,-0.234554,-0.578469,-1.42941,-0.42105,-0.821797,-1.19669,
-0.42105,-0.821797,-1.19669,-0.42105,-0.821797,-1.19669,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-0.234554,-0.578469,-1.42941,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.42105,-0.821797,-1.19669,
-0.42105,-0.821797,-1.19669,-0.79071,-0.759449,-1.10944,-0.980538,-1.55178,-0.498532,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.970542,0.0952288,
-1.6331,-0.970542,0.0952288,-1.6331,-0.79071,-0.759449,-1.10944,-0.42105,-0.821797,
-1.19669,-0.42105,-0.821797,-1.19669,-0.42105,-0.821797,-1.19669,-0.980538,-1.55178,
-0.498532,-0.980538,-1.55178,-0.498532,-0.79071,-0.759449,-1.10944,-0.970542,0.0952288,
-1.6331,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,0.332239,1.40557,
-0.445745,0.332239,1.40557,-0.445745,0.706217,1.31483,-0.453118,0.980538,1.55178,
0.498532,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.805874,0.987141,
-1.41214,0.805874,0.987141,-1.41214,0.706217,1.31483,-0.453118,0.332239,1.40557,
-0.445745,0.332239,1.40557,-0.445745,0.332239,1.40557,-0.445745,0.980538,1.55178,
0.498532,0.980538,1.55178,0.498532,0.706217,1.31483,-0.453118,0.805874,0.987141,
-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.332239,1.40557,
-0.445745,0.332239,1.40557,-0.445745,0.17436,1.54504,-0.123613,-0.789696,1.67778,
-0.423627,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,0.980538,1.55178,
0.498532,0.980538,1.55178,0.498532,0.17436,1.54504,-0.123613,0.332239,1.40557,
-0.445745,0.332239,1.40557,-0.445745,0.332239,1.40557,-0.445745,-0.789696,1.67778,
-0.423627,-0.789696,1.67778,-0.423627,0.17436,1.54504,-0.123613,0.980538,1.55178,
0.498532,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.332239,1.40557,
-0.445745,0.332239,1.40557,-0.445745,0.116139,1.35683,-0.760504,0.805874,0.987141,
-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,-0.789696,1.67778,
-0.423627,-0.789696,1.67778,-0.423627,0.116139,1.35683,-0.760504,0.332239,1.40557,
-0.445745,0.332239,1.40557,-0.445745,0.332239,1.40557,-0.445745,0.805874,0.987141,
-1.41214,0.805874,0.987141,-1.41214,0.116139,1.35683,-0.760504,-0.789696,1.67778,
-0.423627,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.165696,1.4128,
0.511112,-0.165696,1.4128,0.511112,0.00838202,1.54745,0.195339,0.980538,1.55178,
0.498532,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,-0.789696,1.67778,
-0.423627,-0.789696,1.67778,-0.423627,0.00838202,1.54745,0.195339,-0.165696,1.4128,
0.511112,-0.165696,1.4128,0.511112,-0.165696,1.4128,0.511112,0.980538,1.55178,
0.498532,0.980538,1.55178,0.498532,0.00838202,1.54745,0.195339,-0.789696,1.67778,
-0.423627,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.165696,1.4128,
0.511112,-0.165696,1.4128,0.511112,0.042304,1.32447,0.822691,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,0.042304,1.32447,0.822691,-0.165696,1.4128,0.511112,
-0.165696,1.4128,0.511112,-0.165696,1.4128,0.511112,-0.68793,1.00884,1.45843,
-0.68793,1.00884,1.45843,0.042304,1.32447,0.822691,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,-0.165696,1.4128,0.511112,
-0.165696,1.4128,0.511112,-0.547774,1.36647,0.515305,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.68793,1.00884,1.45843,
-0.68793,1.00884,1.45843,-0.547774,1.36647,0.515305,-0.165696,1.4128,0.511112,
-0.165696,1.4128,0.511112,-0.165696,1.4128,0.511112,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-0.547774,1.36647,0.515305,-0.68793,1.00884,1.45843,
-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.318121,0.92005,-1.15629,
-0.318121,0.92005,-1.15629,-0.16093,0.667473,-1.40051,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-0.16093,0.667473,-1.40051,-0.318121,0.92005,-1.15629,
-0.318121,0.92005,-1.15629,-0.318121,0.92005,-1.15629,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,-0.16093,0.667473,-1.40051,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.318121,0.92005,-1.15629,
-0.318121,0.92005,-1.15629,-0.100648,1.19499,-0.997352,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,-0.100648,1.19499,-0.997352,-0.318121,0.92005,-1.15629,
-0.318121,0.92005,-1.15629,-0.318121,0.92005,-1.15629,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-0.100648,1.19499,-0.997352,0.805874,0.987141,-1.41214,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,-0.318121,0.92005,-1.15629,
-0.318121,0.92005,-1.15629,-0.692786,0.897686,-1.07101,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-0.692786,0.897686,-1.07101,-0.318121,0.92005,-1.15629,
-0.318121,0.92005,-1.15629,-0.318121,0.92005,-1.15629,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-0.692786,0.897686,-1.07101,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-1.22672,-0.810096,0.351541,
-1.22672,-0.810096,0.351541,-1.30879,-0.562868,0.634899,-0.805874,-0.987141,1.41214,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.30879,-0.562868,0.634899,-1.22672,-0.810096,0.351541,
-1.22672,-0.810096,0.351541,-1.22672,-0.810096,0.351541,-0.805874,-0.987141,1.41214,
-0.805874,-0.987141,1.41214,-1.30879,-0.562868,0.634899,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.22672,-0.810096,0.351541,
-1.22672,-0.810096,0.351541,-1.00438,-1.11634,0.421716,-0.980538,-1.55178,-0.498532,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.805874,-0.987141,
1.41214,-0.805874,-0.987141,1.41214,-1.00438,-1.11634,0.421716,-1.22672,-0.810096,
0.351541,-1.22672,-0.810096,0.351541,-1.22672,-0.810096,0.351541,-0.980538,-1.55178,
-0.498532,-0.980538,-1.55178,-0.498532,-1.00438,-1.11634,0.421716,-0.805874,-0.987141,
1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-1.22672,-0.810096,
0.351541,-1.22672,-0.810096,0.351541,-1.36701,-0.751081,-0.001992,-1.89376,0.108633,
0.141015,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-0.980538,-1.55178,
-0.498532,-0.980538,-1.55178,-0.498532,-1.36701,-0.751081,-0.001992,-1.22672,
-0.810096,0.351541,-1.22672,-0.810096,0.351541,-1.22672,-0.810096,0.351541,-1.89376,
0.108633,0.141015,-1.89376,0.108633,0.141015,-1.36701,-0.751081,-0.001992,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-1.12919,
0.043444,1.00386,-1.12919,0.043444,1.00386,-0.874331,0.0217143,1.29148,-0.805874,
-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.68793,
1.00884,1.45843,-0.68793,1.00884,1.45843,-0.874331,0.0217143,1.29148,-1.12919,
0.043444,1.00386,-1.12919,0.043444,1.00386,-1.12919,0.043444,1.00386,-0.805874,
-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.874331,0.0217143,1.29148,-0.68793,
1.00884,1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-1.12919,0.043444,
1.00386,-1.12919,0.043444,1.00386,-1.27627,-0.278355,0.852339,-1.89376,0.108633,
0.141015,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-0.805874,-0.987141,
1.41214,-0.805874,-0.987141,1.41214,-1.27627,-0.278355,0.852339,-1.12919,0.043444,
1.00386,-1.12919,0.043444,1.00386,-1.12919,0.043444,1.00386,-1.89376,0.108633,
0.141015,-1.89376,0.108633,0.141015,-1.27627,-0.278355,0.852339,-0.805874,-0.987141,
1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-1.12919,0.043444,
1.00386,-1.12919,0.043444,1.00386,-1.23696,0.386972,0.867769,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.23696,0.386972,0.867769,-1.12919,0.043444,1.00386,
-1.12919,0.043444,1.00386,-1.12919,0.043444,1.00386,-0.68793,1.00884,1.45843,
-0.68793,1.00884,1.45843,-1.23696,0.386972,0.867769,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.28161,-0.449306,-0.663539,
-1.28161,-0.449306,-0.663539,-1.3853,-0.630818,-0.340352,-0.980538,-1.55178,-0.498532,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.3853,-0.630818,-0.340352,-1.28161,-0.449306,-0.663539,
-1.28161,-0.449306,-0.663539,-1.28161,-0.449306,-0.663539,-0.980538,-1.55178,
-0.498532,-0.980538,-1.55178,-0.498532,-1.3853,-0.630818,-0.340352,-1.89376,0.108633,
0.141015,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.28161,-0.449306,
-0.663539,-1.28161,-0.449306,-0.663539,-1.07756,-0.635286,-0.931724,-0.970542,
0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-1.07756,-0.635286,-0.931724,-1.28161,
-0.449306,-0.663539,-1.28161,-0.449306,-0.663539,-1.28161,-0.449306,-0.663539,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-1.07756,-0.635286,-0.931724,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,
-0.498532,-1.28161,-0.449306,-0.663539,-1.28161,-0.449306,-0.663539,-1.38197,
-0.0818148,-0.718541,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.89376,
0.108633,0.141015,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-1.38197,
-0.0818148,-0.718541,-1.28161,-0.449306,-0.663539,-1.28161,-0.449306,-0.663539,
-1.28161,-0.449306,-0.663539,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,
-1.38197,-0.0818148,-0.718541,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,
-1.6331,-0.970542,0.0952288,-1.6331,-1.1238,0.931751,0.391939,-1.1238,0.931751,
0.391939,-0.86714,1.20612,0.475581,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-0.86714,1.20612,0.475581,-1.1238,0.931751,0.391939,-1.1238,0.931751,
0.391939,-1.1238,0.931751,0.391939,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,-0.86714,1.20612,0.475581,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-0.789696,1.67778,-0.423627,-1.1238,0.931751,0.391939,-1.1238,0.931751,
0.391939,-1.23516,0.683075,0.663795,-1.89376,0.108633,0.141015,-1.89376,0.108633,
0.141015,-1.89376,0.108633,0.141015,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,-1.23516,0.683075,0.663795,-1.1238,0.931751,0.391939,-1.1238,0.931751,
0.391939,-1.1238,0.931751,0.391939,-1.89376,0.108633,0.141015,-1.89376,0.108633,
0.141015,-1.23516,0.683075,0.663795,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-1.1238,0.931751,0.391939,-1.1238,0.931751,0.391939,
-1.26908,0.906055,0.0364425,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,
-1.26908,0.906055,0.0364425,-1.1238,0.931751,0.391939,-1.1238,0.931751,0.391939,
-1.1238,0.931751,0.391939,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,
-1.26908,0.906055,0.0364425,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.218,0.627214,-0.638571,-1.218,0.627214,-0.638571,
-1.36077,0.277025,-0.710219,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,
-1.36077,0.277025,-0.710219,-1.218,0.627214,-0.638571,-1.218,0.627214,-0.638571,
-1.218,0.627214,-0.638571,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,
-1.36077,0.277025,-0.710219,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.218,0.627214,-0.638571,-1.218,0.627214,-0.638571,
-0.992746,0.800074,-0.898433,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,
-0.992746,0.800074,-0.898433,-1.218,0.627214,-0.638571,-1.218,0.627214,-0.638571,
-1.218,0.627214,-0.638571,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,
-0.992746,0.800074,-0.898433,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-1.218,0.627214,-0.638571,-1.218,0.627214,-0.638571,
-1.30049,0.804542,-0.307061,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,
-1.30049,0.804542,-0.307061,-1.218,0.627214,-0.638571,-1.218,0.627214,-0.638571,
-1.218,0.627214,-0.638571,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,
-1.30049,0.804542,-0.307061,-0.789696,1.67778,-0.423627,1.218,-0.627214,0.638571,
1.36077,-0.277025,0.710219,1.36077,-0.277025,0.710219,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.218,-0.627214,0.638571,0.970542,-0.0952288,1.6331,
1.36077,-0.277025,0.710219,1.36077,-0.277025,0.710219,1.218,-0.627214,0.638571,
1.218,-0.627214,0.638571,0.970542,-0.0952288,1.6331,1.89376,-0.108633,-0.141015,
1.36077,-0.277025,0.710219,1.36077,-0.277025,0.710219,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,1.89376,-0.108633,-0.141015,1.218,-0.627214,0.638571,
1.30049,-0.804542,0.307061,1.30049,-0.804542,0.307061,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,1.218,-0.627214,0.638571,1.89376,-0.108633,-0.141015,
1.30049,-0.804542,0.307061,1.30049,-0.804542,0.307061,1.218,-0.627214,0.638571,
1.218,-0.627214,0.638571,1.89376,-0.108633,-0.141015,0.789696,-1.67778,0.423627,
1.30049,-0.804542,0.307061,1.30049,-0.804542,0.307061,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,0.789696,-1.67778,0.423627,1.218,-0.627214,0.638571,
0.992746,-0.800074,0.898433,0.992746,-0.800074,0.898433,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,1.218,-0.627214,0.638571,0.789696,-1.67778,0.423627,
0.992746,-0.800074,0.898433,0.992746,-0.800074,0.898433,1.218,-0.627214,0.638571,
1.218,-0.627214,0.638571,0.789696,-1.67778,0.423627,0.970542,-0.0952288,1.6331,
0.992746,-0.800074,0.898433,0.992746,-0.800074,0.898433,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.970542,-0.0952288,1.6331,1.1238,-0.931751,-0.391939,
1.26908,-0.906055,-0.0364424,1.26908,-0.906055,-0.0364424,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.1238,-0.931751,-0.391939,0.789696,-1.67778,0.423627,
1.26908,-0.906055,-0.0364424,1.26908,-0.906055,-0.0364424,1.1238,-0.931751,-0.391939,
1.1238,-0.931751,-0.391939,0.789696,-1.67778,0.423627,1.89376,-0.108633,-0.141015,
1.26908,-0.906055,-0.0364424,1.26908,-0.906055,-0.0364424,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,1.89376,-0.108633,-0.141015,1.1238,-0.931751,-0.391939,
1.23516,-0.683075,-0.663795,1.23516,-0.683075,-0.663795,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,1.1238,-0.931751,-0.391939,1.89376,-0.108633,-0.141015,
1.23516,-0.683075,-0.663795,1.23516,-0.683075,-0.663795,1.1238,-0.931751,-0.391939,
1.1238,-0.931751,-0.391939,1.89376,-0.108633,-0.141015,0.68793,-1.00884,-1.45843,
1.23516,-0.683075,-0.663795,1.23516,-0.683075,-0.663795,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,0.68793,-1.00884,-1.45843,1.1238,-0.931751,-0.391939,
0.86714,-1.20612,-0.475581,0.86714,-1.20612,-0.475581,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,1.1238,-0.931751,-0.391939,0.68793,-1.00884,-1.45843,
0.86714,-1.20612,-0.475581,0.86714,-1.20612,-0.475581,1.1238,-0.931751,-0.391939,
1.1238,-0.931751,-0.391939,0.68793,-1.00884,-1.45843,0.789696,-1.67778,0.423627,
0.86714,-1.20612,-0.475581,0.86714,-1.20612,-0.475581,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.789696,-1.67778,0.423627,1.28161,0.449306,0.663539,
1.3853,0.630818,0.340352,1.3853,0.630818,0.340352,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.28161,0.449306,0.663539,0.980538,1.55178,0.498532,
1.3853,0.630818,0.340352,1.3853,0.630818,0.340352,1.28161,0.449306,0.663539,1.28161,
0.449306,0.663539,0.980538,1.55178,0.498532,1.89376,-0.108633,-0.141015,1.3853,
0.630818,0.340352,1.3853,0.630818,0.340352,0.980538,1.55178,0.498532,0.980538,
1.55178,0.498532,1.89376,-0.108633,-0.141015,1.28161,0.449306,0.663539,1.38197,
0.0818148,0.718541,1.38197,0.0818148,0.718541,0.970542,-0.0952288,1.6331,0.970542,
-0.0952288,1.6331,1.28161,0.449306,0.663539,1.89376,-0.108633,-0.141015,1.38197,
0.0818148,0.718541,1.38197,0.0818148,0.718541,1.28161,0.449306,0.663539,1.28161,
0.449306,0.663539,1.89376,-0.108633,-0.141015,0.970542,-0.0952288,1.6331,1.38197,
0.0818148,0.718541,1.38197,0.0818148,0.718541,1.89376,-0.108633,-0.141015,1.89376,
-0.108633,-0.141015,0.970542,-0.0952288,1.6331,1.28161,0.449306,0.663539,1.07756,
0.635286,0.931724,1.07756,0.635286,0.931724,0.980538,1.55178,0.498532,0.980538,
1.55178,0.498532,1.28161,0.449306,0.663539,0.970542,-0.0952288,1.6331,1.07756,
0.635286,0.931724,1.07756,0.635286,0.931724,1.28161,0.449306,0.663539,1.28161,
0.449306,0.663539,0.970542,-0.0952288,1.6331,0.980538,1.55178,0.498532,1.07756,
0.635286,0.931724,1.07756,0.635286,0.931724,0.970542,-0.0952288,1.6331,0.970542,
-0.0952288,1.6331,0.980538,1.55178,0.498532,1.12919,-0.043444,-1.00386,1.23696,
-0.386972,-0.867769,1.23696,-0.386972,-0.867769,1.89376,-0.108633,-0.141015,1.89376,
-0.108633,-0.141015,1.12919,-0.043444,-1.00386,0.68793,-1.00884,-1.45843,1.23696,
-0.386972,-0.867769,1.23696,-0.386972,-0.867769,1.12919,-0.043444,-1.00386,1.12919,
-0.043444,-1.00386,0.68793,-1.00884,-1.45843,1.89376,-0.108633,-0.141015,1.23696,
-0.386972,-0.867769,1.23696,-0.386972,-0.867769,0.68793,-1.00884,-1.45843,0.68793,
-1.00884,-1.45843,1.89376,-0.108633,-0.141015,1.12919,-0.043444,-1.00386,1.27627,
0.278355,-0.852339,1.27627,0.278355,-0.852339,0.805874,0.987141,-1.41214,0.805874,
0.987141,-1.41214,1.12919,-0.043444,-1.00386,1.89376,-0.108633,-0.141015,1.27627,
0.278355,-0.852339,1.27627,0.278355,-0.852339,1.12919,-0.043444,-1.00386,1.12919,
-0.043444,-1.00386,1.89376,-0.108633,-0.141015,0.805874,0.987141,-1.41214,1.27627,
0.278355,-0.852339,1.27627,0.278355,-0.852339,1.89376,-0.108633,-0.141015,1.89376,
-0.108633,-0.141015,0.805874,0.987141,-1.41214,1.12919,-0.043444,-1.00386,0.874331,
-0.0217143,-1.29148,0.874331,-0.0217143,-1.29148,0.68793,-1.00884,-1.45843,0.68793,
-1.00884,-1.45843,1.12919,-0.043444,-1.00386,0.805874,0.987141,-1.41214,0.874331,
-0.0217143,-1.29148,0.874331,-0.0217143,-1.29148,1.12919,-0.043444,-1.00386,1.12919,
-0.043444,-1.00386,0.805874,0.987141,-1.41214,0.68793,-1.00884,-1.45843,0.874331,
-0.0217143,-1.29148,0.874331,-0.0217143,-1.29148,0.805874,0.987141,-1.41214,0.805874,
0.987141,-1.41214,0.68793,-1.00884,-1.45843,1.22672,0.810096,-0.351541,1.30879,
0.562868,-0.634899,1.30879,0.562868,-0.634899,1.89376,-0.108633,-0.141015,1.89376,
-0.108633,-0.141015,1.22672,0.810096,-0.351541,0.805874,0.987141,-1.41214,1.30879,
0.562868,-0.634899,1.30879,0.562868,-0.634899,1.22672,0.810096,-0.351541,1.22672,
0.810096,-0.351541,0.805874,0.987141,-1.41214,1.89376,-0.108633,-0.141015,1.30879,
0.562868,-0.634899,1.30879,0.562868,-0.634899,0.805874,0.987141,-1.41214,0.805874,
0.987141,-1.41214,1.89376,-0.108633,-0.141015,1.22672,0.810096,-0.351541,1.36701,
0.751081,0.001992,1.36701,0.751081,0.001992,0.980538,1.55178,0.498532,0.980538,
1.55178,0.498532,1.22672,0.810096,-0.351541,1.89376,-0.108633,-0.141015,1.36701,
0.751081,0.001992,1.36701,0.751081,0.001992,1.22672,0.810096,-0.351541,1.22672,
0.810096,-0.351541,1.89376,-0.108633,-0.141015,0.980538,1.55178,0.498532,1.36701,
0.751081,0.001992,1.36701,0.751081,0.001992,1.89376,-0.108633,-0.141015,1.89376,
-0.108633,-0.141015,0.980538,1.55178,0.498532,1.22672,0.810096,-0.351541,1.00438,
1.11634,-0.421716,1.00438,1.11634,-0.421716,0.805874,0.987141,-1.41214,0.805874,
0.987141,-1.41214,1.22672,0.810096,-0.351541,0.980538,1.55178,0.498532,1.00438,
1.11634,-0.421716,1.00438,1.11634,-0.421716,1.22672,0.810096,-0.351541,1.22672,
0.810096,-0.351541,0.980538,1.55178,0.498532,0.805874,0.987141,-1.41214,1.00438,
1.11634,-0.421716,1.00438,1.11634,-0.421716,0.980538,1.55178,0.498532,0.980538,
1.55178,0.498532,0.805874,0.987141,-1.41214,0.318121,-0.92005,1.15629,0.692786,
-0.897686,1.07101,0.692786,-0.897686,1.07101,0.789696,-1.67778,0.423627,0.789696,
-1.67778,0.423627,0.318121,-0.92005,1.15629,0.970542,-0.0952288,1.6331,0.692786,
-0.897686,1.07101,0.692786,-0.897686,1.07101,0.318121,-0.92005,1.15629,0.318121,
-0.92005,1.15629,0.970542,-0.0952288,1.6331,0.789696,-1.67778,0.423627,0.692786,
-0.897686,1.07101,0.692786,-0.897686,1.07101,0.970542,-0.0952288,1.6331,0.970542,
-0.0952288,1.6331,0.789696,-1.67778,0.423627,0.318121,-0.92005,1.15629,0.100648,
-1.19499,0.997352,0.100648,-1.19499,0.997352,-0.805874,-0.987141,1.41214,-0.805874,
-0.987141,1.41214,0.318121,-0.92005,1.15629,0.789696,-1.67778,0.423627,0.100648,
-1.19499,0.997352,0.100648,-1.19499,0.997352,0.318121,-0.92005,1.15629,0.318121,
-0.92005,1.15629,0.789696,-1.67778,0.423627,-0.805874,-0.987141,1.41214,0.100648,
-1.19499,0.997352,0.100648,-1.19499,0.997352,0.789696,-1.67778,0.423627,0.789696,
-1.67778,0.423627,-0.805874,-0.987141,1.41214,0.318121,-0.92005,1.15629,0.16093,
-0.667473,1.40051,0.16093,-0.667473,1.40051,0.970542,-0.0952288,1.6331,0.970542,
-0.0952288,1.6331,0.318121,-0.92005,1.15629,-0.805874,-0.987141,1.41214,0.16093,
-0.667473,1.40051,0.16093,-0.667473,1.40051,0.318121,-0.92005,1.15629,0.318121,
-0.92005,1.15629,-0.805874,-0.987141,1.41214,0.970542,-0.0952288,1.6331,0.16093,
-0.667473,1.40051,0.16093,-0.667473,1.40051,-0.805874,-0.987141,1.41214,-0.805874,
-0.987141,1.41214,0.970542,-0.0952288,1.6331,0.165696,-1.4128,-0.511112,-0.00838202,
-1.54745,-0.195339,-0.00838202,-1.54745,-0.195339,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.165696,-1.4128,-0.511112,-0.980538,-1.55178,-0.498532,
-0.00838202,-1.54745,-0.195339,-0.00838202,-1.54745,-0.195339,0.165696,-1.4128,
-0.511112,0.165696,-1.4128,-0.511112,-0.980538,-1.55178,-0.498532,0.789696,-1.67778,
0.423627,-0.00838202,-1.54745,-0.195339,-0.00838202,-1.54745,-0.195339,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,0.789696,-1.67778,0.423627,0.165696,
-1.4128,-0.511112,0.547774,-1.36647,-0.515305,0.547774,-1.36647,-0.515305,0.68793,
-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.165696,-1.4128,-0.511112,0.789696,
-1.67778,0.423627,0.547774,-1.36647,-0.515305,0.547774,-1.36647,-0.515305,0.165696,
-1.4128,-0.511112,0.165696,-1.4128,-0.511112,0.789696,-1.67778,0.423627,0.68793,
-1.00884,-1.45843,0.547774,-1.36647,-0.515305,0.547774,-1.36647,-0.515305,0.789696,
-1.67778,0.423627,0.789696,-1.67778,0.423627,0.68793,-1.00884,-1.45843,0.165696,
-1.4128,-0.511112,-0.042304,-1.32447,-0.822691,-0.042304,-1.32447,-0.822691,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,0.165696,-1.4128,-0.511112,0.68793,
-1.00884,-1.45843,-0.042304,-1.32447,-0.822691,-0.042304,-1.32447,-0.822691,0.165696,
-1.4128,-0.511112,0.165696,-1.4128,-0.511112,0.68793,-1.00884,-1.45843,-0.980538,
-1.55178,-0.498532,-0.042304,-1.32447,-0.822691,-0.042304,-1.32447,-0.822691,0.68793,
-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.980538,-1.55178,-0.498532,-0.332239,
-1.40557,0.445745,-0.116139,-1.35683,0.760504,-0.116139,-1.35683,0.760504,0.789696,
-1.67778,0.423627,0.789696,-1.67778,0.423627,-0.332239,-1.40557,0.445745,-0.805874,
-0.987141,1.41214,-0.116139,-1.35683,0.760504,-0.116139,-1.35683,0.760504,-0.332239,
-1.40557,0.445745,-0.332239,-1.40557,0.445745,-0.805874,-0.987141,1.41214,0.789696,
-1.67778,0.423627,-0.116139,-1.35683,0.760504,-0.116139,-1.35683,0.760504,-0.805874,
-0.987141,1.41214,-0.805874,-0.987141,1.41214,0.789696,-1.67778,0.423627,-0.332239,
-1.40557,0.445745,-0.17436,-1.54504,0.123613,-0.17436,-1.54504,0.123613,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.332239,-1.40557,0.445745,0.789696,
-1.67778,0.423627,-0.17436,-1.54504,0.123613,-0.17436,-1.54504,0.123613,-0.332239,
-1.40557,0.445745,-0.332239,-1.40557,0.445745,0.789696,-1.67778,0.423627,-0.980538,
-1.55178,-0.498532,-0.17436,-1.54504,0.123613,-0.17436,-1.54504,0.123613,0.789696,
-1.67778,0.423627,0.789696,-1.67778,0.423627,-0.980538,-1.55178,-0.498532,-0.332239,
-1.40557,0.445745,-0.706217,-1.31483,0.453118,-0.706217,-1.31483,0.453118,-0.805874,
-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.332239,-1.40557,0.445745,-0.980538,
-1.55178,-0.498532,-0.706217,-1.31483,0.453118,-0.706217,-1.31483,0.453118,-0.332239,
-1.40557,0.445745,-0.332239,-1.40557,0.445745,-0.980538,-1.55178,-0.498532,-0.805874,
-0.987141,1.41214,-0.706217,-1.31483,0.453118,-0.706217,-1.31483,0.453118,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.805874,-0.987141,1.41214,
0.42105,0.821797,1.19669,0.79071,0.759449,1.10944,0.79071,0.759449,1.10944,0.970542,
-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.42105,0.821797,1.19669,0.980538,
1.55178,0.498532,0.79071,0.759449,1.10944,0.79071,0.759449,1.10944,0.42105,0.821797,
1.19669,0.42105,0.821797,1.19669,0.980538,1.55178,0.498532,0.970542,-0.0952288,
1.6331,0.79071,0.759449,1.10944,0.79071,0.759449,1.10944,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,0.970542,-0.0952288,1.6331,0.42105,0.821797,1.19669,
0.234554,0.578469,1.42941,0.234554,0.578469,1.42941,-0.68793,1.00884,1.45843,
-0.68793,1.00884,1.45843,0.42105,0.821797,1.19669,0.970542,-0.0952288,1.6331,
0.234554,0.578469,1.42941,0.234554,0.578469,1.42941,0.42105,0.821797,1.19669,
0.42105,0.821797,1.19669,0.970542,-0.0952288,1.6331,-0.68793,1.00884,1.45843,
0.234554,0.578469,1.42941,0.234554,0.578469,1.42941,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,-0.68793,1.00884,1.45843,0.42105,0.821797,1.19669,
0.237886,1.12747,1.05122,0.237886,1.12747,1.05122,0.980538,1.55178,0.498532,0.980538,
1.55178,0.498532,0.42105,0.821797,1.19669,-0.68793,1.00884,1.45843,0.237886,1.12747,
1.05122,0.237886,1.12747,1.05122,0.42105,0.821797,1.19669,0.42105,0.821797,1.19669,
-0.68793,1.00884,1.45843,0.980538,1.55178,0.498532,0.237886,1.12747,1.05122,
0.237886,1.12747,1.05122,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,0.980538,
1.55178,0.498532,-0.174421,-0.0245099,1.50122,0.0360638,0.296367,1.53092,0.0360638,
0.296367,1.53092,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,-0.174421,
-0.0245099,1.50122,-0.68793,1.00884,1.45843,0.0360638,0.296367,1.53092,0.0360638,
0.296367,1.53092,-0.174421,-0.0245099,1.50122,-0.174421,-0.0245099,1.50122,-0.68793,
1.00884,1.45843,0.970542,-0.0952288,1.6331,0.0360638,0.296367,1.53092,0.0360638,
0.296367,1.53092,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,0.970542,-0.0952288,
1.6331,-0.174421,-0.0245099,1.50122,-0.00325088,-0.36896,1.51549,-0.00325088,-0.36896,
1.51549,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.174421,-0.0245099,
1.50122,0.970542,-0.0952288,1.6331,-0.00325088,-0.36896,1.51549,-0.00325088,-0.36896,
1.51549,-0.174421,-0.0245099,1.50122,-0.174421,-0.0245099,1.50122,0.970542,-0.0952288,
1.6331,-0.805874,-0.987141,1.41214,-0.00325088,-0.36896,1.51549,-0.00325088,-0.36896,
1.51549,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,-0.805874,-0.987141,
1.41214,-0.174421,-0.0245099,1.50122,-0.556075,-0.000936999,1.45726,-0.556075,
-0.000936999,1.45726,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.174421,
-0.0245099,1.50122,-0.805874,-0.987141,1.41214,-0.556075,-0.000936999,1.45726,
-0.556075,-0.000936999,1.45726,-0.174421,-0.0245099,1.50122,-0.174421,-0.0245099,
1.50122,-0.805874,-0.987141,1.41214,-0.68793,1.00884,1.45843,-0.556075,-0.000936999,
1.45726,-0.556075,-0.000936999,1.45726,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,
1.41214,-0.68793,1.00884,1.45843,0.174421,0.0245099,-1.50122,-0.0360638,-0.296367,
-1.53092,-0.0360638,-0.296367,-1.53092,0.68793,-1.00884,-1.45843,0.68793,-1.00884,
-1.45843,0.174421,0.0245099,-1.50122,-0.970542,0.0952288,-1.6331,-0.0360638,-0.296367,
-1.53092,-0.0360638,-0.296367,-1.53092,0.174421,0.0245099,-1.50122,0.174421,0.0245099,
-1.50122,-0.970542,0.0952288,-1.6331,0.68793,-1.00884,-1.45843,-0.0360638,-0.296367,
-1.53092,-0.0360638,-0.296367,-1.53092,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,
-1.6331,0.68793,-1.00884,-1.45843,0.174421,0.0245099,-1.50122,0.556075,0.000936998,
-1.45726,0.556075,0.000936998,-1.45726,0.805874,0.987141,-1.41214,0.805874,0.987141,
-1.41214,0.174421,0.0245099,-1.50122,0.68793,-1.00884,-1.45843,0.556075,0.000936998,
-1.45726,0.556075,0.000936998,-1.45726,0.174421,0.0245099,-1.50122,0.174421,0.0245099,
-1.50122,0.68793,-1.00884,-1.45843,0.805874,0.987141,-1.41214,0.556075,0.000936998,
-1.45726,0.556075,0.000936998,-1.45726,0.68793,-1.00884,-1.45843,0.68793,-1.00884,
-1.45843,0.805874,0.987141,-1.41214,0.174421,0.0245099,-1.50122,0.00325088,0.36896,
-1.51549,0.00325088,0.36896,-1.51549,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,
-1.6331,0.174421,0.0245099,-1.50122,0.805874,0.987141,-1.41214,0.00325088,0.36896,
-1.51549,0.00325088,0.36896,-1.51549,0.174421,0.0245099,-1.50122,0.174421,0.0245099,
-1.50122,0.805874,0.987141,-1.41214,-0.970542,0.0952288,-1.6331,0.00325088,0.36896,
-1.51549,0.00325088,0.36896,-1.51549,0.805874,0.987141,-1.41214,0.805874,0.987141,
-1.41214,-0.970542,0.0952288,-1.6331,-0.42105,-0.821797,-1.19669,-0.237886,-1.12747,
-1.05122,-0.237886,-1.12747,-1.05122,0.68793,-1.00884,-1.45843,0.68793,-1.00884,
-1.45843,-0.42105,-0.821797,-1.19669,-0.980538,-1.55178,-0.498532,-0.237886,-1.12747,
-1.05122,-0.237886,-1.12747,-1.05122,-0.42105,-0.821797,-1.19669,-0.42105,-0.821797,
-1.19669,-0.980538,-1.55178,-0.498532,0.68793,-1.00884,-1.45843,-0.237886,-1.12747,
-1.05122,-0.237886,-1.12747,-1.05122,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,
-0.498532,0.68793,-1.00884,-1.45843,-0.42105,-0.821797,-1.19669,-0.234554,-0.578469,
-1.42941,-0.234554,-0.578469,-1.42941,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,
-1.6331,-0.42105,-0.821797,-1.19669,0.68793,-1.00884,-1.45843,-0.234554,-0.578469,
-1.42941,-0.234554,-0.578469,-1.42941,-0.42105,-0.821797,-1.19669,-0.42105,-0.821797,
-1.19669,0.68793,-1.00884,-1.45843,-0.970542,0.0952288,-1.6331,-0.234554,-0.578469,
-1.42941,-0.234554,-0.578469,-1.42941,0.68793,-1.00884,-1.45843,0.68793,-1.00884,
-1.45843,-0.970542,0.0952288,-1.6331,-0.42105,-0.821797,-1.19669,-0.79071,-0.759449,
-1.10944,-0.79071,-0.759449,-1.10944,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,
-0.498532,-0.42105,-0.821797,-1.19669,-0.970542,0.0952288,-1.6331,-0.79071,-0.759449,
-1.10944,-0.79071,-0.759449,-1.10944,-0.42105,-0.821797,-1.19669,-0.42105,-0.821797,
-1.19669,-0.970542,0.0952288,-1.6331,-0.980538,-1.55178,-0.498532,-0.79071,-0.759449,
-1.10944,-0.79071,-0.759449,-1.10944,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,
-1.6331,-0.980538,-1.55178,-0.498532,0.332239,1.40557,-0.445745,0.706217,1.31483,
-0.453118,0.706217,1.31483,-0.453118,0.980538,1.55178,0.498532,0.980538,1.55178,
0.498532,0.332239,1.40557,-0.445745,0.805874,0.987141,-1.41214,0.706217,1.31483,
-0.453118,0.706217,1.31483,-0.453118,0.332239,1.40557,-0.445745,0.332239,1.40557,
-0.445745,0.805874,0.987141,-1.41214,0.980538,1.55178,0.498532,0.706217,1.31483,
-0.453118,0.706217,1.31483,-0.453118,0.805874,0.987141,-1.41214,0.805874,0.987141,
-1.41214,0.980538,1.55178,0.498532,0.332239,1.40557,-0.445745,0.17436,1.54504,
-0.123613,0.17436,1.54504,-0.123613,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,0.332239,1.40557,-0.445745,0.980538,1.55178,0.498532,0.17436,1.54504,
-0.123613,0.17436,1.54504,-0.123613,0.332239,1.40557,-0.445745,0.332239,1.40557,
-0.445745,0.980538,1.55178,0.498532,-0.789696,1.67778,-0.423627,0.17436,1.54504,
-0.123613,0.17436,1.54504,-0.123613,0.980538,1.55178,0.498532,0.980538,1.55178,
0.498532,-0.789696,1.67778,-0.423627,0.332239,1.40557,-0.445745,0.116139,1.35683,
-0.760504,0.116139,1.35683,-0.760504,0.805874,0.987141,-1.41214,0.805874,0.987141,
-1.41214,0.332239,1.40557,-0.445745,-0.789696,1.67778,-0.423627,0.116139,1.35683,
-0.760504,0.116139,1.35683,-0.760504,0.332239,1.40557,-0.445745,0.332239,1.40557,
-0.445745,-0.789696,1.67778,-0.423627,0.805874,0.987141,-1.41214,0.116139,1.35683,
-0.760504,0.116139,1.35683,-0.760504,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,0.805874,0.987141,-1.41214,-0.165696,1.4128,0.511112,0.00838202,1.54745,
0.195339,0.00838202,1.54745,0.195339,0.980538,1.55178,0.498532,0.980538,1.55178,
0.498532,-0.165696,1.4128,0.511112,-0.789696,1.67778,-0.423627,0.00838202,1.54745,
0.195339,0.00838202,1.54745,0.195339,-0.165696,1.4128,0.511112,-0.165696,1.4128,
0.511112,-0.789696,1.67778,-0.423627,0.980538,1.55178,0.498532,0.00838202,1.54745,
0.195339,0.00838202,1.54745,0.195339,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,0.980538,1.55178,0.498532,-0.165696,1.4128,0.511112,0.042304,1.32447,
0.822691,0.042304,1.32447,0.822691,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,-0.165696,1.4128,0.511112,0.980538,1.55178,0.498532,0.042304,1.32447,
0.822691,0.042304,1.32447,0.822691,-0.165696,1.4128,0.511112,-0.165696,1.4128,
0.511112,0.980538,1.55178,0.498532,-0.68793,1.00884,1.45843,0.042304,1.32447,
0.822691,0.042304,1.32447,0.822691,0.980538,1.55178,0.498532,0.980538,1.55178,
0.498532,-0.68793,1.00884,1.45843,-0.165696,1.4128,0.511112,-0.547774,1.36647,
0.515305,-0.547774,1.36647,0.515305,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-0.165696,1.4128,0.511112,-0.68793,1.00884,1.45843,-0.547774,1.36647,
0.515305,-0.547774,1.36647,0.515305,-0.165696,1.4128,0.511112,-0.165696,1.4128,
0.511112,-0.68793,1.00884,1.45843,-0.789696,1.67778,-0.423627,-0.547774,1.36647,
0.515305,-0.547774,1.36647,0.515305,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,-0.789696,1.67778,-0.423627,-0.318121,0.92005,-1.15629,-0.16093,0.667473,
-1.40051,-0.16093,0.667473,-1.40051,0.805874,0.987141,-1.41214,0.805874,0.987141,
-1.41214,-0.318121,0.92005,-1.15629,-0.970542,0.0952288,-1.6331,-0.16093,0.667473,
-1.40051,-0.16093,0.667473,-1.40051,-0.318121,0.92005,-1.15629,-0.318121,0.92005,
-1.15629,-0.970542,0.0952288,-1.6331,0.805874,0.987141,-1.41214,-0.16093,0.667473,
-1.40051,-0.16093,0.667473,-1.40051,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,
-1.6331,0.805874,0.987141,-1.41214,-0.318121,0.92005,-1.15629,-0.100648,1.19499,
-0.997352,-0.100648,1.19499,-0.997352,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-0.318121,0.92005,-1.15629,0.805874,0.987141,-1.41214,-0.100648,1.19499,
-0.997352,-0.100648,1.19499,-0.997352,-0.318121,0.92005,-1.15629,-0.318121,0.92005,
-1.15629,0.805874,0.987141,-1.41214,-0.789696,1.67778,-0.423627,-0.100648,1.19499,
-0.997352,-0.100648,1.19499,-0.997352,0.805874,0.987141,-1.41214,0.805874,0.987141,
-1.41214,-0.789696,1.67778,-0.423627,-0.318121,0.92005,-1.15629,-0.692786,0.897686,
-1.07101,-0.692786,0.897686,-1.07101,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,
-1.6331,-0.318121,0.92005,-1.15629,-0.789696,1.67778,-0.423627,-0.692786,0.897686,
-1.07101,-0.692786,0.897686,-1.07101,-0.318121,0.92005,-1.15629,-0.318121,0.92005,
-1.15629,-0.789696,1.67778,-0.423627,-0.970542,0.0952288,-1.6331,-0.692786,0.897686,
-1.07101,-0.692786,0.897686,-1.07101,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-0.970542,0.0952288,-1.6331,-1.22672,-0.810096,0.351541,-1.30879,-0.562868,
0.634899,-1.30879,-0.562868,0.634899,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,
1.41214,-1.22672,-0.810096,0.351541,-1.89376,0.108633,0.141015,-1.30879,-0.562868,
0.634899,-1.30879,-0.562868,0.634899,-1.22672,-0.810096,0.351541,-1.22672,-0.810096,
0.351541,-1.89376,0.108633,0.141015,-0.805874,-0.987141,1.41214,-1.30879,-0.562868,
0.634899,-1.30879,-0.562868,0.634899,-1.89376,0.108633,0.141015,-1.89376,0.108633,
0.141015,-0.805874,-0.987141,1.41214,-1.22672,-0.810096,0.351541,-1.00438,-1.11634,
0.421716,-1.00438,-1.11634,0.421716,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,
-0.498532,-1.22672,-0.810096,0.351541,-0.805874,-0.987141,1.41214,-1.00438,-1.11634,
0.421716,-1.00438,-1.11634,0.421716,-1.22672,-0.810096,0.351541,-1.22672,-0.810096,
0.351541,-0.805874,-0.987141,1.41214,-0.980538,-1.55178,-0.498532,-1.00438,-1.11634,
0.421716,-1.00438,-1.11634,0.421716,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,
1.41214,-0.980538,-1.55178,-0.498532,-1.22672,-0.810096,0.351541,-1.36701,-0.751081,
-0.001992,-1.36701,-0.751081,-0.001992,-1.89376,0.108633,0.141015,-1.89376,0.108633,
0.141015,-1.22672,-0.810096,0.351541,-0.980538,-1.55178,-0.498532,-1.36701,-0.751081,
-0.001992,-1.36701,-0.751081,-0.001992,-1.22672,-0.810096,0.351541,-1.22672,-0.810096,
0.351541,-0.980538,-1.55178,-0.498532,-1.89376,0.108633,0.141015,-1.36701,-0.751081,
-0.001992,-1.36701,-0.751081,-0.001992,-0.980538,-1.55178,-0.498532,-0.980538,
-1.55178,-0.498532,-1.89376,0.108633,0.141015,-1.12919,0.043444,1.00386,-0.874331,
0.0217143,1.29148,-0.874331,0.0217143,1.29148,-0.805874,-0.987141,1.41214,-0.805874,
-0.987141,1.41214,-1.12919,0.043444,1.00386,-0.68793,1.00884,1.45843,-0.874331,
0.0217143,1.29148,-0.874331,0.0217143,1.29148,-1.12919,0.043444,1.00386,-1.12919,
0.043444,1.00386,-0.68793,1.00884,1.45843,-0.805874,-0.987141,1.41214,-0.874331,
0.0217143,1.29148,-0.874331,0.0217143,1.29148,-0.68793,1.00884,1.45843,-0.68793,
1.00884,1.45843,-0.805874,-0.987141,1.41214,-1.12919,0.043444,1.00386,-1.27627,
-0.278355,0.852339,-1.27627,-0.278355,0.852339,-1.89376,0.108633,0.141015,-1.89376,
0.108633,0.141015,-1.12919,0.043444,1.00386,-0.805874,-0.987141,1.41214,-1.27627,
-0.278355,0.852339,-1.27627,-0.278355,0.852339,-1.12919,0.043444,1.00386,-1.12919,
0.043444,1.00386,-0.805874,-0.987141,1.41214,-1.89376,0.108633,0.141015,-1.27627,
-0.278355,0.852339,-1.27627,-0.278355,0.852339,-0.805874,-0.987141,1.41214,-0.805874,
-0.987141,1.41214,-1.89376,0.108633,0.141015,-1.12919,0.043444,1.00386,-1.23696,
0.386972,0.867769,-1.23696,0.386972,0.867769,-0.68793,1.00884,1.45843,-0.68793,
1.00884,1.45843,-1.12919,0.043444,1.00386,-1.89376,0.108633,0.141015,-1.23696,
0.386972,0.867769,-1.23696,0.386972,0.867769,-1.12919,0.043444,1.00386,-1.12919,
0.043444,1.00386,-1.89376,0.108633,0.141015,-0.68793,1.00884,1.45843,-1.23696,
0.386972,0.867769,-1.23696,0.386972,0.867769,-1.89376,0.108633,0.141015,-1.89376,
0.108633,0.141015,-0.68793,1.00884,1.45843,-1.28161,-0.449306,-0.663539,-1.3853,
-0.630818,-0.340352,-1.3853,-0.630818,-0.340352,-0.980538,-1.55178,-0.498532,-0.980538,
-1.55178,-0.498532,-1.28161,-0.449306,-0.663539,-1.89376,0.108633,0.141015,-1.3853,
-0.630818,-0.340352,-1.3853,-0.630818,-0.340352,-1.28161,-0.449306,-0.663539,-1.28161,
-0.449306,-0.663539,-1.89376,0.108633,0.141015,-0.980538,-1.55178,-0.498532,-1.3853,
-0.630818,-0.340352,-1.3853,-0.630818,-0.340352,-1.89376,0.108633,0.141015,-1.89376,
0.108633,0.141015,-0.980538,-1.55178,-0.498532,-1.28161,-0.449306,-0.663539,-1.07756,
-0.635286,-0.931724,-1.07756,-0.635286,-0.931724,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-1.28161,-0.449306,-0.663539,-0.980538,-1.55178,-0.498532,
-1.07756,-0.635286,-0.931724,-1.07756,-0.635286,-0.931724,-1.28161,-0.449306,-0.663539,
-1.28161,-0.449306,-0.663539,-0.980538,-1.55178,-0.498532,-0.970542,0.0952288,
-1.6331,-1.07756,-0.635286,-0.931724,-1.07756,-0.635286,-0.931724,-0.980538,-1.55178,
-0.498532,-0.980538,-1.55178,-0.498532,-0.970542,0.0952288,-1.6331,-1.28161,-0.449306,
-0.663539,-1.38197,-0.0818148,-0.718541,-1.38197,-0.0818148,-0.718541,-1.89376,
0.108633,0.141015,-1.89376,0.108633,0.141015,-1.28161,-0.449306,-0.663539,-0.970542,
0.0952288,-1.6331,-1.38197,-0.0818148,-0.718541,-1.38197,-0.0818148,-0.718541,
-1.28161,-0.449306,-0.663539,-1.28161,-0.449306,-0.663539,-0.970542,0.0952288,
-1.6331,-1.89376,0.108633,0.141015,-1.38197,-0.0818148,-0.718541,-1.38197,-0.0818148,
-0.718541,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-1.89376,0.108633,
0.141015,-1.1238,0.931751,0.391939,-0.86714,1.20612,0.475581,-0.86714,1.20612,
0.475581,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-1.1238,0.931751,0.391939,
-0.789696,1.67778,-0.423627,-0.86714,1.20612,0.475581,-0.86714,1.20612,0.475581,
-1.1238,0.931751,0.391939,-1.1238,0.931751,0.391939,-0.789696,1.67778,-0.423627,
-0.68793,1.00884,1.45843,-0.86714,1.20612,0.475581,-0.86714,1.20612,0.475581,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.68793,1.00884,1.45843,
-1.1238,0.931751,0.391939,-1.23516,0.683075,0.663795,-1.23516,0.683075,0.663795,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.1238,0.931751,0.391939,
-0.68793,1.00884,1.45843,-1.23516,0.683075,0.663795,-1.23516,0.683075,0.663795,
-1.1238,0.931751,0.391939,-1.1238,0.931751,0.391939,-0.68793,1.00884,1.45843,
-1.89376,0.108633,0.141015,-1.23516,0.683075,0.663795,-1.23516,0.683075,0.663795,
-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-1.89376,0.108633,0.141015,
-1.1238,0.931751,0.391939,-1.26908,0.906055,0.0364425,-1.26908,0.906055,0.0364425,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-1.1238,0.931751,0.391939,
-1.89376,0.108633,0.141015,-1.26908,0.906055,0.0364425,-1.26908,0.906055,0.0364425,
-1.1238,0.931751,0.391939,-1.1238,0.931751,0.391939,-1.89376,0.108633,0.141015,
-0.789696,1.67778,-0.423627,-1.26908,0.906055,0.0364425,-1.26908,0.906055,0.0364425,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-0.789696,1.67778,-0.423627,
-1.218,0.627214,-0.638571,-1.36077,0.277025,-0.710219,-1.36077,0.277025,-0.710219,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-1.218,0.627214,-0.638571,
-1.89376,0.108633,0.141015,-1.36077,0.277025,-0.710219,-1.36077,0.277025,-0.710219,
-1.218,0.627214,-0.638571,-1.218,0.627214,-0.638571,-1.89376,0.108633,0.141015,
-0.970542,0.0952288,-1.6331,-1.36077,0.277025,-0.710219,-1.36077,0.277025,-0.710219,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-0.970542,0.0952288,-1.6331,
-1.218,0.627214,-0.638571,-0.992746,0.800074,-0.898433,-0.992746,0.800074,-0.898433,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-1.218,0.627214,-0.638571,
-0.970542,0.0952288,-1.6331,-0.992746,0.800074,-0.898433,-0.992746,0.800074,-0.898433,
-1.218,0.627214,-0.638571,-1.218,0.627214,-0.638571,-0.970542,0.0952288,-1.6331,
-0.789696,1.67778,-0.423627,-0.992746,0.800074,-0.898433,-0.992746,0.800074,-0.898433,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.789696,1.67778,-0.423627,
-1.218,0.627214,-0.638571,-1.30049,0.804542,-0.307061,-1.30049,0.804542,-0.307061,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.218,0.627214,-0.638571,
-0.789696,1.67778,-0.423627,-1.30049,0.804542,-0.307061,-1.30049,0.804542,-0.307061,
-1.218,0.627214,-0.638571,-1.218,0.627214,-0.638571,-0.789696,1.67778,-0.423627,
-1.89376,0.108633,0.141015,-1.30049,0.804542,-0.307061,-1.30049,0.804542,-0.307061,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-1.89376,0.108633,0.141015
      };
    };
  };
 };


 macro mk_icosa_stellated {

  olink out_fld => mesh;
  olink out_obj => obj.obj;

  GDM.DataObject obj {
     in => <-.mesh;
  };

  Polytri set1 {
      npolys=1;
      poly_connect_list[2] = {0,1076};
  };

  Mesh  mesh {
   nnodes = 2157;
   nspace = 3;
   ncell_sets = 1;
   cell_set[ncell_sets] => {<-.set1};

   coordinates {
      units = "inches";
      values = {


   2.02381,-1.04217,1.06104,1.62937,-0.415344,0.85104,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,1.62937,-0.415344,0.85104,2.02381,-1.04217,1.06104,
2.02381,-1.04217,1.06104,2.02381,-1.04217,1.06104,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.62937,-0.415344,0.85104,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,2.02381,-1.04217,1.06104,
2.02381,-1.04217,1.06104,1.56909,-0.942861,0.447883,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.56909,-0.942861,0.447883,2.02381,-1.04217,1.06104,
2.02381,-1.04217,1.06104,2.02381,-1.04217,1.06104,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,1.56909,-0.942861,0.447883,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,2.02381,-1.04217,1.06104,
2.02381,-1.04217,1.06104,1.26135,-0.938393,1.03925,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,1.26135,-0.938393,1.03925,2.02381,-1.04217,1.06104,
2.02381,-1.04217,1.06104,2.02381,-1.04217,1.06104,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,1.26135,-0.938393,1.03925,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,1.86728,-1.54818,-0.651239,
1.86728,-1.54818,-0.651239,1.51691,-1.11153,-0.122876,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,1.51691,-1.11153,-0.122876,1.86728,-1.54818,-0.651239,
1.86728,-1.54818,-0.651239,1.86728,-1.54818,-0.651239,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.51691,-1.11153,-0.122876,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,1.86728,-1.54818,-0.651239,
1.86728,-1.54818,-0.651239,1.48299,-0.888552,-0.750228,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.48299,-0.888552,-0.750228,1.86728,-1.54818,-0.651239,
1.86728,-1.54818,-0.651239,1.86728,-1.54818,-0.651239,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,1.48299,-0.888552,-0.750228,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.86728,-1.54818,-0.651239,
1.86728,-1.54818,-0.651239,1.11497,-1.4116,-0.562014,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,1.11497,-1.4116,-0.562014,1.86728,-1.54818,-0.651239,
1.86728,-1.54818,-0.651239,1.86728,-1.54818,-0.651239,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,1.11497,-1.4116,-0.562014,0.68793,-1.00884,-1.45843,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,2.12951,0.746562,1.10252,
2.12951,0.746562,1.10252,1.66794,0.729903,0.48668,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,1.66794,0.729903,0.48668,2.12951,0.746562,1.10252,
2.12951,0.746562,1.10252,2.12951,0.746562,1.10252,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,1.66794,0.729903,0.48668,0.980538,1.55178,0.498532,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,2.12951,0.746562,1.10252,
2.12951,0.746562,1.10252,1.6646,0.1809,0.86487,0.970542,-0.0952288,1.6331,0.970542,
-0.0952288,1.6331,0.970542,-0.0952288,1.6331,1.89376,-0.108633,-0.141015,1.89376,
-0.108633,-0.141015,1.6646,0.1809,0.86487,2.12951,0.746562,1.10252,2.12951,0.746562,
1.10252,2.12951,0.746562,1.10252,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,
1.6331,1.6646,0.1809,0.86487,1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,
1.89376,-0.108633,-0.141015,2.12951,0.746562,1.10252,2.12951,0.746562,1.10252,
1.3602,0.734371,1.07805,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.980538,
1.55178,0.498532,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,1.3602,
0.734371,1.07805,2.12951,0.746562,1.10252,2.12951,0.746562,1.10252,2.12951,0.746562,
1.10252,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,1.3602,0.734371,1.07805,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,
1.87624,-0.0721855,-1.668,1.87624,-0.0721855,-1.668,1.48598,-0.396553,-1.08915,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,1.48598,-0.396553,-1.08915,
1.87624,-0.0721855,-1.668,1.87624,-0.0721855,-1.668,1.87624,-0.0721855,-1.668,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.48598,-0.396553,-1.08915,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,
1.87624,-0.0721855,-1.668,1.87624,-0.0721855,-1.668,1.52529,0.268774,-1.07372,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.52529,0.268774,-1.07372,
1.87624,-0.0721855,-1.668,1.87624,-0.0721855,-1.668,1.87624,-0.0721855,-1.668,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,1.52529,0.268774,-1.07372,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,
1.87624,-0.0721855,-1.668,1.87624,-0.0721855,-1.668,1.12335,-0.0312948,-1.51286,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,1.12335,-0.0312948,-1.51286,
1.87624,-0.0721855,-1.668,1.87624,-0.0721855,-1.668,1.87624,-0.0721855,-1.668,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,1.12335,-0.0312948,-1.51286,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,
2.03831,1.34605,-0.584115,2.03831,1.34605,-0.584115,1.57931,0.741518,-0.712423,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,1.57931,0.741518,-0.712423,
2.03831,1.34605,-0.584115,2.03831,1.34605,-0.584115,2.03831,1.34605,-0.584115,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.57931,0.741518,-0.712423,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,
2.03831,1.34605,-0.584115,2.03831,1.34605,-0.584115,1.63754,0.929731,-0.0755326,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.63754,0.929731,-0.0755326,
2.03831,1.34605,-0.584115,2.03831,1.34605,-0.584115,2.03831,1.34605,-0.584115,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,1.63754,0.929731,-0.0755326,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,
2.03831,1.34605,-0.584115,2.03831,1.34605,-0.584115,1.27491,1.29499,-0.499241,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,1.27491,1.29499,-0.499241,
2.03831,1.34605,-0.584115,2.03831,1.34605,-0.584115,2.03831,1.34605,-0.584115,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,1.27491,1.29499,-0.499241,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,
0.528584,-1.52874,1.92127,0.528584,-1.52874,1.92127,0.762941,-1.10058,1.326,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.762941,-1.10058,1.326,
0.528584,-1.52874,1.92127,0.528584,-1.52874,1.92127,0.528584,-1.52874,1.92127,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.762941,-1.10058,1.326,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,
0.528584,-1.52874,1.92127,0.528584,-1.52874,1.92127,0.170802,-1.39789,1.25235,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.170802,-1.39789,1.25235,
0.528584,-1.52874,1.92127,0.528584,-1.52874,1.92127,0.528584,-1.52874,1.92127,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,0.170802,-1.39789,1.25235,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,
0.528584,-1.52874,1.92127,0.528584,-1.52874,1.92127,0.231084,-0.870371,1.6555,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,0.231084,-0.870371,1.6555,
0.528584,-1.52874,1.92127,0.528584,-1.52874,1.92127,0.528584,-1.52874,1.92127,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.231084,-0.870371,1.6555,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,
0.275315,-2.34749,-0.849256,0.275315,-2.34749,-0.849256,0.0281577,-1.85902,-0.308053,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,0.0281577,-1.85902,
-0.308053,0.275315,-2.34749,-0.849256,0.275315,-2.34749,-0.849256,0.275315,-2.34749,
-0.849256,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.0281577,-1.85902,
-0.308053,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,
-1.55178,-0.498532,0.275315,-2.34749,-0.849256,0.275315,-2.34749,-0.849256,0.584314,
-1.67804,-0.62802,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.68793,
-1.00884,-1.45843,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.584314,
-1.67804,-0.62802,0.275315,-2.34749,-0.849256,0.275315,-2.34749,-0.849256,0.275315,
-2.34749,-0.849256,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.584314,
-1.67804,-0.62802,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.789696,
-1.67778,0.423627,0.275315,-2.34749,-0.849256,0.275315,-2.34749,-0.849256,-0.00576429,
-1.63604,-0.935406,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,
-1.55178,-0.498532,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.00576429,
-1.63604,-0.935406,0.275315,-2.34749,-0.849256,0.275315,-2.34749,-0.849256,0.275315,
-2.34749,-0.849256,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.00576429,
-1.63604,-0.935406,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.68793,
-1.00884,-1.45843,-0.552046,-2.33547,0.740642,-0.552046,-2.33547,0.740642,-0.189408,
-1.6668,0.858803,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.789696,
-1.67778,0.423627,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.189408,
-1.6668,0.858803,-0.552046,-2.33547,0.740642,-0.552046,-2.33547,0.740642,-0.552046,
-2.33547,0.740642,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,-0.189408,
-1.6668,0.858803,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,
-0.987141,1.41214,-0.552046,-2.33547,0.740642,-0.552046,-2.33547,0.740642,-0.247629,
-1.85501,0.221912,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,
-1.55178,-0.498532,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,-0.247629,
-1.85501,0.221912,-0.552046,-2.33547,0.740642,-0.552046,-2.33547,0.740642,-0.552046,
-2.33547,0.740642,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.247629,
-1.85501,0.221912,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.789696,
-1.67778,0.423627,-0.552046,-2.33547,0.740642,-0.552046,-2.33547,0.740642,-0.779486,
-1.6248,0.551417,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,
-0.987141,1.41214,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.779486,
-1.6248,0.551417,-0.552046,-2.33547,0.740642,-0.552046,-2.33547,0.740642,-0.552046,
-2.33547,0.740642,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.779486,
-1.6248,0.551417,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,
-1.55178,-0.498532,0.69961,1.36549,1.9884,0.69961,1.36549,1.9884,0.883563,0.94068,
1.37334,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,
1.6331,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.883563,0.94068,
1.37334,0.69961,1.36549,1.9884,0.69961,1.36549,1.9884,0.69961,1.36549,1.9884,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.883563,0.94068,1.37334,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,
0.69961,1.36549,1.9884,0.69961,1.36549,1.9884,0.327407,0.7597,1.69331,-0.68793,
1.00884,1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,0.970542,-0.0952288,
1.6331,0.970542,-0.0952288,1.6331,0.327407,0.7597,1.69331,0.69961,1.36549,1.9884,
0.69961,1.36549,1.9884,0.69961,1.36549,1.9884,-0.68793,1.00884,1.45843,-0.68793,
1.00884,1.45843,0.327407,0.7597,1.69331,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,
1.6331,0.970542,-0.0952288,1.6331,0.69961,1.36549,1.9884,0.69961,1.36549,1.9884,
0.330739,1.3087,1.31512,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.980538,
1.55178,0.498532,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,0.330739,1.3087,
1.31512,0.69961,1.36549,1.9884,0.69961,1.36549,1.9884,0.69961,1.36549,1.9884,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.330739,1.3087,1.31512,
-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.289817,
-0.0407247,2.49441,-0.289817,-0.0407247,2.49441,-0.00240177,0.290962,1.86198,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,
-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.00240177,0.290962,1.86198,
-0.289817,-0.0407247,2.49441,-0.289817,-0.0407247,2.49441,-0.289817,-0.0407247,
2.49441,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,-0.00240177,0.290962,
1.86198,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,
-0.289817,-0.0407247,2.49441,-0.289817,-0.0407247,2.49441,-0.0417164,-0.374365,
1.84655,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,
1.41214,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,-0.0417164,-0.374365,
1.84655,-0.289817,-0.0407247,2.49441,-0.289817,-0.0407247,2.49441,-0.289817,-0.0407247,
2.49441,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.0417164,-0.374365,
1.84655,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,
1.6331,-0.289817,-0.0407247,2.49441,-0.289817,-0.0407247,2.49441,-0.59454,-0.00634191,
1.78833,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.59454,-0.00634191,1.78833,
-0.289817,-0.0407247,2.49441,-0.289817,-0.0407247,2.49441,-0.289817,-0.0407247,
2.49441,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.59454,-0.00634191,1.78833,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,
0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,0.00240177,-0.290962,-1.86198,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,0.00240177,-0.290962,-1.86198,
0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.00240177,-0.290962,-1.86198,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,
0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,0.59454,0.00634191,-1.78833,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.59454,0.00634191,-1.78833,
0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.59454,0.00634191,-1.78833,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,
0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,0.0417164,0.374365,-1.84655,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.0417164,0.374365,-1.84655,
0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,0.0417164,0.374365,-1.84655,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,
-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,-1.9884,-0.330739,-1.3087,-1.31512,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.330739,-1.3087,
-1.31512,-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,
-1.9884,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.330739,-1.3087,
-1.31512,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,
-0.498532,-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,-1.9884,-0.327407,-0.7597,
-1.69331,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,
-1.6331,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.327407,-0.7597,
-1.69331,-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,
-1.9884,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.327407,-0.7597,
-1.69331,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.68793,-1.00884,
-1.45843,-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,-1.9884,-0.883563,-0.94068,
-1.37334,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,
-0.498532,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.883563,-0.94068,
-1.37334,-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,
-1.9884,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.883563,-0.94068,
-1.37334,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,
-1.6331,0.552046,2.33547,-0.740642,0.552046,2.33547,-0.740642,0.779486,1.6248,
-0.551417,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.980538,1.55178,
0.498532,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.779486,1.6248,
-0.551417,0.552046,2.33547,-0.740642,0.552046,2.33547,-0.740642,0.552046,2.33547,
-0.740642,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.779486,1.6248,
-0.551417,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,
-1.41214,0.552046,2.33547,-0.740642,0.552046,2.33547,-0.740642,0.247629,1.85501,
-0.221912,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.247629,1.85501,
-0.221912,0.552046,2.33547,-0.740642,0.552046,2.33547,-0.740642,0.552046,2.33547,
-0.740642,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,0.247629,1.85501,
-0.221912,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.980538,1.55178,
0.498532,0.552046,2.33547,-0.740642,0.552046,2.33547,-0.740642,0.189408,1.6668,
-0.858803,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,
-1.41214,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,0.189408,1.6668,
-0.858803,0.552046,2.33547,-0.740642,0.552046,2.33547,-0.740642,0.552046,2.33547,
-0.740642,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.189408,1.6668,
-0.858803,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-0.275315,2.34749,0.849256,-0.275315,2.34749,0.849256,-0.0281577,1.85902,
0.308053,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.980538,1.55178,
0.498532,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.0281577,1.85902,
0.308053,-0.275315,2.34749,0.849256,-0.275315,2.34749,0.849256,-0.275315,2.34749,
0.849256,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,-0.0281577,1.85902,
0.308053,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-0.275315,2.34749,0.849256,-0.275315,2.34749,0.849256,0.00576429,1.63604,
0.935406,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.00576429,1.63604,
0.935406,-0.275315,2.34749,0.849256,-0.275315,2.34749,0.849256,-0.275315,2.34749,
0.849256,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,0.00576429,1.63604,
0.935406,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.980538,1.55178,
0.498532,-0.275315,2.34749,0.849256,-0.275315,2.34749,0.849256,-0.584314,1.67804,
0.62802,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.584314,1.67804,
0.62802,-0.275315,2.34749,0.849256,-0.275315,2.34749,0.849256,-0.275315,2.34749,
0.849256,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.584314,1.67804,
0.62802,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,
-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,-0.231084,0.870371,-1.6555,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.231084,0.870371,-1.6555,
-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,-0.231084,0.870371,-1.6555,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,
-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,-0.170802,1.39789,-1.25235,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,-0.170802,1.39789,-1.25235,
-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.170802,1.39789,-1.25235,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,
-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,-0.762941,1.10058,-1.326,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.762941,1.10058,-1.326,
-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.762941,1.10058,-1.326,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,
-2.03831,-1.34605,0.584115,-2.03831,-1.34605,0.584115,-1.57931,-0.741518,0.712423,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.57931,-0.741518,0.712423,
-2.03831,-1.34605,0.584115,-2.03831,-1.34605,0.584115,-2.03831,-1.34605,0.584115,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-1.57931,-0.741518,0.712423,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,
-2.03831,-1.34605,0.584115,-2.03831,-1.34605,0.584115,-1.27491,-1.29499,0.499241,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,
-0.498532,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-1.27491,-1.29499,
0.499241,-2.03831,-1.34605,0.584115,-2.03831,-1.34605,0.584115,-2.03831,-1.34605,
0.584115,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-1.27491,-1.29499,
0.499241,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,
1.41214,-2.03831,-1.34605,0.584115,-2.03831,-1.34605,0.584115,-1.63754,-0.929731,
0.0755326,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.89376,0.108633,
0.141015,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-1.63754,-0.929731,
0.0755326,-2.03831,-1.34605,0.584115,-2.03831,-1.34605,0.584115,-2.03831,-1.34605,
0.584115,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.63754,-0.929731,
0.0755326,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,
-1.55178,-0.498532,-1.87624,0.0721856,1.668,-1.87624,0.0721856,1.668,-1.12335,
0.0312948,1.51286,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,
-0.987141,1.41214,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-1.12335,
0.0312948,1.51286,-1.87624,0.0721856,1.668,-1.87624,0.0721856,1.668,-1.87624,
0.0721856,1.668,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-1.12335,
0.0312948,1.51286,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.68793,
1.00884,1.45843,-1.87624,0.0721856,1.668,-1.87624,0.0721856,1.668,-1.52529,-0.268774,
1.07372,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.89376,0.108633,
0.141015,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-1.52529,-0.268774,
1.07372,-1.87624,0.0721856,1.668,-1.87624,0.0721856,1.668,-1.87624,0.0721856,1.668,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-1.52529,-0.268774,1.07372,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,
-1.87624,0.0721856,1.668,-1.87624,0.0721856,1.668,-1.48598,0.396553,1.08915,
-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-1.89376,
0.108633,0.141015,-1.89376,0.108633,0.141015,-1.48598,0.396553,1.08915,-1.87624,
0.0721856,1.668,-1.87624,0.0721856,1.668,-1.87624,0.0721856,1.668,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-1.48598,0.396553,1.08915,-1.89376,0.108633,
0.141015,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-2.12951,-0.746562,
-1.10252,-2.12951,-0.746562,-1.10252,-1.66794,-0.729903,-0.48668,-0.980538,-1.55178,
-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-1.89376,
0.108633,0.141015,-1.89376,0.108633,0.141015,-1.66794,-0.729903,-0.48668,-2.12951,
-0.746562,-1.10252,-2.12951,-0.746562,-1.10252,-2.12951,-0.746562,-1.10252,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-1.66794,-0.729903,-0.48668,-1.89376,
0.108633,0.141015,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-2.12951,
-0.746562,-1.10252,-2.12951,-0.746562,-1.10252,-1.3602,-0.734371,-1.07805,-0.970542,
0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-1.3602,-0.734371,-1.07805,-2.12951,
-0.746562,-1.10252,-2.12951,-0.746562,-1.10252,-2.12951,-0.746562,-1.10252,-0.970542,
0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-1.3602,-0.734371,-1.07805,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-2.12951,
-0.746562,-1.10252,-2.12951,-0.746562,-1.10252,-1.6646,-0.1809,-0.86487,-1.89376,
0.108633,0.141015,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-0.970542,
0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-1.6646,-0.1809,-0.86487,-2.12951,
-0.746562,-1.10252,-2.12951,-0.746562,-1.10252,-2.12951,-0.746562,-1.10252,-1.89376,
0.108633,0.141015,-1.89376,0.108633,0.141015,-1.6646,-0.1809,-0.86487,-0.970542,
0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-1.86728,
1.54818,0.651239,-1.86728,1.54818,0.651239,-1.11497,1.4116,0.562014,-0.68793,
1.00884,1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.789696,1.67778,
-0.423627,-0.789696,1.67778,-0.423627,-1.11497,1.4116,0.562014,-1.86728,1.54818,
0.651239,-1.86728,1.54818,0.651239,-1.86728,1.54818,0.651239,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-1.11497,1.4116,0.562014,-0.789696,1.67778,
-0.423627,-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-1.86728,1.54818,
0.651239,-1.86728,1.54818,0.651239,-1.48299,0.888552,0.750228,-1.89376,0.108633,
0.141015,-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-1.48299,0.888552,0.750228,-1.86728,1.54818,
0.651239,-1.86728,1.54818,0.651239,-1.86728,1.54818,0.651239,-1.89376,0.108633,
0.141015,-1.89376,0.108633,0.141015,-1.48299,0.888552,0.750228,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-1.86728,1.54818,0.651239,
-1.86728,1.54818,0.651239,-1.51691,1.11153,0.122876,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.51691,1.11153,0.122876,-1.86728,1.54818,0.651239,
-1.86728,1.54818,0.651239,-1.86728,1.54818,0.651239,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-1.51691,1.11153,0.122876,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-2.02381,1.04217,-1.06104,
-2.02381,1.04217,-1.06104,-1.62937,0.415344,-0.85104,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.62937,0.415344,-0.85104,-2.02381,1.04217,-1.06104,
-2.02381,1.04217,-1.06104,-2.02381,1.04217,-1.06104,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-1.62937,0.415344,-0.85104,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-2.02381,1.04217,-1.06104,
-2.02381,1.04217,-1.06104,-1.26135,0.938393,-1.03925,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-1.26135,0.938393,-1.03925,-2.02381,1.04217,-1.06104,
-2.02381,1.04217,-1.06104,-2.02381,1.04217,-1.06104,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-1.26135,0.938393,-1.03925,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-2.02381,1.04217,-1.06104,
-2.02381,1.04217,-1.06104,-1.56909,0.942861,-0.447883,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-0.789696,1.67778,-0.423627,
-0.789696,1.67778,-0.423627,-1.56909,0.942861,-0.447883,-2.02381,1.04217,-1.06104,
-2.02381,1.04217,-1.06104,-2.02381,1.04217,-1.06104,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-1.56909,0.942861,-0.447883,-0.789696,1.67778,-0.423627,
2.02381,-1.04217,1.06104,1.62937,-0.415344,0.85104,1.62937,-0.415344,0.85104,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,2.02381,-1.04217,1.06104,
0.970542,-0.0952288,1.6331,1.62937,-0.415344,0.85104,1.62937,-0.415344,0.85104,
2.02381,-1.04217,1.06104,2.02381,-1.04217,1.06104,0.970542,-0.0952288,1.6331,
1.89376,-0.108633,-0.141015,1.62937,-0.415344,0.85104,1.62937,-0.415344,0.85104,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,1.89376,-0.108633,-0.141015,
2.02381,-1.04217,1.06104,1.56909,-0.942861,0.447883,1.56909,-0.942861,0.447883,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,2.02381,-1.04217,1.06104,
1.89376,-0.108633,-0.141015,1.56909,-0.942861,0.447883,1.56909,-0.942861,0.447883,
2.02381,-1.04217,1.06104,2.02381,-1.04217,1.06104,1.89376,-0.108633,-0.141015,
0.789696,-1.67778,0.423627,1.56909,-0.942861,0.447883,1.56909,-0.942861,0.447883,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.789696,-1.67778,0.423627,
2.02381,-1.04217,1.06104,1.26135,-0.938393,1.03925,1.26135,-0.938393,1.03925,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,2.02381,-1.04217,1.06104,
0.789696,-1.67778,0.423627,1.26135,-0.938393,1.03925,1.26135,-0.938393,1.03925,
2.02381,-1.04217,1.06104,2.02381,-1.04217,1.06104,0.789696,-1.67778,0.423627,
0.970542,-0.0952288,1.6331,1.26135,-0.938393,1.03925,1.26135,-0.938393,1.03925,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,0.970542,-0.0952288,1.6331,
1.86728,-1.54818,-0.651239,1.51691,-1.11153,-0.122876,1.51691,-1.11153,-0.122876,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.86728,-1.54818,-0.651239,
0.789696,-1.67778,0.423627,1.51691,-1.11153,-0.122876,1.51691,-1.11153,-0.122876,
1.86728,-1.54818,-0.651239,1.86728,-1.54818,-0.651239,0.789696,-1.67778,0.423627,
1.89376,-0.108633,-0.141015,1.51691,-1.11153,-0.122876,1.51691,-1.11153,-0.122876,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,1.89376,-0.108633,-0.141015,
1.86728,-1.54818,-0.651239,1.48299,-0.888552,-0.750228,1.48299,-0.888552,-0.750228,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,1.86728,-1.54818,-0.651239,
1.89376,-0.108633,-0.141015,1.48299,-0.888552,-0.750228,1.48299,-0.888552,-0.750228,
1.86728,-1.54818,-0.651239,1.86728,-1.54818,-0.651239,1.89376,-0.108633,-0.141015,
0.68793,-1.00884,-1.45843,1.48299,-0.888552,-0.750228,1.48299,-0.888552,-0.750228,
1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.68793,-1.00884,-1.45843,
1.86728,-1.54818,-0.651239,1.11497,-1.4116,-0.562014,1.11497,-1.4116,-0.562014,
0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,1.86728,-1.54818,-0.651239,
0.68793,-1.00884,-1.45843,1.11497,-1.4116,-0.562014,1.11497,-1.4116,-0.562014,
1.86728,-1.54818,-0.651239,1.86728,-1.54818,-0.651239,0.68793,-1.00884,-1.45843,
0.789696,-1.67778,0.423627,1.11497,-1.4116,-0.562014,1.11497,-1.4116,-0.562014,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.789696,-1.67778,0.423627,
2.12951,0.746562,1.10252,1.66794,0.729903,0.48668,1.66794,0.729903,0.48668,1.89376,
-0.108633,-0.141015,1.89376,-0.108633,-0.141015,2.12951,0.746562,1.10252,0.980538,
1.55178,0.498532,1.66794,0.729903,0.48668,1.66794,0.729903,0.48668,2.12951,0.746562,
1.10252,2.12951,0.746562,1.10252,0.980538,1.55178,0.498532,1.89376,-0.108633,-0.141015,
1.66794,0.729903,0.48668,1.66794,0.729903,0.48668,0.980538,1.55178,0.498532,0.980538,
1.55178,0.498532,1.89376,-0.108633,-0.141015,2.12951,0.746562,1.10252,1.6646,
0.1809,0.86487,1.6646,0.1809,0.86487,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,
1.6331,2.12951,0.746562,1.10252,1.89376,-0.108633,-0.141015,1.6646,0.1809,0.86487,
1.6646,0.1809,0.86487,2.12951,0.746562,1.10252,2.12951,0.746562,1.10252,1.89376,
-0.108633,-0.141015,0.970542,-0.0952288,1.6331,1.6646,0.1809,0.86487,1.6646,
0.1809,0.86487,1.89376,-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.970542,
-0.0952288,1.6331,2.12951,0.746562,1.10252,1.3602,0.734371,1.07805,1.3602,0.734371,
1.07805,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,2.12951,0.746562,
1.10252,0.970542,-0.0952288,1.6331,1.3602,0.734371,1.07805,1.3602,0.734371,1.07805,
2.12951,0.746562,1.10252,2.12951,0.746562,1.10252,0.970542,-0.0952288,1.6331,
0.980538,1.55178,0.498532,1.3602,0.734371,1.07805,1.3602,0.734371,1.07805,0.970542,
-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.980538,1.55178,0.498532,1.87624,
-0.0721855,-1.668,1.48598,-0.396553,-1.08915,1.48598,-0.396553,-1.08915,1.89376,
-0.108633,-0.141015,1.89376,-0.108633,-0.141015,1.87624,-0.0721855,-1.668,0.68793,
-1.00884,-1.45843,1.48598,-0.396553,-1.08915,1.48598,-0.396553,-1.08915,1.87624,
-0.0721855,-1.668,1.87624,-0.0721855,-1.668,0.68793,-1.00884,-1.45843,1.89376,
-0.108633,-0.141015,1.48598,-0.396553,-1.08915,1.48598,-0.396553,-1.08915,0.68793,
-1.00884,-1.45843,0.68793,-1.00884,-1.45843,1.89376,-0.108633,-0.141015,1.87624,
-0.0721855,-1.668,1.52529,0.268774,-1.07372,1.52529,0.268774,-1.07372,0.805874,
0.987141,-1.41214,0.805874,0.987141,-1.41214,1.87624,-0.0721855,-1.668,1.89376,
-0.108633,-0.141015,1.52529,0.268774,-1.07372,1.52529,0.268774,-1.07372,1.87624,
-0.0721855,-1.668,1.87624,-0.0721855,-1.668,1.89376,-0.108633,-0.141015,0.805874,
0.987141,-1.41214,1.52529,0.268774,-1.07372,1.52529,0.268774,-1.07372,1.89376,
-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.805874,0.987141,-1.41214,1.87624,
-0.0721855,-1.668,1.12335,-0.0312948,-1.51286,1.12335,-0.0312948,-1.51286,0.68793,
-1.00884,-1.45843,0.68793,-1.00884,-1.45843,1.87624,-0.0721855,-1.668,0.805874,
0.987141,-1.41214,1.12335,-0.0312948,-1.51286,1.12335,-0.0312948,-1.51286,1.87624,
-0.0721855,-1.668,1.87624,-0.0721855,-1.668,0.805874,0.987141,-1.41214,0.68793,
-1.00884,-1.45843,1.12335,-0.0312948,-1.51286,1.12335,-0.0312948,-1.51286,0.805874,
0.987141,-1.41214,0.805874,0.987141,-1.41214,0.68793,-1.00884,-1.45843,2.03831,
1.34605,-0.584115,1.57931,0.741518,-0.712423,1.57931,0.741518,-0.712423,1.89376,
-0.108633,-0.141015,1.89376,-0.108633,-0.141015,2.03831,1.34605,-0.584115,0.805874,
0.987141,-1.41214,1.57931,0.741518,-0.712423,1.57931,0.741518,-0.712423,2.03831,
1.34605,-0.584115,2.03831,1.34605,-0.584115,0.805874,0.987141,-1.41214,1.89376,
-0.108633,-0.141015,1.57931,0.741518,-0.712423,1.57931,0.741518,-0.712423,0.805874,
0.987141,-1.41214,0.805874,0.987141,-1.41214,1.89376,-0.108633,-0.141015,2.03831,
1.34605,-0.584115,1.63754,0.929731,-0.0755326,1.63754,0.929731,-0.0755326,0.980538,
1.55178,0.498532,0.980538,1.55178,0.498532,2.03831,1.34605,-0.584115,1.89376,
-0.108633,-0.141015,1.63754,0.929731,-0.0755326,1.63754,0.929731,-0.0755326,2.03831,
1.34605,-0.584115,2.03831,1.34605,-0.584115,1.89376,-0.108633,-0.141015,0.980538,
1.55178,0.498532,1.63754,0.929731,-0.0755326,1.63754,0.929731,-0.0755326,1.89376,
-0.108633,-0.141015,1.89376,-0.108633,-0.141015,0.980538,1.55178,0.498532,2.03831,
1.34605,-0.584115,1.27491,1.29499,-0.499241,1.27491,1.29499,-0.499241,0.805874,
0.987141,-1.41214,0.805874,0.987141,-1.41214,2.03831,1.34605,-0.584115,0.980538,
1.55178,0.498532,1.27491,1.29499,-0.499241,1.27491,1.29499,-0.499241,2.03831,
1.34605,-0.584115,2.03831,1.34605,-0.584115,0.980538,1.55178,0.498532,0.805874,
0.987141,-1.41214,1.27491,1.29499,-0.499241,1.27491,1.29499,-0.499241,0.980538,
1.55178,0.498532,0.980538,1.55178,0.498532,0.805874,0.987141,-1.41214,0.528584,
-1.52874,1.92127,0.762941,-1.10058,1.326,0.762941,-1.10058,1.326,0.789696,-1.67778,
0.423627,0.789696,-1.67778,0.423627,0.528584,-1.52874,1.92127,0.970542,-0.0952288,
1.6331,0.762941,-1.10058,1.326,0.762941,-1.10058,1.326,0.528584,-1.52874,1.92127,
0.528584,-1.52874,1.92127,0.970542,-0.0952288,1.6331,0.789696,-1.67778,0.423627,
0.762941,-1.10058,1.326,0.762941,-1.10058,1.326,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.789696,-1.67778,0.423627,0.528584,-1.52874,1.92127,
0.170802,-1.39789,1.25235,0.170802,-1.39789,1.25235,-0.805874,-0.987141,1.41214,
-0.805874,-0.987141,1.41214,0.528584,-1.52874,1.92127,0.789696,-1.67778,0.423627,
0.170802,-1.39789,1.25235,0.170802,-1.39789,1.25235,0.528584,-1.52874,1.92127,
0.528584,-1.52874,1.92127,0.789696,-1.67778,0.423627,-0.805874,-0.987141,1.41214,
0.170802,-1.39789,1.25235,0.170802,-1.39789,1.25235,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,-0.805874,-0.987141,1.41214,0.528584,-1.52874,1.92127,
0.231084,-0.870371,1.6555,0.231084,-0.870371,1.6555,0.970542,-0.0952288,1.6331,
0.970542,-0.0952288,1.6331,0.528584,-1.52874,1.92127,-0.805874,-0.987141,1.41214,
0.231084,-0.870371,1.6555,0.231084,-0.870371,1.6555,0.528584,-1.52874,1.92127,
0.528584,-1.52874,1.92127,-0.805874,-0.987141,1.41214,0.970542,-0.0952288,1.6331,
0.231084,-0.870371,1.6555,0.231084,-0.870371,1.6555,-0.805874,-0.987141,1.41214,
-0.805874,-0.987141,1.41214,0.970542,-0.0952288,1.6331,0.275315,-2.34749,-0.849256,
0.0281577,-1.85902,-0.308053,0.0281577,-1.85902,-0.308053,0.789696,-1.67778,0.423627,
0.789696,-1.67778,0.423627,0.275315,-2.34749,-0.849256,-0.980538,-1.55178,-0.498532,
0.0281577,-1.85902,-0.308053,0.0281577,-1.85902,-0.308053,0.275315,-2.34749,-0.849256,
0.275315,-2.34749,-0.849256,-0.980538,-1.55178,-0.498532,0.789696,-1.67778,0.423627,
0.0281577,-1.85902,-0.308053,0.0281577,-1.85902,-0.308053,-0.980538,-1.55178,
-0.498532,-0.980538,-1.55178,-0.498532,0.789696,-1.67778,0.423627,0.275315,-2.34749,
-0.849256,0.584314,-1.67804,-0.62802,0.584314,-1.67804,-0.62802,0.68793,-1.00884,
-1.45843,0.68793,-1.00884,-1.45843,0.275315,-2.34749,-0.849256,0.789696,-1.67778,
0.423627,0.584314,-1.67804,-0.62802,0.584314,-1.67804,-0.62802,0.275315,-2.34749,
-0.849256,0.275315,-2.34749,-0.849256,0.789696,-1.67778,0.423627,0.68793,-1.00884,
-1.45843,0.584314,-1.67804,-0.62802,0.584314,-1.67804,-0.62802,0.789696,-1.67778,
0.423627,0.789696,-1.67778,0.423627,0.68793,-1.00884,-1.45843,0.275315,-2.34749,
-0.849256,-0.00576429,-1.63604,-0.935406,-0.00576429,-1.63604,-0.935406,-0.980538,
-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,0.275315,-2.34749,-0.849256,0.68793,
-1.00884,-1.45843,-0.00576429,-1.63604,-0.935406,-0.00576429,-1.63604,-0.935406,
0.275315,-2.34749,-0.849256,0.275315,-2.34749,-0.849256,0.68793,-1.00884,-1.45843,
-0.980538,-1.55178,-0.498532,-0.00576429,-1.63604,-0.935406,-0.00576429,-1.63604,
-0.935406,0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.980538,-1.55178,
-0.498532,-0.552046,-2.33547,0.740642,-0.189408,-1.6668,0.858803,-0.189408,-1.6668,
0.858803,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,-0.552046,-2.33547,
0.740642,-0.805874,-0.987141,1.41214,-0.189408,-1.6668,0.858803,-0.189408,-1.6668,
0.858803,-0.552046,-2.33547,0.740642,-0.552046,-2.33547,0.740642,-0.805874,-0.987141,
1.41214,0.789696,-1.67778,0.423627,-0.189408,-1.6668,0.858803,-0.189408,-1.6668,
0.858803,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,0.789696,-1.67778,
0.423627,-0.552046,-2.33547,0.740642,-0.247629,-1.85501,0.221912,-0.247629,-1.85501,
0.221912,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.552046,-2.33547,
0.740642,0.789696,-1.67778,0.423627,-0.247629,-1.85501,0.221912,-0.247629,-1.85501,
0.221912,-0.552046,-2.33547,0.740642,-0.552046,-2.33547,0.740642,0.789696,-1.67778,
0.423627,-0.980538,-1.55178,-0.498532,-0.247629,-1.85501,0.221912,-0.247629,-1.85501,
0.221912,0.789696,-1.67778,0.423627,0.789696,-1.67778,0.423627,-0.980538,-1.55178,
-0.498532,-0.552046,-2.33547,0.740642,-0.779486,-1.6248,0.551417,-0.779486,-1.6248,
0.551417,-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.552046,-2.33547,
0.740642,-0.980538,-1.55178,-0.498532,-0.779486,-1.6248,0.551417,-0.779486,-1.6248,
0.551417,-0.552046,-2.33547,0.740642,-0.552046,-2.33547,0.740642,-0.980538,-1.55178,
-0.498532,-0.805874,-0.987141,1.41214,-0.779486,-1.6248,0.551417,-0.779486,-1.6248,
0.551417,-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.805874,-0.987141,
1.41214,0.69961,1.36549,1.9884,0.883563,0.94068,1.37334,0.883563,0.94068,1.37334,
0.970542,-0.0952288,1.6331,0.970542,-0.0952288,1.6331,0.69961,1.36549,1.9884,
0.980538,1.55178,0.498532,0.883563,0.94068,1.37334,0.883563,0.94068,1.37334,
0.69961,1.36549,1.9884,0.69961,1.36549,1.9884,0.980538,1.55178,0.498532,0.970542,
-0.0952288,1.6331,0.883563,0.94068,1.37334,0.883563,0.94068,1.37334,0.980538,
1.55178,0.498532,0.980538,1.55178,0.498532,0.970542,-0.0952288,1.6331,0.69961,
1.36549,1.9884,0.327407,0.7597,1.69331,0.327407,0.7597,1.69331,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,0.69961,1.36549,1.9884,0.970542,-0.0952288,1.6331,
0.327407,0.7597,1.69331,0.327407,0.7597,1.69331,0.69961,1.36549,1.9884,0.69961,
1.36549,1.9884,0.970542,-0.0952288,1.6331,-0.68793,1.00884,1.45843,0.327407,0.7597,
1.69331,0.327407,0.7597,1.69331,0.970542,-0.0952288,1.6331,0.970542,-0.0952288,
1.6331,-0.68793,1.00884,1.45843,0.69961,1.36549,1.9884,0.330739,1.3087,1.31512,
0.330739,1.3087,1.31512,0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.69961,
1.36549,1.9884,-0.68793,1.00884,1.45843,0.330739,1.3087,1.31512,0.330739,1.3087,
1.31512,0.69961,1.36549,1.9884,0.69961,1.36549,1.9884,-0.68793,1.00884,1.45843,
0.980538,1.55178,0.498532,0.330739,1.3087,1.31512,0.330739,1.3087,1.31512,-0.68793,
1.00884,1.45843,-0.68793,1.00884,1.45843,0.980538,1.55178,0.498532,-0.289817,-0.0407247,
2.49441,-0.00240177,0.290962,1.86198,-0.00240177,0.290962,1.86198,0.970542,-0.0952288,
1.6331,0.970542,-0.0952288,1.6331,-0.289817,-0.0407247,2.49441,-0.68793,1.00884,
1.45843,-0.00240177,0.290962,1.86198,-0.00240177,0.290962,1.86198,-0.289817,-0.0407247,
2.49441,-0.289817,-0.0407247,2.49441,-0.68793,1.00884,1.45843,0.970542,-0.0952288,
1.6331,-0.00240177,0.290962,1.86198,-0.00240177,0.290962,1.86198,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,0.970542,-0.0952288,1.6331,-0.289817,-0.0407247,
2.49441,-0.0417164,-0.374365,1.84655,-0.0417164,-0.374365,1.84655,-0.805874,-0.987141,
1.41214,-0.805874,-0.987141,1.41214,-0.289817,-0.0407247,2.49441,0.970542,-0.0952288,
1.6331,-0.0417164,-0.374365,1.84655,-0.0417164,-0.374365,1.84655,-0.289817,-0.0407247,
2.49441,-0.289817,-0.0407247,2.49441,0.970542,-0.0952288,1.6331,-0.805874,-0.987141,
1.41214,-0.0417164,-0.374365,1.84655,-0.0417164,-0.374365,1.84655,0.970542,-0.0952288,
1.6331,0.970542,-0.0952288,1.6331,-0.805874,-0.987141,1.41214,-0.289817,-0.0407247,
2.49441,-0.59454,-0.00634191,1.78833,-0.59454,-0.00634191,1.78833,-0.68793,1.00884,
1.45843,-0.68793,1.00884,1.45843,-0.289817,-0.0407247,2.49441,-0.805874,-0.987141,
1.41214,-0.59454,-0.00634191,1.78833,-0.59454,-0.00634191,1.78833,-0.289817,-0.0407247,
2.49441,-0.289817,-0.0407247,2.49441,-0.805874,-0.987141,1.41214,-0.68793,1.00884,
1.45843,-0.59454,-0.00634191,1.78833,-0.59454,-0.00634191,1.78833,-0.805874,-0.987141,
1.41214,-0.805874,-0.987141,1.41214,-0.68793,1.00884,1.45843,0.289817,0.0407247,
-2.49441,0.00240177,-0.290962,-1.86198,0.00240177,-0.290962,-1.86198,0.68793,
-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.289817,0.0407247,-2.49441,-0.970542,
0.0952288,-1.6331,0.00240177,-0.290962,-1.86198,0.00240177,-0.290962,-1.86198,
0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,-0.970542,0.0952288,-1.6331,
0.68793,-1.00884,-1.45843,0.00240177,-0.290962,-1.86198,0.00240177,-0.290962,-1.86198,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,0.68793,-1.00884,-1.45843,
0.289817,0.0407247,-2.49441,0.59454,0.00634191,-1.78833,0.59454,0.00634191,-1.78833,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.289817,0.0407247,-2.49441,
0.68793,-1.00884,-1.45843,0.59454,0.00634191,-1.78833,0.59454,0.00634191,-1.78833,
0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,0.68793,-1.00884,-1.45843,
0.805874,0.987141,-1.41214,0.59454,0.00634191,-1.78833,0.59454,0.00634191,-1.78833,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,0.805874,0.987141,-1.41214,
0.289817,0.0407247,-2.49441,0.0417164,0.374365,-1.84655,0.0417164,0.374365,-1.84655,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,0.289817,0.0407247,-2.49441,
0.805874,0.987141,-1.41214,0.0417164,0.374365,-1.84655,0.0417164,0.374365,-1.84655,
0.289817,0.0407247,-2.49441,0.289817,0.0407247,-2.49441,0.805874,0.987141,-1.41214,
-0.970542,0.0952288,-1.6331,0.0417164,0.374365,-1.84655,0.0417164,0.374365,-1.84655,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,-0.970542,0.0952288,-1.6331,
-0.69961,-1.36549,-1.9884,-0.330739,-1.3087,-1.31512,-0.330739,-1.3087,-1.31512,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.69961,-1.36549,-1.9884,
-0.980538,-1.55178,-0.498532,-0.330739,-1.3087,-1.31512,-0.330739,-1.3087,-1.31512,
-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,-1.9884,-0.980538,-1.55178,-0.498532,
0.68793,-1.00884,-1.45843,-0.330739,-1.3087,-1.31512,-0.330739,-1.3087,-1.31512,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,0.68793,-1.00884,-1.45843,
-0.69961,-1.36549,-1.9884,-0.327407,-0.7597,-1.69331,-0.327407,-0.7597,-1.69331,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.69961,-1.36549,-1.9884,
0.68793,-1.00884,-1.45843,-0.327407,-0.7597,-1.69331,-0.327407,-0.7597,-1.69331,
-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,-1.9884,0.68793,-1.00884,-1.45843,
-0.970542,0.0952288,-1.6331,-0.327407,-0.7597,-1.69331,-0.327407,-0.7597,-1.69331,
0.68793,-1.00884,-1.45843,0.68793,-1.00884,-1.45843,-0.970542,0.0952288,-1.6331,
-0.69961,-1.36549,-1.9884,-0.883563,-0.94068,-1.37334,-0.883563,-0.94068,-1.37334,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-0.69961,-1.36549,-1.9884,
-0.970542,0.0952288,-1.6331,-0.883563,-0.94068,-1.37334,-0.883563,-0.94068,-1.37334,
-0.69961,-1.36549,-1.9884,-0.69961,-1.36549,-1.9884,-0.970542,0.0952288,-1.6331,
-0.980538,-1.55178,-0.498532,-0.883563,-0.94068,-1.37334,-0.883563,-0.94068,-1.37334,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.980538,-1.55178,-0.498532,
0.552046,2.33547,-0.740642,0.779486,1.6248,-0.551417,0.779486,1.6248,-0.551417,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,0.552046,2.33547,-0.740642,
0.805874,0.987141,-1.41214,0.779486,1.6248,-0.551417,0.779486,1.6248,-0.551417,
0.552046,2.33547,-0.740642,0.552046,2.33547,-0.740642,0.805874,0.987141,-1.41214,
0.980538,1.55178,0.498532,0.779486,1.6248,-0.551417,0.779486,1.6248,-0.551417,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.980538,1.55178,0.498532,
0.552046,2.33547,-0.740642,0.247629,1.85501,-0.221912,0.247629,1.85501,-0.221912,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,0.552046,2.33547,-0.740642,
0.980538,1.55178,0.498532,0.247629,1.85501,-0.221912,0.247629,1.85501,-0.221912,
0.552046,2.33547,-0.740642,0.552046,2.33547,-0.740642,0.980538,1.55178,0.498532,
-0.789696,1.67778,-0.423627,0.247629,1.85501,-0.221912,0.247629,1.85501,-0.221912,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,-0.789696,1.67778,-0.423627,
0.552046,2.33547,-0.740642,0.189408,1.6668,-0.858803,0.189408,1.6668,-0.858803,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,0.552046,2.33547,-0.740642,
-0.789696,1.67778,-0.423627,0.189408,1.6668,-0.858803,0.189408,1.6668,-0.858803,
0.552046,2.33547,-0.740642,0.552046,2.33547,-0.740642,-0.789696,1.67778,-0.423627,
0.805874,0.987141,-1.41214,0.189408,1.6668,-0.858803,0.189408,1.6668,-0.858803,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,0.805874,0.987141,-1.41214,
-0.275315,2.34749,0.849256,-0.0281577,1.85902,0.308053,-0.0281577,1.85902,0.308053,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,-0.275315,2.34749,0.849256,
-0.789696,1.67778,-0.423627,-0.0281577,1.85902,0.308053,-0.0281577,1.85902,0.308053,
-0.275315,2.34749,0.849256,-0.275315,2.34749,0.849256,-0.789696,1.67778,-0.423627,
0.980538,1.55178,0.498532,-0.0281577,1.85902,0.308053,-0.0281577,1.85902,0.308053,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,0.980538,1.55178,0.498532,
-0.275315,2.34749,0.849256,0.00576429,1.63604,0.935406,0.00576429,1.63604,0.935406,
-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.275315,2.34749,0.849256,
0.980538,1.55178,0.498532,0.00576429,1.63604,0.935406,0.00576429,1.63604,0.935406,
-0.275315,2.34749,0.849256,-0.275315,2.34749,0.849256,0.980538,1.55178,0.498532,
-0.68793,1.00884,1.45843,0.00576429,1.63604,0.935406,0.00576429,1.63604,0.935406,
0.980538,1.55178,0.498532,0.980538,1.55178,0.498532,-0.68793,1.00884,1.45843,
-0.275315,2.34749,0.849256,-0.584314,1.67804,0.62802,-0.584314,1.67804,0.62802,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.275315,2.34749,0.849256,
-0.68793,1.00884,1.45843,-0.584314,1.67804,0.62802,-0.584314,1.67804,0.62802,
-0.275315,2.34749,0.849256,-0.275315,2.34749,0.849256,-0.68793,1.00884,1.45843,
-0.789696,1.67778,-0.423627,-0.584314,1.67804,0.62802,-0.584314,1.67804,0.62802,
-0.68793,1.00884,1.45843,-0.68793,1.00884,1.45843,-0.789696,1.67778,-0.423627,
-0.528584,1.52874,-1.92127,-0.231084,0.870371,-1.6555,-0.231084,0.870371,-1.6555,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,-0.528584,1.52874,-1.92127,
-0.970542,0.0952288,-1.6331,-0.231084,0.870371,-1.6555,-0.231084,0.870371,-1.6555,
-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,-0.970542,0.0952288,-1.6331,
0.805874,0.987141,-1.41214,-0.231084,0.870371,-1.6555,-0.231084,0.870371,-1.6555,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,0.805874,0.987141,-1.41214,
-0.528584,1.52874,-1.92127,-0.170802,1.39789,-1.25235,-0.170802,1.39789,-1.25235,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.528584,1.52874,-1.92127,
0.805874,0.987141,-1.41214,-0.170802,1.39789,-1.25235,-0.170802,1.39789,-1.25235,
-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,0.805874,0.987141,-1.41214,
-0.789696,1.67778,-0.423627,-0.170802,1.39789,-1.25235,-0.170802,1.39789,-1.25235,
0.805874,0.987141,-1.41214,0.805874,0.987141,-1.41214,-0.789696,1.67778,-0.423627,
-0.528584,1.52874,-1.92127,-0.762941,1.10058,-1.326,-0.762941,1.10058,-1.326,
-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,-1.6331,-0.528584,1.52874,-1.92127,
-0.789696,1.67778,-0.423627,-0.762941,1.10058,-1.326,-0.762941,1.10058,-1.326,
-0.528584,1.52874,-1.92127,-0.528584,1.52874,-1.92127,-0.789696,1.67778,-0.423627,
-0.970542,0.0952288,-1.6331,-0.762941,1.10058,-1.326,-0.762941,1.10058,-1.326,
-0.789696,1.67778,-0.423627,-0.789696,1.67778,-0.423627,-0.970542,0.0952288,-1.6331,
-2.03831,-1.34605,0.584115,-1.57931,-0.741518,0.712423,-1.57931,-0.741518,0.712423,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-2.03831,-1.34605,0.584115,
-1.89376,0.108633,0.141015,-1.57931,-0.741518,0.712423,-1.57931,-0.741518,0.712423,
-2.03831,-1.34605,0.584115,-2.03831,-1.34605,0.584115,-1.89376,0.108633,0.141015,
-0.805874,-0.987141,1.41214,-1.57931,-0.741518,0.712423,-1.57931,-0.741518,0.712423,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-0.805874,-0.987141,1.41214,
-2.03831,-1.34605,0.584115,-1.27491,-1.29499,0.499241,-1.27491,-1.29499,0.499241,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-2.03831,-1.34605,0.584115,
-0.805874,-0.987141,1.41214,-1.27491,-1.29499,0.499241,-1.27491,-1.29499,0.499241,
-2.03831,-1.34605,0.584115,-2.03831,-1.34605,0.584115,-0.805874,-0.987141,1.41214,
-0.980538,-1.55178,-0.498532,-1.27491,-1.29499,0.499241,-1.27491,-1.29499,0.499241,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-0.980538,-1.55178,-0.498532,
-2.03831,-1.34605,0.584115,-1.63754,-0.929731,0.0755326,-1.63754,-0.929731,0.0755326,
-1.89376,0.108633,0.141015,-1.89376,0.108633,0.141015,-2.03831,-1.34605,0.584115,
-0.980538,-1.55178,-0.498532,-1.63754,-0.929731,0.0755326,-1.63754,-0.929731,0.0755326,
-2.03831,-1.34605,0.584115,-2.03831,-1.34605,0.584115,-0.980538,-1.55178,-0.498532,
-1.89376,0.108633,0.141015,-1.63754,-0.929731,0.0755326,-1.63754,-0.929731,0.0755326,
-0.980538,-1.55178,-0.498532,-0.980538,-1.55178,-0.498532,-1.89376,0.108633,0.141015,
-1.87624,0.0721856,1.668,-1.12335,0.0312948,1.51286,-1.12335,0.0312948,1.51286,
-0.805874,-0.987141,1.41214,-0.805874,-0.987141,1.41214,-1.87624,0.0721856,1.668,
-0.68793,1.00884,1.45843,-1.12335,0.0312948,1.51286,-1.12335,0.0312948,1.51286,
-1.87624,0.0721856,1.668,-1.87624,0.0721856,1.668,-0.68793,1.00884,1.45843,-0.805874,
-0.987141,1.41214,-1.12335,0.0312948,1.51286,-1.12335,0.0312948,1.51286,-0.68793,
1.00884,1.45843,-0.68793,1.00884,1.45843,-0.805874,-0.987141,1.41214,-1.87624,
0.0721856,1.668,-1.52529,-0.268774,1.07372,-1.52529,-0.268774,1.07372,-1.89376,
0.108633,0.141015,-1.89376,0.108633,0.141015,-1.87624,0.0721856,1.668,-0.805874,
-0.987141,1.41214,-1.52529,-0.268774,1.07372,-1.52529,-0.268774,1.07372,-1.87624,
0.0721856,1.668,-1.87624,0.0721856,1.668,-0.805874,-0.987141,1.41214,-1.89376,
0.108633,0.141015,-1.52529,-0.268774,1.07372,-1.52529,-0.268774,1.07372,-0.805874,
-0.987141,1.41214,-0.805874,-0.987141,1.41214,-1.89376,0.108633,0.141015,-1.87624,
0.0721856,1.668,-1.48598,0.396553,1.08915,-1.48598,0.396553,1.08915,-0.68793,
1.00884,1.45843,-0.68793,1.00884,1.45843,-1.87624,0.0721856,1.668,-1.89376,0.108633,
0.141015,-1.48598,0.396553,1.08915,-1.48598,0.396553,1.08915,-1.87624,0.0721856,
1.668,-1.87624,0.0721856,1.668,-1.89376,0.108633,0.141015,-0.68793,1.00884,1.45843,
-1.48598,0.396553,1.08915,-1.48598,0.396553,1.08915,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-0.68793,1.00884,1.45843,-2.12951,-0.746562,-1.10252,
-1.66794,-0.729903,-0.48668,-1.66794,-0.729903,-0.48668,-0.980538,-1.55178,-0.498532,
-0.980538,-1.55178,-0.498532,-2.12951,-0.746562,-1.10252,-1.89376,0.108633,0.141015,
-1.66794,-0.729903,-0.48668,-1.66794,-0.729903,-0.48668,-2.12951,-0.746562,-1.10252,
-2.12951,-0.746562,-1.10252,-1.89376,0.108633,0.141015,-0.980538,-1.55178,-0.498532,
-1.66794,-0.729903,-0.48668,-1.66794,-0.729903,-0.48668,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-0.980538,-1.55178,-0.498532,-2.12951,-0.746562,-1.10252,
-1.3602,-0.734371,-1.07805,-1.3602,-0.734371,-1.07805,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-2.12951,-0.746562,-1.10252,-0.980538,-1.55178,-0.498532,
-1.3602,-0.734371,-1.07805,-1.3602,-0.734371,-1.07805,-2.12951,-0.746562,-1.10252,
-2.12951,-0.746562,-1.10252,-0.980538,-1.55178,-0.498532,-0.970542,0.0952288,-1.6331,
-1.3602,-0.734371,-1.07805,-1.3602,-0.734371,-1.07805,-0.980538,-1.55178,-0.498532,
-0.980538,-1.55178,-0.498532,-0.970542,0.0952288,-1.6331,-2.12951,-0.746562,-1.10252,
-1.6646,-0.1809,-0.86487,-1.6646,-0.1809,-0.86487,-1.89376,0.108633,0.141015,
-1.89376,0.108633,0.141015,-2.12951,-0.746562,-1.10252,-0.970542,0.0952288,-1.6331,
-1.6646,-0.1809,-0.86487,-1.6646,-0.1809,-0.86487,-2.12951,-0.746562,-1.10252,
-2.12951,-0.746562,-1.10252,-0.970542,0.0952288,-1.6331,-1.89376,0.108633,0.141015,
-1.6646,-0.1809,-0.86487,-1.6646,-0.1809,-0.86487,-0.970542,0.0952288,-1.6331,
-0.970542,0.0952288,-1.6331,-1.89376,0.108633,0.141015,-1.86728,1.54818,0.651239,
-1.11497,1.4116,0.562014,-1.11497,1.4116,0.562014,-0.68793,1.00884,1.45843,-0.68793,
1.00884,1.45843,-1.86728,1.54818,0.651239,-0.789696,1.67778,-0.423627,-1.11497,
1.4116,0.562014,-1.11497,1.4116,0.562014,-1.86728,1.54818,0.651239,-1.86728,1.54818,
0.651239,-0.789696,1.67778,-0.423627,-0.68793,1.00884,1.45843,-1.11497,1.4116,
0.562014,-1.11497,1.4116,0.562014,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-0.68793,1.00884,1.45843,-1.86728,1.54818,0.651239,-1.48299,0.888552,
0.750228,-1.48299,0.888552,0.750228,-1.89376,0.108633,0.141015,-1.89376,0.108633,
0.141015,-1.86728,1.54818,0.651239,-0.68793,1.00884,1.45843,-1.48299,0.888552,
0.750228,-1.48299,0.888552,0.750228,-1.86728,1.54818,0.651239,-1.86728,1.54818,
0.651239,-0.68793,1.00884,1.45843,-1.89376,0.108633,0.141015,-1.48299,0.888552,
0.750228,-1.48299,0.888552,0.750228,-0.68793,1.00884,1.45843,-0.68793,1.00884,
1.45843,-1.89376,0.108633,0.141015,-1.86728,1.54818,0.651239,-1.51691,1.11153,
0.122876,-1.51691,1.11153,0.122876,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-1.86728,1.54818,0.651239,-1.89376,0.108633,0.141015,-1.51691,1.11153,
0.122876,-1.51691,1.11153,0.122876,-1.86728,1.54818,0.651239,-1.86728,1.54818,
0.651239,-1.89376,0.108633,0.141015,-0.789696,1.67778,-0.423627,-1.51691,1.11153,
0.122876,-1.51691,1.11153,0.122876,-1.89376,0.108633,0.141015,-1.89376,0.108633,
0.141015,-0.789696,1.67778,-0.423627,-2.02381,1.04217,-1.06104,-1.62937,0.415344,
-0.85104,-1.62937,0.415344,-0.85104,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,
-1.6331,-2.02381,1.04217,-1.06104,-1.89376,0.108633,0.141015,-1.62937,0.415344,
-0.85104,-1.62937,0.415344,-0.85104,-2.02381,1.04217,-1.06104,-2.02381,1.04217,
-1.06104,-1.89376,0.108633,0.141015,-0.970542,0.0952288,-1.6331,-1.62937,0.415344,
-0.85104,-1.62937,0.415344,-0.85104,-1.89376,0.108633,0.141015,-1.89376,0.108633,
0.141015,-0.970542,0.0952288,-1.6331,-2.02381,1.04217,-1.06104,-1.26135,0.938393,
-1.03925,-1.26135,0.938393,-1.03925,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-2.02381,1.04217,-1.06104,-0.970542,0.0952288,-1.6331,-1.26135,0.938393,
-1.03925,-1.26135,0.938393,-1.03925,-2.02381,1.04217,-1.06104,-2.02381,1.04217,
-1.06104,-0.970542,0.0952288,-1.6331,-0.789696,1.67778,-0.423627,-1.26135,0.938393,
-1.03925,-1.26135,0.938393,-1.03925,-0.970542,0.0952288,-1.6331,-0.970542,0.0952288,
-1.6331,-0.789696,1.67778,-0.423627,-2.02381,1.04217,-1.06104,-1.56909,0.942861,
-0.447883,-1.56909,0.942861,-0.447883,-1.89376,0.108633,0.141015,-1.89376,0.108633,
0.141015,-2.02381,1.04217,-1.06104,-0.789696,1.67778,-0.423627,-1.56909,0.942861,
-0.447883,-1.56909,0.942861,-0.447883,-2.02381,1.04217,-1.06104,-2.02381,1.04217,
-1.06104,-0.789696,1.67778,-0.423627,-1.89376,0.108633,0.141015,-1.56909,0.942861,
-0.447883,-1.56909,0.942861,-0.447883,-0.789696,1.67778,-0.423627,-0.789696,1.67778,
-0.423627,-1.89376,0.108633,0.141015
      };
    };
  };
 };



 macro mk_torus {

  olink out_fld => mesh;
  olink out_obj => obj.obj;

  GDM.DataObject obj {
     in => <-.mesh;
  };

  Polytri set1 {
      npolys=1;
      poly_connect_list[2] = {0,158};
  };

  Mesh  mesh {
   nnodes = 321;
   nspace = 3;
   ncell_sets = 1;
   cell_set[ncell_sets] => {<-.set1};

   coordinates {
      units = "inches";
      values = {
   1.,2.,0.,0.707107,2.70711,0.,1.,1.41421,1.41421,0.707107,1.91421,1.91421,
1.,1.22465e-16,2.,0.707107,1.65762e-16,2.70711,1.,-1.41421,1.41421,0.707107,-1.91421,
1.91421,1.,-2.,2.44929e-16,0.707107,-2.70711,3.31525e-16,1.,-1.41421,-1.41421,
0.707107,-1.91421,-1.91421,1.,-3.67394e-16,-2.,0.707107,-4.97287e-16,-2.70711,
1.,1.41421,-1.41421,0.707107,1.91421,-1.91421,1.,2.,-4.89859e-16,0.707107,2.70711,
-6.6305e-16,0.707107,2.70711,-6.6305e-16,0.707107,2.70711,0.,0.707107,2.70711,
0.,6.12323e-17,3.,0.,0.707107,1.91421,1.91421,6.12323e-17,2.12132,2.12132,0.707107,
1.65762e-16,2.70711,6.12323e-17,1.83697e-16,3.,0.707107,-1.91421,1.91421,6.12323e-17,
-2.12132,2.12132,0.707107,-2.70711,3.31525e-16,6.12323e-17,-3.,3.67394e-16,0.707107,
-1.91421,-1.91421,6.12323e-17,-2.12132,-2.12132,0.707107,-4.97287e-16,-2.70711,
6.12323e-17,-5.51091e-16,-3.,0.707107,1.91421,-1.91421,6.12323e-17,2.12132,-2.12132,
0.707107,2.70711,-6.6305e-16,6.12323e-17,3.,-7.34788e-16,6.12323e-17,3.,
-7.34788e-16,6.12323e-17,3.,0.,6.12323e-17,3.,0.,-0.707107,2.70711,0.,6.12323e-17,
2.12132,2.12132,-0.707107,1.91421,1.91421,6.12323e-17,1.83697e-16,3.,-0.707107,
1.65762e-16,2.70711,6.12323e-17,-2.12132,2.12132,-0.707107,-1.91421,1.91421,
6.12323e-17,-3.,3.67394e-16,-0.707107,-2.70711,3.31525e-16,6.12323e-17,-2.12132,
-2.12132,-0.707107,-1.91421,-1.91421,6.12323e-17,-5.51091e-16,-3.,-0.707107,-4.97287e-16,
-2.70711,6.12323e-17,2.12132,-2.12132,-0.707107,1.91421,-1.91421,6.12323e-17,3.,
-7.34788e-16,-0.707107,2.70711,-6.6305e-16,-0.707107,2.70711,-6.6305e-16,-0.707107,
2.70711,0.,-0.707107,2.70711,0.,-1.,2.,0.,-0.707107,1.91421,1.91421,-1.,
1.41421,1.41421,-0.707107,1.65762e-16,2.70711,-1.,1.22465e-16,2.,-0.707107,-1.91421,
1.91421,-1.,-1.41421,1.41421,-0.707107,-2.70711,3.31525e-16,-1.,-2.,2.44929e-16,
-0.707107,-1.91421,-1.91421,-1.,-1.41421,-1.41421,-0.707107,-4.97287e-16,-2.70711,
-1.,-3.67394e-16,-2.,-0.707107,1.91421,-1.91421,-1.,1.41421,-1.41421,-0.707107,
2.70711,-6.6305e-16,-1.,2.,-4.89859e-16,-1.,2.,-4.89859e-16,-1.,2.,0.,-1.,
2.,0.,-0.707107,1.29289,0.,-1.,1.41421,1.41421,-0.707107,0.914214,0.914214,-1.,
1.22465e-16,2.,-0.707107,7.91669e-17,1.29289,-1.,-1.41421,1.41421,-0.707107,-0.914214,
0.914214,-1.,-2.,2.44929e-16,-0.707107,-1.29289,1.58334e-16,-1.,-1.41421,-1.41421,
-0.707107,-0.914214,-0.914214,-1.,-3.67394e-16,-2.,-0.707107,-2.375e-16,-1.29289,
-1.,1.41421,-1.41421,-0.707107,0.914214,-0.914214,-1.,2.,-4.89859e-16,-0.707107,
1.29289,-3.16667e-16,-0.707107,1.29289,-3.16667e-16,-0.707107,1.29289,0.,
-0.707107,1.29289,0.,-1.83697e-16,1.,0.,-0.707107,0.914214,0.914214,-1.83697e-16,
0.707107,0.707107,-0.707107,7.91669e-17,1.29289,-1.83697e-16,6.12323e-17,1.,
-0.707107,-0.914214,0.914214,-1.83697e-16,-0.707107,0.707107,-0.707107,-1.29289,
1.58334e-16,-1.83697e-16,-1.,1.22465e-16,-0.707107,-0.914214,-0.914214,-1.83697e-16,
-0.707107,-0.707107,-0.707107,-2.375e-16,-1.29289,-1.83697e-16,-1.83697e-16,-1.,
-0.707107,0.914214,-0.914214,-1.83697e-16,0.707107,-0.707107,-0.707107,1.29289,
-3.16667e-16,-1.83697e-16,1.,-2.44929e-16,-1.83697e-16,1.,-2.44929e-16,-1.83697e-16,
1.,0.,-1.83697e-16,1.,0.,0.707107,1.29289,0.,-1.83697e-16,0.707107,0.707107,0.707107,
0.914214,0.914214,-1.83697e-16,6.12323e-17,1.,0.707107,7.91669e-17,1.29289,-1.83697e-16,
-0.707107,0.707107,0.707107,-0.914214,0.914214,-1.83697e-16,-1.,1.22465e-16,0.707107,
-1.29289,1.58334e-16,-1.83697e-16,-0.707107,-0.707107,0.707107,-0.914214,-0.914214,
-1.83697e-16,-1.83697e-16,-1.,0.707107,-2.375e-16,-1.29289,-1.83697e-16,0.707107,
-0.707107,0.707107,0.914214,-0.914214,-1.83697e-16,1.,-2.44929e-16,0.707107,1.29289,
-3.16667e-16,0.707107,1.29289,-3.16667e-16,0.707107,1.29289,0.,0.707107,1.29289,
0.,1.,2.,0.,0.707107,0.914214,0.914214,1.,1.41421,1.41421,0.707107,7.91669e-17,1.29289,
1.,1.22465e-16,2.,0.707107,-0.914214,0.914214,1.,-1.41421,1.41421,0.707107,-1.29289,
1.58334e-16,1.,-2.,2.44929e-16,0.707107,-0.914214,-0.914214,1.,-1.41421,-1.41421,
0.707107,-2.375e-16,-1.29289,1.,-3.67394e-16,-2.,0.707107,0.914214,-0.914214,1.,
1.41421,-1.41421,0.707107,1.29289,-3.16667e-16,1.,2.,-4.89859e-16,1.,2.,
-4.89859e-16,1.,2.,0.,1.,1.41421,1.41421,1.,1.22465e-16,2.,1.,-1.41421,1.41421,
1.,-2.,2.44929e-16,1.,-1.41421,-1.41421,1.,-3.67394e-16,-2.,1.,1.41421,-1.41421,
1.,2.,-4.89859e-16,0.707107,2.70711,0.,0.707107,1.91421,1.91421,0.707107,1.65762e-16,
2.70711,0.707107,-1.91421,1.91421,0.707107,-2.70711,3.31525e-16,0.707107,-1.91421,
-1.91421,0.707107,-4.97287e-16,-2.70711,0.707107,1.91421,-1.91421,0.707107,2.70711,
-6.6305e-16,6.12323e-17,3.,0.,6.12323e-17,2.12132,2.12132,6.12323e-17,1.83697e-16,
3.,6.12323e-17,-2.12132,2.12132,6.12323e-17,-3.,3.67394e-16,6.12323e-17,-2.12132,
-2.12132,6.12323e-17,-5.51091e-16,-3.,6.12323e-17,2.12132,-2.12132,6.12323e-17,3.,
-7.34788e-16,-0.707107,2.70711,0.,-0.707107,1.91421,1.91421,-0.707107,1.65762e-16,
2.70711,-0.707107,-1.91421,1.91421,-0.707107,-2.70711,3.31525e-16,-0.707107,-1.91421,
-1.91421,-0.707107,-4.97287e-16,-2.70711,-0.707107,1.91421,-1.91421,-0.707107,
2.70711,-6.6305e-16,-1.,2.,0.,-1.,1.41421,1.41421,-1.,1.22465e-16,2.,-1.,
-1.41421,1.41421,-1.,-2.,2.44929e-16,-1.,-1.41421,-1.41421,-1.,-3.67394e-16,-2.,
-1.,1.41421,-1.41421,-1.,2.,-4.89859e-16,-0.707107,1.29289,0.,-0.707107,0.914214,
0.914214,-0.707107,7.91669e-17,1.29289,-0.707107,-0.914214,0.914214,-0.707107,
-1.29289,1.58334e-16,-0.707107,-0.914214,-0.914214,-0.707107,-2.375e-16,-1.29289,
-0.707107,0.914214,-0.914214,-0.707107,1.29289,-3.16667e-16,-1.83697e-16,1.,
0.,-1.83697e-16,0.707107,0.707107,-1.83697e-16,6.12323e-17,1.,-1.83697e-16,-0.707107,
0.707107,-1.83697e-16,-1.,1.22465e-16,-1.83697e-16,-0.707107,-0.707107,-1.83697e-16,
-1.83697e-16,-1.,-1.83697e-16,0.707107,-0.707107,-1.83697e-16,1.,-2.44929e-16,0.707107,
1.29289,0.,0.707107,0.914214,0.914214,0.707107,7.91669e-17,1.29289,0.707107,-0.914214,
0.914214,0.707107,-1.29289,1.58334e-16,0.707107,-0.914214,-0.914214,0.707107,
-2.375e-16,-1.29289,0.707107,0.914214,-0.914214,0.707107,1.29289,-3.16667e-16,1.,
2.,0.,1.,1.41421,1.41421,1.,1.22465e-16,2.,1.,-1.41421,1.41421,1.,-2.,2.44929e-16,
1.,-1.41421,-1.41421,1.,-3.67394e-16,-2.,1.,1.41421,-1.41421,1.,2.,-4.89859e-16,1.,
2.,0.,0.707107,2.70711,0.,6.12323e-17,3.,0.,-0.707107,2.70711,0.,-1.,2.,
0.,-0.707107,1.29289,0.,-1.83697e-16,1.,0.,0.707107,1.29289,0.,1.,2.,0.,
1.,1.41421,1.41421,0.707107,1.91421,1.91421,6.12323e-17,2.12132,2.12132,-0.707107,
1.91421,1.91421,-1.,1.41421,1.41421,-0.707107,0.914214,0.914214,-1.83697e-16,0.707107,
0.707107,0.707107,0.914214,0.914214,1.,1.41421,1.41421,1.,1.22465e-16,2.,
0.707107,1.65762e-16,2.70711,6.12323e-17,1.83697e-16,3.,-0.707107,1.65762e-16,2.70711,
-1.,1.22465e-16,2.,-0.707107,7.91669e-17,1.29289,-1.83697e-16,6.12323e-17,1.,
0.707107,7.91669e-17,1.29289,1.,1.22465e-16,2.,1.,-1.41421,1.41421,0.707107,-1.91421,
1.91421,6.12323e-17,-2.12132,2.12132,-0.707107,-1.91421,1.91421,-1.,-1.41421,
1.41421,-0.707107,-0.914214,0.914214,-1.83697e-16,-0.707107,0.707107,0.707107,
-0.914214,0.914214,1.,-1.41421,1.41421,1.,-2.,2.44929e-16,0.707107,-2.70711,3.31525e-16,
6.12323e-17,-3.,3.67394e-16,-0.707107,-2.70711,3.31525e-16,-1.,-2.,2.44929e-16,-0.707107,
-1.29289,1.58334e-16,-1.83697e-16,-1.,1.22465e-16,0.707107,-1.29289,1.58334e-16,
1.,-2.,2.44929e-16,1.,-1.41421,-1.41421,0.707107,-1.91421,-1.91421,6.12323e-17,-2.12132,
-2.12132,-0.707107,-1.91421,-1.91421,-1.,-1.41421,-1.41421,-0.707107,-0.914214,
-0.914214,-1.83697e-16,-0.707107,-0.707107,0.707107,-0.914214,-0.914214,1.,
-1.41421,-1.41421,1.,-3.67394e-16,-2.,0.707107,-4.97287e-16,-2.70711,6.12323e-17,
-5.51091e-16,-3.,-0.707107,-4.97287e-16,-2.70711,-1.,-3.67394e-16,-2.,-0.707107,
-2.375e-16,-1.29289,-1.83697e-16,-1.83697e-16,-1.,0.707107,-2.375e-16,-1.29289,
1.,-3.67394e-16,-2.,1.,1.41421,-1.41421,0.707107,1.91421,-1.91421,6.12323e-17,2.12132,
-2.12132,-0.707107,1.91421,-1.91421,-1.,1.41421,-1.41421,-0.707107,0.914214,-0.914214,
-1.83697e-16,0.707107,-0.707107,0.707107,0.914214,-0.914214,1.,1.41421,-1.41421,
1.,2.,-4.89859e-16,0.707107,2.70711,-6.6305e-16,6.12323e-17,3.,-7.34788e-16,-0.707107,
2.70711,-6.6305e-16,-1.,2.,-4.89859e-16,-0.707107,1.29289,-3.16667e-16,-1.83697e-16,
1.,-2.44929e-16,0.707107,1.29289,-3.16667e-16,1.,2.,-4.89859e-16
      };
    };
  };
 };



 // Coloured x,y,z axis arrows
 macro mk_axis_col {
	Line  set1 {
	      ncells = 15;
	      node_connect_list = { 0, 1, 1, 2, 1, 3,19, 4, 4, 5,
                               4, 6, 7, 8, 7, 9,20,10,10,11,
				                  10,12,13,14,13,15,16,17,16,18};
	};
	Mesh+Node_Data+Vector+Float  axis {
   		int nnodes = 21;
	   	int nspace = 3;
   		coordinates {

		// 5, 2, 3, 4, 5

      		float values[nvals][veclen] = {
         		{0,0,0},{1,0,0},{0.7,0.15,0},{0.7,-0.15,0},{0,1.05,0},
      			{-0.15,0.75,0},{0.15,0.75,0},
		      	{0,0.85,0},{-0.15,0.55,0},{0.15,0.55,0},
		      	{0,0,1.1},{-0.15,0,0.8},{0.15,0,0.8},{0,0,0.9},
			      {-0.15,0,0.6},{0.15,0,0.6}, {0,0,0.7},{-0.15,0,0.4},{0.15,0,0.4},
               {0,0,0},{0,0,0}
      		};
   		};
  		   int ncell_sets = 1;
   		cell_set[ncell_sets] => {set1};    


         nnode_data = 1;
		   node_data[nnode_data];

         !node_data[0] {
            values[nvals][veclen] = {
			    MK_RED,
			    MK_RED,
			    MK_RED,
			    MK_RED,
			    MK_YELLOW,
			    MK_YELLOW,
			    MK_YELLOW,
			    MK_YELLOW,
			    MK_YELLOW,
			    MK_YELLOW,
			    MK_BLUE,
			    MK_BLUE,
			    MK_BLUE,
			    MK_BLUE,
			    MK_BLUE,
			    MK_BLUE,
			    MK_BLUE,
			    MK_BLUE,
			    MK_BLUE,
			    MK_YELLOW,
			    MK_BLUE
            };

		      nvals => <-.nnodes;
		      veclen = 3;
            id = 667;
         };

   };

   // NOTE: Was DataObjectLite
	GDM.DataObject obj {
      in => axis;
	};

	olink out_fld => axis;
	olink out_obj => obj.obj;
 };


 // simple 3D arrow by John Methot
 macro mk_arrow3d_simple {
   Mesh+Space3+OPort Arrow3D {
      nnodes = 13;
      coordinates {
         values = {0, -0.08, -0.08,
                   0,  0.08, -0.08,
                   0,  0.08,  0.08,
                   0, -0.08,  0.08,
                   0.8, -0.08, -0.08,
                   0.8,  0.08, -0.08,
                   0.8,  0.08,  0.08,
                   0.8, -0.08,  0.08,
                   1.0, 0.0, 0.0,
                   0.75, -0.15, -0.15,
                   0.75,  0.15, -0.15,
                   0.75,  0.15,  0.15,
                   0.75, -0.15,  0.15};
      };
      Quad quad_set {
         ncells = 6;
         node_connect_list = {3,2,1,0,
                              0,1,5,4,
                              1,2,6,5,
                              2,3,7,6,
                              3,0,4,7,
                              12,11,10,9};
      };
      Tri tri_set {
         ncells = 4;
         node_connect_list = {8,9,10,
                              8,12,9,
                              8,11,12,
                              8,10,11};
      };
      ncell_sets = 2;
      cell_set => {quad_set, tri_set};
   };

   // NOTE: Was DataObjectLite
	GDM.DataObject obj {
     	in => Arrow3D;
	};

   olink out_fld => Arrow3D;
   olink out_obj => obj.obj;
};



};
