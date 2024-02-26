// Program written by Tommy Trinh
// February 25, 2024
// This program simulates a database that can read and write to
// text and binary files
#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include <array>
#include <vector>
#include <cctype>
#include <fstream>
using namespace std;

// declare constants
const char ANIMAL_FILE[] = "c:\\test\\animal.dat",	// location of animal.dat
SPECIES_FILE[] = "c:\\test\\species.txt";			// location of species.txt
const int
MIN_COUNT = 0,			// minimum number of animals
ADD_CHOICE = 1,			// choice to add an animal
DISPLAY_CHOICE = 2,		// choice to display animals
SEARCH_CHOICE = 3,
ENDANGERED_CHOICE = 4,	// choice to display endangered animals
QUIT_CHOICE = 5,		// choice to quit program
MENU_CHOICE_SIZE = 1,	// size of menu choice field
ENDANGERED_POP = 100,	// population considered endangered
TYPE_SIZE = 25;			// size of the animal type field

// define structures
struct species
{
	char typeAnimal[TYPE_SIZE] = "none";	// stores type of animal
	char typeSpecies[TYPE_SIZE]="none";		// species of animal
	int numAnimal = 0;						// stores number of animals
	bool endangered = false;				// flag for endangered
};

// define function prototypes
void header();							// display program info to user
void dataEntry(vector<species>&,		// allows data entry of animals
	const vector<string>,fstream&);
void dataDisplay(vector<species>);		// displays data to user
void endangeredDisplay(const vector<species>);// displays endangered animals
void selectionSort(vector<species>&);	// sorts an array by selection sorting
void vectorSwap(species&, species&);	// swaps two vector elements
void animalSearch(vector<species>&, fstream&,	// sorts and then searches for an animal
	const vector<string>);
void readAnimal(vector<species>&, fstream&);// reads data from a file into memory
void readSpecies(vector<string>&, fstream&);// reads data from a file into memory
void writeRecord(fstream&, species, int);	// writes a record to file
void getUpdateData(species&, const vector<string>);	// get data for updating
void updateRecord(fstream&, species, species);// updates a record in the search function
bool positiveValid(int);				// validates a number is positive
bool menuValid(int);					// validates menu is between first and quit
bool noneValid(const char name[]);		// validates user input is not none
bool duplicateValid(const vector<species>,
	const char name[]);					// validates user has not entered duplicate
bool exitProgram();						// ask user if they really want to quit

int main()
{
	// define variables
	string input;					// temporary string for user input
	int menuChoice;					// user entered menu choice
	vector<species> animalPen;		// initialize a new vector for struct
	vector<string> speciesType;		// initialize a new vector for species

	// Open the animal.dat file in read/write/binary mode
	fstream animalFile(ANIMAL_FILE, ios::in | ios::out | ios::binary);

	// If the file was opened, read the contents of animal.dat into memory.
	// Otherwise, give an error and end the program
	if (animalFile.fail())
	{
		cout << "Error - could not open animal.dat. Closing the program.";
		return 0;
	}
	else
		readAnimal(animalPen, animalFile);

	// open the species.txt in read mode
	fstream speciesFile(SPECIES_FILE, ios::in);
	
	// If the file was opened, read the contents of species.txt into memory.
	// Otherwise, give an error and end the program
	if (!speciesFile)
	{
		cout << "Error - could not open species.txt. Closing the program.";
		return 0;
	}
	else
		readSpecies(speciesType, speciesFile);
	// close the files
	animalFile.close();
	speciesFile.close();

	// call header
	header();

	do
	{
		// display a menu and  ask user for a menu choice
		do
		{
			cout << ADD_CHOICE << ". Add animal(s)\n";
			cout << DISPLAY_CHOICE << ". Display animals\n";
			cout << SEARCH_CHOICE << ". Search for animal\n";
			cout << ENDANGERED_CHOICE << ". Display endangered animals\n";
			cout << QUIT_CHOICE << ". Quit\n\n";
			cout << "Enter the menu choice: ";
			getline(cin, input);
		} while (menuValid(stoi(input)));
		menuChoice = stoi(input);

		// switch statement based on user choice
		switch (menuChoice)
		{
		case ADD_CHOICE:
		{
			dataEntry(animalPen, speciesType, animalFile);
			break;
		}
		case DISPLAY_CHOICE:
		{
			dataDisplay(animalPen);
			break;
		}
		case SEARCH_CHOICE:
		{
			animalSearch(animalPen, animalFile, speciesType);
			break;
		}
		case ENDANGERED_CHOICE:
		{
			endangeredDisplay(animalPen);
			break;
		}
		case QUIT_CHOICE:
		{
			if (exitProgram())
				menuChoice = ADD_CHOICE;
			break;
		}
		default:
		{
			cout << "Something went wrong\n\n";
			break;
		}
		}
	} while (menuChoice != QUIT_CHOICE);

	// clear the vector
	animalPen.clear();

	// goodbye message
	cout << "\nThank you for using the CMSY 171 Animal Count Program\n\n";

	// end of main
	system("pause");
	return 0;
}

