#include "CExportMathMLVisitor.h"

CExportMathMLVisitor::CExportMathMLVisitor()
{
	this->ClearVisitor();
}

void CExportMathMLVisitor::Visit(COpExp &exp)
{
	LSVUtils::TOperation operation = exp.getOperation();
	LSVUtils::TPriority priority = LSVUtils::utilsSettings::operationPriorities[operation];
	bool isNotPrioritised = this->priorities.top() > priority;
	this->priorities.push(priority);
	this->openTag("mrow");
	if (isNotPrioritised) {
		this->addOperatorTag("(");
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
		this->addOperatorTag(")");
	}
	this->closeTag("mrow");
	this->priorities.pop();
}

void CExportMathMLVisitor::Visit(CNumExp &exp)
{
	this->addNumberTag(exp.getValue());
}

void CExportMathMLVisitor::Visit(CIdExp &exp)
{
	this->addIdTag(exp.getName());
}

void CExportMathMLVisitor::Visit(CSumExp &exp)
{
	this->priorities.push(LSVUtils::TPriority::SUMMATION);
	this->openTag("mrow");
	this->openTag("munderover");
	this->addOperatorTag("&sum;");
	this->openTag("mrow");
	this->addIdTag(exp.getIndexName());
	this->addOperatorTag("=");
	this->addNumberTag(exp.getStartId());
	this->closeTag("mrow");
	this->addNumberTag(exp.getFinishId());
	this->closeTag("munderover");
	exp.getExpression()->Accept(*this);
	this->closeTag("mrow");

	this->priorities.pop();
}

std::string CExportMathMLVisitor::getFile() const
{
	return "<math xmlns = 'http://www.w3.org/1998/Math/MathML' display = 'block'>" +
		this->description + "</math>\n";
}

void CExportMathMLVisitor::addAriphmeticOp(LSVUtils::TOperation operation, COpExp &exp)
{
	exp.getFirstOperand()->Accept(*this);
	this->addOperatorTag(COpExp::operationNames[operation]);
	exp.getSecondOperand()->Accept(*this);
}

void CExportMathMLVisitor::addFracOperation(COpExp &exp)
{
	this->openTag("mfrac");
	exp.getFirstOperand()->Accept(*this);
	exp.getSecondOperand()->Accept(*this);
	this->closeTag("mfrac");
}

void CExportMathMLVisitor::addPowerOperation(COpExp &exp)
{
	this->openTag("msup");
	exp.getFirstOperand()->Accept(*this);
	exp.getSecondOperand()->Accept(*this);
	this->closeTag("msup");
}

void CExportMathMLVisitor::openTag(const std::string &tag)
{
	this->description += std::string(this->depth, '\t') + "<" + tag + ">\n";
	this->depth += 1;
}

void CExportMathMLVisitor::closeTag(const std::string &tag)
{
	this->depth -= 1;
	this->description += std::string(this->depth, '\t') + "</" + tag + ">\n";
}

void CExportMathMLVisitor::addOperatorTag(const std::string &oper)
{
	this->description += std::string(this->depth, '\t') + "<mo>" + oper + "</mo>\n";
}

void CExportMathMLVisitor::addNumberTag(double value)
{
	this->description += std::string(this->depth, '\t') + "<mn>" + std::to_string(value) + "</mn>\n";
}

void CExportMathMLVisitor::addIdTag(const std::string &name)
{
	this->description += std::string(this->depth, '\t') + "<mi>" + name + "</mi>\n";
}