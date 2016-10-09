#include "CExportMathMLVisitor.h"

CExportMathMLVisitor::CExportMathMLVisitor()
{
	this->ClearVisitor();
}

void CExportMathMLVisitor::ClearVisitor()
{
	this->description = "";
}


void CExportMathMLVisitor::Visit(COpExp &exp)
{
	this->description += "<mrow><mo>(</mo>\n";
	switch (exp.getOperation()) 
	{
		case FRAC: 
		{
			this->addFracOperation(exp);
			break;
		}
		case POWER:
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
	this->description += "<mo>)</mo></mrow>\n";
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
	this->description += "<mrow>\n";
	this->description += "<munderover>\n";
	this->description += "<mo>&sum;</mo>";
	this->description += "<mrow><mi>" + exp.getIndexName() + "</mi>";
	this->description += "<mo>=</mo>";
	this->description += "<mn>" + std::to_string(exp.getStartId()) + "</mn></mrow>\n";
	this->description += "<mn>" + std::to_string(exp.getFinishId()) + "</mn>\n";
	this->description += "</munderover>\n";
	exp.getExpression()->Accept(*this);
	this->description += "</mrow>\n";
}

std::string CExportMathMLVisitor::getMathMLFile() const
{
	return "<math xmlns = 'http://www.w3.org/1998/Math/MathML' display = 'block'>" +
		this->description + "</math>\n";
}

void CExportMathMLVisitor::addAriphmeticOp(TOperation operation, COpExp &exp)
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