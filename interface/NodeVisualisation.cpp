#include "NodeVisualisation.h"

const int SIZE_BETWEEN_CONTROLS = 10;

NodeVisualisation::NodeVisualisation(NodeVisualisation* _nodeParent, NodeType _nodeType,
	bool _isLeftChild, HWND _hWndParentWindow)
	: parent(_nodeParent), leftChild(nullptr), rightChild(nullptr),
		nodeType(_nodeType), isLeftChild(_isLeftChild), hWndParentWindow(_hWndParentWindow)
{
	editControl = CEditControl();
	editControl.Create(hWndParentWindow);
	processNodeType();
}

void NodeVisualisation::processNodeType()
{
	switch (nodeType)
	{
	case Assign:
		::SetWindowText(editControl.GetHandle(), (LPWSTR)L"=");
		break;
	case Plus:
		::SetWindowText(editControl.GetHandle(), (LPWSTR)L"+");
		break;
	case Minus:
		::SetWindowText(editControl.GetHandle(), (LPWSTR)L"-");
		break;
	case Multiply:
		::SetWindowText(editControl.GetHandle(), (LPWSTR)L"*");
		break;
	case Divide:
		::SetWindowText(editControl.GetHandle(), (LPWSTR)L"/");
		break;
	case Power:
		::SetWindowText(editControl.GetHandle(), (LPWSTR)L"^");
		break;
	case Summ:
		::SetWindowText(editControl.GetHandle(), (LPWSTR)L"summ");
		break;
	default:
		break;
	}
}

NodeVisualisation::~NodeVisualisation()
{
	//Разрушение edit'а
}

bool NodeVisualisation::getOrientation()
{
	return isLeftChild;
}

std::shared_ptr<NodeVisualisation> NodeVisualisation::getLeftNode()
{
	return leftChild;
}

std::shared_ptr<NodeVisualisation> NodeVisualisation::getRightNode()
{
	return rightChild;
}

std::shared_ptr<NodeVisualisation> NodeVisualisation::getParentNode()
{
	return parent;
}

unsigned int NodeVisualisation::getTypeOfOperation()
{
	return nodeType;
}

bool NodeVisualisation::createChildrens(NodeType operationType)
{
	leftChild = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(this, Value, true, hWndParentWindow));
	rightChild = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(this, Value, false, hWndParentWindow));

	switch( operationType )
	{
		case Assign:
		{
			break;
		}
		// Здесь нет ошибки, эти операции идентичны.
		case Plus:
		case Minus:
		case Multiply:
		case Divide:
		case Power:
		{
			//leftChild.edit = edit.getText();
			//Изменить edit
			break;
		}
		case Summ:
		{
			leftChild.get()->createChildrens(SummParametres);
			//rightChild.edit = edit.getText();
			//Изенить edit
			break;
		}
		case SummParametres:
		{
			//Удалить edit
			break;
		}
		default:
		{
			leftChild = nullptr;
			rightChild = nullptr;
			return false;
		}
	}

	nodeType = operationType;
	processNodeType();
	return true;
}

void NodeVisualisation::resetNodes()
{
	if( leftChild != nullptr ) {
		leftChild->resetNodes();
	} else {
		leftChild.reset();
		leftChild == nullptr;
	}
	if( rightChild != nullptr ) {
		rightChild->resetNodes();
	} else {
		rightChild.reset();
		rightChild == nullptr;
	}
}

bool NodeVisualisation::changeOneChildren(bool isLeft, NodeVisualisation* node)
{
	//std::shared_ptr<NodeVisualisation> nodeImmutable = (isLeft) ? rightChild : leftChild;
	std::shared_ptr<NodeVisualisation> nodeMutable = (!isLeft) ? rightChild : leftChild;
	/*if( (nodeMutable->getLeftNode() != nullptr) && (nodeMutable->getLeftNode() != nullptr) ) {
		return false;
	}*/

	//nodeMutable.reset(); //Оно удалит только себя или же и поддерево? Надо проверить...
	if( isLeft ) {
		if( node == nullptr ) {
			leftChild = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(this, Value, isLeft, hWndParentWindow));
		} else {
			leftChild = std::shared_ptr<NodeVisualisation>(node);
		}
	} else {
		if( node == nullptr ) {
			rightChild = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(this, Value, isLeft, hWndParentWindow));
		}
		else {
			rightChild = std::shared_ptr<NodeVisualisation>(node);
		}
	}

	// Should be checked
	return true;
}

