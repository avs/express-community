 *
 * Fixed: open file failure not checked JSP 1/10/11
 * patran.cxx
 */

#include "patran.hxx"

load_neutral::load_neutral()
{
	// Set limits to the card buffers and allocate the memory

	// Node data
	BlockSize.NodeData = PN_NODEDATA_BUFLEN;
	BufferMax.NodeData = BlockSize.NodeData;
	NodeData = (struct str_pnNodeData *) 
		malloc(sizeof(struct str_pnNodeData)*BlockSize.NodeData);
	if( ! NodeData ) {
		puts("PATRAN reader cannot allocate buffer");
		exit(1);
	}

	// Element data
	BlockSize.ElementData = PN_ELEMENTDATA_BUFLEN;
	BufferMax.ElementData = BlockSize.ElementData;
	ElementData = (struct str_pnElementData *) 
		malloc(sizeof(struct str_pnElementData)*BlockSize.ElementData);
	if( ! ElementData ) {
		puts("PATRAN reader cannot allocate buffer");
		exit(1);
	}

	// Grid data
	BlockSize.GridData = PN_GRIDDATA_BUFLEN;
	BufferMax.GridData = BlockSize.GridData;
	GridData = (struct str_pnGridData *) 
		malloc(sizeof(struct str_pnGridData)*BlockSize.GridData);
	if( ! GridData ) {
		puts("PATRAN reader cannot allocate buffer");
		exit(1);
	}

	// Line data
	BlockSize.LineData = PN_LINEDATA_BUFLEN;
	BufferMax.LineData = BlockSize.LineData;
	LineData = (struct str_pnLineData *) 
		malloc(sizeof(struct str_pnLineData)*BlockSize.LineData);
	if( ! LineData ) {
		puts("PATRAN reader cannot allocate buffer");
		exit(1);
	}

	// Patch data
	BlockSize.PatchData = PN_PATCHDATA_BUFLEN;
	BufferMax.PatchData = BlockSize.PatchData;
	PatchData = (struct str_pnPatchData *) 
		malloc(sizeof(struct str_pnPatchData)*BlockSize.PatchData);
	if( ! PatchData ) {
		puts("PATRAN reader cannot allocate buffer");
		exit(1);
	}


	/*
	 *  Set all the counters etc ready to read the first file
	 */
	reset_buffer();
}

load_neutral::~load_neutral()
{
	// free the input file
	if(fp)fclose(fp);

	// free the card buffers
	if(NodeData)free(NodeData);
	if(ElementData)free(ElementData);
	if(GridData)free(GridData);
	if(LineData)free(LineData);
	if(PatchData)free(PatchData);

}

int load_neutral::read_neutral_file(char *filename)
{
	int ok = pnOK;
	reset_buffer();
	if(!getFile(filename))
	  return(pnFAIL);
	
	while( ok == pnOK) { 
		ok = load_packet(); 
	}

	collate_connectivity();

	return(ok);
}

int load_neutral::getFile(char *filename)
{
	fp = fopen(filename,"r");
	if( fp ) {
		return(1);
	}
	else {
		printf("cannot open \"%s\"\n",filename);
		return(0);
	}
}

void load_neutral::reset_buffer()
{
	// reinitialise buffers to allow a new data set to be read in
	fp = NULL;

	// reset counters
	BufferCount.NodeData = 0;
	BufferCount.ElementData = 0;
	BufferCount.GridData = 0;
	BufferCount.LineData = 0;
	BufferCount.PatchData = 0;

	// reset pointers into the buffers
	pNodeData = NodeData;
	pElementData = ElementData;
	pGridData = GridData;
	pLineData = LineData;
	pPatchData = PatchData;

	// reset the reoprt
	init_report();
}

int load_neutral::load_packet_header()
{
	/*
	 * load the header line for the next packet
	 *
	 * returns 1 for success; 0 for end of file; -1 for failure
	 */
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) ) return(pnEOF);

	int ok = pnOK;

	if(! intFromString(line_buffer,0,1,&(Header.it)) ) ok = pnFAIL;
	if(! intFromString(line_buffer,2,9,&(Header.id)) ) ok = pnFAIL;
	if(! intFromString(line_buffer,10,17,&(Header.iv)) ) ok = pnFAIL;
	if(! intFromString(line_buffer,18,25,&(Header.kc)) ) ok = pnFAIL;
	if(! intFromString(line_buffer,26,33,&(Header.n1)) ) ok = pnFAIL;
	if(! intFromString(line_buffer,34,41,&(Header.n2)) ) ok = pnFAIL;
	if(! intFromString(line_buffer,42,49,&(Header.n3)) ) ok = pnFAIL;
	if(! intFromString(line_buffer,50,57,&(Header.n4)) ) ok = pnFAIL;
	if(! intFromString(line_buffer,58,65,&(Header.n5)) ) ok = pnFAIL;

	// update the report
	if(Header.it >= 0 && Header.it < 100) Report[Header.it].count++;

	return(ok);
}

