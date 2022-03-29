#include "Board.h"
#include <fstream>


bool Board::ColorON = true;
int Board::countMap = 0;

void Board::displayLivesAndScore(int lives, int score, const Point& location, bool silent)const
{
	if (silent)
		return;

	if (ColorON)
		setTextColor(Color::LIGHTGREEN);
	gotoxy(location.GetX(), location.GetY());
	cout << "lives: " << lives;
	gotoxy(location.GetX(), location.GetY() + 1);
	cout << "Score: " << score;
	hideCursor();
}

void Board::ReadBoardFromFile(string filename, bool& EndGame)
{
	if (!table.empty()) // if it's not the first read from file..
	{
		makeEmptyTable(table, foodarr, wallarr);
	}
	unsigned int i, j;
	ifstream map(filename, std::ios_base::in);

	if (!map)
	{
		clrscr();
		cout << "no Map Exist by this name.." << endl;
		EndGame = true;
		cout << "Press any key to go back to menu..." << endl;
		if (_getch()) {}
		return;
	}
	

	char ch;

	for (i = 0; !map.eof() && i < MAXROWS; i++)
	{
		for (j = 0; j < MAXCOLS && !map.eof(); j++)
		{
			ch = map.get();
			if (ch != EOF)
				table[i][j] = ch;

			if (table[i][j] == '\n')
			{
				break;
			}
		}
	}
	map.close();
	findTLBR();
}

void Board::makeEmptyTable(array <array<char, MAXCOLS>, MAXROWS>& table, array <array<bool, MAXCOLS>, MAXROWS>& food, array <array<bool, MAXCOLS>, MAXROWS>& walls)
{
	for (unsigned int i = 0; i < table.size(); i++)
		for (unsigned int j = 0; j < table[i].size(); j++)
		{
			table[i][j] = '\0';
			food[i][j] = false;
			walls[i][j] = false;
		}
}

void Board::initAndPrintBoard(PacMan& pacman, vector <Ghost>& ghosts, bool silent)
{
	clrscr();
	Point initiator;
	ghosts.resize(0);
	ValidFruitPos.resize(0);

	for (int i = 0; i < MAXROWS; i++)
		for (int j = 0; j < MAXCOLS && table[i][j] != '\0'; j++)
			switch (table[i][j])
			{
			case '#':
				if (ColorON)
				{
					setTextColor(Color::BLUE);
				}
				gotoxy(j, i);
				if (!silent)
					cout << char(WALL);
				wallarr[i][j] = true;
				foodarr[i][j] = false;
				break;
			case '%':
				gotoxy(j, i);
				if (!silent)
					cout << " ";
				wallarr[i][j] = false;
				foodarr[i][j] = false;
				break;
			case '$':
				ghosts.push_back(Ghost(Point(j, i), -1));
				wallarr[i][j] = false;
				foodarr[i][j] = false;
				gotoxy(j, i);
				if (!silent)
					cout << " ";
				break;
			case '@':
				pacman.setLocation(j, i);
				pacman.setinitLocation(j, i);
				wallarr[i][j] = false;
				foodarr[i][j] = false;
				gotoxy(j, i);
				if (!silent)
					cout << " ";
				break;
			case '\n':
				if (!silent)
					cout << endl;
				wallarr[i][j] = false;
				foodarr[i][j] = false;
				break;
			case '&':
				setmikraLocation(j, i);
				PrepareMikraRectangle(i, j);
				wallarr[i][j] = false;
				foodarr[i][j] = false;
				break;
			default:
				if (ColorON)
					setTextColor(Color::WHITE);
				CheckForInvisibleTunnels(j, i, table[i][j]);
				wallarr[i][j] = false;
				gotoxy(j, i);
				if (i > BR.GetY() || i<TL.GetY() || j>BR.GetX() || j < TL.GetX())
				{
					foodarr[i][j] = false;
					if (!silent)
						cout << " ";
				}
				else
				{
					foodarr[i][j] = true;
					if (!silent)
						cout << char(FOOD);
					ValidFruitPos.push_back(Point(j, i));
				}
				break;
			}
}

//make a rectangle of 3 lines 20 characters from the mikra starting position, override all irelevent characters.
void Board::PrepareMikraRectangle(int row, int col)
{
	int i, j;

	for (i = row; i < table.size() && i < row + 3; i++)
	{
		for (j = col; j < table[i].size() && j < col + 20; j++)
		{
			if (table[i][j] != '&') //dont want to overwrite the mikra symbol.
				table[i][j] = '%'; //put a % to print space and overide everything that is not relevent
								   //if ghost and pacman is in the mikra zone they would be override
		}
	}
}

void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	cout.flush();
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

void clrscr()
{
	system("cls");
}

void setTextColor(Color colorToSet) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)colorToSet);
}

void hideCursor()
{
	HANDLE myconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.dwSize = 1;
	CURSOR.bVisible = FALSE;
	SetConsoleCursorInfo(myconsole, &CURSOR);//second argument need pointer
}

void Board::setColorChoice(char choice)
{
	if (choice == '1' || choice == '7')
	{
		ColorON = true;
	}
	else
	{
		ColorON = false;
	}
}

void Board::findTLBR()
{
	bool found = false;
	int i, j;
	for (i = 0; i < table.size() && !found; i++)
		for (j = 0; j < table[i].size(); j++)
		{
			if (table[i][j] == '#')
			{
				TL.setXY(j, i);
				found = true;
				break;
			}
		}
	found = false;
	for (i = table.size() - 1; i >= 0 && !found; i--)
		for (j = table[i].size() - 1; j >= 0; j--)
		{
			if (table[i][j] == '#')
			{
				BR.setXY(j, i);
				found = true;
				break;
			}
		}
}

void Board::CheckForInvisibleTunnels(int x, int y, char space)const
{
	if (space == ' ' && (TL.GetX() == x || TL.GetY() == y || BR.GetX() == x || BR.GetY() == y))
		cout << space;
}

bool Board::checkValidMap()
{
	int pacman_counter = 0, ghosts_counter = 0, mikraCounter = 0;

	if (!ValidBoardSizeFrame())
	{
		clrscr();
		cout << "Invalid size of map, MAX SIZE is" << MAXCOLS << 'X' << MAXROWS << endl;
		Sleep(3000);
		return false;
	}

	for (unsigned int i = 0; i < table.size();i++)
		for(unsigned int j = 0; j < table[i].size(); j++)
			switch (table[i][j])
			{
			case '$':
				ghosts_counter++;
				break;
			case '@':
				pacman_counter++;
				break;
			case '&':
				mikraCounter++;
				break;
			default:
				break;
			}

	if (pacman_counter != 1)
	{
		clrscr();
		cout << "invalid number of pacmans, Valid map should have only 1 pacman" << endl;
		Sleep(3000);
		return false;
	}
	if (ghosts_counter > 4)
	{
		clrscr();
		cout << "too many ghosts, Valid map should have 0 to 4 ghosts" << endl;
		Sleep(3000);
		return false;
	}
	if (mikraCounter != 1)
	{
		clrscr();
		cout << "invalid number of mikra Locations, Valid map should have only 1 mikra location" << endl;
		Sleep(3000);
		return false;
	}

	return true;
}

bool Board::ValidBoardSizeFrame()
{
	if (BR.GetX() - TL.GetX() > MAXCOLS || BR.GetY() - TL.GetY() > MAXROWS)
	{
		return false;
	}
	return true;
}