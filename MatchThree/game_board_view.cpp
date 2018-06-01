#include "game_board_view.h"
#include "global.h"

void GameBoardView::init(GameBoard* gameBoard)
{
	/*
	blueGem = utl::loadSDLImage("Assets/blue gem.jpg");
	redGem = utl::loadSDLImage("Assets/red gem.jpg");
	orangeGem = utl::loadSDLImage("Assets/orange gem.jpg");
	purpleGem = utl::loadSDLImage("Assets/purple gem.jpg");
	greenGem = utl::loadSDLImage("Assets/green gem.jpg");
	*/

	float curX = 0;
	float curY = 0;
	float size = 1;
	float spacing = 0.2;

	int w = gameBoard->getWidth();
	int h = gameBoard->getHeight();
	
	for (int y = 0; y < h; y++)
	{
		vector<WorldObject> row;
		curX = 0;
		for (int x = 0; x < w; x++)
		{
			WorldObject obj = WorldObject();
			obj.setModel(global.modelMgr->get(ModelEnum::centeredQuad));
			obj.setPosition(glm::vec3(curX, curY, 0));

			obj.setScale(1);
			row.push_back(obj);

			curX += size + spacing;
		}

		curY += size + spacing;
		gridCells.push_back(row);
	}
}


WorldObject& GameBoardView::getWorldObject(int x, int y)
{
	return gridCells[y][x];
}

void GameBoardView::reset()
{
	gridCells.clear();
}

void GameBoardView::render(Pipeline& p, GameBoard* gameBoard)
{
	p_renderer = &global.rendererMgr->r_fullColor;

	p_renderer->enableShader();
	for (int y = 0; y < gameBoard->getHeight(); y++)
	{
		for (int x = 0; x < gameBoard->getWidth(); x++)
		{
			glm::vec3 color;
			switch (gameBoard->getGem(x,y))
			{
				case GameBoard::Gem::Red:
					color = COLOR_RED;
					break;
				case GameBoard::Gem::Blue:
					color = COLOR_BLUE;
					break;
				case GameBoard::Gem::Green:
					color = COLOR_GREEN;
					break;
				case GameBoard::Gem::Purple:
					color = COLOR_PURPLE;
					break;
				case GameBoard::Gem::Orange:
					color = COLOR_ORANGE;
					break;
				default:
					break;
			}

			p_renderer->setData(R_FULL_COLOR::u_color, color);
			gridCells[y][x].renderGroup(p, p_renderer);
		}

	}

	p_renderer->disableShader();
}


