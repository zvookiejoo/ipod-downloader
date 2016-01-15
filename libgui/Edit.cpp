#include "Edit.h"

Edit::Edit(const Position *pos, HINSTANCE instance, HWND parent, int id)
: Control(new String(TEXT("EDIT")), NULL, pos, instance, ES_AUTOHSCROLL | ES_LEFT | WS_TABSTOP, parent, id, WS_EX_CLIENTEDGE)
{
}

Edit::~Edit(void)
{
}

String * Edit::getText()
{
	TCHAR *temp = (TCHAR *) malloc(sizeof(TCHAR) * 256);
	memset(temp, 0, sizeof(TCHAR) * 256);
	GetWindowText(handle, temp, 256);

	text = temp;

	return &text;
}

void Edit::setText(const String *text)
{
	if (!text) return;

	this->text = *text;
	SetWindowText(handle, this->text.c_str());
}

void Edit::enable()
{
	SendMessage(handle, EM_SETREADONLY, FALSE, 0);
}

void Edit::disable()
{
	SendMessage(handle, EM_SETREADONLY, TRUE, 0);
}

bool Edit::isEnabled()
{
	int result = GetWindowLong(handle, GWL_STYLE);
	result &= ES_READONLY;
	return result == 0;
}