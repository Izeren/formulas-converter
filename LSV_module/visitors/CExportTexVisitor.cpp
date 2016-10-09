#include "CExportTexVisitor.h"

CExportTexVisitor::CExportTexVisitor()
{
	this->ClearVisitor();
}

void CExportTexVisitor::Visit(COpExp &exp)
{
	LSVUtils::TOperation operation = exp.getOperation();
	LSVUtils::TPriority priority = LSVUtils::utilsSettings::operationPriorities[operation];
	bool isNotPrioritised = this->priorities.top() > priority;
	this->priorities.push(priority);
	this->description += "{";
	if (isNotPrioritised) {
		this->description += "(";
	}
	switch (operation)
	{
		case LSVUtils::TOperation::FRAC:
		{
			this->addFracOperation(exp);
			break;
		}
		case LSVUtils::TOperation::POWER:
		{
			this->addPowerOperation(exp);
			break;
		}
		default:
		{
			this->addAriphmeticOp(exp.getOperation(), exp);
			break;
		}
	}
	if (isNotPrioritised) {
		this->description += ")";
	}
	this->description += "}";
	this->priorities.pop();
}

void CExportTexVisitor::Visit(CNumExp &exp)
{
	this->description += "{" + std::to_string(exp.getValue()) + "}";
}

void CExportTexVisitor::Visit(CIdExp &exp)
{
	this->description += "{" + exp.getName() + "}";
}

void CExportTexVisitor::Visit(CSumExp &exp)
{
	bool isNotPrioritised = this->priorities.top() > LSVUtils::TPriority::SUMMATION;
	this->priorities.push(LSVUtils::TPriority::SUMMATION);

	this->description += "{";
	if (isNotPrioritised) {
		this->description += "(";
	}
	this->description += "\\sum_{";
	this->description += exp.getIndexName() + "=" + std::to_string(exp.getStartId()) + "}^{";
	this->description += std::to_string(exp.getFinishId()) + "} ";
	exp.getExpression()->Accept(*this);
	if (isNotPrioritised) {
		this->description += ")";
	}
	this->description += "}";
	this->priorities.pop();
}

std::string CExportTexVisitor::getFile() const
{
	return this->description;
}

void CExportTexVisitor::addAriphmeticOp(LSVUtils::TOperation operation, COpExp &exp)
{
	exp.getFirstOperand()->Accept(*this);
	this->description += COpExp::operationNames[operation];
	exp.getSecondOperand()->Accept(*this);
}

void CExportTexVisitor::addFracOperation(COpExp &exp)
{
	this->description += "\\frac";
	exp.getFirstOperand()->Accept(*this);
	exp.getSecondOperand()->Accept(*this);
}

void CExportTexVisitor::addPowerOperation(COpExp &exp)
{
	exp.getFirstOperand()->Accept(*this);
	this->description += "^";
	exp.getSecondOperand()->Accept(*this);
}