#pragma once
#include "texture.h"
class VoidCircle
{

public:
	VoidCircle();
	~VoidCircle();


	void Draw() const;
	void Update(float elapsedSec);

	void DrawTexture() const;

	float GetRadius() const;
	bool IsPointInside(const Point2f& point) const;

private:
	float m_Radius{};
	float m_ShrinkSpeed{ 2.0f};
	
	Texture* m_voidCircleTexture{};

};

