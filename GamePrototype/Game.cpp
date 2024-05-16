// Game.cpp
#include "pch.h"
#include "Game.h"
#include <iostream>
#include <cmath> 

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
   // m_Laser = new Laser(1280.0f, 720.0f);

    // Load character textures for score display
    CharacterManager::GetInstance().LoadCharacterTextures("Fonts/DIN-Light.otf", 24); // Adjust font path and size as needed

    SetEnemyPlayerController(); // Add this line

    m_NextEnemySpawnTime = 1.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (m_SpawnInterval - 1.0f)));

	m_BackgroundMusic = new SoundStream("Audio/Bullet_Hell_japanese_hype.wav");
	m_BackgroundMusic->SetVolume(10.0f);
	m_BackgroundMusic->Play(true);

    std::cout << "Game initialized.\n";
}

void Game::Cleanup()
{
    LogCleanup();

    for (Enemy* enemy : m_Enemies)
    {
        delete enemy;
    }
    m_Enemies.clear();

    for (Pickup* pickup : m_Pickups)
    {
        delete pickup;
    }
    m_Pickups.clear();

    delete m_Player;
    m_Player = nullptr;

    delete m_voidCircle;
    m_voidCircle = nullptr;

    //delete m_Laser;
    //m_Laser = nullptr;

	delete m_BackgroundMusic;

    std::cout << "Game cleaned up.\n";
}

void Game::Reset()
{
    Cleanup();
    Initialize();
    m_GameState = GameState::Running;
    m_GameTimer = 0.0f;
    m_EnemySpawnTimer = 0.0f;
    m_SpawnInterval = 4.0f;
	m_Score = 0;
}

void Game::LogCleanup() const
{
    std::cout << "Cleaning up game resources...\n";
    if (m_voidCircle)
    {
        std::cout << "Deleting VoidCircle.\n";
    }
    if (m_Player)
    {
        std::cout << "Deleting PlayerController.\n";
    }
    for (const Enemy* enemy : m_Enemies)
    {
        if (enemy)
        {
            std::cout << "Deleting Enemy at position: (" << enemy->GetPosition().x << ", " << enemy->GetPosition().y << ")\n";
        }
    }
    for (const Pickup* pickup : m_Pickups)
    {
        if (pickup)
        {
            std::cout << "Deleting Pickup at position: (" << pickup->GetPosition().x << ", " << pickup->GetPosition().y << ")\n";
        }
    }
}

