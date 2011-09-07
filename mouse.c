#include "mouse.h"

static int mfh; //mouse file handle (device file)
static char* buffer; //mouse buffer
static struct aiocb cb; //async struct



int main(void){
	if(openAndAllocateMouse() != 0){
		exit(1);
	}
	pollMouse();
	closeMouse();
	return 0;

}

struct mouseMove* processMouseInput(void){
	//only call when certain, that there is mouse data
	
	struct mouseMove mmove;
	//struct mouseMove* mmove = (struct mouseMove)malloc(sizeof(struct mouseMove));

	mmove.buttonPressed = 0;
	mmove.offsetX = *(buffer+1);
	mmove.offsetY = *(buffer+2);
	int currentByte = *(buffer);
	

	if((currentByte & 1) != 0){
		//left
		printf("LB pressed\n");
		mmove.buttonPressed = mmove.buttonPressed | LBUTTON; 
	}


	if((currentByte & 2) != 0){
		//right
		printf("RB pressed\n");
		mmove.buttonPressed = mmove.buttonPressed | RBUTTON; 

	}

	if((currentByte & 4) != 0){
		//middle
		printf("MB pressed\n");
		mmove.buttonPressed = mmove.buttonPressed | MBUTTON; 
	}

	if((currentByte & 8) != 8){
		//always set	
		printf("ERROR!\n");
	}

	if((currentByte & 16) != 0){
		printf("X sign\n");
		mmove.offsetX = (~(++mmove.offsetX))*-1;
	}

	if((currentByte & 32) != 0){
		printf("Y sign\n");
		mmove.offsetY = (~(++mmove.offsetY))*-1;
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

int pollMouse(){
	int aio_ret = 0;

	while(1==1){
		aio_ret = aio_read(&cb);
	
		do{	
			aio_ret = aio_error(&cb);
			if(aio_ret == 0){ //read succesfull
				break;
			}else if(aio_ret == EINPROGRESS){ //read in progress
			}else{ //error
				printf("read error!\n");
				return 3;
			}
		}while(aio_ret == EINPROGRESS);//statt EINPROGRESS wait asynchrones verhalten

		aio_ret = aio_return(&cb);
		if(aio_ret >= 0){
			struct mouseMove* pmouse = processMouseInput();
		}else{
			printf("aio_return error! returned value: %i\n", aio_ret);
		}
	}
	return 0;
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

