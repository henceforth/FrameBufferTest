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

//dbg stuff
static long int screensize = 0;
static int picsDrawn = 0;
static long int totalMicroseconds = 0;
struct timeval start, end;
//dbg end

int allocateBuffer();
int main()
{
        int x = 0, y = 0;
	int green = 0, red = 0, blue = 0;
	
	openFramebuffer();
	allocateBuffer();
	
	//dbg start
	printDebug();
	setTimer();
	//dbg end

	int i = 0;
	for(i = 0; i < vinfo.xres; i++){
		/* Figure out where in memory to put the pixel */
		setPixel(x, y, red, green, blue);	
		x += 1;
		
		if(x == vinfo.xres-1){
			//x reached outter x
			x = 0;
			i = 0;
			y += 1;
		}

		if(y == vinfo.yres-1){
			swapBuffers();

			/* switch if we reach max y */
			if(red == 255)
				break;
			else
				red += 5;

			y = 0;
			x = 0;
			
			//dbg start	
			tick(); //restarts timer
			//dbg end
		}
	}	
	closeFramebuffer();
	printf("total pics: %i, total time: %li average: %f\n", picsDrawn, totalMicroseconds, (float)totalMicroseconds/picsDrawn); 
	return 0;
}

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
	return 0;
}

int getScreensizeInByte(void){
	return vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;  
}
int swapBuffers(void){
	memcpy(fbp, bgp, screensize);
	return 0;
	
}

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

