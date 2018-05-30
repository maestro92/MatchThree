#ifndef GAME_BOARD_VIEW_H_
#define GAME_BOARD_VIEW_H_

#include "utility_sdl.h"
#include "game_board.h"
#include "world_object.h"
#include "model_manager.h"
#include "renderer.h"
#include "renderer_manager.h"

class GameBoardView
{
	public:
		void init(GameBoard* gameBoard);
		void render(Pipeline& p, GameBoard* gameBoard);

		void reset();

		WorldObject& getWorldObject(int x, int y);
	private:

		Renderer*					p_renderer;
		vector< vector<WorldObject> > gridCells;
		/*
		GLuint blueGemTexture;
		GLuint redGemTexture;
		GLuint orangeGemTexture;
		GLuint purpleGemTexture;
		GLuint greenGemTexture;
		*/
};


#endif