HWND NodeVisualisation::getHandle()
{
	return editControl.GetHandle();
}

NodeType NodeVisualisation::getNodeType()
{
	return nodeType;
}

int NodeVisualisation::paint(int top_margin, int left_margin)
{
	::SetWindowPos(editControl.GetHandle(), HWND_TOP, left_margin, top_margin, editControl.GetWidth(), editControl.GetHeight(), 0);
	left_margin += SIZE_BETWEEN_CONTROLS + editControl.GetWidth();
	return left_margin;
}

int NodeVisualisation::paintTree(int top_margin, int left_margin)
{
	if (leftChild) {
		left_margin = leftChild->paintTree(top_margin, left_margin);
	}
	left_margin = this->paint(top_margin, left_margin);
	if (rightChild) {
		left_margin = rightChild->paintTree(top_margin, left_margin);
	}
	return left_margin;
}


CRect NodeVisualisation::determineRectsByLeftChild(CRect neighbour_rect, Positioning positioning)
{
	editControl.SetDefaultRect();
	editControl.SetDefaultRectAroundSubtree();
	if (positioning == NotExist) {
		return editControl.GetRectAroundSubtree();
	} else {
	
	}
	return EMPTY_RECT;
}

CRect NodeVisualisation::changeRectsByRightChild(CRect neighbour_rect, Positioning positioning)
{
	return EMPTY_RECT;
}

CRect NodeVisualisation::determineCoordinatesTree(CRect parent_rect, Positioning pos_against_parent)
{
	// расположение левого ребенка относительно узла
	Positioning pos_left_child = determinePositioning(leftChild.get(), true);
	// расположение правого ребенка относительно узла
	Positioning pos_right_child = determinePositioning(rightChild.get(), true);

	// Речь везде идет о прямоугольниках вокруг поддерева
	CRect left_rect = EMPTY_RECT,
		right_rect = EMPTY_RECT,
		my_rect = EMPTY_RECT;

	if (leftChild) {
		// Например, если pos_against_parent == Bottom, то надо смещать вниз относительно родителя
		left_rect = leftChild->determineCoordinatesTree(parent_rect, pos_against_parent);
	}
	my_rect = determineRectsByLeftChild(left_rect, pos_left_child);
	if (rightChild) {
		right_rect = rightChild->determineCoordinatesTree(my_rect, pos_right_child);
	}
	my_rect = changeRectsByRightChild(right_rect, pos_right_child);
	return my_rect;
}


void NodeVisualisation::setFocus()
{
	::SetFocus(editControl.GetHandle());
}

NodeVisualisation* NodeVisualisation::findNode(HWND hEditControl)
{
	NodeVisualisation* found = nullptr;

	if (editControl.GetHandle() == hEditControl) {
		return this;
	} 

	if (leftChild) {
		found = leftChild->findNode(hEditControl);
		if (found) {
			return found;
		}
	}

	if (rightChild) {
		found = rightChild->findNode(hEditControl);
		if (found) {
			return found;
		}
	}

	return nullptr;
}

Positioning NodeVisualisation::determinePositioning(NodeVisualisation* node, bool isLeftChild)
{
	if (!node) {
		return NotExist;
	}
	switch (node->getNodeType()) {
	case Assign:
	case Plus:
	case Minus:
	case Multiply:
		if (isLeftChild) {
			return Left;
		} else {
			return Right;
		}
		break;
	case Divide:
		if (isLeftChild) {
			return Up;
		}
		else {
			return Bottom;
		}
		break;
	case Power:
		if (isLeftChild) {
			return Left;
		}
		else {
			return UpRight;
		}
		break;
	case Summ:
		// TODO
		break;
	case SummParametres:
		// TODO
		break;
	default:
		break;
	}
	return IsNotKnown;
}

Positioning NodeVisualisation::determinePositioningMy() {
	return determinePositioning(parent.get(), isLeftChild);
}
