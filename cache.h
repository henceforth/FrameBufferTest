/**
cache.h basic cache interface
**/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int openCache(void);
int setCache(char*, int);
char* getCache(int);
void closeCache(void);

#define MAX_SIZE 100
#define FREE -1
#define STATUS_FAIL 0
#define STATUS_FULL 2
#define STATUS_OK 1

struct cacheEntry{
	int id;
	char* start;
	int length; 
};

struct cacheEntry* storage;
int numEntries;
volatile int status;
int cacheEntrySize;