void load_neutral::init_report()
{
	int i;
	for(i=0;i<100;i++) {
		Report[i].possible = 0;
		Report[i].count = 0;
		strcpy(Report[i].text,"");
	}

init_report_element( P_TITLE, "TITLE");
init_report_element( P_SUMMARY , "SUMMARY");  
/* FEM packets */
init_report_element( P_NODE_DATA, "NODE DATA");
init_report_element( P_ELEMENT_DATA	, "ELEMENT DATA");
init_report_element( P_MATERIAL_PROP, "material properties (IGNORED)");	 
init_report_element( P_COORD_FRAME	, "coordinate frame (IGNORED)");
init_report_element( P_DIST_LOAD	, "distributed loads (IGNORED)");
init_report_element( P_NODE_FORCE	, "node forces (IGNORED)");
init_report_element( P_NODE_DISP	, "node displacements (IGNORED)");
init_report_element( P_BAR_IDISP	, "bar element initial displacements (IGNORED)");
init_report_element( P_NODE_TEMP		, "node temperatures (IGNORED)");
init_report_element( P_ELEMENT_TEMP	, "element temperatures (IGNORED)");
init_report_element( P_DOF_LIST, "degree of freedom list (IGNORED)");
init_report_element( P_MECH_ENT	, "mechanism entities (IGNORED)");
init_report_element( P_MPC_DATA	, "mpc data (IGNORED)");
init_report_element( P_NODAL_HEAT, "nodal heat source (IGNORED)");
init_report_element( P_ELEMENT_HEAT, "distributed heat source (IGNORED)");
init_report_element( P_CONVECTION, "convection coefficients (IGNORED)");
init_report_element( P_RADIATION	, "radiation emissivity values (IGNORED)");
init_report_element( P_VIEW_FACTOR, "view factor data (IGNORED)");
init_report_element( P_NAME_COMP	, "named components (IGNORED)");
/* geom model packets */
init_report_element( P_GRID_DATA	, "grid data");
init_report_element( P_LINE_DATA	, "LINE DATA");
init_report_element( P_PATCH_DATA	, "PATCH");
init_report_element( P_HYPER_DATA	, "hyperpatch data (IGNORED)");
init_report_element( P_DATA_LINE_DATA, "data-line data (IGNORED)");
init_report_element( P_DATA_PATCH_DATA, "data-patch data (IGNORED)");
init_report_element( P_DATA_HYPER_DATA, "data-hyperpatch data (IGNORED)");
init_report_element( P_FIELD_DATA	, "field-data (coefficient format) (IGNORED)");
init_report_element( P_FIELD_DATA_PCL, "field-data (pcl format) (IGNORED)");
init_report_element( P_LIST_CARD	, "list card (IGNORED)");
init_report_element( P_DATA_CARD	, "data card (IGNORED)");
/* GFEG/CFEG */
init_report_element( P_GFEG_LINE4	, "gfeg table for a line (IGNORED)");
init_report_element( P_GFEG_PATCH	, "gfeg table for a patch (IGNORED)");
init_report_element( P_GFEG_HYPER	, "gfeg table for a hyperpatch (IGNORED)");
init_report_element( P_CFEG		, "gfeg table (IGNORED)");
/* trimmed surface solid model */
init_report_element( P_PRIM		, "primitive data (IGNORED)");
init_report_element( P_PRIM_FACE	, "primitive face data (IGNORED)");
init_report_element( P_EOF		, "END OF FILE FLAG");

}

/*
 * init_report_element()
 */
void load_neutral::init_report_element(int i, char* s)
{
	Report[i].possible = 1;
	Report[i].count = 0;
	strncpy(Report[i].text,s,49);
}

/*
 * load_packet()
 *
 * read the next packet:
 * - read the packet header (at this point end of file is detectable).
 *   depending upon packet id read the remaining cards via various
 *   rdXxxxx() functions.  Some of these functions store arrays of
 *   things.  These functions can extend (realloc) these arrays.
 */
