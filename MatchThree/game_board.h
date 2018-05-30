#pragma once

#include "utility_math.h"
#include <vector>
#include <functional>
#include "utility_json.h"
#include "utility_debug.h"
#include <iostream>
#include <fstream>

using namespace std;

/*
struct GridCoord
{
	int x;
	int y;

	GridCoord(int xIn, int yIn)
	{
		x = xIn;
		y = yIn;
	}

	bool operator==(const GridCoord& other)
	{
		return this->x == other.x && this->y == other.y;
	}

	GridCoord operator+(const GridCoord& other) 
	{
		GridCoord gc(this->x + other.x, this->y + other.y);
		return gc;
	}

	GridCoord operator-(const GridCoord& other)
	{
		GridCoord gc(this->x - other.x, this->y - other.y);
		return gc;
	}
};
*/

class GameBoard
{
	public: 
		enum class Dir
		{
			Horizontal,
			Vertical,
		};
	
		enum class Gem
		{
			Red,
			Blue,
			Green,
			Orange,
			Purple,
			NUM_TYPES,
		};

		
		struct VisitedFlags
		{
			vector<bool> flags;

			VisitedFlags()
			{

			}

			VisitedFlags(int numFlag)
			{
				flags.resize(numFlag);		
				reset();
			}

			void SetVisit(Gem gem)
			{
				flags[(int)gem] = true;
			}

			bool hasVisited(Gem gem)
			{
				return flags[(int)gem];
			}

			void reset()
			{
				for (int i = 0; i < flags.size(); i++)
				{
					flags[i] = false;
				}
			}

			void debug()
			{
				for (int i = 0; i < flags.size(); i++)
				{
					if (flags[i] == true)
					{
						cout << "T" << " ";
					}
					else
					{
						cout << "F" << " ";
					}
				}

			}
		};

		struct horiSeqComparator 		
		{
			inline bool operator() (const glm::vec2& i, const glm::vec2& j)
			{
				return i.x < j.x;
			}
		};

		/*
		bool vertSeqComparator(glm::vec2 i, glm::vec2 j)
		{
			return i.y < j.y;
		}
		*/

		GameBoard();

		void init(int w, int h);

		void findMatches();

		void debug();
		
		int getWidth();
		int getHeight();

		Gem getColor(int x, int y);

		void save();
		void load(char* filename);
		void reset();

		void resetFlags();

		bool saveLatest;

		void debugConnectedSequences();

		void sortUnorderedTraversals(vector<glm::vec2> unorderedTraversal);

	private:
		Object serializeGem(GameBoard::Gem gem);
		GameBoard::Gem deserializeGem(const mObject& obj);

		// void traverseFromThisCell(glm::vec2 src, vector<glm::vec2>& curTraversal);
		bool CanTraverseInThisDirection(glm::vec2 src, glm::vec2 dst);
		// bool CanStartTraverseFrom(glm::vec2 src);


		void traverseHoriFromThisCell(glm::vec2 src, vector<glm::vec2>& curTraversal);
		void traverseVertFromThisCell(glm::vec2 src, vector<glm::vec2>& curTraversal);


		bool CanStartHoriTraverseFrom(glm::vec2 src);
		bool CanStartVertTraverseFrom(glm::vec2 src);

		vector<vector<glm::vec2>> m_horiConnectedSequences;
		vector<vector<glm::vec2>> m_vertConnectedSequences;

		bool IsValidRange(glm::vec2 coord);

		vector<vector<glm::vec2>> m_matches;
	//	vector<glm::vec2> m_dirsToExamine;
		vector<glm::vec2> m_vertDirsToExamine;
		vector<glm::vec2> m_horiDirsToExamine;

		
		vector<vector<Gem>> board;
		vector<vector<VisitedFlags>> visited;

		vector<vector<bool>> isInHoriSeq;
		vector<vector<bool>> isInVertSeq;

};