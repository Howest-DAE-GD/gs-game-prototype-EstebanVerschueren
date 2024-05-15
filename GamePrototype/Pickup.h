#pragma once
#include "structs.h"

class Pickup
{
public:
    enum class Type
    {
        Health,
        // Add more types if needed
    };

    Pickup(Type type, const Point2f& position);
    ~Pickup();

    void Update(float deltaTime);
    void Draw() const;
    bool CheckCollision(const Rectf& rect) const;
    bool IsActive() const;
    void SetActive(bool active);
    Type GetType() const;

private:
    Type m_Type;
    Point2f m_Position;
    bool m_Active;

	float m_Size{ 10.0f };
};

