#include "pch.h"
#include "Enemy.h"
#include "utils.h"
#include <ctime>
#include <iostream>

Enemy::Enemy(float radius, const VoidCircle& voidCircle)
    : m_ShootInterval(1.0f), m_ShootTimer(0.0f), m_VoidCircle(voidCircle), m_Health(3)
{
    m_Points = new std::vector<Point2f>();
    m_Position = generateRandomSpawnPoint(radius);
    InitializeTriangle();
    m_Phase1 = true;
    m_RetreatTimer = static_cast<float>(std::rand() % 16 + 5);
}

Enemy::~Enemy()
{
    delete m_Points;
    for (Bullet* bullet : m_Bullets)
    {
        delete bullet;
    }
    m_Bullets.clear();
    for (Pickup* pickup : m_Pickups)
    {
        delete pickup;
    }
    m_Pickups.clear();
}


void Enemy::Draw() const
{
    if (!IsAlive()) return;  // Do not draw if the enemy is dead

    utils::SetColor(Color4f(1.0f, 0.0f, 0.0f, 1.0f));
    utils::FillPolygon(m_Points->data(), m_Points->size());

    for (Bullet* bullet : m_Bullets)
    {
        bullet->Draw();
    }

}

void Enemy::Update(float deltaTime, Point2f playerPos, float radius)
{
    if (!IsAlive()) return;  // Do not update if the enemy is dead

    circleRadius = radius;

    CalculateAngle(playerPos);
    UpdateTriangle();
    HandlePhases(deltaTime);

    m_ShootTimer += deltaTime;
    if (m_ShootTimer >= m_ShootInterval)
    {
        ShootBullet(playerPos);
        m_ShootTimer = 0.0f;
    }

    for (Bullet* bullet : m_Bullets)
    {
        bullet->Update(deltaTime);
    }

    // Remove inactive bullets
    m_Bullets.erase(std::remove_if(m_Bullets.begin(), m_Bullets.end(), [](Bullet* bullet)
        {
            return !bullet->IsActive();
        }), m_Bullets.end());
}

void Enemy::CalculateAngle(Point2f playerPos)
{
    angle = atan2(playerPos.y - m_Position.y, playerPos.x - m_Position.x);
}

void Enemy::UpdateTriangle()
{
    InitializeTriangle();
    for (size_t i = 0; i < m_Points->size(); ++i)
    {
        float dx = m_Points->at(i).x - m_Position.x;
        float dy = m_Points->at(i).y - m_Position.y;
        m_Points->at(i).x = m_Position.x + dx * cos(angle) - dy * sin(angle);
        m_Points->at(i).y = m_Position.y + dx * sin(angle) + dy * cos(angle);
    }
}

void Enemy::InitializeTriangle()
{
    m_Points->clear();
    m_Points->push_back(Point2f(m_Position.x + m_EnemySize, m_Position.y)); // Top point
    m_Points->push_back(Point2f(m_Position.x - m_EnemySize, m_Position.y + m_EnemySize)); // Bottom left
    m_Points->push_back(Point2f(m_Position.x - m_EnemySize, m_Position.y - m_EnemySize)); // Bottom right
}

void Enemy::HandlePhases(float deltaTime)
{
    if (m_Phase1)
    {
        Phase1(deltaTime);
    }
    else if (m_Phase2)
    {
        Phase2(deltaTime);
    }
    else if (m_Phase3)
    {
        Phase3(deltaTime);
    }
}

void Enemy::Phase1(float deltaTime)
{
    timeElapsed += deltaTime;
    float duration{ 2.0f }; // duration of the interpolation
    float t = timeElapsed / duration; // duration is the total time for the interpolation
    if (t >= 1.0f)
    {
        m_Position = targetPosition;
        targetPosition = m_Position;
        m_Phase1 = false;
        m_Phase2 = true;
    }
    else
    {
        m_Position = lerp(startPosition, targetPosition, t);
    }
}

