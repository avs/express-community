module MakeLUT<src_file="MakeLUT_stuff.cxx", 
               out_hdr_file="MakeLUT_gen.h", NEx=253.,NEy=132.> {
   cxxmethod+req MakeLUT_update(LUTsize+read+notify+req,
      in_dmap+read+notify+req, b+write, alpha+write, red+write, green+write, blue+write);
   int LUTsize<NEportLevels={2,0}>;
   group &in_dmap<NEportLevels={2,0}>;
   byte red<NEportLevels={0,2}>[]; 
   byte green<NEportLevels={0,2}>[]; 
   byte blue<NEportLevels={0,2}>[]; 
   int  alpha<NEportLevels={0,2}>[]; 
};
