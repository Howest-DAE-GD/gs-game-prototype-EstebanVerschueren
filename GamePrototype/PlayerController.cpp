#include "pch.h"
#include "PlayerController.h"
#include "utils.h"
#include <iostream>
#include <random>

PlayerController::PlayerController(VoidCircle& voidCircle)
    : m_voidCircle(voidCircle), m_KnockbackForce{ 0.0f, 0.0f }, m_KnockbackDecay(0.95f) // Adjust decay rate as needed
{
    m_Points = new std::vector<Point2f>();
    m_Position = Point2f(1280.0f / 2.0f, 720.0f / 2.0f);
    InitializeTriangle();
}

PlayerController::~PlayerController()
{
    delete m_Points;
    m_Points = nullptr;

    for (Bullet* bullet : m_Bullets)
    {
        delete bullet;
    }
    m_Bullets.clear();

    std::cout << "PlayerController cleaned up.\n";
}

void PlayerController::Update(float deltaTime)
{

    m_PlayerDirection = Point2f(m_Points->at(0) - m_Position);
    CalculateAngle();
    UpdateTriangle();
    HandleInput();
    Move(deltaTime);

    // Check if the player is inside the circle
    if (!m_voidCircle.IsPointInside(m_Position))
    {
        m_TimeOutsideCircle += deltaTime;
        if (m_TimeOutsideCircle >= 1.0f)
        {
            DecrementHealth();
            m_TimeOutsideCircle = 0.0f; // Reset the timer
        }
    }
    else
    {
        m_TimeOutsideCircle = 0.0f; // Reset the timer if the player is inside the circle
    }


    // Update bullets
    for (Bullet* bullet : m_Bullets)
    {
        bullet->Update(deltaTime);
    }

    // Remove inactive bullets
    m_Bullets.erase(std::remove_if(m_Bullets.begin(), m_Bullets.end(), [](Bullet* bullet)
        {
            return !bullet->IsActive();
        }), m_Bullets.end());

    // Handle continuous shooting
    if (m_IsShooting)
    {
        m_TimeSinceLastShot += deltaTime;
        if (m_TimeSinceLastShot >= m_BulletCooldown)
        {
            ShootBullet();
            m_TimeSinceLastShot = 0.0f;
        }
    }

    // Update shotgun cooldown timer
    if (m_TimeSinceLastShotgun < m_ShotgunCooldown)
    {
        m_TimeSinceLastShotgun += deltaTime;
    }

    // Apply knockback to the player's position
    m_Position.x += m_KnockbackForce.x * deltaTime;
    m_Position.y += m_KnockbackForce.y * deltaTime;

    // Clamp player position to screen bounds (example: 0 to 1280 for x, 0 to 720 for y)
    m_Position.x = Clamp(m_Position.x, 0.0f, 1280.0f);
    m_Position.y = Clamp(m_Position.y, 0.0f, 720.0f);

    // Decay knockback force over time
    m_KnockbackForce.x *= std::pow(0.95f, deltaTime * 60); // Increase decay rate as needed
    m_KnockbackForce.y *= std::pow(0.95f, deltaTime * 60); // Increase decay rate as needed

    // Stop knockback force when it becomes very small
    if (std::abs(m_KnockbackForce.x) < 0.1f) m_KnockbackForce.x = 0.0f;
    if (std::abs(m_KnockbackForce.y) < 0.1f) m_KnockbackForce.y = 0.0f;

	m_LaserDamageCooldown -= deltaTime;
	if (m_LaserDamageCooldown < 0.0f)
    {
		m_LaserDamageCooldown = 0.0f;
	}
}

void PlayerController::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
    m_MousePos = Point2f(float(e.x), float(e.y));
}

void PlayerController::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
    if (e.button == SDL_BUTTON_LEFT)
    {
        m_IsShooting = true;
        m_TimeSinceLastShot = m_BulletCooldown;
    }
    else if (e.button == SDL_BUTTON_RIGHT && m_TimeSinceLastShotgun >= m_ShotgunCooldown)
    {
        ShootShotgun();
        m_TimeSinceLastShotgun = 0.0f;
    }
}

void PlayerController::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
    if (e.button == SDL_BUTTON_LEFT)
    {
        m_IsShooting = false;
    }
}

void PlayerController::Draw() const
{
    utils::SetColor(Color4f(0.0f, 1.0f, 0.0f, 1.0f));
    utils::FillPolygon(m_Points->data(), m_Points->size());

    for (Bullet* bullet : m_Bullets)
    {
        bullet->Draw();
    }

    DrawHealthCircles();
}

void PlayerController::CalculateAngle()
{
    angle = atan2(m_MousePos.y - m_Position.y, m_MousePos.x - m_Position.x);
}

void PlayerController::UpdateTriangle()
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

void PlayerController::InitializeTriangle()
{
    m_Points->clear();
    m_Points->push_back(Point2f(m_Position.x + m_PlayerSize, m_Position.y));
    m_Points->push_back(Point2f(m_Position.x - m_PlayerSize, m_Position.y + m_PlayerSize));
    m_Points->push_back(Point2f(m_Position.x - m_PlayerSize, m_Position.y - m_PlayerSize));
}

