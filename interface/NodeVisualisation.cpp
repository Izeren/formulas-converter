#include "NodeVisualisation.h"
#include "CEditControl.h"

const int SIZE_BETWEEN_CONTROLS = 10;

NodeVisualisation::NodeVisualisation(NodeVisualisation* _nodeParent, NodeType _nodeType,
	bool _isLeftChild, HWND _hWndParentWindow)
	: parent(_nodeParent), leftChild(nullptr), rightChild(nullptr),
		nodeType(_nodeType), isLeftChild(_isLeftChild), hWndParentWindow(_hWndParentWindow)
{
	editControl = CEditControl();
	editControl.Create(hWndParentWindow);
	processNodeType();
	if (isSpecialSymbol) {
		setControlWidth(MIN_WIDTH_DEFAULT);
		editControl.setIsSpecialSymbol(isSpecialSymbol);
	}
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
		isSpecialSymbol = false;
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

void NodeVisualisation::setOrientation(bool isLeftChild_)
{
	isLeftChild = isLeftChild_;
}

std::shared_ptr<NodeVisualisation> NodeVisualisation::getLeftNode()
{
	return leftChild;
}

std::shared_ptr<NodeVisualisation> NodeVisualisation::getRightNode()
{
	return rightChild;
}

NodeVisualisation* NodeVisualisation::getParentNode()
{
	return parent;
}

void NodeVisualisation::setParentNode(NodeVisualisation* parent_new) 
{
	parent = parent_new;
}


std::shared_ptr<NodeVisualisation> NodeVisualisation::getNode(bool isLeftChild)
{
	if (isLeftChild) {
		return getLeftNode();
	} else {
		return getRightNode();
	}
}


void NodeVisualisation::setChild(bool isLeftChild, std::shared_ptr<NodeVisualisation> child)
{
	if (isLeftChild) {
		leftChild = child;
	} else {
		rightChild = child;
	}
}

unsigned int NodeVisualisation::getTypeOfOperation()
{
	return nodeType;
}

void NodeVisualisation::createChildren()
{
	leftChild = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(this, Value, true, hWndParentWindow));
	rightChild = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(this, Value, false, hWndParentWindow));
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
			HWND leftChildHandle = leftChild->getEditControl().GetHandle();

			HWND handle = editControl.GetHandle();
			int length = SendMessage(handle, WM_GETTEXTLENGTH, 0, 0);
			length++;
			std::wstring text;
			text.resize(length);
			::GetWindowText(handle, (LPWSTR)text.c_str(), length);

			leftChild->setControlWidth(text.length() * MIN_SIZE_SYMBOL);

			::SetWindowText(leftChildHandle, (LPWSTR)text.c_str());
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
	//std::shared_ptr<NodeVisualisation> nodeMutable = (!isLeft) ? rightChild : leftChild;
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

void NodeVisualisation::paint(int top_margin, int left_margin)
{
	::SetWindowPos(
		editControl.GetHandle(), 
		HWND_TOP, 
		left_margin + editControl.GetRect().left, 
		top_margin + editControl.GetRect().top,
		editControl.GetWidth(), 
		editControl.GetHeight(), 0);
}

void NodeVisualisation::paintTree(int top_margin, int left_margin)
{
	if (leftChild) {
		leftChild->paintTree(top_margin, left_margin);
	}
	this->paint(top_margin, left_margin);
	if (rightChild) {
		rightChild->paintTree(top_margin, left_margin);
	}
}

CRect NodeVisualisation::determineRectsByNearestParent(CRect neighbour_rect, Positioning pos_against_neighbour)
{
	editControl.SetDefaultRect();
	switch (pos_against_neighbour) {
	case NotExist:
		break;
	case Right:
	case UpRight:
		editControl.moveLeftAgainstRect(neighbour_rect);
		editControl.offsetInnerRect(POINT{ SIZE_BETWEEN_CONTROLS, 0 });
		break;
	case Bottom:
		editControl.moveDownAgainstRect(neighbour_rect);
		editControl.offsetInnerRect(POINT{ 0, SIZE_BETWEEN_CONTROLS });
		break;
	default:
		break;
	}
	editControl.SetDefaultRectAroundSubtree();
	return editControl.GetRectAroundSubtree();
}

CRect NodeVisualisation::determineRectsByLeftChild(CRect left_rect, Positioning pos_left)
{
	editControl.SetDefaultRect();
	switch (pos_left) {
	case Left:
		editControl.moveLeftCentringHeightAgainstRect(left_rect);
		editControl.offsetInnerRect( POINT{ SIZE_BETWEEN_CONTROLS, 0 } );
		break;
	case Up:
		editControl.moveDownAgainstRect(left_rect);
		// для знака деления
		editControl.copyRectWidthIfLarger(left_rect);
		editControl.offsetInnerRect( POINT{ 0, SIZE_BETWEEN_CONTROLS } );
		break;
	default:
		break;
	}
	editControl.SetDefaultRectAroundSubtree();
	editControl.unionSubtreeRect(left_rect);
	return editControl.GetRectAroundSubtree();
}

