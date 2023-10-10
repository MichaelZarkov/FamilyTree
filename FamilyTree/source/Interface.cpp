
#include <cstdlib>  // 'atoi()'
#include <cstring>
#include <functional>  // std::invoke
#include <iostream>
#include "FamilyTree.h"
#include "Interface.h"


// The elements of 'Commands' and 'COMMANDS' must be in the same positions (some functions depend on that).
// For example if command 'Commands::cmd_1' is in position 3, then the corresponding string "cmd_1_string" from
// 'COMMANDS', must also be at position 3.
enum class Interface::Commands
{
	UNDEF = -1,

	my_pet,
	father,
	mother,
	dir_chld,
	all_chld,
	all_par,
	at_age,
	same_col,
	count,
	get_all,

	set_output,
	cmd_info,
	exit
};
const char Interface::COMMANDS[][11]
{
	"my_pet",
	"father",
	"mother",
	"dir_chld",
	"all_chld",
	"all_par",
	"at_age",
	"same_col",
	"count",
	"get_all",

	"set_output",
	"cmd_info",
	"exit"
};


Interface::Interface(const char* familyTreeFile) : fTree(familyTreeFile)
{
	consoleInput();
}

void Interface::consoleInput()
{
	printIntroText();

	while (true)
	{
		std::cout << "> ";
		char buff[32];
		std::cin.getline(buff, 32);
		Commands cmd = parceCmd(buff);

		// Choose the print function to use depending on the 'printFlag'.
		PrintFunction pf = &Interface::printSimple;
		if (printFlag == 2) { pf = &Interface::printExtendedInfo; }

		switch (cmd)
		{
		case Commands::UNDEF:
			printError();
			break;
		case Commands::my_pet:
			(this->*pf)(fTree.myPet());
			break;
		case Commands::father:
			if (fTree.father()) { (this->*pf)(*fTree.father()); }
			else { std::cout << "Father is unknown.\n"; };
			break;
		case Commands::mother:
			if (fTree.mother()) { (this->*pf)(*fTree.mother()); }
			else { std::cout << "Mother is unknown.\n"; }
			break;
		case Commands::dir_chld:
			if (fTree.directChildren().size()) { print(fTree.directChildren(), pf); }
			else { std::cout << "The pet has no children.\n"; }
			break;
		case Commands::all_chld:
			{
				// Block because "you cannot jump past a declaration with an initializer
				// unless the declaration is enclosed in a block".
				auto allChld = fTree.allChildren();
				if (allChld.size()) { print(allChld, pf); }
				else { std::cout << "The pet has no children.\n"; }
			}
			break;
		case Commands::all_par:
			{
				auto allPar = fTree.allParents();
				if (allPar.size()) { print(allPar, pf); }
				else { std::cout << "Parents are unknown.\n"; }
			}
			break;
		case Commands::at_age:
			if (!printSameAge(buff, pf))
				printInvalidArg();
			break;
		case Commands::same_col:
			{
				std::vector<Relative> sameCol = fTree.sameColour(fTree.myPet().furColour);
				if (sameCol.size()) { print(sameCol, pf); }
				else { std::cout << "No pets with the same colour as my pet.\n"; }
			}
			break;
		case Commands::count:
			std::cout << fTree.relativesCount() << '\n';
			break;
		case Commands::get_all:
			print(fTree.getWholeTree(), pf);
			break;
		case Commands::set_output:
			if (!setOutput(buff))
				printInvalidArg();
			else
				std::cout << "Output was set.\n";
			break;
		case Commands::cmd_info:
			printCmdInfo();
			break;
		case Commands::exit:
			std::cout << "Closing the program...";
			return;
		}
	}
}

