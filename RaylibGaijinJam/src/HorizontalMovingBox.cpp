#include "HorizontalMovingBox.h"
#include <iostream>
using namespace std;

HorizontalMovingBox::HorizontalMovingBox()
{
	float boxVerticalPosition = GetScreenHeight() / 2 - 50;
	_boxA = { 10, boxVerticalPosition , 200, 100 };
	
}

HorizontalMovingBox::HorizontalMovingBox(Rectangle& box, int boxSpeed)
{
	_boxA = box;
	_boxASpeedX = boxSpeed;

}

void HorizontalMovingBox::SetBoxSpeed(int newSpeed)
{
	_boxASpeedX = newSpeed;
}

void HorizontalMovingBox::MoveHorizontal()
{
	_boxA.x += _boxASpeedX;
	//cout << _boxA.x;
	// Bounce box on x screen limits
	if (((_boxA.x + (int)_boxA.width) >= GetScreenWidth()) || (_boxA.x <= 0)) _boxASpeedX *= -1;


}

Rectangle HorizontalMovingBox::GetBox()
{
	return _boxA;
}
