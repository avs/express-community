
flibrary KrigMods <build_dir="xp_comm_proj/krig",
                   out_hdr_file="gen.h",
                   out_src_file="gen.cxx">  {


   module agxint<src_file="agxint.cxx"> {
      float+IPort2 x_array[];
      float+IPort2 y_array[];
      float+IPort2 values[];
      int num_points => array_size(x_array);
      int trigger;

      cxxmethod+notify_inst+req update(
         .x_array+req+read,
         .y_array+req+read,
         .values+req+read,
         .num_points+req+read,
         .grid_out+write,
         .num_x+read+req,
         .num_y+read+req,
         .trigger+read+notify
      );

      int num_x;
      int num_y;
      float grid_out[num_y][num_x];
   };



  module XuKrigging3D<src_file = "xuk3d.cxx"> {

	cxxmethod+notify_inst+req update(
	.xarr+req+read,
	.yarr+req+read,
	.zarr+req+read,
	.values+req+read,
	.radius+req+read,
	.nz+req+read,
	.ny+req+read,
	.nz+req+read,
	.zest+write,
	.zstd+write,
	.poly_x+read,
	.poly_y+read,
	.npoly_pts+read,
	.KrigFactor+req+read,
        .KrigExponent+req+read,
	.trigger+read+notify
	);

      float xarr<NEportLevels={1,0}>[]; 
      float yarr<NEportLevels={1,0}>[]; 
      float zarr<NEportLevels={1,0}>[]; 
      float values<NEportLevels={1,0}>[];

	int np => array_size(values);
	double radius;
	int nz;
	int ny;
	int nx;
        //region parameters
	float poly_x<NEportLevels={1,0}>[];
	float poly_y<NEportLevels={1,0}>[];
	int npoly_pts => array_size(poly_x);
	float zest<NEportLevels={0,2}>[nz][ny][nx];
	float zstd<NEportLevels={0,2}>[nz][ny][nx];
	float KrigFactor = 1.0;
	float KrigExponent = 1.0;
	int trigger;
	
  };
  

  module XuKrigging2D<src_file="xuk2d.cxx">
		    {
	cxxmethod+notify_inst+req update(
	.xarr+req+read,
	.yarr+req+read,
	.values+req+read,
	.radius+req+read,
	.ny+req+read,
	.nx+req+read,
	.zest+write,
	.zstd+write,
	.poly_x+read,
	.poly_y+read,
	.npoly_pts+read,
	.KrigFactor+req+read,
        .KrigExponent+req+read,
	.trigger+read+notify
	);

	float xarr<NEportLevels={1,0}>[];
	float yarr<NEportLevels={1,0}>[];
	float values<NEportLevels={1,0}>[];

	int np => array_size(values);
	double radius;
	
	int ny;
	int nx;

        //region parameters
	float poly_x<NEportLevels={1,0}>[];
	float poly_y<NEportLevels={1,0}>[];
	int npoly_pts => array_size(poly_x);
	float zest<NEportLevels={0,2}>[ny][nx];
	float zstd<NEportLevels={0,2}>[ny][nx];
	float KrigFactor = 1.0;
	float KrigExponent = 1.0;
	int trigger;
   };
};

