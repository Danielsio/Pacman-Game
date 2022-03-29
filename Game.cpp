#include "Game.h"
#include <fstream>

long int Game::moveCounter = 0;
GameMode Game::mode = GameMode::SIMPLE;
bool Game::silentMode = false;

void Game::init(char& choice, bool& status, char level)
{
	if (Board::countMap == 0 && mode == GameMode::SAVE)
		f.WritePlayerSelectionToFile("choiceAndLevel.txt", choice, level);

	string fileName;
	
	moveCounter = 0; //reset point in time

	if (Board::countMap == 0)
	{
		f.searchMapFileName(MapFileNames);
	}
	else if (Board::countMap == MapFileNames.size())
	{
		clrscr();
		cout << "You WON !!" << endl;
		Sleep(2000);
		status = true;
		return;
	}

	if (MapFileNames.size() == 0)
	{
		clrscr();
		cout << "sry you got no maps in the directory, please insert maps to the project directory!!" << endl;
		Sleep(3000);
		status = true;
		return;
	}

	if (choice == '7')
	{
		cout << "enter your map file name please:" << endl;
		cin >> fileName;
		MapFileNames.resize(0);
		MapFileNames.push_back(fileName);
		b.ReadBoardFromFile(MapFileNames[Board::countMap], status);
	}
	else
	{
		b.ReadBoardFromFile(MapFileNames[Board::countMap], status);
	}

	if (!b.checkValidMap())
	{
		Board::countMap++;
		init(choice, status, level); // skip this map
		return;
	}
	if (status)
	{
		return;
	}
	b.setColorChoice(choice);

	b.initAndPrintBoard(pacman, ghosts, silentMode);
	MakeStepListsEmpty();
	
	if (!silentMode)
		reset();
}

void Game::run(char& choice, char level, bool& EndGame)
{
	static unsigned int numOfLives = 3;
	static unsigned int pointCounter = 0;
	int TotalFoodForNextMap = countFood();
	unsigned int foodcounter = 0; //  counter that ignores the addons of the fruit points
	string StepFile,ResultFile;
	char key = 0;
	int dir;
	long int result = 0;

	if (Board::countMap < MapFileNames.size())
	{
		StepFile = ResultFile = MapFileNames[Board::countMap];
		f.HandleFileType(StepFile, "steps");
		f.HandleFileType(ResultFile, "result");
	}

	if (mode == GameMode::LOAD)
	{
		loadSteps(StepFile);
		result = f.loadResultFromFile(ResultFile);
	}


	while (!EndGame)
	{
		if(mode == GameMode::SAVE || mode == GameMode::SIMPLE)
			if (_kbhit())
			{
				key = _getch();
				if (key == ESC)
					gamePaused();

				if ((dir = pacman.getDirection(key)) != -1) // pacman controll
				{
					pacman.setDirection(dir);
				}
			}

		if (PacManGotEaten()) // check if ghost and pacman on the same point.
			numOfLives--;

		pacman.move(b.wallarr);
		if (pacman.isFoodInXY(b.foodarr, pacman.getLocation())) // points mechnisem based on foodArr
		{
			b.updateFoodArr(pacman.getLocation());
			pointCounter++;
			foodcounter++;
		}
		if (PacManGotEaten()) // check if ghost and pacman on the same point.
			numOfLives--;

		if (moveCounter % 100 == 0)
		{
			handleFruitAppearanceMode(fruit.getVisibleMode(), b.getTL(), b.getBR());
		}
		if (moveCounter % 3 == 0)
			fruit.move(b.foodarr, b.wallarr);

		checkIfGhostAtetheFruit();
		checkIfPacManAtetheFruit(pointCounter);

		if (moveCounter % 2 == 0)
		{
			for (unsigned int i = 0; i < ghosts.size(); i++)
				ghosts[i].move(b.foodarr, b.wallarr, level, pacman.getLocation(),ghosts);
		}
		if (!numOfLives) // lives == 0 // gameover
		{
			GameOver(EndGame, pointCounter, numOfLives, result, choice);
			if (mode == GameMode::SAVE)
			{
				SaveCreaturesStepsToFile(StepFile);
				f.SaveResultToFile(ResultFile);
			}
			continue;
		}
		invisibleTunnelCheck(b.getTL(), b.getBR());
		handleSleepSpeed();
		b.displayLivesAndScore(numOfLives, pointCounter, b.getmikraLocation(), silentMode);
		Game::raiseCounter(); // update counter to controll gohst speed and driection

		if (TotalFoodForNextMap == foodcounter)
		{
			Board::countMap++;
			if (choice == '1' || choice == '5')
			{
				if (Board::countMap == MapFileNames.size())
				{
					GameVictory(EndGame, pointCounter, numOfLives, result, choice);
					if (mode == GameMode::SAVE)
					{
						SaveCreaturesStepsToFile(StepFile);
						f.SaveResultToFile(ResultFile);
					}
					continue;
				}
				else
				{
					if (mode == GameMode::SAVE)
					{
						SaveCreaturesStepsToFile(StepFile);
						f.SaveResultToFile(ResultFile);
					}
					continueToTheNextMap();
					init(choice, EndGame, level);
					run(choice, level, EndGame);
				}
			}
			else
			{
				GameVictory(EndGame, pointCounter, numOfLives, result, choice);
				if (mode == GameMode::SAVE)
				{
					SaveCreaturesStepsToFile(StepFile);
					f.SaveResultToFile(ResultFile);
				}
			}
		}
	}
}

