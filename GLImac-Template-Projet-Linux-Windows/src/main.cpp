#include <memory>
#include <iostream>

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "thotEngine/World.hpp"
#include "thotEngine/Component.hpp"
#include "thotEngine/Entity.hpp"
#include "thotEngine/prefab.hpp"
#include "thotEngine/ResourceManager.hpp"
#include "thotEngine/MaterialManager.hpp"
#include "thotEngine/prefabmanager.hpp"

#include "thotEngine/WindowManager.hpp"
#include "thotEngine/Input.hpp"

//scripts
# include "thotEngine/FreeFlyCam.hpp"

#include "Bullet/btBulletCollisionCommon.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char** argv)
{

    // Initialize SDL and open a window
    te::WindowManager windowManager(WINDOW_WIDTH, WINDOW_HEIGHT, "ThotEngine");

    SDL_EnableKeyRepeat(10,10);

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;


    //load resources
    te::ResourceManager& resourceManager = te::ResourceManager::getInstance();
    resourceManager.loadImage("ship_tex_dif", "assets/textures/cobble2.png");
    resourceManager.loadImage("skybox_tex_dif", "assets/textures/skybox_texture.png");
    resourceManager.loadMesh("ship", "assets/models/cube.obj");
    resourceManager.loadProgram("glProg_3D", "shaders/3D.vs.glsl", "shaders/3D.fs.glsl");
    resourceManager.loadProgram("glProg_skybox", "shaders/skyShader.vs.glsl", "shaders/skyShader.fs.glsl");
    resourceManager.loadProgram("glProg_3DLight", "shaders/3DLight.vs.glsl", "shaders/3DLight.fs.glsl");
    resourceManager.loadCubeMap("skybox_cubeTex_dif", {"assets/textures/skyboxes/siege/right.png",
                                                       "assets/textures/skyboxes/siege/left.png",
                                                       "assets/textures/skyboxes/siege/top.png",
                                                       "assets/textures/skyboxes/siege/top.png",
                                                       "assets/textures/skyboxes/siege/front.png",
                                                       "assets/textures/skyboxes/siege/back.png"});

    //make materials
    te::MaterialManager& materialManager = te::MaterialManager::getInstance();
    materialManager.createMaterial<te::UnlitMaterial>("ship_unlit_mat", "glProg_3D", {"ship_tex_dif"});
    materialManager.createMaterial<te::UnlitMaterial>("skybox_mat", "glProg_skybox", {"skybox_tex_dif"});
    materialManager.createMaterial<te::LitMaterial>("ship_mat", "glProg_3DLight", {"ship_tex_dif"}, {0.9f,32.f});
    materialManager.createMaterial<te::LitMaterial>("ship_02_mat", "glProg_3DLight", {}, {0.4f,32.f});
    materialManager.createMaterial<te::SkyboxMaterial>("skybox_mat02", "glProg_skybox", {"skybox_cubeTex_dif"});

    //create world
    te::World world;

    //make a prefab

    //prefab 01
    std::shared_ptr<te::Prefab> prefab01 = std::make_shared<te::Prefab>("prefab01");
    prefab01->setMakeFunction([](te::ExternalHandler<te::Entity> entity, te::World& world){

        entity->setName("prefab01");
        entity->addComponent<te::Transform>(world)->setTranslation(0,0,0);
        auto prefabComponentHandler = entity->addComponent<te::MeshRenderer>(world);
        prefabComponentHandler->setMaterial("ship_02_mat");
        prefabComponentHandler->setMesh("ship");

    });

    te::PrefabManager::getInstance().add(prefab01);

    //populate world

    //ship 01
    auto entityHandler = world.instantiate();
    entityHandler->setName("ship01");
    entityHandler->addComponent<te::Transform>(world)->setTranslation(0,10,0);
    auto componentHandler = entityHandler->addComponent<te::MeshRenderer>(world);
    componentHandler->setMaterial("ship_mat");
    componentHandler->setMesh("ship");

    //test 01
    entityHandler = world.instantiate();
    entityHandler->setName("test01");
    entityHandler->addComponent<te::Transform>(world);
    auto cameraHandler = entityHandler->addComponent<te::Camera>(world);
    cameraHandler->setSkyboxMaterial("skybox_mat02");
    cameraHandler->setUseSkybox(true);
    entityHandler->addComponent<te::FreeFlyCam>(world);
    auto DirectionallightHandler = entityHandler->addComponent<te::DirectionalLight>(world);
    DirectionallightHandler->setDirection(glm::vec3(0,1,0));
    DirectionallightHandler->setColor(1,1,1);
    DirectionallightHandler->setIntensity(0.4);


    //test 02
    entityHandler = world.instantiate();
    entityHandler->setName("ship02");
    entityHandler->addComponent<te::Transform>(world)->setTranslation(20,10,0);
    componentHandler = entityHandler->addComponent<te::MeshRenderer>(world);
    componentHandler->setMaterial("ship_mat");
    componentHandler->setMesh("ship");
    auto PointlightHandler = entityHandler->addComponent<te::PointLight>(world);
    PointlightHandler->setRadius(400);
    PointlightHandler->setColor(0,1,0);
    PointlightHandler->setIntensity(10);


    //test 03
    entityHandler = world.instantiate(prefab01);
    entityHandler->getComponent<te::Transform>()->setScale(50,1,50);

    //test 04
    entityHandler = world.instantiate("prefab01");
    entityHandler->getComponent<te::Transform>()->setTranslation(0,10,20);
    PointlightHandler = entityHandler->addComponent<te::PointLight>(world);
    PointlightHandler->setRadius(400);
    PointlightHandler->setColor(1,1,0);
    PointlightHandler->setIntensity(10);


//    world.destroy(entityHandler);

    //finalize openGL context
    world.pushToGPU();

    //init world
    world.init();

    //main loop
    bool done = false;
    while(!done)
    {
        // Event loop:
        SDL_Event event;

        while(te::Input::pollEvent(event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true; // Leave the loop after this iteration
                break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                break;
            }
        }

        //updates
        world.update();

        //render
        world.render();
        windowManager.swapBuffers();

    }

    return 0;
}
