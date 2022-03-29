#include "Fruit.h"
#include "Game.h"
#include <fstream>

void Fruit::printFruit()const
{
	if (Game::getSilentMode())
		return;

	if (visibleMode)
	{
		if (Board::IsColorOn())
			setTextColor(Color::MAGENTA);

		body.draw(digit); // print new point
		hideCursor();
		if (Board::IsColorOn())
			setTextColor(Color::WHITE);
	}
	else {return;}
}

void Fruit::SaveSpwanLocationsToFile(string MapName)
{
	{
		if (SpawnLocations.size() == 0)
			return;

		std::ofstream stepfile(MapName, std::ios::app);
		
		if (!stepfile)
			cout << "file could not be opened!" << endl;

		for (auto iterator = SpawnLocations.begin(); iterator != SpawnLocations.end(); ++iterator)
			stepfile << *iterator;
		stepfile << "\n";

		stepfile.close();
	}
}

void Fruit::move(array <array<bool, MAXCOLS>, MAXROWS>& food, array <array<bool, MAXCOLS>, MAXROWS>& walls)
{
	if (!visibleMode)
	{
		Steps.push_back(direction);
		return;
	}
		
	Point saver = body;

	if (Game::getMode() == GameMode::LOAD)
	{
		direction = (int)Steps.front();
		Steps.pop_front();
	}
	else
	{
		if (Game::getMoveCounter() % 20 == 0)
		{
			setRandDirection(); // random direction
		}
	}
	
	if (isFoodInXY(food, body)) {
		if (Board::IsColorOn())
			setTextColor(Color::WHITE);
		body.draw(FOOD); // create effect of a ghost flying over the food 
		hideCursor();
	}
	else
	{
		body.draw(' '); // delete last point
		hideCursor();
	}
	body.moveWIthDirection(direction);
	if (!IsWallInXY(walls, body))
	{
		printFruit();
	}
	else
	{
		body = saver;
		printFruit();
	}
	if (Game::getMode() == GameMode::SAVE)
		Steps.push_back(direction);
}

void Fruit::SavefruitLocationsToFile(string MapName, list<Point> CreatureLocation)
{
	Point EndOF(-1, -1);

	if (CreatureLocation.size() == 0)
		return;

	std::ofstream stepfile(MapName, std::ios::app);

	if (!stepfile)
		cout << "file could not be opened!" << endl;

	for (auto iterator = CreatureLocation.begin(); iterator != CreatureLocation.end(); ++iterator)
		stepfile << *iterator;

	stepfile << EndOF << endl;

	stepfile.close();
}

void Fruit::SaveFruitDigitsToFile(string MapName, list<char> Creaturedigits)
{
	if (Creaturedigits.size() == 0)
		return;

	std::ofstream stepfile(MapName, std::ios::app);

	if (!stepfile)
		cout << "file could not be opened!" << endl;

	for (auto iterator = Creaturedigits.begin(); iterator != Creaturedigits.end(); ++iterator)
		stepfile << (int)*iterator;
	stepfile << "\n";

	stepfile.close();
}