void NodeVisualisation::offsetTreeDown(CPoint offset) {
	editControl.offsetBothRects(offset);
	if (leftChild) {
		leftChild->offsetTreeDown(offset);
	}
	if (rightChild) {
		rightChild->offsetTreeDown(offset);
	}
}

void NodeVisualisation::offsetTreeUp(CPoint offset, Positioning pos_stop) {
	if (parent == nullptr || parent->determinePositioning(isLeftChild) == pos_stop) {
		return;
	}
	parent->editControl.offsetBothRects(offset);
	NodeVisualisation* other_child = parent->getNode(!isLeftChild).get();
	if (other_child) {
		other_child->offsetTreeDown(offset);
	}
	parent->offsetTreeUp(offset, pos_stop);
}

void NodeVisualisation::makeOffset(bool is_right_offset, CPoint offset) {
	if (is_right_offset) {
		if (rightChild) {
			rightChild->offsetTreeDown(offset);
		}
	}
	else {
		editControl.offsetBothRects(offset);
		if (leftChild) {
			leftChild->offsetTreeDown(offset);
		}
	}
}

CRect NodeVisualisation::changeRectsByRightChild(CRect right_rect, Positioning pos_right)
{
	switch (pos_right) {
	case Right: {
		// определяем высоту правого и левого ребенка, центрируем
		int this_height = editControl.GetRectAroundSubtree().Height();
		int right_height = right_rect.Height();
		int offset_down = (right_height - this_height) / 2;
		if (offset_down > 0) {
			makeOffset(false, POINT{ 0, offset_down });
		}
		else if (offset_down < 0) {
			makeOffset(true, POINT{ 0, -offset_down });
		}
		break;
	}
	case UpRight: {
		int offset_down = right_rect.Height();
		if (offset_down > 0) {
			makeOffset(false, POINT{ 0, offset_down });
			// нужно подумать над тем, что делать с операцией power, ибо сначала нужно все сместить,
			// а потом центрировать не учитывая степень
			//offsetTreeUp(POINT{ 0, offset_down }, Bottom);
		}
		break;
	}
	case Bottom: {
		// определяем ширину правого и левого ребенка, центрируем
		int this_width = editControl.GetRectAroundSubtree().Width();
		int right_width = right_rect.Width();
		int offset_right = (right_width - this_width) / 2;
		if (offset_right > 0) {
			makeOffset(false, POINT{ offset_right, 0 });
			editControl.copyRectWidthIfLarger(right_rect);
		} 
		else if (offset_right < 0) {
			makeOffset(true, POINT{ -offset_right, 0 });
		}
		break;
	}
	default: {
		break;
	}
	}
	editControl.unionSubtreeRect(right_rect);
	return editControl.GetRectAroundSubtree();
}

CRect NodeVisualisation::determineCoordinatesTree(CRect parent_rect, Positioning pos_against_parent)
{
	// расположение левого ребенка относительно узла
	Positioning pos_left_child = determinePositioning(this, true);
	// расположение правого ребенка относительно узла
	Positioning pos_right_child = determinePositioning(this, false);

	// Речь везде идет о прямоугольниках вокруг поддерева
	CRect left_rect = EMPTY_RECT,
		right_rect = EMPTY_RECT,
		my_rect = EMPTY_RECT;

	if (leftChild) {
		// Например, если pos_against_parent == Bottom, то надо смещать вниз относительно родителя
		left_rect = leftChild->determineCoordinatesTree(parent_rect, pos_against_parent);
		my_rect = determineRectsByLeftChild(left_rect, pos_left_child);
	}
	else {
		my_rect = determineRectsByNearestParent(parent_rect, pos_against_parent);
	}
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
		return PositioningError;
	}
	switch (node->getNodeType()) {
	case Value:
		return NotExist;
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
	return PositioningError;
}

Positioning NodeVisualisation::determinePositioning(bool isLeftChild) {
	return determinePositioning(this, isLeftChild);
}

CEditControl NodeVisualisation::getEditControl() const{
	return editControl;
}

void NodeVisualisation::setControlWidth(int width) {
	editControl.SetWidth(width);
}

NodeVisualisation* NodeVisualisation::createCopy(NodeVisualisation* parent) {
	NodeVisualisation* copiedNode = new NodeVisualisation(parent, nodeType, isLeftChild, hWndParentWindow);
	std::wstring text = editControl.GetText();
	::SetWindowText(copiedNode->editControl.GetHandle(), (LPWSTR)text.c_str());
	CEditControl copiedEditControl = copiedNode->getEditControl();
	copiedEditControl.deleteWhiteSpaces();
	copiedNode->setControlWidth(copiedEditControl.GetWidth());
	if (leftChild) {
		NodeVisualisation* copiedLeftChild = leftChild->createCopy(copiedNode);
		copiedNode->setChild(true, std::shared_ptr<NodeVisualisation>(copiedLeftChild));
	}
	if (rightChild) {
		NodeVisualisation* copiedRightChild = rightChild->createCopy(copiedNode);
		copiedNode->setChild(false, std::shared_ptr<NodeVisualisation>(copiedRightChild));
	}
	return copiedNode;
}