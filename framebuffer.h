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
//public access
int openFramebuffer(void);
int getScreensizeInByte(void);
int setPixel(int x, int y, int red, int blue, int green);
int swapBuffers(void);
void closeFramebuffer();
int getGraphicMode();

//dbg
int printDebug(void);
void tick(void);
void setTimer(void);

#define CONSOLE_DEVICE_FILE "/dev/console"
#define VIDEO_DEVICE_FILE "/dev/fb0"
