#include "expression_tree/Expression.h"
#include "visitors/CSintacticValdationVisitor.h"
#include "visitor_results/CSintacticVisitorResults.h"
#include <iostream>

int main()
{
	IExpression *operationTree = new COpExp(
		reinterpret_cast<IExpression *>(new CIdExp("a")),
		reinterpret_cast<IExpression *>(new CNumExp("0.5"))
	);
	CSintacticValidationVisitor *validationVisitor = new CSintacticValidationVisitor();
	CSintacticVisitorResults *results = 
		reinterpret_cast<CSintacticVisitorResults *>(operationTree->Accept(validationVisitor));
	std::cout << results->isValidated() << "\n";
	delete operationTree;
	delete results;
	
	return 0;
}