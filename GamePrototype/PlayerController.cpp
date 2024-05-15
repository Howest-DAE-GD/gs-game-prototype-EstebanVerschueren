#include "pch.h"
#include "PlayerController.h"
#include "utils.h"
#include <iostream>

PlayerController::PlayerController(const VoidCircle& voidCircle) : m_voidCircle(voidCircle)
{
    m_Points = new std::vector<Point2f>();
    m_Position = Point2f(1280.0f / 2.0f, 720.0f / 2.0f);
    InitializeTriangle();
}

PlayerController::~PlayerController()
{
    delete m_Points;

    for (Bullet* bullet : m_Bullets)
    {
        delete bullet;
    }
    m_Bullets.clear();
}

void PlayerController::Update(float deltaTime)
{
    m_PlayerDirection = Point2f(m_Points->at(0) - m_Position);
    CalculateAngle();
    UpdateTriangle();
    HandleInput();
    Move(deltaTime);

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
}

void PlayerController::HandlePickup(Pickup* pickup)
{
    if (pickup->GetType() == Pickup::Type::Health && m_Health < 5)
    {
        m_Health++;
        pickup->SetActive(false);
    }
}
