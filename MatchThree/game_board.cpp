#include "game_board.h"

GameBoard::GameBoard()
{
	/*
	m_dirsToExamine.push_back(glm::vec2(-1, 0));
	m_dirsToExamine.push_back(glm::vec2(0, -1));
	m_dirsToExamine.push_back(glm::vec2(1, 0));
	m_dirsToExamine.push_back(glm::vec2(0, 1));
	*/

	m_horiDirsToExamine.push_back(glm::vec2(1, 0));
	m_horiDirsToExamine.push_back(glm::vec2(-1, 0));
//	m_horiDirsToExamine.push_back(glm::vec2(0, 1));
//	m_horiDirsToExamine.push_back(glm::vec2(0, -1));

	m_vertDirsToExamine.push_back(glm::vec2(0, 1));
	m_vertDirsToExamine.push_back(glm::vec2(0, -1));
}

void GameBoard::init(int w, int h)
{
	for (int y = 0; y < h; y++)
	{
		vector<Gem> row(w);
		board.push_back(row);

		vector<VisitedFlags> visitedRow(w);
		horiVisited.push_back(visitedRow);
		vertVisited.push_back(visitedRow);
		
		vector<bool> vertRow(w);
		vector<bool> horiRow(w);

		isInHoriSeq.push_back(horiRow);
		isInVertSeq.push_back(vertRow);
	}

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			board[y][x] = static_cast<GameBoard::Gem>((utl::randInt(0, (int)(Gem::NUM_TYPES))));
			horiVisited[y][x] = VisitedFlags((int)Gem::NUM_TYPES);
			vertVisited[y][x] = VisitedFlags((int)Gem::NUM_TYPES);
		}
	}

	if (saveLatest)
	{
		save();
	}


}


void GameBoard::debugConnectedSequences()
{
	cout << "Printing horizontal sequences" << endl;
	for (int y = 0; y < m_horiConnectedSequences.size(); y++)
	{
		for (int x = 0; x < m_horiConnectedSequences[y].size(); x++)
		{
			cout << m_horiConnectedSequences[y][x].x << " " << m_horiConnectedSequences[y][x].y << ", ";
		}
		cout << endl;
	}

	cout << "Printing vertical sequences" << endl;
	for (int y = 0; y < m_vertConnectedSequences.size(); y++)
	{
		for (int x = 0; x < m_vertConnectedSequences[y].size(); x++)
		{
			cout << m_vertConnectedSequences[y][x].x << " " << m_vertConnectedSequences[y][x].y << ", ";
		}
		cout << endl;
	}

}

void GameBoard::findMatches()
{
	resetFlags();

	m_horiConnectedSequences.clear();
	m_vertConnectedSequences.clear();
	for (int y = 0; y < getHeight(); y++)
	{
		for (int x = 0; x < getWidth(); x++)
		{
			glm::vec2 src(x, y);
			if (CanStartHoriTraverseFrom(src) )
			{
				vector<glm::vec2> curTraversal;
				traverseHoriFromThisCell(src, curTraversal);

				if (curTraversal.size() > 1)
				{
					m_horiConnectedSequences.push_back(curTraversal);
				}
			}

			if (CanStartVertTraverseFrom(src))
			{
				vector<glm::vec2> curTraversal;
				traverseVertFromThisCell(src, curTraversal);

				if (curTraversal.size() > 1)
				{
					m_vertConnectedSequences.push_back(curTraversal);
				}
			}			
		}
	}

	debugConnectedSequences();
}


void GameBoard::findPotentialMatches()
{
	// the idea is taken from this link https://dgkanatsios.com/2015/02/25/building-a-match-3-game-in-unity-3/
	// he is doing a check starting from every grid

	// in my case, if I have already ran findMatches, that means I already have the list of connected Sequence
	// in that case, I just need to loop through the connected sequence and check for potential matches instead of iterating
	// thru every cell

	findPotentialHoriMatches();
	findPotentialVertMatches();
}



vector<vector<glm::vec2>> GameBoard::findPotentialHoriMatches()
{
	for (int i = 0; i < m_horiConnectedSequences.size(); i++)
	{
		// this is assuming horiConnectedSeq goes from small x to large x
		vector<glm::vec2> seq = m_horiConnectedSequences[i];

		findPotMatchHori0(seq);
		findPotMatchHori1(seq);
		findPotMatchHori2(seq);
	}

	for (int i = 0; i < m_vertConnectedSequences.size(); i++)
	{
		// this is assuming horiConnectedSeq goes from small x to large x
		vector<glm::vec2> seq = m_vertConnectedSequences[i];

		findPotMatchVert0(seq);
		findPotMatchVert1(seq);
		findPotMatchVert2(seq);
	}

}