int load_neutral::load_packet()
{
	/* return:
	 *   2 for ok but a buffer is full
	 *   1 for ok
	 *   0 for end of file encountered instead of a new packet
	 *  -1 for failure during a record
	 */

	// get the header
	int ok = load_packet_header();

	if(ok == 1) {
		// header successfully read in
		// read packet body depending upon Header.it (packet type).
		switch (Header.it) {
		case P_TITLE: 
			{
				// read the title card
				ok = rdTitle();
			}
		break; 
		case P_SUMMARY:
			{
				ok = rdSummary();
			}
		break;
		case P_NODE_DATA:
			{
				ok = rdNodeData();
			}
		break;
		case P_ELEMENT_DATA:
			{
				ok = rdElementData();
			}
		break;
		case P_GRID_DATA:
			{
				ok = rdGridData();
			}
		break;
		case P_LINE_DATA:
			{
				ok = rdLineData();
			}
		break;
		case P_PATCH_DATA:
			{
				ok = rdPatchData();
			}
		break;
		case P_EOF:
			{
				return(pnEOF);
			}
		break;
		default: 
			{
				// packet type is not handled; just scan
				// the required number of packet cards
				int i;
				for(i=0;i<Header.kc;i++) {
					if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) ) ok = pnFAIL;
				}
			}
		}
	}


	return(ok);
}

/*
 * collate_connectivity()
 * The connectivity list for each element is stored in nodes[].
 * Translate this to connectivity[] which uses node order rather
 * than node-id.
 *
 * The search assumes that node-id increase with node-order
 */
void load_neutral::collate_connectivity()
{
	int Nnodes = BufferCount.NodeData;
	int Nelements = BufferCount.ElementData;
	int Iele;
	int Inod;
	struct str_pnElementData *ed;
#define MY_NODE_ID(i) (NodeData[ i ].id)

	Inod = 0;


	for(Iele=0;Iele<Nelements;Iele++) {
		ed = &(ElementData[Iele]);
		int Icon;
		int Nconn = ed->n_nodes;

		ed->nodes_do_exist = 1; // flag; all nodes are found

		/** LOOP OVER NODES **/
		for(Icon=0;Icon<Nconn;Icon++) {
			int found = 0;

			// the node id to find
			int id2find = ed->nodes[Icon];

			// first attempt to find the node:
			// assume that the node is the on that was accessed last
			if(MY_NODE_ID(Inod) == id2find) {
				found = 1;
				ed->connectivity[Icon] = Inod;
			}

			// second attempt to find the node.
			// assume that node order and id are identical
			// (except that node-id is 1-based and node order
			// is zero based.)
			if( ! found ) {
				if((id2find > 0) && (id2find <= Nnodes)) {
					Inod = id2find - 1;
					if(id2find == MY_NODE_ID(Inod)) {
						found = 1;
						ed->connectivity[Icon] = Inod;
					}
				}
			}

			// third attempt:
			// assume that things run cleanly in ascending order;
			// search downward or upward from the halfway point.
			int half_way = Nnodes / 2;

			if( ! found ) {
				if( MY_NODE_ID(half_way) < id2find ) {
					for(Inod = half_way; Inod<Nnodes; Inod++) {
						if(id2find == MY_NODE_ID(Inod)) {
							found = 1;
							ed->connectivity[Icon] = Inod;
							break; // MV added
						}
					}
				}
				else {
					for(Inod = half_way; Inod>=0; Inod--) {
						if(id2find == MY_NODE_ID(Inod)) {
							found = 1;
							ed->connectivity[Icon] = Inod;
							break; // MV added
						}
					}
				}
			}

			// lastly search the rest of the tree
			if( ! found ) {
				if( MY_NODE_ID(half_way) >= id2find ) {
					for(Inod = half_way; Inod<Nnodes; Inod++) {
						if(id2find == MY_NODE_ID(Inod)) {
							found = 1;
							ed->connectivity[Icon] = Inod;
							break; // MV added
						}
					}
				}
				else {
					for(Inod = half_way; Inod>=0; Inod--) {
						if(id2find == MY_NODE_ID(Inod)) {
							found = 1;
							ed->connectivity[Icon] = Inod;
							break; // MV added
						}
					}
				}
			}

			// if it still isn't found set it to node 0
			if( ! found ) {
				Inod = 0;
				ed->connectivity[Icon] = Inod;

				// failed to find a node so we cannot use this cell
				ed->nodes_do_exist = 0;
			}
		}  // END OF LOOP OVER CONNECTIVITY

	} // END OF LOOP OVER ELEMENTS
}

