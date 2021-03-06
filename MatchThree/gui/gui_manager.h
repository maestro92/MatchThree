#ifndef GUI_MANAGER_H_
#define GUI_MANAGER_H_

#include "pipeline.h"
#include "utility.h"
#include "utility_math.h"
#include "Rect.h"
#include "Slider.h"
#include "list_box.h"
#include "button.h"
#include "control.h"
#include "renderer.h"

#include "gui_renderer_constants.h"
#include "custom_gui/bar.h"
#include "renderer_constants.h"

class GUIManager
{
    public:
        void init(int screenWidth, int screenHeight);

		void initGUIRenderingSetup();
		void setupRenderToScreen(int x, int y, int width, int height);
		void renderTextureFullScreen(GLuint textureId);
		void renderTextureFullScreen(GLuint textureId, GLuint fboTarget);
		void renderDepthTextureFullScreen(GLuint textureId);
		void renderDepthTextureFullScreen(GLuint textureId, GLuint fboTarget);
		void renderTexture(GLuint textureId, int x, int y, int width, int height, Renderer& r);
		void renderTexture(GLuint textureId, GLuint fboTarget, int x, int y, int width, int height, Renderer& r);
		void renderTexture(GLuint textureId, GLuint fboTarget, Rect rect);

		void renderSnipeScopeView(GLuint sceneTextureId);
		/*
		void renderTextureSingle(GLuint textureId, int x, int y, int width, int height);
		void renderTextureSingle(GLuint textureId, GLuint fboTarget, int x, int y, int width, int height);
		void renderTextureSingle(GLuint textureId, GLuint fboTarget, Rect rect);
		*/

		/*
		void setHorAimIndex(int index);
		void setVerAimIndex(int index);
		void setSniperZoomMode(bool b);
		void setFPSLabel(Label* fps);
		void setFPS(int fps);
		*/


		void initRenderer(const Array arr, Renderer* r, string name);
		Object findRendererObject(const Array arr, string name);
		void initRenderer(const Object obj, Renderer* r);



        void renderGUI();

		Renderer r_texture;
		Renderer r_depthTexture;
		Renderer r_coloredRect;
		Renderer r_texturedRect;
		Renderer r_listBoxItemHighlight;
		Renderer r_text;


        void updateAndRender(MouseState mouseState);
        void renderGUIComponents();
        void addGUIComponent(Control* control);
		int getNumGUIComponent();

		void setHP(int HP);
		void setArmor(int armor);
		void setAmmo(int ammo);
		void setFPS(int FPS);

		void setDrawingModeFlag(bool flag);

		void addDebugLabel(Control* control);
		void removeDebugLabels();

		Pipeline& getPipeline();

    private:



        int m_GUIComponentsID;
        int m_GUIComponentsFlags;

		Bar* m_HPBar;
		Bar* m_armorBar;
		Bar* m_ammoBar;

		Label* m_horiAim;
		Label* m_vertAim;
		Label* m_fpsLabel;
		Label* m_drawingModeLabel;

        vector<Control*> m_GUIComponents;
		vector<Control*> m_debugLabels;

		bool m_sniperZoomMode;
		int m_horAimIndex;
		int m_verAimIndex;



        QuadModel m_textureQuad;
        Pipeline m_GUIPipeline;
        int m_screenWidth;
        int m_screenHeight;
};

#endif
