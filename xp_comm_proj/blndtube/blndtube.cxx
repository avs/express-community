
// blndtube.cxx
// v1.2
// by James S Perrin Mar 1999

// v1.0 Working version
// v1.1 Added scale data inversion and offset
// v1.2 Added support for single color component, no scaling component <ianc@avs.com>

#include <math.h>
#include <iostream.h>

#include "xp_comm_proj/blndtube/gen.hxx"
#include "avs/mat.h"

#define PI 3.1415927
#define DEGPERPI 57.29578

// <<<<<< LOCAL FUNCTIONS >>>>>>

// ** create rot mat zyrot to orientate with the normal n **
void MakeRotMat(float n[3], float zyrot[4][4])
{
  float zrot[4][4], yrot[4][4];
  /*  float zrot[4][4], yrot[4][4], untwistyrot[4][4], tmpmat[4][4] */
  float a, b, alpha, beta;

  a=sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
  b=sqrt(n[0]*n[0] + n[2]*n[2]);

  // Express's coordinate system seems to be completely confused
  // and the rotations were resolved only after a lot of trial and error
  // (more trial than anything else!!)
  alpha = acos(n[1]/a);
  
  if(b==0)
    beta = 0;
  else if(n[2]<0)
    beta = acos(n[0]/b);
  else
    beta = -acos(n[0]/b);
  
  // setup rotation matrix
  MATmat4_z_rotate(DEGPERPI*alpha, zrot);
  MATmat4_y_rotate(DEGPERPI*beta, yrot);
  
  // attempt to untwist tube, not working yet really
  //MATmat4_y_rotate(DEGPERPI*beta, untwistyrot);
  
  // concatenate rot matrices  
  MATmat4_multiply(zrot, yrot, zyrot);

  // concatenate rot matrices, with untwist
  //MATmat4_multiply(untwistyrot,zrot, tmpmat);
  //MATmat4_multiply(tmpmat, yrot, zyrot);
}



