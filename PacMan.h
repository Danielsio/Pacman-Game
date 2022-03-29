#pragma once
#include "Creature.h"
#include "EnumsAndSTL.h"

class PacMan : public Creature
{
	char arrowKeys[10] = { 'w','x','a','d','s','W','X','A','D','S' };
public:
	void move(array <array<bool, MAXCOLS>, MAXROWS>& walls);
	int getDirection(char key) const;
	void PrintPacman()const;
};