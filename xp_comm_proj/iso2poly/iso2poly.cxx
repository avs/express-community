/*
 * Isoline2Polyline
 *
 * Converters the Line cell sets produced by the Isoline module
 * into polyline cell sets
 *
 * Downsize means now many nodes along each isoline to skip
 * before creating another node in the polyline. Downsize of 1 means every
 * node will be mapped.
 *
 * Federico Gamba, AVS Italy, 
 * Adopted from IsolineLabels project by I. Curington
 * Complete Redesign & Bug Fixing by Gamba Federico 13/07/2000
 *
 */

#include <malloc.h>
#include "xp_comm_proj/iso2poly/gen.hxx"

//#define GFDEBUG


void I2P_StoreConnection(int* ConnA, int* ConnB, int II, int JJ) {	
   if (ConnA[II]<0) ConnA[II]=JJ; else ConnB[II]=JJ;
   if (ConnA[JJ]<0) ConnA[JJ]=II; else ConnB[JJ]=II;
}

int I2P_IsConnected(int* ConnA, int* ConnB, int JJ, int II) {
   return ( (ConnA[II]==JJ) || (ConnB[II]==JJ) );
}

void I2P_SearchNext(int* ConnA, int* ConnB, int* VisitFlag, int& JJ, int& GoOn) {
   int AA = ConnA[JJ];
   int BB = ConnB[JJ];

   if ((AA>=0) && (!VisitFlag[AA])) {
      JJ=AA; GoOn=1; 
   }
   else {
      if ((BB>=0) && (!VisitFlag[BB])) {
         JJ=BB; GoOn=1; 
      }
      else {
         GoOn=0;
      }
   }
}

void I2P_MarkAndPost (int JJ, int* VisitFlag, int FreqCounter, int Downsize,
                      int* OutFlag, int& Nnodes_out, int MaxSize ) {

   VisitFlag[JJ]=1;  // mark as visited
   if ((FreqCounter%Downsize)==0) {
      if (Nnodes_out<MaxSize) {     // just to be sure (could be removed to improve performance
         OutFlag[Nnodes_out]=JJ;    // post this node to output mesh
      } else {
         printf("Warning: Nnodes_out (%d) >= MaxSize (%d)\n", Nnodes_out, MaxSize);
      }
      Nnodes_out++;
#ifdef GFDEBUG
      printf("Debug: Added node %d on output\n", JJ);
#endif
   }
}

void I2P_AppendChain ( int*& Chains, int StartId, int EndId, int& ChainSize) {
   int OldByteSize = (ChainSize*2)*sizeof(int);
   int NewByteSize = OldByteSize + (2*sizeof(int));
   int* App = Chains;
   Chains = (int*)malloc(NewByteSize);
   if (App) {
      memcpy(Chains,App,OldByteSize);
      free(App);
   }
   ChainSize++;
   Chains[ChainSize*2-2] = StartId;
   Chains[ChainSize*2-1] = EndId;
}

void I2P_AppendIsClosed ( int*& IsClosed_arr, int IsClosed, int Size) {
   int OldByteSize = Size*sizeof(int);
   int NewByteSize = OldByteSize + sizeof(int);
   int* App = IsClosed_arr;
   IsClosed_arr = (int*)malloc(NewByteSize);
   if (App) {
      memcpy(IsClosed_arr,App,OldByteSize);
      free(App);
   }
   IsClosed_arr[Size] = IsClosed;
}

