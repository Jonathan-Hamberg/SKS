#include "SKS.h"
#include <math.h>
#include <climits>

// Globals sks variable
SKS sks;

KeyValue::KeyValue()
{
	m_int = 0;
	m_real = 0;
	m_string = NULL;
	m_name = NULL;
}
KeyValue::KeyValue(int integer)
{
	m_int = integer;
}
KeyValue::KeyValue(float fl)
{
	m_real = fl;
}
KeyValue::KeyValue(char * ch)
{
	m_string = ch;
}
KeyValue::~KeyValue()
{
	// Delete string
	if(m_string)
		delete []m_string;
}
void KeyValue::SetName(char *name)
{
	m_name = name;
}
char *KeyValue::GetName()
{
	return m_name;
}
void KeyValue::SetInteger(int integer)
{
	m_int = integer;
}
int KeyValue::GetInteger()
{
	return m_int;
}
void KeyValue::SetString(char *ch)
{
	m_string = ch;
}
char *KeyValue::GetString()
{
	return m_string;
}
void KeyValue::SetReal(float fl)
{
	m_real = fl;
}
float KeyValue::GetReal()
{
	return m_real;
}
bool KeyValue::isInteger()
{
	return m_keyValueType == TYPE_INTEGER ? true : false;
}
bool KeyValue::isReal()
{
	return m_keyValueType == TYPE_REAL ? true : false;
}
bool KeyValue::isString()
{
	return m_keyValueType == TYPE_STRING ? true : false;
}
void KeyValue::SetKeyValueType(KEY_VALUE_TYPES type)
{
	m_keyValueType = type;
}
KEY_VALUE_TYPES KeyValue::GetKeyValueType()
{
	return m_keyValueType;
}

Namespace::Namespace()
{
	m_name = NULL;
	m_currentKeyValueIndex = 0;
	m_currentNamespaceIndex = 0;
}
Namespace::~Namespace()
{
}
void Namespace::SetName(char *name)
{
	m_name = name;
}
char *Namespace::GetName()
{
	return m_name;
}
void Namespace::AddKeyValue(KeyValue *val)
{
	// Inserts the new key value into the map
	keyValueHolder.push_back(val);
	// Stores data to tell that the index is a key value
	isIndexKeyValue.push_back(true);
}
void Namespace::AddNamespace(Namespace *namesp)
{
	namespaceHolder.push_back(namesp);
	// Stores data to tell that the index is not a key value
	isIndexKeyValue.push_back(false);
}
void Namespace::GetFirstIndex()
{
	m_currentKeyValueIndex = 0;
	m_currentNamespaceIndex = 0;
}
bool Namespace::isNextIndexKeyValue()
{
	// First make sure that we arn't requesting something that isn't there
	if(m_currentKeyValueIndex + m_currentNamespaceIndex >= isIndexKeyValue.size())
		return false;
	// Gets the current location in the namespace by adding that 
	//last know key value and namespace indexes
	return isIndexKeyValue[m_currentKeyValueIndex + m_currentNamespaceIndex];
}
bool Namespace::isNextIndexNamespace()
{
	// First make sure that we arn't requesting something that isn't there
	if(m_currentKeyValueIndex + m_currentNamespaceIndex >= isIndexKeyValue.size())
		return false;
	// Gets the current location in the namespace by adding the
	// last know key value and namespace indexes
	return !isIndexKeyValue[m_currentKeyValueIndex + m_currentNamespaceIndex];
}
KeyValue *Namespace::GetNextKeyValue()
{
	// Increments the last know key value location
	m_currentKeyValueIndex++;
	return keyValueHolder[m_currentKeyValueIndex - 1];
}
Namespace *Namespace::GetNextNamespace()
{
	//increments the last know namespace location
	m_currentNamespaceIndex++;
	return namespaceHolder[m_currentNamespaceIndex - 1];
}
KeyValue *Namespace::GetCurrentKeyValue()
{
	return keyValueHolder[m_currentKeyValueIndex];
}
Namespace *Namespace::GetCurrentNamespace()
{
	return namespaceHolder[m_currentNamespaceIndex];
}


