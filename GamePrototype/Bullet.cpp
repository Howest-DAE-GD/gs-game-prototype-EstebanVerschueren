#include "pch.h"
#include "Bullet.h"

Bullet::Bullet(const Point2f& start, const Point2f& end, const VoidCircle& voidCircle)
    : m_Start(start), m_End(end), m_CurrentPosition(start), m_Speed(500.0f), m_Active(true), m_ElapsedTime(0.0f), m_Color{ 1.0f, 0.0f, 0.0f, 1.0f }, m_VoidCircle(voidCircle)  // Default color red
{
}

Bullet::~Bullet()
{
}

void Bullet::Update(float deltaTime)
{
    if (!m_Active) return;

    m_ElapsedTime += deltaTime;
    m_CurrentPosition = Lerp(m_Start, m_End, m_ElapsedTime * m_Speed / utils::GetDistance(m_Start, m_End));

    if (!m_VoidCircle.IsPointInside(m_CurrentPosition))
    {
        m_Active = false;
    }
}

void Bullet::Draw() const
{
    if (!m_Active) return;

    utils::SetColor(m_Color);  // Use the color
    utils::FillRect(m_CurrentPosition.x, m_CurrentPosition.y, 10.0f, 10.0f);
}

bool Bullet::IsActive() const
{
    return m_Active;
}

bool Bullet::CheckCollision(const Rectf& rect)
{
    if (utils::IsPointInRect(m_CurrentPosition, rect))
    {
        m_Active = false;
        return true;
    }
    return false;
}


Point2f Bullet::Lerp(const Point2f& start, const Point2f& end, float t)
{
    return Point2f(start.x * (1 - t) + end.x * t, start.y * (1 - t) + end.y * t);
}

void Bullet::SetColor(const Color4f& color)  // Implement SetColor
{
    m_Color = color;
}

void Bullet::SetSpeed(float speed)  // Implement SetSpeed
{
    m_Speed = speed;
}
