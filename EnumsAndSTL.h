#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <queue>
#include <list>

using std::list;
using std::array;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::queue;
using std::ifstream;
using std::ofstream;
using std::fstream;

enum dimensions { MAXROWS = 25, MAXCOLS = 81 };
enum icon { FOOD = char(250), WALL = char(219) };
enum class Color{	
	BLACK = 0,
	BLUE = 1,	
	GREEN = 2,
	CYAN = 3,	
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHTGREY = 7,
	DARKGREY = 8,
	LIGHTBLUE = 9,
	LIGHTGREEN = 10,
	LIGHTCYAN = 11,
	LIGHTRED = 12,
	LIGHTMAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};
enum menuKeys { ESC = 27, INSTRUCTIONS = 56, EXIT = 57 };
enum creatures { GHOST = (char)234, PACMAN = (char)001};
enum class Direction { UP, DOWN, LEFT, RIGHT, STAY };
enum class GameMode {SIMPLE, SAVE, LOAD};
