CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm -ldl#-lXrandr

all: walk2 sqlite3.o

walk2: walk2.cpp log.cpp masonP.cpp hasunK.cpp fernandoH.cpp emilM.cpp victorM.cpp sqlite3.o
	g++ $(CFLAGS) walk2.cpp log.cpp masonP.cpp hasunK.cpp fernandoH.cpp emilM.cpp victorM.cpp sqlite3.o libggfonts.a -Wall -Wextra $(LFLAGS) -owalk2

walk2Test: walk2.cpp log.cpp masonP.cpp hasunK.cpp fernandoH.cpp emilM.cpp victorM.cpp sqlite3.o
	g++ $(CFLAGS) walk2.cpp log.cpp masonP.cpp hasunK.cpp fernandoH.cpp emilM.cpp victorM.cpp sqlite3.o libggfonts.a -Wall -Wextra $(LFLAGS) -owalk2Test -DPROFILE_VICTOR

victorMTests: victorMTests.cpp victorM.cpp
	g++ $(CFLAGS) victorMTests.cpp victorM.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o victorMTests

sqlite3.o: sqlite3.c
	gcc -o sqlite3.o -c sqlite3.c

clean:
	rm -f walk2Test
	rm -f victorMTests
	rm -f walk2
	rm -f *.o

