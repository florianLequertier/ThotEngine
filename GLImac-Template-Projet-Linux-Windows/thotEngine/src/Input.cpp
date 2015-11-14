#include "ThotEngine/Input.hpp"

namespace te {

//std::vector<SDLKey> Input::m_keyPressed;
//std::vector<Uint8> Input::m_mouseButtonPressed;

//std::vector<SDLKey> Input::m_keyJustPressed;
//std::vector<SDLKey> Input::m_keyJustReleased;
//std::vector<Uint8> Input::m_mouseButtonJustPressed;
//std::vector<Uint8> Input::m_mouseButtonJustReleased;

//glm::vec2 Input::m_mousePosition;

//glm::ivec2 m_mousePos(0,0);

//Input::Input()
//{
//}


//Input::~Input()
//{
//}

void Input::updateEvents()
{
    SDL_PumpEvents();
}

bool Input::getKey(SDLKey key)
{
    return SDL_GetKeyState(nullptr)[key];
}

bool Input::getMouse(uint32_t button)
{
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button);
}

glm::ivec2 Input::getMousePosition()
{
    glm::ivec2 m_mousePos;
    SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
    return m_mousePos;
}

bool Input::pollEvent(SDL_Event& e)
{
    return SDL_PollEvent(&e);
}



//void Input::reset()
//{
//    m_keyPressed.clear();
//    m_mouseButtonPressed.clear();
//}

//void Input::update(const SDL_Event& event)
//{
//    m_keyJustPressed.clear();
//    m_keyJustReleased.clear();
//    m_mouseButtonJustPressed.clear();
//    m_mouseButtonJustReleased.clear();

//    switch (event.type)
//    {
//    case SDL_KEYDOWN:
//        m_keyPressed.push_back(event.key.keysym.sym);
//        m_keyJustPressed.push_back(event.key.keysym.sym);
//        std::cout << "key pressed : " << std::to_string(event.key.keysym.sym) << std::endl;
//        break;
//    case SDL_KEYUP:
//        m_keyPressed.erase(std::find(m_keyPressed.begin(), m_keyPressed.end(), event.key.keysym.sym));
//        m_keyJustReleased.push_back(event.key.keysym.sym);
//        break;
//    case SDL_MOUSEBUTTONDOWN:
//        m_mouseButtonPressed.push_back(event.button.button);
//        m_mouseButtonJustPressed.push_back(event.button.button);
//        break;
//    case SDL_MOUSEBUTTONUP:
//        m_mouseButtonPressed.erase(std::find(m_mouseButtonPressed.begin(), m_mouseButtonPressed.end(), event.button.button));
//        m_mouseButtonJustReleased.push_back(event.button.button);
//        break;
//    case SDL_MOUSEMOTION:
//        m_mousePosition.x = event.motion.x;
//        m_mousePosition.y = event.motion.y;
//        break;
//    }
//}

//bool Input::getKeyDown(SDLKey key)
//{
//    if (std::find(m_keyJustPressed.begin(), m_keyJustPressed.end(), key) != m_keyJustPressed.end())
//    {
//        return true;
//    }
//    return false;
//}

//bool Input::getKey(SDLKey key)
//{
//    if (std::find(m_keyPressed.begin(), m_keyPressed.end(), key) != m_keyPressed.end())
//    {
//        return true;
//    }
//    return false;
//}

//bool Input::getKeyUp(SDLKey key)
//{
//    if (std::find(m_keyJustReleased.begin(), m_keyJustReleased.end(), key) != m_keyJustReleased.end())
//    {
//        return true;
//    }
//    return false;
//}

//bool Input::getMouseDown(Uint8 button)
//{
//    if (std::find(m_mouseButtonJustPressed.begin(), m_mouseButtonJustPressed.end(), button) != m_mouseButtonJustPressed.end())
//    {
//        return true;
//    }
//    return false;
//}

//bool Input::getMouse(Uint8 button)
//{
//    if (std::find(m_mouseButtonPressed.begin(), m_mouseButtonPressed.end(), button) != m_mouseButtonPressed.end())
//    {
//        return true;
//    }
//    return false;
//}

//bool Input::getMouseUp(Uint8 button)
//{
//    if (std::find(m_mouseButtonJustReleased.begin(), m_mouseButtonJustReleased.end(), button) != m_mouseButtonJustReleased.end())
//    {
//        return true;
//    }
//    return false;
//}

//glm::vec2 Input::getMousePosition()
//{
//    return m_mousePosition;
//}

//void Input::setMousePosition(const glm::vec2& mousePosition)
//{
//    SDL_WarpMouse(mousePosition.x, mousePosition.y);
//}

}

