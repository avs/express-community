Applications.ModuleStack{
MODS.Mappers.orthoslice orthoslice<NEx=330.,NEy=253.> 
   {
   in_field => <-.Read_Field.field;
   OrthoSliceUI 
      {
      -axis_slider;
      UIpanel<NEx=11.,NEy=99.> 
         {
         x = 5;
         y = 5;
         height = 140;
         visible => 1;
         parent => assist0.assist1.frames[1].br;
         };
      plane_slider 
         {
         parent => <-.UIpanel;
         x = 45;
         y = 20;
         value=9;
         title => "";
         };
      UIradioBoxLabel plane<NEx=11.,NEy=242.> 
         {
         labels => <-.planes;
         title = "Plane";
         label_cmd<NEx=11.,NEy=429.> 
            {
            cmd = {{label="x",,,,,},
                   {label="y",,,,,},
                   {set=1,,,,label="z",,,,,}};
            dim = 3;
            };
            width = 40;
            parent => <-.UIpanel;
            selectedItem => <-.param.axis;
            };
         string planes<NEportLevels={1,1},NEx=11.,NEy=198.>[3] = {"x","y","z"};
         };
      };
   

$push
$link Viewer3D.objs_in orthoslice.out_obj
$pop
};
!assist0.assist1.frames[1]{old=1;};
