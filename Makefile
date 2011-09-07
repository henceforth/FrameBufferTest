FBOFILE = framebuffer
FBIFILE = framebuffer.c

MFBIFILE = mouse.c
MFBOFILE = mouse

GLOBAL_LINKS = 
FLAGS = -Wall -g

#clean and compile all
all: framebuffer mouse clean
	${CC} ${GLOBAL_LINKS} ${FLAGS} -o ${FBOFILE} ${FBIFILE} 

#make all and run framebuffer
run: all
	./${FBOFILE}

#compile framebuffer only
framebuffer: framebuffer.c clean
	${CC} ${GLOBAL_LINKS} ${FLAGS} -o ${FBOFILE} ${FBIFILE} 

#compile mouse only
mouse: mouse.c clean
	${CC} -o ${MFBOFILE} ${FLAGS} ${MFBIFILE} -lrt

#git commit
git: clean all
	git add ${FBIFILE} ${MFBIFILE} Makefile
	git commit

#git show log
gitl:
	git log

#remove output files
clean:
	rm -f ${FBOFILE} ${MFBOFILE}

#start debugger for framebuffer
debug: framebuffer 
	gdb ${FBOFILE}
