#include "pch.h"
#include "VoidCircle.h"
#include "utils.h"




VoidCircle::VoidCircle()
{
	m_Radius = 720.0f/2;
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
