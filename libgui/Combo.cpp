#include "Combo.h"

Combo::Combo(const Position *pos, HINSTANCE instance, HWND parent, int id)
: Control(new String(TEXT("COMBOBOX")), NULL, pos, instance, CBS_DROPDOWNLIST | WS_TABSTOP, parent, id)
{
	items = new std::vector<String>;
}

Combo::~Combo(void)
{
}

void Combo::addItem(const String *item)
{
	items->push_back(*item);
	refresh();
}

void Combo::clear()
{
	items->clear();
	SendMessage(handle, CB_RESETCONTENT, 0, 0);
}

void Combo::refresh()
{
	SendMessage(handle, CB_RESETCONTENT, 0, 0);

	for (std::vector<String>::iterator it = items->begin(); it != items->end(); ++it) {
		String item = *it;
		SendMessage(handle, CB_ADDSTRING, NULL, LPARAM(item.c_str()));
	}

	SendMessage(handle, CB_SETCURSEL, 0, NULL);
}

String * Combo::getItem(int itemIndex)
{
	String *item = new String(items->at(itemIndex));
	return item;
}

String * Combo::getSelectedItem()
{
	int index = SendMessage(handle, CB_GETCURSEL, 0, 0);
	return getItem(index);
}