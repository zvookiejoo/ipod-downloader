#pragma once
#include "control.h"

class Edit :
	public Control
{
	String text;
	bool enabled;
public:
	Edit(const Position *pos, HINSTANCE instance, HWND parent, int id);
	~Edit(void);
	String * getText(void);
	void setText(const String *text);
	void enable();
	void disable();
	bool isEnabled();
};