#include "pch.h"
#include "VoidCircle.h"
#include "utils.h"




VoidCircle::VoidCircle()
{
	m_Radius = 720.0f/2;
	m_voidCircleTexture = new Texture("voidcircleTexture.png");
}


VoidCircle::~VoidCircle()
{
}

void VoidCircle::Draw() const
{
	utils::SetColor(Color4f{ 1.0f, 1.0f, 1.0f, 1.0f });
	utils::FillEllipse(1280.0f/2.0f, 720.0f/2.0f, m_Radius, m_Radius);
}

void VoidCircle::Update(float deltaTime)
{
	m_Radius -= m_ShrinkSpeed * deltaTime;



}

void VoidCircle::DrawTexture() const
{
	float width = 1280.0f + m_Radius*30;
	float height = 720.0f + m_Radius*30/1280 *720;



	Rectf destRect = Rectf{ 1280/2 - (width/2), 720/2 - (height/2), width, height };


	Rectf sourceRect = Rectf{ 0.0f, 0.0f, m_voidCircleTexture->GetWidth(), m_voidCircleTexture->GetHeight()};

	m_voidCircleTexture->Draw(destRect, sourceRect);
}

float VoidCircle::GetRadius() const
{
	return m_Radius;
}