/********************/
/** PACKET READERS **/
/********************/

int load_neutral::rdTitle() 
{
  // title should only contain one card.
	if(Header.kc != 1)return(pnFAIL);
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	strncpy(Summary.title,line_buffer,PN_TITLE_LENGTH-1);
	return(pnOK);
}

int load_neutral::rdSummary() 
{
  // summary should only contain one card.
	if(Header.kc != 1)return(pnFAIL);
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	strncpy(Summary.title2,line_buffer,PN_TITLE_LENGTH-1);
	Summary.summary_n_nodes = Header.n1;
	Summary.summary_n_elements = Header.n2;
	Summary.summary_n_materials = Header.n3;
	Summary.summary_n_elem_props = Header.n4;
	Summary.summary_n_coord_fram = Header.n5;
	return(pnOK);
}

int load_neutral::rdNodeData()
{
	// node data should only contain 2 cards
	if(Header.kc != 2)return(pnFAIL);

	int ok = pnOK;

	// load data from the header
	pNodeData->id = Header.id;

	// the first card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&(pNodeData->x)) ) ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&(pNodeData->y)) ) ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&(pNodeData->z)) ) ok = pnFAIL;

	// the second card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! intFromString(line_buffer,0,0,&(pNodeData->icf)))ok=pnFAIL;
	pNodeData->gtype = line_buffer[1];
	if( ! intFromString(line_buffer,2,9,&(pNodeData->ndf)))ok=pnFAIL;
	if( ! intFromString(line_buffer,10,17,&(pNodeData->config)))ok=pnFAIL;
	if( ! intFromString(line_buffer,18,25,&(pNodeData->cid)))ok=pnFAIL;
	if( ! intFromString(line_buffer,28,28,&(pNodeData->pspc1)))ok=pnFAIL;
	if( ! intFromString(line_buffer,29,29,&(pNodeData->pspc2)))ok=pnFAIL;
	if( ! intFromString(line_buffer,30,30,&(pNodeData->pspc3)))ok=pnFAIL;
	if( ! intFromString(line_buffer,31,31,&(pNodeData->pspc4)))ok=pnFAIL;
	if( ! intFromString(line_buffer,32,32,&(pNodeData->pspc5)))ok=pnFAIL;
	if( ! intFromString(line_buffer,33,33,&(pNodeData->pspc6)))ok=pnFAIL;

	// if everything was OK increment the header count and pointer
	if(ok == 1) {
		BufferCount.NodeData++;
		pNodeData++;

		// detect whether the buffer is full; extend the buffer
		if(BufferCount.NodeData == BufferMax.NodeData) {
			int new_max = BufferMax.NodeData + BlockSize.NodeData;
			int new_size = new_max * sizeof(str_pnNodeData);
			NodeData = (struct str_pnNodeData *) realloc((void *)NodeData,new_size);
			if( NodeData ) {
				BufferMax.NodeData = new_max;
				pNodeData = NodeData;
				int i;
				for(i=0;i<=BufferCount.NodeData;i++)pNodeData++;
				ok = pnOK;
			}
			else {
				ok = pnCATASTROPHE;
			}
		}
	}

	return(ok);
}


