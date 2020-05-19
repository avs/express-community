
#include <malloc.h>
#include "xp_comm_proj/isolbl2/gen.hxx"

//#define GFDEBUG
//#define GFDEBUG2


void SearchNext(int* Back, int* Forw, int* VisitFlag, int& JJ, int& GoOn) {
    if ((Forw[JJ]>=0) && (!VisitFlag[Forw[JJ]])) { // try forward
      JJ=Forw[JJ]; GoOn=1; 
    }
    else 
    if ((Back[JJ]>=0) && (!VisitFlag[Back[JJ]])) { // try backward
      JJ=Back[JJ]; GoOn=1;
    }
}
void MarkAndPost (int JJ, int* VisitFlag, int FreqCounter, int Frequency, int markNodes, int* OutFlag, int& Nnodes_out) {

    VisitFlag[JJ]=1;	// mark as visited
    if ( (markNodes) && ((FreqCounter%Frequency)==0) ) {
          OutFlag[JJ]=1;	// post this node to output mesh
          Nnodes_out++;
#ifdef GFDEBUG2
      printf("Added node %d on output\n",JJ);
#endif
    }
}

int
Isolabel2_Isolabel2Mods_Isolabel2Core::update(OMevent_mask event_mask, int seq_num)
{
      // Active (OMXint read notify)
      if ((int)Active==0) return(1);

      // how much downsize to use
      int ContourFreqs=(int)ContourFrequency;
      int Freqs=(int)NodeFrequency;

      if (ContourFreqs<=0) ContourFreqs = 1;
      if (Freqs<=0) Freqs = 1;

#ifdef GFDEBUG
      printf("Contour Frequency: %d\n", ContourFreqs);
      printf("Node Frequency: %d\n", Freqs);
#endif
      
      int II,JJ;
      
      // reading connectivity of each cell set I build these two array, to fast visit the
      // isolines network.
      int* Forw=NULL; // will be Forw[in_fld.nnodes]; if Forw[I]=J it means I==>J, i.e. the natural Isoline connectivity
      int* Back=NULL; // will be Back[in_fld.nnodes]; if Back[I]=J it means I<==J, the back connectivity
      
      int Nnodes_in=(int)in_fld.nnodes;
#ifdef GFDEBUG
      printf("Nnodes_in: %d\n",Nnodes_in);
#endif

      // this will count the number of out nodes
      int Nnodes_out=0;

      if (Nnodes_in==0) {
            out.nnodes=0;
            out.ncell_sets=0;
            out.nnode_data=0;
            return(1);
      }
      if (((int)in_fld.nnode_data==0)) {
            out.nnodes=0;
            out.ncell_sets=0;
            out.nnode_data=0;
            return(1);
      }

      // this will keep 0/1 if the in node will be copied into the out field
      int* OutFlag=NULL; // it will be OutFlag[in_fld.nnodes]; I used char, indeed I need just a flag bit.
      
      // this will keep 0/1 if the node has been already visited
      int* VisitFlag=NULL; // it will be OutFlag[in_fld.nnodes]; I used char, indeed I need just a flag bit.
      
   // in_fld (Mesh+Node_Data read notify)

      // first of all alloc the required arrays
      Forw = (int*) malloc(sizeof(int)*Nnodes_in);
      Back = (int*) malloc(sizeof(int)*Nnodes_in);
      OutFlag = (int*) malloc(sizeof(int)*Nnodes_in);
      VisitFlag = (int*) malloc(sizeof(int)*Nnodes_in);
      for (II=0;II<Nnodes_in;II++) {
            Forw[II]=-1;		//not connected
            Back[II]=-1;		//not connected
            OutFlag[II]=0;		//false
            VisitFlag[II]=0;	//false
      }
#ifdef GFDEBUG
      printf("Allocation done\n");
#endif

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
   
   // now collect Back and Forw
#ifdef GFDEBUG
      printf("in_fld_ncell_sets: %d\n",in_fld_ncell_sets);
      printf("Collect Back & Forw\n");
#endif

   for (in_fld_set=0; in_fld_set < in_fld.ncell_sets; in_fld_set++) {
      in_fld_ncell_connect = (int)in_fld.cell_set[in_fld_set].ncells;
#ifdef GFDEBUG
      printf("Visiting cell_set number: %d; cells are: %d\n",in_fld_set,in_fld_ncell_connect);
#endif
      // here I check that cell_nnodes = 2, Isolines are in a line_mesh
      if (! ((int)in_fld.cell_set[in_fld_set].cell_nnodes==2) ) printf("Something wrong in cell_nnodes!");
      
      in_fld_node_connect_list = (int *)in_fld.cell_set[in_fld_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);
      for (II=0;II<in_fld_ncell_connect;II++) {
            From = in_fld_node_connect_list[II*2+0];
            To = in_fld_node_connect_list[II*2+1];
            Forw[From]=To;
            Back[To]=From;
      };
      if (in_fld_node_connect_list) ARRfree(in_fld_node_connect_list);
   }

      // this counts nodes during IsoLines traversal, each time it reachs the Frequency
      // a node is marked for output posting. (and count restarts)
      int FreqCounter=0;
      
#ifdef GFDEBUG
      printf("Following chains\n");
#endif
      
   int GoOn = 0;
   int GoFwd = -1;
   int contoursFound = 0;
   int markNodes = 1;

   // now follows chains of nodes.
   for (II=0;II<Nnodes_in;II++) {
#ifdef GFDEBUG2
      printf("Visit[%d]=%d, Back[%d]=%d, Forw[%d]=%d\n",II,VisitFlag[II],II,Back[II],II,Forw[II]);
#endif
      if (!VisitFlag[II]) {
#ifdef GFDEBUG2
      printf("Node %d is a Starting point\n",II);
#endif

          if ((contoursFound%ContourFreqs)==0) {
             markNodes = 1;
          } else {
             markNodes = 0;
          }

          contoursFound++;
          FreqCounter=0;
          JJ = II;
          
          // mark as visited and post it
          MarkAndPost(JJ, VisitFlag, FreqCounter, Freqs, markNodes, OutFlag, Nnodes_out);
          
          FreqCounter=1;
          GoOn=1;
          // start to follow Isoline chain
          while (GoOn) {
#ifdef GFDEBUG2
            printf("Follow Chain, now on node JJ: %d, Freq=%d\n",JJ,FreqCounter);
#endif
                  // mark as visited and post it if necessary
                MarkAndPost(JJ, VisitFlag, FreqCounter, Freqs, markNodes, OutFlag, Nnodes_out);
                FreqCounter++;
                // try to search another node to go on
                GoOn=0;
                SearchNext(Back, Forw, VisitFlag, JJ, GoOn); // if found it modify JJ and GoOn==1
          }
          
          JJ=II;
          FreqCounter=1;
          GoOn=1;
          // start to follow Isoline chain
          while (GoOn) {
#ifdef GFDEBUG2
            printf("Follow Chain, now on node JJ: %d, Freq=%d\n",JJ,FreqCounter);
#endif
                  // mark as visited and post it if necessary
                MarkAndPost(JJ, VisitFlag, FreqCounter, Freqs, markNodes, OutFlag, Nnodes_out);
                FreqCounter++;
                // try to search another node to go on
                GoOn=0;
                SearchNext(Back, Forw, VisitFlag, JJ, GoOn); // if found it modify JJ and GoOn==1
          }
      }	
   }

#ifdef GFDEBUG
      printf("Collected Nnodes_out: %d\n",Nnodes_out);
#endif


      // now get all the in info (coords and node_data[0]) and copy them to the out field
      
   float *in_fld_coordinates = (float *)in_fld.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
   float *in_fld_node_data = (float *)in_fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD);

    // here some check: Isolines has only one node_data (scalar), float
   if (!((int)in_fld.nnode_data==1)) printf("Warning Isolines has more than one node_data!\n");
   if (!((int)in_fld.node_data[0].veclen==1)) printf("Warning Isoline node_data veclen is expected to be 1!\n");

   // out (Mesh+Node_Data write)
   int NSpace = (int)in_fld.nspace;
   out.nspace = NSpace;
   
   out.nnodes = (int)Nnodes_out;
   float *out_coordinates = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
   if (!out_coordinates) printf("Cannot get out coordinates\n");

   out.ncell_sets=0;
   out.nnode_data=1;
   out.node_data[0].veclen=1;

   int NodeType=0;
   FLDget_node_data_type (in_fld.obj_id(),0,&NodeType);
   FLDset_node_data_type (out.obj_id(),0,NodeType);
   float *out_fld_node_data = NULL;
   if (!(NodeType==4)) printf("Warning NodeType should be float (4)!\n");
   else out_fld_node_data = (float *)out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);

