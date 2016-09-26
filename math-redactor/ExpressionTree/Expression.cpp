#include "Expressions.h"
#include "LSVUtils.h"
#include <string>

//CIdExp:
//-------------------------------------------------------------------------------------------------
CIdExp::CIdExp(std::string name)
{
	if (LSVUtils::checkIdName(name))
	{
		this->name = name;
	}
	else
	{
		this->name = "error_name";
	}
}

IVisitorResult* CIdExp::Accept(IVisitor *visitor) 
{
	return visitor->Visit(this);
}

CIdExp::~CIdExp() {}

bool CIdExp::setName(std::string name)
{
	bool isValidName = false;
	if (LSVUtils::checkIdName(name))
	{
		this->name = name;
		isValidName = true;
	}
	return isValidName;
}

std::string CIdExp::getName() const 
{
	return this->name;

}

bool CIdExp::setAddress(const double* address)
{
	bool isNonzeroAddress = false;
	if (address) {
		this->valueAddress = address;
		isNonzeroAddress = true;
	}
	return isNonzeroAddress;
}

const double *CIdExp::getAddress() const
{
	return this->valueAddress;
}

double CIdExp::getValue() const
{
	if (this->valueAddress) {
		return *(this->valueAddress);
	}
}

//CNumExp:
//-------------------------------------------------------------------------------------------------
CNumExp::CNumExp(std::string value)
{
	if (LSVUtils::checkDouble(value)) {
		this->value = atof(value.c_str());
	}
	else 
	{
		this->value = 0;
	}
}

CNumExp::CNumExp(double value)
{
	this->value = value;
}

IVisitorResult* CNumExp::Accept(IVisitor *visitor)
{
	return visitor->Visit(this);
}

CNumExp::~CNumExp() {}

bool CNumExp::setValue(std::string value)
{
	bool isValidValue = false;
	if (LSVUtils::checkDouble(value)) {
		this->value = atof(value.c_str());
		isValidValue = true;
	}
	return isValidValue;
}

void CNumExp::setValue(double value)
{
	this->value = value;
}

double CNumExp::getValue() const
{
	return this->value;
}


//COpExp:
//-------------------------------------------------------------------------------------------------


COpExp::COpExp(IExpression *firstOperand, IExpression *secondOperand, TOperation operation) {
	this->firstOperand = firstOperand;
	this->secondOperand = secondOperand;
	this->operation = operation;
}

IVisitorResult* COpExp::Accept(IVisitor *visitor)
{
	return visitor->Visit(this);
}

COpExp::~COpExp() {
	if (firstOperand) {
		delete firstOperand;
	}
	if (secondOperand) {
		delete secondOperand;
	}
}

bool COpExp::setFirstOperand(IExpression *pointer)
{
	bool isNonzeroPointer = false;
	if (pointer) {
		this->firstOperand = pointer;
		isNonzeroPointer = true;
	}
	return isNonzeroPointer;
}

IExpression *COpExp::getFirstOperand() const
{
	return this->firstOperand;
}

bool COpExp::setSecondOperand(IExpression *pointer)
{
	bool isNonzeroPointer = false;
	if (pointer) {
		this->secondOperand = pointer;
		isNonzeroPointer = true;
	}
	return isNonzeroPointer;
}

IExpression *COpExp::getSecondOperand() const
{
	return this->secondOperand;
}

void COpExp::setOperation(TOperation operation)
{
	this->operation = operation;
}

TOperation COpExp::getOperation() const
{
	return this->operation;
}

//CSumExp
//-------------------------------------------------------------------------------------------------

CSumExp::CSumExp(std::string indexName, int startId, int finishId, IExpression *expression)
{
	if (LSVUtils::checkIdName(indexName)) 
	{
		this->indexName = indexName;
	}
	else
	{
		indexName = "error_name";
	}
	this->startId = startId;
	this->finishId = finishId;
	this->expression = expression;
}

IVisitorResult* CSumExp::Accept(IVisitor *visitor)
{
	return visitor->Visit(this);
}

CSumExp::~CSumExp() {}

bool CSumExp::setIndexName(std::string indexName)
{
	bool isValidName = false;
	if (LSVUtils::checkIdName(indexName)) {
		this->indexName = indexName;
		isValidName = true;
	}
	return isValidName;
}

std::string CSumExp::getIndexName() const
{
	return this->indexName;
}

void CSumExp::setStartId(int startId)
{
	this->startId = startId;
}

int CSumExp::getStartId() const
{
	return this->startId;
}

void CSumExp::setFinishId(int finishId)
{
	this->finishId = finishId;
}

int CSumExp::getFinishId() const
{
	return this->finishId;
}

bool CSumExp::setExpression(IExpression *pointer)
{
	bool isNonzeroPointer = false;
	if (pointer) 
	{
		this->expression = pointer;
		isNonzeroPointer = true;
	}
	return isNonzeroPointer;
}

IExpression *CSumExp::getExpression() const
{
	return this->expression;
}