int
BlendyTubes_BlendyTubesMods_BlendyTubesCore::update(OMevent_mask event_mask, int seq_num)
{
  // lines_in (Mesh+Node_Data read req notify)
  // tubes_out (Mesh+Node_Data write)

  // scale (OMXfloat read req notify)
  // facets (OMXint read req notify)
  // offsetScale (OMXint read req notify)
  // invertScale (OMXint read req notify)

  // <<<<<< LOCAL VARIABLES >>>>>>
  int i, face, pt, pt_in; // obligatry i indexer and friends for nodes
  int bone, quad, quad_node, cell;  // same for cells
  int polyline, npolylines;
  
  float v1[3], v2[3], n[3];  // vectors and normal
  float zyrot[4][4], scl[4][4], transl[4][4], transf[4][4], tmpmat[4][4];  // transformation matrices
  
  int sclComp, colComp;  // which components should be used for tube col and scaling
  float sclOffset, sclMax; // offset for scale data so all scaling is +ve, sclMax max of scaling comp
  float *circle, *circleTransf, angle;  // values to create circular cross section
  int plStart, plEnd;  // index of start and end of polyline

  float sclTotal; // final scaling of each section
  int nnodes_in, nnodes_out;
  float *coords_in=NULL, *coords_out=NULL;  // coord pointers
  int *line_cells_in=NULL, *tube_cells_out=NULL;  // connectivity pointers
  int size, type, cell_type; // FLD parameters
  float *scaleData_in=NULL;  // data pointers
  char *colData_in=NULL, *data_char_in, *data_char_out;
  unsigned char *data_byte_in, *data_byte_out;
  short *data_short_in, *data_short_out;
  int *data_int_in, *data_int_out;
  float *data_float_in, *data_float_out;
  double *data_double_in, *data_double_out;


  // --- specify what are the node data components are to be used for ---
  colComp=0;
  sclComp=1;


  // <<<<<< CHECK VALID INPUT >>>>>>
  if (lines_in.nnodes.valid_obj() != 1)
    return 1;
  
  if (lines_in.nspace.valid_obj() != 1)
    return 1;

  if (lines_in.nspace != 3) {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: requires lines_in coordinates to have 3 dimensions");
    return 1;
  }

  if(lines_in.nnode_data < 1)
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: requires lines_in to have at least 1 scalar data components");
    return 1;
  }
  else if(lines_in.nnode_data == 1)
  {
    sclComp = -1;
  }

  if(lines_in.ncell_sets < 1)
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: requires lines_in to have at least 1 polyline cell set");
    return 1;
  }

  if (facets < 3) {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: Tubes cannot have less than 3 facets");
    facets = 3;
  }


  // <<<<<< INITIALISE VALUES >>>>>> 
  // ------ coords -------
  nnodes_in = lines_in.nnodes;
  coords_in = (float *)lines_in.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
  if(coords_in==NULL)
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: can't get array lines_in.coordinates.values");
    return 1;
  }
  nnodes_out = nnodes_in * facets;
  tubes_out.nspace = 3;
  tubes_out.nnodes = nnodes_out;
  coords_out = (float *)tubes_out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
  if(coords_out==NULL)
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: can't get array tubes_out.coordinates.values");
    ARRfree((char *)coords_in);
    return 1;
  }
  
  // ------ connectivity ------ 
  if(FLDget_poly_connect(lines_in.cell_set[0].obj_id(), &line_cells_in, &size, OM_GET_ARRAY_RD) != OM_STAT_SUCCESS )
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: can't get array lines_in.cell_set[0].poly_connect_list");
    ARRfree((char *)coords_in);
    ARRfree((char *)coords_out);
    return 1;
  }
  FLDget_cell_type(lines_in.cell_set[0].obj_id(), &cell_type);
  if(cell_type != 3) // not Polyline
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: lines_in.cell_set[0] must be of type Polyline");
    ARRfree((char *)coords_in);
    ARRfree((char *)coords_out);
    ARRfree((char *)line_cells_in);
    return 1;
  }
  FLDget_npolys(lines_in.cell_set[0].obj_id(), &npolylines);
  
  tubes_out.ncell_sets = 1;
  FLDset_cell_set(tubes_out.cell_set[0].obj_id(), "Quad");
  tubes_out.cell_set[0].ncells = (nnodes_in - npolylines) *  facets;
  tube_cells_out = (int *)tubes_out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
  if(tube_cells_out==NULL)
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: can't get array tubes_out.cell_set[0].node_connect_list");
    ARRfree((char *)coords_in);
    ARRfree((char *)coords_out);
    ARRfree((char *)line_cells_in);
    return 1;
  }

  // ------ get data, check both data components are scalar ------ 
  
  if((sclComp>=0) && (lines_in.node_data[sclComp].veclen!=1))
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: requires scaling node data to be scalar");
    ARRfree((char *)coords_in);
    ARRfree((char *)coords_out);
    ARRfree((char *)line_cells_in);
    ARRfree((char *)tube_cells_out);
    return 1;
  }
  if(lines_in.node_data[colComp].veclen!=1)
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: requires colour node data to be scalar");
    ARRfree((char *)coords_in);
    ARRfree((char *)coords_out);
    ARRfree((char *)line_cells_in);
    ARRfree((char *)tube_cells_out);
    return 1;
  }
    
  if(sclComp>=0) {
    // --- get scaling data ---
    // convert to floats to make life easier since we only need to read the values
    scaleData_in = (float *)lines_in.node_data[sclComp].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_FLOAT, &size);
    if(scaleData_in==NULL)
    {
      ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: can't get array lines_in.node_data[].values for scaling");
      ARRfree((char *)coords_in);
      ARRfree((char *)coords_out);
      ARRfree((char *)line_cells_in);
      ARRfree((char *)tube_cells_out);
      return 1;
    }

    // -- Calculate scaling offset if required
    if (offsetScale && (lines_in.node_data[sclComp].min<0))
      sclOffset = -(float)lines_in.node_data[sclComp].min;
    else
      sclOffset = 0.0;
    sclMax = lines_in.node_data[sclComp].max;
  }
  else {
    sclOffset = 0.0;
    sclMax = 1.0;
  }
  
  // --- set colour data ---
  colData_in = (char *)lines_in.node_data[colComp].values.ret_array_ptr(OM_GET_ARRAY_RD, &size, &type);
  if(colData_in==NULL)
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"BlendyTubes: can't get array lines_in.node_data[].values for colour");
    ARRfree((char *)coords_in);
    ARRfree((char *)coords_out);
    ARRfree((char *)line_cells_in);
    ARRfree((char *)tube_cells_out);
    ARRfree((char *)colData_in);
    return 1;
  }
  
  tubes_out.nnode_data = 1;
  tubes_out.node_data[0].veclen=1;
  FLDcopy_node_minmax(lines_in.obj_id(), tubes_out.obj_id(), colComp, 0);

  // macro to cope with different datatypes
