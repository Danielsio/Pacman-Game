#include "PacMan.h"
#include "Game.h"

void PacMan::move(array <array<bool, MAXCOLS>, MAXROWS>& walls)
{
	Point saver = body;
	body.draw(' '); // delete last point
	if (Game::getMode() == GameMode::LOAD)
	{
		direction = (int)Steps.front();
		Steps.pop_front();
	}
	body.moveWIthDirection(direction);
	if (!IsWallInXY(walls, body))
	{
		PrintPacman();
	}
	else
	{
		body = saver;
		PrintPacman();
	}
	if (Game::getMode() == GameMode::SAVE)
		Steps.push_back(direction);
}

int PacMan::getDirection(char key) const
{
	for (int i = 0; i < 10; i++)
	{
		if (key == arrowKeys[i])
			return i % 5;
	}
	return -1;
}

void PacMan::PrintPacman()const
{
	if (Game::getSilentMode())
		return;

	if (Board::IsColorOn())
		setTextColor(Color::YELLOW);

	body.draw(PACMAN); // print new point
	hideCursor();
}