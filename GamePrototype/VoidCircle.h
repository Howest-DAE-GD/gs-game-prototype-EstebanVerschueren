#pragma once
#include "texture.h"
class VoidCircle
{
public:
    VoidCircle();
    ~VoidCircle();

    void Draw() const;
    void Update(float elapsedSec);

    void DrawTexture() const;

    float GetRadius() const;
    bool IsPointInside(const Point2f& point) const;
    void IncreaseRadius(float amount); // Add this line
    void SetShrinkSpeed(float speed); // Add this line
    float GetShrinkSpeed() const; // Add this line
    float GetMaxRadius() const { return m_MaxRadius; }

private:
    float m_Radius{};
    float m_ShrinkSpeed{ 0.05f };
    const float m_MaxRadius{ 720.0f / 2 }; // Maximum radius
    Texture* m_voidCircleTexture{};
};
