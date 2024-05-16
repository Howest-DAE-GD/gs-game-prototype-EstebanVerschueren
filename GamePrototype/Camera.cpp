#include "pch.h"
#include "Camera.h"
#include <iostream>


Camera::Camera(float screenWidth, float screenHeight) :
	m_ScreenWidth(screenWidth),
	m_ScreenHeight(screenHeight)
{
}

void Camera::Aim(const Point2f& trackCenter)
{
	float camX = trackCenter.x - m_ScreenWidth / 2.0f;
	float camY = trackCenter.y - m_ScreenHeight / 2.0f;


    if (m_ShakeTimeLeft > 0)
    {
        camX += (static_cast<float>(rand()) / RAND_MAX * 2 - 1) * m_ShakeMagnitude;
        camY += (static_cast<float>(rand()) / RAND_MAX * 2 - 1) * m_ShakeMagnitude;
    }


	glPushMatrix();
	glTranslatef(-camX, -camY, 0);


}

void Camera::Aim(float levelW, float levelH, Point2f trackCenter)
{
    float camX = trackCenter.x - m_ScreenWidth / 2.0f;
    float camY = trackCenter.y - m_ScreenHeight / 2.0f;

    if (camX > levelW) camX = levelW;
    if (camX < 0) camX = 0;
    if (camY > levelH) camY = levelH;
    if (camY < 0) camY = 0;

    if (m_ShakeTimeLeft > 0)
    {
        camX += (static_cast<float>(rand()) / RAND_MAX * 2 - 1) * m_ShakeMagnitude;
        camY += (static_cast<float>(rand()) / RAND_MAX * 2 - 1) * m_ShakeMagnitude;
    }

    glPushMatrix();
    glTranslatef(-camX, -camY, 0);
}

void Camera::Update(float elapsedSec)
{

    if (m_ShakeTimeLeft > 0)
    {
        m_ShakeTimeLeft -= elapsedSec;
        if (m_ShakeTimeLeft < 0)
        {
            m_ShakeTimeLeft = 0;
            
        }
    }
}

void Camera::Reset()
{
	glPopMatrix();
}

void Camera::Shake(float duration, float magnitude)
{
	m_ShakeDuration = duration;
	m_ShakeMagnitude = magnitude;
	m_ShakeTimeLeft = duration;
}