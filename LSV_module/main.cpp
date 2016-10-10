#include "expression_tree/Expression.h"
#include "visitors/CSintacticValdationVisitor.h"
#include "visitors/CPrintVisitor.h"
#include "visitors/CExportMathMLVisitor.h"
#include "visitors/CExportOpenMathVisitor.h"
#include "visitors/CExportTexVisitor.h"
#include "parsers/MathMLParser.h"
#include "parsers/TexParser.h"
#include <fstream>
#include <iostream>

int main()
{
	CTexParser parser;

	std::ofstream out("graph");

	try {
		std::shared_ptr<IExpression> operationTree = parser.parseFromFile("format_files/expr.tex");
		CPrintVisitor printVisitor = CPrintVisitor();
		operationTree->Accept(printVisitor);
		out << printVisitor.getDigraphDescription();
	} catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}

	system("pause");

	return 0;
}