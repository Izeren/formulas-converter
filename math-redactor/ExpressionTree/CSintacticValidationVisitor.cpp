#include "CSintacticValdationVisitor.h"
#include "CSintacticVisitorResults.h"
#include <iostream>
#include <algorithm>
#include "LSVUtils.h"

CSintacticValidationVisitor::CSintacticValidationVisitor() {}

void CSintacticValidationVisitor::ClearVisitor()
{
	visibleIds.clear();
}


IVisitorResult* CSintacticValidationVisitor::Visit(COpExp *exp)
{

	CSintacticVisitorResults* result;
	bool leftValidationStatus = false, rightValidationStatus = false;
	if (exp->leftOperand)
	{
		result = reinterpret_cast<CSintacticVisitorResults*>(exp->leftOperand->Accept(this));
		leftValidationStatus = result->isValidated();
		delete result;
	}
	
	if (exp->rightOperand)
	{
		result = reinterpret_cast<CSintacticVisitorResults*>(exp->rightOperand->Accept(this));
		rightValidationStatus = result->isValidated();
		delete result;
	}
	
	bool validationStatus = leftValidationStatus & rightValidationStatus;
	return new CSintacticVisitorResults(validationStatus);

}

IVisitorResult* CSintacticValidationVisitor::Visit(CNumExp *exp) {

	bool validationStatus = false;
	if (exp->value != "")
	{
		validationStatus = LSVUtils::checkDouble(exp->value);
	}
	return new CSintacticVisitorResults(validationStatus);
}

IVisitorResult* CSintacticValidationVisitor::Visit(CIdExp *exp) {

	bool validationStatus = false;
	if (exp->name != "") 
	{
		validationStatus = LSVUtils::checkIdName(exp->name);
	}
	return new CSintacticVisitorResults(validationStatus);
}