void Game::reset()
{
	Point pacmanStartPoint = pacman.getInitLocation();
	Point GhostStartPoint;

	gotoxy(pacman.getLocation().GetX(), pacman.getLocation().GetY());
	cout << ' ';
	pacman.setLocation(pacmanStartPoint.GetX(), pacmanStartPoint.GetY()); pacman.setDirection((int)Direction::STAY);

	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		GhostStartPoint = ghosts[i].getInitLocation();
		gotoxy(ghosts[i].getLocation().GetX(), ghosts[i].getLocation().GetY());
		cout << ' ';
		ghosts[i].setLocation(GhostStartPoint.GetX(), GhostStartPoint.GetY());
	}
}

bool Game::PacManGotEaten()
{
	for (unsigned int i = 0; i < ghosts.size();i++)
		if (ghosts[i].getLocation() == pacman.getLocation())
			{ reset(); return true; }

	return false;
}

void Game::displayGameOver()const
{
	if (silentMode)
		return;

	if(Board::IsColorOn())
		setTextColor(Color::RED);
	clrscr();
	cout <<
		"********************************************************\n"
		"*   ____                         ___                   *\n"
		"*  / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __   *\n"
		"* | |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|  *\n"
		"* | |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |     *\n"
		"*  \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|     *\n"
		"*                                                      *\n"
		"********************************************************\n";
	cout << "Press any key to go back to menu...";
	if (_getch()) {} // required to swallow up the input in buffer, in order to avoid entering "Invalid-choice" messege.
}

void Game::displayGameVictory()const
{
	if (silentMode)
		return;

	if (Board::IsColorOn())
		setTextColor(Color::LIGHTGREEN);
	clrscr();
	cout << "\nCongratulations!!\n" << endl;
	cout << "**********************************************************************\n"
			"* ##    ##  #######  ##     ##    ##      ##  #######  ##    ## #### *\n"
			"*  ##  ##  ##     ## ##     ##    ##  ##  ## ##     ## ###   ## #### *\n"
			"*   ####   ##     ## ##     ##    ##  ##  ## ##     ## ####  ## #### *\n"
			"*    ##    ##     ## ##     ##    ##  ##  ## ##     ## ## ## ##  ##  *\n"
			"*    ##    ##     ## ##     ##    ##  ##  ## ##     ## ##  ####      *\n"
			"*    ##    ##     ## ##     ##    ##  ##  ## ##     ## ##   ### #### *\n"
			"*    ##     #######   #######      ###  ###   #######  ##    ## #### *\n"
			"**********************************************************************\n";
	cout << "Press any key to go back to menu...\n";
	if (_getch()) {} // required to swallow up the input in buffer, in order to avoid entering "Invalid-choice" messege.
}

void Game::continueToTheNextMap()const
{
	if (silentMode)
		return;

	if (Board::IsColorOn())
		setTextColor(Color::WHITE);
	clrscr();
	cout << "you have finished this map successfully !!" << endl;
	cout << "Loading the next Map..." << endl << endl;
	Sleep(2000);
}

void Game::checkIfGhostAtetheFruit()
{
	if (fruit.getVisibleMode() == false)
	{
		return;
	}

	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		if (ghosts[i].getLocation() == fruit.getLocation()) 
		{
			fruit.setvisibleMode(false);
			break;
		}
	}
}

