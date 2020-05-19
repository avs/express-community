
#define XP_WIDE_API	/* Use Wide APIs */

#include "gen.h"
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 100

/*
 "ColorName","NumOfColors","Type","CritVal","ColorNum","ColorLetter","R","G","B","SchemeType"
*/

typedef struct colormap{
	char *cmap_name;
	char *cat_name;
	char * color_letter;
 	int * color_number;
	int ncolors;
	float crit_val;
	int *colors;
} colormap;

typedef struct CB_colormap{
	int ncolormaps;
	colormap *colormaps;
	int ncats;
	int * cat_col;
	int * cat_col_nmap;
	int ** cat_col_names;
	int ** cat_col_ncmaps; 
	int *** cat_col_cmapcol;
	int * cat_col_num_nmap;
	int ** cat_col_num_names;
	int ** cat_col_num_ncmaps; 
	int *** cat_col_num_cmapcol;
} CB_colormap;


char* drop_quot(char *in){
	char *out = in; 
	while (strlen(out) && out[0]=='"'){
		out++;
	}
	while (strlen(out) && out[strlen(out)-1]=='"'){
		out[strlen(out)-1]='\0';
	}
	return out;
}

int
CB_ColorMap_Inst(OMobj_id CB_ColorMapCore_id, OMevent_mask event_mask, int seq_num)
{
	/***********************/
	/*  Declare variables  */
	/***********************/
	char  *cbFile = NULL;
	int  ncats=0;
	char  **categories = NULL;
	CB_colormap * cb_colormaps = NULL;
	int * cat_col;
	int * cat_col_nmap;
	int ** cat_col_names;
	int ** cat_col_ncmaps; 
	int *** cat_col_cmapcol;
	int * cat_col_num_nmap;
	int ** cat_col_num_names;
	int ** cat_col_num_ncmaps; 
	int *** cat_col_num_cmapcol;
	char type[100];
    char buffer[512];
	char name[15][100];
	char *token, *rest;
    char *read_str;
	int cnum,a,i,j,k;
	int max_cats = CHUNK_SIZE;
	colormap* colormaps = NULL;
	colormap* cmap = NULL;
	int max_colormaps = CHUNK_SIZE;
	int ncolormaps=0;
	int hascat,hascmap;
	float crit;

	OMobj_id categories_id;
	xp_long  categories_size = 0, categories_count;
	FILE *fp;
	/***********************/
	/*  Get input values   */
	/***********************/
	/* Get cbFile's value */
	if (OMget_name_str_val(CB_ColorMapCore_id, OMstr_to_name("cbFile"), &cbFile, 0) != 1){
		fprintf(stderr,"CB_ColorMapCore: Error, could not get filename value.\n");
		return(0);
	};
	if ((fp = fopen(cbFile, "r")) == NULL) {
		fprintf(stderr,"CB_ColorMapCore: Error, could not open Colourmap file.\n");
		if (cbFile != NULL) free(cbFile);
		return(0);
	};

	/***********************/
	/* Function's Body     */
	/***********************/
		
	colormaps = (colormap*) malloc(max_colormaps*sizeof(colormap));
	if (!colormaps){
		fprintf(stderr,"no memory for Colourmaps.\n");
		if (cbFile != NULL) free(cbFile);
		if (fp != NULL) fclose(fp);
		return(0);
	}
	cat_col = (int*) malloc(max_cats*sizeof(int));
	cat_col_nmap = (int*) malloc(max_cats*sizeof(int));
	cat_col_names = (int**) malloc(max_cats*sizeof(int*));
	cat_col_ncmaps = (int**) malloc(max_cats*sizeof(int*));
	cat_col_cmapcol = (int***)malloc(max_cats*sizeof(int**));
	
	cat_col_num_nmap = (int*) malloc(max_cats*sizeof(int));
	cat_col_num_names = (int**) malloc(max_cats*sizeof(int*));
	cat_col_num_ncmaps = (int**) malloc(max_cats*sizeof(int*));
	cat_col_num_cmapcol = (int***)malloc(max_cats*sizeof(int**));
	
	read_str = fgets(buffer,512,fp);
	while (read_str = fgets(buffer,512,fp)) { 
		read_str[strlen(read_str)-1]='\0';
		a=0;
		rest=read_str;
		token=read_str;
		do{
			strcpy(type,"");
			rest=strchr(rest,',');
			if (rest){
				if (rest-token){
					strncpy(type,token+1,rest-token);
					type[rest-token-1]='\0';
					strcpy(name[a],drop_quot(type));
				} else {
					strcpy(name[a],"");
				}
				token = rest++;	
			} else {
				strcpy(name[a],drop_quot(token+1));
			}
			a++;					
		}
		while (rest);
		if (strcmp(name[0],"")){
			cmap = colormaps+ncolormaps;
			cmap->cmap_name = (char*)malloc((strlen(name[0])+1)*sizeof(char));
			strcpy(cmap->cmap_name,name[0]);
			cmap->ncolors = atoi(name[1]);
			cmap->color_number = (int*)malloc(cmap->ncolors * sizeof(int));
			cmap->color_letter = (char*)malloc(cmap->ncolors * sizeof(char));
			cmap->colors = (int*)malloc(3 * cmap->ncolors * sizeof(int));
			cnum = 0;
			cmap->cat_name = (char*)malloc((strlen(name[2])+1)*sizeof(char));
			strcpy (cmap->cat_name,name[2]);
			hascat=0;
			for (i = 0; i<ncats;i++){
				if (!strcmp(colormaps[cat_col[i]].cat_name,cmap->cat_name)){
					hascat=1;
					break;
				}
			}
			if (!hascat){
				cat_col[ncats]=ncolormaps;
				cat_col_nmap[ncats]=0;
				cat_col_cmapcol[ncats] = (int**)malloc(max_cats*sizeof(int*));;
				cat_col_names[ncats] = (int*)malloc(max_cats*sizeof(int));
				cat_col_ncmaps[ncats] = (int*) malloc(max_cats*sizeof(int));
				cat_col_num_nmap[ncats]=0;
				cat_col_num_cmapcol[ncats] = (int**)malloc(max_cats*sizeof(int*));;
				cat_col_num_names[ncats] = (int*)malloc(max_cats*sizeof(int));
				cat_col_num_ncmaps[ncats] = (int*) malloc(max_cats*sizeof(int));
				ncats++;
			}
			for (i = 0; i<ncats;i++){
				if (!strcmp(colormaps[cat_col[i]].cat_name,cmap->cat_name)){
					hascmap=0;
					for (j=0;j<cat_col_nmap[i];j++){
						if (!strcmp(colormaps[cat_col_names[i][j]].cmap_name,cmap->cmap_name)){
							hascmap=1;
							break;
						}
					}
					if (!hascmap){
						cat_col_ncmaps[i][j]=0;
						cat_col_cmapcol[i][cat_col_nmap[i]] = (int*)malloc(max_cats*sizeof(int));
						cat_col_names[i][cat_col_nmap[i]] = ncolormaps;
						cat_col_nmap[i]++;
					}
					hascmap=0;
					for (j=0;j<cat_col_num_nmap[i];j++){
						if (colormaps[cat_col_num_names[i][j]].ncolors == cmap->ncolors){
							hascmap=1;
							break;
						}
					}
					if (!hascmap){
						cat_col_num_ncmaps[i][j]=0;
						cat_col_num_cmapcol[i][cat_col_num_nmap[i]] = (int*)malloc(max_cats*sizeof(int));
						cat_col_num_names[i][cat_col_num_nmap[i]] = ncolormaps;
						cat_col_num_nmap[i]++;
					}
					for (j=0;j<cat_col_num_nmap[i];j++){
						if (colormaps[cat_col_num_names[i][j]].ncolors == cmap->ncolors){
							cat_col_num_cmapcol[i][j][cat_col_num_ncmaps[i][j]] = ncolormaps;
							cat_col_num_ncmaps[i][j]++;
						}
					}
					for (j=0;j<cat_col_nmap[i];j++){
						if (!strcmp(colormaps[cat_col_names[i][j]].cmap_name,cmap->cmap_name)){
							cat_col_cmapcol[i][j][cat_col_ncmaps[i][j]] = ncolormaps;
							cat_col_ncmaps[i][j]++;
						}
					}
				}
				
			}
			cmap->crit_val= atof(name[3]);
			ncolormaps++;
			if (ncolormaps==max_colormaps){
				max_colormaps += CHUNK_SIZE;
				colormaps = (colormap*) realloc(colormaps, max_colormaps * sizeof(colormap));
				if (!colormaps){
					fprintf(stderr,"no memory for Colourmaps.\n");
					if (cbFile != NULL) free(cbFile);
					if (fp != NULL) fclose(fp);
					return(0);
				}
				cmap = colormaps+(ncolormaps-1);
			}
		
		}
		if (cnum < cmap->ncolors){
			cmap->color_number[cnum]= atoi(name[4]);
			cmap->color_letter[cnum] = name[5][0];
			cmap->colors[3*cnum] = atoi(name[6]);
			cmap->colors[3*cnum+1] = atoi(name[7]);
			cmap->colors[3*cnum+2] = atoi(name[8]);
			cnum++;
		}
	}

	cb_colormaps = (CB_colormap*) malloc (sizeof(CB_colormap));
	cb_colormaps->ncolormaps = ncolormaps;
	cb_colormaps->colormaps = (colormap*) realloc(colormaps, ncolormaps *sizeof(colormap));;
	cb_colormaps->ncats = ncats;
	cat_col = (int*) realloc(cat_col, ncats*sizeof(int));
	cat_col_nmap = (int*) realloc(cat_col_nmap, ncats*sizeof(int));
	cat_col_num_nmap = (int*) realloc(cat_col_num_nmap,ncats*sizeof(int));

	cat_col_names = (int**) realloc(cat_col_names, ncats*sizeof(int*));
	cat_col_ncmaps = (int**) realloc(cat_col_ncmaps, ncats*sizeof(int*));
	cat_col_num_names = (int**) realloc(cat_col_num_names, ncats*sizeof(int*));
	cat_col_num_ncmaps = (int**) realloc(cat_col_num_ncmaps, ncats*sizeof(int*));

	cat_col_cmapcol = (int***) realloc(cat_col_cmapcol, ncats*sizeof(int**));
	cat_col_num_cmapcol = (int***) realloc(cat_col_num_cmapcol,ncats*sizeof(int**));

	for (i=0;i<ncats;i++){
		cat_col_names[i] = (int*) realloc(cat_col_names[i], cat_col_nmap[i]*sizeof(int));
		cat_col_ncmaps[i] = (int*) realloc(cat_col_ncmaps[i], cat_col_nmap[i]*sizeof(int));
		cat_col_num_names[i] = (int*) realloc(cat_col_num_names[i], cat_col_num_nmap[i]*sizeof(int));
		cat_col_num_ncmaps[i] = (int*) realloc(cat_col_num_ncmaps[i], cat_col_num_nmap[i]*sizeof(int));

		cat_col_cmapcol[i] = (int**) realloc(cat_col_cmapcol[i], cat_col_nmap[i]*sizeof(int*));
		cat_col_num_cmapcol[i] = (int**) realloc(cat_col_num_cmapcol[i], cat_col_num_nmap[i]*sizeof(int*));
		for (j=0;j<cat_col_nmap[i];j++){
			cat_col_cmapcol[i][j] = (int*) realloc(cat_col_cmapcol[i][j], cat_col_ncmaps[i][j]*sizeof(int));
		}
		for (j=0;j<cat_col_num_nmap[i];j++){
			cat_col_num_cmapcol[i][j] = (int*) realloc(cat_col_num_cmapcol[i][j], cat_col_num_ncmaps[i][j]*sizeof(int));
		}
	}
	
	cb_colormaps->cat_col = cat_col;
	cb_colormaps->cat_col_nmap = cat_col_nmap;
	cb_colormaps->cat_col_num_nmap = cat_col_num_nmap;

	cb_colormaps->cat_col_names = cat_col_names;
	cb_colormaps->cat_col_ncmaps = cat_col_ncmaps;
	cb_colormaps->cat_col_num_names = cat_col_num_names;
	cb_colormaps->cat_col_num_ncmaps = cat_col_num_ncmaps;

	cb_colormaps->cat_col_cmapcol = cat_col_cmapcol;
	cb_colormaps->cat_col_num_cmapcol = cat_col_num_cmapcol;

	
	/***********************/
	/*  Set output values  */
	/***********************/
	/* Set ncats's value */  
	OMset_name_int_val(CB_ColorMapCore_id, OMstr_to_name("ncats"), ncats);
	/* Set categories array */ 
	categories_id = OMfind_subobj(CB_ColorMapCore_id, OMstr_to_name("categories"), OM_OBJ_RW);
	for (i = 0; i < ncats;  i++) {
		OMset_str_array_val(categories_id, i, cb_colormaps->colormaps[cat_col[i]].cat_name);
	}

	OMset_name_ptr_val(CB_ColorMapCore_id,OMstr_to_name("cmapptr"), (void *)cb_colormaps, 0); 

	/*************************/
	/*  Free input variables */
	/*************************/
	if (cbFile)
		free(cbFile);

	return(1);
}

