#pragma once
#include "SDL.h"

#include<SDL.h>
#include<string>

class Texture;

class Renderer
{
public:
	Renderer() = default;

	bool Initialize();
	bool CreateWindow(std::string title, int width, int height);
	
	friend class Text;
	

private:
	SDL_Renderer* m_renderer = nullptr;
	SDL_Window* m_window = nullptr;
	
};
