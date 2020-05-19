
flibrary GenerateTestDataMacs{

  // GenerateTestDataUI
  // User Interface macro
  macro GenerateTestDataUI{

    XP_COMM_PROJ.GenerateTestData.GenerateTestDataMods.GenerateTestDataParams &Params <NEportLevels={2,1}>;

    UI.UImod_panel UImod_panel{
	  title = "GenerateTestData";
    };
    UItoggle Pattern{
      parent => <-.UImod_panel;
	  set =><-.Params.pattern;
	  width => parent.clientWidth;
      y = 10;
      x = 10;
    };
    UIslider Width{
      parent => <-.UImod_panel;
	  value =><-.Params.width;
	  width => parent.clientWidth;
      y => <-.Pattern.y + <-.Pattern.height + 10;
      min = 0;
      max+nres => 300;
    };
    UIslider Height{
      parent => <-.UImod_panel;
	  value =><-.Params.height;
	  width => parent.clientWidth;
      y => <-.Width.y + <-.Width.height + 10;
      min = 0;
      max+nres => 300;
    };
    UIslider Num_frames{
      parent => <-.UImod_panel;
	  value =><-.Params.num_frames;
 	  width => parent.clientWidth;
      y => <-.Height.y + <-.Height.height + 10;
      min = 0;
      max+nres => 100;
    };
  };


 // GenerateTestDataFunc
 // Funcitonal macro
 macro GenerateTestDataFunc{
    XP_COMM_PROJ.GenerateTestData.GenerateTestDataMods.GenerateTestDataParams &Params <NEportLevels={2,1}>;
	XP_COMM_PROJ.GenerateTestData.GenerateTestDataMods.Generate_Test_Data Generate_Test_Data{
      Params =><-.Params;
    };

	olink out => Generate_Test_Data.output;
 };

 // GenerateTestData
 // User Macro 
 macro GenerateTestData{

   XP_COMM_PROJ.GenerateTestData.GenerateTestDataMods.GenerateTestDataParams &Params;

   XP_COMM_PROJ.GenerateTestData.GenerateTestDataMacs.GenerateTestDataFunc GenerateTestDataFunc{
     &Params => <-.Params;
   };

   XP_COMM_PROJ.GenerateTestData.GenerateTestDataMacs.GenerateTestDataUI GenerateTestDataUI{
      &Params => <-.Params;
      UImod_panel{
	    title => name_of(<-.<-.<-);
		};
   };

   olink out => GenerateTestDataFunc.out;
 };

};
