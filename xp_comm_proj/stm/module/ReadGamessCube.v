      module ReadGamessCube<src_file="ReadGamessCube.c",process="express",build_dir="xp_comm_proj/stm/src"> {
         omethod+req ReadCGamess(
            FileName+read+req,TotalSize+write,dims+write,Coords+write,
            Data+write,trigger+notify
         ) = "readcgamess";
         string FileName<export=2,NEportLevels={2,0}>;
         int TotalSize<export=2,NEportLevels={0,2}>;
         int dims<export=2,NEportLevels={0,2}>[3];
         float Coords<export=2,NEportLevels={0,2}>[prod(dims)][3];
         float Data<export=2,NEportLevels={0,2}>[prod(dims)][TotalSize];
         int trigger<NEportLevels={2,1},export=2>;
      };
