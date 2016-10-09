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
	if (isNotPrioritised) {
		this->description += "<mo>(</mo>\n";
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
		this->description += "<mo>)</mo>\n";
	}
	this->priorities.pop();
}

void CExportMathMLVisitor::Visit(CNumExp &exp)
{
	this->description += "<mn>" + std::to_string(exp.getValue()) + "</mn>\n";
}

void CExportMathMLVisitor::Visit(CIdExp &exp)
{
	this->description += "<mi>" + exp.getName() + "</mi>\n";
}

void CExportMathMLVisitor::Visit(CSumExp &exp)
{
	bool isNotPrioritised = this->priorities.top() > LSVUtils::TPriority::SUMMATION;
	this->priorities.push(LSVUtils::TPriority::SUMMATION);
	if (isNotPrioritised) {
		this->description += "<mo>(</mo>\n";
	}
	this->description += "<mrow>";
	this->description += "<munderover>";
	this->description += "<mo>&sum;</mo>\n";
	this->description += "<mrow><mi>" + exp.getIndexName() + "</mi>\n";
	this->description += "<mo>=</mo>\n";
	this->description += "<mn>" + std::to_string(exp.getStartId()) + "</mn>\n</mrow>\n";
	this->description += "<mn>" + std::to_string(exp.getFinishId()) + "</mn>\n";
	this->description += "</munderover>\n";
	exp.getExpression()->Accept(*this);
	this->description += "</mrow>\n";
	if (isNotPrioritised) {
		this->description += "<mo>)</mo>\n";
	}
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
	this->description += "<mo>" + COpExp::operationNames[operation] + "</mo>\n";
	exp.getSecondOperand()->Accept(*this);
}

void CExportMathMLVisitor::addFracOperation(COpExp &exp)
{
	this->description += "<mfrac>\n";
	exp.getFirstOperand()->Accept(*this);
	exp.getSecondOperand()->Accept(*this);
	this->description += "</mfrac>\n";
}

void CExportMathMLVisitor::addPowerOperation(COpExp &exp)
{
	this->description += "<msup>\n";
	exp.getFirstOperand()->Accept(*this);
	exp.getSecondOperand()->Accept(*this);
	this->description += "</msup>\n";
}