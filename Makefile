FBOFILE = framebuffer
FBIFILE = framebuffer.c

MBIFILE = mouse.c
MBOFILE = mouse

GLOBAL_LINKS = 
FLAGS = -Wall -g

#clean and compile all
all: framebuffer mouse tags clean
	${CC} ${GLOBAL_LINKS} ${FLAGS} -o ${FBOFILE} ${FBIFILE} 

#tag list
tags: ${FBIFILE} ${Makefile} ${MBIFILE}
	ctags .

#make all and run framebuffer
run: all
	./${FBOFILE}

#compile framebuffer only
framebuffer: ${FBIFILE} clean
	${CC} ${GLOBAL_LINKS} ${FLAGS} -o ${FBOFILE} ${FBIFILE} 

#compile ${MBIFILE} only
mouse: ${MBIFILE} clean
	${CC} -o ${MBOFILE} ${FLAGS} ${MBIFILE} -lrt

#git commit
git: clean all
	git add ${FBIFILE} ${MBIFILE} Makefile Session.vim
	git commit

#git show log
gitl:
	git log

#remove output files
clean:
	rm -f ${FBOFILE} ${MBOFILE}

#start debugger for framebuffer
debug: framebuffer 
	gdb ${FBOFILE}
