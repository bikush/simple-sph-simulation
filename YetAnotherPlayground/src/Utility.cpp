#include "Utility.h"
#include <iostream>
#include <stdlib.h>
#include <iomanip>

using namespace std;



vector<string> split( const char* line, char splitter )
{
	return split( string(line), splitter );
}

vector<string> split( string line, char splitter )
{
	string L = line;
	
	string::iterator start = L.begin();
	string::iterator end = start;
	vector<string> output = vector<string>();	
	while(end != L.end())
	{		
		if(*end == splitter)
		{
			if(end != start) output.push_back(string( start, end ));
			start = end+1;
			end = start;
		}else{
			end++;
		}
	}
	if(end != start) output.push_back(string( start, end ));
	return output;
}



string join( const vector<string>& parts, string joiner )
{
	if( parts.size() == 0 )
		return "";

	string outString( *parts.begin() );
	for (auto element = parts.begin() + 1; element != parts.end(); element++)
	{
		outString += joiner;
		outString += *element;
	}

	return outString;
}

int countChar( const char* line, char c )
{
	int i=0;
	int count=0;
	while( line[i]!='\0' )
	{
		if(line[i]==c) count++;
		i++;
	}
	return count;
}

bool equals( std::string first, std::string second )
{
	return first.compare( second ) == 0;
}

string getFilePathPart( const string& fileName )
{
	string filePath;

	int lastSlash = fileName.rfind('/');
	if( lastSlash == string::npos )
	{
		filePath = string("./");
	}else
	{
		filePath = string( fileName, 0, lastSlash+1);
	}

	return filePath;
}

string getFileExtension( const string& fileName )
{
	string extension = "";

	int lastDot = fileName.rfind('.')+1;
	if( lastDot != string::npos )
	{
		extension = string( fileName, lastDot, fileName.length() - lastDot );
	}

	return extension;
}


void loadFile( std::string& output, const char* filePath )
{
	ifstream file( filePath, ios::binary );	
	output.clear();

	//file.seekg(0, std::ios::end);   
	//output.reserve(file.tellg());
	//file.seekg(0, std::ios::beg);

	output.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();
}

void print(const glm::mat4& m)
{
	for (int i = 0; i < 4; i++ )
	{
		cout << setw(10) << setprecision(2) << fixed << "\t" << m[i][0] << "\t" << m[i][1] << "\t" << m[i][2] << "\t" << m[i][3] << endl;
	}	
}