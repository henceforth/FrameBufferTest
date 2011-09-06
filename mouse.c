#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>
#include <aio.h>
#include <sys/mman.h>
#define BUFSIZE 16


int main(void){
	printf("started!\n");

	int mfh = open("/dev/psaux", O_RDONLY);
	if(mfh == 0){
		printf("couldnt open /dev/psaux\n");
		return 1;
	}
/**
	FILE* fd = fdopen(mfh, "r");
	fseek(fd, 0, SEEK_END);
	int count = ftell(fd);
	rewind(fd);
	printf("count: %i\n", count);
**/
	char* buffer = (char*)malloc(BUFSIZE);
	if(buffer == NULL){
		printf("couldnt allocate buffer\n");
		return 2;
	}
	memset(buffer, 0, BUFSIZE);
/**
	if(mmap(buffer, BUFSIZE, PROT_READ, MAP_SHARED, mfh, 0) == MAP_FAILED){
		printf("mmap failed, errno %i!\n", errno);
	}else{
		printf("mmap succesful!\n");
	}
**/
/**
	int a = 0;
	int i = 0;
	while(1==1){
		a = read(mfh, buffer, BUFSIZE);
		for(i = 0; i < a; i++){
			printf("%X ", buffer+i);
		}
		printf("\n");
	}
**/
/**	
	int get = 0;
	while(1 == 1){
	get = fgetc(fd);
	printf("%i", get);
	}
**/
	/**
	int readret = fread(buffer, sizeof(char), BUFSIZE/sizeof(char), fd);
	printf("fread returned %i\n", readret);
	if(readret >= 0){
		printf("%s\n", buffer);
	}
	**/

	//int rbytes = 0;
	
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
				sleep(1);
			}else{ //error
				printf("read error!\n");
				return 3;
			}
		}while(aio_ret = EINPROGRESS);

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
//	fclose(fd);
	close(mfh);
	free(buffer);

	return 0;

}
