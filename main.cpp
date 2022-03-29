#include "Game.h"
#include "Menu.h"
/****************************************************************
*							PACMAN #1							*
*			Made by: Daniel Karmazin     Daniel Sionov			*
*			ID's:     208063362           318670668				*
*																*
****************************************************************/

bool load_is_over = false;

int main(int argc,char* argv[])
{	
	char choice, level;
	Game game;
	Menu mainMenu;
	
	switch (argc)
	{
	case 1: // simple
		break;
	case 2: // load || save
		game.initGameMode(argv[1]);
		break;
	case 3: // silent
		game.initGameMode(argv[1]);
		game.initGameMode(argv[2]);
		break;
	default: 
		cout << "Invalid_Input!" << endl;
		return 0;
	}

	do
	{
		bool EndGame = false;
		if (Game::getMode() == GameMode::SAVE || Game::getMode() == GameMode::SIMPLE)
		{
			choice = mainMenu.printMenu(level);
			if (choice == INSTRUCTIONS)
			{
				mainMenu.printManualInstrctions(); continue;
			}
		}
		else 
		{
			game.ReadPlayerMenuSelections("choiceAndLevel.txt", choice, level);
		}

		if (choice != EXIT)
		{
			game.init(choice, EndGame, level);
			game.run(choice, level, EndGame);
		}
	} while (choice != EXIT);
}


