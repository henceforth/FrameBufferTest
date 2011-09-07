#file stuff, for gcc, git, clean...
FBOFILE = framebuffer
FBIFILE = framebuffer.c

MBIFILE = mouse.c
MBOFILE = mouse

CFILES = mouse.c framebuffer.c
HEADERFILES = mouse.h framebuffer.h
MISCFILES = Makefile Session.vim

#Compiler/Linker stuff
GLOBAL_LINKS = 
FLAGS = -Wall -g

CC = gcc

#clean and compile all
all: framebuffer mouse clean
#	${CC} ${GLOBAL_LINKS} ${FLAGS} -o ${FBOFILE} ${FBIFILE} 
	echo "rebuild it all"


#make all and run framebuffer
run: all
	./${FBOFILE}

#compile framebuffer only
framebuffer: ${FBIFILE} clean
	${CC} ${GLOBAL_LINKS} ${FLAGS} -o ${FBOFILE} ${FBIFILE} 

#compile ${MBIFILE} only
mouse: ${MBIFILE} clean
	${CC} ${GLOBAL_LINKS} ${FLAGS} -o ${MBOFILE} ${MBIFILE} -lrt

#git commit
git: clean all
	git add ${CFILES} ${HEADERFILES} ${MISCFILES} 
	git commit

#git show log
gitl:
	git log

#remove output files
clean:
	rm -f ${FBOFILE} ${MBOFILE}

#start debugger for framebuffer
debugf: framebuffer 
	gdb ${FBOFILE}

debugm: mouse
	gdb ${MBOFILE}