// If the argument of 'input' is a positive integer prints the pets with the given age and returns true.
// Otherwise Doesn't print anything and returns false.
// Ex: input == "same_age 4", prints and returns true.
//     input == "same_age -9", doesn't print and returns false.
bool Interface::printSameAge(const char* input, PrintFunction pf) const
{
	// Find the positin of the argument.
	const char* argument = input + (strlen(COMMANDS[(unsigned)Commands::at_age]));
	int petAge = atoi(argument);
	if (petAge <= 0)
		return false;

	std::vector<Relative> petsTheSameAge = fTree.sameAge(petAge);
	if (petsTheSameAge.size())
		print(petsTheSameAge, pf);
	else
		std::cout << "There are no pets with this age.\n";
	
	return true;
}

void Interface::print
(const std::vector<Relative>& toPrint, PrintFunction pf) const
{
	int i = 1;
	for (const Relative& rel : toPrint)
	{
		std::cout << '\t' << i++ << ".\t";
		(this->*pf)(rel);
	}
}
void Interface::print
(const std::vector<std::pair<Relative, unsigned>>& toPrint, PrintFunction pf) const
{
	int i = 1;
	for (const std::pair<Relative,unsigned>& rel : toPrint)
	{
		std::cout << '\t' << i++ << ".\t";
		(this->*pf)(rel.first);
		std::cout << "\t\tGenerations apart: " << rel.second << '\n';
	}
}

void Interface::printSimple(const Relative& toPrint) const
{
	std::cout << "name: " << toPrint.name << '\n';
}
void Interface::printExtendedInfo(const Relative& toPrint) const
{
	std::cout << "Name: " << toPrint.name << " | "
		<< "Age: " << toPrint.age << " | "
		<< "Fur colour: ";  printColour(toPrint.furColour);
	
	std::cout << " | " << "Father: " << (toPrint.father == -1 ? "NA" : fTree.getWholeTree()[toPrint.father].name);
	std::cout << " | " << "Mother: " << (toPrint.mother == -1 ? "NA" : fTree.getWholeTree()[toPrint.mother].name);
	std::cout << std::endl;
}

void Interface::printColour(FurColour c)
{
	switch (c)
	{
	case FurColour::WHITE:        std::cout << "white";       return;
	case FurColour::BLACK:        std::cout << "black";       return;
	case FurColour::ORANGE:       std::cout << "orange";      return;
	case FurColour::BROWN:        std::cout << "brown";       return;
	case FurColour::GREY:         std::cout << "grey";        return;
	case FurColour::DARK_BROWN:   std::cout << "dark brown";  return;
	default:                      std::cout << "NA";          return;
	}
}

void Interface::printIntroText()
{
	std::cout << "Display information from a pet family tree.\n"
		<< "Type '" << COMMANDS[(unsigned)Commands::cmd_info] << "' to see available commands.\n";
}
void Interface::printError()
{
	std::cout << "Wrong command! Try again.\n";
}
void Interface::printInvalidArg()
{
	std::cout << "Invalid argument! Try again.\n";
}
void Interface::printCmdInfo()
{
	std::cout << "All commands:\n";
	int i = 1;
	for (const char* cmd : COMMANDS)
	{
		std::cout << "\t" << i++ << ".\t" << cmd << '\n';
	}
}


Interface::Commands Interface::parceCmd(const char* input)
{
	int command = 0;
	for (const char* commandString : COMMANDS)
	{
		if (strncmp(input, commandString, strlen(commandString)) == 0)
		{
			return (Commands)command;
		}
		++command;
	}

	return Commands::UNDEF;
}

// Sets 'printFlag' and returns true if the argument is valid flag value.
// Doesn't change 'printFlag' and returns false if the argument is not valid.
bool Interface::setOutput(const char* input)
{
	// Find the positin of the argument.
	const char* argument = input + (strlen(COMMANDS[(unsigned)Commands::set_output]));
	int newFlag = atoi(argument);
	switch (newFlag)
	{
	case 1:
	case 2:
		printFlag = newFlag;
		return true;
	default:
		return false;
	}
}