library star_low < build_dir="src",process="express"> {
      module readSTAR<f_src_files="readStar.f postconvert_lib.f"> {
         fmethod+req update(
            trigger+notify+req,case+read+req,load_type+read+req,cdconv+read,
            cdsel+read,pconv+read,tconv+read,wdconv+read,wdsel+read,trtype+read,
            trstep+read,trtime+read,out_fld+write
         ) = "readStar_f";
         int trigger<NEportLevels={2,0}>;
         string case<NEportLevels={2,0}>;
         int load_type<NEportLevels={2,0}>;
         int cdconv<NEportLevels={2,0}>;
         int cdsel<NEportLevels={2,0}>[];
         int pconv<NEportLevels={2,0}>;
         int tconv<NEportLevels={2,0}>;
         int wdconv<NEportLevels={2,0}>;
         int wdsel<NEportLevels={2,0}>[];
         int trtype<NEportLevels={2,0}>;
         int trstep<NEportLevels={2,0}>;
         float trtime<NEportLevels={2,0}>;
         Field out_fld<NEportLevels={0,2}> {
            cell_set {
               cell_data {
                  values+float;
                  min+float;
                  max+float;
                  min_vec+float;
                  max_vec+float;
                  null_value+float;
               };
            };
         };
      };
};