int load_neutral::rdElementData()
{
	// element data should only contain at least 2 cards
	if(Header.kc < 2)return(pnFAIL);

	int ok = pnOK;

	int n_data_cards = Header.kc - 2;

	// load data from the header
	pElementData->id = Header.id;
	pElementData->type = Header.iv;
	pElementData->n_data = Header.n1;
	pElementData->id_node_in_xy = Header.n2;

	// the first card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! intFromString(line_buffer, 0, 7,&(pElementData->n_nodes)) )ok=pnFAIL;
	if( ! intFromString(line_buffer, 8,15,&(pElementData->config)) )ok=pnFAIL;
	int ii = 0;
	if( ! intFromString(line_buffer,16,23,&ii) )ok=pnFAIL;
	if(ii > 0){
		pElementData->property_id = ii;
		pElementData->material_id = 0;
	}
	if(ii < 0){
		pElementData->property_id = 0;
		pElementData->material_id = -1 * ii;
	}
	else {
		pElementData->property_id = 0;
		pElementData->material_id = 0;
	}
	if( ! intFromString(line_buffer,24,31,&(pElementData->ceid)) )ok=pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&(pElementData->theta1)) )ok=pnFAIL;
	if( ! doubleFromString(line_buffer,48,63,&(pElementData->theta2)) )ok=pnFAIL;
	if( ! doubleFromString(line_buffer,64,79,&(pElementData->theta3)) )ok=pnFAIL;

	// the second card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	int i_nodes;
	for(i_nodes=0;i_nodes<PN_MAX_ELEM_NODES;i_nodes++) {
		if(i_nodes < pElementData->n_nodes) {
			int jj;
			if(intFromString(line_buffer,i_nodes*8,7+i_nodes*8,&jj)) {
				pElementData->nodes[i_nodes] = jj;
			}
			else {
				ok = pnFAIL;
				pElementData->nodes[i_nodes] = 0;
			}
		}
		else {
			pElementData->nodes[i_nodes] = 0;
		}
	}


	// the (optional) third card - for data
	if(n_data_cards > 0) {
		if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
		int i;
		for(i=0;i<PN_MAX_ELEM_DATA;i++) {
			if( i < pElementData->n_data) {
				double r = 0.0;
				if( ! doubleFromString(line_buffer,i*8,7+i*8,&r) )ok = pnFAIL;
				pElementData->elem_data[i] = r;
			}
			else {
				pElementData->elem_data[i] = 0.0;
			}
		}
	}

	// subsequent data cards; all ignored
	for(int k=1;k<n_data_cards;k++) {
		if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	}


	// if everything was OK increment the header count and pointer
	if(ok == 1) {
		BufferCount.ElementData++;
		pElementData++;

		// detect whether the buffer is full; extend the buffer
		if(BufferCount.ElementData == BufferMax.ElementData) {
			int new_max = BufferMax.ElementData + BlockSize.ElementData;
			int new_size = new_max * sizeof(str_pnElementData);
			ElementData = (struct str_pnElementData *) realloc((void *)ElementData,new_size);
			if( ElementData ) {
				BufferMax.ElementData = new_max;
				pElementData = ElementData;
				int i;
				for(i=0;i<=BufferCount.ElementData;i++)pElementData++;
				ok = pnOK;
			}
			else {
				ok = pnCATASTROPHE;
			}
		}
	}

	return(ok);
}


int load_neutral::rdGridData()
{
	// grid data should only contain 1 card
	if(Header.kc != 1)return(pnFAIL);

	int ok = pnOK;

	// load data from the header
	pGridData->id = Header.id;

	// the first card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&(pGridData->x)) ) ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&(pGridData->y)) ) ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&(pGridData->z)) ) ok = pnFAIL;

	// if everything was OK increment the header count and pointer
	if(ok == 1) {
		BufferCount.GridData++;
		pGridData++;

		// detect whether the buffer is full; extend the buffer
		if(BufferCount.GridData == BufferMax.GridData) {
			int new_max = BufferMax.GridData + BlockSize.GridData;
			int new_size = new_max * sizeof(str_pnGridData);
			GridData = (struct str_pnGridData *) realloc((void *)GridData,new_size);
			if( GridData ) {
				BufferMax.GridData = new_max;
				pGridData = GridData;
				int i;
				for(i=0;i<=BufferCount.GridData;i++)pGridData++;
				ok = pnOK;
			}
			else {
				ok = pnCATASTROPHE;
			}
		}
	}

	return(ok);
}


/*
 * read a line (spline)
 */
int load_neutral::rdLineData()
{
	int ok = pnOK;
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	puts("need to finish line-data reader");

	// if everything was OK increment the header count and pointer
	if(ok == 1) {
		BufferCount.LineData++;
		pLineData++;

		// detect whether the buffer is full; extend the buffer
		if(BufferCount.LineData == BufferMax.LineData) {
			int new_max = BufferMax.LineData + BlockSize.LineData;
			int new_size = new_max * sizeof(str_pnLineData);
			LineData = (struct str_pnLineData *) realloc((void *)LineData,new_size);
			if( ElementData ) {
				BufferMax.LineData = new_max;
				pLineData = LineData;
				int i;
				for(i=0;i<=BufferCount.LineData;i++)pLineData++;
				ok = pnOK;
			}
			else {
				ok = pnCATASTROPHE;
			}
		}
	}
	return(ok);
}

/*
 * read a patch (spline surface)
 */
