#include "game_board.h"

void GameBoard::init(int w, int h)
{
	for (int y = 0; y < h; y++)
	{
		vector<Gem> row(w);
		board.push_back(row);
	}

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			board[y][x] = static_cast<GameBoard::Gem>((utl::randInt(0, (int)(Gem::NUM_TYPES))));
		}
	}

}


int GameBoard::getWidth()
{
	return board[0].size();
}

int GameBoard::getHeight()
{
	return board.size();
}

GameBoard::Gem GameBoard::getColor(int x, int y)
{
	return board[y][x];
}


void GameBoard::debug()
{
	for (int y = 0; y < getHeight(); y++)
	{
		string s = "";
		for (int x = 0; x < getWidth(); x++)
		{
			s += utl::intToStr((int)(board[y][x]));
		}
		cout << s << endl;
	}
	cout << endl;
}