int
Isoline2Polyline_Isoline2PolylineMods_Isoline2PolylineCore::update(OMevent_mask event_mask,
                                                                   int seq_num)
{
   // Active (OMXint read notify)
   if ((int)Active==0) return(1);

   // how much downsize to use
   int Freqs=(int)Downsize;
   if (Freqs<1) Freqs=1;

   // if post out the node data or not
   int UseND=(int)UseNodeData;
   
   // if the polyline will be automatically closed
   int AutoC=(int)AutoClose;
   
   int II,JJ;
   
   // reading connectivity of each cell set I build these two array, to fast visit the
   // isolines network.
   // if there is a connection between II and JJ, it will be sure that:
   // ConnA[II]==JJ || ConnB[II]==JJ and that
   // ConnA[JJ]==II || ConnB[JJ]==II
   int* ConnB=NULL; 
   int* ConnA=NULL; 
   
   int Nnodes_in=(int)in_fld.nnodes;

   // this will count the number of out nodes
   int Nnodes_out=0;

   if (Nnodes_in==0) {
      out.nnodes=0;
      out.ncell_sets=0;
      out.nnode_data=0;
      return(1);
   }

   // OutFlag[II] = JJ means that the input II-th node 
   //			will become the JJ-th into the out field
   int* OutFlag=NULL; // it will be OutFlag[in_fld.nnodes]; 
   
   // this will keep 0/1 if the node has been already visited
   int* VisitFlag=NULL; // it will be OutFlag[in_fld.nnodes]; 
   int* VisitFlag2=NULL; // it will be OutFlag[in_fld.nnodes]; 
   
   int* Chains=NULL; // it will contain the polyline connectivity; 
   int* IsClosed_arr=NULL; // it will contain the closed info for each chain; 
   
   // in_fld (Mesh+Node_Data read notify)

   // first of all alloc the required arrays
   ConnB = (int*) malloc(sizeof(int)*Nnodes_in);
   ConnA = (int*) malloc(sizeof(int)*Nnodes_in);
   OutFlag = (int*) malloc(sizeof(int)*Nnodes_in);
   VisitFlag = (int*) malloc(sizeof(int)*Nnodes_in);
   VisitFlag2 = (int*) malloc(sizeof(int)*Nnodes_in);
   for (II=0;II<Nnodes_in;II++) {
      ConnB[II]=-1;		//not connected
      ConnA[II]=-1;		//not connected
      OutFlag[II]=-1;	//not yet sorted
      VisitFlag[II]=0;	//false
      VisitFlag2[II]=0;	//false
   }


   // in_fld.nspace (int) 
   // in_fld.nnodes (int)
   // in_fld.coordinates.values (float [])
   // in_fld.ncell_sets (int)
   int in_fld_set;
   int *in_fld_node_connect_list=NULL;
   int in_fld_ncell_sets = in_fld.ncell_sets;
   int in_fld_ncell_connect = 0;
   int From = -1;
   int To = -1;
   
   // now collect ConnA and ConnB

   for (in_fld_set=0; in_fld_set < in_fld.ncell_sets; in_fld_set++) {
      in_fld_ncell_connect = (int)in_fld.cell_set[in_fld_set].ncells;

      // here I check that cell_nnodes = 2, Isolines are in a line_mesh
      int cell_type = -1;
      if (FLDget_cell_type(in_fld.cell_set[in_fld_set].obj_id(), &cell_type) != OM_STAT_SUCCESS) {
         printf("Warning: could not get cell_type for cell_set[%d]\n", in_fld_set);
         continue;
      }
      else if ( (cell_type!=2) || ((int)in_fld.cell_set[in_fld_set].cell_nnodes!=2) ) {
         printf("Warning: cell_set[%d] does not appear to be a Line cell set\n", in_fld_set);
         continue;
      }
      
      in_fld_node_connect_list = (int *)in_fld.cell_set[in_fld_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);
      for (II=0;II<in_fld_ncell_connect;II++) {
         From = in_fld_node_connect_list[II*2+0];
         To = in_fld_node_connect_list[II*2+1];
         I2P_StoreConnection(ConnA,ConnB,From,To);
      }
      if (in_fld_node_connect_list) ARRfree(in_fld_node_connect_list);
   }

   // this counts nodes during IsoLines traversal, each time it reachs the Downsize
   // a node is marked for output posting. (and count restarts)
   int FreqCounter=0;
      
      
   int GoOn=0;
   int GoFwd=-1;
   int ChainNumber=-1;
   int ChainSize=0;
   int IsClosed=0;
   int StartSize=-1;
   int EndSize=-1;
   int StartId=-1;
   int EndId=-1;
   // now follows chains of nodes.
   for (II=0;II<Nnodes_in;II++) {
      if (!VisitFlag[II]) {
          ChainNumber++;

          // search for a good starting point
          JJ = II;
          memcpy(VisitFlag2,VisitFlag,sizeof(int)*Nnodes_in);
          GoOn=1;
          VisitFlag2[JJ]=1;
          while (GoOn) {
            GoOn=0;
            I2P_SearchNext(ConnA, ConnB, VisitFlag2, JJ, GoOn);
            VisitFlag2[JJ]=1;
          };

          // Now JJ should be a good starting point, for both Open and Closed Isolines	    
          StartSize = Nnodes_out;
            StartId = JJ;
          
          FreqCounter=0;
          GoOn=1;
          // start to follow Isoline chain
          while (GoOn) {
               // mark as visited and post it if necessary
             I2P_MarkAndPost(JJ, VisitFlag, FreqCounter, Freqs, OutFlag, Nnodes_out, Nnodes_in);
             FreqCounter++;
               // try to search another node to go on
             GoOn=0;
             I2P_SearchNext(ConnA, ConnB, VisitFlag, JJ, GoOn); // if found it modify JJ and GoOn==1
          }
          EndSize = Nnodes_out-1;
          EndId = JJ;

          IsClosed = I2P_IsConnected(ConnA,ConnB,StartId,EndId);
            // this will create a new couple in the Chains arr (update also ChainSize)
          I2P_AppendChain(Chains, StartSize, EndSize, ChainSize );
            // this will create a new element in the IsClosed arr (it uses the old ChainSize)
          if (AutoC) I2P_AppendIsClosed(IsClosed_arr, IsClosed, ChainSize-1 );
      }	
   }


   if (AutoC) {
      // now cares about closing IsClosed chains

      // start counting how many extra_nodes are necessary
      int ExtraNodes = 0;
      for (II=0;II<ChainSize;II++) if (IsClosed_arr[II]) ExtraNodes++;

      // reallocate the OutFlag, and modify Chains and OutFlag to
      // take care of extranodes and closed chains
      int OldSize = Nnodes_out;
      int NewSize = OldSize+ExtraNodes;
      Nnodes_out = NewSize;

      int* NewOutFlag = (int*)malloc(NewSize*sizeof(int));
      int* CurrentOldPos = OutFlag;	//this will move through the Old OutFlag
      int* CurrentNewPos = NewOutFlag; //this will move through the New OutFlag
      int AppSize = 0;
      ExtraNodes = 0;

      for (II=0;II<ChainSize;II++) {
         //update Chains (taking care of ExtraNodes already founded)
         StartSize = Chains[II*2+0] += ExtraNodes;
         EndSize = Chains[II*2+1] += ExtraNodes;
         AppSize = EndSize - StartSize + 1;

         if (IsClosed_arr[II]) {
              //copy old indexes, add one to close the chain, modify Chains
            memcpy(CurrentNewPos,CurrentOldPos,sizeof(int)*AppSize);
              // Add an extra reference to the starting node
            CurrentNewPos[AppSize] = CurrentOldPos[0];
            CurrentNewPos += AppSize+1;
            CurrentOldPos += AppSize;
            Chains[II*2+1] += 1;
            ExtraNodes++;
         } else {
              // leave chains as it is, and memcpy OutFlag infos
            memcpy(CurrentNewPos,CurrentOldPos,sizeof(int)*AppSize);
            CurrentOldPos += AppSize;
            CurrentNewPos += AppSize;
         }
      }

      // now replace the OutFlag with the New one
      free(OutFlag);
      OutFlag = NewOutFlag;
   }


      // now get all the in info (coords and node_data[0]) and copy them to the out field
      
      // First setup coordinate info and get coordinate arrays.

   float *in_fld_coordinates = (float *)in_fld.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
   if (!in_fld_coordinates) {
      printf("Error: Cannot get in_fld.coordinates\n");
      free(ConnB);
      free(ConnA);
      free(OutFlag);
      free(Chains);
      if (AutoC) free(IsClosed_arr);
      free(VisitFlag);
      free(VisitFlag2);
      return(0);
   }

   // out (Mesh+Node_Data write)
   int NSpace = (int)in_fld.nspace;
   out.nspace = NSpace;
   
   out.nnodes = (int)Nnodes_out;
   float *out_coordinates = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

   if (!out_coordinates) {
      printf("Error: Cannot get out.coordinates\n");
      free(ConnB);
      free(ConnA);
      free(OutFlag);
      free(Chains);
      if (AutoC) free(IsClosed_arr);
      free(VisitFlag);
      free(VisitFlag2);
      if (in_fld_coordinates) ARRfree(in_fld_coordinates);
      return(0);
   }


      // Next setup node data info and get node data arrays.

   float *in_fld_node_data = NULL;
   if (UseND) {
      if ((int)in_fld.nnode_data<1) {
         printf("Warning: Isolines have no node_data!\n");
         UseND=0;
      }
      else {
         in_fld_node_data = (float *)in_fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD);
         if (!in_fld_node_data) {
            printf("Warning: Cannot get node data\n");
            UseND=0;
         } else if ((int)in_fld.node_data[0].veclen!=1) {
            printf("Warning: Isoline node_data is expected to be scalar!\n");
            UseND=0;
         }
         else {
               // here be some final checking: Isolines should only have one node_data
            if ((int)in_fld.nnode_data>1) printf("Warning: Only first set of node data is being used!\n");
         }
      }
   }

   float *out_fld_node_data = NULL;
   if (UseND) {
      out.nnode_data=1;
      out.node_data[0].veclen=1;
      int NodeType=0;
      FLDget_node_data_type (in_fld.obj_id(),0,&NodeType);
      FLDset_node_data_type (out.obj_id(),0,NodeType);
      if (!(NodeType==4)) {
         printf("Warning: NodeType should be float (4)!\n");
         UseND=0;
         out.nnode_data=0;
      }
      else {
         out_fld_node_data = (float *)out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
         if (!out_fld_node_data) {
            printf("Warning: Cannot get out node_data\n");
            UseND=0;
            out.nnode_data=0;
         }
      }
   }
   else {
      out.nnode_data=0;
   }


   int CheckCounter=0;
   int VecLen=1;  // Future extensions could set VecLen properly
   int InIndex=0;

   for (II=0; II<Nnodes_out; II++) {
      InIndex = OutFlag[II];	
      if (InIndex>=0) {	// this node must be posted out
           // copy coordinates
         memcpy(out_coordinates+(II*NSpace),in_fld_coordinates+(InIndex*NSpace),sizeof(float)*NSpace);
           // copy node_data
         if (UseND) memcpy(out_fld_node_data+(II*VecLen),in_fld_node_data+(InIndex*VecLen),sizeof(float)*VecLen);
         CheckCounter++;
      }
   }

   if (CheckCounter!=Nnodes_out) 
      printf("Warning: CheckCounter (%d) not equal to Nnodes_out (%d)\n", CheckCounter, Nnodes_out);


      // Finally setup cell_set data

   out.ncell_sets=1;
   out.cell_set[0].npolys = ChainSize;
   int *out_conn = (int *)out.cell_set[0].poly_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
   if (!out_conn) {
      printf("Warning: Cannot get out connectivity\n");
   } else {
      memcpy(out_conn, Chains, sizeof(int)*2*ChainSize);
   }


      // Free allocated arrays

   free(ConnB);
   free(ConnA);
   free(OutFlag);
   free(Chains);
   if (AutoC) free(IsClosed_arr);
   free(VisitFlag);
   free(VisitFlag2);
   
   if (in_fld_coordinates) ARRfree(in_fld_coordinates);
   if (out_coordinates) ARRfree(out_coordinates);
   if (out_conn) ARRfree(out_conn);
   if (UseND) {
      if (in_fld_node_data) ARRfree(in_fld_node_data);
      if (out_fld_node_data) ARRfree(out_fld_node_data);
   }


   // return 1 for success
   return(1);
}