vector<vector<glm::vec2>> GameBoard::findPotentialVertMatches()
{

}


vector<glm::vec2> buildPotMatchSeq(glm::vec2 move, vector<glm::vec2> seq, bool addToFront)
{
	vector<glm::vec2> pot;
	if (addToFront)
	{
		pot.push_back(move);
	}

	for (int i = 0; i < seq.size(); i++)
	{
		pot.push_back(seq[i]);
	}

	if (addToFront == false)
	{
		pot.push_back(move);
	}

	return pot;
}

void GameBoard::findPotMatchHori0(vector<glm::vec2> seq)
{
	// https://github.com/dgkanatsios/MatchThreeGame/blob/master/Assets/Scripts/Utilities.cs
	/* example *\
	* * * * *
	* * * * *
	* * * * *
	* & & * *
	& * * * *
	\* example  */

	glm::vec2 coord0 = seq[0];
	glm::vec2 coord1(coord0.x - 1, coord0.y - 1);

	if (IsValidRange(coord1))
	{		
		vector<glm::vec2> potMatch = buildPotMatchSeq(coord1, seq, true);
		m_potentialMatches.push_back(potMatch);
	}


	/* example *\
	* * * * *
	* * * * *
	& * * * *
	* & & * *
	* * * * *
	\* example  */

	glm::vec2 coord2(coord0.x - 1, coord0.y + 1);
	if (IsValidRange(coord1))
	{
		vector<glm::vec2> potMatch = buildPotMatchSeq(coord1, seq, false);
		m_potentialMatches.push_back(potMatch);
	}
}

void GameBoard::findPotMatchHori1(vector<glm::vec2> seq)
{
	/* example *\
	* * * * *
	* * * * *
	* * * * *
	* & & * *
	* * * & *
	\* example  */

	glm::vec2 coord0 = seq[seq.size()-1];
	glm::vec2 coord1(coord0.x + 1, coord0.y - 1);

	if (IsValidRange(coord1))
	{
		vector<glm::vec2> potMatch = buildPotMatchSeq(coord1, seq, true);
		m_potentialMatches.push_back(potMatch);
	}


	/* example *\
	* * * * *
	* * * * *
	* * * & *
	* & & * *
	* * * * *
	\* example  */

	coord1 = glm::vec2(coord0.x + 1, coord0.y + 1);
	if (IsValidRange(coord1))
	{
		vector<glm::vec2> potMatch = buildPotMatchSeq(coord1, seq, false);
		m_potentialMatches.push_back(potMatch);
	}
}



void GameBoard::findPotMatchHori2(vector<glm::vec2> seq)
{
	/* example *\
	* * * * *
	* * * * *
	* * * * *
	* & * & &
	* * * * *
	\* example  */

	glm::vec2 coord0 = seq[0];
	glm::vec2 coord1(coord0.x - 2, coord0.y);

	if (IsValidRange(coord1) && getGem(coord0) == getGem(coord1))
	{
		vector<glm::vec2> potMatch = buildPotMatchSeq(coord1, seq, true);
		m_potentialMatches.push_back(potMatch);
	}


	/* example *\
	* * * * *
	* * * * *
	* * * * *
	* & & * &
	* * * * *
	\* example  */
	coord0 = seq[seq.size() - 1];
	coord1 = glm::vec2(coord0.x + 2, coord0.y);

	if (IsValidRange(coord1))
	{
		vector<glm::vec2> potMatch = buildPotMatchSeq(coord1, seq, false);
		m_potentialMatches.push_back(potMatch);
	}
}

vector<glm::vec2> GameBoard::checkHori2(int x, int y)
{

}

vector<glm::vec2> GameBoard::checkVert0(int x, int y)
{

}

vector<glm::vec2> GameBoard::checkVert1(int x, int y)
{

}

vector<glm::vec2> GameBoard::checkVert2(int x, int y)
{

}



