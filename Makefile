#file stuff, for gcc, git, clean...
CFILES = mouse.c framebuffer.c client.c
HEADERFILES = mouse.h framebuffer.h client.h
MISCFILES = Makefile #included in git
OUTFILE = client

#Compiler/Linker stuff
GLOBAL_LINKS = -lrt 
FLAGS = -Wall 
DEBUG_FLAGS = -g

client: client.c mouse.o framebuffer.o 
	${CC} ${FLAGS} ${GLOBAL_LINKS} -o ${OUTFILE} client.c *.o 

%.o: %.c
	${CC} ${FLAGS} ${DEBUG_FLAGS} -o $@ -c $<

#remove output files
clean:
	rm -f *.o ${OUTFILE} 

debug: client.c mouse.o framebuffer.o
	${CC} ${FLAGS} ${DEBUG_FLAGS} ${GLOBAL_LINKS} -o ${OUTFILE} client.c *.o 
	gdb ${OUTFILE}

#git commit
git: clean client 
	git add ${CFILES} ${HEADERFILES} ${MISCFILES} 
	git commit

#git show log
gitl:
	git log

