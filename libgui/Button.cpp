#include "Button.h"

Button::Button(const String *caption, const Position *pos, HINSTANCE instance, HWND parent, int id)
: Control(new String(TEXT("BUTTON")), caption, pos, instance, BS_PUSHBUTTON | WS_TABSTOP, parent, id)
{
}

Button::~Button(void)
{
}

void Button::enable()
{
	EnableWindow(handle, TRUE);
}

void Button::disable()
{
	EnableWindow(handle, FALSE);
}