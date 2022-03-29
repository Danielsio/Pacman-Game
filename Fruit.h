#pragma once
#include "Creature.h"

class Fruit : public Creature
{
	bool visibleMode = false;
	char digit = '0';
	list<Point> SpawnLocations;
	list<char> digitValues;

public:
	Fruit() {}
	void move(array <array<bool, MAXCOLS>, MAXROWS>& food, array <array<bool, MAXCOLS>, MAXROWS>& walls);
	void setRandDirection() { direction = rand() % 4; }
	void printFruit()const;
	bool getVisibleMode()const {return visibleMode;	}
	void setvisibleMode(bool mode) { visibleMode = mode; }
	void setrandomDigit() { digit = rand() % 5 + '5'; }
	int getdigit()const { return digit - '0'; }
	void setSpawn(const Point& init) { SpawnLocations.push_back(init); }
	void setDigit(const char& digit) { digitValues.push_back(digit); }
	list <Point> getSpawnList() const { return SpawnLocations; }
	list <char> getdigitValues()const { return digitValues; }
	void MakeEmptySpawnList() { SpawnLocations.resize(0); }
	void MakeEmptyDigitList() { digitValues.resize(0); }
	void SaveSpwanLocationsToFile(string MapName);
	void SavefruitLocationsToFile(string MapName, list<Point> Creaturelocation);
	void SaveFruitDigitsToFile(string MapName, list<char> Creaturedigits);
};