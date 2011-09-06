OFILE = framebuffer
IFILE = framebuffer.c
LINKS = -lm -Wall

all: framebuffer.c clean
	gcc ${LINKS} -o ${OFILE} ${IFILE} 

run: all
	./${OFILE}

MIFILE = mouse.c
MOFILE = mouse
mouse: mouse.c clean
	gcc -g -o ${MOFILE} ${MIFILE} -lrt

git: clean all
	git add ${IFILE} ${MIFILE} Makefile
	git commit

gitl:
	git log

clean:
	rm -f ${OFILE} ${MOFILE}

debug: framebuffer.c
	gcc -g ${LINKS} -o ${OFILE} ${IFILE}
	gdb ${OFILE}