// shows program title, copyright, and program info to user
void header()
{
	cout << "\t\t\t\tCMSY 171 Lab 3\n"
		<< "Copyright 2024 - Howard Community College All rights reserved;"
		<< "Unauthorized duplication prohibited.\n\n"
		<< "\t\t\tWelcome to CMSY-171 Animal Count Program\n\n";
}


// validates menu choice, returns true if invalid and keeps looping, returns false if
// valid and exits loop
bool menuValid(int choice)
{
	if (ADD_CHOICE > choice || choice > QUIT_CHOICE)
	{
		cout << "Invalid choice! Try again: \n\n";
		return true;
	}
	return false;
}

// tests if animal count is positive, returns true if invalid and keeps looping,
// returns false if valid and exits loop
bool positiveValid(int num)
{
	if (num < MIN_COUNT)
	{
		cout << "Invalid count! Try again\n";
		return true;
	}
	return false;
}

// tests if user entered none. If user entered none, return true and stop loop
// if not none, return false and ask for more info
bool noneValid(const char name[])
{
	char temp[TYPE_SIZE];	// temp cstring to test none
	for (int index = 0; index < TYPE_SIZE; index++)
		temp[index] = toupper(name[index]);
	if (!strcmp(temp, "NONE"))
		return true;
	return false;
}

// ask user if they really want to quit, return true if they do not and keep
// looping, return false if they want to quit and leave loop
bool exitProgram()
{
	char temp[TYPE_SIZE];	// holds user's choice to quit
	cout << "\nDo you wish to exit the program (Y or N): ";
	cin.getline(temp, TYPE_SIZE);
	while (toupper(temp[0]) != 'Y' && toupper(temp[0]) != 'N')
	{
		cout << "Please enter Y or N: ";
		cin.getline(temp, TYPE_SIZE);
	}
	if (toupper(temp[0]) == 'Y')
		return false;
	else
		return true;
}

// lets user enter data on the animal types, if user enters none, quit out
// Added "&" to the argument because otherwise the changes were local
// and did not appear to the dataDisplay function
void dataEntry(vector<species>& animalPen, const vector<string>speciesType,
	fstream& animalFile)
{
	string input;				// temporary input string
	bool noneFlag = true;		// flag for none 
	species newAnimal = { "","",0,false };	// temporary struct for data entry

	// loop until none input
	while (noneFlag)
	{
		do // ask user for animal name, if duplicate then repeat prompt
		{  // if not duplicate, or user entered none, stop looping
			cout << "Please enter an animal type (none to stop): ";
			cin.getline(newAnimal.typeAnimal, TYPE_SIZE);
		} while (duplicateValid(animalPen, newAnimal.typeAnimal));
		// if user types none, set flag to false and exit loop
		if (noneValid(newAnimal.typeAnimal))
			noneFlag = false;

		// if not none and not duplicate, ask for the rest of info. 
		// Added endangered=false because it would always be true once 
		// one entry is true, never false anymore
		if (noneFlag)
		{
			do
			{
				// ask for the animal species
				cout << "\nSelect the species from the list:\n";
				for (int index = 0; index < speciesType.size(); index++)
					cout << index+1<<". "<<speciesType[index] << endl;
				cout << "Please enter number of species here: ";
				getline(cin, input);
				while (stoi(input) < 1 || stoi(input) > speciesType.size())
				{ // validate it is between 1 and max
					cout << "Error - entry must be a valid option. Try again: ";
					getline(cin, input);
				}
				// copy the species into the struct
				for (int index = 0; index < speciesType[stoi(input)-1].length(); index++)
				{
					if (isalpha(speciesType[stoi(input) - 1][index]))
					{
						newAnimal.typeSpecies[index] = speciesType[stoi(input) - 1][index];
					}
					else
						newAnimal.typeSpecies[index] = '\0';
				}

				cout << "Enter the animal type's count: ";
				getline(cin, input);
			} while (positiveValid(stoi(input)));
			newAnimal.numAnimal = stoi(input);
			if (newAnimal.numAnimal < ENDANGERED_POP)
				newAnimal.endangered = true;
			else
				newAnimal.endangered = false;
			// write data from temp struct to animal.dat file
			writeRecord(animalFile, newAnimal, -1);
			// Add data from temp struct to permanent struct
			animalPen.push_back(newAnimal);
		}
	}
	cout << endl;
}

