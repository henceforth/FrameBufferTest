#include "framebuffer.h"
#include "mouse.h"

int main(void){
	
	openFramebuffer();
	openAndAllocateMouse();

	int i = 0;
	for(i = 0; i < 600; i++){
		setPixel(i, i, 0, 255, 0);
	}
	swapBuffers();
	
	closeFramebuffer();
	closeMouse();
	return 0;
}
