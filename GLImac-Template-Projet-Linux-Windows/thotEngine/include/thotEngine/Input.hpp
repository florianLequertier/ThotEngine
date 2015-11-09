#ifndef INCLUDE_HPP
#define INCLUDE_HPP

#include <cstdlib>
#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include "glm.hpp"

namespace te{

class Input
{
private:
    static std::vector<SDLKey> m_keyPressed;
    static std::vector<Uint8> m_mouseButtonPressed;

    static std::vector<SDLKey> m_keyJustPressed;
    static std::vector<SDLKey> m_keyJustReleased;
    static std::vector<Uint8> m_mouseButtonJustPressed;
    static std::vector<Uint8> m_mouseButtonJustReleased;

    static glm::vec2 m_mousePosition;

public:

    static void reset();
    static void update(const SDL_Event& event);

    static bool getKeyDown(SDLKey key);
    static bool getKey(SDLKey key);
    static bool getKeyUp(SDLKey key);

    static bool getMouseDown(Uint8 button);
    static bool getMouse(Uint8 button);
    static bool getMouseUp(Uint8 button);

    static glm::vec2 getMousePosition();
    static void setMousePosition(const glm::vec2& mousePosition);

public:
    Input();
    ~Input();
};

}

#endif // INCLUDE_HPP
