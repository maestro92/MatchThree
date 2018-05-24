#pragma once

#include "utility_math.h"
#include <vector>

using namespace std;

class GameBoard
{
	public: 
		enum class Gem
		{
			Red,
			Blue,
			Green,
			Orange,
			Purple,
			NUM_TYPES,
		};

		
		void init(int w, int h);

		void debug();
		
		int getWidth();
		int getHeight();

		Gem getColor(int x, int y);

	private:
		vector<vector<Gem>> board;

};