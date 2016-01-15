#pragma once
#include "stdafx.h"
#include "control.h"

class ListView :
	public Control
{
	int columnCount;
	int itemIndex;
	int padding;
public:
	ListView(const Position *pos, HINSTANCE instance, HWND parent, int padding = 0);
	~ListView(void);
	void addColumn(const String *name);
	int addItem(const String *itemName);
	void addSubItem(const String *subItemName, int itemIndex, int subItemIndex);
	void resize(int width, int height);
};
