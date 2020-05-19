
flibrary RadioBoxMacs {

  /* No macros */
  


  // UI Macros

  // Functional macros (Field I/O)

  // User Macros (Functional and UI)

  // Example Applications (SingleWindowApp)

	macro RadioBoxEg<NEdisplayMode="maximized"> {
		UIshell RadioBoxEg;
		XP_COMM_PROJ.RadioBox.RadioBoxMods.RadioBox RadioBox {
			set => {<-.Option_0.do,
				<-.Option_1.do,<-.Option_2.do,<-.Option_3.do};
		};
		UIbutton Option_0 {
			parent => <-.RadioBoxEg;
			y = 0;
		};
		UIbutton Option_1 {
			parent => <-.RadioBoxEg;
			y => <-.Option_0.y + <-.Option_0.height;
		};
		UIbutton Option_2 {
			parent => <-.RadioBoxEg;
			y => <-.Option_1.y + <-.Option_1.height;
		};
		UIbutton Option_3 {
			parent => <-.RadioBoxEg;
			y => <-.Option_2.y + <-.Option_2.height;
		};
		UIlabel Selected_Option {
			parent => <-.RadioBoxEg;
			y => <-.Option_3.y + <-.Option_3.height;
		};
		UIfield selected {
			parent => <-.RadioBoxEg;
			value => <-.RadioBox.selectedItem;
			y => <-.Selected_Option.y + <-.Selected_Option.height;
		};
		UIlabel Previous_Option {
			parent => <-.RadioBoxEg;
			y => <-.selected.y + <-.selected.height;
		};
		UIfield previous {
			parent => <-.RadioBoxEg;
			value => <-.RadioBox.previousItem;
			y => <-.Previous_Option.y + <-.Previous_Option.height;
		};
	};

};



