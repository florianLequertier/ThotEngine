#ifndef FREEFLYCAM_HPP
#define FREEFLYCAM_HPP

#include "WindowManager.hpp"
#include "Camera.hpp"
#include "Component.hpp"
#include "CArray.hpp"
#include "Script.hpp"
#include "Input.hpp"

namespace te{

class FreeFlyCam : public Script
{
private:
    ExternalHandler<Transform> m_transformPtr;

    float m_xPre;
    float m_yPre;
    float m_translationSpeed;
    float m_rotationSpeed;

public:
    FreeFlyCam();
    ~FreeFlyCam();

    virtual void start() override;
    virtual void update() override;

    void updateLookAt(float x, float y);
    void move(te::Camera::CAMERA_MOVEMENT mouvement);
};

}


#endif // FREEFLYCAM_HPP
