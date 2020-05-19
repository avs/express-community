// Technische Universit�t M�nchen - Peter.Breitling@in.tum.de
// Please read the file LICENSE for conditions associated with this file.

/*
 * Isosurface structure in 2D and 3D for efficient polygon generation.
 *
 * Generated by IsoFaceGen.pl at Fri Feb  5 21:09:00 1999
 */

static int myIsoPrim2DVertex2Edge[2*4] = {
  0,1, 0,2, 1,3, 2,3
};

static int myIsoPrim2DVertex2Axis[4] = {
  0, 1, 1, 0
};

static float myIsoPrim2DVertex2Offset[2*4] = {
  0,0, 0,0, 1,0, 0,1
};

static int myIsoPrim2D[8*16] = {
  // 0000:
  0, -1,-1,-1,-1, -1,-1,-1,
  // 0001:
  1,  0, 1,-1,-1, -1,-1,-1,
  // 0010:
  1,  0, 2,-1,-1, -1,-1,-1,
  // 0011:
  1,  1, 2,-1,-1, -1,-1,-1,
  // 0100:
  1,  1, 3,-1,-1, -1,-1,-1,
  // 0101:
  1,  0, 3,-1,-1, -1,-1,-1,
  // 0110:
  2,  0, 1, 2, 3, -1,-1,-1,
  // 0111:
  1,  2, 3,-1,-1, -1,-1,-1,
  // 1000:
  1,  2, 3,-1,-1, -1,-1,-1,
  // 1001:
  2,  0, 1, 2, 3, -1,-1,-1,
  // 1010:
  1,  0, 3,-1,-1, -1,-1,-1,
  // 1011:
  1,  1, 3,-1,-1, -1,-1,-1,
  // 1100:
  1,  1, 2,-1,-1, -1,-1,-1,
  // 1101:
  1,  0, 2,-1,-1, -1,-1,-1,
  // 1110:
  1,  0, 1,-1,-1, -1,-1,-1,
  // 1111:
  0, -1,-1,-1,-1, -1,-1,-1,
};

static int myIsoPrim3DVertex2Edge[2*12] = {
  0,1, 0,2, 0,4, 1,3, 1,5, 2,3, 2,6, 3,7, 4,5, 4,6, 5,7, 6,7
};

static int myIsoPrim3DVertex2Axis[12] = {
  0, 1, 2, 1, 2, 0, 2, 2, 0, 1, 1, 0
};

static float myIsoPrim3DVertex2Offset[3*12] = {
  0,0,0, 0,0,0, 0,0,0, 1,0,0, 
  1,0,0, 0,1,0, 0,1,0, 1,1,0,
  0,0,1, 0,0,1, 1,0,1, 0,1,1
};

