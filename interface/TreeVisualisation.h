#pragma once

#include <Windows.h>
#include <memory>

#include "NodeVisualisation.h"

class TreeVisualisation
{
public:

	TreeVisualisation(HWND parentHandle);
	~TreeVisualisation();

	void createChildrens(unsigned int operationType);
	void deleteNode();

private:

	HWND mainWindow;
	std::shared_ptr<NodeVisualisation> head;
	NodeVisualisation* activeNode;
};

