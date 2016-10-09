#include "CExportTexVisitor.h"

CExportTexVisitor::CExportTexVisitor()
{
	this->ClearVisitor();
}

void CExportTexVisitor::ClearVisitor()
{
	this->description = "";
}

void CExportTexVisitor::Visit(COpExp &exp)
{
	this->description += "{(";
	switch (exp.getOperation())
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
	this->description += ")}";
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
	this->description += "{(\sum_{";
	this->description += exp.getIndexName() + "=" + std::to_string(exp.getStartId()) + "}^{";
	this->description += std::to_string(exp.getFinishId()) + "} ";
	exp.getExpression()->Accept(*this);
	this->description += ")}";
}

std::string CExportTexVisitor::getTexFile() const
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
	this->description += "\frac";
	exp.getFirstOperand()->Accept(*this);
	exp.getSecondOperand()->Accept(*this);
}

void CExportTexVisitor::addPowerOperation(COpExp &exp)
{
	exp.getFirstOperand()->Accept(*this);
	this->description += "^";
	exp.getSecondOperand()->Accept(*this);
}