#define CREATE_COL_DATA(TYPE, D_IN, D_OUT) \
  { \
    D_IN=(TYPE *)colData_in; \
    D_OUT=(TYPE *)ARRalloc(NULL, type, nnodes_out, NULL); \
    for(i=0; i<nnodes_in; i++) \
      for(face=0; face < facets; face++) \
        D_OUT[i*facets+face]=D_IN[i]; \
    FLDset_node_data(tubes_out.obj_id(), 0, (char *)D_OUT, type, nnodes_out, OM_SET_ARRAY_FREE); \
  }
  
  switch(type)
  {
  case DTYPE_CHAR:
    CREATE_COL_DATA(char, data_char_in, data_char_out)
      break;
  case DTYPE_BYTE:
    CREATE_COL_DATA(unsigned char, data_byte_in, data_byte_out)
      break;
  case DTYPE_SHORT:
    CREATE_COL_DATA(short, data_short_in, data_short_out)
      break;
  case DTYPE_INT:
    CREATE_COL_DATA(int, data_int_in, data_int_out)
      break;
  case DTYPE_FLOAT:
    CREATE_COL_DATA(float, data_float_in, data_float_out)
      break;
  case DTYPE_DOUBLE:
    CREATE_COL_DATA(double, data_double_in, data_double_out)
      break;
  };
  
  
  // ------- create a circle to be used as cross section -------
  circle = new float [3*facets];
  circleTransf = new float [3*facets];
  
  angle = (float)(PI+PI)/facets;
  
  for(i=0; i<facets; i++)
  {
    circle[3*i]=cos(angle*i);
    circle[3*i+1]=0;
    circle[3*i+2]=sin(angle*i);
  }
   
  // <<<<<< MAIN LOOP >>>>>>
  // for each polyline
  for(polyline=0; polyline<npolylines; polyline++)
  {
    plStart=line_cells_in[polyline*2];
    plEnd=line_cells_in[polyline*2+1];

    //    cout << "polyline " << ' '  << polyline <<  ' '  << plStart << ' ' << plEnd << endl;
    
    // ------ orientate first end same as first bone ------
    //    cout << "first" << endl;
    
    // calculate vector for first bone
    for(i=0; i<3; i++)
      n[i]=coords_in[(plStart+1)*3+i]-coords_in[plStart*3+i];

    // calculate scaling matrix
    if(sclComp>=0) {
      if(invertScale)
        // sclOffsets cancel
        sclTotal= scale * (sclMax - scaleData_in[plStart]);
      else
        sclTotal= scale * (scaleData_in[plStart]+sclOffset);
    }
    else {
      sclTotal = scale;
    }
	
    MATmat4_scale(scl, sclTotal, 0, sclTotal);
    
    // calculate rotion matrix from normal
    MakeRotMat(n, zyrot);
  
    // setup translation matrix
    MATmat4_translate(transl, coords_in[plStart*3], coords_in[plStart*3+1], coords_in[plStart*3+2]);

    // create transformation matrix
    MATmat4_multiply(scl, zyrot, tmpmat);
    MATmat4_multiply(tmpmat, transl, transf);

    // transform circle points
    MATxform_verts3(facets, (Matr3 *)circle, transf, (Matr3 *)circleTransf);

    // add transformed circle to new coords
    for(face=0; face<facets; face++)
    {
      pt=(plStart*facets+face)*3;
      coords_out[pt]=circleTransf[3*face];
      coords_out[pt+1]=circleTransf[3*face+1];
      coords_out[pt+2]=circleTransf[3*face+2];

      //     cout << (plStart*facets+face) << ' '<< circleTransf[3*face] << ' '<< circleTransf[3*face+1] << ' '<< circleTransf[3*face+2] << endl;
    }
  
    // ------ orientate each circle to the average of the neighbouring bones ------
    //    cout << "knuckles" << endl;
    
    // for each pt_in ( not first or last )
    for(pt_in=plStart+1; pt_in < plEnd; pt_in++)
    {
      // calculate vecs for line either side of pt_in
      for(i=0; i<3; i++)
      {
        v1[i]=coords_in[pt_in*3+i]-coords_in[(pt_in-1)*3+i];
        v2[i]=coords_in[(pt_in+1)*3+i]-coords_in[pt_in*3+i];
      }
      
      // calculate normal of bisector as average of v1 v2
      for(i=0; i<3; i++)
        n[i]=(v2[i]+v1[i])/2.0;
	
      // calculate scaling matrix
      if(sclComp>=0) {
        if(invertScale)
          // sclOffsets cancel
          sclTotal= scale * (sclMax - scaleData_in[pt_in]);
        else
          sclTotal= scale * (scaleData_in[pt_in]+sclOffset);
      }
      else {
        sclTotal = scale;
      }
      
      MATmat4_scale(scl, sclTotal, 0, sclTotal);

      // calculate rotion matrix from normal
      MakeRotMat(n, zyrot);
    
      // setup translation matrix
      MATmat4_translate(transl, coords_in[pt_in*3], coords_in[pt_in*3+1], coords_in[pt_in*3+2]);

      // create transformation matrix
      MATmat4_multiply(scl, zyrot, tmpmat);
      MATmat4_multiply(tmpmat, transl, transf);

      // transform circle points
      MATxform_verts3(facets, (Matr3 *)circle, transf, (Matr3 *)circleTransf);

      // add transformed circle to new coords
      for(face=0; face<facets; face++)
      {
        pt=(pt_in*facets+face)*3;
        coords_out[pt]=circleTransf[3*face];
        coords_out[pt+1]=circleTransf[3*face+1];
        coords_out[pt+2]=circleTransf[3*face+2];	
        //	cout << (pt_in*facets+face) << ' ' <<circleTransf[3*face] << ' '<< circleTransf[3*face+1] << ' ' <<circleTransf[3*face+2] << endl;
      }
    }
  
    // ----- orientate last end same as last bone ------
    //    cout << "last" << endl;
    
    // calculate vec for last bone
    for(i=0; i<3; i++)
      n[i]=coords_in[plEnd*3+i]-coords_in[(plEnd-1)*3+i];

    // calculate scaling matrix
    if(sclComp>=0) {
      if(invertScale)
        // sclOffsets cancel
        sclTotal= scale * (sclMax - scaleData_in[plEnd]);
      else
        sclTotal= scale * (scaleData_in[plEnd]+sclOffset);
    }
    else {
      sclTotal = scale;
    }
    
    MATmat4_scale(scl, sclTotal, 0, sclTotal);
    
    // calculate rotion matrix from normal
    MakeRotMat(n, zyrot);  

    // setup translation matrix
    pt=plEnd*3;
    MATmat4_translate(transl, coords_in[pt], coords_in[pt+1], coords_in[pt+2]);

    // create transformation matrix
    MATmat4_multiply(scl, zyrot, tmpmat);
    MATmat4_multiply(tmpmat, transl, transf);

    // transform circle points
    MATxform_verts3(facets, (Matr3 *)circle, transf, (Matr3 *)circleTransf);

    // add transformed circle to new coords
    for(face=0; face<facets; face++)
    {
      pt=(facets*plEnd+face)*3;
      coords_out[pt]=circleTransf[3*face];
      coords_out[pt+1]=circleTransf[3*face+1];
      coords_out[pt+2]=circleTransf[3*face+2];
      //      cout << (facets*plEnd+face) <<' ' <<circleTransf[3*face] << ' '<< circleTransf[3*face+1] << ' '<< circleTransf[3*face+2] << endl;
    }

    // ------ generate quads -------  
    //    cout << "quads" << endl;

    // quads in wrong (clockwise) order to correct rotation problems
    for(bone=plStart; bone<plEnd; bone++)
    {
      for(cell=0; cell<facets-1; cell++)
      {
        quad=bone*facets+cell;
        quad_node=(quad-polyline*facets)*4;
    
        tube_cells_out[quad_node]=quad+facets;
        tube_cells_out[quad_node+1]=quad+facets+1;
        tube_cells_out[quad_node+2]=quad+1;
        tube_cells_out[quad_node+3]=quad;
        //	cout << quad << ' ' << quad+facets << ' ' << quad+facets+1 << ' ' << quad+1 << ' ' << quad << endl;
      }

      // closing quad
      quad=bone*facets+cell;
      quad_node=(quad-polyline*facets)*4;

      tube_cells_out[quad_node]=quad+facets;
      tube_cells_out[quad_node+1]=quad+1;
      tube_cells_out[quad_node+2]=quad-(facets-1);
      tube_cells_out[quad_node+3]=quad;
      //      cout << quad << ' ' << quad+facets << ' ' << quad+1 << ' ' << quad-(facets-1) << ' ' << quad << endl;
    }

  }


  // <<<<<< SET EVERTHING FREE >>>>>>
  ARRfree((char *)coords_in);
  ARRfree((char *)coords_out);
  ARRfree((char *)line_cells_in);
  ARRfree((char *)tube_cells_out);
  ARRfree((char *)colData_in);
  if(sclComp>=0)
    ARRfree((char *)scaleData_in);

  // return 1 for success
  return(1);
}

