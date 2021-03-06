#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/kd.h>
#include <errno.h>

/**
openFrambuffer() muss gerufen werden, öffnet handles, alloziiert speicher, etc
setPixel() schreibt Pixel an Screenaddresse x, y in farbe red, green, blue
swapBuffer() malt das Bild
closeFramebuffer() beendet die Session
**/

#define _DEBUG
//public access
int openFramebuffer(void);
int setPixel(int x, int y, int red, int green, int blue) ;
int swapBuffers(void);
void closeFramebuffer();

//misc
int getGraphicMode();
int getScreensizeInByte(void);
int getMaxX();
int getMaxY();
char* getCurrentBuffer();
int setBuffer(char*);

int printDebug(void);
void tick(void);
void setTimer(void);
void printTimes(void);

#ifdef _DEBUG
int picsDrawn;
long int totalMicroseconds;
struct timeval start, end;
#endif

#define CONSOLE_DEVICE_FILE "/dev/console"
#define VIDEO_DEVICE_FILE "/dev/fb0"
