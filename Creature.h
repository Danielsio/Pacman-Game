#pragma once
#include "Point.h"
#include "EnumsAndSTL.h"


class Creature {
protected:
	Point body;
	int direction = 0;
	Point initLocation;
	list <int> Steps;

	public:
		Creature() {}
		Creature(Point& p, int dir, Point& init) : body(p), direction(dir), initLocation(init) {}
		void setLocation(int x, int y) { body.setXY(x, y); }
		void setinitLocation(int x, int y) { initLocation.setXY(x, y); }
		void setDirection(int dir) { direction = dir; }
		Point getLocation()const { return body; }
		Point getInitLocation()const { return initLocation; }
		bool IsWallInXY(array <array<bool, MAXCOLS>, MAXROWS>& WallsArray, Point obj)const { return WallsArray[obj.GetY()][obj.GetX()]; }
		bool isFoodInXY(array <array<bool, MAXCOLS>, MAXROWS>& foodArr, Point obj)const { return foodArr[obj.GetY()][obj.GetX()]; }
		void setStep(int dir) { Steps.push_back(dir); }
		list <int> getStepList () const { return Steps; }
		void MakeEmptyList() { Steps.resize(0); }
};