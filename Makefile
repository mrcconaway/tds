a.out:
	g++ -o tds *.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
	./tds
w:
	g++ -o tds *.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17
	./tds