
#include <iostream>
#include "Renderer.h"
#include "Framebuffer.h"
#include "PostProcess.h"
#include "Image.h"
#include "Color.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


int main(int argc, char* argv[])
{
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));

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
        
        //Image colorIMG;
        //SetBlendMode(BlendMode::Normal);
        //colorIMG.Load("colors.png");
        //framebuffer.DrawImage(60, 50, colorIMG);
        
       // PostProcess::Alpha(colorIMG.m_buffer, 90);

       // PostProcess::Invert(framebuffer.m_buffer);
       // 
       // PostProcess::Monochrome(framebuffer.m_buffer);
       // 
       // PostProcess::ColorBalance(framebuffer.m_buffer,150, -50, -50);
       // 
        //PostProcess::Brightness(framebuffer.m_buffer,50);
       // 
       // PostProcess::Noise(framebuffer.m_buffer,50);

       // PostProcess::Threshold(framebuffer.m_buffer,130);
        //PostProcess::Posterize(framebuffer.m_buffer,5);
     
        PostProcess::BoxBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
      
        //PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
       

       
       // PostProcess::Emboss(framebuffer.m_buffer, image.m_width, image.m_height);
       // PostProcess::EdgeDetection(framebuffer.m_buffer, image.m_width, image.m_height);



        framebuffer.Update();
        renderer.CopyFramebuffer(framebuffer);


        SDL_RenderPresent(renderer.m_renderer);
    }
  
    return 0;
}