#pragma once
#include "control.h"

class Button :
	public Control
{
public:
	Button(const String *caption, const Position *pos, HINSTANCE instance, HWND parent, int id);
	~Button(void);
	void enable();
	void disable();
};
