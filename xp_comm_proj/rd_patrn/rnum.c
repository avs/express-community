#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "rnum.h"

#define LOG_OF_10 2.302585

#define ERR_RET return 0;

/* input a character "c".  determine whether it is a digit, */
/* a space, a plus, a minus, a point (full stop), an        */
/* exponent (D,d,E or e) or something else.  return a value */
/* to indicate what it is (see avsReadNum.h).  If it is a   */
/* digit, return the integer value of the digit in "i".     */
int processChar(c, i)
char c;
int *i;
{
	int ic;
	int theThing;
	ic = (int) (c);
	theThing = IS_OTHER;
	if( isdigit( ic ) ) {
		if( c == '0')      *i = 0;
		else if( c == '1') *i = 1;
		else if( c == '2') *i = 2;
		else if( c == '3') *i = 3;
		else if( c == '4') *i = 4;
		else if( c == '5') *i = 5;
		else if( c == '6') *i = 6;
		else if( c == '7') *i = 7;
		else if( c == '8') *i = 8;
		else 	           *i = 9;
		theThing = IS_DIGIT;
	}
	else if( isspace( ic ) ) {
		theThing = IS_SPACE;
	}
	else if( ispunct( ic ) ) {
		if( c == '-' ) theThing = IS_MINUS;
		else if( c == '+' ) theThing = IS_PLUS;
		else if( c == '.' ) theThing = IS_POINT;
	}
	else if( isalpha( ic ) ) {
		if( c == 'E' ) theThing = IS_EXPONENT;
		else if( c == 'e' ) theThing = IS_EXPONENT;
		else if( c == 'D' ) theThing = IS_EXPONENT;
		else if( c == 'd' ) theThing = IS_EXPONENT;
	}
	return theThing;
}

