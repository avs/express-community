
flibrary Scl2TexMacs
{

   //
   // Scalar2Texture
   //
   // High Quality Colormaps, avoid RGB interpolation
   //
   // create UV texture coords from scalar node data
   // I. Curington, January 28, 1999, AVS
   //
   // Inputs: - mesh + any scalar node data
   //         - full colour image for texture look up,
   //           only colours on the diagonal are used.
   // Output: - mesh + new UV texture mapped node data
   //         - full GD object with texture applied
   //
   macro Scalar2Texture {
      ilink in_fld;
      ilink in_image;

      Mesh+Node_Data+nres+Port &in => in_fld;

      int+OPort nnodes => in.nnodes;
      float+OPort min => in.node_data[0].min;
      float+OPort max => in.node_data[0].max;
      float+OPort &v1[nnodes] => in.node_data[0].values;
      float+nres+OPort v2[nnodes][1] => (v1 - min)/(max-min);

      // float+nres v3[nnodes][2] => combine_array(v2,v2);

      FLD_MAP.interleave_2_arrays interleave {
         in1 => <-.v2;
         in2 => <-.v2;
      };

      Node_Data+Float nduv {
   		nnodes => <-.nnodes;
	   	nnode_data = 1;
		   node_data {
			   veclen = 2;
   			float+nres+IPort3 values[nvals][veclen] => <-.<-.interleave.out;
   			id = 670;
	   		labels = "texture coords";
		   	units = "unit range";
		   };
      };

      Mesh+Node_Data+OPort &out => merge(nduv,in);
      GDM.DataObject data_obj {
         in => <-.out;
         texture_in+IPort2 => in_image;
         Obj {
            name => name_of(<-.<-.<-);
         };
      };

      olink out_fld => out;
      olink out_obj => data_obj.obj;
   };

};

