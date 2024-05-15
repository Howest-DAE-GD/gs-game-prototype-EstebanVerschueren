#include "pch.h"
#include "Pickup.h"
#include "utils.h"
#include <iostream>

Pickup::Pickup(Type type, const Point2f& position)
    : m_Type(type), m_Position(position), m_Active(true)
{
}

Pickup::~Pickup()
{
}

void Pickup::Update(float deltaTime)
{
    // Update logic if any
}

void Pickup::Draw() const
{
    if (m_Active)
    {
		utils::SetColor(Color4f{ 0.0f, 0.0f, 1.0f, 1.0f });
		utils::FillRect(m_Position.x - m_Size / 2, m_Position.y - m_Size*2 / 2, m_Size, m_Size*2);
    }
}

bool Pickup::CheckCollision(const Rectf& rect) const
{
    if (!m_Active)
        return false;

    Rectf pickupRect{ m_Position.x - m_Size / 2, m_Position.y - m_Size * 2 / 2, m_Size, m_Size * 2 };
    return utils::IsOverlapping(pickupRect, rect);
}

bool Pickup::IsActive() const
{
    return m_Active;
}

void Pickup::SetActive(bool active)
{
    m_Active = active;
}

Pickup::Type Pickup::GetType() const
{
    return m_Type;
}
