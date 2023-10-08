#pragma once
#include <vector>
#include <utility>
#include "Relative.h"

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

	const std::vector<Relative>& getWholeTree() const;

private:
	void readFromFile(const char* fileName);

private:
	std::vector<Relative> fTree;
};