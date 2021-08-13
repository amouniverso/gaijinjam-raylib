#include "MouseFollowingBox.h"

MouseFollowingBox::MouseFollowingBox(Rectangle& box, int screenUpperLimit)
{
	_box = box;
	_screenUpperLimit = screenUpperLimit;
}

void MouseFollowingBox::SetBoxPositionToMouse()
{
	_box.x = GetMouseX() - _box.width / 2;
	_box.y = GetMouseY() - _box.height / 2;
	// Make sure Box B does not go out of move area limits
	if ((_box.x + _box.width) >= GetScreenWidth()) _box.x = GetScreenWidth() - _box.width;
	else if (_box.x <= 0) _box.x = 0;

	if ((_box.y + _box.height) >= GetScreenHeight()) _box.y = GetScreenHeight() - _box.height;
	else if (_box.y <= _screenUpperLimit) _box.y = _screenUpperLimit;
}

Rectangle MouseFollowingBox::GetBox()
{
	return _box;
}
