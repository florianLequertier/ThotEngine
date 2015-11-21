#include <cmath>
#include "thotEngine/FreeFlyCam.hpp"
#include "thotEngine/Transform.hpp"

namespace te{

FreeFlyCam::FreeFlyCam(): m_xPre(0), m_yPre(0), m_translationSpeed(1), m_rotationSpeed(0.5)
{

}

FreeFlyCam::~FreeFlyCam()
{

}

void FreeFlyCam::start()
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
    if (Input::getKey(SDLK_DOWN))
    {
        move(te::Camera::BACKWARD);
    }
    if (Input::getKey(SDLK_RIGHT))
    {
        move(te::Camera::RIGHT);
    }
    if (Input::getKey(SDLK_LEFT))
    {
        move(te::Camera::LEFT);
    }
    if (Input::getKey(SDLK_w))
    {
        move(te::Camera::DOWN);
    }
    if (Input::getKey(SDLK_SPACE))
    {
        move(te::Camera::UP);
    }

    if(Input::getMouse(SDL_BUTTON_LEFT))
    {
        updateLookAt((Input::getMousePosition().x - m_xPre)*m_rotationSpeed, (Input::getMousePosition().y - m_yPre)*m_rotationSpeed);

        SDL_WarpMouse((Uint16)WindowManager::getWindowDim().x / 2, (Uint16)WindowManager::getWindowDim().y / 2);
        m_xPre = WindowManager::getWindowDim().x / 2;
        m_yPre = WindowManager::getWindowDim().y / 2;
    }
}

void FreeFlyCam::updateLookAt(float x, float y)
{
    //m_alpha -= y*glm::pi<float>() / 180;
    //m_beta += x*glm::pi<float>() / 180;

    //m_transformPtr->rotateAround( (y*glm::pi<float>() / 180) , glm::vec3(0,1,0));
    //m_transformPtr->rotateAround( (x*glm::pi<float>() / 180) , glm::vec3(0,0,-1));

    m_transformPtr->localRotateAround( (y*glm::pi<float>() / 180),  Transform::getRight() );
    m_transformPtr->rotateAround( -(x*glm::pi<float>() / 180), Transform::getUp() );

    //m_forward = glm::vec3(-cosf(m_alpha)*sinf(m_beta), sinf(m_alpha), cosf(m_alpha)*cosf(m_beta));
    //m_right = glm::cross(m_forward, glm::vec3(0,1,0));
}

void FreeFlyCam::move(te::Camera::CAMERA_MOVEMENT mouvement)
{
    switch(mouvement)
    {
    case te::Camera::FORWARD :
        m_transformPtr->localTranslate(m_translationSpeed * Transform::getForward());
        break;
    case te::Camera::BACKWARD :
        m_transformPtr->localTranslate(-m_translationSpeed * Transform::getForward());
        break;
    case te::Camera::RIGHT:
        m_transformPtr->localTranslate(-m_translationSpeed * Transform::getRight());
        break;
    case te::Camera::LEFT:
        m_transformPtr->localTranslate(m_translationSpeed * Transform::getRight());
        break;
    case te::Camera::UP :
        m_transformPtr->localTranslate(m_translationSpeed * Transform::getUp());
        break;
    case te::Camera::DOWN :
        m_transformPtr->localTranslate(-m_translationSpeed * Transform::getUp());
        break;
    }
}

}
