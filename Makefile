CFLAGS = -I ./include
LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: background

background: background.cpp hcheema.cpp
	g++ $(CFLAGS) background.cpp hcheema.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -obackground

clean:
	rm -f background
	rm -f *.o
