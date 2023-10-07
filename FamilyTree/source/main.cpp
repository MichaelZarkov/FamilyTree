

#include "FamilyTree.h"

int main()
{
	FamilyTree fTree("sampleTrees/familyTree.ftree");

	fTree.allChildren();
	fTree.allParents();
}