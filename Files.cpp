#include "Files.h"
#include <filesystem>
#include <iterator>
#include <fstream>
#include "Game.h"

namespace fs = std::filesystem;

void Files::searchMapFileName(vector<string>& filename)const
{
	string path = ".";
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (entry.path().string().ends_with(".screen"))
		{
			filename.push_back((entry.path().string().substr(2, entry.path().string().length())));
		}
	}
}

void Files::SaveGameStepsToFile(string MapName, list<int> CreatureSteps)
{
	if (CreatureSteps.size() == 0)
		return;

	std::ofstream stepfile(MapName, std::ios::app);

	for (auto iterator = CreatureSteps.begin(); iterator != CreatureSteps.end(); ++iterator)
		stepfile << *iterator;
	stepfile << "\n";

	stepfile.close();
}

void Files::HandleFileType(string& filename, const char* type)
{
	size_t pos = filename.find(".");

	filename.replace(filename.begin() + pos + 1, filename.end(), type);
}

void Files::WritePlayerSelectionToFile(const char* filename, char choice, char level)
{
	std::ofstream fwrite(filename, std::ios::beg);

	if (!fwrite)
		cout << "file could not be opened!" << endl;

	fwrite << choice;
	fwrite << level;

	fwrite.close();
}

void Files::SaveResultToFile(string filename)
{
	std::ofstream fwrite(filename, std::ios::trunc);

	if (!fwrite)
		cout << "file could not be opened!" << endl;

	fwrite << Game::getMoveCounter();
}

void Files::loadCreatreSteps(ifstream& file, Creature& creture)
{
	char tmp = file.get();

	while (tmp != '\n')
	{
		tmp -= '0';
		creture.setStep(tmp);
		tmp = file.get();
	}
}

void Files::loadFruitLocations(ifstream& file, Fruit& fruit)
{
	Point tmp;

	file >> tmp;
	while (tmp != Point(-1, -1))
	{
		fruit.setSpawn(tmp);
		file >> tmp;
	}
}

void Files::loadFruitDigits(ifstream& file, Fruit& fruit)
{
	char tmp = '0';

	tmp = file.get();
	while (tmp == ' ' || tmp == '\n')	 // clean the ' ' space in file;
		tmp = file.get();
	while (tmp != '\n')
	{
		fruit.setDigit(tmp);
		tmp = file.get();
	}
}

long int Files::loadResultFromFile(string ResultFile)
{
	long int tmp;
	ifstream resultfile(ResultFile, std::ios::in);

	if (!resultfile)
		cout << "file could not be opened!" << endl;

	resultfile >> tmp;
	return tmp;
	resultfile.close();
}