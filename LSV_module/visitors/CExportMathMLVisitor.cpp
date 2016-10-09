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
	this->description += "<mrow>\n";
	exp.getFirstOperand()->Accept(*this);
	switch (exp.getOperation()) 
	{
		case FRAC: 
		{
			this->addFracOperation(exp);
		}
		case POWER:
		{
			this->addPowerOperation(exp);
		}
		default:
		{
			this->addAriphmeticOp(exp.getOperation());
		}
	}
	this->description += "</mrow>\n";
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

void CExportMathMLVisitor::addNode(const std::string &name, const std::string &label)
{
	this->description += "\t" + name + "[label=\"" + label + "\"];\n";
}
void CExportMathMLVisitor::addEdge(const std::string &from, const std::string &to)
{
	this->description += "\t" + from + " -> " + to + ";\n";
}

void CExportMathMLVisitor::addEdge(int from, int to)
{
	addEdge(std::to_string(from), std::to_string(to));
}

void CExportMathMLVisitor::addAriphmeticOp(TOperation operation)
{
	this->description += "<mo>" + COpExp::operationNames[operation] + "</mo>\n";
}

void CExportMathMLVisitor::addFracOperation(COpExp &exp)
{
	this->description += "<mfrac>\n";
	exp.getFirstOperand()->Accept(*this);
	exp.getFirstOperand()->Accept(*this);
	this->description += "</mfrac>\n";
}

void CExportMathMLVisitor::addPowerOperation(COpExp &exp)
{
	this->description += "<msup>\n";
	exp.getFirstOperand()->Accept(*this);
	exp.getFirstOperand()->Accept(*this);
	this->description += "</msup>\n";
}