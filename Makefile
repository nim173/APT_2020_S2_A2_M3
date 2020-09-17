.default: all

all: APT_2020_S2_A2

clean:
	rm -f APT_2020_S2_A2 *.o

APT_2020_S2_A2: Game.o MainMenu.o Player.o LinkedList.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^
