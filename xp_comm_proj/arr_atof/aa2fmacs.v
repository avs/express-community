
flibrary ArrayAToFMacs {

  /* No macros */
  


  // UI Macros

  // Functional macros (Field I/O)

  // User Macros (Functional and UI)

  // Example Applications (SingleWindowApp)
	macro ArrayAToFEg<NEdisplayMode="maximized"> {
		XP_COMM_PROJ.ArrayAToF.ArrayAToFMods.ArrayAToF ArrayAToF<NEdisplayMode="open"> {
			strings_in => {<-.string,<-.string#1,<-.string#2};
			floats_out<NEdisplayMode="open">[];
		};
		string+Port string<NEdisplayMode="open"> = "1.5";
		string+Port string#1<NEdisplayMode="open"> = "1.5E+5";
		string+Port string#2<NEdisplayMode="open"> = "12e-5";
	};

};



