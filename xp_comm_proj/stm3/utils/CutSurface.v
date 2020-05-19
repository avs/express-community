MODS.cut_plane CutSurface<NEhelpContextID=1,NEhelpFile="STM3/CutSurface.html"> {
   UIpanel {
      parent<NEportLevels={2,0}>;
		title => name_of(<-.<-,1);
   };
   DVcell_data_labels {
      labels[];
   };
   DVnode_data_labels {
      labels[];
   };
   cut_obj {
      Modes {
         normals = "Inherit";
      };
		Obj {
         cache_size => cache(switch(is_valid(getenv("CACHE_SIZE"))+1, 64, getenv("CACHE_SIZE")));
			name => name_of(<-.<-.<-,1);
		};
   };
};
