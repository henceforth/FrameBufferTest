OFILE = framebuffer
IFILE = framebuffer.c

MIFILE = mouse.c
MOFILE = mouse

LINKS = 
FLAGS = -Wall -g

all: framebuffer mouse clean
	${CC} ${LINKS} ${FLAGS} -o ${OFILE} ${IFILE} 

run: all
	./${OFILE}

framebuffer: framebuffer.c clean
	${CC} ${LINKS} ${FLAGS} -o ${OFILE} ${IFILE} 

mouse: mouse.c clean
	${CC} -o ${MOFILE} ${FLAGS} ${MIFILE} -lrt

git: clean all
	git add ${IFILE} ${MIFILE} Makefile
	git commit

gitl:
	git log

clean:
	rm -f ${OFILE} ${MOFILE}

debug: framebuffer.c
	${CC} -g ${LINKS} -o ${OFILE} ${IFILE}
	gdb ${OFILE}
