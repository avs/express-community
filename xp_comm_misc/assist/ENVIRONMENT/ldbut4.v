Applications.ModuleStack
   {
   macro multislice<NEx=11.,NEy=264.>
      {
      UIpanel UIpanel<NEx=11.,NEy=99.> 
         {
         x = 5;
         y = 5;
         visible => 1;
         parent => Applications.assist0.assist1.frames[3].br;
         height = 140;
         width = 200;
         };
      UIradioBoxLabel Axis<NEx=11.,NEy=242.> 
         {
         labels => <-.planes;
         title = "Planes";
         label_cmd<NEx=11.,NEy=429.> 
            {
            cmd = {{set=1,label="x"},
                   {label="y"},
                   {label="z"}};
            dim = 3;
            };
         width = 40;
         parent => <-.UIpanel;
         height = 100;
         };
      string planes<NEportLevels={1,1},NEx=11.,NEy=198.>[3] = {"x","y","z"};
      UIslider number_of_slices<NEx=200.,NEy=100.> 
         {
         parent => <-.UIpanel;
         value = 6;
         y = 100;
         width=100;
         min = 2;
         max = 20;
         decimalPoints = 0;
         };
      UIradioBoxLabel Render_mode<NEx=11.,NEy=242.> 
         {
         labels => <-.modes;
         title = "Rendered as";
         label_cmd<NEx=11.,NEy=429.> 
            {
            cmd = {{label="points",,,,,},
                   {label="lines",,,,,},
                   {label="surfaces",,,,,},
                   {set=1,,,,label="default",,,,,}};
            dim = 4;
            };
         x=110;
         y=0;
         width = 90;
         parent => <-.UIpanel;
         };
      string modes<NEportLevels={1,1},NEx=11.,NEy=198.>[4] = 
          {"points","lines","surfaces","default"};
      int RenderModes[4][5]={2,1,1,0,0, 1,2,1,0,0, 1,1,4,0,0, 0,0,0,0,0};

      int num_slices<NEportLevels={1,0},NEx=200.,NEy=150.> => number_of_slices.value;
      int render_modes[5]=>RenderModes[Render_mode.selectedItem];
      macro slices<NEx=200.,NEy=200.>[num_slices] 
         {
         int i<NEx=220.,NEy=66.> => index_of(slices);
         link link<NEportLevels={3,1},NEx=55.,NEy=33.> => <-.<-.Read_Field.field;
         MODS.Mappers.orthoslice orthoslice<NEx=110.,NEy=154.> 
            {
            in_field => <-.link;
            OrthoSliceParam.axis => multislice.Axis.selectedItem;
            OrthoSliceParam.plane => (i*(in_field.dims[axis]-1))/(num_slices-1);
            out_obj<NEportLevels={1,3}>;
            obj.Modes.mode => <-.<-.<-.<-.render_modes;
            };
        };
     };
   DefaultObject.objects => 
          combine_array(multislice.slices.orthoslice.out_obj,<-.stdlink);
   };
!assist0.assist1.frames[3]{old=1;};
