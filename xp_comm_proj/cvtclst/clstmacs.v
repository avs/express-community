
flibrary ConvertConnectListMacs <compile_subs=0> {

        macro ConvertConnectListFunc {

                ilink input;

                XP_COMM_PROJ.ConvertConnectList.ConvertConnectListMods.ConvertConnectListCore ConvertConnectListCore {
                        input => <-.input;
		};

                
		olink poly_nodes => ConvertConnectListCore.poly_nodes;
		olink connect_list => ConvertConnectListCore.connect_list;
			
	};

       macro ConvertConnectList {

                ilink input;

                XP_COMM_PROJ.ConvertConnectList.ConvertConnectListMacs.ConvertConnectListFunc ConvertConnectListFunc {
                        input => <-.input;
                };


                olink poly_nodes => ConvertConnectListFunc.poly_nodes;
                olink connect_list => ConvertConnectListFunc.connect_list;


       };

};
