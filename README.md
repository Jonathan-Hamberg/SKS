Simplistic Keyvalues Scripting
This is basicly a configuration file with simple syntax that is easily acessable from code.
The files are made of namespaces and key values.  Key values hold one of three types of data, Integer, Real, String.
An integer will hold up to the max size of an in in most cases this is 2,147,483,647.  Namespaces can hold key values and other namespaces.
SKS follows a simple systax where everything has a name.  After the name can either be a colon then the value or a new namespace. 
holding the contentsIf overflow occures the integer will be set to the max value of the integer.  A real number is held by a double which it's max value is 1.7E +/- 308 (15 digits).  Like the integer if the double is overflowed it will be set to it's max value.
A string is a array of characters which is currently limited to 255 characters.  One important thing to note is that character string require escape characters to
be used for certian characters which are the end line, double quote, single quote, and the back slash.  They are used by putting a back slash and then putting a corrisponding
character.  Here is what each would look like.  Endline \n Double Quotes \" Single Quotes \' and a Back slash \\
Here is an example string with all the escape character in it. "Marry\n had \'a\' \"little Lamb\""
This is what the actual text would look like.
Marry
 had 'a' "Little Lamb"


Here is some example code for the SKS to read.
namespace
{
	myString : "myString";
	myInt : 123;
	myFloat : 123.455
}
