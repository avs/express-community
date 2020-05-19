Applications.ModuleStack
{

MODS.Filters.explode_materials explode_materials<NEx=352.,NEy=110.> {
   in_field => <-.Read_UCD.field;
};
MODS.Filters.explode_fields explode_fields<NEx=352.,NEy=187.> {
   in_fields => <-.explode_materials.out_fld;
   Explode_Fields {
      DVexplode_xform {
         out_field_mrg = {
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])},
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])},
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])},
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])},
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])},
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])},
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])},
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])},
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])},
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])},
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])},
            {out_fld=>
               merge(out[arr_ind],in[arr_ind])}
         };
         out = {
            
            {xform=
               {xlate=
                  {10.4755,-0.25,0.}}},
            {xform=
               {xlate=
                  {-0.545761,-0.0880013,12.176}}},
            {xform=
               {xlate=
                  {-1.45551,0.0269985,10.984}}},
            {xform=
               {xlate=
                  {-0.261505,-0.142,0.911499}}},
            {xform=
               {xlate=
                  {-1.51151,-1.9245,-0.00224876}}},
            {xform=
               {xlate=
                  {-0.126251,7.6875,10.4992}}},
            {xform=
               {xlate=
                  {10.4777,-0.200001,1.601}}},
            {xform=
               {xlate=
                  {9.9555,6.22625,10.264}}},
            {xform=
               {xlate=
                  {-0.149506,-6.675,-6.35}}},
            {xform=
               {xlate=
                  {-1.88701,5.725,11.025}}},
            {xform=
               {xlate=
                  {-12.1245,0.535999,1.75}}},
            {xform=
               {xlate=
                  {-0.527756,-0.0152512,10.574}}}
         };
      };
      compute_extents = {
         
         {
            tmp_coord=
            {min_vec={192.5,40.8,20.3},max_vec=
               {210.,67.,82.4}}
         },
         {
            tmp_coord=
            {min_vec=
               {168.077,42.129,70.281},
             max_vec=
               {190.338,66.319,81.123}}
         },
         {
            tmp_coord=
            {min_vec=
               {159.552,42.239,65.66},
             max_vec=
               {195.224,66.669,80.976}}
         },
         {
            tmp_coord=
            {min_vec=
               {159.552,41.39,25.799},
             max_vec=
               {200.,66.842,80.547}}
         },
         {
            tmp_coord=
            {min_vec=
               {159.552,41.948,28.154},
             max_vec=
               {195.,59.154,74.537}}
         },
         {
            tmp_coord=
            {min_vec={150.,64.9,62.447},
             max_vec=
               {210.093,74.65,82.25}}
         },
         {
            tmp_coord=
            {min_vec={192.5,41.,27.6},
             max_vec=
               {210.009,67.,81.504}}
         },
         {
            tmp_coord=
            {min_vec=
               {189.822,66.541,63.856},
             max_vec=
               {210.598,67.164,79.9}}
         },
         {
            tmp_coord=
            {min_vec={150.,34.15,20.4},max_vec=
               {210.,47.95,56.9}}
         },
         {
            tmp_coord=
            {min_vec={150.,64.8,65.15},max_vec=
               {203.05,66.9,81.65}}
         },
         {
            tmp_coord=
            {min_vec={150.,45.644,41.2},max_vec=
               {162.1,65.3,68.5}}
         },
         {
            tmp_coord=
            {min_vec=
               {168.647,42.256,
64.02},max_vec=
               {189.84,66.483,80.976}}
         }
      };
   };
   objs {
      dos = {
         
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         },
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         },
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         },
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         },
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         },
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         },
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         },
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         },
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         },
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         },
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         },
         {
            MinMax=
            {min_value=-4.999999874e-06,max_value=
               4.999999874e-06,},,,,,,,PickInfo=
            {npicked=0,selected=0}
         }
      };
   };
   ExplFldUI {
      UIpanel {
         width = 104;
      };
      x_slider {
         parent => assist0.assist1.frames[2].br;
         y = 20;
         x = 16;
         max=2;
      };
      y_slider {
         parent => assist0.assist1.frames[2].br;
         y = 20;
         x = 148;
         max=2;
      };
      z_slider {
         parent => assist0.assist1.frames[2].br;
         y = 20;
         x = 276;
         max=2;
      };
   };

};
$link Viewer3D.objs_in explode_fields.out_obj
$push
$del_elem_ref Viewer3D.objs_in Read_UCD.out_obj
$pop

};

!assist0.assist1.frames[2]{old=1;};
