
#include <iostream>
#include "Renderer.h"
#include "Framebuffer.h"
#include "PostProcess.h"
#include "Image.h"
#include "Model.h"
#include "Transform.h"
#include "ETime.h"
#include "Input.h"
#include "Camera.h"
#include "Actor.h"
#include "Random.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <memory>


int main(int argc, char* argv[])
{
    Time time;


    std::string name = "title";
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow(name, 800, 600);

    Input input;
    input.Initialize();
    input.Update();

    Camera camera(800, 600);
    camera.SetProjection(80, 800 / (float)600, 0.1f, 100.0f);

    Transform cameraTransform{ { 0, 0, -20  } };
    //camera.SetView(glm::vec3{ 0, 0, -20 }, glm::vec3{ 0 });
    //camera.SetProjection(60.0f, 800.0f / 600, 0.1f, 200.0f);

    Framebuffer framebuffer(renderer, 800, 600);
    SetBlendMode(BlendMode::Normal);

   // Transform transform;
    Image image;
    image.Load("callie.png");
    

    //Image imageAlpha;
    //imageAlpha.Load("colors.png");
    //PostProcess::Alpha(imageAlpha.m_buffer, 128);
    //


    //Model model;
    //model.Load("torus.obj");
    //model.SetColor({ 0, 255, 0, 255 });
    std::shared_ptr<Model> model = std::make_shared<Model>();
    std::shared_ptr<Model> model2 = std::make_shared<Model>();
    std::shared_ptr<Model> model3 = std::make_shared<Model>();
    model->Load("5Hpen.obj");
    model2->Load("CallieModel.obj");
    model3->Load("cube.obj");

    model->SetColor({ 102,148, 255, 255 });
    model2->SetColor({ 255,60,  255,255 });
    model3->SetColor({ 0,255, 0, 255 });

    std::vector<std::unique_ptr<Actor>> actors;
    
   
        Transform transform{ {0,5,0}, glm::vec3{0,90,-180}, glm::vec3{5} };
        Transform transform2{ {10,5,0}, glm::vec3{0,5,-180}, glm::vec3{3} };
        Transform transform3{ {20,5,0}, glm::vec3{0,5,-180}, glm::vec3{3} };
        std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, model);
        std::unique_ptr<Actor> actor2 = std::make_unique<Actor>(transform2, model2);
        std::unique_ptr<Actor> actor3 = std::make_unique<Actor>(transform3, model3);
        //actor->SetColor(color_t{ (1, 255,255, 255) });
        //actor2->SetColor(color_t{ (255, 255,255) });
        //actor3->SetColor(color_t{ (255, 255,255) });
        actors.push_back(std::move(actor));
        actors.push_back(std::move(actor2));
        actors.push_back(std::move(actor3));
  

    bool quit = false;
    while (!quit)
    {

        time.Tick();
        input.Update();
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


        framebuffer.Clear(color_t{ 0,0,0,255 });

        framebuffer.DrawImage(32, 1, image);
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        if (input.GetMouseButtonDown(2))
        {
            input.SetRelativeMode(true);

            glm::vec3 direction{ 0 };
            if (input.GetKeyDown(SDL_SCANCODE_D)) direction.x = -1;
            if (input.GetKeyDown(SDL_SCANCODE_A)) direction.x = 1;
            if (input.GetKeyDown(SDL_SCANCODE_E)) direction.y = -1;
            if (input.GetKeyDown(SDL_SCANCODE_Q)) direction.y = 1;
            if (input.GetKeyDown(SDL_SCANCODE_W)) direction.z = 1;
            if (input.GetKeyDown(SDL_SCANCODE_S)) direction.z = -1;
            cameraTransform.rotation.y += input.GetMouseRelative().x * -0.1f;
            cameraTransform.rotation.x += input.GetMouseRelative().y * -0.1f;

            glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };

            cameraTransform.position += offset * 30.0f * time.GetDeltaTime();

     
        }
        else {
            input.SetRelativeMode(false);
        }

        camera.SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());
        for (auto& actor : actors) {
            actor->Draw(framebuffer, camera);
        }

            framebuffer.Update();
            renderer.CopyFramebuffer(framebuffer);
            SDL_RenderPresent(renderer.m_renderer);
    }
  
    return 0;
}