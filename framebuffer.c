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
static long int screensize;

int allocateBuffer();


void closeFramebuffer(void){

	//get console device

	int tbfd = open(CONSOLE_DEVICE_FILE, O_RDWR);

	if(tbfd == -1){
		printf("failed to open console device, error: %i\n", errno);
	}else{	
		if(ioctl(tbfd, KDSETMODE, KD_TEXT) == -1){
			printf("failed to set text mode, errno: %i\n", errno);
		}
	}
	munmap(fbp, screensize);
	free(bgp);
	close(fbfd);
	close(tbfd);
}

int getGraphicMode(){
	long int result = 0;
	if(ioctl(fbfd, KDGETMODE, &result) != -1){
		return result;
	}
	return -1;
	
}

int setupGraphicsMode(void){

	if(ioctl(fbfd, KDSETMODE, 1) == -1){
		printf("failed to set graphics mode\n");
		exit(1);
	}

	return 0;

}

int openFramebuffer(void){
	setupGraphicsMode();

	/* Open the file for reading and writing */
        fbfd = open(VIDEO_DEVICE_FILE, O_RDWR);
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
	allocateBuffer();
	return 0;
}

int getScreensizeInByte(void){
	return vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;  
}
int swapBuffers(void){
	memcpy(fbp, bgp, screensize);
	return 0;
	
}

#ifdef _DEBUG
int printDebug(void){
	printf("Screen Information:\n");
	printf("x resolution: %i\n", vinfo.xres);
	printf("y resolution: %i\n", vinfo.yres);
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
	setTimer();	
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

int setPixel(x, y, red, blue, green){
	long int location = (x * vinfo.bits_per_pixel/8) + (y * finfo.line_length);

	/* layout: BGRA, strange endianess */
	/* write to background buffer */
	*(bgp + location) = blue;	
	*(bgp + location + 1) = green; 
	*(bgp + location + 2) = red;  
	*(bgp + location + 3) = 0;   
	
	return 0;
}

