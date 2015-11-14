#pragma once

#include <cstdint>
#include <SDL/SDL.h>
#include "glm.hpp"

namespace te {

class WindowManager {

private:
    static int m_windowWidth;
    static int m_windowHeight;

public:
    WindowManager(uint32_t width, uint32_t height, const char* title);

    ~WindowManager();

    static void swapBuffers();

    static int getTime();

    static void delay(int delta);

    static glm::ivec2 getWindowDim();
};

}