static int myIsoPrim3D[16*256] = {
  // 00000000:
  0,   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00000001:
  1,  0x2,0x1,0x0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00000010:
  1,  0x0,0x3,0x4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00000011:
  2,  0x3,0x4,0x2,0x2,0x1,0x3, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00000100:
  1,  0x5,0x1,0x6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00000101:
  2,  0x0,0x2,0x6,0x6,0x5,0x0, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00000110:
  2,  0x0,0x3,0x4,0x6,0x5,0x1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00000111:
  3,  0x3,0x4,0x5,0x5,0x4,0x6,0x6,0x4,0x2, -1, -1, -1, -1,-1,-1,
  // 00001000:
  1,  0x3,0x5,0x7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00001001:
  2,  0x3,0x5,0x7,0x2,0x1,0x0, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00001010:
  2,  0x4,0x0,0x5,0x5,0x7,0x4, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00001011:
  3,  0x5,0x7,0x1,0x1,0x7,0x2,0x2,0x7,0x4, -1, -1, -1, -1,-1,-1,
  // 00001100:
  2,  0x7,0x3,0x1,0x1,0x6,0x7, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00001101:
  3,  0x0,0x2,0x3,0x3,0x2,0x7,0x7,0x2,0x6, -1, -1, -1, -1,-1,-1,
  // 00001110:
  3,  0x1,0x6,0x0,0x0,0x6,0x4,0x4,0x6,0x7, -1, -1, -1, -1,-1,-1,
  // 00001111:
  2,  0x4,0x6,0x7,0x4,0x2,0x6, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00010000:
  1,  0x2,0x8,0x9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00010001:
  2,  0x8,0x9,0x1,0x1,0x0,0x8, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00010010:
  2,  0x4,0x0,0x3,0x9,0x2,0x8, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00010011:
  3,  0x8,0x9,0x4,0x4,0x9,0x3,0x3,0x9,0x1, -1, -1, -1, -1,-1,-1,
  // 00010100:
  2,  0x1,0x6,0x5,0x8,0x9,0x2, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00010101:
  3,  0x6,0x5,0x9,0x9,0x5,0x8,0x8,0x5,0x0, -1, -1, -1, -1,-1,-1,
  // 00010110:
  3,  0x4,0x0,0x3,0x1,0x6,0x5,0x8,0x9,0x2, -1, -1, -1, -1,-1,-1,
  // 00010111:
  4,  0x8,0x9,0x6,0x8,0x6,0x5,0x8,0x5,0x4,0x4,0x5,0x3, -1,-1,-1,
  // 00011000:
  2,  0x3,0x5,0x7,0x8,0x9,0x2, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00011001:
  3,  0x9,0x1,0x8,0x1,0x0,0x8,0x7,0x3,0x5, -1, -1, -1, -1,-1,-1,
  // 00011010:
  3,  0x7,0x4,0x5,0x4,0x0,0x5,0x9,0x2,0x8, -1, -1, -1, -1,-1,-1,
  // 00011011:
  4,  0x9,0x1,0x5,0x9,0x5,0x4,0x9,0x4,0x8,0x4,0x5,0x7, -1,-1,-1,
  // 00011100:
  3,  0x3,0x1,0x7,0x1,0x6,0x7,0x8,0x9,0x2, -1, -1, -1, -1,-1,-1,
  // 00011101:
  4,  0x6,0x7,0x3,0x8,0x6,0x3,0x8,0x9,0x6,0x8,0x3,0x0, -1,-1,-1,
  // 00011110:
  4,  0x8,0x9,0x2,0x1,0x6,0x0,0x0,0x6,0x4,0x4,0x6,0x7, -1,-1,-1,
  // 00011111:
  3,  0x8,0x9,0x4,0x9,0x6,0x4,0x6,0x7,0x4, -1, -1, -1, -1,-1,-1,
  // 00100000:
  1,  0x4,0xa,0x8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00100001:
  2,  0x8,0x4,0xa,0x1,0x0,0x2, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00100010:
  2,  0x0,0x3,0xa,0xa,0x8,0x0, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00100011:
  3,  0x2,0x1,0x8,0x8,0x1,0xa,0xa,0x1,0x3, -1, -1, -1, -1,-1,-1,
  // 00100100:
  2,  0x4,0xa,0x8,0x1,0x6,0x5, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00100101:
  3,  0x5,0x0,0x6,0x0,0x2,0x6,0xa,0x8,0x4, -1, -1, -1, -1,-1,-1,
  // 00100110:
  3,  0x8,0x0,0xa,0x0,0x3,0xa,0x6,0x5,0x1, -1, -1, -1, -1,-1,-1,
  // 00100111:
  4,  0x2,0x6,0x5,0xa,0x2,0x5,0xa,0x8,0x2,0xa,0x5,0x3, -1,-1,-1,
  // 00101000:
  2,  0x4,0xa,0x8,0x5,0x7,0x3, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00101001:
  3,  0x7,0x3,0x5,0x0,0x2,0x1,0xa,0x8,0x4, -1, -1, -1, -1,-1,-1,
  // 00101010:
  3,  0xa,0x8,0x7,0x7,0x8,0x5,0x5,0x8,0x0, -1, -1, -1, -1,-1,-1,
  // 00101011:
  4,  0xa,0x8,0x2,0xa,0x2,0x1,0xa,0x1,0x7,0x7,0x1,0x5, -1,-1,-1,
  // 00101100:
  3,  0x6,0x7,0x1,0x7,0x3,0x1,0x8,0x4,0xa, -1, -1, -1, -1,-1,-1,
  // 00101101:
  4,  0xa,0x8,0x4,0x0,0x2,0x3,0x3,0x2,0x7,0x7,0x2,0x6, -1,-1,-1,
  // 00101110:
  4,  0x8,0x0,0x1,0x8,0x1,0x7,0x8,0x7,0xa,0x7,0x1,0x6, -1,-1,-1,
  // 00101111:
  3,  0xa,0x8,0x7,0x8,0x2,0x7,0x2,0x6,0x7, -1, -1, -1, -1,-1,-1,
  // 00110000:
  2,  0x9,0x2,0x4,0x4,0xa,0x9, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00110001:
  3,  0x4,0xa,0x0,0x0,0xa,0x1,0x1,0xa,0x9, -1, -1, -1, -1,-1,-1,
  // 00110010:
  3,  0x0,0x3,0x2,0x2,0x3,0x9,0x9,0x3,0xa, -1, -1, -1, -1,-1,-1,
  // 00110011:
  2,  0x3,0x9,0x1,0x3,0xa,0x9, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 00110100:
  3,  0xa,0x9,0x4,0x9,0x2,0x4,0x5,0x1,0x6, -1, -1, -1, -1,-1,-1,
  // 00110101:
  4,  0x5,0x0,0x4,0x5,0x4,0x9,0x5,0x9,0x6,0x9,0x4,0xa, -1,-1,-1,
  // 00110110:
  4,  0x6,0x5,0x1,0x0,0x3,0x2,0x2,0x3,0x9,0x9,0x3,0xa, -1,-1,-1,
  // 00110111:
  3,  0x6,0x5,0x9,0x5,0x3,0x9,0x3,0xa,0x9, -1, -1, -1, -1,-1,-1,
  // 00111000:
  3,  0x2,0x4,0x9,0x4,0xa,0x9,0x5,0x7,0x3, -1, -1, -1, -1,-1,-1,
  // 00111001:
  4,  0x5,0x7,0x3,0x4,0xa,0x0,0x0,0xa,0x1,0x1,0xa,0x9, -1,-1,-1,
  // 00111010:
  4,  0x0,0x5,0x7,0x9,0x0,0x7,0x9,0x2,0x0,0x9,0x7,0xa, -1,-1,-1,
  // 00111011:
  3,  0x5,0x7,0x1,0x7,0xa,0x1,0xa,0x9,0x1, -1, -1, -1, -1,-1,-1,
  // 00111100:
  4,  0xa,0x9,0x4,0x4,0x9,0x2,0x7,0x3,0x6,0x3,0x1,0x6, -1,-1,-1,
  // 00111101:
  3,  0x9,0x6,0xa,0xa,0x6,0x7,0x0,0x4,0x3, -1, -1, -1, -1,-1,-1,
  // 00111110:
  3,  0x7,0xa,0x6,0x6,0xa,0x9,0x0,0x1,0x2, -1, -1, -1, -1,-1,-1,
  // 00111111:
  2,  0xa,0x6,0x7,0x6,0xa,0x9, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01000000:
  1,  0x6,0x9,0xb, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01000001:
  2,  0x2,0x1,0x0,0xb,0x6,0x9, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01000010:
  2,  0x0,0x3,0x4,0x9,0xb,0x6, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01000011:
  3,  0x4,0x2,0x3,0x2,0x1,0x3,0xb,0x6,0x9, -1, -1, -1, -1,-1,-1,
  // 01000100:
  2,  0x5,0x1,0x9,0x9,0xb,0x5, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01000101:
  3,  0x9,0xb,0x2,0x2,0xb,0x0,0x0,0xb,0x5, -1, -1, -1, -1,-1,-1,
  // 01000110:
  3,  0xb,0x5,0x9,0x5,0x1,0x9,0x4,0x0,0x3, -1, -1, -1, -1,-1,-1,
  // 01000111:
  4,  0xb,0x5,0x3,0xb,0x3,0x2,0xb,0x2,0x9,0x2,0x3,0x4, -1,-1,-1,
  // 01001000:
  2,  0x5,0x7,0x3,0x9,0xb,0x6, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01001001:
  3,  0x2,0x1,0x0,0x5,0x7,0x3,0x9,0xb,0x6, -1, -1, -1, -1,-1,-1,
  // 01001010:
  3,  0x0,0x5,0x4,0x5,0x7,0x4,0x9,0xb,0x6, -1, -1, -1, -1,-1,-1,
  // 01001011:
  4,  0x9,0xb,0x6,0x5,0x7,0x1,0x1,0x7,0x2,0x2,0x7,0x4, -1,-1,-1,
  // 01001100:
  3,  0x7,0x3,0xb,0xb,0x3,0x9,0x9,0x3,0x1, -1, -1, -1, -1,-1,-1,
  // 01001101:
  4,  0x9,0xb,0x7,0x9,0x7,0x3,0x9,0x3,0x2,0x2,0x3,0x0, -1,-1,-1,
  // 01001110:
  4,  0x7,0x4,0x0,0x9,0x7,0x0,0x9,0xb,0x7,0x9,0x0,0x1, -1,-1,-1,
  // 01001111:
  3,  0x9,0xb,0x2,0xb,0x7,0x2,0x7,0x4,0x2, -1, -1, -1, -1,-1,-1,
  // 01010000:
  2,  0xb,0x6,0x2,0x2,0x8,0xb, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01010001:
  3,  0x1,0x0,0x6,0x6,0x0,0xb,0xb,0x0,0x8, -1, -1, -1, -1,-1,-1,
  // 01010010:
  3,  0x6,0x2,0xb,0x2,0x8,0xb,0x3,0x4,0x0, -1, -1, -1, -1,-1,-1,
  // 01010011:
  4,  0x1,0x3,0x4,0xb,0x1,0x4,0xb,0x6,0x1,0xb,0x4,0x8, -1,-1,-1,
  // 01010100:
  3,  0x2,0x8,0x1,0x1,0x8,0x5,0x5,0x8,0xb, -1, -1, -1, -1,-1,-1,
  // 01010101:
  2,  0x0,0xb,0x5,0x0,0x8,0xb, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01010110:
  4,  0x3,0x4,0x0,0x2,0x8,0x1,0x1,0x8,0x5,0x5,0x8,0xb, -1,-1,-1,
  // 01010111:
  3,  0x3,0x4,0x5,0x4,0x8,0x5,0x8,0xb,0x5, -1, -1, -1, -1,-1,-1,
  // 01011000:
  3,  0x8,0xb,0x2,0xb,0x6,0x2,0x3,0x5,0x7, -1, -1, -1, -1,-1,-1,
  // 01011001:
  4,  0x7,0x3,0x5,0x1,0x0,0x6,0x6,0x0,0xb,0xb,0x0,0x8, -1,-1,-1,
  // 01011010:
  4,  0x8,0xb,0x2,0x2,0xb,0x6,0x4,0x0,0x7,0x0,0x5,0x7, -1,-1,-1,
  // 01011011:
  3,  0x4,0x8,0x7,0x7,0x8,0xb,0x1,0x5,0x6, -1, -1, -1, -1,-1,-1,
  // 01011100:
  4,  0x8,0xb,0x7,0x8,0x7,0x1,0x8,0x1,0x2,0x1,0x7,0x3, -1,-1,-1,
  // 01011101:
  3,  0x7,0x3,0xb,0x3,0x0,0xb,0x0,0x8,0xb, -1, -1, -1, -1,-1,-1,
  // 01011110:
  3,  0xb,0x7,0x8,0x8,0x7,0x4,0x1,0x2,0x0, -1, -1, -1, -1,-1,-1,
  // 01011111:
  2,  0x8,0x7,0x4,0x7,0x8,0xb, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01100000:
  2,  0xa,0x8,0x4,0x6,0x9,0xb, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01100001:
  3,  0xa,0x8,0x4,0x2,0x1,0x0,0xb,0x6,0x9, -1, -1, -1, -1,-1,-1,
  // 01100010:
  3,  0x3,0xa,0x0,0xa,0x8,0x0,0x6,0x9,0xb, -1, -1, -1, -1,-1,-1,
  // 01100011:
  4,  0xb,0x6,0x9,0x2,0x1,0x8,0x8,0x1,0xa,0xa,0x1,0x3, -1,-1,-1,
  // 01100100:
  3,  0x1,0x9,0x5,0x9,0xb,0x5,0x4,0xa,0x8, -1, -1, -1, -1,-1,-1,
  // 01100101:
  4,  0x4,0xa,0x8,0x9,0xb,0x2,0x2,0xb,0x0,0x0,0xb,0x5, -1,-1,-1,
  // 01100110:
  4,  0x1,0x9,0x5,0x5,0x9,0xb,0x0,0x3,0x8,0x3,0xa,0x8, -1,-1,-1,
  // 01100111:
  3,  0x5,0x3,0xb,0xb,0x3,0xa,0x2,0x9,0x8, -1, -1, -1, -1,-1,-1,
  // 01101000:
  3,  0x9,0xb,0x6,0x7,0x3,0x5,0x8,0x4,0xa, -1, -1, -1, -1,-1,-1,
  // 01101001:
  4,  0x8,0x4,0xa,0x1,0x0,0x2,0x3,0x5,0x7,0x6,0x9,0xb, -1,-1,-1,
  // 01101010:
  4,  0x6,0x9,0xb,0xa,0x8,0x7,0x7,0x8,0x5,0x5,0x8,0x0, -1,-1,-1,
  // 01101011:
  3,  0x8,0x2,0x9,0x6,0x1,0x5,0xa,0xb,0x7, -1, -1, -1, -1,-1,-1,
  // 01101100:
  4,  0x8,0x4,0xa,0x7,0x3,0xb,0xb,0x3,0x9,0x9,0x3,0x1, -1,-1,-1,
  // 01101101:
  3,  0xb,0x7,0xa,0x4,0x3,0x0,0x9,0x8,0x2, -1, -1, -1, -1,-1,-1,
  // 01101110:
  3,  0x0,0x1,0x8,0x8,0x1,0x9,0x7,0xa,0xb, -1, -1, -1, -1,-1,-1,
  // 01101111:
  2,  0xb,0x7,0xa,0x2,0x9,0x8, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01110000:
  3,  0xb,0x6,0xa,0xa,0x6,0x4,0x4,0x6,0x2, -1, -1, -1, -1,-1,-1,
  // 01110001:
  4,  0x1,0x0,0x4,0x1,0x4,0xa,0x1,0xa,0x6,0x6,0xa,0xb, -1,-1,-1,
  // 01110010:
  4,  0x3,0xa,0xb,0x3,0xb,0x2,0x3,0x2,0x0,0x2,0xb,0x6, -1,-1,-1,
  // 01110011:
  3,  0xb,0x6,0xa,0x6,0x1,0xa,0x1,0x3,0xa, -1, -1, -1, -1,-1,-1,
  // 01110100:
  4,  0x2,0x4,0xa,0x5,0x2,0xa,0x5,0x1,0x2,0x5,0xa,0xb, -1,-1,-1,
  // 01110101:
  3,  0x4,0xa,0x0,0xa,0xb,0x0,0xb,0x5,0x0, -1, -1, -1, -1,-1,-1,
  // 01110110:
  3,  0xa,0xb,0x3,0x3,0xb,0x5,0x2,0x0,0x1, -1, -1, -1, -1,-1,-1,
  // 01110111:
  2,  0x3,0xb,0x5,0xb,0x3,0xa, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01111000:
  4,  0x3,0x5,0x7,0xb,0x6,0xa,0xa,0x6,0x4,0x4,0x6,0x2, -1,-1,-1,
  // 01111001:
  3,  0x0,0x4,0x3,0x7,0xa,0xb,0x1,0x5,0x6, -1, -1, -1, -1,-1,-1,
  // 01111010:
  3,  0x2,0x0,0x6,0x6,0x0,0x5,0xa,0xb,0x7, -1, -1, -1, -1,-1,-1,
  // 01111011:
  2,  0x7,0xa,0xb,0x1,0x5,0x6, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01111100:
  3,  0x1,0x2,0x3,0x3,0x2,0x4,0xb,0x7,0xa, -1, -1, -1, -1,-1,-1,
  // 01111101:
  2,  0x3,0x0,0x4,0xb,0x7,0xa, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01111110:
  2,  0x2,0x0,0x1,0xa,0xb,0x7, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 01111111:
  1,  0x7,0xa,0xb, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10000000:
  1,  0x7,0xb,0xa, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10000001:
  2,  0x2,0x1,0x0,0xa,0x7,0xb, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10000010:
  2,  0x3,0x4,0x0,0xb,0xa,0x7, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10000011:
  3,  0x1,0x3,0x2,0x3,0x4,0x2,0xb,0xa,0x7, -1, -1, -1, -1,-1,-1,
  // 10000100:
  2,  0x7,0xb,0xa,0x1,0x6,0x5, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10000101:
  3,  0x2,0x6,0x0,0x6,0x5,0x0,0xa,0x7,0xb, -1, -1, -1, -1,-1,-1,
  // 10000110:
  3,  0x0,0x3,0x4,0x7,0xb,0xa,0x1,0x6,0x5, -1, -1, -1, -1,-1,-1,
  // 10000111:
  4,  0xb,0xa,0x7,0x3,0x4,0x5,0x5,0x4,0x6,0x6,0x4,0x2, -1,-1,-1,
  // 10001000:
  2,  0x3,0x5,0xb,0xb,0xa,0x3, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10001001:
  3,  0xa,0x3,0xb,0x3,0x5,0xb,0x2,0x1,0x0, -1, -1, -1, -1,-1,-1,
  // 10001010:
  3,  0x4,0x0,0xa,0xa,0x0,0xb,0xb,0x0,0x5, -1, -1, -1, -1,-1,-1,
  // 10001011:
  4,  0x4,0x2,0x1,0xb,0x4,0x1,0xb,0xa,0x4,0xb,0x1,0x5, -1,-1,-1,
  // 10001100:
  3,  0xb,0xa,0x6,0x6,0xa,0x1,0x1,0xa,0x3, -1, -1, -1, -1,-1,-1,
  // 10001101:
  4,  0xa,0x3,0x0,0xa,0x0,0x6,0xa,0x6,0xb,0x6,0x0,0x2, -1,-1,-1,
  // 10001110:
  4,  0xb,0xa,0x4,0xb,0x4,0x0,0xb,0x0,0x6,0x6,0x0,0x1, -1,-1,-1,
  // 10001111:
  3,  0xb,0xa,0x6,0xa,0x4,0x6,0x4,0x2,0x6, -1, -1, -1, -1,-1,-1,
  // 10010000:
  2,  0xb,0xa,0x7,0x2,0x8,0x9, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10010001:
  3,  0x0,0x8,0x1,0x8,0x9,0x1,0x7,0xb,0xa, -1, -1, -1, -1,-1,-1,
  // 10010010:
  3,  0xb,0xa,0x7,0x4,0x0,0x3,0x9,0x2,0x8, -1, -1, -1, -1,-1,-1,
  // 10010011:
  4,  0x7,0xb,0xa,0x8,0x9,0x4,0x4,0x9,0x3,0x3,0x9,0x1, -1,-1,-1,
  // 10010100:
  3,  0x8,0x9,0x2,0x6,0x5,0x1,0xa,0x7,0xb, -1, -1, -1, -1,-1,-1,
  // 10010101:
  4,  0xa,0x7,0xb,0x6,0x5,0x9,0x9,0x5,0x8,0x8,0x5,0x0, -1,-1,-1,
  // 10010110:
  4,  0x4,0x0,0x3,0x9,0x2,0x8,0x1,0x6,0x5,0xb,0xa,0x7, -1,-1,-1,
  // 10010111:
  3,  0x9,0x6,0xb,0x7,0x5,0x3,0x8,0xa,0x4, -1, -1, -1, -1,-1,-1,
  // 10011000:
  3,  0x5,0xb,0x3,0xb,0xa,0x3,0x2,0x8,0x9, -1, -1, -1, -1,-1,-1,
  // 10011001:
  4,  0xa,0x3,0xb,0xb,0x3,0x5,0x8,0x9,0x0,0x9,0x1,0x0, -1,-1,-1,
  // 10011010:
  4,  0x9,0x2,0x8,0x4,0x0,0xa,0xa,0x0,0xb,0xb,0x0,0x5, -1,-1,-1,
  // 10011011:
  3,  0x1,0x5,0x9,0x9,0x5,0xb,0x4,0x8,0xa, -1, -1, -1, -1,-1,-1,
  // 10011100:
  4,  0x2,0x8,0x9,0xb,0xa,0x6,0x6,0xa,0x1,0x1,0xa,0x3, -1,-1,-1,
  // 10011101:
  3,  0x3,0x0,0xa,0xa,0x0,0x8,0x6,0xb,0x9, -1, -1, -1, -1,-1,-1,
  // 10011110:
  3,  0xa,0x4,0x8,0x2,0x0,0x1,0xb,0x9,0x6, -1, -1, -1, -1,-1,-1,
  // 10011111:
  2,  0xa,0x4,0x8,0x6,0xb,0x9, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10100000:
  2,  0x8,0x4,0x7,0x7,0xb,0x8, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10100001:
  3,  0xb,0x8,0x7,0x8,0x4,0x7,0x1,0x0,0x2, -1, -1, -1, -1,-1,-1,
  // 10100010:
  3,  0x7,0xb,0x3,0x3,0xb,0x0,0x0,0xb,0x8, -1, -1, -1, -1,-1,-1,
  // 10100011:
  4,  0x1,0x3,0x7,0x1,0x7,0x8,0x1,0x8,0x2,0x8,0x7,0xb, -1,-1,-1,
  // 10100100:
  3,  0x4,0x7,0x8,0x7,0xb,0x8,0x1,0x6,0x5, -1, -1, -1, -1,-1,-1,
  // 10100101:
  4,  0xb,0x8,0x7,0x7,0x8,0x4,0x6,0x5,0x2,0x5,0x0,0x2, -1,-1,-1,
  // 10100110:
  4,  0x1,0x6,0x5,0x7,0xb,0x3,0x3,0xb,0x0,0x0,0xb,0x8, -1,-1,-1,
  // 10100111:
  3,  0x8,0x2,0xb,0xb,0x2,0x6,0x3,0x7,0x5, -1, -1, -1, -1,-1,-1,
  // 10101000:
  3,  0x3,0x5,0x4,0x4,0x5,0x8,0x8,0x5,0xb, -1, -1, -1, -1,-1,-1,
  // 10101001:
  4,  0x2,0x1,0x0,0x3,0x5,0x4,0x4,0x5,0x8,0x8,0x5,0xb, -1,-1,-1,
  // 10101010:
  2,  0x0,0x5,0xb,0x0,0xb,0x8, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10101011:
  3,  0x2,0x1,0x8,0x1,0x5,0x8,0x5,0xb,0x8, -1, -1, -1, -1,-1,-1,
  // 10101100:
  4,  0x3,0x1,0x6,0x8,0x3,0x6,0x8,0x4,0x3,0x8,0x6,0xb, -1,-1,-1,
  // 10101101:
  3,  0x6,0xb,0x2,0x2,0xb,0x8,0x3,0x0,0x4, -1, -1, -1, -1,-1,-1,
  // 10101110:
  3,  0x1,0x6,0x0,0x6,0xb,0x0,0xb,0x8,0x0, -1, -1, -1, -1,-1,-1,
  // 10101111:
  2,  0xb,0x2,0x6,0x2,0xb,0x8, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10110000:
  3,  0x9,0x2,0xb,0xb,0x2,0x7,0x7,0x2,0x4, -1, -1, -1, -1,-1,-1,
  // 10110001:
  4,  0x4,0x7,0xb,0x1,0x4,0xb,0x1,0x0,0x4,0x1,0xb,0x9, -1,-1,-1,
  // 10110010:
  4,  0x9,0x2,0x0,0x9,0x0,0x3,0x9,0x3,0xb,0xb,0x3,0x7, -1,-1,-1,
  // 10110011:
  3,  0x7,0xb,0x3,0xb,0x9,0x3,0x9,0x1,0x3, -1, -1, -1, -1,-1,-1,
  // 10110100:
  4,  0x5,0x1,0x6,0x9,0x2,0xb,0xb,0x2,0x7,0x7,0x2,0x4, -1,-1,-1,
  // 10110101:
  3,  0x0,0x4,0x5,0x5,0x4,0x7,0x9,0x6,0xb, -1, -1, -1, -1,-1,-1,
  // 10110110:
  3,  0x2,0x0,0x1,0x5,0x3,0x7,0x9,0x6,0xb, -1, -1, -1, -1,-1,-1,
  // 10110111:
  2,  0x5,0x3,0x7,0x9,0x6,0xb, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10111000:
  4,  0x2,0x4,0x3,0x2,0x3,0xb,0x2,0xb,0x9,0xb,0x3,0x5, -1,-1,-1,
  // 10111001:
  3,  0xb,0x9,0x5,0x5,0x9,0x1,0x4,0x3,0x0, -1, -1, -1, -1,-1,-1,
  // 10111010:
  3,  0x9,0x2,0xb,0x2,0x0,0xb,0x0,0x5,0xb, -1, -1, -1, -1,-1,-1,
  // 10111011:
  2,  0x5,0x9,0x1,0x9,0x5,0xb, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10111100:
  3,  0x4,0x3,0x2,0x2,0x3,0x1,0xb,0x9,0x6, -1, -1, -1, -1,-1,-1,
  // 10111101:
  2,  0x0,0x4,0x3,0x9,0x6,0xb, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10111110:
  2,  0x2,0x0,0x1,0xb,0x9,0x6, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 10111111:
  1,  0x6,0xb,0x9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11000000:
  2,  0xa,0x7,0x6,0x6,0x9,0xa, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11000001:
  3,  0x7,0x6,0xa,0x6,0x9,0xa,0x0,0x2,0x1, -1, -1, -1, -1,-1,-1,
  // 11000010:
  3,  0x9,0xa,0x6,0xa,0x7,0x6,0x0,0x3,0x4, -1, -1, -1, -1,-1,-1,
  // 11000011:
  4,  0x9,0xa,0x6,0x6,0xa,0x7,0x2,0x1,0x4,0x1,0x3,0x4, -1,-1,-1,
  // 11000100:
  3,  0x5,0x1,0x7,0x7,0x1,0xa,0xa,0x1,0x9, -1, -1, -1, -1,-1,-1,
  // 11000101:
  4,  0x9,0xa,0x7,0x0,0x9,0x7,0x0,0x2,0x9,0x0,0x7,0x5, -1,-1,-1,
  // 11000110:
  4,  0x4,0x0,0x3,0x5,0x1,0x7,0x7,0x1,0xa,0xa,0x1,0x9, -1,-1,-1,
  // 11000111:
  3,  0x2,0x9,0x4,0x4,0x9,0xa,0x5,0x3,0x7, -1, -1, -1, -1,-1,-1,
  // 11001000:
  3,  0x6,0x9,0x5,0x5,0x9,0x3,0x3,0x9,0xa, -1, -1, -1, -1,-1,-1,
  // 11001001:
  4,  0x0,0x2,0x1,0x6,0x9,0x5,0x5,0x9,0x3,0x3,0x9,0xa, -1,-1,-1,
  // 11001010:
  4,  0x0,0x5,0x6,0x0,0x6,0xa,0x0,0xa,0x4,0xa,0x6,0x9, -1,-1,-1,
  // 11001011:
  3,  0xa,0x4,0x9,0x9,0x4,0x2,0x5,0x6,0x1, -1, -1, -1, -1,-1,-1,
  // 11001100:
  2,  0x3,0x1,0x9,0x3,0x9,0xa, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11001101:
  3,  0x0,0x2,0x3,0x2,0x9,0x3,0x9,0xa,0x3, -1, -1, -1, -1,-1,-1,
  // 11001110:
  3,  0x4,0x0,0xa,0x0,0x1,0xa,0x1,0x9,0xa, -1, -1, -1, -1,-1,-1,
  // 11001111:
  2,  0x9,0x4,0x2,0x4,0x9,0xa, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11010000:
  3,  0xa,0x7,0x8,0x8,0x7,0x2,0x2,0x7,0x6, -1, -1, -1, -1,-1,-1,
  // 11010001:
  4,  0x0,0x8,0xa,0x0,0xa,0x6,0x0,0x6,0x1,0x6,0xa,0x7, -1,-1,-1,
  // 11010010:
  4,  0x0,0x3,0x4,0xa,0x7,0x8,0x8,0x7,0x2,0x2,0x7,0x6, -1,-1,-1,
  // 11010011:
  3,  0x6,0x1,0x7,0x7,0x1,0x3,0x8,0xa,0x4, -1, -1, -1, -1,-1,-1,
  // 11010100:
  4,  0xa,0x7,0x5,0xa,0x5,0x1,0xa,0x1,0x8,0x8,0x1,0x2, -1,-1,-1,
  // 11010101:
  3,  0xa,0x7,0x8,0x7,0x5,0x8,0x5,0x0,0x8, -1, -1, -1, -1,-1,-1,
  // 11010110:
  3,  0x7,0x5,0x3,0x0,0x1,0x2,0xa,0x4,0x8, -1, -1, -1, -1,-1,-1,
  // 11010111:
  2,  0x4,0x8,0xa,0x5,0x3,0x7, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11011000:
  4,  0xa,0x3,0x5,0x2,0xa,0x5,0x2,0x8,0xa,0x2,0x5,0x6, -1,-1,-1,
  // 11011001:
  3,  0x8,0xa,0x0,0x0,0xa,0x3,0x6,0x1,0x5, -1, -1, -1, -1,-1,-1,
  // 11011010:
  3,  0x5,0x6,0x0,0x0,0x6,0x2,0xa,0x4,0x8, -1, -1, -1, -1,-1,-1,
  // 11011011:
  2,  0x4,0x8,0xa,0x1,0x5,0x6, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11011100:
  3,  0x2,0x8,0x1,0x8,0xa,0x1,0xa,0x3,0x1, -1, -1, -1, -1,-1,-1,
  // 11011101:
  2,  0x0,0xa,0x3,0xa,0x0,0x8, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11011110:
  2,  0x8,0xa,0x4,0x1,0x2,0x0, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11011111:
  1,  0x4,0x8,0xa, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11100000:
  3,  0x8,0x4,0x9,0x9,0x4,0x6,0x6,0x4,0x7, -1, -1, -1, -1,-1,-1,
  // 11100001:
  4,  0x1,0x0,0x2,0x8,0x4,0x9,0x9,0x4,0x6,0x6,0x4,0x7, -1,-1,-1,
  // 11100010:
  4,  0x8,0x0,0x3,0x6,0x8,0x3,0x6,0x9,0x8,0x6,0x3,0x7, -1,-1,-1,
  // 11100011:
  3,  0x3,0x7,0x1,0x1,0x7,0x6,0x8,0x2,0x9, -1, -1, -1, -1,-1,-1,
  // 11100100:
  4,  0x4,0x7,0x5,0x4,0x5,0x9,0x4,0x9,0x8,0x9,0x5,0x1, -1,-1,-1,
  // 11100101:
  3,  0x7,0x5,0x4,0x4,0x5,0x0,0x9,0x8,0x2, -1, -1, -1, -1,-1,-1,
  // 11100110:
  3,  0x9,0x8,0x1,0x1,0x8,0x0,0x7,0x5,0x3, -1, -1, -1, -1,-1,-1,
  // 11100111:
  2,  0x3,0x7,0x5,0x8,0x2,0x9, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11101000:
  4,  0x8,0x4,0x3,0x8,0x3,0x5,0x8,0x5,0x9,0x9,0x5,0x6, -1,-1,-1,
  // 11101001:
  3,  0x4,0x3,0x0,0x1,0x5,0x6,0x8,0x2,0x9, -1, -1, -1, -1,-1,-1,
  // 11101010:
  3,  0x6,0x9,0x5,0x9,0x8,0x5,0x8,0x0,0x5, -1, -1, -1, -1,-1,-1,
  // 11101011:
  2,  0x1,0x5,0x6,0x8,0x2,0x9, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11101100:
  3,  0x8,0x4,0x9,0x4,0x3,0x9,0x3,0x1,0x9, -1, -1, -1, -1,-1,-1,
  // 11101101:
  2,  0x4,0x3,0x0,0x9,0x8,0x2, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11101110:
  2,  0x8,0x1,0x9,0x1,0x8,0x0, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11101111:
  1,  0x2,0x9,0x8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11110000:
  2,  0x4,0x7,0x6,0x4,0x6,0x2, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11110001:
  3,  0x1,0x0,0x6,0x0,0x4,0x6,0x4,0x7,0x6, -1, -1, -1, -1,-1,-1,
  // 11110010:
  3,  0x0,0x3,0x2,0x3,0x7,0x2,0x7,0x6,0x2, -1, -1, -1, -1,-1,-1,
  // 11110011:
  2,  0x7,0x1,0x3,0x1,0x7,0x6, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11110100:
  3,  0x5,0x1,0x7,0x1,0x2,0x7,0x2,0x4,0x7, -1, -1, -1, -1,-1,-1,
  // 11110101:
  2,  0x4,0x5,0x0,0x5,0x4,0x7, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11110110:
  2,  0x3,0x7,0x5,0x2,0x0,0x1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11110111:
  1,  0x3,0x7,0x5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11111000:
  3,  0x3,0x5,0x4,0x5,0x6,0x4,0x6,0x2,0x4, -1, -1, -1, -1,-1,-1,
  // 11111001:
  2,  0x0,0x4,0x3,0x6,0x1,0x5, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11111010:
  2,  0x0,0x6,0x2,0x6,0x0,0x5, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11111011:
  1,  0x5,0x6,0x1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11111100:
  2,  0x3,0x2,0x4,0x2,0x3,0x1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11111101:
  1,  0x0,0x4,0x3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11111110:
  1,  0x2,0x0,0x1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
  // 11111111:
  0,   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,
};