#pragma once
#include <bits.h>
#include "EnumsAndSTL.h"
#include "Creature.h"

// A Data Structure for queue used in BFS
struct Node {
	Point pt;
	int dist;
};

class Ghost : public Creature
{
public:
	Ghost() {};
	Ghost(Point initlocation, int dir = 0) : Creature(initlocation, dir, initlocation) {}
	void move(array <array<bool, MAXCOLS>, MAXROWS>& food, array <array<bool, MAXCOLS>, MAXROWS>& walls,
		char& level, const Point& pacman, vector <Ghost>& brotherGhosts);
	void setRandDirection() { direction = rand() % 4; }
	void simpleMove(array <array<bool, MAXCOLS>, MAXROWS>& food, array <array<bool, MAXCOLS>, MAXROWS>& walls);
	void printGhost()const;
	void pathMoves(array <array<bool, MAXCOLS>, MAXROWS>& mat, Point src, Point dest, string& path)const;
	void BestMove(array <array<bool, MAXCOLS>, MAXROWS>& walls, array <array<bool, MAXCOLS>, MAXROWS>& food,
		Point src, Point dest, vector <Ghost>& brotherGhosts);
	bool isValid(int row, int col)const {return (row >= 0) && (col >= 0)	&& (row < MAXROWS) && (col < MAXCOLS);}
	void ReverseDirection();
};