#include "TreeVisualisation.h"

TreeVisualisation::TreeVisualisation(HWND parentHandle) : mainWindow(parentHandle)
{
	head = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(mainWindow, nullptr, NodeVisualisation::operationAssign, true));
	head->createChildrens(NodeVisualisation::operationAssign);
	activeNode = head->getRightNode().get();
}

TreeVisualisation::~TreeVisualisation()
{
	head->resetNodes();
	head.reset();
}

void TreeVisualisation::createChildrens(unsigned int operationType)
{
	if( !(activeNode->createChildrens(operationType)) ) {
		return;
	}

	activeNode = activeNode->getLeftNode().get();
	if( operationType == NodeVisualisation::operationSum ) {
		activeNode = activeNode->getLeftNode().get();
	}
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
	if( activeNode->getTypeOfOperation() != NodeVisualisation::operationValue ) {
		return;
	}

	if( activeNode->getParentNode()->getTypeOfOperation == NodeVisualisation::operationSum ) {
		if( !(activeNode->getOrientation()) ) {
			activeNode = activeNode->getParentNode().get();
			bool isLeft = activeNode->getOrientation();
			activeNode = activeNode->getParentNode().get();
			if( isLeft ) {
				activeNode->getLeftNode()->resetNodes();
			} else {
				activeNode->getRightNode()->resetNodes();
			}
			activeNode->changeOneChildren(isLeft);
		}
		return;
	}

	if( activeNode->getParentNode()->getTypeOfOperation == NodeVisualisation::operationSumParameters ) {
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
