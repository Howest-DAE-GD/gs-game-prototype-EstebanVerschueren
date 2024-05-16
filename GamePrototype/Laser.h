#pragma once
#include "utils.h"

class Laser
{
public:
    Laser(float screenWidth, float screenHeight);
    void Update(float deltaTime);
    void Draw() const;
    bool IsActive() const;

    bool IsRed() const;
    bool Intersects(const Rectf& playerRect) const;

private:
    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_Timer;
    float m_Thickness;
    bool m_Active;

    Point2f m_StartPoint;
    Point2f m_EndPoint1;
    Point2f m_EndPoint2;
    Color4f m_Color;

    void ResetLaser();
};
