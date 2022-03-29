#include "Menu.h"

void Menu::printDificultyMenu()const
{
	cout << "Choose dificulty level: " << endl << "(a) BEST" << endl << "(b) GOOD" << endl << "(c) NOVICE" << endl << endl;
}

bool Menu::isValidLevel(char& level)const
{
	if (level == 'a' || level == 'A')
	{
		level = 'a';
		return true;
	}
	else if (level == 'b' || level == 'B')
	{
		level = 'b';
		return true;
	}
	else if (level == 'c' || level == 'C')
	{
		level = 'c';
		return true;
	}
	else
	{
		return false;
	}
}

bool Menu::isValidChoice(char choice)const
{
	if (choice != '1' && choice != '7' && choice != '8' && choice != '9' && choice != '5')
		return false;

	return true;
}

void Menu::printManualInstrctions()const
{
	if (Board::IsColorOn())
		setTextColor(Color::WHITE);

	clrscr();
	cout << "Hello there!, in this game you play the pacMan game." << endl
		<< "controlls are:" << endl << endl << "W -> up\nX -> down\nA -> left\nD -> right\nS -> STAY Mode" << endl << endl
		<< "You have 3 Lives, when ever a ghost \"eats\" you, your life gets reduce by one." << endl
		<< "Your score increase by one every time you eat a breadcrumb, to win you need to eat all the breadcrumbs in 3 diffrent maps" << endl
		<< "Sometimes a fruit represting random number apears on the screen, eating the fruit will grant you extra bonus points" << endl
		<< "If you want to Pause the game you can Press ESC,\nWhen ever you want to continue from the exact same point that you paused the game, press ESC again." << endl << endl;
}

char Menu::printMenu(char& level)const
{
	clrscr();
	if (Board::IsColorOn())
		setTextColor(Color::WHITE);

	char choice = 0;
	while (isValidChoice(choice) == false)
	{
		cout << menu;

		cout << endl << "(1) Start a new game with existed map file" << endl << "(5) start without Colors" << endl
			<< "(7) Start a new game with map file of your own" << endl << "(8) Present instructions and keys" << endl << "(9) Exit" << endl << endl;
		choice = _getch();
		if (isValidChoice(choice))
		{
			if (choice == EXIT)
				return choice;

			printDificultyMenu();
			level = _getch();
			while (isValidLevel(level) == false)
			{
				printDificultyMenu();
				level = _getch();
				if (isValidLevel(level))
				{
					break;
				}
				else
				{
					clrscr();
					cout << endl << "Invalid level Choice please try again! " << endl << endl;
					continue;
				}
			}
		}
		else
		{
			clrscr();
			cout << "Invalid Choice please try again! " << endl << endl;
			continue;
		}
	}
	return choice;
}