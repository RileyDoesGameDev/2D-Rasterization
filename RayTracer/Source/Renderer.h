#pragma once
#include "SDL.h"

#include<SDL.h>
#include<string>
#include "Framebuffer.h"

class Texture;

class Renderer
{
public:
	Renderer() = default;

	bool Initialize();
	bool CreateWindow(std::string& name, int width, int height);
	void CopyFramebuffer(const Framebuffer& framebuffer);

	//void operator = (const class Framebuffer&)

	friend class Text;
	

	SDL_Renderer* m_renderer = nullptr;
	SDL_Window* m_window = nullptr;

	
};