int
CB_ColorMapCore(OMobj_id CB_ColorMapCore_id, OMevent_mask event_mask, int seq_num)
{
	/***********************/
	/*  Declare variables  */
	/***********************/
	int  catidx;
	int  mapidx;
	int  colidx;
	int  max_mapidx;
	int  ncmap, i;
    char buffer[512];
	char  *selector = NULL;
	CB_colormap * cb_colormaps = NULL;
	OMobj_id catmaps_id;
	OMobj_id colors_id, colormaps_id, cmap_id;
	int ncatmaps=0;
	int* avs_col;
	int size;

	/***********************/
	/*  Get input values   */
	/***********************/
	if ( OMget_name_ptr_val(CB_ColorMapCore_id,OMstr_to_name("cmapptr"), (void **)&cb_colormaps,0)!= 1){ 
		cb_colormaps = NULL;
		return(0);
	}
	
	/* Get selector's value */
	if (OMget_name_str_val(CB_ColorMapCore_id, OMstr_to_name("selector"), &selector, 0) != 1){
		selector = NULL;
		return (0);
	}
	
	/* Get catidx's value */ 
	if (OMget_name_int_val(CB_ColorMapCore_id, OMstr_to_name("catidx"), &catidx) != 1) {
		catidx = 0;
			
	}
	if (catidx<0){
		catidx = 0;
		OMset_name_int_val(CB_ColorMapCore_id, OMstr_to_name("catidx"), catidx);
	}
	if (catidx >= cb_colormaps->ncats){
		catidx = cb_colormaps->ncats - 1;
		OMset_name_int_val(CB_ColorMapCore_id, OMstr_to_name("catidx"), catidx);
	}

		
	/* Get mapidx's value */ 
	if (OMget_name_int_val(CB_ColorMapCore_id, OMstr_to_name("mapidx"), &mapidx) != 1) 
		mapidx = 0;

	if (mapidx < 0){
		mapidx = 0;
		OMset_name_int_val(CB_ColorMapCore_id, OMstr_to_name("mapidx"), mapidx);
	}

	/* Get colidx's value */ 
	if (OMget_name_int_val(CB_ColorMapCore_id, OMstr_to_name("colidx"), &colidx) != 1) 
		colidx = 0;
		
	/***********************/
	/* Function's Body     */
	/***********************/

	/***********************/
	/*  Set output values  */
	/***********************/
	if (strcmp(selector,"Name")){
		ncatmaps = cb_colormaps->cat_col_num_nmap[catidx];
		if (mapidx >= ncatmaps){
			mapidx = ncatmaps - 1;
			OMset_name_int_val(CB_ColorMapCore_id, OMstr_to_name("mapidx"), mapidx);
		}

		OMset_name_int_val(CB_ColorMapCore_id, OMstr_to_name("ncatmaps"), ncatmaps);
		/* Set categories array */ 
		catmaps_id = OMfind_subobj(CB_ColorMapCore_id, OMstr_to_name("catmaps"), OM_OBJ_RW);
		for (i = 0; i < ncatmaps;  i++) {
			sprintf(buffer,"%i",cb_colormaps->colormaps[cb_colormaps->cat_col_num_names[catidx][i]].ncolors);
			OMset_str_array_val(catmaps_id, i, buffer);
		}

		/* Set ncmap's value */  
		ncmap = cb_colormaps->cat_col_num_ncmaps[catidx][mapidx];
		OMset_name_int_val(CB_ColorMapCore_id, OMstr_to_name("ncmap"), ncmap);
		colormaps_id = OMfind_subobj(CB_ColorMapCore_id, OMstr_to_name("colormaps"),OM_OBJ_RW);
		for (i=0;i<ncmap;i++){
			OMget_array_val(colormaps_id, i, &cmap_id, OM_OBJ_RW);
			OMset_name_int_val(cmap_id, OMstr_to_name("ncolors"),cb_colormaps->colormaps[cb_colormaps->cat_col_num_cmapcol[catidx][mapidx][i]].ncolors);
			OMset_name_str_val(cmap_id, OMstr_to_name("category"),cb_colormaps->colormaps[cb_colormaps->cat_col_num_cmapcol[catidx][mapidx][i]].cat_name);
			OMset_name_str_val(cmap_id, OMstr_to_name("name"),cb_colormaps->colormaps[cb_colormaps->cat_col_num_cmapcol[catidx][mapidx][i]].cmap_name);
			colors_id = OMfind_subobj(cmap_id, OMstr_to_name("colors"),OM_OBJ_RW);
			avs_col =(int*) OMret_typed_array_ptr(colors_id, OM_GET_ARRAY_WR,OM_TYPE_INT,&size);
			memcpy(avs_col,cb_colormaps->colormaps[cb_colormaps->cat_col_num_cmapcol[catidx][mapidx][i]].colors,size*sizeof(int));
			if (avs_col){
				ARRfree(avs_col);
			}
		}
	} else {
		ncatmaps = cb_colormaps->cat_col_nmap[catidx];
		if (mapidx >= ncatmaps){
			mapidx = ncatmaps - 1;
			OMset_name_int_val(CB_ColorMapCore_id, OMstr_to_name("mapidx"), mapidx);
		}
		OMset_name_int_val(CB_ColorMapCore_id, OMstr_to_name("ncatmaps"), ncatmaps);
		/* Set categories array */ 
		catmaps_id = OMfind_subobj(CB_ColorMapCore_id, OMstr_to_name("catmaps"), OM_OBJ_RW);
		for (i = 0; i < ncatmaps;  i++) {
			OMset_str_array_val(catmaps_id, i, cb_colormaps->colormaps[cb_colormaps->cat_col_names[catidx][i]].cmap_name);
		}
		ncmap = cb_colormaps->cat_col_ncmaps[catidx][mapidx];
		/* Set ncmap's value */  
		OMset_name_int_val(CB_ColorMapCore_id, OMstr_to_name("ncmap"), ncmap);
		
		colormaps_id = OMfind_subobj(CB_ColorMapCore_id, OMstr_to_name("colormaps"),OM_OBJ_RW);
		for (i=0;i<ncmap;i++){
			OMget_array_val(colormaps_id, i, &cmap_id, OM_OBJ_RW);
			OMset_name_int_val(cmap_id, OMstr_to_name("ncolors"),cb_colormaps->colormaps[cb_colormaps->cat_col_cmapcol[catidx][mapidx][i]].ncolors);
			OMset_name_str_val(cmap_id, OMstr_to_name("category"),cb_colormaps->colormaps[cb_colormaps->cat_col_cmapcol[catidx][mapidx][i]].cat_name);
			OMset_name_str_val(cmap_id, OMstr_to_name("name"),cb_colormaps->colormaps[cb_colormaps->cat_col_cmapcol[catidx][mapidx][i]].cmap_name);
			colors_id = OMfind_subobj(cmap_id, OMstr_to_name("colors"),OM_OBJ_RW);
			avs_col =(int*) OMret_typed_array_ptr(colors_id, OM_GET_ARRAY_WR,OM_TYPE_INT,&size);
			memcpy(avs_col,cb_colormaps->colormaps[cb_colormaps->cat_col_cmapcol[catidx][mapidx][i]].colors,size*sizeof(int));
			if (avs_col){
				ARRfree(avs_col);
			}
		}
	}
	return(1);
}


