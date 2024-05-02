#include "pch.h"
#include "PlayerController.h"
#include "utils.h"

PlayerController::PlayerController()
{

	m_Points = new std::vector<Point2f>();
	m_Position = Point2f(1280.0f / 2.0f, 720.0f / 2.0f);
	
	InitializeTriangle();
}

PlayerController::~PlayerController()
{
	delete m_Points;
}


void PlayerController::Update(float deltaTime)
{
    m_PlayerDirection = Point2f(m_Points->at(0) - m_Position);
	CalculateAngle();
	UpdateTriangle();
	HandleInput();
	Move(deltaTime);

}

void PlayerController::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	m_MousePos = Point2f(float(e.x), float(e.y));
}

void PlayerController::Draw() const
{
	utils::SetColor(Color4f(0.0f, 1.0f, 0.0f, 1.0f));
	utils::FillPolygon(m_Points->data(), m_Points->size());
}

void PlayerController::CalculateAngle()
{
	//calculating the angle between playerdirection and mouseposition

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
	m_Points->push_back(Point2f(m_Position.x + m_PlayerSize, m_Position.y)); // Top point
	m_Points->push_back(Point2f(m_Position.x - m_PlayerSize, m_Position.y + m_PlayerSize)); // Bottom left
	m_Points->push_back(Point2f(m_Position.x - m_PlayerSize, m_Position.y - m_PlayerSize)); // Bottom right
}

void PlayerController::HandleInput()
{
    const Uint8* keyStates = SDL_GetKeyboardState(NULL);

    // Reset direction
    m_Direction.x = 0;
    m_Direction.y = 0;
    // Check for right arrow key
    if (keyStates[SDLK_d])
    {
        m_Direction.x = 1;
    }
    // Check for left arrow key
    if (keyStates[SDLK_a])
    {
        m_Direction.x -= 1;
    }
    // Check for up arrow key
    if (keyStates[SDLK_w])
    {
        m_Direction.y = 1;
    }
    // Check for down arrow key
    if (keyStates[SDLK_s])
    {
        m_Direction.y -= 1;
    }

}

void PlayerController::Move(float deltaTime)
{
	m_Position.x +=  m_Direction.x * m_Speed * deltaTime;
	m_Position.y +=  m_Direction.y * m_Speed * deltaTime;
}