macro Cube_on_Surface<NEhelpContextID=1,NEhelpFile="STM3/Cube_on_Surface.html"> {
   link surface<NEportLevels={2,1},NEx=187.,NEy=66.>;
   link cube<NEportLevels={2,1},NEx=187.,NEy=110.>;
#ifdef VIZ_EXPRESS
   MODS.interp_data DVinterp_data<NEx=341.,NEy=198.> {
      in_field => <-.cube;
      in_probe => <-.surface;
		link out<NEportLevels={1,2}> => out_fld;
   };
#else
   DVinterp_data DVinterp_data<NEx=341.,NEy=198.> {
      in => <-.cube;
      probe => <-.surface;
      comps = {0};
   };
#endif
   GDM.DataObject DataObject<NEx=462.,NEy=308.> {
      in => <-.DVinterp_data.out;
	   Obj {
	     name = "mapped surface";
        cache_size => cache(switch(is_valid(getenv("CACHE_SIZE"))+1, 64, getenv("CACHE_SIZE")));
	   };
      Props {
         material = {0.2,0.4,0.4,50.};
      };
   };
   link out<NEportLevels={1,2},NEx=627.,NEy=418.> => .DataObject.obj;
};
