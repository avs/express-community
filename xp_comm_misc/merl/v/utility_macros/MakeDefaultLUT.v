module MakeDefaultLUT<src_file="MakeDefaultLUT.cxx"> {

   int+IPort2   LUTsize;
   int+OPort2   alpha[LUTsize]; 
   byte+OPort2  red[LUTsize]; 
   byte+OPort2  green[LUTsize]; 
   byte+OPort2  blue[LUTsize]; 

   cxxmethod+req update
     ( 
       LUTsize +read+notify+req,
       alpha   +write, 
       red     +write, 
       green   +write, 
       blue    +write
     );
};