void Game::Update(float elapsedSec)
{
    if (m_GameState == GameState::Ended)
    {
        return; // Do not update if the game has ended
    }

    float previousRadius = m_voidCircle->GetRadius();
    m_GameTimer += elapsedSec; // Update the game timer
    m_EnemySpawnTimer += elapsedSec; // Update the enemy spawn timer

    // Calculate logarithmic increases for difficulty
    float timeFactor = std::log(1.0f + m_GameTimer); // Logarithmic growth

    // Adjust spawn interval: decrease over time
    float minSpawnInterval = 1.0f; // Minimum spawn interval
    float maxSpawnInterval = 10.0f; // Initial spawn interval
    m_SpawnInterval = std::max(minSpawnInterval, maxSpawnInterval - timeFactor * 0.7f);

    // Adjust movement interval: decrease over time
    float minMovementInterval = 1.5f; // Minimum movement interval
	m_MinMovementInterval = minMovementInterval;
    float maxMovementInterval = 9.0f; // Initial movement interval
    m_MovementInterval = std::max(minMovementInterval, maxMovementInterval - timeFactor * 1.0f);

    // Adjust shrink speed: increase over time
    float initialShrinkSpeed = 4.0f;
    float minShrinkSpeed = 4.0f;
    float maxShrinkSpeed = 10.0f;
    float shrinkSpeed = std::min(maxShrinkSpeed, initialShrinkSpeed + timeFactor * (maxShrinkSpeed - minShrinkSpeed) / 10.0f);
    m_voidCircle->SetShrinkSpeed(shrinkSpeed);


    // Spawn enemies at regular intervals
    if (m_GameTimer >= m_NextEnemySpawnTime)
    {
        SpawnEnemy();
        m_NextEnemySpawnTime = m_GameTimer + 1.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (GetRandomFloat(minSpawnInterval, m_SpawnInterval) - 1.0f)));
    }

    m_voidCircle->Update(elapsedSec);
    m_Player->Update(elapsedSec);

    for (Enemy* enemy : m_Enemies)
    {
        enemy->Update(elapsedSec, m_Player->GetPlayerPosition(), m_voidCircle->GetRadius());
    }


    Rectf playerRect = m_Player->GetPlayerRect();
    for (Enemy* enemy : m_Enemies)
    {
        for (Bullet* bullet : enemy->m_Bullets)
        {
            if (bullet->CheckCollision(playerRect))
            {
                m_Player->DecrementHealth();
            }
        }

        // Handle player bullet collision with enemies and update score
        Rectf enemyRect = enemy->GetEnemyRect();
        for (Bullet* bullet : m_Player->m_Bullets)
        {
            if (bullet->CheckCollision(enemyRect) && enemy->IsAlive())
            {
                enemy->DecrementHealth();
                if (!enemy->IsAlive())
                {
                    std::vector<Pickup*> enemyPickups = enemy->GetPickups();
                    m_Pickups.insert(m_Pickups.end(), enemyPickups.begin(), enemyPickups.end());

                    // Increase score based on current radius
                    float radiusFactor = m_voidCircle->GetMaxRadius() / m_voidCircle->GetRadius();
                    UpdateScore(static_cast<int>(radiusFactor * 100)); // Increase score when enemy is defeated
                }
            }
        }

    }

    std::vector<Pickup*> pickupsToDelete;

    for (Pickup* pickup : m_Pickups)
    {
        pickup->Update(elapsedSec);
        if (!m_voidCircle->IsPointInside(pickup->GetPosition()))
        {
            pickup->SetActive(false);
        }
        if (pickup->CheckCollision(playerRect))
        {
            m_Player->HandlePickup(pickup);
        }
        if (!pickup->IsActive())
        {
            pickupsToDelete.push_back(pickup);
        }
    }

    for (Pickup* pickup : pickupsToDelete)
    {
        auto it = std::find(m_Pickups.begin(), m_Pickups.end(), pickup);
        if (it != m_Pickups.end())
        {
            m_Pickups.erase(it);
            delete pickup;
        }
    }

    // Check radius changes and update score
    float currentRadius = m_voidCircle->GetRadius();
    if (currentRadius > previousRadius)
    {
        UpdateScore(static_cast<int>((currentRadius - previousRadius) * 10)); // Increase score when radius enlarges
    }

    // Remove dead enemies and their bullets
    m_Enemies.erase(std::remove_if(m_Enemies.begin(), m_Enemies.end(), [](Enemy* enemy)
        {
            if (!enemy->IsAlive())
            {
                enemy->ClearBullets();
                delete enemy;
                return true;
            }
            return false;
        }), m_Enemies.end());

    // Check end conditions
    if (m_Player->GetHealth() <= 0 || m_voidCircle->GetRadius() <= 0)
    {
        m_GameState = GameState::Ended;
    }
}


void Game::Draw() const
{
    ClearBackground();

    if (m_GameState == GameState::Ended)
    {
        DrawEndScreen();
    }
    else
    {
        m_voidCircle->Draw();

        for (Enemy* enemy : m_Enemies)
        {
            enemy->Draw();
        }

        for (Pickup* pickup : m_Pickups)
        {
            pickup->Draw();
        }

        m_Player->Draw();
        m_voidCircle->DrawTexture();

        //if (m_Laser->IsActive())
        //{
        //    m_Laser->Draw();
        //}

        DrawScore(); // Draw the score on the screen
    }
}

void Game::DrawScore() const
{
    std::string scoreText = "Score: " + std::to_string(m_Score);
    float x = 10.0f;
    float y = 680.0f;

    for (char c : scoreText)
    {
        Texture* charTexture = CharacterManager::GetInstance().GetCharacterTexture(c);
        if (charTexture)
        {
            charTexture->Draw(Point2f{ x, y });
            x += charTexture->GetWidth();
        }
    }
}