/* take <string> and attempt to read a float into <theNumber> */
/* from a field within <string> starting at <field_start>     */
/* and finishing at <field_end>.  Return 0=fail, 1=success.   */
int doubleFromString(string,field_start, field_end, theNumber)
char *string;
int field_start;
int field_end;
double *theNumber;
{
	/* divide the field into three regions to be read:		*/
	/* 	1. preceeding the decimal point				*/
	/*	2. fractional part of the number			*/
	/*	3. exponent						*/
	/* the three fields will be separated by a "." and by one of    */
	/* "E", "e", "D" or "d".					*/
	/* there must be no white spaces within the number		*/
	int num_beg, num_end;
	int fra_beg, fra_end;
	int exp_beg, exp_end;

	/* flags for status of traversal of number field:	*/
	/* wait		...	no number encountered		*/
	/* in_num	...	reading whole number digits	*/
	/* in_fra	...	reading fraction digits		*/
	/* in_exp	...	reading exponent digits		*/
	/* at_dot	...	sitting on the decimal point	*/
	/* at_E		...	sitting on the exponent letter	*/
	/* at_num_sign	...	sign for the number		*/
	/* at_exp_sign	...	sign for the exponent		*/
	int wait, in_num, in_fra, in_exp, at_dot, at_E, at_num_sign, at_exp_sign;

	long int number, fraction, exponent, num_sign, exp_sign;
	int bit;
	int i;
	int dummy;
	int precision;
	char *c;

	c= &(string[field_start]);
	wait = 1;
	in_num= in_fra= in_exp= at_dot= at_E= at_num_sign= at_exp_sign=0;
	num_beg = num_end = fra_beg = fra_end = exp_beg = exp_end = -1;
	num_sign = 1;
	exp_sign = 1;
	number = 0;
	fraction = 0;
	exponent = 0;
	for(i=field_start; i<=field_end; i++) {
		bit = processChar( *c, &dummy ); 
/*
printf("parse: $%c$ = %d out of _ %d - %d + %d # %d, . %d e %d\n",
*c,bit,IS_SPACE,IS_MINUS,IS_PLUS,IS_DIGIT,IS_POINT,IS_EXPONENT);
*/
		c++;
		if( wait ) {
			if( bit == IS_SPACE ) {
			}
			else if( bit == IS_MINUS ) {
				at_num_sign = 1;
				wait = 0;
				num_sign = -1;
			}
			else if( bit == IS_PLUS ) {
				at_num_sign = 1;
				wait = 0;
				num_sign = 1;
			}
			else if( bit == IS_DIGIT ) {
				in_num = 1;
				wait = 0;
				num_beg = i;
				num_end = i;
			}
			else if( bit == IS_POINT ) {
				at_dot = 1;
				wait = 0;
				number = 0;
			}
			else {
				ERR_RET;
			}
		}
		else if( at_num_sign ) {
			if( bit == IS_DIGIT ) {
				at_num_sign = 0;
				in_num = 1;
				num_beg = i;
				num_end = i;
			}
			else if( bit == IS_POINT ) {
				at_num_sign = 0;
				at_dot = 1;
				number = 0;
			}
			else {
				ERR_RET;
			}
		}
		else if( in_num ) {
			if( bit == IS_DIGIT ) {
				num_end = i;
			}
			else if( bit == IS_POINT ) {
				in_num = 0;
				at_dot = 1;
			}
			else {
				ERR_RET;
			}
		}
		else if( at_dot ) {
			if( bit == IS_DIGIT ) {
				at_dot = 0;
				in_fra = 1;
				fra_beg = i;
				fra_end = i;
			}
			else if( bit == IS_SPACE ) {
				at_dot = 0;
				fraction = 0;
				exponent = 0;
			}
			else if( bit == IS_EXPONENT ) {
				at_dot = 0;
				at_E = 1;
				fraction = 0;
			}
			else {
				ERR_RET;
			}
		}
		else if( in_fra ) {
			if( bit == IS_DIGIT ) {
				fra_end = i;
			}
			else if( bit == IS_SPACE ) {
				in_fra = 0;
				exponent = 0;
			}
			else if( bit == IS_EXPONENT ) {
				in_fra = 0;
				at_E = 1;
				fraction = 0;
			}
			else {
				ERR_RET;
			}
		}
		else if( at_E ) {
			if( bit == IS_MINUS ) {
				at_E = 0;
				at_exp_sign = 1;
				exp_sign = -1;
			}
			else if( bit == IS_PLUS ) {
				at_E = 0;
				at_exp_sign = 1;
				exp_sign = 1;
			}
			else if( bit == IS_DIGIT ) {
				at_E = 0;
				exp_beg = i;
				exp_end = i;
			}
			else {
				ERR_RET;
			}
		}
		else if( at_exp_sign ) {
			if( bit == IS_DIGIT ) {
				at_exp_sign = 0;
				in_exp = 1;
				exp_beg = i;
				exp_end = i;
			}
			else {
				ERR_RET;
			}
		}
		else if( in_exp ) {
			if( bit == IS_DIGIT ) {
				exp_end = i;
			}
			else if( bit == IS_SPACE ) {
				in_exp = 0;
			}
			else {
				ERR_RET;
			}
		}
		else { /* done */
			if( bit != IS_SPACE ) ERR_RET;
		}
/*
printf("w %d   +/- %d   # %d   . %d   # %d   e %d   +/- %d   # %d\n",
wait,at_num_sign,in_num,at_dot,in_fra,at_E,at_exp_sign,in_exp);
*/
	}
	/* check that at least one of the whole/fractional */
	/* fields has a non-zero width - otherwise there   */
	/* is no number!				   */
	if( num_beg<0  &&  fra_beg<0 ) ERR_RET;
	if(num_beg >= 0) {
		longIntFromString(string,num_beg,num_end,&number);
		*theNumber = (float) number;
	}
	if(fra_beg >= 0) {
		longIntFromString(string,fra_beg,fra_end,&fraction);
		precision = 1;
		for(i=fra_beg;i<=fra_end;i++) precision = precision*10;
		*theNumber = *theNumber+((float)fraction)/((float)(precision));
	}
	if(exp_beg >= 0) {
		longIntFromString(string,exp_beg,exp_end,&exponent);
		*theNumber = (float) exp( log((double) *theNumber) + 
					  exp_sign*exponent*LOG_OF_10);
	}
	*theNumber = *theNumber*num_sign;
	
/*
printf("%s,%s,%d\n","$$"," number sign ",num_sign);
printf("%s,%s,%d\n","  "," number ",number);
printf("%s,%s,%d\n","  "," fraction ",fraction);
printf("%s,%s,%d\n","  "," exponent sign ",exp_sign);
printf("%s,%s,%d\n","  "," exponent ",exponent);
printf("    field num %d %d \n",num_beg,num_end);
printf("    field fra %d %d \n",fra_beg,fra_end);
printf("    field exp %d %d \n",exp_beg,exp_end);
*/
	return 1;
}

