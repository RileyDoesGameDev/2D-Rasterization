#include "SDL.h"
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
#include "Tracer.h"
#include "Scene.h"
#include "Plane.h"
#include "Triangle.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>

int main(int argc, char* argv[])
{

	srand((unsigned int)time(NULL));

	Time time;
	std::string str = "title";
	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow(str, 800, 600);

	SetBlendMode(BlendMode::Normal);
	
	Framebuffer framebuffer(renderer, 800, 600);

	Camera camera{ 70.0f, renderer.GetWidth() / (float)(renderer.GetHeight()) };
	camera.SetView({ 0,0,-30 }, { 0,0,0 });

	Scene scene;
	//std::shared_ptr<Lambertian> material = std::make_shared<Lambertian>(color3_t{ 1,0,0 });

	
	std::shared_ptr<Material> red = std::make_shared<Metal>(color3_t{ 1,0,0 }, 0.1f);
	std::shared_ptr<Material> green = std::make_shared<Emissive>(color3_t{ 0,1,0 });
	std::shared_ptr<Material> gray = std::make_shared<Metal>(color3_t{ 0.5f },0.1f);
	std::shared_ptr<Material> blue = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
	//std::shared_ptr<Material> blue = std::make_shared<Dielectric>(color3_t{ 0, 0, 1 }, 1.5f);
	std::shared_ptr<Material> white = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1.333f);
	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(red);
	materials.push_back(green);
	materials.push_back(blue);
	materials.push_back(white);
	//glm::vec3 v1(10.0f, 0.0f, 0.0f);   // First vertex, moved further along the x-axis
	//glm::vec3 v2(0.0f, 10.0f, 0.0f);   // Second vertex, moved further along the y-axis
	//glm::vec3 v3(0.0f, 0.0f, 10.0f)






	//for (int i = 0; i < 1; i++)
	//{
	//
	//	auto object = std::make_unique<Sphere>(randomf(glm::vec3{ -10 }, glm::vec3{ 10 }), randomf(0.2f, 3.0f), materials[random(0, (int)materials.size())]);
	//	scene.AddObject(std::move(object));
	//}


	auto object = std::make_unique<Sphere>(glm::vec3{ -10, 0, 0 },  1.0f, blue);
	auto object1 = std::make_unique<Sphere>(glm::vec3{ -5, 10, 0 }, 1.0f , blue);
	auto object2 = std::make_unique<Sphere>(glm::vec3{ 0, 0, 0 }, 1.0f , blue);
	scene.AddObject(std::move(object));
	scene.AddObject(std::move(object1));
	scene.AddObject(std::move(object2));

	auto triangle = std::make_unique<Triangle>(glm::vec3{-10,0,0}, glm::vec3{-5,10,0}, glm::vec3{0,0,0}, green);
	scene.AddObject(std::move(triangle));
	
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, -5, 0 }, glm::vec3{ 0, 1, 0 }, gray);
	scene.AddObject(std::move(plane));


	framebuffer.Clear(ColorConvert(color4_t{ 0,0.25,0,255 }));
	scene.Render(framebuffer, camera, 10, 3);

	bool quit = false;
	while (!quit)
	{
		time.Tick();
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

		

		framebuffer.Update();
		renderer = framebuffer;
		SDL_RenderPresent(renderer.m_renderer);
	}



	return 0;
}