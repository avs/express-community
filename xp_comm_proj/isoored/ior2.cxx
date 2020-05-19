// This code is part of the CAMARA project
// funded by the Sir Jules Thorne Trust 
// by James S Perrin  Time-stamp: <Wednesday 23/05/01 15:37:41 zzcgujp>
// ior2.cxx
// find all the isotopically separate objs in the isosurface and display
// the larget N of them,
// the recursive search is computationally expensive hence the results are
// cached and the method only recalc them if the isosurface has changed
// v1.0 
// v1.1 - sorted list of objs is also cached, very little difference on small #objs
// v2.0 - given an array of objs ids to display the method will return those

//#define DEBUG

#include "xp_comm_proj/isoored/gen.hxx"
#include <stdlib.h>

#ifdef DEBUG
#include <iostream.h>
#include <time.h>
#endif

#define MAX_TRIS_PER_NODE 20

static int *vObjs;
static int *tObjs;
static int *trisIn;
static int nTrisIn;

static int *TMPnTris;

// GetTrisOfNode: return array of node's triangles that are unassigned
//                and assigned them to obj
static int GetTrisOfNode( int node, int obj, int *vTris)
{
  int i, v, *tv;
  int nVTris=0;
  
  tv=trisIn;
  
  for(i=0; i<nTrisIn; i++)
  {
    if(tObjs[i]==-1)
    { 
      for(v=0; v<3; v++)
      {
	if(*tv==node)
	{
	  tObjs[i] = obj;
	  vTris[nVTris]=i;
	  nVTris++;
	}
	
	++tv;
      }
    } else
      tv+=3;

  }

  return nVTris;
}


void IsoObjsReduce_IsoObjsReduceMods_iso_objs_reduce2::FindNodesAndTris( int node, int obj, int &nNodes, int &nTris)
{
  int vTris[MAX_TRIS_PER_NODE], nVTris;
  int v, vert, t, tri;
  int *tPtr;

  nVTris = GetTrisOfNode(node, obj, vTris);
  nTris += nVTris;

  for(t=0; t<nVTris; t++)
  {
    tri = vTris[t];
    
    tPtr=&trisIn[tri*3];
      
    for(v=0; v<3; v++)
    {
      vert = *(tPtr++);
      
      if(vObjs[vert]==-1)
      {
	vObjs[vert]=obj;
	++nNodes;
	FindNodesAndTris(vert, obj, nNodes, nTris);
      }
    }
  }

  return;
}


// SortObjs - sort a list of obj ids based on objs.nTris
// highest value first
static int SortObjs(const void *a, const void *b)
{
  if(TMPnTris[*((int *)a)] > TMPnTris[*((int *)b)])
    return -1;
  else if(TMPnTris[*((int *)a)] < TMPnTris[*((int *)b)])
    return 1;
  else
    return 0;
}

int IsoObjsReduce_IsoObjsReduceMods_iso_objs_reduce2::threshold(OMevent_mask event_mask, int seq_num)
{
  int i,v,t;
  int nNodesIn;
  float *nodesIn;
  int *tPtr;
  int *sortedObjs;
  int *show_objs_arr, show_objs_size;
  int currObj;
  int vLeft, tLeft, count;
  int recalc=1;
  OMobj_id id;
  
  nNodesIn =  in_mesh.nnodes;
  
  if(nNodesIn==0)
    return 1;
  
  nodesIn = (float *)in_mesh.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
  nTrisIn = in_mesh.cell_set[0].ncells;
  trisIn = (int *)in_mesh.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);
  
  // has the mesh changed
  OMget_obj_val(in_mesh.obj_id(), &id);

  if(!OMchanged(id, seq_num) && !OMchanged(in_mesh.obj_id(), seq_num))
  {
#ifdef DEBUG
    cout << "not changed\n";
#endif
    // check if we have valid values stored as UImultiList causes triggering
    // on instancing
    recalc = 0;
    
  }

#ifdef DEBUG
  // start time
  clock_t st = clock();