// test if name is a duplicate. If it is, return true and keep looping
// if not, return false and exit loop
bool duplicateValid(const vector <species> animal, const char name[])
{
	for (auto index : animal)
		if (!strcmp(index.typeAnimal, name))
		{
			cout << "Error - this animal is already in the database. "
				<< "Please reenter.\n";
			return true;
		}
	return false;
}


// display contents of animal array to user, if an entry has 0 number of
// animals, end the loop and redisplay menu
void dataDisplay(vector<species> animals)
{
	// if the vector is empty, give an error 
	if (animals.empty())
		cout << "You have not entered any data, returning to menu.\n\n";
	else
	{
		// send the vector to a sort function
		selectionSort(animals);
		cout << "\nThere are " << animals.size() << " animals in the database.\n";
		for (int index = 0; index < animals.size(); index++)
		{
			cout << "\nAnimal: " << animals.at(index).typeAnimal << endl;
			cout << "The species is: " << animals.at(index).typeSpecies << endl;
			cout << "Has a count of: " << animals.at(index).numAnimal << endl;

			if (animals.at(index).endangered)
				cout << "This animal is endangered!!\n\n";
			else
				cout << "This animal is not endangered!!\n\n";
		}

	}
}

// display endangered species to user, if there are no endangered species
// then counter variable is equal to 0, which is the same as false
// if counter is incremented, it is not 0, which is the same as true
// if counter is false, display error message and redisplay menu
void endangeredDisplay(const vector<species> animals)
{
	int counter = 0;	// temporary counter variable
	for (auto status : animals)
		if (status.endangered)
		{
			cout << "Animal: " << status.typeAnimal << " is endangered!!\n";
			counter++;
		}
	if (!counter)
		cout << "There are no endangered species. Redisplaying menu.\n";
	cout << endl;
}

// Sort an array using selection sorting in alphabetical order according
// to the ADT animal type/name
void selectionSort(vector<species>& animal)
{
	string min;
	int index;

	for (int i = 0; i < animal.size(); i++)
	{
		index = i;
		min = animal.at(i).typeAnimal;
		for (int j = i + 1; j < animal.size(); j++)
		{
			if (animal.at(j).typeAnimal < min)
			{
				index = j;
				min = animal.at(j).typeAnimal;
			}
		}
		vectorSwap(animal.at(index), animal.at(i));
	}
}

// Swaps two vector elements, using a temporary ADT
void vectorSwap(species& a, species& b)
{
	species temp;
	temp = a;
	a = b;
	b = temp;
}

// Sort an ADT and then asks user for a name to search for.
// Then searches for an animal name using
// a binary search approach
void animalSearch(vector<species>& animal,fstream &aFile,vector<string>speciesList)
{
	char temp[TYPE_SIZE];		//temporary input holder
	species newAnimal;
	// first sort by alphabetical order
	selectionSort(animal);
	// ask for the name to search for
	cout << "Please enter the name of the animal you are looking for: ";
	cin.getline(temp, TYPE_SIZE);

	// set up the search
	int first = 0,					// first vector element
		last = animal.size() - 1,	// last vector element
		middle,						// mid point of search
		position = -1;				// position of search value
	bool found = false;				// flag for found

	// search for the entered animal
	while (!found && first <= last)
	{
		middle = (first + last) / 2;// calculate midpoint
		if (!strcmp(animal.at(middle).typeAnimal, temp))
		{							// if value is found at mid
			found = true;
			position = middle;
		}
		else if (strcmp(temp, animal.at(middle).typeAnimal) < 0)
			last = middle - 1;		// if value is in lower half
		else
			first = middle + 1;		// if value is in upper half
	}

	// display the results of the search
	if (found)
	{
		cout << "Animal was found:\n"
			<< "Animal: " << animal.at(position).typeAnimal << endl
			<< "The species is: " << animal.at(position).typeSpecies << endl
			<< "Has a count of: " << animal.at(position).numAnimal << endl;
		if (animal.at(position).endangered)
			cout << "This animal is endangered!!\n\n";
		else
			cout << "This animal is not endangered!!\n\n";

		// ask user if they want to update record
		cout << "Do you want to update this record? (y/n): ";
		cin.getline(temp, TYPE_SIZE);
		while (toupper(temp[0]) != 'Y' && toupper(temp[0]) != 'N')
		{
			cout << "Please enter Y or N: ";
			cin.getline(temp, TYPE_SIZE);
		}
		if (toupper(temp[0]) == 'Y')
		{
			getUpdateData(newAnimal,speciesList);
			writeRecord(aFile, newAnimal, position);
			animal.at(position) = newAnimal;
		}
	}
	else
	{
		cout << "We could not find your animal in the database at this time. "
			<< "Please add the animal to the database.\n\n";
	}
}

