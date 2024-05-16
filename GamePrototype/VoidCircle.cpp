#include "pch.h"
#include "VoidCircle.h"
#include "utils.h"

VoidCircle::VoidCircle()
{
    m_Radius = 720.0f / 2;
    m_voidCircleTexture = new Texture("voidcircleTexture.png");
}

VoidCircle::~VoidCircle()
{
}

void VoidCircle::Draw() const
{
    utils::SetColor(Color4f{ 1.0f, 1.0f, 1.0f, 1.0f });
    utils::FillEllipse(1280.0f / 2.0f, 720.0f / 2.0f, m_Radius, m_Radius);
}

void VoidCircle::Update(float deltaTime)
{
    m_Radius -= m_ShrinkSpeed * deltaTime;
}

void VoidCircle::DrawTexture() const
{
    float width = 1280.0f + m_Radius * 30;
    float height = 720.0f + m_Radius * 30 / 1280 * 720;

    Rectf destRect = Rectf{ 1280 / 2 - (width / 2), 720 / 2 - (height / 2), width, height };
    Rectf sourceRect = Rectf{ 0.0f, 0.0f, m_voidCircleTexture->GetWidth(), m_voidCircleTexture->GetHeight() };

    m_voidCircleTexture->Draw(destRect, sourceRect);
}

float VoidCircle::GetRadius() const
{
    return m_Radius;
}

bool VoidCircle::IsPointInside(const Point2f& point) const
{
    float centerX = 1280.0f / 2.0f;
    float centerY = 720.0f / 2.0f;
    float distance = sqrt(pow(point.x - centerX, 2) + pow(point.y - centerY, 2));
    return distance < m_Radius;
}

void VoidCircle::IncreaseRadius(float amount) // Add this method
{
    m_Radius = std::min(m_Radius + amount, m_MaxRadius);
}

void VoidCircle::SetShrinkSpeed(float speed) // Add this method
{
    m_ShrinkSpeed = speed;
}

float VoidCircle::GetShrinkSpeed() const // Add this method
{
    return m_ShrinkSpeed;
}
