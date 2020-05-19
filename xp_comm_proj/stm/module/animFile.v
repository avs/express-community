      module animFile<process="express",src_file="animFile.c",build_dir="xp_comm_proj/stm/src"> {
         omethod+req animFile(
            Root_Name+read+notify+req,time_step+read+notify+req,FileName+write,
            num+read+notify+req) = "animFile";
         string Root_Name<export=2,NEportLevels={2,0}>;
         int num<export=2,NEportLevels={2,0}>;
         int time_step<export=2,NEportLevels={2,0}>;
         string FileName<export=2,NEportLevels={0,2}>;
      };
