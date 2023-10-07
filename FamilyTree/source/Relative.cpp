
#include "Relative.h"

std::ifstream& operator>>(std::ifstream& file, Relative& rel)
{
	if (!file.is_open()) { throw "No file is opened!";}
	if (!file.good())    { throw "Error reading from file!"; }

	file.read((char*)&rel, sizeof(Relative) - sizeof(rel.children));  // Read relative without children vector.
	size_t childrenCount;
	file.read((char*)&childrenCount, sizeof(childrenCount));
	rel.children.resize(childrenCount);
	if (childrenCount != 0)
		file.read((char*)&rel.children.front(), sizeof(int) * childrenCount);  // Read the array of children.

	if (!file.good()) { throw "Error reading from file! File may be invalid!"; }

	return file;
}
std::ofstream& operator<<(std::ofstream& file, const Relative& rel)
{
	if (!file.is_open()) { throw "No file is opened!"; }
	if (!file.good())    { throw "Error writing to file!"; }

	file.write((const char*)&rel, sizeof(Relative) - sizeof(rel.children));    // Write relative without children vector.
	size_t childrenCount = rel.children.size();
	file.write((const char*)&childrenCount, sizeof(childrenCount));            // Write number of children.
	if (rel.children.size() != 0)
		file.write((const char*)&rel.children.front(), sizeof(int) * rel.children.size()); // Write the children.

	if (!file.good()) { throw "Error writing to file! File may be corrupted!"; }

	return file;
}