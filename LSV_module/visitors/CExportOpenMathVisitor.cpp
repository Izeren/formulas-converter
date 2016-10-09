#include "CExportOpenMathVisitor.h"

CExportOpenMathVisitor::CExportOpenMathVisitor()
{
	this->ClearVisitor();
}

void CExportOpenMathVisitor::Visit(COpExp &exp)
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

void CExportOpenMathVisitor::Visit(CNumExp &exp)
{
	this->description += "{" + std::to_string(exp.getValue()) + "}";
}

void CExportOpenMathVisitor::Visit(CIdExp &exp)
{
	this->description += "{" + exp.getName() + "}";
}

void CExportOpenMathVisitor::Visit(CSumExp &exp)
{
	bool isNotPrioritised = this->priorities.top() > LSVUtils::TPriority::SUMMATION;
	this->priorities.push(LSVUtils::TPriority::SUMMATION);
	
	this->description += "{";
	if (isNotPrioritised) {
		this->description += "(";
	}
	this->description += "sum from{";
	this->description += exp.getIndexName() + "=" + std::to_string(exp.getStartId()) + "} to{";
	this->description += std::to_string(exp.getFinishId()) + "} ";
	exp.getExpression()->Accept(*this);
	if (isNotPrioritised) {
		this->description += ")";
	}
	this->description += "}";
	this->priorities.pop();
}

std::string CExportOpenMathVisitor::getFile() const
{
	return 	this->description;
}

void CExportOpenMathVisitor::addAriphmeticOp(LSVUtils::TOperation operation, COpExp &exp)
{
	exp.getFirstOperand()->Accept(*this);
	this->description += COpExp::operationNames[operation];
	exp.getSecondOperand()->Accept(*this);
}

void CExportOpenMathVisitor::addFracOperation(COpExp &exp)
{
	exp.getFirstOperand()->Accept(*this);
	this->description += " over ";
	exp.getSecondOperand()->Accept(*this);
}

void CExportOpenMathVisitor::addPowerOperation(COpExp &exp)
{
	exp.getFirstOperand()->Accept(*this);
	this->description += " ^ ";
	exp.getSecondOperand()->Accept(*this);
}