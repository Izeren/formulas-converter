#pragma once

#include "IVisitor.h"
#include "IVisitorResult.h"
#include "Expressions.h"
#include <string>
#include <set>
class CSintacticValidationVisitor : public IVisitor {
public:

	CSintacticValidationVisitor();
	void ClearVisitor();

	IVisitorResult* Visit(COpExp *exp) override;
	IVisitorResult* Visit(CNumExp *exp) override;
	IVisitorResult* Visit(CIdExp *exp) override;
	IVisitorResult* Visit(CSumExp *exp) override;

private:
	std::set<std::string> visibleIds;
};