#pragma once
#include "control.h"

class Combo :
	public Control
{
	std::vector<String> *items;
	void refresh();
public:
	Combo(const Position *pos, HINSTANCE instance, HWND parent, int id);
	~Combo(void);
	void addItem(const String *item);
	void clear();
	String * getItem(int itemIndex);
	String * getSelectedItem();
};
