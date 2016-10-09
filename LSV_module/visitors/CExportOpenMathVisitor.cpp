#include "CExportOpenMathVisitor.h"

CExportOpenMathVisitor::CExportOpenMathVisitor()
{
	this->ClearVisitor();
}

void CExportOpenMathVisitor::ClearVisitor()
{
	this->description = "";
}


void CExportOpenMathVisitor::Visit(COpExp &exp)
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
	this->description += "{(sum from{";
	this->description += exp.getIndexName() + "=" + std::to_string(exp.getStartId()) + "} to{";
	this->description += std::to_string(exp.getFinishId()) + "} ";
	exp.getExpression()->Accept(*this);
	this->description += ")}";
}

std::string CExportOpenMathVisitor::getOpenMathFile() const
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