#endif
  
  if(recalc)
  {
    if(!nodesIn || !trisIn)
    {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"iso_obj_threshold::could get in_mesh tris or nodes\n");
      return 1;
    }
  
    //  shall assume only one tri cell_set
    vObjs = new int [nNodesIn];
    tObjs = new int [nTrisIn];

    for(i=0; i<nNodesIn; i++) vObjs[i]=-1;
    for(i=0; i<nTrisIn; i++) tObjs[i]=-1;
  
    vLeft = nNodesIn;
    tLeft = nTrisIn;
    currObj = 0;
    count = 0;
  
    // ** main build loop **
    while(tLeft)
    {
      int nNodes = 0, nTris = 0;
      
      // dynamically extend the _objs array
      tot_objs = currObj+1;
          
      // find next unassigned tri
      for(t=currObj; t< nTrisIn; t++) // first currObj trisIn assigned at least
	if(tObjs[t]==-1)
	  break;

      if(t!=nTrisIn)
      {
	tObjs[t]=currObj;
	tPtr=&trisIn[3*t];
	nTris++;
      
	// recursively search for objs v & t for each v
	// ** unecessary to test all verts as recursive fn should tag them all
	for(v=0; v<3; v++)
	{
	  if(vObjs[tPtr[v]]==-1)
	  {
	    vObjs[tPtr[v]]=currObj;
	    nNodes++;
	    FindNodesAndTris(tPtr[v], currObj, nNodes, nTris);
	  }
	}
      
	// how many left to assign
	vLeft -= nNodes;
	tLeft -= nTris;

	// save size for later
	_objs[currObj].nNodes = nNodes;
	_objs[currObj].nTris = nTris;
            
	currObj++;
      }
      else
	break;
    }
    
    // sort sortedObjs list on objs.nTris (copied to TMPnTris so that SortObjs has access)
    sortedObjs = new int [tot_objs];
    TMPnTris = new int [tot_objs];
    
    for(i=0; i<tot_objs; i++)
    {
      sortedObjs[i]=i;
      TMPnTris[i]=_objs[i].nTris;
    }
    
    qsort(sortedObjs, tot_objs, sizeof(int),  SortObjs);
  
    // store values for later
    _vObjs.set_array(OM_TYPE_INT, (char *)vObjs, nNodesIn, OM_SET_ARRAY_COPY);
    _tObjs.set_array(OM_TYPE_INT, (char *)tObjs, nTrisIn, OM_SET_ARRAY_COPY);
    _sortedObjs.set_array(OM_TYPE_INT, (char *)sortedObjs, tot_objs, OM_SET_ARRAY_COPY);

    // by default return the top 5% objs
    show_objs_size = tot_objs/20;
    show_objs_arr = new int [show_objs_size];

    for(i=0; i<show_objs_size; i++)
      show_objs_arr[i]=i;
    
  } else {
    // use previously stored
    vObjs = (int *)_vObjs.ret_array_ptr(OM_GET_ARRAY_RD,&nNodesIn);
    tObjs = (int *)_tObjs.ret_array_ptr(OM_GET_ARRAY_RD,&nTrisIn);
    sortedObjs = (int *)_sortedObjs.ret_array_ptr(OM_GET_ARRAY_RD);

    // as well as usual checking for NULL ptrs UImultiList can cause incorrect firing of module
    // when instanced, see IsoReduceObj2Eg
    if(!vObjs || !tObjs || !sortedObjs)
      return 1;
    
    // get this list of objs to return
    show_objs_arr = (int *)show_objs.ret_array_ptr(OM_GET_ARRAY_RD, &show_objs_size);
  }
  
#ifdef DEBUG
  // end time
  clock_t en = clock();
  float tt =(en-st)/(float)CLOCKS_PER_SEC;
  cout << "Time: " << tt << endl;
#endif
  
  //  if(tot_objs<get_objs)
  //  get_objs = tot_objs;

  int nTrisOut=0, nNodesOut=0;
  int offset=0, offset3=0;
  int *node_map;
  int i3, o, j;
    
  // ** parse obj structs and build values for out_mesh **
  for(i=0; i<show_objs_size; i++)
  {
    o = show_objs_arr[i];
    nTrisOut+=_objs[sortedObjs[o]].nTris;
    nNodesOut+=_objs[sortedObjs[o]].nNodes;
  }
  
  // ** set up out_mesh **
  out_mesh.nspace = 3;
  out_mesh.nnodes = nNodesOut;
  float *nodesOut = (float *)out_mesh.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
  out_mesh.ncell_sets = 1;
  FLDset_cell_set(out_mesh.cell_set[0].obj_id(),"Tri");
  out_mesh.cell_set[0].ncells=nTrisOut;
  int *trisOut = (int *)out_mesh.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);

  // ** add nodes and create node map **
  node_map = new int [in_mesh.nnodes];

  for(i=0; i<nNodesIn; i++)
  {
    for(o=0; o<show_objs_size; o++)
    {
      if(vObjs[i]==sortedObjs[show_objs_arr[o]])
      {
	node_map[i]=offset;
	i3=i*3;
	
	for(j=0; j<3; j++)
	  nodesOut[offset3+j] = nodesIn[i3+j];

	offset++;
	offset3+=3;
	
	break; // don't bother search rest of list
      }
    }
  }

  // ** add tris using node map **
  offset=0; offset3=0;
  
  for(i=0; i<nTrisIn; i++)
  {
    for(o=0; o<show_objs_size; o++)
    {
      if(tObjs[i]==sortedObjs[show_objs_arr[o]])
      {
	i3=i*3;
	
	for(j=0; j<3; j++)
	  trisOut[offset3+j] = node_map[trisIn[i3+j]];
	
	offset++;
	offset3+=3;

	break;
      }
    }
  }
  
  //  ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: ESP_iso_obj_threshold::threshold\n");

  // clean up

  if (nodesIn)
    ARRfree(nodesIn);
  
  if(trisIn)
    ARRfree(trisIn);

  if (nodesOut)
    ARRfree(nodesOut);

  if (trisOut)
    ARRfree(trisOut);
  
  if(recalc)
  {
    delete [] vObjs;
    delete [] tObjs;
    delete [] sortedObjs;
  } else {
    ARRfree(vObjs);
    ARRfree(tObjs);
    ARRfree(sortedObjs);
  }
  
  delete [] node_map;
  
  return 1;
}
