Applications.ModuleStack
   {
   macro thresholded_glyphs<NEx=44.,NEy=209.>
      {
      int comps<NEx=220.,NEy=0.>[8][7] =   
          {{1,2,3,4,5,6,7},
           {0,2,3,4,5,6,7},
           {0,1,3,4,5,6,7},
           {0,1,2,4,5,6,7},
           {0,1,2,3,5,6,7},
           {0,1,2,3,4,6,7},
           {0,1,2,3,4,5,7},
           {0,1,2,3,4,5,6}};

      Root.Templates.MODS.Filters.downsize downsize<NEx=0.,NEy=40.>
         {
         in_field<NEportLevels={3,0}> => <-.<-.Read_Field.field;
         panel.visible=>1;
         DownsizeUI
             {
             factor0_slider
                 {
                 parent => assist0.assist1.frames[2].br;
                 x = 0;
                 y = 0;
                 width = 100;
                 height = 48;
                 visible=1;
                 value => in_field.dims[0]/12+1;
                 max => in_field.dims[0]/6+1;
                 title = "";
                 };
             factor1_slider
                 {
                 parent => assist0.assist1.frames[2].br;
                 x = 0;
                 y = 40;
                 width = 100;
                 height = 48;
                 visible=1;
                 value => in_field.dims[1]/12+1;
                 max => in_field.dims[1]/6+1;
                 title = "";
                 };
             factor2_slider
                 {
                 parent => assist0.assist1.frames[2].br;
                 x = 0;
                 y = 88;
                 width = 100;
                 visible=1;
                 value => in_field.dims[2]/12+1;
                 max => in_field.dims[2]/6+1;
                 title = "downsize x, y, z";
                 };
             };
         DownsizeParam
             {
             factor0 => <-.DownsizeUI.factor0_slider.value;
             factor1 => <-.DownsizeUI.factor1_slider.value;
             factor2 => <-.DownsizeUI.factor2_slider.value;
             };
         };
      Root.Templates.MODS.Filters.threshold threshold<NEx=77.,NEy=100.>
         {
         in_field => <-.downsize.out_fld;
         UIpanel.visible=1;
         ThresholdUI
             {
             min_slider
                {
                parent => assist0.assist1.frames[2].br;
                visible => 1;
                width = 150;
                height = 48;
                x = 100;
                y = 0;
                title => "";
                };
             UIradioBoxLabel_thresh
                {
                visible => 1;
                UIpanel
                   {
                   visible => 1;
                   parent => assist0.assist1.frames[2].br;
                   x = 120;
                   y = 40;
                   };
                UIlabel.label => "Ignore data below";
                };
             };
         };
      int thr_comp<NEx=220.,NEy=30.> => threshold.ThreshParam.thresh_vector;
      int comb_comp<NEx=220.,NEy=60.> => max_array({0,threshold.in_field.nnode_data-2});
      Root.Templates.MODS.Filters.combine_comp combine_comp<NEx=0.,NEy=130.>
         {
         in_field1 => <-.threshold.out_fld;
         in_field2 => <-.downsize.out_fld;
         CombCompParam
            {
            comp2 => <-.<-.comps[<-.<-.thr_comp][0:<-.<-.comb_comp];
            };
         };
      Root.Templates.MODS.Mappers.thresh_null thresh_null<NEx=70.,NEy=180.>
         {
         in_field => <-.combine_comp.out_fld;
         };
      GEOMS.Diamond3D Diamond3D<NEx=220.,NEy=180.>;
      MODS.Mappers.glyph glyph<NEx=100.,NEy=220.> 
         {
         in_field => <-.thresh_null.out_fld;
         in_glyph => <-.Diamond3D.out_fld;
         out_obj<NEportLevels={0,3}>;
         obj.Modes.mode={0,0,0,0,0};
         GlyphParam
            {
            scale=>(2 / <-.in_field.node_data[glyph_comp].max);
            };
         UIpanel.visible=1;
         GlyphUI 
            {
            scale_slider 
                {
                parent => assist0.assist1.frames[2].br;
                min => 0.;
                max => 
              (3.5 / <-.<-.in_field.node_data[<-.param.glyph_comp].max);
                y = 0;
                x = 270;
                visible=>1;
                width = 120;
                height = 48;
                title = "";
                };
            UIradioBoxLabel_glyph 
                {
                visible=>1;
                UIpanel
                   {
                   parent => assist0.assist1.frames[2].br;
                   y = 40;
                   x = 270;
                   };
                UIlabel.label => "Glyphs scaled by";
                };
            UIradioBoxLabel_map 
                {
                visible=>1;
                UIpanel
                   {
                   parent => assist0.assist1.frames[2].br;
                   y = 40;
                   x = 390;
                   };
                UIlabel.label => "Glyphs colored by";
                };
            };
         };
      };
$link stdlink thresholded_glyphs.glyph.out_obj
   };
!assist0.assist1.frames[2]{old=1;};
