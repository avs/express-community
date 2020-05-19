
flibrary Dmap2ImageApps <compile_subs=0>
{
	APPS.SingleWindowApp Datamap2ImageEg<NEdisplayMode="maximized"> {
		GDM.DataObject.Datamap Datamap{
			dataMin=>0;
			dataMax=>1;
		};

		XP_COMM_PROJ.Dmap2Image.Dmap2ImageMacs.Dmap2Image Dmap2Image {
			dmap_in => <-.Datamap;
		};
		GDM.Uviewer2D Uviewer2D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.Dmap2Image.out_obj};
				};
			};
		};
	};

	APPS.SingleWindowApp Datamap2ImageSquareEg<NEdisplayMode="maximized"> {
		GDM.DataObject.Datamap Datamap{
			dataMin=>0;
			dataMax=>1;
		};

		XP_COMM_PROJ.Dmap2Image.Dmap2ImageMacs.Dmap2ImageSquare Dmap2ImageSquare {
			dmap_in => <-.Datamap;
		};
		GDM.Uviewer2D Uviewer2D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.Dmap2ImageSquare.out_obj};
				};
			};
		};
	};

	APPS.SingleWindowApp Datamap2ImageLegendEg<NEdisplayMode="maximized"> {
		GDM.DataObject.Datamap Datamap{
			dataMin=>0;
			dataMax=>1;
		};
		XP_COMM_PROJ.Dmap2Image.Dmap2ImageMacs.Dmap2ImageLegend Dmap2ImageLegend {
			dmap_in => <-.Datamap;
		};
		GDM.Uviewer2D Uviewer2D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.Dmap2ImageLegend.out_obj};
				};
			};
		};
	};
	
};
