// Game.h
#pragma once
#include "BaseGame.h"
#include "VoidCircle.h"
#include "PlayerController.h"
#include "Enemy.h"
#include "CharacterManager.h"
#include <vector>
#include "Laser.h"
#include "SoundStream.h"
#include "Camera.h"
#include <fstream>
#include <string>
#include <sstream>



class Game : public BaseGame
{
public:
    explicit Game(const Window& window);
    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;
    Game(Game&& other) = delete;
    Game& operator=(Game&& other) = delete;
    ~Game();

    void Update(float elapsedSec) override;
    void Draw() const override;

    // Event handling
    void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
    void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
    void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
    void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
    void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:
    enum class GameState
    {
        Running,
        Ended
    };

    void Initialize();
    void Cleanup();
    void ClearBackground() const;
    void Reset(); // Add reset method

    VoidCircle* m_voidCircle{};
    PlayerController* m_Player{};
    std::vector<Enemy*> m_Enemies{};
    std::vector<Pickup*> m_Pickups;
    Laser* m_Laser{};

    // Add logging for cleanup
    void LogCleanup() const;

    // Timer for the game
    float m_GameTimer{ 0.0f };
    const float m_ShrinkSpeedIncrementInterval{ 40.0f }; // Time interval to increase shrink speed
    const float m_InitialShrinkSpeed{ 4.0f }; // Starting shrink speed
    const float m_MinShrinkSpeed{ 2.0f }; // Minimum shrink speed for fluctuation
    const float m_MaxShrinkSpeed{ 4.0f }; // Maximum shrink speed for fluctuation


    // Enemy spawning
    float m_EnemySpawnTimer{ 0.0f };
    float m_SpawnInterval{ 2.5f }; // Initial spawn interval
    const float m_SpawnRateIncrementInterval{ 10.0f }; // Time interval to increase spawn rate
    const float m_SpawnIntervalDecrement{ 0.00125f }; // Amount to decrease spawn interval each time
    void SpawnEnemy();
    void DrawScore() const;
    void SetEnemyPlayerController();

    // Difficulty curve variables
    const float m_MovementIntervalDecrementInterval{ 10.0f }; // Time interval to decrease enemy movement interval
    const float m_MinMovementDuration{ 1.0f }; // Maximum enemy speed
    float m_MinMovementInterval{ 1.0f }; // Minimum movement interval
	float m_MovementInterval{ 3.0f }; // Initial movement interval
	float m_MovementDuration{ 4.0f }; // Initial movement duration

	float m_NextEnemySpawnTime{ 0.0f };
	float tempm_MovementInterval{ 3.0f };

    GameState m_GameState{ GameState::Running }; // Add game state member variable
    void DrawEndScreen() const; // Add method to draw the end screen

    int m_Score{ 0 }; // Add score member variable
    void UpdateScore(int amount); // Method to update score


    float GetRandomFloat(float min, float max)
    {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

	SoundStream* m_BackgroundMusic{};
	Camera* m_Camera{};

	void SaveHighScore(const std::string& playerName, int score, const std::string& filename);
};
