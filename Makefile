WFLAGS = -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17
LFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
a.out:
	g++ -o tds *.cpp $(LFLAGS)
	./tds
w: build
	./tds
build: main.cpp olcPixelGameEngine.o game.o
	g++ -o tds main.cpp olcPixelGameEngine.o game.o $(WFLAGS)
olcPixelGameEngine.o: olcPixelGameEngine.h olcPixelGameEngine.cpp
	g++ -c olcPixelGameEngine.cpp
game.o: game.h game.cpp scoreSystem.h
	g++ -c game.cpp
clean:
	-del *.o 
	-rm *.o 