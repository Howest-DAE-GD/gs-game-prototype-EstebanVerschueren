#pragma once
#include "structs.h"
#include "utils.h"
#include "VoidCircle.h"

class Bullet
{
public:
    Bullet(const Point2f& start, const Point2f& end, const VoidCircle& voidCircle);
    ~Bullet();

    void Update(float deltaTime);
    void Draw() const;
    bool IsActive() const;
    bool CheckCollision(const Rectf& rect);
    void SetColor(const Color4f& color);
    void SetSpeed(float speed);


private:
    Point2f m_Start;
    Point2f m_End;
    Point2f m_CurrentPosition;
    float m_Speed;
    bool m_Active;
    float m_ElapsedTime;
    Color4f m_Color{ Color4f{ 1.0f, 0.0f, 0.0f, 1.0f } };

    const VoidCircle& m_VoidCircle;

    Point2f Lerp(const Point2f& start, const Point2f& end, float t);
};
