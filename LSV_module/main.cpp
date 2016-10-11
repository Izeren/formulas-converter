#include "expression_tree/Expression.h"
#include "visitors/CSintacticValdationVisitor.h"
#include "visitors/CPrintVisitor.h"
#include "parsers/TexParser.h"
#include "parsers/OpenMathParser.h"
#include "converters/Converter.h"
#include <fstream>
#include <iostream>

int main()
{
	COpenMathParser parser;
	Converter converter;
	std::ofstream out("graph");

	try {

		//std::cout << converter.convert("format_files/right_complex_expr.openmath", LSVUtils::TFormat::TEX, LSVUtils::TFormat::OPENMATH) << "\n";
		std::shared_ptr<IExpression> operationTree = parser.parseFromFile("format_files/right_complex_expr.openmath");

		CPrintVisitor graph = CPrintVisitor();
		operationTree->Accept(graph);
		out << graph.getDigraphDescription();

	} catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}

	system("pause");

	return 0;
}