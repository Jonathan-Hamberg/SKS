#ifndef _SKS_H
#define _SKS_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// Defines
const unsigned int MAX_STATEMENT = 255;

// Enumerations
enum KEY_VALUE_TYPES
{
	TYPE_INTEGER,
	TYPE_REAL,
	TYPE_STRING
};

struct Position
{
	int pos;
	int line;
	int colum;

	Position()
	{
		pos = 0;
		line = 1;
		colum = 0;
	}
};

class KeyValue
{
public:
	KeyValue();
	KeyValue(int);
	KeyValue(float);
	KeyValue(char *);
	~KeyValue();
	void SetString(char *);
	void SetInteger(int);
	void SetReal(float);
	char *GetString();
	int GetInteger();
	float GetReal();
	void SetName(char *);
	char *GetName();
	bool isInteger();
	bool isReal();
	bool isString();
	void SetKeyValueType(KEY_VALUE_TYPES type);
	KEY_VALUE_TYPES GetKeyValueType();

private:
	int m_int;
	float m_real;
	char * m_string;
	char *m_name;
	KEY_VALUE_TYPES m_keyValueType;
};
class Namespace
{
public:
	Namespace();
	~Namespace();
	void SetName(char *);
	char *GetName();
	void AddKeyValue(KeyValue *);
	void AddNamespace(Namespace *);
	void GetFirstIndex();
	bool isNextIndexKeyValue();
	bool isNextIndexNamespace();
	KeyValue *GetNextKeyValue();
	Namespace *GetNextNamespace();
private:
	// maps that the key value and Namespaces are held in
	 vector<Namespace *> namespaceHolder;
	 vector<KeyValue *> keyValueHolder;
	 char *m_name;  // Holds name until it can be put in a map

	 // Holds information for the order of key values and namespaces
	 vector<bool> isIndexKeyValue;
	 // Used to remember the last index for the GetNext functions
	 int m_currentKeyValueIndex;
	 int m_currentNamespaceIndex;
};
class SKS
{
public:
	SKS();
	~SKS();
	Namespace *GetNamespaceFromFile(const char *);
	char *GetVersion();
private:
	bool isLineKeyValue(Position pos);
	bool isLineNamespace(Position pos);
	bool isLineNamespaceEnd(Position pos);
	bool islineEndOfFile(Position pos);
	KeyValue *GetKeyValue(Position &pos);
	Namespace *GetNamespace(Position &pos, bool noName = false);
	char *GetName(Position &pos);
	void ExitNamespace(Position &pos);
	int Advance(Position &pos);
	char *m_fileContents;
	unsigned int m_lengthOfFile;
};


// Global variable extern
extern SKS sks;

#endif