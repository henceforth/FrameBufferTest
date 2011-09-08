#include "mouse.h"

static int mfh; //mouse file handle (device file)
static char* buffer; //mouse buffer
static struct aiocb cb; //async struct

struct mouseMove* processMouseInput(void);

struct mouseMove* processMouseInput(void){
	//only call when certain there is mouse data in the buffer

	if(buffer == NULL)
		return NULL;

	struct mouseMove mmove;
	mmove.buttonPressed = 0;
	mmove.offsetX = *(buffer+1);
	mmove.offsetY = *(buffer+2);
	int currentByte = *(buffer);
	

	if((currentByte & 1) != 0){
		//left
		mmove.buttonPressed = mmove.buttonPressed | LBUTTON; 
	}


	if((currentByte & 2) != 0){
		//right
		mmove.buttonPressed = mmove.buttonPressed | RBUTTON; 

	}

	if((currentByte & 4) != 0){
		//middle
		mmove.buttonPressed = mmove.buttonPressed | MBUTTON; 
	}

	if((currentByte & 8) != 8){
		//always set	
		printf("ERROR!\n");
	}

	if((currentByte & 16) != 0){
		//mmove.offsetX = (~(++mmove.offsetX))*-1;
		mmove.offsetX += 1;
		mmove.offsetX = ~mmove.offsetX;
		mmove.offsetX *= -1;
	}

	if((currentByte & 32) != 0){
		mmove.offsetY += 1;
		mmove.offsetY = ~mmove.offsetY;
		mmove.offsetY *= -1;
	}
	
	
	/**
	//ignore mouse overflow
	if((currentByte & 64) != 0){
		//ignored
		printf("X overflow\n");
	}

	if((currentByte & 128) != 0){
		//ignored
		printf("Y overflow\n");
	}**/


	//MUST BE FREED!!!!!
	struct mouseMove* pmmove = NULL;
	pmmove = (struct mouseMove*)malloc(sizeof(struct mouseMove));
	if(pmmove != NULL){
		memcpy(pmmove, &mmove, sizeof(struct mouseMove));
		return pmmove;
	}

	return NULL;

}

struct mouseMove* pollMouse(){
	int aio_ret = 0;
	struct mouseMove* pMouse = NULL;
	aio_ret = aio_read(&cb);

	do{	
		aio_ret = aio_error(&cb);
		if(aio_ret == 0){ //read succesfull
			break;
		}else if(aio_ret == EINPROGRESS){ //read in progress
		}else{ //error
			printf("read error!\n");
			return NULL;
		}
	}while(aio_ret == EINPROGRESS);

	aio_ret = aio_return(&cb);
	if(aio_ret >= 0){
		pMouse = processMouseInput();
	}else{
		printf("aio_return error! returned value: %i\n", aio_ret);
		return NULL;
	}
	return pMouse;
}

int openAndAllocateMouse(void){
	mfh = 0;
	buffer = NULL;
	
	if(BUFSIZE <= 0){
		printf("invalid buffer size!\n");
		return 4;
	}
	
	mfh = open(DEVICE_TO_OPEN, O_RDONLY);
	if(mfh == 0){
		printf("couldnt open %s\n", DEVICE_TO_OPEN);
		return 1;
	}

	buffer = (char*)malloc(BUFSIZE);
	if(buffer == NULL){
		printf("couldnt allocate buffer\n");
		return 2;
	}

	memset(buffer, 0, BUFSIZE);
	memset(&cb, 0, sizeof(struct aiocb));

	cb.aio_fildes = mfh;
	cb.aio_offset = 0;
	cb.aio_buf = buffer;
	cb.aio_nbytes = BUFSIZE;
	return 0;
}
void closeMouse(void){
	close(mfh);
	free(buffer);
}

