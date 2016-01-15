#pragma once
#include "control.h"

class Label :
	public Control
{
public:
	Label(const String *caption, const Position *pos, HINSTANCE instance, HWND parent);
	~Label(void);
	void setCaption(const String *caption);
	String * getCaption(void);
};
