#include "Expression.h"
#include "../utils/LSVUtils.h"
#include <string>

//CIdExp:
//-------------------------------------------------------------------------------------------------
CIdExp::CIdExp(const std::string &name)
{
	if (LSVUtils::checkIdName(name))
	{
		this->name = name;
	}
	else
	{
		this->name = BAD_ID;
	}
}

CIdExp::CIdExp()
{
	this->name = BAD_ID;
}

void CIdExp::Accept(IVisitor &visitor) 
{
	visitor.Visit(*this);
}

bool CIdExp::setName(const std::string &name)
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
	else {
		return 0;
	}
}

//CNumExp:
//-------------------------------------------------------------------------------------------------
CNumExp::CNumExp(const std::string &value)
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

void CNumExp::Accept(IVisitor &visitor)
{
	visitor.Visit(*this);
}

bool CNumExp::setValue(const std::string &value)
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

COpExp::COpExp() {
	this->firstOperand = NULL;
	this->secondOperand = NULL;
	this->operation = PLUS;
}

COpExp::COpExp(std::shared_ptr<IExpression> firstOperand, std::shared_ptr<IExpression> secondOperand, TOperation operation) {
	this->firstOperand = firstOperand;
	this->secondOperand = secondOperand;
	this->operation = operation;
}

void COpExp::Accept(IVisitor &visitor)
{
	visitor.Visit(*this);
}

bool COpExp::setFirstOperand(std::shared_ptr<IExpression> pointer)
{
	bool isNonzeroPointer = false;
	if (pointer) {
		this->firstOperand = pointer;
		isNonzeroPointer = true;
	}
	return isNonzeroPointer;
}

std::shared_ptr<IExpression> COpExp::getFirstOperand() const
{
	return this->firstOperand;
}

bool COpExp::setSecondOperand(std::shared_ptr<IExpression> pointer)
{
	bool isNonzeroPointer = false;
	if (pointer) {
		this->secondOperand = pointer;
		isNonzeroPointer = true;
	}
	return isNonzeroPointer;
}

std::shared_ptr<IExpression> COpExp::getSecondOperand() const
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

CSumExp::CSumExp()
{
	this->indexName = BAD_ID;
	this->startId = 1;
	this->finishId = 5;
	this->expression = NULL;
}

CSumExp::CSumExp(const std::string &indexName, int startId, int finishId, std::shared_ptr<IExpression> expression)
{
	if (LSVUtils::checkIdName(indexName)) 
	{
		this->indexName = indexName;
	}
	else
	{
		this->indexName = BAD_ID;
	}
	this->startId = startId;
	this->finishId = finishId;
	this->expression = expression;
}

void CSumExp::Accept(IVisitor &visitor)
{
	visitor.Visit(*this);
}

bool CSumExp::setIndexName(const std::string &indexName)
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

bool CSumExp::setExpression(std::shared_ptr<IExpression> pointer)
{
	bool isNonzeroPointer = false;
	if (pointer) 
	{
		this->expression = pointer;
		isNonzeroPointer = true;
	}
	return isNonzeroPointer;
}

std::shared_ptr<IExpression> CSumExp::getExpression() const
{
	return this->expression;
}