int load_neutral::rdPatchData()
{
	// should contain 10 lines
	if(Header.kc != 10)return(pnFAIL);

	int ok = pnOK;

	// number of quads along each side (i.e. the resolution)
	// 0 = use corners only
	// 1 = use coefficients as vertices (i.e. 3x3)
	// 2 = double number of quads (i.e. 6x6)
	// 3 = triple the number of quads (i.e. 9x9)
	// etc.
	// (Currently hard code to 0)

	int res = 0;
	pPatchData->resolution = res;
	if(res == 0) {
		pPatchData->verts_per_side = 2;
		pPatchData->n_quads = 1;
		pPatchData->n_verts = 4;
	}
	else if(res == 1) {
		pPatchData->verts_per_side = 4;
		pPatchData->n_quads = 9;
		pPatchData->n_verts = 16;
	}
	else {
		pPatchData->verts_per_side = 3*res + 1;
		pPatchData->n_quads = (3*res)*(3*res);
		pPatchData->n_verts = (3*res+1)*(3*res+1);
	}

	// set id from header
	pPatchData->id = Header.id;

	double c1,c2,c3,c4,c5;
	double *pc = &(pPatchData->coefficients[0][0]);
	int i1,i2,i3,i4;

	// first card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&c1) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&c2) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&c3) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,48,63,&c4) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,64,79,&c5) )ok = pnFAIL;
	*pc=c1; pc++; *pc=c2;pc++; *pc=c3;pc++; *pc=c4;pc++; *pc=c5;pc++;

	// second card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&c1) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&c2) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&c3) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,48,63,&c4) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,64,79,&c5) )ok = pnFAIL;
	*pc=c1; pc++; *pc=c2;pc++; *pc=c3;pc++; *pc=c4;pc++; *pc=c5;pc++;

	// third card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&c1) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&c2) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&c3) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,48,63,&c4) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,64,79,&c5) )ok = pnFAIL;
	*pc=c1; pc++; *pc=c2;pc++; *pc=c3;pc++; *pc=c4;pc++; *pc=c5;pc++;
	// fourth card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&c1) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&c2) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&c3) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,48,63,&c4) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,64,79,&c5) )ok = pnFAIL;
	*pc=c1; pc++; *pc=c2;pc++; *pc=c3;pc++; *pc=c4;pc++; *pc=c5;pc++;

	// fifth card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&c1) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&c2) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&c3) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,48,63,&c4) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,64,79,&c5) )ok = pnFAIL;
	*pc=c1; pc++; *pc=c2;pc++; *pc=c3;pc++; *pc=c4;pc++; *pc=c5;pc++;

	// sixth card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&c1) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&c2) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&c3) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,48,63,&c4) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,64,79,&c5) )ok = pnFAIL;
	*pc=c1; pc++; *pc=c2;pc++; *pc=c3;pc++; *pc=c4;pc++; *pc=c5;pc++;

	// seventh card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&c1) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&c2) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&c3) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,48,63,&c4) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,64,79,&c5) )ok = pnFAIL;
	*pc=c1; pc++; *pc=c2;pc++; *pc=c3;pc++; *pc=c4;pc++; *pc=c5;pc++;

	// eigth card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&c1) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&c2) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&c3) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,48,63,&c4) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,64,79,&c5) )ok = pnFAIL;
	*pc=c1; pc++; *pc=c2;pc++; *pc=c3;pc++; *pc=c4;pc++; *pc=c5;pc++;

	// ninth card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&c1) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&c2) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&c3) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,48,63,&c4) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,64,79,&c5) )ok = pnFAIL;
	*pc=c1; pc++; *pc=c2;pc++; *pc=c3;pc++; *pc=c4;pc++; *pc=c5;pc++;

	// tenth card
	if( ! fgets(line_buffer,PN_MAX_BUF_LEN-1,fp) )return(pnFAIL);
	if( ! doubleFromString(line_buffer, 0,15,&c1) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,16,31,&c2) )ok = pnFAIL;
	if( ! doubleFromString(line_buffer,32,47,&c3) )ok = pnFAIL;
	if( ! intFromString(line_buffer,48,55,&i1) )ok = pnFAIL;
	if( ! intFromString(line_buffer,56,63,&i2) )ok = pnFAIL;
	if( ! intFromString(line_buffer,64,71,&i3) )ok = pnFAIL;
	if( ! intFromString(line_buffer,72,79,&i4) )ok = pnFAIL;
	*pc=c1; pc++; *pc=c2;pc++; *pc=c3;pc++;
	pPatchData->corners[0] = i1; pPatchData->corners[1] = i2;
	pPatchData->corners[2] = i3; pPatchData->corners[3] = i4;


	// if everything was OK increment the header count and pointer
	if(ok == 1) {
		BufferCount.PatchData++;
		pPatchData++;

		// detect whether the buffer is full; extend the buffer
		if(BufferCount.PatchData == BufferMax.PatchData) {
			int new_max = BufferMax.PatchData + BlockSize.PatchData;
			int new_size = new_max * sizeof(str_pnPatchData);
			PatchData = (struct str_pnPatchData *) realloc((void *)PatchData,new_size);
			if( PatchData ) {
				BufferMax.PatchData = new_max;
				pPatchData = PatchData;
				int i;
				for(i=0;i<=BufferCount.PatchData;i++)pPatchData++;
				ok = pnOK;
			}
			else {
				ok = pnCATASTROPHE;
			}
		}
	}
	return(ok);
}

