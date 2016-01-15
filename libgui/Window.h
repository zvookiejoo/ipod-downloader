#pragma once
#include "stdafx.h"
#include "Label.h"
#include "Button.h"
#include "Edit.h"
#include "Combo.h"
#include "ListView.h"

#define CTL_ID_START 100

class Window
{
	HWND handle;
	HINSTANCE instance;
	std::map<int, void (*)(int)> *actions;
	std::vector<Control *> *resizableControls;
	String className;
	String title;
	int controlId;
public:
	Window(const String *title, const Position *pos, HINSTANCE instance);
	~Window(void);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	Control* ControlFactory(ControlType type, const String *caption, const Position *pos, void (*action)(int) = NULL);
	HWND getHandle();
	void addStyle(DWORD dwStyle);
	void removeStyle(DWORD dwStyle);
	std::vector<Control *> * getResizableControls();
};
