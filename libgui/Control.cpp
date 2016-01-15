#include "Control.h"

HLOCAL Control::font = GetStockObject(DEFAULT_GUI_FONT);

Control::Control(const String *className,
				 const String *caption,
				 const Position *pos,
				 HINSTANCE instance,
				 DWORD style,
				 HWND parent,
				 int id,
				 DWORD extStyle)
{
	if (!className) return;
	if (!pos) return;

	this->className = *className;
	this->parent = parent;

	this->caption = (caption == NULL) ? TEXT("") : *caption;

	this->pos = *pos;

	HMENU controlId = reinterpret_cast<HMENU>(static_cast<DWORD>(id));

	this->handle = CreateWindowEx(extStyle,
		this->className.c_str(),
		this->caption.c_str(),
		WS_CHILD | WS_VISIBLE | style,
		pos->x,
		pos->y,
		pos->width,
		pos->height,
		this->parent,
		controlId,
		instance,
		NULL);

	SendMessage(handle, WM_SETFONT, (WPARAM) Control::font, TRUE);
}

Control::~Control(void)
{
}

void Control::resize(int, int) {}