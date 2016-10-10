#include "CPrintVisitor.h"

CPrintVisitor::CPrintVisitor()
{
	this->ClearVisitor();
}

void CPrintVisitor::ClearVisitor()
{
	this->description = "";
	this->lastVisited = 1;
}


void CPrintVisitor::Visit(COpExp &exp)
{
	auto firstOperand = exp.getFirstOperand();
	int leftId = IExpression::MISSED_NODE, rightId = IExpression::MISSED_NODE;
	if (firstOperand != NULL) {
		firstOperand->Accept(*this);
		leftId = this->lastVisited;
	}
	auto secondOperand = exp.getSecondOperand();
	if (secondOperand != NULL) {
		secondOperand->Accept(*this);
		rightId = this->lastVisited;
	}
	int current = 1 + this->lastVisited++;
	this->addEdge(current, leftId);
	this->addEdge(current, rightId);
	this->addNode(std::to_string(current), std::string("Operation: ") + exp.getStringOperation());

}

void CPrintVisitor::Visit(CNumExp &exp) 
{
	this->addNode(std::to_string(++(this->lastVisited)), "Num: " + std::to_string(exp.getValue()));
}

void CPrintVisitor::Visit(CIdExp &exp)
{
	this->addNode(std::to_string(++(this->lastVisited)), "IdName: " + exp.getName());
}

void CPrintVisitor::Visit(CSumExp &exp)
{
	auto expression = exp.getExpression();
	int expressionId = IExpression::MISSED_NODE;
	if (expression != NULL) {
		expression->Accept(*this);
		expressionId = this->lastVisited;
	}
	this->addNode(
		std::to_string(++this->lastVisited),
		std::string("Summation by: ") +
		exp.getIndexName() +
		"\nfrom: " + std::to_string(exp.getStartId()) +
		"\nto: " + std::to_string(exp.getFinishId())
		);
	this->addEdge(this->lastVisited, expressionId);
}

std::string CPrintVisitor::getDigraphDescription() const
{
	return "Digraph G{\ngraph[ordering=\"out\"];\n" + this->description + "}\n";
}

void CPrintVisitor::addNode(const std::string &name, const std::string &label)
{
	this->description += "\t" + name + "[label=\"" + label + "\"];\n";
}
void CPrintVisitor::addEdge(const std::string &from, const std::string &to)
{
	this->description += "\t" + from + " -> " + to + ";\n";
}

void CPrintVisitor::addEdge(int from, int to)
{
	addEdge(std::to_string(from), std::to_string(to));
}