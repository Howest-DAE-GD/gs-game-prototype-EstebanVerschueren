// Enemy.h
#pragma once
#include "vector"
#include "Bullet.h"
#include "Pickup.h"
#include "PlayerController.h"

class Enemy
{
public:
    Enemy(float radius, const VoidCircle& voidCircle, float movementDuration, float movementInterval);
    ~Enemy();

    void Draw() const;
    void Update(float elapsedSec, Point2f playerPos, float radius);
    void ShootBullet(Point2f playerPos);
    std::vector<Bullet*> m_Bullets;

    void DecrementHealth();
    bool IsAlive() const;
    Point2f GetPosition() const { return m_Position; }
    Rectf GetEnemyRect() const;

    std::vector<Pickup*> GetPickups() const;
    void ClearBullets(); 
    void SetPlayerController(PlayerController* player);

	float GetMovementDuration() const { return moveDuration; }
	float GetIntervalBetweenMovement() const { return m_IntervalBetweenMovement; }

	float SetMovementDuration(float duration) { return moveDuration = duration; }
	float SetIntervalBetweenMovement(float interval) { return m_IntervalBetweenMovement = interval; }


private:
    float m_ShootInterval;
    float m_ShootTimer;
    const VoidCircle& m_VoidCircle;
    bool m_MovingToPoint{ false };

    Point2f m_Position{};
    float m_EnemySize{ 20.0f };
    float m_Angle{};
    float m_Speed{ 100.0f }; // Movement speed along the circle

    void CalculateAngle(Point2f playerPos);
    float angle{ 0.0f };
    void UpdateTriangle();
    void InitializeTriangle();
    Point2f generateRandomSpawnPoint(float radius);
    Point2f lerp(const Point2f& start, const Point2f& end, float t);
    float timeElapsed{};
    Point2f targetPosition{};
    Point2f startPosition{};

    std::vector<Point2f>* m_Points{};
    std::vector<Point2f>* m_InitialPoints{};

    Point2f generateRandomPositionInsideCircle(float radius);
    bool IsTooCloseToEdge(float threshold) const;

    void HandlePhases(float deltaTime);
    void Phase1(float deltaTime);
    void Phase2(float deltaTime);
    void Phase3(float deltaTime);

    bool m_Phase1{ false };
    bool m_Phase2{ false };
    bool m_Phase3{ false };  // Add phase 3 flag

    float circleRadius{ 0.0f };
    float moveDuration{ 4.0f }; // duration of the movement

    int m_Health{ 3 };

    float m_TimeInsideCircle{ 0.0f };  // Timer to track time inside the circle
    float m_MaxTimeInsideCircle{ 10.0f };  // Max time before retreating
    bool m_IsRetreating{ false };  // Flag to check if the enemy is retreating

    float m_RetreatTimer{ 0.0f };  // Timer for phase 3
    Point2f m_RetreatPosition{};   // Target position for retreat

    std::vector<Pickup*> m_Pickups;
    void DropPickups();

    bool m_PickupDrop{ false };
    PlayerController* m_Player{ nullptr };

	float m_IntervalBetweenMovement{ 3.0f }; // Interval between movements

};