// Reads structure data from a file and copies it into memory
void readAnimal(vector<species>& animalPen, fstream& animalFile)
{
	species tempAnimal = { "","",0,false },	// temporary structure for push back
		tempFile;		// temporary structure for file

	// read the first record
	animalFile.read(reinterpret_cast<char*>(&tempFile), sizeof(tempFile));
	while (!animalFile.eof())
	{
		// c strings are arrays so go through them one index at a time
		for (int index = 0; index < strlen(tempFile.typeAnimal); index++)
			tempAnimal.typeAnimal[index] = tempFile.typeAnimal[index];
		// same for species, one index at a time
		for (int index = 0; index < strlen(tempFile.typeSpecies); index++)
			tempAnimal.typeSpecies[index] = tempFile.typeSpecies[index];
		// copy over the count and endangered flag
		tempAnimal.numAnimal = tempFile.numAnimal;
		tempAnimal.endangered = tempFile.endangered;
		// push back the vector with a new entry
		animalPen.push_back(tempAnimal);
		// clear the temp structure in preparation for the next loop
		tempAnimal = { "","",0,false };
		// read the next record
		animalFile.read(reinterpret_cast<char*>(&tempFile), sizeof(tempFile));
	}
}

// Read species data from a file and copy it into memory
void readSpecies(vector<string>& speciesType, fstream& speciesFile)
{
	string temp;	// temp string
	// read the first record
	getline(speciesFile, temp, '\n');
	// if successful, popback into vector and continue
	while (speciesFile)
	{
		speciesType.push_back(temp);
		getline(speciesFile, temp, '\n');
	}
}

// write record
void writeRecord(fstream& file, species animal, int recNum)
{
	long startRecordOffset;

	// if record number is less than 0, append a record to the end of file
	if (recNum>=0)
	{
		file.open(ANIMAL_FILE, ios::out | ios::binary);
		// set the initial offset if not zero
		startRecordOffset = recNum * sizeof(species);
		file.seekp(startRecordOffset, ios::beg);
	}
	else // append to the end of the file
	{
		file.open(ANIMAL_FILE, ios::app | ios::binary);
	}
	if (!file)	// if cannot open, give error
	{
		cout << "Error - cannot open Animal.dat\n\n";
		return;
	}
	
	// write the record
	file.write(reinterpret_cast<char*>(&animal), sizeof(species));
	file.close();
}

// get update data
void getUpdateData(species& newEntry, const vector<string>speciesType)
{
	string input;	// temp variable for input
	// get name
	cout << "Please enter an animal name: ";
	cin.getline(newEntry.typeAnimal, TYPE_SIZE);
	// get species
	cout << "\nSelect the species from the list:\n";
	for (int index = 0; index < speciesType.size(); index++)
		cout << index + 1 << ". " << speciesType[index] << endl;
	cout << "Please enter number of species here: ";
	getline(cin, input);
	while (stoi(input) < 1 || stoi(input) > speciesType.size())
	{ // validate it is between 1 and max
		cout << "Error - entry must be a valid option. Try again: ";
		getline(cin, input);
	}
	// copy the species into the struct
	for (int index = 0; index < speciesType[stoi(input) - 1].length(); index++)
	{
		if (isalpha(speciesType[stoi(input) - 1][index]))
		{
			newEntry.typeSpecies[index] = speciesType[stoi(input) - 1][index];
		}
		else
			newEntry.typeSpecies[index] = '\0';
	}
	do
	{
		cout << "Enter the animal type's count: ";
		getline(cin, input);
	} while (positiveValid(stoi(input)));

		newEntry.numAnimal = stoi(input);
		if (newEntry.numAnimal < ENDANGERED_POP)
			newEntry.endangered = true;
		else
			newEntry.endangered = false;
}

// update record
void recordUpdate(fstream& file, species oldEntry, species newEntry)
{
	return;
}