/* take <string> and attempt to read a float into <theNumber> */
/* from a field within <string> starting at <field_start>     */
/* and finishing at <field_end>.  Return 0=fail, 1=success.   */
int floatFromString(string,field_start, field_end, theNumber)
char *string;
int field_start;
int field_end;
float *theNumber;
{
	/* divide the field into three regions to be read:		*/
	/* 	1. preceeding the decimal point				*/
	/*	2. fractional part of the number			*/
	/*	3. exponent						*/
	/* the three fields will be separated by a "." and by one of    */
	/* "E", "e", "D" or "d".					*/
	/* there must be no white spaces within the number		*/
	int num_beg, num_end;
	int fra_beg, fra_end;
	int exp_beg, exp_end;

	/* flags for status of traversal of number field:	*/
	/* wait		...	no number encountered		*/
	/* in_num	...	reading whole number digits	*/
	/* in_fra	...	reading fraction digits		*/
	/* in_exp	...	reading exponent digits		*/
	/* at_dot	...	sitting on the decimal point	*/
	/* at_E		...	sitting on the exponent letter	*/
	/* at_num_sign	...	sign for the number		*/
	/* at_exp_sign	...	sign for the exponent		*/
	int wait, in_num, in_fra, in_exp, at_dot, at_E, at_num_sign, at_exp_sign;

	long int number, fraction, exponent, num_sign, exp_sign;
	int bit;
	int i;
	int dummy;
	int precision;
	char *c;

	c= &(string[field_start]);
	wait = 1;
	in_num= in_fra= in_exp= at_dot= at_E= at_num_sign= at_exp_sign=0;
	num_beg = num_end = fra_beg = fra_end = exp_beg = exp_end = -1;
	num_sign = 1;
	exp_sign = 1;
	number = 0;
	fraction = 0;
	exponent = 0;
	for(i=field_start; i<=field_end; i++) {
		bit = processChar( *c, &dummy ); 
/*
printf("parse: $%c$ = %d out of _ %d - %d + %d # %d, . %d e %d\n",
*c,bit,IS_SPACE,IS_MINUS,IS_PLUS,IS_DIGIT,IS_POINT,IS_EXPONENT);
*/
		c++;
		if( wait ) {
			if( bit == IS_SPACE ) {
			}
			else if( bit == IS_MINUS ) {
				at_num_sign = 1;
				wait = 0;
				num_sign = -1;
			}
			else if( bit == IS_PLUS ) {
				at_num_sign = 1;
				wait = 0;
				num_sign = 1;
			}
			else if( bit == IS_DIGIT ) {
				in_num = 1;
				wait = 0;
				num_beg = i;
				num_end = i;
			}
			else if( bit == IS_POINT ) {
				at_dot = 1;
				wait = 0;
				number = 0;
			}
			else {
				ERR_RET;
			}
		}
		else if( at_num_sign ) {
			if( bit == IS_DIGIT ) {
				at_num_sign = 0;
				in_num = 1;
				num_beg = i;
				num_end = i;
			}
			else if( bit == IS_POINT ) {
				at_num_sign = 0;
				at_dot = 1;
				number = 0;
			}
			else {
				ERR_RET;
			}
		}
		else if( in_num ) {
			if( bit == IS_DIGIT ) {
				num_end = i;
			}
			else if( bit == IS_POINT ) {
				in_num = 0;
				at_dot = 1;
			}
			else {
				ERR_RET;
			}
		}
		else if( at_dot ) {
			if( bit == IS_DIGIT ) {
				at_dot = 0;
				in_fra = 1;
				fra_beg = i;
				fra_end = i;
			}
			else if( bit == IS_SPACE ) {
				at_dot = 0;
				fraction = 0;
				exponent = 0;
			}
			else if( bit == IS_EXPONENT ) {
				at_dot = 0;
				at_E = 1;
				fraction = 0;
			}
			else {
				ERR_RET;
			}
		}
		else if( in_fra ) {
			if( bit == IS_DIGIT ) {
				fra_end = i;
			}
			else if( bit == IS_SPACE ) {
				in_fra = 0;
				exponent = 0;
			}
			else if( bit == IS_EXPONENT ) {
				in_fra = 0;
				at_E = 1;
				fraction = 0;
			}
			else {
				ERR_RET;
			}
		}
		else if( at_E ) {
			if( bit == IS_MINUS ) {
				at_E = 0;
				at_exp_sign = 1;
				exp_sign = -1;
			}
			else if( bit == IS_PLUS ) {
				at_E = 0;
				at_exp_sign = 1;
				exp_sign = 1;
			}
			else if( bit == IS_DIGIT ) {
				at_E = 0;
				exp_beg = i;
				exp_end = i;
			}
			else {
				ERR_RET;
			}
		}
		else if( at_exp_sign ) {
			if( bit == IS_DIGIT ) {
				at_exp_sign = 0;
				in_exp = 1;
				exp_beg = i;
				exp_end = i;
			}
			else {
				ERR_RET;
			}
		}
		else if( in_exp ) {
			if( bit == IS_DIGIT ) {
				exp_end = i;
			}
			else if( bit == IS_SPACE ) {
				in_exp = 0;
			}
			else {
				ERR_RET;
			}
		}
		else { /* done */
			if( bit != IS_SPACE ) ERR_RET;
		}
/*
printf("w %d   +/- %d   # %d   . %d   # %d   e %d   +/- %d   # %d\n",
wait,at_num_sign,in_num,at_dot,in_fra,at_E,at_exp_sign,in_exp);
*/
	}
	/* check that at least one of the whole/fractional */
	/* fields has a non-zero width - otherwise there   */
	/* is no number!				   */
	if( num_beg<0  &&  fra_beg<0 ) ERR_RET;
	if(num_beg >= 0) {
		longIntFromString(string,num_beg,num_end,&number);
		*theNumber = (float) number;
	}
	if(fra_beg >= 0) {
		longIntFromString(string,fra_beg,fra_end,&fraction);
		precision = 1;
		for(i=fra_beg;i<=fra_end;i++) precision = precision*10;
		*theNumber = *theNumber+((float)fraction)/((float)(precision));
	}
	if(exp_beg >= 0) {
		longIntFromString(string,exp_beg,exp_end,&exponent);
		*theNumber = (float) exp( log((double) *theNumber) + 
					  exp_sign*exponent*LOG_OF_10);
	}
	*theNumber = *theNumber*num_sign;
	
/*
printf("%s,%s,%d\n","$$"," number sign ",num_sign);
printf("%s,%s,%d\n","  "," number ",number);
printf("%s,%s,%d\n","  "," fraction ",fraction);
printf("%s,%s,%d\n","  "," exponent sign ",exp_sign);
printf("%s,%s,%d\n","  "," exponent ",exponent);
printf("    field num %d %d \n",num_beg,num_end);
printf("    field fra %d %d \n",fra_beg,fra_end);
printf("    field exp %d %d \n",exp_beg,exp_end);
*/
	return 1;
}

