
#define XP_WIDE_API

#include "avs/f_utils.h"
#include "xp_comm_proj/rd_patrn/gen.h"
#include "stdio.h"

int
ReadPatran_ReadPatranMods_ReadPatranCore::update(OMevent_mask event_mask, int seq_num)
{
 
   /***********************/
   /* Function's Body     */
   /***********************/

	// filename
        char Filename[AVS_PATH_MAX];
	FILEmap_variables( (const char *)filename, Filename );

	if(0 > loadN.read_neutral_file(Filename)) {
		puts("failed");
		return(0);
	}
	loadN.print_report();

	neutral_output.title1 = loadN.title1();
	neutral_output.title2 = loadN.title2();
	neutral_output.n_nodes = (xp_long)loadN.n_nodes();
	if(loadN.n_nodes()) {
		double *xyz_arr = (double *)neutral_output.xyz.ret_array_ptr(OM_GET_ARRAY_WR);
		if(xyz_arr) {
			loadN.double_coordinates(xyz_arr);
			ARRfree(xyz_arr);
		}
	}


	int Nbar = loadN.n_elements(PE_BAR, -1, -1);
	neutral_output.n_bar = (xp_long)Nbar;
	if(Nbar > 0) {
		int *c = (int *)neutral_output.bar_conn.ret_array_ptr(OM_GET_ARRAY_WR);
		if(c) {
			loadN.connectivity(PE_BAR, -1, -1, c);
			ARRfree(c);
		}
	}


	int Ntri = loadN.n_elements(PE_TRI, -1, -1);
	neutral_output.n_tri = (xp_long)Ntri;
	if(Ntri > 0) {
		int *c = (int *)neutral_output.tri_conn.ret_array_ptr(OM_GET_ARRAY_WR);
		if(c) {
			loadN.connectivity(PE_TRI, -1, -1, c);
			ARRfree(c);
		}
	}


	int Nquad = loadN.n_elements(PE_QUAD, -1, -1);
	neutral_output.n_quad = (xp_long)Nquad;
	if(Nquad > 0) {
		int *c = (int *)neutral_output.quad_conn.ret_array_ptr(OM_GET_ARRAY_WR);
		if(c) {
			loadN.connectivity(PE_QUAD, -1, -1, c);
			ARRfree(c);
		}
	}


	int Ntet = loadN.n_elements(PE_TET, -1, -1);
	neutral_output.n_tet = (xp_long)Ntet;
	if(Ntet > 0) {
		int *c = (int *)neutral_output.tet_conn.ret_array_ptr(OM_GET_ARRAY_WR);
		if(c) {
			loadN.connectivity(PE_TET, -1, -1, c);
			ARRfree(c);
		}
	}


	int Nwedge = loadN.n_elements(PE_WEDGE, -1, -1);
	neutral_output.n_wedge = (xp_long)Nwedge;
	if(Nwedge > 0) {
		int *c = (int *)neutral_output.wedge_conn.ret_array_ptr(OM_GET_ARRAY_WR);
		if(c) {
			loadN.connectivity(PE_WEDGE, -1, -1, c);
			ARRfree(c);
		}
	}


	int Nhex = loadN.n_elements(PE_HEX, -1, -1);
	neutral_output.n_hex =(xp_long)Nhex;
	if(Nhex > 0) {
		int *c = (int *)neutral_output.hex_conn.ret_array_ptr(OM_GET_ARRAY_WR);
		if(c) {
			loadN.connectivity(PE_HEX, -1, -1, c);
			ARRfree(c);
		}
	}

	int NpatchQuads = loadN.n_patch_quads();
	neutral_output.n_patch = (xp_long)NpatchQuads;
	if(NpatchQuads > 0) {
		int *c = (int *)neutral_output.patch.ret_array_ptr(OM_GET_ARRAY_WR);
		if(c) {
			loadN.patch_connectivity(c);
			ARRfree(c);
		}
	}

   // return 1 for success
   return(1);
}
// --- end of file ---

