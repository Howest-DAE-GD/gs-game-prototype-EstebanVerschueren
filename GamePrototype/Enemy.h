#pragma once
class Enemy
{
public:
	Enemy();
	~Enemy();

	void Draw() const;
	void Update(float elapsedSec);

private:

	void CalculateAngle();
	float angle{ 0.0f };
	void UpdateTriangle();
	void InitializeTriangle();



};

