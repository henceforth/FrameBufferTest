#include "cache.h"

/**
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

setCachellall stop:
int setCache(ch
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

int openCache(int ceSize){
	numEntries = 0;
	cacheEntrySize = ceSize;	

	//allocate a full array of cache entries
	storage = (struct cacheEntry*)malloc(sizeof(struct cacheEntry)*MAX_SIZE);

	if(storage == NULL){
		//give up
		status = STATUS_FAIL; 
		return -1;
	}else{
		status = STATUS_OK;
		memset(storage, FREE, sizeof(struct cacheEntry)*MAX_SIZE);
	}
	return 0;
}

int setCache(char* pointer){
	if(status == STATUS_FAIL || status == STATUS_FULL){
		printf("storage full or failed\n");
		return -1;
	}

	if(numEntries == MAX_SIZE){
		printf("storage full!\n");
		status = STATUS_FULL;
		return -1;
	}
	
	char* tmp = (char*)malloc(cacheEntrySize);
	if(tmp == NULL){
		printf("no space to allocate for entry\n");
		return -2;
	}

	struct cacheEntry* centry = storage+numEntries;
	centry->id = numEntries;
	centry->start = tmp;
	centry->length = cacheEntrySize;
	memcpy(centry->start, pointer, cacheEntrySize);
	return centry->id;
}

char* getCache(int id){
	if(status == STATUS_FAIL)
		return NULL;

	int i = 0;
	for(i = 0; i < numEntries; i++){
		if((storage+i)->id == id)
			return (storage+i)->start;
	}
	return NULL;
}

void closeCache(){
	if(status == STATUS_FAIL)
		return;
	
	int i = 0;
	for(i = 0; i < numEntries; i++){
		free((storage+i)->start);
	}
	free(storage);
}