void PlayerController::HandleInput()
{
    const Uint8* keyStates = SDL_GetKeyboardState(NULL);
    m_Direction.x = 0;
    m_Direction.y = 0;
    if (keyStates[SDL_SCANCODE_D])
    {
        m_Direction.x = 1;
    }
    if (keyStates[SDL_SCANCODE_A])
    {
        m_Direction.x -= 1;
    }
    if (keyStates[SDL_SCANCODE_W])
    {
        m_Direction.y = 1;
    }
    if (keyStates[SDL_SCANCODE_S])
    {
        m_Direction.y -= 1;
    }
}

void PlayerController::Move(float deltaTime)
{
    m_Position.x += m_Direction.x * m_Speed * deltaTime;
    m_Position.y += m_Direction.y * m_Speed * deltaTime;
}

Point2f PlayerController::GetPlayerPosition() const
{
    return m_Position;
}

Rectf PlayerController::GetPlayerRect() const
{
    return Rectf{ m_Position.x - m_PlayerSize, m_Position.y - m_PlayerSize, m_PlayerSize * 2, m_PlayerSize * 2 };
}

void PlayerController::DecrementHealth()
{
    std::cout << m_Health << std::endl;
    if (m_Health > 0) m_Health--;
}

int PlayerController::GetHealth() const
{
    return m_Health;
}

void PlayerController::DrawHealthCircles() const
{
    utils::SetColor(Color4f(0.0f, 0.0f, 1.0f, 1.0f));
    float radius = 40.0f;
    float angleStep = 2.0f * 3.1415926f / 5;
    for (int i = 0; i < m_Health; ++i)
    {
        float angle = i * angleStep;
        float x = m_Position.x + radius * cos(angle);
        float y = m_Position.y + radius * sin(angle);
        utils::FillEllipse(x, y, 10.0f, 10.0f);
    }
}

void PlayerController::ShootBullet()
{
    Bullet* newBullet = new Bullet(m_Position, m_MousePos, m_voidCircle);
    newBullet->SetColor(Color4f{ 0.0f, 1.0f, 0.0f, 1.0f });
    newBullet->SetSpeed(500.0f);
    m_Bullets.push_back(newBullet);

    // Apply small knockback
    Point2f direction = Point2f{ m_Position - m_MousePos };
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
    {
        direction.x /= length;
        direction.y /= length;
    }
    m_KnockbackForce.x += direction.x * 50.0f;
    m_KnockbackForce.y += direction.y * 50.0f;
}

void PlayerController::ShootShotgun()
{
    float spreadAngle = 0.1f;
    for (int i = -1; i <= 1; ++i)
    {
        float newAngle = angle + i * spreadAngle;
        Point2f target(m_Position.x + cos(newAngle) * 1000.0f, m_Position.y + sin(newAngle) * 1000.0f);
        Bullet* newBullet = new Bullet(m_Position, target, m_voidCircle);
        newBullet->SetColor(Color4f{ 0.0f, 1.0f, 0.0f, 1.0f });
        newBullet->SetSpeed(500.0f);
        m_Bullets.push_back(newBullet);
    }

    // Apply larger knockback
    Point2f direction = Point2f{ m_Position - m_MousePos };
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
    {
        direction.x /= length;
        direction.y /= length;
    }
    m_KnockbackForce.x += direction.x * 500.0f;
    m_KnockbackForce.y += direction.y * 500.0f;
}

void PlayerController::HandlePickup(Pickup* pickup)
{
    if (pickup->GetType() == Pickup::Type::Health && m_Health < 5)
    {
        m_Health++;
        pickup->SetActive(false);
    }
    else if (pickup->GetType() == Pickup::Type::IncreaseRadius)
    {
        m_voidCircle.IncreaseRadius(125.0f); // Increase the radius
        pickup->SetActive(false);
    }
}

float PlayerController::GetIncreaseRadiusProbability(float radius, float maxRadius)
{
    float radiusPercentage = radius / maxRadius;
    if (radiusPercentage > 0.25f)
    {
        return 0.15f; // Constant probability of 25% when radius percentage is above 25%
    }
    else
    {
        // Rapidly increasing probability as radius decreases
        float factor = radiusPercentage / 0.25f; // Normalize to range [0, 1]
        return 0.25f + (1.0f - factor) * 0.75f; // Rapid increase, reaching 100% as radius approaches zero
    }
}


float PlayerController::GetHealthPickupProbability(int health, int maxHealth)
{
    float healthPercentage = static_cast<float>(health) / maxHealth;
    if (healthPercentage > 0.4f)
    {
        return 0.15f; // Constant probability of 15% when health percentage is above 40%
    }
    else
    {
        // Rapidly increasing probability as health decreases
        float factor = healthPercentage / 0.4f; // Normalize to range [0, 1]
        return 0.15f + (1.0f - factor) * 0.85f; // Rapid increase, reaching 100% as health approaches zero
    }
}

void PlayerController::TakeDamageFromLaser()
{
    if (m_LaserDamageCooldown <= 0.0f)
    {
        DecrementHealth();
        m_LaserDamageCooldown = 1.0f; // Reset cooldown to 1 second
    }
}
