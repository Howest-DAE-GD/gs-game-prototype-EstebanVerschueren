#include "pch.h"
#include "Game.h"
#include <iostream>

Game::Game(const Window& window)
    : BaseGame{ window }
{
    Initialize();
}

Game::~Game()
{
    Cleanup();
}

void Game::Initialize()
{
    m_voidCircle = new VoidCircle();
    m_Player = new PlayerController(*m_voidCircle);
    m_Enemy = new Enemy(m_voidCircle->GetRadius(), *m_voidCircle);
}

void Game::Cleanup()
{
    delete m_voidCircle;
    delete m_Player;
    delete m_Enemy;
    for (Pickup* pickup : m_Pickups)
    {
        delete pickup;
    }
    m_Pickups.clear();
}

void Game::Update(float elapsedSec)
{
    m_voidCircle->Update(elapsedSec);
    m_Player->Update(elapsedSec);
    m_Enemy->Update(elapsedSec, m_Player->GetPlayerPosition(), m_voidCircle->GetRadius());

    // Check for bullet collisions with the player
    Rectf playerRect = m_Player->GetPlayerRect();
    for (Bullet* bullet : m_Enemy->m_Bullets)
    {
        if (bullet->CheckCollision(playerRect))
        {
            m_Player->DecrementHealth();
        }
    }

    // Check for bullet collisions with the enemy
    Rectf enemyRect = m_Enemy->GetEnemyRect();
    for (Bullet* bullet : m_Player->m_Bullets)
    {
        if (bullet->CheckCollision(enemyRect))
        {
            m_Enemy->DecrementHealth();
            if (!m_Enemy->IsAlive())
            {
                // Handle enemy death if needed
                std::vector<Pickup*> enemyPickups = m_Enemy->GetPickups();
                m_Pickups.insert(m_Pickups.end(), enemyPickups.begin(), enemyPickups.end());
            }
        }
    }

    // Update pickups and check for collisions with the player
    for (Pickup* pickup : m_Pickups)
    {
        pickup->Update(elapsedSec);
        if (pickup->CheckCollision(playerRect))
        {
            m_Player->HandlePickup(pickup);
        }
    }

    // Remove inactive pickups
    auto it = std::remove_if(m_Pickups.begin(), m_Pickups.end(), [](Pickup* pickup)
        {
            if (!pickup->IsActive())
            {
                delete pickup;
                return true;
            }
            return false;
        });
    m_Pickups.erase(it, m_Pickups.end());
}

void Game::Draw() const
{
    ClearBackground();
    m_voidCircle->Draw();
    m_Enemy->Draw();

    for (Pickup* pickup : m_Pickups)
    {
        pickup->Draw();
    }

    m_Player->Draw();
    m_voidCircle->DrawTexture();
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
    // Handle key down events if necessary
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
    // Handle key up events if necessary
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
    m_Player->ProcessMouseMotionEvent(e);
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
    m_Player->ProcessMouseDownEvent(e);
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
    m_Player->ProcessMouseUpEvent(e);
}

void Game::ClearBackground() const
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
