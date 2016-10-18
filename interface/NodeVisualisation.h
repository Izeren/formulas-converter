#pragma once

#include <Windows.h>
#include <memory>

#include "CEditControl.h"

class NodeVisualisation
{
public:

	NodeVisualisation(const NodeVisualisation* nodeParent, const unsigned int type, const bool isLeftChild, HWND parentWindow = 0);
	~NodeVisualisation();

	bool getOrientation() const; // left == true, rigth == false
	unsigned int getTypeOfOperation() const;
	std::shared_ptr<NodeVisualisation> getLeftNode() const;
	std::shared_ptr<NodeVisualisation> getRightNode() const;
	std::shared_ptr<NodeVisualisation> getParentNode() const;
	
	bool createChildrens(unsigned int operationValue);
	void resetNodes();
	bool changeOneChildren(bool isLeft, NodeVisualisation* node = nullptr);

	HWND getHandle() const;

	const static unsigned int operationPlus;
	const static unsigned int operationMinus;
	const static unsigned int operationMultiply;
	const static unsigned int operationSum;
	const static unsigned int operationFrac;
	const static unsigned int operationPower;
	const static unsigned int operationSumParameters;
	const static unsigned int operationValue;
	const static unsigned int operationAssign;

private:

	NodeVisualisation() = delete;

	unsigned int typeOfOperation;
	bool orientationIsLeft;
	std::shared_ptr<NodeVisualisation> parent;
	std::shared_ptr<NodeVisualisation> leftChild;
	std::shared_ptr<NodeVisualisation> rightChild;

	HWND mainWindow;
	CEditControl value;
};

