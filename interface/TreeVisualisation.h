#pragma once

#include <Windows.h>
#include <memory>

#include "NodeVisualisation.h"

class TreeVisualisation
{
public:

	TreeVisualisation();
	TreeVisualisation(HWND parentHandle);
	~TreeVisualisation();

	/*
	* После создания двух листьев (в случае суммы -- один сразу раздваивается на части) знак операции показыается в неизменяемом edit control,
	* по бокам от него возникают две ячейки. Если операция была вызвана из какой-то ячейки, то данный блок заполняется в левую
	* (в случае суммы -- под знак суммы).
	*/
	void createChildrens(NodeType operationType);

	/*
	* Удаление узлов. В случае, когда другой блок не пустой, он переносится на место операции
	* То есть "123| + 42", где | - каретка, заменится на "42|".
	*/
	void deleteNode();

	void paint(int top_margin, int left_margin);

	void changeActiveNode(HWND hEditControl);

private:

	HWND mainWindow;
	std::shared_ptr<NodeVisualisation> head = nullptr;
	NodeVisualisation* activeNode;
};

