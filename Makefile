#file stuff, for gcc, git, clean...
DEPS = client.c mouse.o framebuffer.o cache.o
CFILES = mouse.c framebuffer.c client.c cache.c
HEADERFILES = mouse.h framebuffer.h client.h cache.h
MISCFILES = Makefile #included in git
OUTFILE = client

#Compiler/Linker stuff
GLOBAL_LINKS = -lrt 
FLAGS = -Wall 
DEBUG_FLAGS = -g

client: ${DEPS} 
	${CC} ${FLAGS} ${GLOBAL_LINKS} -o ${OUTFILE} ${DEPS} 

%.o: %.c
	${CC} ${FLAGS} ${DEBUG_FLAGS} -o $@ -c $<

#remove output files
clean:
	rm -f *.o ${OUTFILE} 

debug: ${DEPS}
	${CC} ${FLAGS} ${DEBUG_FLAGS} ${GLOBAL_LINKS} -o ${OUTFILE} ${DEPS} 
	gdb ${OUTFILE}

#git commit
git: clean client 
	git add ${CFILES} ${HEADERFILES} ${MISCFILES} 
	git commit

#git show log
gitl:
	git log