void Game::DrawEndScreen() const
{
    std::string gameOverText = "Game Over";
    std::string restartText = "Press Space to Start Again";
    std::string helpText = "Killing enemies and enlarging the circle give more points when the circle is smaller";
    std::string scoreText = "Score: " + std::to_string(static_cast<int>(m_Score));
    

    float centerX = 1280.0f / 2.0f;
    float centerY = 720.0f / 2.0f;

    float gameOverTextWidth = 0.0f;
    float restartTextWidth = 0.0f;
	float helpTextWidth = 0.0f;

    // Calculate widths for centering the texts
    for (char c : gameOverText)
    {
        Texture* charTexture = CharacterManager::GetInstance().GetCharacterTexture(c);
        if (charTexture)
        {
            gameOverTextWidth += charTexture->GetWidth();
        }
    }

    for (char c : restartText)
    {
        Texture* charTexture = CharacterManager::GetInstance().GetCharacterTexture(c);
        if (charTexture)
        {
            restartTextWidth += charTexture->GetWidth();
        }
    }    
    
    for (char c : helpText)
    {
        Texture* charTexture = CharacterManager::GetInstance().GetCharacterTexture(c);
        if (charTexture)
        {
            helpTextWidth += charTexture->GetWidth();
        }
    }

    float gameOverX = centerX - gameOverTextWidth / 2;
    float restartX = centerX - restartTextWidth / 2;
	float helpX = centerX - helpTextWidth / 2;
    float scoreX = centerX - (scoreText.size() * 10) / 2; // Assuming 24 is the character width

    float y = centerY - 40; // Adjust as needed

    // Draw "Game Over" text
    for (char c : gameOverText)
    {
        Texture* charTexture = CharacterManager::GetInstance().GetCharacterTexture(c);
        if (charTexture)
        {
            charTexture->Draw(Point2f{ gameOverX, y });
            gameOverX += charTexture->GetWidth();
        }
    }

    y -= 40; // Move down for the next line

    // Draw "Press Enter to Start Again" text
    for (char c : restartText)
    {
        Texture* charTexture = CharacterManager::GetInstance().GetCharacterTexture(c);
        if (charTexture)
        {
            charTexture->Draw(Point2f{ restartX, y });
            restartX += charTexture->GetWidth();
        }
    }

    y += 120; // Move down for the next line

    // Draw "Score" text
    for (char c : scoreText)
    {
        Texture* charTexture = CharacterManager::GetInstance().GetCharacterTexture(c);
        if (charTexture)
        {
            charTexture->Draw(Point2f{ scoreX, y });
            scoreX += charTexture->GetWidth();
        }
    }

    y -= 300;

    // Draw "Press Enter to Start Again" text
    for (char c : helpText)
    {
        Texture* charTexture = CharacterManager::GetInstance().GetCharacterTexture(c);
        if (charTexture)
        {
            charTexture->Draw(Point2f{ helpX, y });
            helpX += charTexture->GetWidth();
        }
    }
}


void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
    if (m_GameState == GameState::Ended && e.keysym.sym == SDLK_SPACE)
    {
        Reset();
    }
    // Handle other key down events if necessary
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
    if (m_GameState == GameState::Ended)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    }

    glClear(GL_COLOR_BUFFER_BIT);
}

void Game::SetEnemyPlayerController()
{
    for (Enemy* enemy : m_Enemies)
    {
        enemy->SetPlayerController(m_Player);
    }
}

void Game::SpawnEnemy()
{
	int random = rand() % 100;
	int spawnNumber = 1;
    if (random < 10)
    {
        spawnNumber = 2;
	}

	for (int i = 0; i < spawnNumber; ++i)
	{
        float radius = m_voidCircle->GetRadius();
        Enemy* newEnemy = new Enemy(radius, *m_voidCircle, m_MovementDuration, GetRandomFloat(m_MinMovementInterval, m_MovementInterval));
        newEnemy->SetPlayerController(m_Player);
        m_Enemies.push_back(newEnemy);
	}
}

void Game::UpdateScore(int amount)
{
    m_Score += amount;
}


