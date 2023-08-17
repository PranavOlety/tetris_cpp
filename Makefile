CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -g
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system



all: tetris.exe tetris_tests.exe

tetris: tetris.exe


test:  tetris_tests.exe
	   ./tetris_tests.exe

tetris_tests.exe: grid.cpp grid.hpp tetris_tests.cpp
	$(CXX) $(CXXFLAGS) grid.cpp tetris_tests.cpp -o tetris_tests.exe $(SFML_LIBS)
	

tetris.exe: grid.cpp grid.hpp main.cpp
	$(CXX) $(CXXFLAGS) grid.cpp main.cpp -o tetris.exe $(SFML_LIBS)

clean:
	rm -vf *.exe
