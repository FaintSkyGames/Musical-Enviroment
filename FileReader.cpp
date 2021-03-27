#include "FileReader.h"

#include<string>

#include <iostream>
#include <fstream>
using namespace std;

/**
* modify this procedure according to your own liking to test run your class
*/
void FileReader::runCustomCommands()
{
	//Your code here
}

FileReader::FileReader() {
	currentFileName = "";
	textFile = nullptr;
}


bool FileReader::openFile(std::string fileName) {
	if (storage.find(fileName) != storage.end())
	{
		// File exsists
		return false;
	}

	streams.push_back(ifstream(fileName.c_str()));
	storage[fileName] = std::to_string(streams.size() - 1);

	if (streams.back().is_open())
	{
		currentFileName = fileName;
		readFile(fileName);

		return true;
	}

	return false;
}

std::string FileReader::currentFile() {
	return currentFileName;
}

int FileReader::lineCount()
{
	string fileLength = currentFile() + "length";
	int count;

	try
	{
		count = stoi(storage[fileLength]);
	}
	catch (const std::exception&)
	{
		return 0;
	}

	return 	count;
}

std::string FileReader::getLine(int line)
{
	return storage[currentFile() + to_string(line)];
}

std::vector<std::string> FileReader::getLines(int start, int end)
{

	vector<string> lines;

	int numLines = lineCount();

	if (numLines == 0)
	{
		return lines;
	}

	if (end > numLines)
	{
		end = numLines - 1;
	}


	for (size_t i = start; i < end + 1; i++)
	{
		lines.push_back(getLine(i));
	}

	return lines;
}

std::vector<std::string> FileReader::inspectForFirst(std::string nodeType)
{
	string fileName = currentFile();
	int fileLength = stoi(storage[fileName + "length"]);

	int line = -1;
	bool endFound = false;
	size_t tempPos;
	size_t firstFoundPos;
	static const size_t npos = -1;

	vector<string> nodeLines;

	while (endFound == false && line != fileLength - 1)
	{
		line++;

		// Reset tempPos
		tempPos = npos;
		tempPos = storage[fileName + to_string(line)].find(nodeType);

		if (tempPos != npos)
		{
			firstFoundPos = tempPos;

			// Remove indentation and add to vector
			// -1 to include <
			nodeLines.push_back(storage[fileName + to_string(line)].substr(firstFoundPos - 1));

			// Reset tempPos
			tempPos = npos;

			// Check opening line for closing node
			tempPos = storage[fileName + to_string(line)].find(nodeType, firstFoundPos + 1);
			if (tempPos != npos)
			{
				endFound = true;
			}

			// If still searching for end
			while (endFound == false && line != fileLength - 1)
			{
				line++;

				// Reset tempPos
				tempPos = npos;
				tempPos = storage[fileName + to_string(line)].find(nodeType);

				nodeLines.push_back(storage[fileName + to_string(line)].substr(firstFoundPos - 1));

				if (tempPos != npos)
				{
					endFound = true;
				}
			}
		}
	}

	return nodeLines;
}

std::vector<std::string> FileReader::inspectForFirst(std::string nodeType, std::string subNodeType)
{
	string fileName = currentFile();
	int fileLength = stoi(storage[fileName + "length"]);

	int line = -1;
	bool endFound = false;
	size_t tempPos;
	size_t firstFoundPos;
	static const size_t npos = -1;

	vector<string> nodeLines;

	while (endFound == false && line != fileLength - 1)
	{
		line++;

		// Reset tempPos
		tempPos = npos;
		tempPos = storage[fileName + to_string(line)].find(nodeType);

		// Node Found
		if (tempPos != npos)
		{
			while (endFound == false && line != fileLength - 1)
			{
				line++;

				// Reset tempPos
				tempPos = npos;
				tempPos = storage[fileName + to_string(line)].find(subNodeType);

				if (tempPos != npos) {
					firstFoundPos = tempPos;

					// Remove indentation and add to vector
					// -1 to include <
					nodeLines.push_back(storage[fileName + to_string(line)].substr(firstFoundPos - 1));

					// Reset tempPos
					tempPos = npos;

					// Check line for closing node
					tempPos = storage[fileName + to_string(line)].find(subNodeType, firstFoundPos + 1);
					if (tempPos != npos)
					{
						endFound = true;
					}

					// If still searching for end
					while (endFound == false && line != fileLength - 1)
					{
						line++;

						// Reset tempPos
						tempPos = npos;
						tempPos = storage[fileName + to_string(line)].find(subNodeType);

						nodeLines.push_back(storage[fileName + to_string(line)].substr(firstFoundPos - 1));

						if (tempPos != npos)
						{
							endFound = true;
						}
					}
				}
			}
		}
	}
	return nodeLines;
}

