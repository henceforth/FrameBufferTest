#include "framebuffer.h"
#include "mouse.h"
#include <signal.h>

void openAll();
void closeAll();

int main(void){
	signal(SIGINT, closeAll);
	signal(SIGTERM, closeAll);
	signal(SIGSEGV, closeAll); //at least graphics mode needs to be switched off

	openAll();
	const int XMAX = getMaxX();
	const int YMAX = getMaxY();

	struct mouseMove* mmove;
	int red = 0, green = 0, blue = 255;
	int j = 0, i = 0;

	for(j = 0; j< XMAX; j++){
		for(i = 0; i < YMAX; i++){
			setPixel(j, i, red, green, blue);
		}
	}

	int x = XMAX/2, y = YMAX/2;
	red = 0, green = 0, blue = 0;
	setTimer();

	int run = 1;
	while(run){
		tick();
		swapBuffers();
		setTimer();
		
		for(i = -5; i < 5; i++)
			for(j = -5; j < 5; j++){
				setPixel(x+i,y+j,red,green,blue);
			}
		
		//mouse handling
		mmove = pollMouse();
		if(mmove == NULL){
			continue;
		}

		if((mmove->buttonPressed & RBUTTON))
			run = 0;

		if((mmove->buttonPressed & LBUTTON)){
			red -= 10;
			if(red <= 0)
				red = 255;
		}

		if(mmove->offsetX != 0 || mmove->offsetY != 0){
			//inverted axis
			x += mmove->offsetX;
			y -= mmove->offsetY;
			if(x >= XMAX)
				x = XMAX-1;
			if(x < 0)
				x = 0;
			if(y >= YMAX)
				y = YMAX-1;
			if(y < 0)
				y = 0;
		}
		free(mmove);
	}
	printDebug();
	closeAll();
	return 0;
}

void openAll(){
	openFramebuffer();
	openMouse();
}

void closeAll(){
	closeFramebuffer();
	closeMouse();
	exit(0);//or the signal handler will return
}
