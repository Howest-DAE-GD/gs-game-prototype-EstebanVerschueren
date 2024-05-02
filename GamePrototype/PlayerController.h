#pragma once
#include "vector"
class PlayerController
{
public:
	PlayerController();
	~PlayerController();


	void Draw() const;
	void Update(float elapsedSec);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);

private:
	std::vector<Point2f>* m_Points{};
	std::vector<Point2f>* m_InitialPoints{};

	Point2f m_Position{ };
	float m_PlayerSize{ 20.0f };
	Point2f m_PlayerDirection{ };

	Point2f m_MousePos{ };


	void CalculateAngle();
	float angle{ 0.0f };
	void UpdateTriangle();
	void InitializeTriangle();


	void HandleInput();
	Point2f m_Direction{};

	void Move(float elapsedSec);
	float m_Speed{ 200.0f };

};

