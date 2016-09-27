#include "CSintacticValdationVisitor.h"

void CSintacticValidationVisitor::ClearVisitor()
{
	this->visibleIds.clear();
}


void CSintacticValidationVisitor::Visit(COpExp &exp)
{

	
	bool leftValidationStatus = false, rightValidationStatus = false;
	if (exp.getFirstOperand())
	{
		exp.getFirstOperand()->Accept(*this);
		leftValidationStatus = this->isValidated;
	}
	
	if (exp.getSecondOperand())
	{
		exp.getSecondOperand()->Accept(*this);
		rightValidationStatus = this->isValidated;
	}
	
	this->isValidated &= leftValidationStatus & rightValidationStatus;
}

void CSintacticValidationVisitor::Visit(CNumExp &exp) {}

void CSintacticValidationVisitor::Visit(CIdExp &exp)
{
	bool validationStatus = exp.getName() != BAD_ID;
	this->isValidated &= validationStatus;
}

void CSintacticValidationVisitor::Visit(CSumExp &exp) {

	bool validationStatus = true;
	if (exp.getIndexName() == BAD_ID || exp.getExpression() == NULL)
	{
		validationStatus = false;
	}
	this->isValidated &= validationStatus;
}

bool CSintacticValidationVisitor::getValidationStatus() const
{
	return this->isValidated;
}
