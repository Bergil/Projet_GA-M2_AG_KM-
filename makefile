CFLAGS = -c -g -Wall
GL_INCLUDE    = -I/usr/include/GL
GL_LIBDIR    = -L. -L/usr/X11R6/lib
#GL_LIBRARIES = -lfreeglut -lopengl32 -lglu32  #Pour windows
GL_LIBRARIES = -lglut -lGL -lm  -lGLU -lXmu -lXext -lXi -lX11 #Pour linus

#changer les librairies par celle du prof sous linux#
CC=gcc
OS = "unknown"

all: main

%.o : %.c
	$(CC) $(GL_INCLUDE) $(CFLAGS) $<
main: main.o FilePrio.o List.o Vertex.o Node.o Triangle.o
	$(CC) $(GL_LIBDIR) $^ $(GL_LIBRARIES) -o $@
	
#
clean:
	@echo "operating system = $(OS)"
	rm -rf *.o main DOX

################################################################################
# Generate doxygen documentation of file main.c using command file main.dox.
dox: main.c
	rm -rf DOX
	doxygen main.dox
#