/**************************************************************/
/* take <string> and attempt to read a int into <theNumber>   */
/* from a field within <string> starting at <field_start>     */
/* and finishing at <field_end>.  Return 0=fail, 1=success.   */
/**************************************************************/
int intFromString(string, field_start, field_end, theNumber)
char *string;
int field_start;
int field_end;
int *theNumber;
{
	char *c;
	int i;
	int power;
	int wait;
	int midway;
	int success;
	int total, subtotal;
	int bit;
	c= &(string[field_end]);
	wait = 1;
	midway = 0;
	success = 1;
	for(i=field_end; i >= field_start; i-- ) {
		bit = processChar( *c, &subtotal );
		if( bit == IS_DIGIT ) {
			if(midway) {
				power = power*10;
				total = total + subtotal * power;
			}
			else if(wait) {
				wait = 0;
				midway = 1;
				power = 1;
				total = subtotal;
			}
			else {
				success = 0;
				return success;
			}
		}
		else if( bit == IS_MINUS ) {
			if(midway) {
				total = -total;
				midway = 0;
			}
			else {
				success = 0;
				return success;
			}
		}
		else if( bit == IS_PLUS ) {
			if(midway) {
				midway = 0;
			}
			else {
				success = 0;
				return success;
			}
		}
		else if(bit == IS_SPACE ) {
			if(midway) {
				midway = 0;
			}
		}
		else {
			success = 0;
			return success;
		}
		c--;
	}
	*theNumber = total;
	return success;
}

