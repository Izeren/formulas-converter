#pragma once

#include <Windows.h>
#include <memory>

#include "CEditControl.h"

enum NodeType {
	Value,
	Assign,
	Plus,
	Minus,
	Multiply,
	Divide,
	Power,
	Summ,
	SummParametres
};

class NodeVisualisation
{
public:

	NodeVisualisation(NodeVisualisation* _nodeParent, NodeType _nodeType, bool isLeftChild, HWND _hWndParentWindow);
	~NodeVisualisation();

	bool getOrientation(); // left == true, rigth == false
	unsigned int getTypeOfOperation();
	std::shared_ptr<NodeVisualisation> getLeftNode();
	std::shared_ptr<NodeVisualisation> getRightNode();
	std::shared_ptr<NodeVisualisation> getParentNode();
	
	bool createChildrens(NodeType operationValue);
	// Рекурсивная очистка ячеек
	void resetNodes();
	// Замена одного из поддеревьев.
	bool changeOneChildren(bool isLeft, NodeVisualisation* node);

	HWND getHandle();

	// В дальнейшем нужно не принимать параметры смещения сверху и слева, а заранее предпосчитывать их
	int paint(int top_margin, int left_margin);
	int paintTree(int top_margin, int left_margin);

private:

	NodeVisualisation() = delete;

	NodeType nodeType;
	bool orientationIsLeft;
	std::shared_ptr<NodeVisualisation> parent;
	std::shared_ptr<NodeVisualisation> leftChild;
	std::shared_ptr<NodeVisualisation> rightChild;

	HWND hWndParentWindow;
	CEditControl editControl;

	void processNodeType();
};

