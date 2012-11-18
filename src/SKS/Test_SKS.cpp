// Test_SKS.cpp : Defines the entry point for the console application.
//

#include <tchar.h>
#include <iostream>
#include "SKS.h"

using namespace std;

void PrintMenu();
int main(int argc, char* argv[])
{
	Namespace *name = sks.GetNamespaceFromFile("..\\test.sks");
	KeyValue *keyValue = NULL;
	char *input = new char [60];
	char choice;
	do
	{
		// Prints the menu
		PrintMenu();

		cin.getline(input, 60);
		choice = input[0];
			
		if(strcmp(input, "1") == 0)
		{
			// Frees old one of it's memory and reads the next one
			delete name;
			name = sks.GetNamespaceFromFile("..//test.sks");
		}
		else if(strcmp(input, "2") == 0)
		{
			// Prints the contents of the namespace
			sks.PrintNamespace(name);
		}
		else if(strcmp(input, "3") == 0)
		{
			// Gets users key
			do
			{
				cout << "Enter Key: ";
				cin.getline(input, 60);
			}
			while(strlen(input) == 0);

			// Finds and prints a keyvalue
			keyValue = name->GetKeyValue(input);

			if(keyValue)
				sks.PrintKeyValue(keyValue);
			else
				cout << "Could not find that key" << endl;
		}
		else if(!strcmp(input, "4") == 0)
		{
			cout << input << " is not recognized" << endl;
		}
	}
	while(strcmp(input, "4") != 0);

	return 0;
}
void PrintMenu()
{
	cout << endl
		<< "1. Read File" << endl
		<< "2. Display File" << endl
		<< "3. Find KeyValue" << endl
		<< "4. Quit" << endl;
}
