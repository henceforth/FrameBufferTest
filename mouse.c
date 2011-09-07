#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <aio.h>
#include <unistd.h>

#define BUFSIZE 5
#define DEVICE_TO_OPEN "/dev/psaux"

int main(void){
	if(BUFSIZE <= 0){
		printf("invalid buffer size!\n");
		return 4;
	}
	
	int mfh = open(DEVICE_TO_OPEN, O_RDONLY);
	if(mfh == 0){
		printf("couldnt open %s\n", DEVICE_TO_OPEN);
		return 1;
	}

	char* buffer = (char*)malloc(BUFSIZE);
	if(buffer == NULL){
		printf("couldnt allocate buffer\n");
		return 2;
	}
	memset(buffer, 0, BUFSIZE);
	
	struct aiocb cb;
	memset(&cb, 0, sizeof(struct aiocb));

	cb.aio_fildes = mfh;
	cb.aio_offset = 0;
	cb.aio_buf = buffer;
	cb.aio_nbytes = BUFSIZE;

	int aio_ret = 0;

	while(1==1){
		aio_ret = aio_read(&cb);
		do{	
			aio_ret = aio_error(&cb);
			if(aio_ret == 0){ //read succesfull
				printf("\n");
				break;
			}else if(aio_ret == EINPROGRESS){ //read in progress
			}else{ //error
				printf("read error!\n");
				return 3;
			}
		}while(aio_ret == EINPROGRESS);//statt EINPROGRESS wait asynchrones verhalten

		aio_ret = aio_return(&cb);
		if(aio_ret >= 0){
			int i = 0;
			for(i = 0; i < aio_ret; i++){
				printf("%8X ", *(buffer+i));
			}
		}else{
			printf("return returned %i\n", aio_ret);
		}
	}
	close(mfh);
	free(buffer);

	return 0;

}
