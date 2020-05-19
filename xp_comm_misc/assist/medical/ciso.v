Applications.ModuleStack{
macro cropped_iso<NEx=319.,NEy=374.,NEwidth=275.,NEheight=286.> {
   link in_field<NEportLevels={2,1},NEx=44.,NEy=11.> => <-.Read_Field.field;
   link out_obj<NEportLevels={1,2},NEx=55.,NEy=352.> => obj.obj;
   DV_Param_crop crop_param<NEx=88.,NEy=77.,NEportLevels={0,1}> {
      min+nres<NEportLevels={2,2}>[]={15,18,4};
      max+nres<NEportLevels={2,2}>[]={25,28,14};
   };
   DVcrop DVcrop<NEx=33.,NEy=132.> {
      in => <-.in_field;
      DV_Param_crop &crop_param<NEportLevels={2,0}>=><-.crop_param;
      min=>crop_param.min;
      max=>crop_param.max;
   };
   DV_Param_iso_vol IsoVolParam<NEx=143.,NEy=176.,NEportLevels={0,1}> {
      iso_component<NEportLevels={2,2}>=0;
      iso_level+nres<NEportLevels={2,2}>=900;
      map_component<NEportLevels={2,2}>={0};
      above<NEportLevels={2,2}>=1;
   };
   DVcut DVcut<NEx=33.,NEy=220.> {
      in => <-.DVcrop.out;
      &cut_comp=>param.iso_component;
      &level=>param.iso_level;
      &above=>param.above;
      &map_comp=>param.map_component;
      DV_Param_iso_vol &param<NEportLevels={2,0}>=><-.IsoVolParam;
   };
   macro UI<NEx=143.,NEy=253.,NEwidth=396.,NEheight=506.> {
      ilink in_fld<NEx=176.,NEy=11.,NEportLevels={2,1}> => in_field;
      link link<NEportLevels=1,NEx=0.,NEy=11.> => 
             Applications.assist0.assist1.frames[2].br;
      DV_Param_crop &crop_param<NEportLevels={2,0},NEx=11.,NEy=55.>=>
          <-.crop_param;
      UIslider min0_slider<NEy=154.,NEx=11.> {
         width=100;
         visible=1;
         x=150;y=40;
         parent => <-.link;
         decimalPoints=0;increment=1;
         min=0;
         max=><-.in_fld.dims[0];
         value=><-.crop_param.min[0];
      };
      UIslider max0_slider<NEy=187.,NEx=11.> {
         width=100;
         visible=1;
         x=150;y=80;
         parent => <-.link;
         decimalPoints=0;increment=1;
         min=0;
         max => <-.in_fld.dims[0];
         value=><-.crop_param.max[0];
      };
      UIslider min1_slider<NEy=220.,NEx=11.> {
         width=100;
         visible=1;
         x=260;y=40;
         parent => <-.link;
         decimalPoints=0;increment=1;
         min=0;
         max => <-.in_fld.dims[1];
         value => <-.crop_param.min[1];
      };
      UIslider max1_slider<NEy=253.,NEx=11.> {
         width=100;
         visible=1;
         x=260;y=80;
         parent => <-.link;
         decimalPoints=0;increment=1;
         min=0;
         max => <-.in_fld.dims[1];
         value => <-.crop_param.max[1];
      };
      UIslider min2_slider<NEy=286.,NEx=11.> {
         width=100;
         visible=1;
         x=370;y=40;
         parent => <-.link;
         decimalPoints=0;increment=1;
         min=0;
         max => <-.in_fld.dims[2];
         value => <-.crop_param.min[2];
      };
      UIslider max2_slider<NEy=319.,NEx=11.> {
         width=100;
         visible=1;
         x=370;y=80;
         parent => <-.link;
         decimalPoints=0;increment=1;
         min=0;
         max => <-.in_fld.dims[2];
         value => <-.crop_param.max[2];
      };
      DV_Param_iso_vol &iso_param<NEportLevels={2,0},NEx=242.,NEy=55.> {
         iso_component<NEportLevels={2,2}>;
         map_component<NEportLevels={2,2}>;
         above<NEportLevels={2,2}>;
         iso_level<NEportLevels={2,2}>;
      };
      UItoggle above_toggle<NEx=176.,NEy=143.> {
         parent => <-.link;
         x=0;y=120;
         set<NEportLevels={2,0}>=>iso_param.above;
      };
      UIslider cut_level<NEx=286.,NEy=231.> {
         parent => <-.link;
         x=0;y=50;
         width=140;
         min=>in_field.node_data[0].min;
         max=>in_field.node_data[0].max;
         value<NEportLevels={2,0}>=>iso_param.iso_level;
      };
   };
   DataObject obj<NEx=44.,NEy=297.>
      {
      in=><-.DVcut.out;
      Modes.normals=2;
      };
};

$push
$link Viewer3D.objs_in cropped_iso.out_obj
$pop
};
!assist0.assist1.frames[2]{old=1;};