int
CB_ColorMap_Destroy(OMobj_id CB_ColorMapCore_id, OMevent_mask event_mask, int seq_num)
{
	CB_colormap * cb_colormaps = NULL;
	int i,j;
	
	/***********************/
	/*  Get input values   */
	/***********************/
	if ( OMget_name_ptr_val(CB_ColorMapCore_id, OMstr_to_name("cmapptr"), (void **)&cb_colormaps,0)!= 1){ 
		cb_colormaps = NULL;
		return(0);
	}
	for (i=0;i<cb_colormaps->ncats;i++) {
		for (j=0;j<cb_colormaps->cat_col_nmap[i];j++){
			if (cb_colormaps->cat_col_cmapcol[i][j]){
				free(cb_colormaps->cat_col_cmapcol[i][j]);
			}				
		}
		if (cb_colormaps->cat_col_cmapcol[i]){
			free (cb_colormaps->cat_col_cmapcol[i]);
		}
		if (cb_colormaps->cat_col_names[i]){
			free (cb_colormaps->cat_col_names[i]); 
		}
		if (cb_colormaps->cat_col_ncmaps[i]){
			free (cb_colormaps->cat_col_ncmaps[i]);
		}
		for (j=0;j<cb_colormaps->cat_col_num_nmap[i];j++){
			if (cb_colormaps->cat_col_num_cmapcol[i][j]){
				free(cb_colormaps->cat_col_num_cmapcol[i][j]);
			}				
		}
		if (cb_colormaps->cat_col_num_cmapcol[i]){
			free (cb_colormaps->cat_col_num_cmapcol[i]);
		}
		if (cb_colormaps->cat_col_num_names[i]){
			free (cb_colormaps->cat_col_num_names[i]); 
		}
		if (cb_colormaps->cat_col_num_ncmaps[i]){
			free (cb_colormaps->cat_col_num_ncmaps[i]);
		}
	}
	
	if (cb_colormaps->cat_col) {
		free(cb_colormaps->cat_col);
	}
	if (cb_colormaps->cat_col_nmap) {
		free(cb_colormaps->cat_col_nmap);
	}
	if (cb_colormaps->cat_col_names) {
		free(cb_colormaps->cat_col_names);
	}
	if (cb_colormaps->cat_col_ncmaps) {
		free(cb_colormaps->cat_col_ncmaps);
	}
	if (cb_colormaps->cat_col_cmapcol) {
		free(cb_colormaps->cat_col_cmapcol);
	}
	if (cb_colormaps->cat_col_num_nmap) {
		free(cb_colormaps->cat_col_num_nmap);
	}
	if (cb_colormaps->cat_col_num_names) {
		free(cb_colormaps->cat_col_num_names);
	}
	if (cb_colormaps->cat_col_num_ncmaps) {
		free(cb_colormaps->cat_col_num_ncmaps);
	}
	if (cb_colormaps->cat_col_num_cmapcol) {
		free(cb_colormaps->cat_col_num_cmapcol);
	}
	for (i=0;i<cb_colormaps->ncolormaps;i++){
		free(cb_colormaps->colormaps[i].cmap_name);
		free(cb_colormaps->colormaps[i].cat_name);
		free(cb_colormaps->colormaps[i].color_number);
		free(cb_colormaps->colormaps[i].color_letter);
		free(cb_colormaps->colormaps[i].colors);
	}
	if (cb_colormaps->colormaps){
		free(cb_colormaps->colormaps);
	}
	
	free(cb_colormaps);

	return(1);
}
