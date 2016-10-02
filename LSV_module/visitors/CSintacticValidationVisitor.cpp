#include "CSintacticValdationVisitor.h"

void CSintacticValidationVisitor::ClearVisitor()
{
	this->visibleIds.clear();
}


void CSintacticValidationVisitor::Visit(COpExp &exp)
{
	if (exp.getFirstOperand())
	{
		exp.getFirstOperand()->Accept(*this);
		if (!this->isValidated) {
			return;
		}
	}
	else 
	{
		return this->setError("Missing first operand\n");
	}
	
	if (exp.getSecondOperand())
	{
		exp.getSecondOperand()->Accept(*this);
	}
	else
	{
		return this->setError("Missing second operand\n");
	}
	
}

void CSintacticValidationVisitor::Visit(CNumExp &exp) {}

void CSintacticValidationVisitor::Visit(CIdExp &exp)
{
	std::string &name = exp.getName();
	if (name == BAD_ID) {
		return this->setError("Wrong or empty variable name\n");
	}
	if (visibleIds.find(name) == visibleIds.end())
	{
		return this->setError(std::string("The unxpected variable: ") + exp.getName() + "\n");
	}
}

void CSintacticValidationVisitor::Visit(CSumExp &exp) {

	if (exp.getExpression() == NULL) {
		return this->setError("Missed expression for sum\n");
	}
	if (exp.getIndexName() == BAD_ID)
	{
		return this->setError("Wrong or empty index name for sum\n");
	}
	if (visibleSumIndices.find(exp.getIndexName()) != visibleSumIndices.end()) {
		return this->setError("Index name of sum is already used by upper sum\n");
	}

	this->visibleSumIndices.insert(exp.getIndexName());
	exp.getExpression()->Accept(*this);
	this->visibleSumIndices.erase(exp.getIndexName());
}

bool CSintacticValidationVisitor::getValidationStatus() const
{
	return this->isValidated;
}

void CSintacticValidationVisitor::setVisibleIds(const std::set<std::string> &visibleIds)
{
	this->visibleIds = visibleIds;
}

void CSintacticValidationVisitor::setError(const std::string &errorText)
{
	this->isValidated = false;
	this->validationError = errorText;
}