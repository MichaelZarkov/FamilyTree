#pragma once
#include <fstream>
#include <vector>

enum class Animal { UNDEF = -1, DOG, CAT, HAMSTER };
enum class FurColour { UNDEF = -1, WHITE, BLACK, ORANGE, BROWN, GREY };

// Represents a single relative in the family tree.
struct Relative
{
	char name[32];
	Animal animalType;
	unsigned age;
	FurColour furColour;
	char maleOrFemale;         // 'M' - male, 'F' - female.

	int father;                  // Index of the father. -1 if it's unknown.
	int mother;                  // Index of the mother. -1 if it's unknown.
	std::vector<int> children;   // Indexes of all direct children.
};

std::ifstream& operator>>(std::ifstream& file, Relative& r);
std::ofstream& operator<<(std::ofstream& file, const Relative& r);