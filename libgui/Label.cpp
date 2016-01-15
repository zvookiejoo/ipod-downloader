#include "Label.h"

Label::Label(const String *caption, const Position *pos, HINSTANCE instance, HWND parent)
: Control(new String(TEXT("STATIC")), caption, pos, instance, SS_LEFT, parent)
{
}

Label::~Label(void)
{
}

void Label::setCaption(const String *caption)
{
	if (!caption) return;

	this->caption.clear();

	this->caption = *caption;

	SetWindowText(handle, this->caption.c_str());
}

String * Label::getCaption()
{
	String *result = new String(this->caption);

	return result;
}