SKS::SKS(void)
{
	m_fileContents = NULL;
}
SKS::~SKS(void)
{

}
char *SKS::GetVersion()
{
	return ".01 Alpha";
}
Namespace *SKS::GetNamespaceFromFile(const char *fileName)
{
	// Creates a stream to read the file from
	fstream sksFile(fileName, ios::in);

	// Breaks of the file couldn't be open
	if(sksFile.fail())
		return NULL;

	// Seek end of file
	sksFile.seekg(0, ios::end);

	// saves length of file
	m_lengthOfFile = (unsigned int)sksFile.tellg();

	// Seeks the beginning of the file
	sksFile.seekg(0, ios::beg);

	// Allocates memory for the file in memory
	m_fileContents = new char [m_lengthOfFile];

	// Reads the entire file
	sksFile.read(m_fileContents, m_lengthOfFile);

	sksFile.close();

	// Currents position in the file
	Position curPosition;

	// Creates the namespace that gets filled with the info from the file
	Namespace *topLevel;

	// Adds sets the namespace without a name
	topLevel = GetNamespace(curPosition, true);

	// Deletes the character buffer
	delete []m_fileContents;
	m_fileContents = NULL;

	return topLevel;
}
char *SKS::GetName(Position &pos)
{
	// This is the actual meat of the parser that looks for the identifier
	// name of the key value or namespace
	// it looks for a character string for the name until it hits a semi colon or left brace

	char * name = new char [MAX_STATEMENT];
	int namePosition = 0;

	// Gets rid of leading whitespace
	while(isspace(m_fileContents[pos.pos]))
		Advance(pos);

	bool errorMessage = false;
	// Read until hit space or : or newline
	while(m_fileContents[pos.pos] != ' ' && m_fileContents[pos.pos] != ':' &&
		m_fileContents[pos.pos] != '\n')
	{
		// only add to the name if it is currently under 255 characters
		if(namePosition < MAX_STATEMENT)
		{
			name[namePosition] = m_fileContents[pos.pos];
			namePosition++;
		}
		else
			errorMessage = true;
		Advance(pos);
	}

	// Add Null terminator toAtring
	name[namePosition] = NULL;

	if(errorMessage)
		cerr << name << " is not under " << MAX_STATEMENT << " characters long.\n";
	// Advance until one after the colon character or left brace
	while(m_fileContents[pos.pos] != ':' && m_fileContents[pos.pos] != '{')
		Advance(pos);
	Advance(pos);

	// Name of the key value or namespace
	return name;
}
// The first thing it get will be the key value
KeyValue *SKS::GetKeyValue(Position &pos)
{
	// Creates the key value to be sent back
	KeyValue *val = new KeyValue();

	// Position should be located one character after the colon
	val->SetName(GetName(pos));

	// Gets rid of leading whitespace
	while(isspace(m_fileContents[pos.pos]))
		Advance(pos);
	// One reaches first character determine if a string or some sort of real number
	if(m_fileContents[pos.pos] == '"')
	{
		// move position to the first value of the string
		Advance(pos);

		// Creates the string that will holst the value
		char * string = new char [MAX_STATEMENT];

		// loop until the end of the string
		int stringPos = 0;
		while(m_fileContents[pos.pos] != '"')
		{
			// Inserts any escape characters
			if(m_fileContents[pos.pos] == '\\')
			{
				Advance(pos);
				if(m_fileContents[pos.pos] == '"') // Quotes
					string[stringPos] = m_fileContents[pos.pos];
				else if(m_fileContents[pos.pos] == '\'') // Quote
					string[stringPos] = m_fileContents[pos.pos];
				else if(m_fileContents[pos.pos] == '\\')
					string[stringPos] = m_fileContents[pos.pos];
				else if(m_fileContents[pos.pos] == 'n') // Newline
					string[stringPos] = '\n';
				else
				{
					cerr << "Unknown escape character \'" <<
						m_fileContents[pos.pos] << "' Line: " << pos.line << endl;
					// Just put the character after the backslash
					// in anyways
					string[stringPos] = m_fileContents[pos.pos];
				}

				// Increments their position
				stringPos++;
				Advance(pos);
			}
			else
			{
				string[stringPos] = m_fileContents[pos.pos];
				stringPos++;
				Advance(pos);
			}
		}

		// Adds null terminator to end of string
		string[stringPos] = NULL;

		Advance(pos);  // Advances to the character after the ending quotes

		// Tells the key value what it is holding
		val->SetKeyValueType(TYPE_STRING);
		val->SetString(string);
		
	}
	else if(isdigit(m_fileContents[pos.pos]) || m_fileContents[pos.pos] == '-')
	{
		int integer = 0, fraction = 0;
		// string big enough to hold an 32 bit integer
		char num[11];
		
		bool isNegitive = false;
		if(m_fileContents[pos.pos] == '-')
		{
			// Tells that the number is negitive
			isNegitive = true;

			//Advanes to the actual first number
			Advance(pos);

			//Gets rid of any spaces that might be inbetween the negitive sign
			// and the start of the number
			while(isspace(m_fileContents[pos.pos]))
				Advance(pos);
		}

		// Read until a space is hit or a . is hit for a real number or the semi colon
		int i = 0;
		while(isdigit(m_fileContents[pos.pos]))
		{
			num[i++] = m_fileContents[pos.pos];
			Advance(pos);
			
			if(i >= 10) // Bigger number then the 2 billion of a 4 byte integer
			{
				cerr << "Integer Overflow. Line:  " << pos.line << endl;
				break;
			}
		}
		// Puts null terminator at the end of the string
		num[i] = 0;

		// figures out the integer value
		integer = isNegitive ? -atoi(num) : atoi(num);



		// if there is a fraction part to the number
		if(m_fileContents[pos.pos] == '.')
		{
			Advance(pos);
			i = 0;
			while(isdigit(m_fileContents[pos.pos]))
			{
				num[i++] = m_fileContents[pos.pos];

				Advance(pos);

				if(i >= 10) // Bigger number then the 2 billion of a 4 byte integer
				{
					// Prints error message
					cerr << "Integer Overflow. Line:  " << pos.line << endl;

					// Advances the position past the rest of the number
					while(isdigit(m_fileContents[pos.pos]))
						Advance(pos);
					break;
				}
			}
			// add null termintaor to string
			num[i] = 0;

			// figures out the fractal part
			fraction = atoi(num);

			//Tells the key value what it is holding
			val->SetKeyValueType(TYPE_REAL);
			if(isNegitive)
				val->SetReal(integer - fraction / (float)pow((double)10, (double)strlen(num)));
			else
				val->SetReal(integer + fraction / (float)pow((double)10, (double)strlen(num)));
		}
		else
		{
			// Tells the key value what it is holding
			val->SetKeyValueType(TYPE_INTEGER);
			// Update position to after the 
			val->SetInteger(integer);
		}
	}

	return val;
}
Namespace *SKS::GetNamespace(Position &pos, bool noName)
{
	Namespace *name = new Namespace();
	if(!noName)
		name->SetName(GetName(pos));

	while(true)
	{
		if(islineEndOfFile(pos))
		{
			break;
		}
		else if(isLineNamespaceEnd(pos))
		{
			ExitNamespace(pos);
			break;
		}
		else if(isLineNamespace(pos))
		{
			name->AddNamespace(GetNamespace(pos));
		}
		else if(isLineKeyValue(pos))
		{
			name->AddKeyValue(GetKeyValue(pos));
		}
		else
			break;
	}
	return name;
}
bool SKS::isLineNamespaceEnd(Position pos)
{
	// Get rid of leading whitespace and newlines
	while(isspace(m_fileContents[pos.pos]))
		Advance(pos);

	// it is the linespace end if the first character found is a right parentheses
	return m_fileContents[pos.pos] == '}' ? true : false;
}
void SKS::ExitNamespace(Position &pos)
{
	// Advances to the character after the right parentheses
	while(m_fileContents[pos.pos] != '}')
		Advance(pos);
	Advance(pos);
}
bool SKS::isLineKeyValue(Position pos)
{
	return !isLineNamespace(pos);
}
bool SKS::isLineNamespace(Position pos)
{
	bool foundName = false;

	// Gets rid of leading white spaces
	while(m_fileContents[pos.pos] == ' ')
		Advance(pos);

	// Gets past the name and value then check for the next key value or left brach
	while(m_fileContents[pos.pos] != ':' && m_fileContents[pos.pos] != '{')
		Advance(pos);

	// If left brace then it's a namespace
	if(m_fileContents[pos.pos] == '{')
		return true;
	return false; // if not left brance must be key value
}
bool SKS::islineEndOfFile(Position pos)
{
	// Advances until it has found a non whitespace or passes the length of the file
	while(pos.pos < m_lengthOfFile && isspace(m_fileContents[pos.pos]))
		Advance(pos);

	// return true if pos >= m_lenthOfFile
	if(pos.pos >= m_lengthOfFile)
		return true;
	return false;
}
// This function advances the index of the file and also
// keeps track of line number and colum
int SKS::Advance(Position &pos)
{
	pos.colum++;
	if(m_fileContents[pos.pos] == '\n')
	{
		pos.line++;
		pos.colum = 0;
	}
	return ++pos.pos;
}