/**********************/
/** ACCESS FUNCTIONS **/
/**********************/
const char *load_neutral::title1()
{
	return((const char *)Summary.title);
}

const char *load_neutral::title2()
{
	return((const char *)Summary.title2);
}


/*
 * n_nodes()
 *
 * return number of nodes required to represent the model.
 * NB this will include things used to build patches as well
 * as the nodes themselves.
 *
 * Also works out number of nodes for patches and the index of
 * the first node for patches.
 */
int load_neutral::n_nodes()
{
	int n;
	int i;

	// N.B., any changes to this routine must be reflected in the double_coordinates
	// routine which packs data into the final node data array.  If the order is changed
	// it will upset all offsets/calculations.

	// node data are first (1)
	n = BufferCount.NodeData;

    // Griddata are next (2)
	// need to include them so we can work out real id for connectivity

	int grid_start = n;
    n = n + BufferCount.GridData;

	// patch data are next (3)
	// also work out the index of the first vertex.
	// Patches index the grid data
	// If Patchdata resolution is 0 then do not include space
	// for tessellated surface as it will use grid data

	for(i=0;i<BufferCount.PatchData;i++) {
		PatchData[i].first_vertex_index = n+1;
        PatchData[i].grid_vertex_index = grid_start;
        if (PatchData[i].resolution != 0)
		{
			// add space for tessellated surface quads
			n = n + PatchData[i].n_verts;
		}
	}

	return(n);
}

/*
 * nodes_per_cell() returns the number of nodes for a cell type
 */
int load_neutral::nodes_per_cell(int type)
{
	if( type == PE_BAR ) return(2);
	else if( type == PE_TRI ) return(3);
	else if( type == PE_QUAD ) return(4);
	else if( type == PE_TET ) return(4);
	else if( type == PE_WEDGE ) return(6);
	else if( type == PE_HEX ) return(8);
	else return(0);

}

/*
 * n_elements( type, material, property )
 * Number of elements classified by type, material and property.
 * A filter value of -1 for material / property means select all.
 */
int load_neutral::n_elements(int type, int material, int property)
{
	int n = 0;
	struct str_pnElementData *ed = ElementData;
	int ne = BufferCount.ElementData;
	int i;

	for(i=0;i<ne;i++) {
		int ok = 1;
		if( ! (ed->nodes_do_exist)) ok = 0;
		else if(type != ed->type) ok = 0;
		else if((material >= 0) && (material != ed->material_id)) ok = 0;
		else if((property >= 0) && (property != ed->property_id)) ok = 0;
		
		
		if(ok) n++;
		ed++;
	}

	return(n);
}

/*
 * connectivity( type, material, property, conn)
 * Connectivity of elements classified by type, material and property.
 * A filter value of -1 for material / property means select all.
 *
 * conn is the connectivity array for all cells in the set.  it is
 * malloc()ed and free()ed by the calling routine
 */

int load_neutral::connectivity(int type, int material, int property, int *conn)
{
	int n = 0;
	struct str_pnElementData *ed = ElementData;
	int ne = BufferCount.ElementData;
	int i;

	for(i=0;i<ne;i++) {
		int ok = 1;
		if( ! (ed->nodes_do_exist)) ok = 0;
		else if(type != ed->type) ok = 0;
		else if((material >= 0) && (material != ed->material_id)) ok = 0;
		else if((property >= 0) && (property != ed->property_id)) ok = 0;
		
		
		if(ok) {
			int i;
			for(i=0;i<(ed->n_nodes);i++) {
				conn[n] = ed->connectivity[i];
				n++;
			}
		}
		ed++;
	}

	return((n > 0));
}


