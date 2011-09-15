#include "cache.h"

/**
buffer cache

setup:
int setup(void);
allokiert volles array an cacheEntry Einträgen
kein exit on fail, nimmt nur keine Aufträge an

int setCache(char*, int)
bekommt pointer, größe
kopiert space@pointer:pointer+größe
gibt identifier++ zurück (int)

getCache:
char* getCache(int);
bekommt identifier
schaut nach identifier, return Adresse
sonst return NULL

setdown:
void setdown(void);
free(cache)

**/

int openCache(void){
	numEntries = 0;

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

int setCache(char* pointer, int iLength){
	/**
	checks if storage is full
	allocates memory of preset size
	copies preset size to the buffer
	saves it and returns an identifier
	**/
	if(status == STATUS_FAIL || status == STATUS_FULL || pointer == NULL){
		printf("storage full or failed\n");
		return -1;
	}

	if(numEntries == MAX_SIZE){
		//yeah, paranoia
		printf("storage full!\n");
		status = STATUS_FULL;
		return -1;
	}
	
	char* tmp = (char*)malloc(iLength);
	if(tmp == NULL){
		printf("no space to allocate for entry\n");
		return -2;
	}

	struct cacheEntry* centry = storage+numEntries;
	centry->id = numEntries++;
	centry->start = tmp;
	centry->length = iLength;
	memcpy(centry->start, pointer, iLength);
	return centry->id;
}

char* getCache(int id){
	if(status == STATUS_FAIL || id >= numEntries || id < 0)
		return NULL;

	return (char*)(storage+id)->start;	
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
