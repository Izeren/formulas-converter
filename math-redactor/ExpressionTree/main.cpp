#include "Expressions.h"
#include "CSintacticValdationVisitor.h"
#include "CSintacticVisitorResults.h"
#include <iostream>

int main()
{
	IExpression *operationTree = new COpExp(
		reinterpret_cast<IExpression *>(new CIdExp("a")),
		reinterpret_cast<IExpression *>(new CNumExp(".5"))
	);
	CSintacticValidationVisitor *validationVisitor = new CSintacticValidationVisitor();
	CSintacticVisitorResults *results = 
		reinterpret_cast<CSintacticVisitorResults *>(operationTree->Accept(validationVisitor));
	std::cout << results->isValidated() << "\n";
	
	return 0;
}