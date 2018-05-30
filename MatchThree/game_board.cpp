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
		visited.push_back(visitedRow);
		
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
			visited[y][x] = VisitedFlags((int)Gem::NUM_TYPES);
		}
	}

	if (saveLatest)
	{
		save();
	}


}


void GameBoard::debugConnectedSequences()
{
	for (int y = 0; y < m_horiConnectedSequences.size(); y++)
	{
		for (int x = 0; x < m_horiConnectedSequences[y].size(); x++)
		{
			cout << m_horiConnectedSequences[y][x].x << " " << m_horiConnectedSequences[y][x].y << ", ";
		}
		cout << endl;
	}
}

void GameBoard::findMatches()
{
	resetFlags();

	m_horiConnectedSequences.clear();

	for (int y = 0; y < getHeight(); y++)
	{
		for (int x = 0; x < getWidth(); x++)
		{
			glm::vec2 src(x, y);
			if (CanStartHoriTraverseFrom(src))
			{
				vector<glm::vec2> curTraversal;


				if (x == 4 && y == 0)
				{
					cout << "4 0 is " << (int)(board[y][x]) << endl;
					int a = 1;
				
					cout << "5 0 is " << (int)(board[y][x+1]) << endl;
					a = 1;

				}

				traverseHoriFromThisCell(src, curTraversal);



				// sort curTraversal
			//	sort(curTraversal.begin(), curTraversal.end(), horiSeqComparator());


				if (x == 4 && y == 0)
				{
					cout << "size is " << curTraversal.size() << endl;
					int a = 1;
				}

				if (curTraversal.size() > 1)
				{
					m_horiConnectedSequences.push_back(curTraversal);

					if (src.x == 4 && src.y == 0)
					{
						for (int i = 0; i < curTraversal.size(); i++)
						{
							cout << curTraversal[i].x << " " << curTraversal[i].y << ", ";
						}
						cout << endl;
					}

				}
			}

			/*
			if (CanStartVertTraverseFrom(src))
			{
				vector<glm::vec2> curTraversal;
				traverseVertFromThisCell(src, curTraversal);

				// sort curTraversal
				sort(curTraversal.begin(), curTraversal.end(), vertSeqComparator);
				m_vertConnectedSequences.push_back(curTraversal);
			}
			*/
		}
	}

	debugConnectedSequences();
}






void GameBoard::traverseHoriFromThisCell(glm::vec2 src, vector<glm::vec2>& curTraversal)
{
	Gem gem = board[src.y][src.x];

	visited[src.y][src.x].SetVisit(gem);

	curTraversal.push_back(src);

	bool isDeadEnd = true;
	for (int i = 0; i<m_horiDirsToExamine.size(); i++)
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

		visited[dst.y][dst.x].SetVisit(gem);


		if (dst.x == 5 && dst.y == 0)
		{
			utl::debug("dst is ", dst);
			Gem gem = board[dst.y][dst.x];

			if (visited[dst.y][dst.x].hasVisited(gem))
			{
				cout << "I have visited " << dst.x << " " << dst.y << endl;
			}

		}

		if (CanTraverseInThisDirection(src, dst) == true)
		{
			traverseHoriFromThisCell(dst, curTraversal);
			isDeadEnd = false;
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

	if (isDeadEnd)
	{
//		m_horiConnectedSequences.push_back(curTraversal);
	}

}

/*
void GameBoard::traverseVertFromThisCell(glm::vec2 src, vector<glm::vec2>& curTraversal)
{
	curTraversal.push_back(src);

	bool isDeadEnd = true;
	for (int i = 0; i<m_vertDirsToExamine.size(); i++)
	{
		glm::vec2 dst = src + m_vertDirsToExamine[i];
		if (CanTraverseInThisDirection(src, dst) == true)
		{
			traverseVertFromThisCell(dst, curTraversal);
			isDeadEnd = false;
		}
	}

	if (isDeadEnd)
	{
		m_horiConnectedSequences.push_back(curTraversal);
	}

}
*/


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
	return true;
}

bool GameBoard::CanStartVertTraverseFrom(glm::vec2 src)
{
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


bool GameBoard::CanTraverseInThisDirection(glm::vec2 src, glm::vec2 dst)
{
	if (IsValidRange(dst) == false)
		return false;

	if (board[dst.y][dst.x] != board[src.y][src.x])	
		return false;	

	Gem dstGem = board[dst.y][dst.x];

	if (visited[dst.y][dst.x].hasVisited(dstGem))
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


void GameBoard::resetFlags()
{
	for (int y = 0; y < getHeight(); y++)
	{
		for (int x = 0; x < getWidth(); x++)
		{			
			visited[y][x].reset();
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
	vector<vector<VisitedFlags>> newVisited;
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

		newVisited.push_back(booleanRow);

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
	visited = newVisited;
	isInHoriSeq = newHoriFlags;
	isInVertSeq = newVertFlags;

//	debug();
}

