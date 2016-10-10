#include "expression_tree/Expression.h"
#include "visitors/CSintacticValdationVisitor.h"
#include "visitors/CPrintVisitor.h"
#include "visitors/CExportMathMLVisitor.h"
#include "visitors/CExportOpenMathVisitor.h"
#include "visitors/CExportTexVisitor.h"
#include "parsers/MathMLParser.h"
#include <iostream>

int main()
{
	CMathMLParser parser;

	try {
		std::shared_ptr<IExpression> operationTree = parser.parseFromFile("format_files/right_complex_expr.tex");
		CSintacticValidationVisitor validationVisitor = CSintacticValidationVisitor();
		std::set<std::string> visibleIds = {"x", "y"};
		validationVisitor.setVisibleIds(visibleIds);
		CPrintVisitor printVisitor = CPrintVisitor();
		operationTree->Accept(printVisitor);
		operationTree->Accept(validationVisitor);
		std::cout << printVisitor.getDigraphDescription();
		std::cout << validationVisitor.getValidationStatus() << " " << validationVisitor.getError() << "\n";
		CExportMathMLVisitor exportVisitor = CExportMathMLVisitor();
		operationTree->Accept(exportVisitor);
		std::cout << exportVisitor.getFile() << "\n";
	} catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}

	system("pause");

	return 0;
}