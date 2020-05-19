      macro readGamessCube<NEx=418.,NEy=77.> {
         FLD_MAP.struct_mesh struct_mesh<NEx=165.,NEy=55.> {
            in_dims => <-.ReadGamessCube.dims;
            coord => <-.ReadGamessCube.Coords;
         };
         FLD_MAP.combine_mesh_data combine_mesh_data<NEx=99.,NEy=99.> {
            in_mesh => <-.struct_mesh.out;
            in_nd => <-.node_vector.out;
         };
         MODS.isosurface isosurface<NEx=187.,NEy=187.> {
            in_field => <-.data_math.out_fld;
            IsoParam {
               iso_level<NEdisplayMode="open"> => 0.0001;
            };
            obj {
               Datamap {
                  DataRange = {
                     {
                        selectValues=1,UIMaxValue=1.26,,,,,,,,,,,,
                     }};
               };
               Props {
                  material = {0.22,0.15,0.24,
50.};
                  inherit = 0;
                  trans = 0.4900000095;
               };
            };
            out_obj<NEportLevels={1,4}>;
            IsoUI {
               UIiso_level {
                  decimalPoints = 4;
               };
            };
         };
         MODS.bounds bounds<NEx=341.,NEy=187.> {
            in_field => <-.combine_mesh_data.out;
            out_obj<NEportLevels={1,4}>;
            obj {
               Props {
                  line_width = 1;
                  inherit = 0;
               };
            };
         };
         MODS.orthoslice orthoslice<NEx=22.,NEy=187.> {
            in_field => <-.data_math.out_fld;
            OrthoSliceParam {
               plane => 11;
               axis = 1;
            };
            obj {
               Datamap {
                  DataRange = {
                     {
                        selectValues=1,UIMaxValue=1.26,,,,,,,,,,,,
                     }};
               };
               Obj {
                  chunk = 0;
               };
               Props {
                  material = {0.36,0.36,0.33,
50.};
                  inherit = 0;
               };
            };
            out_obj<NEportLevels={1,4}>;
         };
         MODS.isoline isoline<NEx=132.,NEy=231.> {
            in_field => <-.orthoslice.out_fld;
            IsoParam {
               ncontours<NEdisplayMode="open"> = 12;
               level_min<NEdisplayMode="open"> => -0.18;
               level_max<NEdisplayMode="open"> => 0.18;
            };
            obj {
               Datamap {
                  DataRange = {
                     {
                        selectValues=1,UIMaxValue=1.26,,,,,,,,,,,,
                     }};
               };
               Props {
                  jitter = 3;
                  inherit = 0;
                  line_width = 2;
                  subdiv = 14;
               };
            };
            out_obj<NEportLevels={1,4}>;
         };
         FLD_MAP.node_vector node_vector<NEx=11.,NEy=55.> {
            in_data => <-.ReadGamessCube.Data;
         };
         MODS.extract_scalar extract_scalar<NEx=110.,NEy=143.> {
            in_field => <-.combine_mesh_data.out;
            ExtractScalarParam {
               component = 36;
            };
         };
         XP_COMM_PROJ.STM.MODULE.ReadGamessCube ReadGamessCube<NEx=77.,NEy=11.> {
            FileName<NEportLevels={3,0}>;
            TotalSize<NEportLevels={0,0}>;
            trigger<NEportLevels={3,1}>;
         };
         MODS.data_math data_math<NEx=429.,NEy=132.> {
            in_field1 => <-.extract_scalar.out_fld;
            in_field2 => <-.extract_scalar.out_fld;
            UIpanel {
               option {
                  set = 1;
               };
            };
            expres = "#1 ";
         };
      };
