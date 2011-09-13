#include "framebuffer.h"
#include "mouse.h"
#include "cache.h"
#include "shapes.h"
#include <signal.h>

void openAll();
void closeAll();

int drawRectangle(int x, int y, int h, int w, int red, int green, int blue){
//simple rectangle	
	int j = 0;
	int i = 0;

	struct shapeRect r1;
	r1.x = x;
	r1.y = y;
	r1.h = h;
	r1.w = w;
	r1.red = red;
	r1.green = green;
	r1.blue = blue;

	for(j = 0; j< h; j++){
		for(i = 0; i < w; i++){
			setPixel(x+j, y+i, red, green, blue);
		}
	}
	return 0;
}

int main(void){
	signal(SIGINT, closeAll);
	signal(SIGTERM, closeAll);
	signal(SIGSEGV, closeAll); //at least graphics mode needs to be switched off

	openAll();
	const int XMAX = getMaxX();
	const int YMAX = getMaxY();

	struct mouseMove* mmove;
	int red = 0, green = 0, blue = 255;
	
	drawRectangle(0, 0, XMAX, YMAX, red, green, blue);

	int bid = -1;
	//store background image (w/o cursor)
	bid = setCache(getCurrentBuffer());

	int x = XMAX/2, y = YMAX/2;
	red = 255, green = 255, blue = 255;
	int run = 1;

	while(run){
		//setTimer();
		swapBuffers();
		//tick();

		//mouse handling
		mmove = pollMouse();
		if(mmove == NULL){
			continue;
		}
		
		//set background
		setBuffer(getCache(bid));

		//cursor	
		drawRectangle(x, y, 10, 10, red, green, blue);
		if(mmove->buttonPressed & RBUTTON)
			run = 0;

		if(mmove->buttonPressed & LBUTTON){
			red = 255-red;
			blue = 255-blue;
			green = 255-green;
		}


		if(mmove->offsetX != 0 || mmove->offsetY != 0){
			//inverted axis
			x += mmove->offsetX*2;
			y -= mmove->offsetY*2;
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
#ifdef _DEBUG
	printDebug();
#endif
	closeAll();
	return 0;
}

void openAll(){
	openFramebuffer();
	openMouse();
	openCache(getScreensizeInByte());
}

void closeAll(){
	closeFramebuffer();
	closeMouse();
	closeCache();
	exit(0);//or the signal handler will return
}
