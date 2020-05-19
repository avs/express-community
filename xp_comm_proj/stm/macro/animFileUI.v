      macro animFileUI<NEx=187.,NEy=55.,NEwidth=396.,NEheight=264.,NEscalingFactor=1.016897082,NExOffset=-11.,NEyOffset=-2.017372131,NEdisplayMode="closed"> {
         UIapp Animate_File_Name_UI<NEx=55.,NEy=22.> {
            x = 15;
            y = 36;
            width = 284;
            height = 209;
         };
         UIframe animFile_Frame<NEx=121.,NEy=66.> {
            x = 5;
            y = 4;
            width = 265;
            height = 150;
            parent => <-.Animate_File_Name_UI;
         };
         UItext Root_File_Name<NEx=33.,NEy=110.> {
            x = 4;
            y = 60;
            width = 250;
            color<NEdisplayMode="open"> {
               foregroundColor = "red";
               backgroundColor = "grey";
            };
            fontAttributes {
               height = 16;
            };
            parent => <-.animFile_Frame;
            text<NEportLevels={0,3}> = "";
         };
         UIslider Number_of_digits<NEx=143.,NEy=143.> {
            x = 4;
            y = 90;
            width = 250;
            height = 45;
            fontAttributes {
               height = 16;
            };
            parent => <-.animFile_Frame;
            min = 1.;
            max = 5.;
            value<NEportLevels={2,3}> = 3.;
            mode = "integer";
            decimalPoints = 0;
            increment = 1.;
         };
         UIlabel File_Root_Name<NEx=297.,NEy=99.> {
            x = 4;
            y = 40;
            width = 250;
            height => 23;
            fontAttributes<NEdisplayMode="open"> {
               height = 16;
            };
            parent => <-.animFile_Frame;
            alignment = "left";
         };
         UIlabel Animate_File_Name_<NEx=319.,NEy=154.> {
            x = 4;
            y = 0;
            width = 250;
            height => 36;
            color {
               foregroundColor = "yellow";
               backgroundColor = "black";
            };
            fontAttributes {
               height = 30;
            };
            parent => animFile_Frame;
         };
      };
