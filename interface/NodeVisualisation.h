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
	PositioningError
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
	std::shared_ptr<NodeVisualisation> getNode(bool isLeftChild);
	
	bool createChildrens(NodeType operationValue);
	// Рекурсивная очистка ячеек
	void resetNodes();
	// Замена одного из поддеревьев.
	bool changeOneChildren(bool isLeft, NodeVisualisation* node);

	HWND getHandle();
	NodeType getNodeType();

	CEditControl getEditControl() const;

	// В дальнейшем нужно не принимать параметры смещения сверху и слева, а заранее предпосчитывать их
	void paint(int top_margin, int left_margin);
	void paintTree(int top_margin, int left_margin);
	// Определяем положение узла у которого нет детей. 
	// neighbour_rect - местоположение ближайшего узла, находящегося левее по дереву и являющегося листом (они будут нарисованы рядом)
	// pos_against_neighbour - расположение данного узла относительно neighbour_rect
	CRect determineRectsByNearestParent(CRect neighbour_rect, Positioning pos_against_neighbour);
	// Определяем положение узла у которого уже разобрались с левым ребенком. 
	// left_rect - местоположение левого ребенка
	// pos_left - расположение left_rect относительно данного узла
	CRect determineRectsByLeftChild(CRect left_rect, Positioning pos_left);
	// Возвращаемся из правого ребенка, смещаем узлы, центрируя друг относительно друга 
	// right_rect - местоположение правого ребенка
	// pos_right - расположение right_rect относительно данного узла
	CRect changeRectsByRightChild(CRect right_rect, Positioning pos_right);
	// Определить местоположение всех узлов поддерева
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
	// определить местоположение ребенка узла относительно родителя
	Positioning determinePositioning(NodeVisualisation* node, bool isLeftChild);
	// определить местоположение ребенка этой вершины относительно родителя
	Positioning determinePositioning(bool isLeftChild);

	// смещаем местоположение всех узлов поддерева на offset
	void offsetTreeDown(CPoint offset);
	// смещаем данный узел на offset
	// если is_right_offset = true, то смещается правое поддерево, иначе текущий узел и левое поддерево
	void makeOffset(bool is_right_offset, CPoint offset);
	// смещаем все узлы дерева над нами
	// пока не встретим узел, относительно которого текущий узел будет расположен как pos_stop
	void offsetTreeUp(CPoint offset, Positioning pos_stop);
};

