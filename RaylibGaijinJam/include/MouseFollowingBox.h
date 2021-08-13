#pragma once
#include <raylib.h>
class MouseFollowingBox
{
public:
	MouseFollowingBox(Rectangle& box, int screenUpperLimit);
	void SetBoxPositionToMouse();
	Rectangle GetBox();

private:
	Rectangle _box;
	int _screenUpperLimit;
};

