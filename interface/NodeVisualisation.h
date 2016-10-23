#pragma once

#include <Windows.h>
#include <memory>

#include "CEditControl.h"
#include "atltypes.h"
#include "Windef.h"

const CRect EMPTY_RECT = CRect(0, 0, 0, 0);
const CPoint EMPTY_POINT = CPoint(0, 0);

// label ячейки
enum NodeType 
{
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

// Местоположение узла относительно родителя
enum Positioning
{
	Left,
	Right,
	Up,
	Bottom,
	UpRight,
	NotExist,
	IsNotKnown
};

class NodeVisualisation
{
public:

	NodeVisualisation(NodeVisualisation* _nodeParent, NodeType _nodeType, bool _isLeftChild, HWND _hWndParentWindow);
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
	NodeType getNodeType();

	// В дальнейшем нужно не принимать параметры смещения сверху и слева, а заранее предпосчитывать их
	int paint(int top_margin, int left_margin);
	int paintTree(int top_margin, int left_margin);
	CRect determineRectsByLeftChild(CRect neighbour_rect, Positioning positioning);
	CRect changeRectsByRightChild(CRect neighbour_rect, Positioning positioning);
	CRect determineCoordinatesTree(CRect parent_rect, Positioning pos_against_parent);

	void setFocus();

	NodeVisualisation* findNode(HWND hEditControl);

private:

	NodeVisualisation() = delete;

	NodeType nodeType;
	bool isLeftChild;
	std::shared_ptr<NodeVisualisation> parent;
	std::shared_ptr<NodeVisualisation> leftChild;
	std::shared_ptr<NodeVisualisation> rightChild;

	HWND hWndParentWindow;
	CEditControl editControl;

	void processNodeType();
	// определить, в какую сторону относительно родителя сдвинут эдит-контрол
	Positioning determinePositioning(NodeVisualisation* node, bool isLeftChild);
	Positioning determinePositioningMy();
};

