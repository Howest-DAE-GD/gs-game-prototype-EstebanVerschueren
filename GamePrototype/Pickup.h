// Pickup.h
#pragma once
#include "structs.h"
#include "VoidCircle.h"

class Pickup
{
public:
    enum class Type
    {
        Health,
        IncreaseRadius, // Add this line
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
    Point2f GetPosition() const;


private:
    Type m_Type;
    Point2f m_Position;
    bool m_Active;
    float m_Size{ 10.0f };

};
