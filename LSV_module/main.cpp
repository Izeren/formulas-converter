#include "expression_tree/Expression.h"
#include "visitors/CSintacticValdationVisitor.h"
#include "parsers\MathMLParser.h"
#include <iostream>

int main()
{
	auto operationTree = std::make_shared<COpExp>(
		static_cast<std::shared_ptr<IExpression>>(std::make_shared<CIdExp>("a")),
		static_cast<std::shared_ptr<IExpression>>(std::make_shared<CNumExp>("0.5")),
		PLUS
	);
	CSintacticValidationVisitor temp = CSintacticValidationVisitor();
	IVisitor &visitor = temp;
	operationTree->Accept(visitor);
	std::cout << temp.getValidationStatus() << "\n";
	
	CMathMLParser parser;

	try {
		parser.parseFromFile("format_files/expr.mathml");
	} catch (std::exception &ex) {
		std::cout << ex.what();
	}

	system("pause");

	return 0;
}