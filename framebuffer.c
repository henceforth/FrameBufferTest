#include "framebuffer.h"


//variable screen info
static struct fb_var_screeninfo vinfo;
//fixed screen info
static struct fb_fix_screeninfo finfo;
///dev/fb0 file hadnle
static int fbfd;
//mapped area of /dev/fb0
static char *fbp = 0;
//mapped background buffer
static char* bgp = 0;
//screen size in byte
static long int screensize;
//flag to indicate need to update the framebuffer
int needToRefresh;
//setup function
int allocateBuffer();


char* getCurrentBuffer(){
	//for caching
	return bgp;
}

int setBuffer(char* pointer){
	//for caching
	if(pointer == NULL)
		return -1;

	memcpy(bgp, pointer, screensize);
	needToRefresh = 1;
	return 0;
}

void closeFramebuffer(void){
	//set down function

	//get console device
	int tbfd = open(CONSOLE_DEVICE_FILE, O_RDWR);
	if(tbfd == -1){
		printf("failed to open console device, error: %i\n", errno);
	}else{	
		if(ioctl(tbfd, KDSETMODE, KD_TEXT) == -1){
			printf("failed to set text mode, errno: %i\n", errno);
		}
	}
	
	if(fbp != NULL)
		munmap(fbp, screensize);
	if(bgp != NULL)
		free(bgp);
	if(fbfd != 0)
		close(fbfd);
	if(tbfd != 0)
		close(tbfd);
}

int getGraphicMode(){
	//get current display mode (text/graphic)
	long int result = 0;
	if(ioctl(fbfd, KDGETMODE, &result) != -1){
		return result;
	}
	return -1;
	
}

int setupGraphicMode(void){
	/**
	switches to graphics mode
	**/

	if(ioctl(fbfd, KDSETMODE, 1) == -1){
		printf("failed to set graphics mode\n");
		exit(1);
	}

	return 0;

}

int openFramebuffer(void){
	/**
	switches to graphics moed
	opens framebuffer handle
	gets information
	allocates buffers and maps framebuffer
	**/

#ifndef _DEBUG
	setupGraphicMode();
#endif

	/* Open the file for reading and writing */
        fbfd = open(VIDEO_DEVICE_FILE, O_RDWR | O_APPEND);
        if (!fbfd) {
                printf("Error: cannot open framebuffer device.\n");
                exit(1);
        }

        /* Get fixed screen information */
        if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
                printf("Error reading fixed information.\n");
                exit(2);
        }

        /* Get variable screen information */
        if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
                printf("Error reading variable information.\n");
                exit(3);
        }

        screensize = getScreensizeInByte(); 
	needToRefresh = 0;
	allocateBuffer();
	return 0;
}

int getMaxX(){
	return vinfo.xres;
}

int getMaxY(){
	return vinfo.yres;
}

int getScreensizeInByte(void){
	return vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;  
}

int swapBuffers(void){
	//memcpy(fbp, bgp, screensize);
	if(needToRefresh){
#ifdef _DEBUG
		setTimer();
#endif
		write(fbfd, bgp, screensize);
#ifdef _DEBUG
		tick();
#endif
	}
	needToRefresh = 0;
	lseek(fbfd, 0, SEEK_SET);
	return 0;
}

#ifdef _DEBUG
int printDebug(void){
	printf("Screen Information:\n");
	printf("x resolution: %i\n", vinfo.xres);
	printf("y resolution: %i\n", vinfo.yres);
	printf("total ticks: %i, total time: %li average: %f, ticks/second: %li\n", picsDrawn, totalMicroseconds, (float)totalMicroseconds/picsDrawn, (totalMicroseconds>>6)/picsDrawn); 
	return 0;
}

void setTimer(void){
	gettimeofday(&start, NULL);
}

void tick(void){
	gettimeofday(&end, NULL);
	picsDrawn++;
	if(end.tv_usec - start.tv_usec > 0){
		totalMicroseconds += end.tv_usec - start.tv_usec;
	}else{
		totalMicroseconds += (end.tv_sec - start.tv_sec + ((end.tv_usec - start.tv_usec)>>6));
	}
}

void printTimes(void){
	printf("last tick took %li seconds and %li microseconds\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
}
#endif

int allocateBuffer(){
        /* Map the device to memory */
        fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);  
        
	if ((int)fbp == -1) { 
		printf("Error: failed to map framebuffer device to memory.\n"); 
		exit(4);
        }
	
	/* Allocate background buffer */
	bgp = (char*)malloc(screensize);

	if(bgp == NULL){
		printf("failed to allocate background buffer, requested size: %li byte\n", screensize);
		exit(5);
	}
	return 0;
}
long int location;
int setPixel(x, y, red, green, blue){
	//checks expensive, but prevent segfaults :-/
	if(x < 0 || x >= vinfo.xres || y < 0 || y >= vinfo.yres) 
		return -1;

	location = (x * vinfo.bits_per_pixel/8) + (y * finfo.line_length);
	needToRefresh = 1;
/**	
	*(bgp + location) = blue;	
	*(bgp + location + 1) = green; 
	*(bgp + location + 2) = red;  
	*(bgp + location + 3) = 0;   
**/	

	char tmp[4];
	tmp[0] = blue;
	tmp[1] = green;
	tmp[2] = red;
	tmp[3] = 0;
	memcpy(bgp+location, tmp, sizeof(tmp));
	return 0;
}

