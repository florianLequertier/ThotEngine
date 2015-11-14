#include <cmath>
#include "thotEngine/FreeFlyCam.hpp"
#include "thotEngine/Transform.hpp"

namespace te{

FreeFlyCam::FreeFlyCam(): m_xPre(0), m_yPre(0), m_translationSpeed(1), m_rotationSpeed(1)
{

}

FreeFlyCam::~FreeFlyCam()
{

}

void FreeFlyCam::init()
{
    m_transformPtr = transform();
    //m_translationSpeed = 1;
    //m_rotationSpeed = 1;
}

void FreeFlyCam::update()
{
    if (Input::getKey(SDLK_UP))
    {
        move(te::Camera::FORWARD);
    }
    else if (Input::getKey(SDLK_DOWN))
    {
        move(te::Camera::BACKWARD);
    }
    else if (Input::getKey(SDLK_RIGHT))
    {
        move(te::Camera::RIGHT);
    }
    else if (Input::getKey(SDLK_LEFT))
    {
        move(te::Camera::LEFT);
    }
    else if (Input::getKey(SDLK_w))
    {
        move(te::Camera::DOWN);
    }
    else if (Input::getKey(SDLK_SPACE))
    {
        move(te::Camera::UP);
    }

    updateLookAt((Input::getMousePosition().x - m_xPre)*m_rotationSpeed, (Input::getMousePosition().y - m_yPre)*m_rotationSpeed);

    SDL_WarpMouse((Uint16)800 / 2, (Uint16)600 / 2);
    m_xPre = 800 / 2;
    m_yPre = 600 / 2;
}

void FreeFlyCam::updateLookAt(float x, float y)
{
    //m_alpha -= y*glm::pi<float>() / 180;
    //m_beta += x*glm::pi<float>() / 180;

    m_transformPtr->rotateAround( (y*glm::pi<float>() / 180) , glm::vec3(1,0,0));
    m_transformPtr->rotateAround( (x*glm::pi<float>() / 180) , glm::vec3(0,0,1));

    //m_forward = glm::vec3(-cosf(m_alpha)*sinf(m_beta), sinf(m_alpha), cosf(m_alpha)*cosf(m_beta));
    //m_right = glm::cross(m_forward, glm::vec3(0,1,0));
}

void FreeFlyCam::move(te::Camera::CAMERA_MOVEMENT mouvement)
{
    switch(mouvement)
    {
    case te::Camera::FORWARD :
        m_transformPtr->localTranslate(m_translationSpeed, 0, 0);
        break;
    case te::Camera::BACKWARD :
        m_transformPtr->localTranslate(-m_translationSpeed, 0, 0);
        break;
    case te::Camera::RIGHT:
        m_transformPtr->localTranslate(0, m_translationSpeed, 0);
        break;
    case te::Camera::LEFT:
        m_transformPtr->localTranslate(0, -m_translationSpeed, 0);
        break;
    case te::Camera::UP :
        m_transformPtr->localTranslate(0, 0, m_translationSpeed);
        break;
    case te::Camera::DOWN :
        m_transformPtr->localTranslate(0, 0, -m_translationSpeed);
        break;
    }
}

}