std::string FileReader::inspectForFirstAttribute(std::string nodeType, std::string attrib)
{
	bool endFound = false;
	int line = -1;
	static const size_t npos = -1;
	size_t startPosition = npos;
	int endPosition = -1;
	string details = "";

	vector<string> nodeLines = inspectForFirst(nodeType);

	while (endFound == false)
	{
		line++;
		startPosition = nodeLines[line].find(attrib);
		if (startPosition != npos)
		{
			startPosition = startPosition + attrib.length() + 2; // include ="
			endPosition = startPosition - 1;
			while (endFound == false && endPosition != nodeLines[line].length())
			{
				endPosition++;
				if (nodeLines[line][endPosition] == '"')
				{
					endFound = true;
				}
			}
		}
	}

	return nodeLines[line].substr(startPosition, endPosition - startPosition);
}

std::string FileReader::inspectForFirstAttribute(std::string nodeType, std::string subNodeType, std::string attrib)
{
	bool endFound = false;
	int line = -1;
	static const size_t npos = -1;
	size_t startPosition = npos;
	int endPosition = -1;
	string details = "";

	vector<string> nodeLines = inspectForFirst(nodeType, subNodeType);

	while (endFound == false)
	{
		line++;
		startPosition = nodeLines[line].find(attrib);
		if (startPosition != npos)
		{
			startPosition = startPosition + attrib.length() + 2; // include ="
			endPosition = startPosition - 1;
			while (endFound == false && endPosition != nodeLines[line].length())
			{
				endPosition++;
				if (nodeLines[line][endPosition] == '"')
				{
					endFound = true;
				}
			}
		}
	}

	return nodeLines[line].substr(startPosition, endPosition - startPosition);
}

void FileReader::printDataToCMD(std::string* lines)
{
	cout << lines;
}

bool FileReader::readFile(std::string fileName) {

	// Check file is open and exsists
	try
	{
		if (!streams.at(stoi(storage[fileName])).is_open())
		{
			return false;
		}
	}
	catch (const std::exception&)
	{
		return false;
	}

	currentFileName = fileName;

	int numLines = -1;

	streams.at(stoi(storage[fileName])).clear();  // Clears the end of file flag
	streams.at(stoi(storage[fileName])).seekg(0); // Sets line pointer back to 0

	while (streams.at(stoi(storage[fileName])))
	{
		numLines++;
		getline(streams.at(stoi(storage[fileName])), storage[fileName + to_string(numLines)]);
	}

	string fileLength = fileName + "length";
	storage[fileLength] = to_string(numLines);

	return true;
}

bool FileReader::overrideCurrentFile(std::string fileName, std::string* lines) {

	//Your code here
	// You also need to modify the return value
	return false;
}
bool FileReader::closeFile(std::string fileName) {
	if (fileName == currentFile())
	{
		return closeCurrentFile();
	}
	if (storage.find(fileName) != storage.end())
	{
		// File exsists
		streams.at(stoi(storage[fileName])).close();

		if (streams.at(stoi(storage[fileName])).is_open())
		{
			return false;
		}


		for (size_t i = 0; i <= stoi(storage[fileName + "length"]); i++)
		{
			storage.erase(fileName + to_string(i));
		}

		storage.erase(fileName + "length");
		storage.erase(fileName);

		return true;
	}

	return false;
}
bool FileReader::closeCurrentFile()
{
	if (currentFile() == "")
	{
		return false;
	}

	streams.at(stoi(storage[currentFile()])).close();

	if (streams.at(stoi(storage[currentFile()])).is_open())
	{
		return false;
	}

	// Clear file data
	for (size_t i = 0; i <= stoi(storage[currentFile() + "length"]); i++)
	{
		storage.erase(currentFile() + to_string(i));
	}

	storage.erase(currentFile() + "length");

	storage.erase(currentFile());


	if (storage.size() == 0)
	{
		currentFileName.clear();
	}
	else {
		map<string, string> streamIndexs;

		// Find all stream indexs
		for (map<string, string>::iterator i = storage.begin(); i != storage.end(); i++)
		{
			// If anything appends the file name it does not contain a file stream index
			if (i->first.substr(0, i->first.find(".dae") + 4) == i->first)
			{
				if (i->second != "")
				{
					streamIndexs[i->first] = i->second;
				}
			}
		}

		if (!streamIndexs.empty())
		{
			map<string, string>::iterator it = streamIndexs.begin();
			bool newStreamFound = false;
			while (it != streamIndexs.end() && newStreamFound == false)
			{
				try
				{
					if (streams.at(stoi(it->second)).is_open())
					{
						currentFileName = it->first;
						newStreamFound = true;
					}
				}
				catch (const std::exception&)
				{

				}
				it++;
			}
		}
		else
		{
			currentFileName.clear();
		}

	}

	return true;
}

// Your additional Code om implemeting the FileReader here
