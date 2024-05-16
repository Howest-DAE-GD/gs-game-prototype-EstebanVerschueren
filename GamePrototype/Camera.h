#pragma once
class Camera
{
public:
	Camera(float screenWidth, float screenHeight);
	void Aim(float levelW, float levelH, Point2f trackCenter);
	void Aim(const Point2f& trackCenter);
	void Update(float elapsedSec);
	void Reset();

	void Shake(float time, float magnitude);


private:
	float m_ScreenWidth{};
	float m_ScreenHeight{};

	float m_ShakeTimeLeft{};
	float m_ShakeDuration{};
	float m_ShakeMagnitude{};
};

