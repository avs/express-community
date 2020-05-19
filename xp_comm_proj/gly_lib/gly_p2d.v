
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

flibrary Params2D <compile_subs=0> {

  macro mk_arc2D {
   float+IPort2 start = 0;
   float+IPort2 end = MK_PI;
   float+IPort2 in_r = 2.2;
   float+IPort2 out_r = 3;

   int dims<NEportLevels={1,1}>[] = {2,20};
   float range<NEportLevels={1,1}>[2][2] => {in_r,start,out_r,end};

   FLD_MAP.uniform_mesh uniform_mesh {
      in_dims => <-.dims;
      out {
         coordinates {
            values => get_coords_cyl_unif(<-.ndim,<-.dims,<-.nspace,<-.points);
         };
         points<export=3,NEportLevels={3,0}> => <-.<-.range;
      };
   };

   olink out => .uniform_mesh.out;
   olink obj => .uniform_mesh.obj;
  };

};

