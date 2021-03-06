#ifndef _SKS_H
#define _SKS_H

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
	KeyValue *GetKeyValue(int index);
	KeyValue *GetKeyValue(char *key);
	Namespace *GetNamespace(int index);
	bool isIndexKeyValue(int index);
	bool isIndexNamespace(int index);
	bool isIndexEnd(int index);
	void SortIndexes();
private:
	// maps that the key value and Namespaces are held in
	 vector<Namespace *> namespaceHolder;
	 vector<KeyValue *> keyValueHolder;
	 vector<int> sortedNamespace;
	 vector<int> sortedKeyValue;
	 char *m_name;  // Holds name until it can be put in a map

	 // Holds information for the order of key values and namespaces
	 vector<bool> keyValueIndex;
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
	void PrintNamespace(Namespace *name, int indent = 0);
	void PrintKeyValue(KeyValue *, int indent = 0);
	char *GetVersion();
private:
	void NextStatement(Position &pos);
	void ExitNamespace(Position &pos);
	void Next(Position &pos, bool skipComment = true);
	void SkipComment(Position &pos);
	bool isLineKeyValue(Position pos);
	bool isLineNamespace(Position pos);
	bool isLineNamespaceEnd(Position pos);
	bool islineEndOfFile(Position pos);
	KeyValue *GetKeyValue(Position &pos);
	Namespace *GetNamespace(Position &pos, bool noName = false);
	char *GetName(Position &pos);

	char *m_fileContents;
	unsigned int m_lengthOfFile;
};


// Global variable extern
extern SKS sks;

#endif