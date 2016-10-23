#include "TreeVisualisation.h"

TreeVisualisation::TreeVisualisation() 
{
}

TreeVisualisation::TreeVisualisation(HWND parentHandle) : mainWindow(parentHandle)
{
	head = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(nullptr, Value, true, parentHandle));
	//вернуть - убрано для debug'a
	//head->createChildrens(Assign);
	head->createChildrens(Divide);
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
	if( !(activeNode->createChildrens(operationType)) ) {
		return;
	}

	activeNode = activeNode->getLeftNode().get();
	if( operationType == Summ ) {
		activeNode = activeNode->getLeftNode().get();
	}
	activeNode->setFocus();
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
			activeNode = activeNode->getParentNode().get();
			bool isLeft = activeNode->getOrientation();
			activeNode = activeNode->getParentNode().get();
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
		activeNode = activeNode->getParentNode()->getParentNode().get();
		activeNode->getLeftNode()->resetNodes();
		activeNode->getLeftNode().reset();
		bool isLeft = activeNode->getOrientation();
		NodeVisualisation* valueUnderSum = activeNode->getRightNode().get();
		activeNode = activeNode->getParentNode().get();
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
	activeNode = activeNode->getParentNode().get();
	if( !isLeft ) {
		value = activeNode->getLeftNode().get();
	} else {
		value = activeNode->getRightNode().get();
	}
	isLeft = activeNode->getOrientation();
	activeNode = activeNode->getParentNode().get();
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
