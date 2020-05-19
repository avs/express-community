MODS.Mappers.city_plot city_plot<NEx=187.,NEy=99.> {
   in_field<NEx=11.,NEy=11.> => <-.in_field;
   CityParam<NEx=11.,NEy=55.> {
      glyph_comp<NEportLevels={2,2}>;
      scale_x => ;
   };
   CityUI<NEx=286.,NEy=264.> {
      param<NEx=11.,NEy=55.>;
      DVnode_data_labels {
         labels[];
      };
      UIpanel {
         width = 144;
      };
      UIradioBoxLabel_glyph {
         UIradioBox {
            y = 20;
            parent => {
               clientWidth=563.,clientHeight=144.,,
            };
            x = 140;
         };
         UIlabel {
            y = 4;
            parent => {
               clientWidth=563.,clientHeight=144.,,
            };
            x = 144;
         };
      };
      UIradioBoxLabel_map {
         UIradioBox {
            y = 24;
            parent => {
               clientWidth=563.,clientHeight=144.,,
            };
            x = 296;
         };
         UIlabel {
            y = 4;
            parent => {
               clientWidth=563.,clientHeight=144.,,
            };
            x = 276;
         };
      };
      scale_slider<NEx=198.,NEy=198.,NEwidth=572.,NEheight=506.> {
         visible<NEportLevels={3,1}>;
         parent => {
            clientWidth=563.,clientHeight=144.,,
         };
         y = 0;
         max<NEdisplayMode="open",NEportLevels={3,0}> => cache((50 / <-.<-.in_field.node_data[<-.<-.CityParam.glyph_comp].max));
      };
   };
   out_obj<NEportLevels={1,3}>;
};
