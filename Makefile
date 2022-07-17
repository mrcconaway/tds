a.out:
	g++ -o tds *.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
	./tds