void Game::checkIfPacManAtetheFruit(unsigned int& points)
{
	if (fruit.getVisibleMode() == false)
	{
		return;
	}

	if (pacman.getLocation() == fruit.getLocation())
	{
		fruit.setvisibleMode(false);
		points += fruit.getdigit();
	}
}

int Game::countFood()const
{
	unsigned int counter = 0;
	for (int i = 0; i < MAXROWS; i++)
		for (int j = 0; j < MAXCOLS; j++)
		{
			if (b.foodarr[i][j])
				counter++;
		}
	return counter;
}

void Game::handleFruitAppearanceMode(bool visibleMode, const Point& TL, const Point& BR)
{
	Point temp;
	int i;
	int size = b.GetValidFruitPos().size();
	int rows = BR.GetY() - TL.GetY(), cols = (BR.GetX() - TL.GetX());

	if (visibleMode)
	{
		temp = fruit.getLocation();
		gotoxy(temp.GetX(), temp.GetY());
		cout << " ";
		fruit.setvisibleMode(false);
	}
	else
	{
		fruit.setvisibleMode(true);
		fruit.setrandomDigit();
		fruit.setDigit(fruit.getdigit()); // save current digit in a list in order to write to file later..
		i = rand() % size;
		fruit.setLocation(b.GetValidFruitPos()[i].GetX(), b.GetValidFruitPos()[i].GetY());
		fruit.setSpawn(b.GetValidFruitPos()[i]); // save current spawn location in a list in order to write to file later..
	}
}

void Game::invisibleTunnelCheck(const Point& TL, const Point& BR)
{
	//implement invisible tunnels influence on ghosts
	GhostTunnelBehaviour(TL, BR);

	//implement invisible tunnels influence on pacman
	PacmanTunnelBehaviour(TL, BR);

	//implement invisible tunnels influence on fruit
	int fruitX = fruit.getLocation().GetX();
	int fruitY = fruit.getLocation().GetY();

	if (fruitX == TL.GetX() || fruitY == TL.GetY() || fruitX == BR.GetX() || fruitY == BR.GetY())
	{
		fruit.setvisibleMode(false);
		gotoxy(fruitX, fruitY);
		cout << " ";
	}
}

//implement invisible tunnels influence on ghosts
void Game::GhostTunnelBehaviour(const Point& TL, const Point& BR)
{
	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		int x = ghosts[i].getLocation().GetX();
		int y = ghosts[i].getLocation().GetY();
		if (x == TL.GetX() || y == TL.GetY() || x == BR.GetX() || y == BR.GetY())
		{
			char temp_level = 'c';
			ghosts[i].ReverseDirection();
			ghosts[i].move(b.foodarr, b.wallarr, temp_level, pacman.getLocation(), ghosts);
		}
	}
}
//implement invisible tunnels influence on pacman
void Game::PacmanTunnelBehaviour(const Point& TL, const Point& BR)
{
	Point saver = pacman.getLocation();

	if (pacman.getLocation().GetX() == TL.GetX())
	{
		pacman.setLocation(BR.GetX(), pacman.getLocation().GetY());
		if (pacman.IsWallInXY(b.wallarr, pacman.getLocation()))
		{
			pacman.setLocation(saver.GetX(), saver.GetY());
			pacman.setDirection((int)Direction::STAY);
		}
		else
		{
			gotoxy(saver.GetX(), saver.GetY());
			cout << ' ';
		}
	}
	else if (pacman.getLocation().GetX() == BR.GetX())
	{
		pacman.setLocation(TL.GetX(), pacman.getLocation().GetY());
		if (pacman.IsWallInXY(b.wallarr, pacman.getLocation()))
		{
			pacman.setLocation(saver.GetX(), saver.GetY());
			pacman.setDirection((int)Direction::STAY);
		}
		else
		{
			gotoxy(saver.GetX(), saver.GetY());
			cout << ' ';
		}
	}
	else if (pacman.getLocation().GetY() == TL.GetY())
	{
		pacman.setLocation(pacman.getLocation().GetX(), BR.GetY());
		if (pacman.IsWallInXY(b.wallarr, pacman.getLocation()))
		{
			pacman.setLocation(saver.GetX(), saver.GetY());
			pacman.setDirection((int)Direction::STAY);
		}
		else
		{
			gotoxy(saver.GetX(), saver.GetY());
			cout << ' ';
		}
	}
	else if (pacman.getLocation().GetY() == BR.GetY())
	{
		pacman.setLocation(pacman.getLocation().GetX(), TL.GetY());
		if (pacman.IsWallInXY(b.wallarr, pacman.getLocation()))
		{
			pacman.setLocation(saver.GetX(), saver.GetY());
			pacman.setDirection((int)Direction::STAY);
		}
		else
		{
			gotoxy(saver.GetX(), saver.GetY());
			cout << ' ';
		}
	}
}

