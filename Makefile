CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk2

walk2: walk2.cpp log.cpp masonP.cpp hasunK.cpp fernandoH.cpp emilM.cpp victorM.cpp
	g++ $(CFLAGS) walk2.cpp log.cpp masonP.cpp hasunK.cpp fernandoH.cpp emilM.cpp victorM.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -owalk2

clean:
	rm -f walk2
	rm -f *.o