/*
 * coordinates() 
 * takes a preallocated buffer and loads it with the nodal coordinates
 *
 * NB returns double precision
 */
void load_neutral::double_coordinates(double *ptr)
{
	if(ptr) {
		int i;

		double *p = ptr;

		// first the simple nodes
		int nn = BufferCount.NodeData;
		if(nn > 0) {
			int i;
			struct str_pnNodeData *s = NodeData;
			for(i=0;i<nn;i++) {
				*p = s->x; p++;
				*p = s->y; p++;
				*p = s->z; p++;
				s++;
			}
		}

	    // put the grid data into the array
		// at some later stage remove this grid data and put directly into
		// patch data structure. (also edit n_nodes to fix offsets)

		struct str_pnGridData *gd = GridData;
		
		for (i=0; i < BufferCount.GridData; i++)
		{
			(*p) = gd[i].x; p++;
			(*p) = gd[i].y; p++;
			(*p) = gd[i].z; p++;
		}

		// next the vertices for the patches
		struct str_pnPatchData *pd = PatchData;

		for(i=0;i<BufferCount.PatchData;i++) 
		{			

			if(pd->resolution == 0) 
			{
				// Use a single quad coincinding with patch corners.
				// The patches index is into the grid data
				
				// at the moment nothing to be done as we already add
				// grid data to node array
			}
			else 
			{
				// Tessellate surface - not sure how to do this from 16,3 and grid pts.

				puts("CANNOT WORK OUT SPLINE SURFACE");
			}

			pd++;
		}
	}

}


/*
 * Print a report to standard out on the current file
 * contents.
 */ 
void load_neutral::print_report()
{
	printf("\n\n\nIMPORTED PATRAN DATA:\n");
	printf("\"%s\"\n",Summary.title);
	printf("\"%s\"\n",Summary.title2);
	printf("\nSummary of elements imported.\n");
	printf("Count     Element\n");
	int i;
	for(i=0;i<100;i++) {
		if(Report[i].possible) {
			printf("%8d  %s\n",Report[i].count,Report[i].text);
		}
	}
}

/*
 * number of griddata items (all these are put into node array)
 */

int load_neutral::n_grid_data()
{
	int n;

	n = BufferCount.GridData;

	return(n);
}


/*
 * number of quads used to represent patches
 */
int load_neutral::n_patch_quads()
{
	int n = 0;
	int i;
	for(i=0;i<BufferCount.PatchData;i++) {
		n = n + PatchData[i].n_quads;
	}
	return(n);
}

/*
 * connectivity list for quads used to represent all patches
 *
 * NB assumes resolution=1 (i.e. use 3x3 quads per patch.)
 */

int load_neutral::patch_connectivity(int *conn)
{
	int *c = conn;
	struct str_pnPatchData *pd = PatchData;
    struct str_pnGridData  *gd = GridData;

    int resolution;
	int n = BufferCount.PatchData;
	int i;

	for(i=0;i<n;i++) 
	{
      resolution = pd->resolution;

      if (resolution == 0)
	  { // use corner grid points
        
		  int PatchCorner;
		  int SearchId;
		  int FoundIt;
		  int GridStrId;

		  for (PatchCorner=0; PatchCorner < 4; PatchCorner++)
		  {
			SearchId = pd->corners[PatchCorner];
			FoundIt = -1;
			for (GridStrId=0; GridStrId < BufferCount.GridData; GridStrId++)
			{
				if (gd[GridStrId].id == SearchId)
					FoundIt = GridStrId;
			}

			if (FoundIt == -1)
			{
				puts("Error cannot find id");
			}
			 
			*c = FoundIt + pd->grid_vertex_index;
			c++;
		  }
	  }
	else
	{
        int nv = pd->verts_per_side;
	    int nq = nv - 1;
		int first = pd->first_vertex_index;
		int j1;
		int j2;
		for(j1=0;j1<nq;j1++) 
		{
			for(j2=0;j2<nq;j2++) 
			{
				*c = first + j1*nv +      j2;			c++;
				*c = first + j1*nv +      j2 + 1;		c++;
				*c = first + j1*nv + nv + j2 + 1;   	c++;
				*c = first + j1*nv + nv + j2;		    c++;
			}
		}
	}

		pd++;
	}

	return(1);
}
