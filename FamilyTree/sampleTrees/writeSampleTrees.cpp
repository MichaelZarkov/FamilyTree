
#include <cstring>
#include <fstream>
#include <vector>
#include "../source/Relative.h"

#pragma warning (disable : 4996)

/*
	This function generates and returns the following family tree as a vector:
	Note: It is not actually a "tree" by definition. It is a DAG (directed acyclic graph).
	      Why is a family tree always a DAG: because if it wasn't there would be someone who is a parent of themselves,
		  which is a contradiction.
	---------------------------------------------------------------------------------------------------------------
		The graph where the edges are poiners to parents:
		  - 0 is the node of my pet.
		  - the direction of all edges is from bottom to top.
		  - if connected with an edge, a higher node is a parent of a lower node.
		  - fathers are to the left and mothers are to the right.
			
			     4
				 |\       __--5
				 | \     /   /|
				 |  \	7	/ |
			     |   \	   1  |  
				  \   \   /   |
				   6   \ /    |
				   |	0     |
				   |   /     /
					\ /     /
					 2     /
					  \   /
					   \ /
						3

		In this tree the parent relationships are as follows (father == F, mother == M, unknown == ?):
		  - 0: F4, M1
		  - 1: F?, M5
		  - 2: F6, M0
		  - 3: F2, M5
		  - 4: F?, M?
		  - 5: F?, M?
		  - 6: F4, M?
		  - 7: F?, M5
		Note: There is some incest going on but that is due to the small example. In reality the relations would be
		      similar just spaced out over more generations.
	---------------------------------------------------------------------------------------------------------------
		The tree where the edges are pointers to children:
		  - 0 is the node of my pet.
		  - the direction of all edges is from top to bottom.
		  - if connected with an edge a lower node is a child of a higher node.
		This graph is the same as the previous, where just the direction of the edges is reversed (and it is also a DAG).
*/
std::vector<Relative> generateSampleTree()
{
	std::vector<Relative> T(8);    // Family tree.

	for (size_t i = 0; i < T.size(); ++i)
		T[i].animalType = Animal::DOG;   // Init animal type.
	
	// Init name, age, fur colour and male or female.
	strcpy(T[0].name, "Doggo");  T[0].age = 5;  T[0].furColour = FurColour::BLACK;  T[0].maleOrFemale = 'F';
	strcpy(T[1].name, "Mika");   T[1].age = 6;  T[1].furColour = FurColour::BLACK;  T[1].maleOrFemale = 'F';
	strcpy(T[2].name, "Mori");   T[2].age = 3;  T[2].furColour = FurColour::ORANGE; T[2].maleOrFemale = 'M';
	strcpy(T[3].name, "Peso");   T[3].age = 1;  T[3].furColour = FurColour::WHITE;  T[3].maleOrFemale = 'M';
	strcpy(T[4].name, "Alan");   T[4].age = 6;  T[4].furColour = FurColour::BROWN;  T[4].maleOrFemale = 'M';
	strcpy(T[5].name, "Alla");   T[5].age = 10; T[5].furColour = FurColour::BROWN;  T[5].maleOrFemale = 'F';
	strcpy(T[6].name, "Kiko");   T[6].age = 5;  T[6].furColour = FurColour::GREY;   T[6].maleOrFemale = 'M';
	strcpy(T[7].name, "Lana");   T[7].age = 4;  T[7].furColour = FurColour::BLACK;  T[7].maleOrFemale = 'F';

	// Init parent-child relations. -1 means unknown parent(parent not in the family tree).
	T[0].father =  4;  T[0].mother =  1;
	T[1].father = -1;  T[1].mother =  5;
	T[2].father =  6;  T[2].mother =  0;
	T[3].father =  2;  T[3].mother =  5;
	T[4].father = -1;  T[4].mother = -1;
	T[5].father = -1;  T[5].mother = -1;
	T[6].father =  4;  T[6].mother = -1;
	T[7].father = -1;  T[7].mother =  5;

	T[0].children.resize(1);  T[0].children[0] = 2;
	T[1].children.resize(1);  T[1].children[0] = 0;
	T[2].children.resize(1);  T[2].children[0] = 3;
	T[4].children.resize(2);  T[4].children[0] = 0;    T[4].children[1] = 6;
	T[5].children.resize(3);  T[5].children[0] = 1;    T[5].children[1] = 3;  T[5].children[2] = 7;
	T[6].children.resize(1);  T[6].children[0] = 2;

	return T;
}
void saveTreeToFile(const std::vector<Relative>& T, const char* fileName)
{
	std::ofstream file(fileName, std::ios::binary);
	if (!file.is_open()) { throw "Cannot save family tree to file! Filepath if wrong!"; }

	size_t relativesCount = T.size();
	file.write((const char*)&relativesCount, sizeof(relativesCount));
	for (size_t i = 0; i < T.size(); ++i)
	{
		file << T[i];
	}

	if (!file.good()) { throw "Error writing to file! File may be corrupted!"; }

	file.close();
}