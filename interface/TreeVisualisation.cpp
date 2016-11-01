#include "TreeVisualisation.h"

TreeVisualisation::TreeVisualisation() 
{
}

TreeVisualisation::TreeVisualisation(HWND parentHandle) : mainWindow(parentHandle)
{
	head = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(nullptr, Value, true, parentHandle));
	//вернуть - убрано для debug'a
	head->createChildrens(Assign);
	//head->createChildrens(Divide);
	activeNode = head->getLeftNode().get();
	activeNode->setFocus();
}

TreeVisualisation::~TreeVisualisation()
{
	if (head != nullptr) {
		head->resetNodes();
		head.reset();
	}
}

void TreeVisualisation::createChildrens(NodeType operationType)
{
	NodeVisualisation* parent_node = activeNode->getParentNode();
	bool new_node_orientation = activeNode->getOrientation();
	NodeVisualisation* new_node = new NodeVisualisation(parent_node, operationType, new_node_orientation, mainWindow);
	activeNode->setOrientation(true);
	activeNode->setParentNode(new_node);
	new_node->setChild(true, parent_node->getNode(new_node_orientation));
	new_node->changeOneChildren(false, new NodeVisualisation(new_node, Value, false, mainWindow));
	parent_node->changeOneChildren(new_node_orientation, new_node);
	activeNode->setFocus();

	//if (!(activeNode->createChildrens(operationType))) {
	//	return;
	//}
	//activeNode = activeNode->getLeftNode().get();
	//activeNode->setFocus();
}

void TreeVisualisation::deleteNode()
{
	if( activeNode == nullptr ) {
		return;
	}
	if( activeNode->getParentNode() == nullptr) {
		//edit.text = '';
		return;
	}
	if( activeNode->getTypeOfOperation() != Value ) {
		return;
	}

	if( activeNode->getParentNode()->getTypeOfOperation() == Summ ) {
		if( !(activeNode->getOrientation()) ) {
			activeNode = activeNode->getParentNode();
			bool isLeft = activeNode->getOrientation();
			activeNode = activeNode->getParentNode();
			if( isLeft ) {
				activeNode->getLeftNode()->resetNodes();
			} else {
				activeNode->getRightNode()->resetNodes();
			}
			activeNode->changeOneChildren(isLeft, nullptr);
		}
		return;
	}

	if( activeNode->getParentNode()->getTypeOfOperation() == SummParametres ) {
		activeNode = activeNode->getParentNode()->getParentNode();
		activeNode->getLeftNode()->resetNodes();
		activeNode->getLeftNode().reset();
		bool isLeft = activeNode->getOrientation();
		NodeVisualisation* valueUnderSum = activeNode->getRightNode().get();
		activeNode = activeNode->getParentNode();
		activeNode->changeOneChildren(isLeft, valueUnderSum);
		if( isLeft ) {
			activeNode = activeNode->getLeftNode().get();
		} else {
			activeNode = activeNode->getRightNode().get();
		}
		return;
	}

	bool isLeft = activeNode->getOrientation();
	NodeVisualisation* value;
	activeNode = activeNode->getParentNode();
	if( !isLeft ) {
		value = activeNode->getLeftNode().get();
	} else {
		value = activeNode->getRightNode().get();
	}
	isLeft = activeNode->getOrientation();
	activeNode = activeNode->getParentNode();
	activeNode->changeOneChildren(isLeft, value);
	if( isLeft ) {
		activeNode = activeNode->getLeftNode().get();
	}
	else {
		activeNode = activeNode->getRightNode().get();
	}
}

void TreeVisualisation::paint(int top_margin, int left_margin) {
	head->determineCoordinatesTree(EMPTY_RECT, NotExist);
	head->paintTree(top_margin, left_margin);
}


void TreeVisualisation::changeActiveNode(HWND hEditControl)
{
	NodeVisualisation* found = head->findNode(hEditControl);
	if (found) {
		activeNode = found;
		activeNode->setFocus();
	}
}

NodeVisualisation TreeVisualisation::getActiveNode() {
	return *(this->activeNode);
}

void TreeVisualisation::setControlWidth(int width) {
	activeNode->setControlWidth(width);
}


void TreeVisualisation::moveActiveControlLeft() {
	if (activeNode->getLeftNode()) {
		activeNode = activeNode->getLeftNode().get();
		while (activeNode->getRightNode()) {
			activeNode = activeNode->getRightNode().get();
		}
	} else {
		NodeVisualisation* activeNodeNew = activeNode;
		while (activeNodeNew->getOrientation() == true) {
			activeNodeNew = activeNodeNew->getParentNode();
			if (!activeNodeNew->getParentNode()) {
				return;
			}
		}
		activeNode = activeNodeNew->getParentNode();
	}
	activeNode->setFocus();
}

void TreeVisualisation::moveActiveControlRight() {
	// should be corrected to check that we are behind the last letter in the editcontrol
	//if so, do moving active control, else - not

	//POINT point;
	//if (GetCursorPos(&point)) {
	//	ScreenToClient(activeNode->getEditControl().GetHandle(), &point);
	//	long coord = point.x;
	//	std::wstring str = std::to_wstring(coord);
	//	MessageBox(mainWindow, str.c_str(), L"Не выбрана ячейка", MB_OK | MB_ICONWARNING);
	//}

	if (activeNode->getRightNode()) {
		activeNode = activeNode->getRightNode().get();
		while (activeNode->getLeftNode()) {
			activeNode = activeNode->getLeftNode().get();
		}
	}
	else {
		NodeVisualisation* activeNodeNew = activeNode;
		while (activeNodeNew->getOrientation() == false) {
			activeNodeNew = activeNodeNew->getParentNode();
			if (!activeNodeNew->getParentNode()) {
				return;
			}
		}
		activeNode = activeNodeNew->getParentNode();
	}
	activeNode->setFocus();
}