void Game::GameOver(bool& status, unsigned int& points, unsigned int& lives, long int result, char& choice)
{
	if (silentMode && moveCounter == result)
	{
		cout << "Test Successful !!" << endl;
		exit(0); // assuming player shouldn't continue to menu while silent mode is on
	}
	else if (silentMode && moveCounter != result)
	{
		cout << "Test Failed !!" << endl; 
		exit(0); // assuming player shouldn't continue to menu while silent mode is on
	}
	else
	{
		displayGameOver();
		status = true;
		Board::countMap = 0;
		points = 0;
		lives = 3;
	}

	if (mode == GameMode::LOAD)
	{
		choice = EXIT;
	}
}

void Game::GameVictory(bool& status, unsigned int& points, unsigned int& lives, long int result, char& choice)
{
	if (silentMode && moveCounter == result)
	{
		cout << "Test Successful !!" << endl;
		exit(0); // assuming player shouldn't continue to menu while silent mode is on
	}
	else if (silentMode && moveCounter != result)
	{
		cout << "Test Failed !!" << endl;
		exit(0); // assuming player shouldn't continue to menu while silent mode is on
	}
	else
	{
		status = true;
		displayGameVictory();
		Board::countMap = 0;
		points = 0;
		lives = 3;
	}

	if (mode == GameMode::LOAD)
	{
		choice = EXIT;
	}
}

void Game::MakeStepListsEmpty()
{
	pacman.MakeEmptyList();
	for (unsigned int i = 0; i< ghosts.size(); i++)
		ghosts[i].MakeEmptyList();
	fruit.MakeEmptyList();
	fruit.MakeEmptySpawnList();
	fruit.MakeEmptyDigitList();
}

void Game::SaveCreaturesStepsToFile(string filename)
{
	std::ofstream stepfile(filename, std::ios::trunc); // clean opening of file to save steps 

	if (!stepfile)
		cout << "file could not be opened!" << endl;

	//save pacman steps vector to step file
	f.SaveGameStepsToFile(filename, pacman.getStepList());

	//save ghosts steps vector to step file
	for(unsigned int i = 0; i < ghosts.size();i++)
		f.SaveGameStepsToFile(filename, ghosts[i].getStepList());

	//save fruit steps vector and spawn location vector to step file

	f.SaveGameStepsToFile(filename, fruit.getStepList());
	fruit.SavefruitLocationsToFile(filename, fruit.getSpawnList());
	fruit.SaveFruitDigitsToFile(filename, fruit.getdigitValues());

	stepfile.close();
}

void Game::initGameMode(char* load_save)
{
	if (strcmp(load_save, "-load") == 0)
		this->mode = GameMode::LOAD;
	else if (strcmp(load_save, "-save") == 0)
		this->mode = GameMode::SAVE;
	else if (strcmp(load_save, "-silent") == 0 && mode == GameMode::LOAD)
		silentMode = true;
	else
	{
		
		// throw "unValid mode";
	}
}

void Game::loadSteps(string stepFileName)
{
	ifstream file(stepFileName, std::ios::in);

	if (!file)
		cout << "file could not be opened!" << endl;

	// pacman load
	f.loadCreatreSteps(file, pacman);
		
	// ghosts load
	for (unsigned int i = 0; i <ghosts.size(); i++)
	{
		f.loadCreatreSteps(file, ghosts[i]);
	}

	// fruit laod
	f.loadCreatreSteps(file, fruit);
	f.loadFruitLocations(file, fruit);
	f.loadFruitDigits(file, fruit);

}

void Game::handleSleepSpeed()
{
	if ((!silentMode) && mode == GameMode::LOAD)
			Sleep(50);
	else if (mode == GameMode::SAVE || mode == GameMode::SIMPLE)
		Sleep(100);
}

void Game::ReadPlayerMenuSelections(const char* filename, char& choice, char& level)
{
	ifstream file;
	file.open(filename);
	if (!file)
	{
		cout << "could not open \"player selection\" file" << endl;
		return;
	}
	file >> choice >> level;
}