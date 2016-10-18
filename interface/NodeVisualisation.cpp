#include "NodeVisualisation.h"

const unsigned int NodeVisualisation::operationPlus = 0;
const unsigned int NodeVisualisation::operationMinus = 1;
const unsigned int NodeVisualisation::operationMultiply = 2;
const unsigned int NodeVisualisation::operationSum = 3;
const unsigned int NodeVisualisation::operationFrac = 4;
const unsigned int NodeVisualisation::operationPower = 5;
const unsigned int NodeVisualisation::operationSumParameters = 6;
const unsigned int NodeVisualisation::operationValue = 7;
const unsigned int NodeVisualisation::operationAssign = 8;

NodeVisualisation::NodeVisualisation(const NodeVisualisation* nodeParent, const unsigned int operationType,
	const bool isLeftChild, const HWND parentHandle = 0)
	: mainWindow(nodeParent->getHandle()), parent(nodeParent), leftChild(nullptr), rightChild(nullptr),
		typeOfOperation(operationType), orientationIsLeft(isLeftChild)
{
	value.Create(mainWindow);
	if( nodeParent == nullptr ) {
		parent == nullptr;
	}
}

NodeVisualisation::~NodeVisualisation()
{
	//Разрушение edit'а
}

bool NodeVisualisation::getOrientation() const
{
	return orientationIsLeft;
}

std::shared_ptr<NodeVisualisation> NodeVisualisation::getLeftNode() const
{
	return leftChild;
}

std::shared_ptr<NodeVisualisation> NodeVisualisation::getRightNode() const
{
	return rightChild;
}

std::shared_ptr<NodeVisualisation> NodeVisualisation::getParentNode() const
{
	return parent;
}

unsigned int NodeVisualisation::getTypeOfOperation() const
{
	return typeOfOperation;
}

bool NodeVisualisation::createChildrens(unsigned int operationType)
{
	leftChild = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(this, operationValue, true));
	rightChild = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(this, operationValue, false));

	switch( operationType )
	{
		case operationAssign:
		{
			break;
		}
		// Здесь нет ошибки, эти операции идентичны.
		case operationPlus:
		case operationMinus:
		case operationMultiply:
		case operationFrac:
		case operationPower:
		{
			//leftChild.edit = edit.getText();
			//Изменить edit
			break;
		}
		case operationSum:
		{
			leftChild.get()->createChildrens(operationSumParameters);
			//rightChild.edit = edit.getText();
			//Изенить edit
			break;
		}
		case operationSumParameters:
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

	typeOfOperation = operationType;
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

bool NodeVisualisation::changeOneChildren(bool isLeft, NodeVisualisation* node = nullptr)
{
	//std::shared_ptr<NodeVisualisation> nodeImmutable = (isLeft) ? rightChild : leftChild;
	std::shared_ptr<NodeVisualisation> nodeMutable = (!isLeft) ? rightChild : leftChild;
	/*if( (nodeMutable->getLeftNode() != nullptr) && (nodeMutable->getLeftNode() != nullptr) ) {
		return false;
	}*/

	//nodeMutable.reset(); //Оно удалит только себя или же и поддерево? Надо проверить...
	if( isLeft ) {
		if( node == nullptr ) {
			leftChild = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(this, operationValue, isLeft));
		} else {
			leftChild = std::shared_ptr<NodeVisualisation>(node);
		}
	} else {
		if( node == nullptr ) {
			rightChild = std::shared_ptr<NodeVisualisation>(new NodeVisualisation(this, operationValue, isLeft));
		}
		else {
			rightChild = std::shared_ptr<NodeVisualisation>(node);
		}
	}
}

HWND NodeVisualisation::getHandle() const
{
	return value.GetHandle();
}
