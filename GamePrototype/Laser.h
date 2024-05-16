#pragma once
#include "utils.h"
#include "VoidCircle.h"

class Laser
{
public:
    Laser(float screenWidth, float screenHeight, VoidCircle* voidcirle);
    void Update(float deltaTime);
    void Draw() const;
    bool IsActive() const;

    bool IsRed() const;
    bool Intersects(const Rectf& playerRect) const;
    bool m_Active;

private:
    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_Timer;
    float m_Thickness;

    Point2f m_StartPoint;
    Point2f m_EndPoint1;
    Point2f m_EndPoint2;
    Color4f m_Color;

    void ResetLaser();

	bool previusValue = false;

	bool m_IsRed{ false };

	VoidCircle* m_VoidCircle;
};
