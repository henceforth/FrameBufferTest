#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <aio.h>
#include <unistd.h>

#define BUFSIZE 5
#define DEVICE_TO_OPEN "/dev/psaux"

#define LBUTTON 1
#define RBUTTON 2
#define MBUTTON 4

struct mouseMove{
	int offsetX;
	int offsetY;
	int buttonPressed;
};


int openMouse(void);
struct mouseMove* pollMouse(void);
void closeMouse(void);

