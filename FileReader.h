#ifndef FILEREADER_H
#define FILEREADER_H
#include <iostream>
#include <fstream>

#include <map>

//#include "FileReader.cpp"

#ifdef __unix__         


#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) 


#endif

#include <vector>

class FileReader
{
private:
	std::string currentFileName;
	std::string* textFile;
	std::vector<std::ifstream> streams;
	std::map<std::string, std::string> storage;

public:
	FileReader();

	void runCustomCommands();
	bool openFile(std::string fileName);
	bool readFile(std::string fileName);
	bool closeFile(std::string fileName);
	bool closeCurrentFile();

	std::string currentFile();
	int lineCount();
	std::string getLine(int line);
	std::vector<std::string> getLines(int start, int end);
	std::vector<std::string> inspectForFirst(std::string nodeType);
	std::vector<std::string> inspectForFirst(std::string nodeType, std::string subNodeType);
	std::string inspectForFirstAttribute(std::string nodeType, std::string attrib);
	std::string inspectForFirstAttribute(std::string nodeType, std::string subNodeType, std::string attrib);
	void printDataToCMD(std::string* lines);
	bool overrideCurrentFile(std::string fileName, std::string* lines);
};
#endif