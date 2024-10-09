#include <SDL.h>
#include <iostream>
#include "Renderer.h"
#include "Framebuffer.h"
#include "PostProcess.h"
#include "Image.h"
int main(int argc, char* argv[])
{
    std::string name = "title";
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow(name, 800, 600);

    Framebuffer framebuffer(renderer, 800, 600);

    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
        }
        framebuffer.Clear(color_t{0, 0, 0, 255 });


     //  for (int i = 0; i < 10000; i++)
     //  {
     //      int ran1 = (rand() % 800) + 1;
     //      int ran2 = (rand() % 600) + 1;
     //  
     //  framebuffer.DrawPoint(ran1, ran2, {255, 255, 255, 255});
     //  
     //  }
        framebuffer.DrawRect(10, 10, 50, 50, { 80, 10, 255,255 });
        framebuffer.DrawTriangle(100, 10, 120, 60, 80, 60, {0, 100, 255, 255});
        framebuffer.CircleBres(100, 500, 30, { 0, 100, 255, 255 });

        Image image;
        image.Load("office clown.png");
        framebuffer.DrawImage(50, 50, image);

       
        //PostProcess::Invert(framebuffer.m_buffer);


       // framebuffer.DrawLinearCurve(100, 100, 200, 200, { 0, 100, 255, 255 });
       // framebuffer.DrawQuadraticCurve(100, 200, 200, 100, 300, 200, { 0, 100, 255, 255 });
        framebuffer.Update();
        renderer.CopyFramebuffer(framebuffer);


        SDL_RenderPresent(renderer.m_renderer);
    }
  
    return 0;
}