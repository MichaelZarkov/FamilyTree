#include "pch.h"
#include "CppUnitTest.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <utility>
#include <vector>

#include "../FamilyTree/source/FamilyTree.h"
// This can't be a good solution but without the bellow two includes the linker gives an error,
// even though project 'Tests' has a reference to project 'FamilyTree'.
#include "../FamilyTree/source/FamilyTree.cpp"
#include "../FamilyTree/source/Relative.cpp"
#include "../FamilyTree/source/Relative.h"


#pragma warning (disable : 4996)   // Disable 'strcpy() unsafe' warning.

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(FamilyTreeTests)
	{
	public:
		FamilyTreeTests() : originalTree(generateAndSave("sampleFamilyTree.ftree")), testTree("sampleFamilyTree.ftree") {}
		~FamilyTreeTests()
		{
			std::remove("sampleFamilyTree.ftree");  // Clean up the test tree file.
		}

	private:
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
						   |   0     |
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
		static std::vector<Relative> generateSampleTree()
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
			T[0].father = 4;   T[0].mother = 1;
			T[1].father = -1;  T[1].mother = 5;
			T[2].father = 6;   T[2].mother = 0;
			T[3].father = 2;   T[3].mother = 5;
			T[4].father = -1;  T[4].mother = -1;
			T[5].father = -1;  T[5].mother = -1;
			T[6].father = 4;   T[6].mother = -1;
			T[7].father = -1;  T[7].mother = 5;

			T[0].children.resize(1);  T[0].children[0] = 2;
			T[1].children.resize(1);  T[1].children[0] = 0;
			T[2].children.resize(1);  T[2].children[0] = 3;
			T[4].children.resize(2);  T[4].children[0] = 0;   T[4].children[1] = 6;
			T[5].children.resize(3);  T[5].children[0] = 1;   T[5].children[1] = 3;  T[5].children[2] = 7;
			T[6].children.resize(1);  T[6].children[0] = 2;

			return T;
		}
		/*
			Saves family tree to file with the following format:
				- 8 byte unsigned int (size of the vector)
				- size-of-vector number of Relatives.
		*/
		static void saveTreeToFile(const std::vector<Relative>& T, const char* fileName)
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
		static std::vector<Relative> generateAndSave(const char* fileName)
		{
			std::vector<Relative> familyTree = generateSampleTree();
			saveTreeToFile(familyTree, fileName);
			return familyTree;
		}

	private:
		const std::vector<Relative> originalTree;   // Identical family tree as to one in 'testTree'. See 'generateSampleTree()' for definition.
		const FamilyTree testTree;                  // The object some of the tests are performed on.


		TEST_METHOD(ConstructorTest)
		{
			Assert::IsTrue(originalTree == testTree.getWholeTree()/*,{ L"'FamilyTree' doesn't read correctly a tree from file." }*/);
		}
		TEST_METHOD(myPetTest)
		{
			Assert::IsTrue(originalTree[0] == testTree.myPet());
		}
		TEST_METHOD(fatherTest)
		{
			Assert::IsTrue(originalTree[4] == *testTree.father());
		}
		TEST_METHOD(motherTest)
		{
			Assert::IsTrue(originalTree[1] == *testTree.mother());
		}
		TEST_METHOD(directChildrenTest)
		{
			std::vector<Relative> answer{ originalTree[2] };
			Assert::IsTrue(answer == testTree.directChildren());
		}
		TEST_METHOD(allChildrenTest)
		{
			std::vector<std::pair<Relative, unsigned>> answer{ {originalTree[2], 1}, {originalTree[3], 2} };
			Assert::IsTrue(answer == testTree.allChildren());
		}
		TEST_METHOD(allParentsTest)
		{
			std::vector<std::pair<Relative, unsigned>> answer{ {originalTree[4], 1}, {originalTree[1], 1}, {originalTree[5], 2} };
			Assert::IsTrue(answer == testTree.allParents());
		}
		TEST_METHOD(sameAgeTest)
		{
			std::vector<Relative> answer100;
			std::vector<Relative> answer5{ originalTree[0], originalTree[6]};
			std::vector<Relative> answer1{ originalTree[3] };
			Assert::IsTrue(answer100 == testTree.sameAge(100));
			Assert::IsTrue(answer5 == testTree.sameAge(5));
			Assert::IsTrue(answer1 == testTree.sameAge(1));
		}
		TEST_METHOD(sameColour)
		{
			std::vector<Relative> ansDARK_BROWN;
			std::vector<Relative> ansBLACK{ originalTree[0], originalTree[1], originalTree[7]};
			Assert::IsTrue(ansDARK_BROWN == testTree.sameColour(FurColour::DARK_BROWN));
			Assert::IsTrue(ansBLACK == testTree.sameColour(FurColour::BLACK));
		}
		TEST_METHOD(getWholeTree)
		{
			Assert::IsTrue(originalTree == testTree.getWholeTree());
		}
	};
}
