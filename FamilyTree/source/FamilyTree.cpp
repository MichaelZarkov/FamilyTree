
#include <fstream>
#include <queue>
#include <utility>
#include "FamilyTree.h"

FamilyTree::FamilyTree(const char* fileName)
{
	readFromFile(fileName);
}

// By default the first element in 'fTree' is considered the main pet.
const Relative& FamilyTree::myPet() const
{
	return fTree.front();
}
// Returns pointer to the father of the main pet.
// Returns nullptr if the father is unknown (not in the family tree).
const Relative* FamilyTree::father() const
{
	if (fTree.front().father != -1)
	{
		return &fTree[fTree.front().father];
	}
	else
	{
		return nullptr;
	}
}
// Returns pointer to the mother of the main pet.
// Returns nullptr if the mother is unknown (not in the family tree).
const Relative* FamilyTree::mother() const
{
	if (fTree.front().mother != -1)
	{
		return &fTree[fTree.front().mother];
	}
	else
	{
		return nullptr;
	}
}

// Returns only the direct children of the main pet (so without grandchildren).
/*
	Example:
				0
			   /|\
			  1 2 3
			 /     \
			6	    4
				   /
				  5
		In the given tree 0 is the main pet.
		The function will return 1,2,3 (only the direct children).
*/
std::vector<Relative> FamilyTree::directChildren() const
{
	std::vector<Relative> dirChildren(fTree.front().children.size());
	for (size_t i = 0; i < dirChildren.size(); ++i)
	{
		dirChildren[i] = fTree[fTree.front().children[i]];
	}
	return dirChildren;
}
// Returns all children of the main pet.
// Also returns the generation difference between the main pet and every child.
// The function basically performs BFS (breadth first search).
/*
	Example:
				0
			   /|\
			  1 2 3
			 /     \
			6	    4
				   /
				  5
		In the given tree 0 is the main pet.
		The function will return 1,2,3,6,4,5 (all descendants).
		  - The generation difference of 1,2,3 is 1.
		  - The generation difference of 4,6 is 2.
		  - The generation difference of 5 is 3.
*/
std::vector<std::pair<Relative, unsigned>> FamilyTree::allChildren() const
{
	std::vector<std::pair<Relative, unsigned>> descendants;   // Array to be returned.
	if (fTree.front().children.empty())
		return descendants;

	// The following is basically BFS.
	std::queue<std::pair<unsigned, unsigned>> Q;  // <index, genDiff>. Queue of the indexes of the pets and generation difference.
	std::vector<bool> visited(fTree.size());      // True if a node(pet) is already visited.

	for (size_t i = 0; i < fTree.front().children.size(); ++i)  // Put all direct children in the queue.
	{
		Q.emplace(fTree.front().children[i], 1);    // Generation difference is one.
		visited[fTree.front().children[i]] = true;
	}
		
	while (!Q.empty())
	{
		std::pair<unsigned, unsigned> current = Q.front();
		Q.pop();
		descendants.push_back({ fTree[current.first], current.second });  // Put current in the final array.
		
		// Put children of current in queue.
		for (size_t i = 0; i < fTree[current.first].children.size(); ++i)
			if (!visited[fTree[current.first].children[i]])
			{
				Q.emplace(fTree[current.first].children[i], current.second + 1);
				visited[fTree[current.first].children[i]] = true;
			}
	}
	
	return descendants;
}
// Does the same as 'allChildren()' but for the parents of the main pet.
/*
	Example:
		   6   5 
			\ /
		 4	 3
		  \   \
		   1   2
			\ /
			 0
		If 0 is the main pet and a higher node is a parent the function will return 1,2,4,3,6,5.
		  - The generation difference of 1,2 is 1
		  - The generation difference of 3,4 is 2
		  - The generation difference of 5,6 is 3
*/
std::vector<std::pair<Relative, unsigned>> FamilyTree::allParents() const
{
	std::vector<std::pair<Relative, unsigned>> predecessors;      // Array to be returned.
	if (fTree.front().father == -1 && fTree.front().mother == -1)
		return predecessors;

	// The following is basically BFS.
	std::queue<std::pair<unsigned, unsigned>> Q;  // <index, genDiff>. Queue of the indexes of the pets and generation difference.
	std::vector<bool> visited(fTree.size());      // True if a node(pet) is already visited.

	// Put first parents in the queue.
	if (fTree.front().father != -1)
	{
		Q.emplace(fTree.front().father, 1);
		visited[fTree.front().father] = true;
	}
	if (fTree.front().mother != -1)
	{
		Q.emplace(fTree.front().mother, 1);
		visited[fTree.front().mother] = true;
	}

	while (!Q.empty())
	{
		std::pair<unsigned, unsigned> current = Q.front();
		Q.pop();
		predecessors.push_back({ fTree[current.first], current.second });  // Put current in the final array.

		// Put parents of current in queue.
		if (fTree[current.first].father != -1 && !visited[fTree[current.first].father])
		{
			Q.emplace(fTree[current.first].father, current.second + 1);
			visited[fTree[current.first].father] = true;
		}
		if (fTree[current.first].mother != -1 && !visited[fTree[current.first].mother])
		{
			Q.emplace(fTree[current.first].mother, current.second + 1);
			visited[fTree[current.first].mother] = true;
		}
	}

	return predecessors;
}
// Returns all pets in the family tree with the given age.
std::vector<Relative> FamilyTree::sameAge(unsigned age) const
{
	std::vector<Relative> sameAgePets;
	for (size_t i = 0; i < fTree.size(); ++i)
		if (fTree[i].age == age)
			sameAgePets.push_back(fTree[i]);
	
	return sameAgePets;
}
// Returns all pets in the family tree with the given fur colour. 
std::vector<Relative> FamilyTree::sameColour(FurColour colour) const
{
	std::vector<Relative> sameFurPets;
	for (size_t i = 0; i < fTree.size(); ++i)
		if (fTree[i].furColour == colour)
			sameFurPets.push_back(fTree[i]);

	return sameFurPets;
}

size_t FamilyTree::relativesCount() const { return fTree.size(); }
const std::vector<Relative>& FamilyTree::getWholeTree() const
{
	return fTree;
}

/*
	Reads a file of the following format:
	  - 8 byte unsigned int - Relatives count 'R'.
	  - 'R' number of Relatives.
	The file must contain at least one relative otherwise exception is thrown!
*/
// Exceptions: const char* if file does not exist or error during reading.
void FamilyTree::readFromFile(const char* fileName)
{
	std::ifstream file(fileName, std::ios::binary);
	if (!file.is_open())
		throw "Invalid file name! File doesn't exist!";

	size_t relativeCount = 0;
	file.read((char*)&relativeCount, sizeof(size_t));  // Read how many relatives.
	if (relativeCount == 0) { throw "Family tree is empty! The family tree must have at least one relative!"; }
	fTree.resize(relativeCount);

	for (size_t i = 0; i < relativeCount && file.good(); ++i)   // Read all relatives.
		file >> fTree[i];

	if (!file.good()) { throw "Error reading from file! File may be invalid!"; }

	file.close();
}
