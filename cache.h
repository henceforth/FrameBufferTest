/**
cache.h basic cache interface

**/
int openCache(void);
int setCache(char*);
int getCache(i);
int closeCache(void);


struct cacheEntry{
	int id;
	char* start;
	int length; 
}
#define FREE -1

#define STAUTS_FULL 2
#define STATUS_OK 1
#define STATUS_FAIL 0
volatile int status;
