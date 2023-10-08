#pragma once
#include <vector>
#include <utility>
#include "Relative.h"

/*
	This class:
	  - loads a family tree from file and allows you to get some information about it (how many relatives,
	    mother, father, relatives with a particular trait an so on). The tree must have at leat one pet.
	    A family tree is just an array of relatives.
	  - does not edit the family tree (only returns info).
	  - works even if the tree is invalid (has circular relations like if a pet is a parent of itself) but
	    no guarantees are given.
	  - by default "the main pet/your pet" is always the first first pet in the vector and the info given
	    (by some of the functions) is relative to it. (Note: without much trouble, functionality to get info
		relative to other pets from the tree can be added.)
*/
class FamilyTree
{
public:
	FamilyTree(const char* fileName);

public:
	const Relative& myPet() const;
	const Relative* father() const;
	const Relative* mother() const;

	std::vector<Relative> directChildren() const;
	std::vector<std::pair<Relative, unsigned>> allChildren() const;
	std::vector<std::pair<Relative, unsigned>> allParents() const;
	std::vector<Relative> sameAge(unsigned age) const;
	std::vector<Relative> sameColour(FurColour colour) const;

	size_t relativesCount() const;
	const std::vector<Relative>& getWholeTree() const;

private:
	void readFromFile(const char* fileName);

private:
	std::vector<Relative> fTree;
};