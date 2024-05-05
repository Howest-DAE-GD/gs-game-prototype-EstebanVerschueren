#include "pch.h"
#include "Enemy.h"
#include "utils.h"
#include <ctime>
#include <iostream>

Enemy::Enemy(float radius)
{
	m_Points = new std::vector<Point2f>();

	m_Position = generateRandomSpawnPoint(radius);
	InitializeTriangle();
	m_Phase1 = true;
}

Enemy::~Enemy()
{
	delete m_Points;
}

void Enemy::Draw() const
{
	utils::SetColor(Color4f(1.0f, .0f, 0.0f, 1.0f));
	utils::FillPolygon(m_Points->data(), m_Points->size());

}

void Enemy::Update(float deltaTime, Point2f playerPos, float radius)
{
	circleRadius = radius;

	CalculateAngle(playerPos);
	UpdateTriangle();
	HandlePhases(deltaTime);
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

}

void Enemy::Phase1(float deltaTime)
{
	timeElapsed += deltaTime;
	float duration{ 2.0f }; // duration of the interpolation
	float t = timeElapsed / duration; // duration is the total time for the interpolation
	if (t >= 1.0f)
	{
		m_Position = targetPosition;
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

	// shoot random angle at player

	// move ranomly around the circle

	// dont go outside the circle





}

Point2f Enemy::generateRandomSpawnPoint(float radius)
{

	float angle = static_cast<float>(std::rand()) / RAND_MAX * 360;
	//float randomRadius = static_cast<float>(std::rand()) / RAND_MAX * radius;


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

Point2f Enemey::GenerateRandomPoint(float radius)
{

}


Point2f Enemy::lerp(const Point2f& start, const Point2f& end, float t)
{

	return Point2f( start.x * (1 - t) + end.x * t, start.y * (1 - t) + end.y * t);
}