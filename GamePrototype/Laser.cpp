#include "pch.h"
#include "Laser.h"
#include "utils.h"
#include <cmath>
#include <iostream>

Laser::Laser(float screenWidth, float screenHeight, VoidCircle* voidcirle)
    : m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight), m_VoidCircle(voidcirle), m_Timer(0.0f), m_Thickness(2.0f), m_Active(false), m_Color(Color4f{1.0f, 0.0f, 1.0f, 1.0f})
{
    ResetLaser();
}

void Laser::Update(float deltaTime)
{
	if (previusValue != m_Active)
    {
		ResetLaser();
		previusValue = m_Active;
	}

	if (!m_Active) return;
    m_Timer += deltaTime;
    if (m_Timer >= 0.75f)
    {
        m_Color = Color4f{ 1.0f, 0.0f, 0.0f, 1.0f };
        m_Thickness = 400.0f;
		m_IsRed = true;
        if (m_Timer >= 1.0f)
        {
            m_Active = false;
            previusValue = m_Active;
			m_IsRed = false;
        }
    }
}

void Laser::Draw() const
{
    if (!m_Active) return;

    utils::SetColor(m_Color);
    utils::DrawLine(m_StartPoint.x, m_StartPoint.y, m_EndPoint1.x, m_EndPoint1.y, m_Thickness);
    utils::DrawLine(m_StartPoint.x, m_StartPoint.y, m_EndPoint2.x, m_EndPoint2.y, m_Thickness);
}

bool Laser::IsActive() const
{
    return m_Active;
}

void Laser::ResetLaser()
{
    m_Timer = 0.0f;
    m_Thickness = 5.0f;
    m_Active = true;
    m_Color = Color4f{ 1.0f, 0.0f, 1.0f, 1.0f };

    // Circle parameters
    float circleRadius = m_VoidCircle->GetRadius() - 40; // Circle radius
    float centerX = m_ScreenWidth / 2.0f;
    float centerY = m_ScreenHeight / 2.0f;

    // Random point on the circle's circumference
    float angle1 = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;
    m_StartPoint.x = centerX + circleRadius * cos(angle1);
    m_StartPoint.y = centerY + circleRadius * sin(angle1);

    // Random angle for the laser direction
    float angle2 = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;

    // Calculate the endpoints using the random angle
    float length = std::max(m_ScreenWidth, m_ScreenHeight); // Make sure the line is long enough to go off-screen
    m_EndPoint1.x = m_StartPoint.x + length * cos(angle2);
    m_EndPoint1.y = m_StartPoint.y + length * sin(angle2);
    m_EndPoint2.x = m_StartPoint.x - length * cos(angle2);
    m_EndPoint2.y = m_StartPoint.y - length * sin(angle2);
}

bool Laser::IsRed() const
{
    return m_Color.r == 1.0f && m_Color.g == 0.0f && m_Color.b == 0.0f;
}

bool Laser::Intersects(const Rectf& playerRect) const {

	std::cout << "Intersects" << std::endl;
    if (!m_Active || !m_IsRed) return false;

	std::cout << "Intersects truly" << std::endl;
    // Check intersection with both lines of the laser
	if (utils::IsOverlapping(m_StartPoint, m_EndPoint1, playerRect) || utils::IsOverlapping(m_StartPoint, m_EndPoint2, playerRect))
    {
		return true;
    }
}