void Enemy::Phase2(float deltaTime)
{
    static float timeUntilNextMove = static_cast<float>(std::rand()) / RAND_MAX * 15.0f; // Move every 0-5 seconds

    // If the enemy is moving to a point, continue moving
    if (m_MovingToPoint)
    {
        timeElapsed += deltaTime;
        float t = timeElapsed / moveDuration;
        if (t >= 1.0f)
        {
            m_Position = targetPosition;
            m_MovingToPoint = false;
            timeElapsed = 0.0f;  // Reset the time elapsed for the next movement
        }
        else
        {
            m_Position = lerp(startPosition, targetPosition, t);
        }
    }
    else
    {
        timeElapsed += deltaTime;

        // Check if it's time to move to a new random position or if the enemy is too close to the edge
        if (timeElapsed >= timeUntilNextMove || IsTooCloseToEdge(50.0f))
        {
            timeElapsed = 0.0f;  // Reset time elapsed for the new movement
            timeUntilNextMove = static_cast<float>(std::rand()) / RAND_MAX * 15.0f; // Reset the timer for next movement

            // Set a new target position
            targetPosition = generateRandomPositionInsideCircle(circleRadius - 60);
            startPosition = m_Position;
            moveDuration = 1.0f; // Duration of the movement
            m_MovingToPoint = true;
        }
    }

    // Transition to phase 3 after the timer expires
    m_RetreatTimer -= deltaTime;
    if (m_RetreatTimer <= 0.0f)
    {
        m_Phase2 = false;
        m_Phase3 = true;
        m_RetreatPosition = generateRandomSpawnPoint(circleRadius); // Position outside the circle
        startPosition = m_Position;
        timeElapsed = 0.0f;
    }
}

void Enemy::Phase3(float deltaTime)
{
    // Move the enemy to the retreat position
    timeElapsed += deltaTime;
    float duration{ 3.0f }; // Duration of the retreat movement
    float t = timeElapsed / duration;

    if (t >= 1.0f)
    {
        m_Position = m_RetreatPosition;
        m_IsRetreating = true;
        m_Phase3 = false;
    }
    else
    {
        m_Position = lerp(startPosition, m_RetreatPosition, t);
    }
}

Point2f Enemy::generateRandomSpawnPoint(float radius)
{
    float angle = static_cast<float>(std::rand()) / RAND_MAX * 2 * M_PI;

    // Center of the circle
    float centerX = 1280.0f / 2.0f;
    float centerY = 720.0f / 2.0f;

    // Calculating the spawn position outside the circle
    float spawnOffset = 20.0f; // Offset to push the spawn point outside the circle
    float x = centerX + std::cos(angle) * (radius + spawnOffset);
    float y = centerY + std::sin(angle) * (radius + spawnOffset);

    // Calculating the target position inside the circle
    float targetOffset = 200.0f; // Offset to pull the target point inside the circle
    float targetx = centerX + std::cos(angle) * (radius - targetOffset);
    float targety = centerY + std::sin(angle) * (radius - targetOffset);

    // Updating class members
    targetPosition = Point2f{ targetx, targety };
    startPosition = Point2f{ x, y };

    return Point2f{ x, y };
}

Point2f Enemy::lerp(const Point2f& start, const Point2f& end, float t)
{
    return Point2f(start.x * (1 - t) + end.x * t, start.y * (1 - t) + end.y * t);
}

void Enemy::ShootBullet(Point2f playerPos)
{
    Bullet* newBullet = new Bullet(m_Position, playerPos, m_VoidCircle);
    m_Bullets.push_back(newBullet);
}

Point2f Enemy::generateRandomPositionInsideCircle(float radius)
{
    float angle = static_cast<float>(std::rand()) / RAND_MAX * 2 * M_PI;
    float r = sqrt(static_cast<float>(std::rand()) / RAND_MAX) * radius; // To ensure uniform distribution

    float centerX = 1280.0f / 2.0f;
    float centerY = 720.0f / 2.0f;

    float x = centerX + r * cos(angle);
    float y = centerY + r * sin(angle);

    return Point2f{ x, y };
}

bool Enemy::IsTooCloseToEdge(float threshold) const
{
    float centerX = 1280.0f / 2.0f;
    float centerY = 720.0f / 2.0f;
    float distanceToCenter = sqrt(pow(m_Position.x - centerX, 2) + pow(m_Position.y - centerY, 2));
    return (circleRadius - distanceToCenter) < threshold;
}

void Enemy::DecrementHealth()
{
    if (m_Health > 0) m_Health--;
    if (!IsAlive() && !m_PickupDrop)
    {
        DropPickups();
    }
}

void Enemy::DropPickups()
{
    // Add a health pickup
    m_Pickups.push_back(new Pickup(Pickup::Type::Health, m_Position));
	m_PickupDrop = true;
}

std::vector<Pickup*> Enemy::GetPickups() const
{
    return m_Pickups;
}

bool Enemy::IsAlive() const
{
    return m_Health > 0;
}


Rectf Enemy::GetEnemyRect() const
{
    return Rectf{ m_Position.x - m_EnemySize, m_Position.y - m_EnemySize, m_EnemySize * 2, m_EnemySize * 2 };
}
