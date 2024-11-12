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

void InitScene01(Scene& scene, Camera& camera)
{
	camera.SetFOV(20.0f);
	camera.SetView({ 13, 2, 3 }, { 0, 0, 0 });

	auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f));
	scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0 } }, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = randomf();
		
			glm::vec3 center(a + 0.9 * randomf(), 0.2, b + 0.9 * randomf());

			if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
					sphere_material = std::make_shared<Lambertian>(albedo);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
					
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
					auto fuzz = randomf(0.0f, 0.5f);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					auto teapot = std::make_unique<Model>(Transform{ glm::vec3{center}, glm::vec3{randomf(1.0f, 100.0f)}, glm::vec3{0.5f}}, sphere_material);
					teapot->Load("Models/cube.obj");
					scene.AddObject(std::move(teapot));
					//scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
					auto teapot = std::make_unique<Model>(Transform{ glm::vec3{center}, glm::vec3{randomf(1.0f, 100.0f)}, glm::vec3{0.3f}}, sphere_material);
					teapot->Load("Models/suzanne.obj");
					scene.AddObject(std::move(teapot));

					//scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 0, 1, 0 } }, 1.0f, material1));


	auto material2 = std::make_shared<Lambertian>(color3_t(0.4f, 0.2f, 0.1f));
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ -4, 1, 0 } }, 1.0f, material2));

	auto material3 = std::make_shared<Metal>(color3_t(0.7f, 0.6f, 0.5f), 0.0f);
	//scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 4, 1, 0 } }, 1.0f, material3));
	auto teapot = std::make_unique<Model>(Transform{ glm::vec3{4,0,0}, glm::vec3{0,0,0}, glm::vec3{0.3f} }, material3);
	teapot->Load("Models/teapot.obj");
	scene.AddObject(std::move(teapot));
}









void InitScene02(Scene& scene, Camera& camera)
{
	
	//scene.SetSky(HSVtoRGB(240, 1, 0.2f), HSVtoRGB(240, 1, 1));
	camera.SetFOV(20.0f);
	camera.SetView({ 0, 2, -30 }, { 0, 0, 0 });

	auto white_material = std::make_shared<Lambertian>(color3_t(1,1,1));
	auto red_material = std::make_shared<Lambertian>(color3_t(1, 0, 0));
	auto green_material = std::make_shared<Lambertian>(color3_t(0, 1, 0));
	auto light_material = std::make_shared<Emissive>(color3_t(1, 1, 1), 12);
	auto gray_material = std::make_shared <Lambertian>(color3_t(0.5f));
	auto grayMetal_material = std::make_shared <Metal>(color3_t(0.5f),0.5f);
	auto blue_material = std::make_shared <Lambertian>(color3_t(0,0,1));
	// floor
	scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0,-2,0 } }, white_material));

	//roof
	scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0,4,0 } }, white_material));
	//back wall
	scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0,0,15 }, glm::vec3{90,0,0} }, white_material));
	//left wall
	scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ -5,0,0 }, glm::vec3{0,0,90 } }, red_material));
	//right wall
	scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 5,0,0 }, glm::vec3{0,0,90 } }, green_material));

	//auto triangle = std::make_unique<Triangle>(glm::vec3{ -10,0,0 }, glm::vec3{ -5,10,0 }, glm::vec3{ 0,0,0 }, green_material);
	//scene.AddObject(std::move(triangle));

	auto light = std::make_unique<Model>(Transform{ glm::vec3{0,4,0}, glm::vec3{90,0,0}, glm::vec3{2,1,2} }, light_material);
	light->Load("Models/quad.obj");
	scene.AddObject(std::move(light));

	//auto light2 = std::make_unique<Model>(Transform{ glm::vec3{0,-0.8f,-10}, glm::vec3{0,0,0}, }, light_material);
	//light2->Load("Models/quad.obj");
	//scene.AddObject(std::move(light2));

	auto stand = std::make_unique<Model>(Transform{ glm::vec3{0,-1.3f,-5}, glm::vec3{0,0,0}, glm::vec3{1.5f,1.5f,1.5f} }, gray_material);
	stand->Load("Models/cube.obj");
	scene.AddObject(std::move(stand));
	
	auto teapot = std::make_unique<Model>(Transform{ glm::vec3{0,-0.5f,-5}, glm::vec3{0,0,0}, glm::vec3{0.2f} }, blue_material);
	teapot->Load("Models/teapot.obj");
	scene.AddObject(std::move(teapot));




	int z = -10;
	
	for (int i = 0; i < 3; i++)
	{

	
	auto Callie = std::make_unique<Model>(Transform{ glm::vec3{-3,-2,z}, glm::vec3{0,110,0}, glm::vec3{1} }, gray_material);
	Callie->Load("CallieModel.obj");
	scene.AddObject(std::move(Callie));

	auto Callie2 = std::make_unique<Model>(Transform{ glm::vec3{3,-2,z}, glm::vec3{0,-70,0}, glm::vec3{1} }, gray_material);
	Callie2->Load("CallieModel.obj");
	scene.AddObject(std::move(Callie2));
	z += 5;
	}
}









void InitScene(Scene& scene)
{
	std::shared_ptr<Lambertian> material = std::make_shared<Lambertian>(color3_t{ 1,0,0 });

	scene.SetSky(HSVtoRGB(240, 1, 0.2f), HSVtoRGB(240, 1, 1));
	std::shared_ptr<Material> red = std::make_shared<Metal>(color3_t{ 1,0,0 }, 0.1f);
	std::shared_ptr<Material> green = std::make_shared<Emissive>(color3_t{ 0,1,0 });
	std::shared_ptr<Material> gray = std::make_shared<Metal>(color3_t{ 0.5f }, 0.1f);
	std::shared_ptr<Material> blue = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
	std::shared_ptr<Material> purple = std::make_shared<Lambertian>(color3_t{ 0, 1, 1 });
	//std::shared_ptr<Material> blue = std::make_shared<Dielectric>(color3_t{ 0, 0, 1 }, 1.5f);
	std::shared_ptr<Material> white = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1.333f);
	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(red);
	materials.push_back(green);
	materials.push_back(blue);
	materials.push_back(purple);
	materials.push_back(white);

	//auto triangle = std::make_unique<Triangle>(glm::vec3{-10,0,0}, glm::vec3{-5,10,0}, glm::vec3{0,0,0}, red);
	//scene.AddObject(std::move(triangle));

	//auto sphere = std::make_unique<Sphere>(vertice, blue)

	vertices_t vertice = { glm::vec3{ 0 }, glm::vec3{ 0 }, glm::vec3{ 0 } };
	


	auto model = std::make_unique<Model>(Transform{ glm::vec3{0},glm::vec3{0, 0,0}, glm::vec3{1} }, purple);
	//auto model = std::make_unique<Model>(vertice, blue);
	model->Load("Models/teapot.obj");
	scene.AddObject(std::move(model));

	//
	auto plane = std::make_unique<Plane>(Transform{ glm::vec3{0,-2,0}, glm::vec3{0,0,0} }, gray);
	scene.AddObject(std::move(plane));
}

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
	camera.SetView({ 0,0,-10 }, { 0,0,0 });

	Scene scene;
	//InitScene(scene);
	InitScene01(scene, camera);

	scene.Update();
	framebuffer.Clear(ColorConvert(color4_t{ 0,0.25,0,255 }));
	scene.Render(framebuffer, camera, 500, 6);

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

