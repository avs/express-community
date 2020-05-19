#include "GridClasses.h"
#include "ConnectivityClasses.h"

//includes e define per la costruzione del database


#define DCOMMENT 0
//geometria-connettivita'
#define DGRID 1
#define DCTRIAX6 2
#define DCTETRA 4
#define DCHEXA 5
#define DCPENTA 6
#define DCORD2R 7
#define DCORD2C 8
//materiali
#define DPSOLID 24

#define DENDDATA -2
#define DOTHER   -1

//funzione che, data una linea, restituisce il tipo della scheda in essa contenuta
int DetectCardType(char* line);
