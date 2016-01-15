#pragma once
#include "stdafx.h"

class Control
{
protected:
	HWND parent;
	HWND handle;
	String className;
	String caption;
	static HLOCAL font;
	Position pos;
public:
	Control(const String *className, const String *caption, const Position *pos, HINSTANCE instance,
		DWORD style, HWND parent, int id = 0, DWORD extStyle = 0);
	~Control(void);
	virtual void resize(int, int);
};
