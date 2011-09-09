/**
#include "cache.h"

buffer cache

struct cacheEntry{
	int id;
	char* start;
	int length; 
}

static const SIZE; 
volatile int status;
#define STAUTS_FULL 2
#define STATUS_OK 1
#define STATUS_FAIL 0

setup:
int setup(void);
bekommt groessenangabe CONST!
allokiert groesse 1x
kein exit on fail, nimmt nur keine Aufträge an

setCache:
int setCache(char*);
bekommt pointer, große
kopiert space@pointer:pointer+größe
gibt identifier zurück (int)
allokiert 2*aktuellenSpeicherCount
aktuellerSpeicherCount += newSpeicherCount

getCache:
char* getCache(int);
bekommt identifier
schaut nach identifier, return Adresse
sonst return NULL

setdown:
void setdown(void);
free(cache)

**/

