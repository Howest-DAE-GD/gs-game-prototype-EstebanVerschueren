#pragma once
class VoidCircle
{

public:
	VoidCircle();
	~VoidCircle();


	void Draw() const;
	void Update(float elapsedSec);

private:
	float m_Radius{};
	float m_ShrinkSpeed{ 2.0f};
	


};

