#include "CSintacticValdationVisitor.h"

CSintacticValidationVisitor::CSintacticValidationVisitor() {}

void CSintacticValidationVisitor::ClearVisitor()
{
	visibleIds.clear();
}


IVisitorResult* CSintacticValidationVisitor::Visit(COpExp *exp)
{

	CSintacticVisitorResults* result;
	bool leftValidationStatus = false, rightValidationStatus = false;
	if (exp->getFirstOperand())
	{
		result = reinterpret_cast<CSintacticVisitorResults*>(exp->getFirstOperand()->Accept(this));
		leftValidationStatus = result->isValidated();
		delete result;
	}
	
	if (exp->getSecondOperand())
	{
		result = reinterpret_cast<CSintacticVisitorResults*>(exp->getSecondOperand()->Accept(this));
		rightValidationStatus = result->isValidated();
		delete result;
	}
	
	bool validationStatus = leftValidationStatus & rightValidationStatus;
	return new CSintacticVisitorResults(validationStatus);

}

IVisitorResult* CSintacticValidationVisitor::Visit(CNumExp *exp) {
	bool validationStatus = true;
	return new CSintacticVisitorResults(validationStatus);
}

IVisitorResult* CSintacticValidationVisitor::Visit(CIdExp *exp)
{

	bool validationStatus = exp->getName() != "error_name";
	return new CSintacticVisitorResults(validationStatus);
}

IVisitorResult* CSintacticValidationVisitor::Visit(CSumExp *exp) {

	bool validationStatus = true;
	if (exp->getIndexName() == "error_name" || exp->getExpression() == 0)
	{
		validationStatus = false;
	}
	return new CSintacticVisitorResults(validationStatus);
}
