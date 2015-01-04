CFLAGS = -c -g -Wall
GL_INCLUDE    = -I/usr/include/GL
GL_LIBDIR    = -L. -L/usr/X11R6/lib
#GL_LIBRARIES = -lfreeglut -lopengl32 -lglu32  #Pour windows
GL_LIBRARIES = -lglut -lGL -lm  -lGLU -lXmu -lXext -lXi -lX11 #Pour linus

#changer les librairies par celle du prof sous linux#
CC=gcc
OS = "unknown"
SRC= $(wildcard src/*.c)
DIR_OBJ=obj/
OBJ= $(SRC:.c=.o)

all: delaunay

$(DIR_OBJ)%.o : src/%.c
	$(CC) $(GL_INCLUDE) $(CFLAGS) $<
delaunay: $(OBJ)
	$(CC) $(GL_LIBDIR) $^ $(GL_LIBRARIES) -o $@
	
#
clean:
	@echo "operating system = $(OS)"
	rm -rf obj/*.o src/*.o delaunay DOX

################################################################################
# Generate doxygen documentation of file main.c using command file main.dox.
dox: main.c
	rm -rf DOX
	doxygen main.dox
#

