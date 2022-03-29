#pragma once
#include <conio.h>
#include "Point.h"
#include "PacMan.h"
#include "Ghost.h"
#include "Fruit.h"
#include "EnumsAndSTL.h"
void setTextColor(Color);
void hideCursor();
void gotoxy(int, int);
void clrscr();

class Board
{
	Point TL, BR;
	Point mikra;
	array <array<char,MAXCOLS>,MAXROWS> table;
	static bool ColorON;
	vector<Point> ValidFruitPos;
	
public:
	static int countMap;
	array <array<bool, MAXCOLS>, MAXROWS> foodarr = { false };
	array <array<bool, MAXCOLS>, MAXROWS> wallarr = { false };
	void updateFoodArr(const Point& obj) { foodarr[obj.GetY()][obj.GetX()] = false; }
	Point getmikraLocation()const { return mikra; }
	void setmikraLocation(int x, int y) { mikra.setXY(x, y); }
	void displayLivesAndScore(int lives, int score, const Point& location, bool silent)const;
	void searchMapFileName(vector<string>& filename)const;
	void ReadBoardFromFile(string filename, bool& EndGame);
	void initAndPrintBoard(PacMan& pacman, vector <Ghost>& ghosts, bool silent);
	void makeEmptyTable(array <array<char, MAXCOLS>, MAXROWS>& table, array <array<bool, MAXCOLS>, MAXROWS>& food, array <array<bool,MAXCOLS>, MAXROWS>& walls);
	static void setColorChoice(char choice);
	static bool IsColorOn() { return ColorON; }
	Point getTL()const { return TL; }
	Point getBR()const { return BR; }
	void findTLBR();
	void CheckForInvisibleTunnels(int x, int y, char space)const;
	void PrepareMikraRectangle(int row, int col);
	const vector<Point>& GetValidFruitPos() { return ValidFruitPos; }
	bool checkValidMap();
	bool ValidBoardSizeFrame();
};