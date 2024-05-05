#pragma once
#include "vector"
class Enemy
{
public:
	Enemy(float radius);
	~Enemy();

	void Draw() const;
	void Update(float elapsedSec, Point2f playerPos, float radius);

private:

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
	Point2f m_Position{ };
	float m_EnemySize{ 20.0f };



	void HandlePhases(float deltaTime);
	void Phase1(float deltaTime);
	void Phase2(float deltaTime);

	bool m_Phase1{ false };
	bool m_Phase2{ false };

	float circleRadius{ 0.0f };
};

