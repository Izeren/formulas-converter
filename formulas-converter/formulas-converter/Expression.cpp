#include "Expressions.h"


//CIdExp:
CIdExp::CIdExp(std::string name)
{
	this->name = name;
}

IVisitorResult* CIdExp::Accept(IVisitor *visitor) 
{
	return visitor->Visit(this);
}

CIdExp::~CIdExp() {}

//CNumExp:
CNumExp::CNumExp(std::string value)
{
	this->value = value;
}

IVisitorResult* CNumExp::Accept(IVisitor *visitor)
{
	return visitor->Visit(this);
}

CNumExp::~CNumExp() {}

//COpExp:
IVisitorResult* COpExp::Accept(IVisitor *visitor) {
	return visitor->Visit(this);
}

COpExp::COpExp(IExpression *leftOperand, IExpression *rightOperand, TOperation operation) {
	this->leftOperand = leftOperand;
	this->rightOperand = rightOperand;
	this->operation = operation;

}

COpExp::~COpExp() {
	if (leftOperand) {
		delete leftOperand;
	}
	if (rightOperand) {
		delete rightOperand;
	}
}