void GameBoard::traverseHoriFromThisCell(glm::vec2 src, vector<glm::vec2>& curTraversal)
{
	Gem gem = board[src.y][src.x];



	horiVisited[src.y][src.x].SetVisit(gem);

	horiVisited[0][5].debug();

	curTraversal.push_back(src);

	bool isDeadEnd = true;
	for (int i = 0; i < m_horiDirsToExamine.size(); i++)
	{
		glm::vec2 dst = src + m_horiDirsToExamine[i];

		if (src.x == 4 && src.y == 0)
		{
			utl::debug("dst is ", dst);
		}

		if (IsValidRange(dst) == false)
		{
			continue;
		}

		if (dst.x == 5 && dst.y == 0)
		{
			utl::debug("dst is ", dst);
			Gem gem = board[dst.y][dst.x];

			if (horiVisited[dst.y][dst.x].hasVisited(gem))
			{
				cout << "I have visited " << dst.x << " " << dst.y << endl;
			}

		}

		if (CanTraverseHoriInThisDirection(src, dst) == true)
		{
			horiVisited[dst.y][dst.x].SetVisit(gem);
			traverseHoriFromThisCell(dst, curTraversal);
			isDeadEnd = false;
		}
		else
		{
			horiVisited[dst.y][dst.x].SetVisit(gem);
		}
	}

	if (src.x == 4 && src.y == 0)
	{
		for (int i = 0; i < curTraversal.size(); i++)
		{
			cout << curTraversal[i].x << " " << curTraversal[i].y << ", ";
		}
		cout << endl;
	}
}


void GameBoard::traverseVertFromThisCell(glm::vec2 src, vector<glm::vec2>& curTraversal)
{
	Gem gem = board[src.y][src.x];
	vertVisited[src.y][src.x].SetVisit(gem);
	curTraversal.push_back(src);

	bool isDeadEnd = true;
	for (int i = 0; i < m_vertDirsToExamine.size(); i++)
	{
		glm::vec2 dst = src + m_vertDirsToExamine[i];

		if (IsValidRange(dst) == false)
		{
			continue;
		}


		if (CanTraverseVertInThisDirection(src, dst) == true)
		{
			vertVisited[dst.y][dst.x].SetVisit(gem);
			traverseVertFromThisCell(dst, curTraversal);
			isDeadEnd = false;
		}
		else
		{
			vertVisited[dst.y][dst.x].SetVisit(gem);
		}
	}
}



/*
void GameBoard::traverseFromThisCell(glm::vec2 src, vector<glm::vec2>& curTraversal)
{
	curTraversal.push_back(src);

	bool isDeadEnd = true;
	for(int i=0; i<m_dirsToExamine.size(); i++)
	{
		glm::vec2 dst = src + m_dirsToExamine[i];
		if (CanTraverseInThisDirection(src, dst) == true)
		{
			traverseFromThisCell(dst, curTraversal);
			isDeadEnd = false;
		}
	}

	if (isDeadEnd)
	{
		m_horiConnectedSequences.push_back(curTraversal);
	}

//	curTraversal.pop_back();
}
*/

bool GameBoard::CanStartHoriTraverseFrom(glm::vec2 src)
{
	Gem gem = board[src.y][src.x];

	if (horiVisited[src.y][src.x].hasVisited(gem))	
		return false;
	
	if (isInHoriSeq[src.y][src.x] == true)
		return false;

	return true;
}

bool GameBoard::CanStartVertTraverseFrom(glm::vec2 src)
{
	Gem gem = board[src.y][src.x];

	if (vertVisited[src.y][src.x].hasVisited(gem))
		return false;

	if (isInVertSeq[src.y][src.x] == true)
		return false;

	return true;


	return true;
}


bool GameBoard::IsValidRange(glm::vec2 coord)
{
	if (coord.x < 0)	return false;
	if (coord.y < 0)	return false;
	if (coord.x >= getWidth())	return false;
	if (coord.y >= getHeight())	return false;
	return true;
}


bool GameBoard::CanTraverseHoriInThisDirection(glm::vec2 src, glm::vec2 dst)
{
	if (IsValidRange(dst) == false)
		return false;

	if (board[dst.y][dst.x] != board[src.y][src.x])	
		return false;	

	Gem dstGem = board[dst.y][dst.x];

	if (horiVisited[dst.y][dst.x].hasVisited(dstGem))
	{		
		return false;
	}
	return true;
}