/********************************************************************/
/* take <string> and attempt to read a short int into <theNumber>   */
/* from a field within <string> starting at <field_start>           */
/* and finishing at <field_end>.  Return 0=fail, 1=success.         */
/********************************************************************/
int shortIntFromString(string, field_start, field_end, theNumber)
char *string;
int field_start;
int field_end;
short int *theNumber;
{
	char *c;
	int i;
	int power;
	int wait;
	int midway;
	int success;
	int total, subtotal;
	int bit;
	c= &(string[field_end]);
	wait = 1;
	midway = 0;
	success = 1;
	for(i=field_end; i >= field_start; i-- ) {
		bit = processChar( *c, &subtotal );
		if( bit == IS_DIGIT ) {
			if(midway) {
				power = power*10;
				total = total + subtotal * power;
			}
			else if(wait) {
				wait = 0;
				midway = 1;
				power = 1;
				total = subtotal;
			}
			else {
				success = 0;
				return success;
			}
		}
		else if( bit == IS_MINUS ) {
			if(midway) {
				total = -total;
				midway = 0;
			}
			else {
				success = 0;
				return success;
			}
		}
		else if( bit == IS_PLUS ) {
			if(midway) {
				midway = 0;
			}
			else {
				success = 0;
				return success;
			}
		}
		else if(bit == IS_SPACE ) {
			if(midway) {
				midway = 0;
			}
		}
		else {
			success = 0;
			return success;
		}
		c--;
	}
	*theNumber = (short int)total;
	return success;
}

/*******************************************************************/
/* take <string> and attempt to read a long int into <theNumber>   */
/* from a field within <string> starting at <field_start>          */
/* and finishing at <field_end>.  Return 0=fail, 1=success.        */
/*******************************************************************/
int longIntFromString(string, field_start, field_end, theNumber)
char *string;
int field_start;
int field_end;
long int *theNumber;
{
	char *c;
	int i;
	int power;
	int wait;
	int midway;
	int success;
	long int total;
    int subtotal;
	int bit;
	c= &(string[field_end]);
	wait = 1;
	midway = 0;
	success = 1;

	for(i=field_end; i >= field_start; i-- ) {
		bit = processChar( *c, &subtotal );
		if( bit == IS_DIGIT ) {
			if(midway) {
				power = power*10;
				total = total + subtotal * power;
			}
			else if(wait) {
				wait = 0;
				midway = 1;
				power = 1;
				total = subtotal;
			}
			else {
				success = 0;
				return success;
			}
		}
		else if( bit == IS_MINUS ) {
			if(midway) {
				total = -total;
				midway = 0;
			}
			else {
				success = 0;
				return success;
			}
		}
		else if( bit == IS_PLUS ) {
			if(midway) {
				midway = 0;
			}
			else {
				success = 0;
				return success;
			}
		}
		else if(bit == IS_SPACE ) {
			if(midway) {
				midway = 0;
			}
		}
		else {
			success = 0;
			return success;
		}
		c--;
	}
	*theNumber = total;
	return success;
}
