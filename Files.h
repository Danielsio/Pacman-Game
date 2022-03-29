#pragma once
#include "EnumsAndSTL.h"
#include "Creature.h"
#include "Fruit.h"


class Files 
{
public:

	void searchMapFileName(vector<string>& filename)const;
	void SaveGameStepsToFile(string MapName, list <int> CreatureSteps);
	void HandleFileType(string& filename, const char* type);
	void WritePlayerSelectionToFile(const char* filename, char choice, char level);
	void loadCreatreSteps(ifstream& file, Creature& creture);
	void loadFruitLocations(ifstream& file, Fruit& fruit);
	void loadFruitDigits(ifstream& file, Fruit& fruit);
	long int loadResultFromFile(string ResultFile);
	void SaveResultToFile(string filename);
};