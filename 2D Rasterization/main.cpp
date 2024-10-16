
#include <iostream>
#include "Renderer.h"
#include "Framebuffer.h"
#include "PostProcess.h"
#include "Image.h"
#include "Model.h"
#include "Transform.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


int main(int argc, char* argv[])
{


    std::string name = "title";
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow(name, 800, 600);
    Framebuffer framebuffer(renderer, 800, 600);

    Transform transform;
    Image image;
    image.Load("office clown.png");

    Image imageAlpha;
    imageAlpha.Load("colors.png");
    PostProcess::Alpha(imageAlpha.m_buffer, 128);
    

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

        SetBlendMode(BlendMode::Alpha);
      
       framebuffer.DrawImage(50, 50, image);
     
       framebuffer.DrawImage(100, 100, imageAlpha);
     

       
        framebuffer.Update();
        renderer.CopyFramebuffer(framebuffer);


        SDL_RenderPresent(renderer.m_renderer);
    }
  
    return 0;
}