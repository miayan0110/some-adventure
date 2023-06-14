#pragma once
#include "../Library/gameutil.h"
using namespace game_framework;

class MyBitmap :public CMovingBitmap {
public:
	/* Getter */
	int GetRight();
	int GetBottom();
	double CenterX();
	double CenterY();

	/* Is Function */
	bool IsEaten(MyBitmap mb1, MyBitmap mb2);

	/* Animation */
	void	StopKeepAni(bool start);
};