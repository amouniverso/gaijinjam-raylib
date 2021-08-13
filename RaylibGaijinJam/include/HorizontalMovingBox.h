#pragma once
#include <raylib.h>
#include <iostream>
class HorizontalMovingBox
{
	public:
		HorizontalMovingBox();
		HorizontalMovingBox(Rectangle& box, int boxSpeed);
		void SetBoxSpeed(int newSpeed);
		void MoveHorizontal();
		Rectangle GetBox();

	private:
		Rectangle _boxA;
		int _boxASpeedX = 4;
};

