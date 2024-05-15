#pragma once
#include "vector"
#include "Texture.h"
#include "Bullet.h"
#include "Pickup.h"

class PlayerController
{
public:
    PlayerController(const VoidCircle& voidCircle);
    ~PlayerController();

    void Draw() const;
    void Update(float elapsedSec);
    void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
    void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
    void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);

    Point2f GetPlayerPosition() const;
    Rectf GetPlayerRect() const;
    void DecrementHealth();

    int GetHealth() const;
    std::vector<Bullet*> m_Bullets;
    void HandlePickup(Pickup* pickup);

private:
    std::vector<Point2f>* m_Points{};
    std::vector<Point2f>* m_InitialPoints{};

    Point2f m_Position{};
    float m_PlayerSize{ 20.0f };
    Point2f m_PlayerDirection{};
    Point2f m_MousePos{};
    int m_Health{ 5 };
    float m_BulletCooldown{ 0.2f };
    float m_TimeSinceLastShot{ 0.0f };

    void CalculateAngle();
    float angle{ 0.0f };
    void UpdateTriangle();
    void InitializeTriangle();

    void HandleInput();
    Point2f m_Direction{};

    void Move(float elapsedSec);
    void DrawHealthCircles() const;
    float m_Speed{ 200.0f };

    void ShootBullet();
    void ShootShotgun();
    const VoidCircle& m_voidCircle;

    bool m_IsShooting{ false };
    float m_ShotgunCooldown{ 1.0f };
    float m_TimeSinceLastShotgun{ 0.0f };
};
