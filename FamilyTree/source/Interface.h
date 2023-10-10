#pragma once
#include <utility>   // std::pair
#include <vector>
#include "FamilyTree.h"
#include "Relative.h"

/*
	This class:
	  - takes input from the console and displays information from a family tree.
*/
class Interface
{
public:
	Interface(const char* familyTreeFile);

private:
	void consoleInput();
	

	// Make it easier to read the function pointer syntax.
	typedef void (Interface::* PrintFunction)(const Relative&) const;

	bool printSameAge(const char* consoleInput, PrintFunction pf) const;

	void print
	(const std::vector<Relative>& toPrint, PrintFunction pf) const;
	void print
	(const std::vector<std::pair<Relative, unsigned>>& toPrint, PrintFunction pf) const;
	
	void printSimple(const Relative& toPrint) const;
	void printExtendedInfo(const Relative& toPrint) const;

	static void printColour(FurColour);
	static void printIntroText();
	static void printError();
	static void printInvalidArg();
	static void printCmdInfo();

private:
	enum class Commands;
	static Commands parceCmd(const char* consoleInput);

	bool setOutput(const char* consoleInput);

private:

	const FamilyTree fTree;

	// 1 - simple print with reduced info.
    // 2 - print more info. 
	int printFlag{ 1 };
	
	enum class Commands;
	static const char COMMANDS[][11];
};

