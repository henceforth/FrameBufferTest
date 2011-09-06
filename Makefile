OFILE = framebuffer
IFILE = framebuffer.c
LINKS = -lm -Wall

all: framebuffer.c
	gcc ${LINKS} -o ${OFILE} ${IFILE} 

run: all
	./${OFILE}

git: all clean
	git add ${IFILE} Makefile
	git commit

gitl:
	git log

clean:
	rm -f ${OFILE}

debug: framebuffer.c
	gcc -g ${LINKS} -o ${OFILE} ${IFILE}
	gdb ${OFILE}
