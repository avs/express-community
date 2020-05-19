/*
 * rnum.h
 */
#ifndef _avsReadNum_h_
#define _avsReadNum_h_
#define IS_DIGIT 0
#define IS_MINUS 1
#define IS_SPACE 2
#define IS_EXPONENT 3
#define IS_PLUS 4
#define IS_POINT 5
#define IS_OTHER -1
int processChar(char c, int *i);
int intFromString(char *string, int field_start, int field_end, int *theNumber);
int shortIntFromString(char *string, int field_start, int field_end, short int *theNumber);
int longIntFromString(char *string, int field_start, int field_end, long int *theNumber);
int floatFromString(char *string, int field_start, int field_end, float *theNumber);
int doubleFromString(char *string, int field_start, int field_end, double *theNumber);
#endif

