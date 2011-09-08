#include "framebuffer.h"
#include "mouse.h"

#include <signal.h>


#define XMAX 1024
#define YMAX 768

int main(void){
	
	openFramebuffer();
	openMouse();

	signal(SIGINT, closeFramebuffer);
	signal(SIGTERM, closeFramebuffer);
	struct mouseMove* mmove;

	int red = 0;
	int green = 0;
	int blue = 255;

	int j = 0;
	int i = 0;

	for(j = 0; j< XMAX; j++){
		for(i = 0; i < YMAX; i++){
			setPixel(j, i, red, green, blue);
		}
	}

	int x = XMAX/2;
	int y = YMAX/2;
	red = 0;
	green = 0;
	blue = 0;
	setTimer();
	while(1==1){
		tick();
		swapBuffers();
		setTimer();
		
		int i = 0;
		int j = 0;
		for(i = 0; i < 10; i++)
			for(j = 0; j < 10; j++){
				
				setPixel(x+i,y+j,red,green,blue);
			}
		
		//mouse handling
		mmove = pollMouse();
		if(mmove == NULL){
			usleep(10);
			continue;
		}

		if((mmove->buttonPressed & RBUTTON))
			break;

		if((mmove->buttonPressed & LBUTTON)){
			blue -= 50;
			if(blue <= 0)
				blue = 255;
		}

		if(mmove->offsetX != 0 || mmove->offsetY != 0){
			//inverted axis
			x += mmove->offsetX;
			y -= mmove->offsetY;
			if(x >= XMAX)
				x = XMAX;
			if(x < 0)
				x = 0;
			if(y >= YMAX)
				y = YMAX;
			if(y < 0)
				y = 0;
		}
		free(mmove);
	}
	printDebug();
	closeFramebuffer();
	closeMouse();
	return 0;
}