bool GameBoard::CanTraverseVertInThisDirection(glm::vec2 src, glm::vec2 dst)
{
	if (IsValidRange(dst) == false)
		return false;

	if (board[dst.y][dst.x] != board[src.y][src.x])
		return false;

	Gem dstGem = board[dst.y][dst.x];

	if (vertVisited[dst.y][dst.x].hasVisited(dstGem))
	{
		return false;
	}
	return true;
}

int GameBoard::getWidth()
{
	return board[0].size();
}

int GameBoard::getHeight()
{
	return board.size();
}

GameBoard::Gem GameBoard::getGem(int x, int y)
{
	return board[y][x];
}

GameBoard::Gem GameBoard::getGem(glm::vec2 coord)
{
	return board[coord.y][coord.x];
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


void GameBoard::resetFlags()
{
	for (int y = 0; y < getHeight(); y++)
	{
		for (int x = 0; x < getWidth(); x++)
		{			
			horiVisited[y][x].reset();
			vertVisited[y][x].reset();
			isInHoriSeq[y][x] = false;
			isInVertSeq[y][x] = false;
		}
	}
}


void GameBoard::reset()
{

}


Object GameBoard::serializeGem(GameBoard::Gem gem)
{
	Object pointObj;

	pointObj.push_back(Pair("gem", (int)gem));

	return pointObj;
}


GameBoard::Gem GameBoard::deserializeGem(const mObject& obj)
{
	int intGem = utl::findValue(obj, "gem").get_int();
	return (GameBoard::Gem)intGem;
}


void GameBoard::save()
{
	ofstream myfile;
	myfile.open("data.txt");
	cout << "saving data " << endl;


	Object boardObj;
	boardObj.push_back(Pair("h", (int)board.size()));
	boardObj.push_back(Pair("w", (int)board[0].size()));

	Array array;
	for (int y = 0; y < board.size(); y++)
	{
		Array row;
		for (int x = 0; x < board[0].size(); x++)
		{
			Object vObj = serializeGem(board[y][x]);
			row.push_back(vObj);
		}

		array.push_back(row);
	}

	boardObj.push_back(Pair("board", array));
	write(boardObj, myfile, pretty_print);
	myfile.close();
}




void GameBoard::load(char* filename)
{
	mValue content = utl::readJsonFileToMap(filename);

	const mObject& obj = content.get_obj();

	int w = utl::findValue(obj, "w").get_int();
	int h = utl::findValue(obj, "h").get_int();

	cout << w << " " << h << endl;

	vector<vector<Gem>> newBoard;
	vector<vector<VisitedFlags>> newHoriVisited;
	vector<vector<VisitedFlags>> newVertVisited;
	vector<vector<bool>> newHoriFlags;
	vector<vector<bool>> newVertFlags;


	for (int y = 0; y < h; y++)
	{
		vector<Gem> row(w);
		newBoard.push_back(row);

		vector<VisitedFlags> booleanRow(w);

		for (int i = 0; i < w; i++)
		{
			booleanRow[i] = VisitedFlags((int)Gem::NUM_TYPES);
		}

		newHoriVisited.push_back(booleanRow);
		newVertVisited.push_back(booleanRow);
		vector<bool> vertRow(w);
		vector<bool> horiRow(w);

		newHoriFlags.push_back(horiRow);
		newVertFlags.push_back(vertRow);
	}

	const mArray& boardArray = utl::findValue(obj, "board").get_array();
	for (int y = 0; y < h; y++)
	{
		const mArray& row = boardArray[y].get_array();
		vector<Gem> gemRow(w);
		for (int x = 0; x < w; x++)
		{
			const mObject obj = row[x].get_obj();
			Gem gem = deserializeGem(obj);
			newBoard[y][x] = gem;
		}
	}

	/*
	cout << "printing newBoard" << endl;
	for (int y = 0; y < h; y++)
	{
		string s = "";
		for (int x = 0; x < w; x++)
		{
			s += utl::intToStr((int)(newBoard[y][x]));
		}
		cout << s << endl;
	}
	cout << "end newBoard" << endl;
	*/
	board = newBoard;
	horiVisited = newHoriVisited;
	vertVisited = newVertVisited;
	isInHoriSeq = newHoriFlags;
	isInVertSeq = newVertFlags;

//	debug();
}


void GameBoard::debugVisitedFlags()
{
	for (int y = 0; y < getHeight(); y++)
	{
		for (int x = 0; x < getWidth(); x++)
		{
			horiVisited[y][x].debug();
		}
	}
}


