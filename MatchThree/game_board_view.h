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
		void init(int w, int h);
		void render(Pipeline& p, GameBoard* gameBoard);

	private:
		vector< vector<WorldObject> > gridCells;
		Renderer*					p_renderer;

		/*
		GLuint blueGemTexture;
		GLuint redGemTexture;
		GLuint orangeGemTexture;
		GLuint purpleGemTexture;
		GLuint greenGemTexture;
		*/
};


#endif
