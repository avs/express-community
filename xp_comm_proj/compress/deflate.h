

/*

  Copyright / Disclaimer :

  This software/documentation was produced as part of the INDEX project
  (Intelligent Data Extraction) which is funded under contract ESPRIT EP22745 of 
  the European Community. For further details see
  http://www.man.ac.uk/MVC/research/INDEX/Public/. 

  Copyright (c) June 1998, Manchester Visualisation Centre, UK. All Rights Reserved.

  Permission to use, copy, modify and distribute this software and its documentation
  is hereby granted without fee, provided that the above copyright notice and this
  permission notice appear in all copies of this software/documentation.

  This software/documentation is provided with no warranty, express or implied, 
  including, without limitation, warrant of merchantability or fitness for a 
  particular purpose.

*/


/*
  deflate.h

  Functions to use zlib deflate/inflate.
*/


#ifndef DEFLATE_FN_H

#define DEFLATE_FN_H


#include "zlib.h"


int deflated_array
  (Bytef *array_p);

void deflate_array
  (Bytef *in_array_p, int in_size, Bytef **out_array_pp, int *out_size_p,
   int level);

void inflate_array
  (Bytef *in_array_p, int in_size, Bytef **out_array_pp, int *out_size_p);

void deflate_file
  (FILE *in_file_p, FILE *out_file_p, int level);

void inflate_file
  (FILE *in_file_p, FILE *out_file_p);


#endif
