#include "thotEngine/WindowManager.hpp"
#include <iostream>

namespace te {

int WindowManager::m_windowWidth = 800;
int WindowManager::m_windowHeight = 600;

WindowManager::WindowManager(uint32_t width, uint32_t height, const char* title)
{

    m_windowWidth = width;
    m_windowHeight = height;

    if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    if(!SDL_SetVideoMode(width, height, 32, SDL_OPENGL)) {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    SDL_WM_SetCaption(title, nullptr);
}

WindowManager::~WindowManager()
{
    SDL_Quit();
}

void WindowManager::swapBuffers()
{
    SDL_GL_SwapBuffers();
}

int WindowManager::getTime()
{
    return SDL_GetTicks();
}

void WindowManager::delay(int delta)
{
    SDL_Delay(delta);
}

glm::ivec2 WindowManager::getWindowDim()
{
    return glm::ivec2(m_windowWidth, m_windowHeight);
}

}
