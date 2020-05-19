
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

flibrary Params3D <compile_subs=0> {


  // Cube, made of quad sides
  //    with parametric size controls
  //    (used in IPV demo)

   macro mk_cube {
      float+notify+IPort2 length = 1.0;
	   float+notify+IPort2 width  = 1.0;
	   float+notify+IPort2 height = 1.0;

      float+notify+IPort2 xcentre = 0.0;
      float+notify+IPort2 ycentre = 0.0;
      float+notify+IPort2 zcentre = 0.0;

      Quad+notify  set1 {
         ncells = 6;
         node_connect_list = {
               0,3,2,1,
				   0,1,5,4,
				   1,2,6,5,
				   2,3,7,6,
				   3,0,4,7,
				   4,5,6,7
				  };
      };

      Mesh+notify  glyph2 {
	      float l => (<-.length * 0.5);
	      float w => (<-.width * 0.5);
	      float h => (<-.height * 0.5);

	      float xc => <-.xcentre;
	      float yc => <-.ycentre;
	      float zc => <-.zcentre;

         int nnodes = 8;
         int nspace = 3;
         coordinates+notify {
            float+notify values[nvals][veclen] => {
               {-l+xc,-w+yc,-h+zc},
               { l+xc,-w+yc,-h+zc},
               { l+xc, w+yc,-h+zc},
               {-l+xc, w+yc,-h+zc},
               {-l+xc,-w+yc, h+zc},
               { l+xc,-w+yc, h+zc},
               { l+xc, w+yc, h+zc},
               {-l+xc, w+yc, h+zc}
		      };
         };

         int+notify ncell_sets = 1;
         cell_set[ncell_sets] => {set1};
      };

      // std object output configuration
      GDM.DataObject+notify obj {
	      in+notify => glyph2;
      };

      olink out_fld => glyph2;
      olink out_obj => obj.obj;
   };


   // Simple cylinder with parametric controls.
   // Angles are in radians.
   // Normals are not generated because this causes a glich in the drawing routines.
   macro mk_cylinder {
      Mesh_Cyl_Unif+Node_Data cyl_field<NEportLevels={0,1},NEx=99,NEy=66> {
         dims => {2,<-.subdiv,2};
         ndim = 3;
         nnode_data = 1;
         node_data {
            veclen = 1;
         };
         nspace = 3;
         points => {
            0.,0.,0.,<-.radius,<-.angle,<-.height
         };
      };

      float radius<NEportLevels={1,1},NEx=297.,NEy=33.> = 5.;
      float height<NEportLevels={1,1},NEx=297.,NEy=66.> = 5.;
      float subdiv<NEportLevels={1,1},NEx=297.,NEy=99.> = 15.;
      float angle <NEportLevels={1,1},NEx=297.,NEy=132.> = 2*MK_PI;

      GDM.DataObject DataObject<NEx=231.,NEy=165.> {
         in => <-.cyl_field;
         Modes {
            normals = GD_NO_NORMALS;
         };
      };

      link out_fld<NEportLevels={1,2},NEx=33.,NEy=165.> => .cyl_field;
      link obj<NEportLevels={1,2},NEx=231.,NEy=209.> => .DataObject.obj;

      macro UI<NEx=495.,NEy=143.> {
         UImod_panel Cylinder_glyph<NEx=55.,NEy=33.>;
         UIslider Subdivision<NEx=187.,NEy=253.> {
            parent => <-.Cylinder_glyph;
            value<NEportLevels={3,2}> => <-.<-.subdiv;
            y = 96;
            min = 4.;
            max = 30.;
            mode = "integer";
         };
         UIslider Radius<NEx=187.,NEy=121.> {
            parent => <-.Cylinder_glyph;
            value<NEportLevels={3,2}> => <-.<-.radius;
            y = 0;
            min = 0.;
            max = 10.;
         };
         UIslider Height<NEx=187.,NEy=165.> {
            parent => <-.Cylinder_glyph;
            value<NEportLevels={3,2}> => <-.<-.height;
            y = 32;
            min = 0.;
            max = 10.;
         };
         UIslider Angle<NEx=187.,NEy=209.> {
            parent => <-.Cylinder_glyph;
            value<NEportLevels={3,2}> => <-.<-.angle;
            y = 64;
            min = 0.;
            max = 2*MK_PI;
         };
      };
   };



  //
  // wireframe box, made from single polyline cell set
  // David Curington, Age 9
  //
  macro mk_wirebox3D {
   float+IPort2 rr = 1.0;
   FLD_MAP.polyline_mesh polyline_mesh {
      coord => <-.coordinates;
      connect => <-.dims;
   };
   int dims<NEportLevels={1,1}>[] = {0,16};
   float coordinates<NEportLevels={1,1}>[17][3] => {
      0.,0.,rr,
      rr,0.,rr,
      rr,0.,0.,
      0.,0.,0.,
      0.,0.,rr,
      0.,rr,rr,
      rr,rr,rr,
      rr,0.,rr,
      rr,rr,rr,
      rr,rr,0.,
      rr,0.,0.,
      rr,rr,0.,
      0.,rr,0.,
      0.,0.,0.,
      0.,rr,0.,
      0.,rr,rr,
      0.,0.,rr
   };

   olink fld => .polyline_mesh.out;
   olink obj => .polyline_mesh.obj;
  };



// 3D Cone Macro - scalable controls
// 
// September 18, 1995 
// Margit said:
// 
// >I need to create a cone to represent a ground station field of view or a
// >satellite field of view. I need to make it variable size, etc. and be able
// >to fix the position of the cone wrt to ground, etc.
// 
// Alex Y. said:
// 
// Here is V code for cone. It has 4 parameters:
// - radius
// - number of sub-divisions along circle
// - height
// - number of sub-divisions along height
// -----------------------------------


  macro mk_hyper_cone {
	float+IPort2  r=0.5;
	int+IPort2 nr=20;

	float+IPort2  h=1;
	int+IPort2 nh=2;

	float fPI=acos(-1.0);

	float rad[nh] => init_array(nh, r, 0);
	float hh[nh] => init_array(nh, 0, h);
	float ang[nr] => init_array(nr, 0, 2*fPI);

	group z_slice {
		float+IPort2 r_z;
		float+IPort2 h_z;
		float x[nr][1] => r_z*cos(ang);
		float y[nr][1] => r_z*sin(ang);
		float z[nr][1] => init_array(nr, h_z, h_z);
		float xyz[nr][3] => combine_array(x,y,z);
	};
		
	z_slice slice[nh] {
		r_z => <-.rad[index_of(slice)];
		h_z => <-.hh[index_of(slice)];
	};

	float xyz[nh*nr][3] => slice.xyz;

	Mesh_Struct+OPort2 out{
		ndim = 2;
		nspace = 3;
		dims=> {<-.nr, <-.nh};
		coordinates.values => <-.<-.xyz;
	};
    	GDM.DataObject obj {
		in => <-.out;
		Obj {
	    		name => name_of(<-.<-.<-);
		};
    	};
    	olink out_obj => obj.obj;
  };



  //From: John Methot <jmethot@avs.com>
  //To: se@avs.com
  //Cc: express@avs.com
  //Subject: Alex's cone can easily produce any surface of revolution
  //
  //SE's,
  //
  //   I was explaining Alex's cone V example to someone when I realized it
  //could be slightly modified to produce any surface of revolution.  I've
  //attached an example that produces a "wine glass".  You can learn some
  //intricacies of V by studying the original cone example or the "surf_rev"
  //attached here.
  //   Just load it into a ModuleStack and plug it into a Viewer3D.
  //
  //[jts: note that if this were modified to connect the node_data to the
  //radius, it could replace the swell_tube I wrote for CSG]
  //
  //
  //============================================================
  // creates surface of revolution using 1D vector of radii
  // 
  // Assumptions and limitations (all easily fixed):
  // - assumes uniform spacing of "slices" in Z direction
  // - only creates surface of rev. around Z axis
  // - surface is always "complete" 360 degrees
  //
  // Hard to fix problem: no way to tell renderer that surface is closed,
  // so there is a shading discontinuity at angle=0.  This could conceivably
  // be fixed by not using a structured mesh - use an unstructured mesh with
  // autocomputed (in V) connectivity and make the angle=0 and angle=360
  // vertices be shared (but then this wouldn't work when the surface is
  // not a full 360 degrees).
  //
  // Inputs:
  //   nr:   (int) number of vertices in angular dimension (nr-1 is
  //         the number of segments each circular section of the surface
  //         will be represented with)
  //   h:    (float) height of surface.  Surface goes from [0,h] in Z.
  //   rad:  (float) array of radii defining profile of surface of revolution
  //         

  macro mk_surf_rev {
	int+IPort2   nr=20;
	float+IPort2 h=1;
	float+IPort2 rad[]={0.45,0.5,0.375,0.2,0.125,0.1,0.1,0.1,0.1,0.1,0.45};

	float fPI=acos(-1.);

	int          nh=>array_size(rad);
	float hh[nh] => init_array(nh, 0, h);
	float ang[nr] => init_array(nr, 0, 2*fPI);

	group z_slice {
		float+IPort2 r_z;
		float+IPort2 h_z;
		float x[nr][1] => r_z*cos(ang);
		float y[nr][1] => r_z*sin(ang);
		float z[nr][1] => init_array(nr, h_z, h_z);
		float xyz[nr][3] => combine_array(x,y,z);
	};
		
	z_slice slice[nh] {
		r_z => <-.rad[index_of(slice)];
		h_z => <-.hh[index_of(slice)];
	};

	float xyz[nh*nr][3] => slice.xyz;

	Mesh_Struct+OPort2 out{
		ndim = 2;
		nspace = 3;
		dims=> {<-.nr, <-.nh};
		coordinates.values => <-.<-.xyz;
	};
    	GDM.DataObject obj {
		in => <-.out;
		Obj {
	    		name => name_of(<-.<-.<-);
		};
    	};
    	olink out_obj => obj.obj;
    };



  // mk_col_surf_rev: derived from John Methot's mk_surf_rev to
  // allow differently coloured rings around the tube.  col[] is
  // an additional float array (same dimension as rad) which 
  // supplies the colour for each radius.
  mk_surf_rev mk_col_surf_rev {
	// float+IPort2 col[]={0,1,2,3,4,5,6,7,8,9,10};
	float+IPort2 col[];

	z_slice {
		float+IPort2 val;
		float v[nr][1] => val;
        };
	z_slice slice[nh] {
                val => <-.col[index_of(slice)];
		r_z => <-.rad[index_of(slice)];
		h_z => <-.hh[index_of(slice)];
        };
	float dat[nh*nr][1] => slice.v;
	Mesh_Struct+Node_Data+Scalar+Float+OPort2 out{
		ndim = 2;
		nspace = 3;
		dims=> {<-.nr, <-.nh};
		coordinates.values => <-.<-.xyz;
                ! node_data[0] {
                   values => <-.<-.dat;
                };
	};
  };



  // mk_snake: derived from John Methot's mk_surf_rev.  Each slice
  // has an x,y offset.  This is supplied by x_off[] and y_off[]
  // which have the same dimension as rad.  All slices are normal
  // to the x=0,y=0 plane.
  // radscale is used to scale the radius
  mk_surf_rev mk_snake {
	float+IPort2 x_off[];
	float+IPort2 y_off[];
	/// float+IPort2 x_off[]={0,.01,.02,.03,.04,.05,.04,.03,.02,.01,0};
	/// float+IPort2 y_off[]={0,.01,.02,.03,.04,.05,.04,.03,.02,.01,0};
	float+IPort2 radscale = 1.0;
	float+IPort2 radconst = 0.0;
        z_slice {
		float xoff<NEportLevels={2,0}>;
		float yoff<NEportLevels={2,0}>;
		float x[nr][1] => r_z*cos(ang) + xoff;
		float y[nr][1] => r_z*sin(ang) + yoff;
        };
	z_slice+nres slice[nh] {
		r_z => ( <-.rad[index_of(slice)] * <-.radscale ) + <-.radconst;
		h_z => <-.hh[index_of(slice)];
                xoff => <-.x_off[index_of(slice)];
                yoff => <-.y_off[index_of(slice)];
        };
         
  };



  // mk_col_snake: derived from mk_snake and mk_col_surf_rev
  // and allows a coloured tube with variation in x,y offset.
  mk_snake+mk_col_surf_rev+nres mk_col_snake {
     z_slice+nres {
		float+IPort2 val;
		float v[nr][1] => val;
		float xoff<NEportLevels={2,0}>;
		float yoff<NEportLevels={2,0}>;
		x[nr][1] => r_z*cos(ang) + xoff;
		y[nr][1] => r_z*sin(ang) + yoff;
     };
     z_slice+nres slice[nh] {
		r_z => ( <-.rad[index_of(slice)] * <-.radscale ) + <-.radconst;
		h_z => <-.hh[index_of(slice)];
                xoff => <-.x_off[index_of(slice)];
                yoff => <-.y_off[index_of(slice)];
                val => <-.col[index_of(slice)];
     };
  };




};

