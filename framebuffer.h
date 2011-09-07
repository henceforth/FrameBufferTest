#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>

//public access
int openFramebuffer(void);
int getScreensizeInByte(void);
int setPixel(int x, int y, int red, int blue, int green);
int swapBuffers(void);

//dbg
int printDebug(void);
void tick(void);
void setTimer(void);

#define DEVICE_FILE "/dev/fb0"