#ifdef GFDEBUG
      printf("NSpace: %d\n",NSpace);
      printf("Nnodes_out: %d\n",Nnodes_out);
      printf("NodeType: %d\n",NodeType);
#endif

   if (!(NodeType==4)) printf("Warning NodeType should be float (4)!\n");
   if (!out_fld_node_data) printf("Cannot get out node_data\n");
#ifdef GFDEBUG
      printf("Start copy of input to output\n");
#endif

   int CheckCounter=0;
   int VecLen=1;
   for (II=0; II<Nnodes_in; II++) {
      if (OutFlag[II]) {	// this node must be posted out
            // copy coordinates
            memcpy(out_coordinates+(CheckCounter*NSpace),in_fld_coordinates+(II*NSpace),sizeof(float)*NSpace);
            // copy node_data (if float)
            if (out_fld_node_data) 
                  memcpy(out_fld_node_data+(CheckCounter*VecLen),in_fld_node_data+(II*VecLen),sizeof(float)*VecLen); 
            CheckCounter++;
      }
   }

   if (!CheckCounter==Nnodes_out) printf("Something wrong: CheckCounter=%d\n",CheckCounter);


   free(Forw);
   free(Back);
   free(OutFlag);
   free(VisitFlag);
   
   if (in_fld_coordinates) ARRfree(in_fld_coordinates);
   if (in_fld_node_data) ARRfree(in_fld_node_data);
   if (out_coordinates) ARRfree(out_coordinates);
   if (out_fld_node_data) ARRfree(out_fld_node_data);
   


   // return 1 for success
   return(1);
}

