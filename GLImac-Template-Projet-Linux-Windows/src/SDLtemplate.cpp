/*
#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/camera.h>
#include <vector>
#include <glimac/Image.hpp>

#define NBTEXTURECUBE 2

#include "glimac/chunk.hpp"
#include "glimac/perlin.h"
#include "glimac/map.hpp"
#include "glimac/config.hpp"
#include "glimac/player.hpp"
#include "glimac/character.hpp"
#include "glimac/cubeCollider.hpp"
#include "glimac/light.hpp"
#include "glimac/pointLight.hpp"
#include "glimac/directionnalLight.hpp"
#include "glimac/lightManager.hpp"
#include "glimac/skybox.hpp"
#include "glimac/model.hpp"
#include "glimac/itemModel.hpp"
#include "glimac/particuleEmitter.hpp"
#include "glimac/particuleManager.hpp"
#include "glimac/shared.hpp"
#include "glimac/visualLoad.hpp"
#include "glimac/menu.hpp"


#include "glimac/save.hpp"



using namespace glimac;



glm::mat4 scale(float tx, float ty, float tz)
{
    return glm::mat4(glm::vec4(tx,0,0,0),glm::vec4(0,ty,0,0),glm::vec4(0,0,tz,0), glm::vec4(0,0,0,1));
}








static int WINDOW_WIDTH = 1000;
static int WINDOW_HEIGHT = 700;







void jeu( FilePath& applicationPath, SDLWindowManager& windowManager,int optionMenu)
{
    bool done = false;

    Config confData;
    confData.setConfig(6);
    confData.setMagFilter(0);
    confData.setMinFilter(0);
    confData.setMipMapFilter(true);

    ///particule manager :
    ParticuleManager particuleManager(applicationPath, applicationPath.dirPath()+"assets/textures/fire_unity.png");

    ///map debut :
     std::unique_ptr<Image> ptr_texture = loadImage(applicationPath.dirPath()+"assets/textures/texture_cubes_04.png");
     //std::unique_ptr<Image> ptr_texture = loadImage("C:/Users/Alexflo/Google Drive/opengl/opengl_test/GLImac-Template-TPs-Linux-Windows/assets/textures/texture_cubes_03.png");

     if(ptr_texture ==NULL)
     {
        std::cout<<"coucou je suis une erreur de chargement de texture !!! "<<std::cout;
     }
    GLuint texture_cube;
    glGenTextures(1, &texture_cube);
    glBindTexture(GL_TEXTURE_2D, texture_cube);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ptr_texture->getWidth(), ptr_texture->getHeight(), 0, GL_RGBA, GL_FLOAT, ptr_texture->getPixels());

        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if(confData.getMipMapFilter())
        {
            glEnable(GL_TEXTURE_2D);
            glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
        else
        {
            if(confData.getMinFilter()==0)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            else if(confData.getMinFilter()==1)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            if(confData.getMaxFilter()==0)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            if(confData.getMaxFilter()==1)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }




    glBindTexture(GL_TEXTURE_2D, 0);

    std::vector<std::string> pathTexturesVisualLoad = {std::string(applicationPath.dirPath()+"assets/textures/chargement_texture.png"), std::string(applicationPath.dirPath()+"assets/textures/fond_minecraft.png")};
    VisualLoad visualLoad(applicationPath, pathTexturesVisualLoad );
    //Map(const glimac::FilePath& applicationPath, GLuint _textureCube, int nbTexturecube, int _nbChunk_x, int _nbChunk_y, int _nbChunk_z, int _nbCubeByRowByChunk, int seed, int nbStrates);
    //Chunk(glimac::Program const& program, int longueur, int largeur, int nbOctave, int persistance, int pas, int seed, int mini, int maxi );
    //Chunk chunk_test(program, 10, 10, 8, 0.5, 128, rand()%100, 0, 6);

    //Map map_test(applicationPath, texture_cube, 2, 3, 3, 3, 20, 100, 1);

    Map map_test;

   if(optionMenu==0)
   {
         map_test = Map(applicationPath, texture_cube, 5, 5, 6, 5, 20, 100, 3, &visualLoad, &windowManager);
   }
   else
   {
        map_test = Map(applicationPath, 5, 5, 6, 5, 20, 100, particuleManager, &visualLoad, &windowManager);
   }

   ///map fin



    ///caméra :
       Camera camera;
       camera.setVitesseDeplacement(3.f);
       camera.setVitesseLook(0.1f);
       camera.setPosition(glm::vec3(0, 0, 0));

       Player joueur;
       //joueur.setUseGravity(true, windowManager.getTime());
       joueur.toogleIsSwimming(false);
       joueur.getHead().setVitesseLook(0.1f);
       joueur.setSpeed(0.3f, 0.3f);
       joueur.setPosition(map_test.getPositionSpawn());

       ItemManager itemManager;

       LightManager lightManager(20);
       DirectionnalLight* sun = new DirectionnalLight(glm::vec3(3,10,0), glm::vec3(0.5,0.5,0.2));
       lightManager.addDirectionnalLight(sun);

       Skybox skybox(applicationPath);
       skybox.setupCubeMap( applicationPath.dirPath()+"assets/textures/skybox_texture_opt02.png");
       //skybox.setupCubeMap("C:/Users/Alexflo/Google Drive/opengl/opengl_test/GLImac-Template-TPs-Linux-Windows/assets/textures/skybox_texture_opt02.png");

    //    //GLchar* modelPathGL = applicationPath.dirPath()+"assets/models/pioche/pioche.obj";
    //    Model test_model(applicationPath, (std::string)(applicationPath.dirPath()+"assets/models/pioche/pioche.obj"));






       //SDL_WM_GrabInput(SDL_GRAB_ON);
       float xPre = 0, yPre = 0;
       int tpPre =0, tpAct = 0;



       // Application loop:
       while(!done) {
           // Event loop:
           SDL_Event event;

           if (windowManager.isKeyPressed(SDLK_UP) || windowManager.isKeyPressed(SDLK_z) || windowManager.isKeyPressed(SDLK_w))
           {
               joueur.prepareMove(Character::CHARACTER_MOVEMENT::FORWARD);
           }
           if (windowManager.isKeyPressed(SDLK_DOWN) || windowManager.isKeyPressed(SDLK_s) )
           {
               joueur.prepareMove(Character::CHARACTER_MOVEMENT::BACKWARD);
           }
           if(windowManager.isKeyPressed(SDLK_RIGHT) || windowManager.isKeyPressed(SDLK_d) )
           {
               joueur.prepareMove(Character::CHARACTER_MOVEMENT::RIGHT);
           }
           if(windowManager.isKeyPressed(SDLK_LEFT) || windowManager.isKeyPressed(SDLK_q) || windowManager.isKeyPressed(SDLK_a))
           {
               joueur.prepareMove(Character::CHARACTER_MOVEMENT::LEFT);
           }
           if(windowManager.isKeyPressed(SDLK_w) )
           {

           }
           if(windowManager.isKeyPressed(SDLK_SPACE) )
           {
               joueur.jump(windowManager.getTime());
           }
           if(windowManager.isKeyPressed(SDLK_n) )
           {
               saveGame(joueur, joueur, map_test, &visualLoad, &windowManager);
           }



           while(windowManager.pollEvent(event)) {

               switch (event.type)
               {
                   case SDL_QUIT:
                       done = true; // Leave the loop after this iteration
                   break;

                   case SDL_KEYDOWN:

                       if(event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_RETURN)
                       {
                           done = true;
                       }

                       if (event.key.keysym.sym == SDLK_UP)
                       {
                           //camera.move(camera.FORWARD);
                           joueur.prepareMove(Character::CHARACTER_MOVEMENT::FORWARD);
                       }
                       else if (event.key.keysym.sym == SDLK_DOWN)
                       {
                           //camera.move(camera.BACKWARD);
                           joueur.prepareMove(Character::CHARACTER_MOVEMENT::BACKWARD);
                       }
                       else if (event.key.keysym.sym == SDLK_RIGHT)
                       {
                           //camera.move(camera.RIGHT);
                           joueur.prepareMove(Character::CHARACTER_MOVEMENT::RIGHT);
                       }
                       else if (event.key.keysym.sym == SDLK_LEFT)
                       {
                           //camera.move(camera.LEFT);
                           joueur.prepareMove(Character::CHARACTER_MOVEMENT::LEFT);
                       }
                       else if (event.key.keysym.sym == SDLK_c)
                       {
                           //camera.move(camera.DOWN);
                           joueur.toogleIsSwimming(true);
                       }
                       else if (event.key.keysym.sym == SDLK_x)
                       {
                           //camera.move(camera.DOWN);
                           joueur.toogleIsSwimming(false);
                       }
                       else if (event.key.keysym.sym == SDLK_SPACE)
                       {
                           joueur.jump(windowManager.getTime());
                           //camera.move(camera.UP);
                       }
                       else if(event.key.keysym.sym == SDLK_e)
                       {
                           Model tempModel(applicationPath, (std::string)(applicationPath.dirPath()+"assets/models/pioche/pioche.obj") );
                           ItemModel* tempItem = new ItemModel(tempModel, applicationPath, "pioche", 1, 1);
                           tempItem->set3DPosition(joueur.getPosition() + joueur.getHead().getForawrd()*2.f);
                           itemManager.addItem(tempItem);
                       }
                       else if(event.key.keysym.sym == SDLK_r)
                       {
                           ParticuleEmitter* tempEmitter = new ParticuleEmitter(applicationPath, applicationPath.dirPath()+"assets/textures/fire_unity.png" , ParticuleEmitter::TYPE_EMISSION::FIRE);
                           tempEmitter->setPosition(joueur.getPosition() + joueur.getHead().getForawrd()*2.f);
                           particuleManager.addEmitter(tempEmitter);
                       }
                       else if(event.key.keysym.sym == SDLK_l)
                       {
                           lightManager.addPointLight( new PointLight(joueur.getPosition(), glm::vec3(5,5,0)) );
                       }

                   break;
                   case SDL_MOUSEBUTTONDOWN:
                       if(event.button.button == SDL_BUTTON_LEFT )
                       {
                           joueur.swingSword(map_test, applicationPath, itemManager, particuleManager);
                       }
                       else if(event.button.button == SDL_BUTTON_RIGHT)
                       {
                           if(windowManager.isKeyPressed(SDLK_LSHIFT) )
                           {
                               joueur.dropItem(map_test, particuleManager, applicationPath, applicationPath.dirPath()+"assets/textures/fire_unity.png", TYPE_CUBE::BASE_FEU01);
                               //lightManager.addPointLight( new PointLight(joueur.getPosition(), glm::vec3(5,5,0)) );
                           }
                           else
                           {
                               joueur.dropItem(map_test, particuleManager, applicationPath, applicationPath.dirPath()+"assets/textures/fire_unity.png", TYPE_CUBE::TERRE01);

                           }
                       }
                   break;
                   case SDL_MOUSEMOTION :
                   joueur.updateTarget(map_test);
                   //camera.updateLookAt((event.motion.x - xPre)*camera.getVitesseLook(), (event.motion.y - yPre)*camera.getVitesseLook() );
                   joueur.updateLookAt((event.motion.x - xPre)*joueur.getHead().getVitesseLook(), (event.motion.y - yPre)*joueur.getHead().getVitesseLook());


                   SDL_WarpMouse((Uint16)WINDOW_WIDTH/2 , (Uint16)WINDOW_HEIGHT/2);
                   xPre = WINDOW_WIDTH / 2;
                   yPre = WINDOW_HEIGHT / 2;

                   break;

               }
           }
           joueur.updatePosition(map_test);
           joueur.updateForces(windowManager.getTime());



            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 Projection = glm::perspective(70.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

            // Camera matrix
            glm::mat4 View = glm::lookAt(
                glm::vec3(0,0,0), // Camera is at (4,3,3), in World Space
                joueur.getHead().getForawrd(), // and looks at the origin
                glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
                );

            skybox.draw(Projection, View);


                   // Camera matrix
            View = glm::lookAt(
                   joueur.getHead().getPosition(),//camera.getPosition(), // Camera is at (4,3,3), in World Space
                   joueur.getHead().getPosition()+ joueur.getHead().getForawrd(),//camera.getPosition() + camera.getForawrd(), // and looks at the origin
                   glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
                   );

           //chunk_test.Draw(Projection, View);
           lightManager.computeLights(View);
           //en 3 eme argument : map_test.getActiveChunkPosition(joueur.getPosition()) si ancienne version du draw :
           map_test.Draw(Projection, View, joueur.getPosition(), texture_cube, glm::normalize(joueur.getHead().getForawrd()), true, lightManager );

           itemManager.draw(Projection, View, texture_cube, windowManager.getTime()/400.f);

    //        test_model.Draw(Projection, View );

           particuleManager.update((windowManager.getTime() - tpPre)/1000.f, joueur.getHead().getPosition());
           particuleManager.draw(Projection, View);


           // Update the display
           windowManager.swapBuffers();

           tpAct = windowManager.getTime();
           if(tpAct - tpPre < 33 )
           {
               windowManager.delay(33 - (tpAct - tpPre) );
           }
           //std::cout<<1000/(float)(windowManager.getTime() - tpPre)<<std::endl;
           tpPre = tpAct;
       }

        glDeleteTextures(1, &texture_cube);
}









/*

int main(int argc, char** argv) {

    // Initialize SDL and open a window
    SDLWindowManager windowManager(WINDOW_WIDTH, WINDOW_HEIGHT, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    ////////////////////////////////
    // HERE SHOULD COME THE INITIALIZATION CODE
    ///////////////////////////////

         FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/tex2D.vs.glsl",
                                    applicationPath.dirPath() + "shaders/tex2D.fs.glsl");
    program.use();

    //pour la gestion des boucles infinies :
    bool doneGame = false;


    while(!doneGame)
    {

     //pour la gestion des boucles infinies :
     bool done = false;
     bool doneMenu = false;



    //gestion variable opengl :
     //SDL_EnableKeyRepeat(10, 10);
     glEnable(GL_DEPTH_TEST);
     glDepthFunc(GL_LESS);
     glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
     glHint(GL_FOG_HINT, GL_NICEST);
     glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
     glEnable (GL_LINE_SMOOTH);
     glEnable(GL_BLEND);
     glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

    // Menu debut :
     Menu menu(applicationPath, std::string(applicationPath.dirPath()+"assets/textures/menu_fond.png"));


    // menu loop:

    while(!doneMenu)
    {

    // Event loop:
    SDL_Event event;

    //evenements :

    while(windowManager.pollEvent(event))
    {

        switch (event.type)
        {
            case SDL_QUIT:
                doneGame = true;
                doneMenu = true;
                done = true;
            break;

            case SDL_KEYDOWN:

                if(event.key.keysym.sym == SDLK_ESCAPE )
                {
                    doneGame = true;
                    doneMenu = true;
                    done = true;
                }
                else if(event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_a ||  event.key.keysym.sym == SDLK_LEFT )
                {
                    menu.setSelectedOption(0);
                }
                else if( event.key.keysym.sym == SDLK_d ||  event.key.keysym.sym == SDLK_RIGHT )
                {
                    menu.setSelectedOption(1);
                }
                else if(event.key.keysym.sym == SDLK_RETURN )
                {
                    doneMenu = true;
                }
        }
    }


    //dessin :
    glClear(GL_COLOR_BUFFER_BIT );
        glDisable(GL_DEPTH_TEST);
            menu.Draw();
        glEnable(GL_DEPTH_TEST);
    windowManager.swapBuffers();

    }
    // Menu fin


    if(!done)
    {

        jeu(applicationPath, windowManager, menu.getSelectedOption());

    }

    }
    return EXIT_SUCCESS;
}

*/
