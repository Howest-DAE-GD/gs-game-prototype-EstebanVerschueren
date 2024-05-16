#pragma once
#include "vector"
#include "Texture.h"
#include "Bullet.h"
#include "Pickup.h"
#include "VoidCircle.h"
#include <algorithm> // For std::max and std::min

class PlayerController
{
public:
    PlayerController(VoidCircle& voidCircle);
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
    float GetHealthPickupProbability(int health, int maxHealth);
    float GetIncreaseRadiusProbability(float radius, float maxRadius);
	int m_MaxHealth{ 5 };
    void TakeDamageFromLaser();

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
    VoidCircle& m_voidCircle;

    bool m_IsShooting{ false };
    float m_ShotgunCooldown{ 1.0f };
    float m_TimeSinceLastShotgun{ 0.0f };

    float m_TimeOutsideCircle{ 0.0f }; // Timer to track time outside the circle

    Point2f m_KnockbackForce; // Knockback force to be applied
    float m_KnockbackDecay;   // How quickly the knockback force decays


    // Utility function to clamp a value between a min and max range
    template <typename T>
    T Clamp(T value, T min, T max)
    {
        return std::max(min, std::min(value, max));
    }


	float m_LaserDamageCooldown{ 0.0f };
};
