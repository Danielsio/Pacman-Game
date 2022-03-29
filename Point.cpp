#include "Game.h"
#include "Point.h"
#include <fstream>

void Point::draw(char ch) const 
{
	if (Game::getSilentMode())
		return;
	gotoxy(x, y);
	cout << char(ch) << endl;
}

void Point::moveWIthDirection(int dir)
{
	switch (dir)
	{
	case 0: // UP
		--y;
		break;
	case 1: // DOWN
		++y;
		break;
	case 2: // LEFT
		--x;
		break;
	case 3: // RIGHT
		++x;
		break;
	case 4: // STAY
		break;
	}
}

bool operator==(const Point& p1, const Point& p2)
{
	return (p1.x == p2.x) && (p1.y == p2.y);
}

ifstream& operator>>(ifstream& is, Point& p)
{
	int x;
	int y;

	is >> x;
	is >> y;

	p.setXY(x, y);
	return is;
}

ofstream& operator<<(ofstream& is, Point& p)
{
	is << p.GetX() << ' ' << p.GetY() << ' ';

	return is;
}
