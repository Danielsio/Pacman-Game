#pragma once
#include <windows.h>
#include <process.h>
#include "EnumsAndSTL.h"
#include "Board.h"
#include "Files.h"

class Game
{
	Board b;
	PacMan pacman;
	vector <Ghost> ghosts;
	Fruit fruit;
	Files f;
	static long int moveCounter;
	vector <string> MapFileNames;
	static GameMode mode;
	static bool silentMode;

public:
	void init(char& choice, bool& status, char level);
	void run(char& choice, char level, bool& EndGame);
	void reset();
	bool PacManGotEaten();
	static void gamePaused() {while (_getch() != ESC){	continue;}	}
	void displayGameOver()const;
	void displayGameVictory()const;
	static void raiseCounter() { moveCounter++; }
	static int getMoveCounter() { return moveCounter; }
	static void resetMoveCounter() {  moveCounter = 0; }
	void checkIfGhostAtetheFruit();
	void checkIfPacManAtetheFruit(unsigned int& points);
	void invisibleTunnelCheck(const Point& TL, const Point& BR);
	void continueToTheNextMap()const;
	int countFood()const;
	void handleFruitAppearanceMode(bool visibleMode, const Point& TL, const Point& BR);
	void GameOver(bool& status, unsigned int& points, unsigned int& lives, long int result, char& choice);
	void GameVictory(bool& status, unsigned int& points, unsigned int& lives, long int result, char& choice);
	void PacmanTunnelBehaviour(const Point& TL, const Point& BR);
	void GhostTunnelBehaviour(const Point& TL, const Point& BR);
	void WritePlayerSelectionToFile(const char* filename, char choice, char level);
	void MakeStepListsEmpty();
	void SaveCreaturesStepsToFile(string filename);
	void SaveResultToFile(string filename);
	void initGameMode(char* load_save);
	void setSilentMode(bool Mode) { silentMode = Mode; }
	static GameMode getMode() { return mode; }
	void loadSteps(string stepFileName);
	void handleSleepSpeed();
	static bool getSilentMode() { return silentMode; }
	void ReadPlayerMenuSelections(const char* filename, char& choice, char& level);
 };