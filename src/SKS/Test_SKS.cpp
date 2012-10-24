// Test_SKS.cpp : Defines the entry point for the console application.
//

#include <tchar.h>
#include <iostream>
#include "SKS.h"

using namespace std;
void PrintNamespaceContents(Namespace*, int);

int main(int argc, char* argv[])
{
	Namespace *name = sks.GetNamespaceFromFile("..\\test.sks");
	PrintNamespaceContents(name, 0);
	cin.get();
	return 0;
}
void PrintNamespaceContents(Namespace *name, int tabLevel)
{	
	name->GetFirstIndex();

	if(name->GetName())
		cout << name->GetName() << endl;
	while(true)
	{
		// Makes indentations for easy recongnition of depth
		for(int i = 0;i < tabLevel;i++)
			cout << "  ";
		if(name->isNextIndexKeyValue())
		{
			KeyValue *val = name->GetNextKeyValue();
				
			// if the key value is null that mean that we have reached the last key value
			if(!val)
				return;

			// prints name and colon
			cout << val->GetName() << " : ";

			if(val->isInteger())
				cout << val->GetInteger() << endl;
			else if(val->isReal())
				cout << val->GetReal() << endl;
			else if(val->isString())
				cout << val->GetString() << endl;
		}
		else if(name->isNextIndexNamespace())
		{
			if(!name)
				return;
			PrintNamespaceContents(name->GetNextNamespace(), tabLevel + 1);
		}
		else
			return;
	}
}	