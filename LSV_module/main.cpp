#include "expression_tree/Expression.h"
#include "visitors/CSintacticValdationVisitor.h"
#include "visitors/CPrintVisitor.h"
#include "visitors/CExportMathMLVisitor.h"
#include "visitors/CExportOpenMathVisitor.h"
#include "parsers/MathMLParser.h"
#include <iostream>

int main()
{
	CMathMLParser parser;

	try {
		std::shared_ptr<IExpression> operationTree = parser.parseFromFile("format_files/expr.mathml");
		CSintacticValidationVisitor validationVisitor = CSintacticValidationVisitor();
		std::set<std::string> visibleIds = {"x", "y"};
		validationVisitor.setVisibleIds(visibleIds);
		CPrintVisitor printVisitor = CPrintVisitor();
		operationTree->Accept(printVisitor);
		operationTree->Accept(validationVisitor);
		std::cout << printVisitor.getDigraphDescription();
		std::cout << validationVisitor.getValidationStatus() << " " << validationVisitor.getError() << "\n";

		CExportOpenMathVisitor exportVisitor = CExportOpenMathVisitor();
		operationTree->Accept(exportVisitor);
		std::cout << exportVisitor.getOpenMathFile() << "\n";
	} catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}

	system("pause");

	return 0;
}