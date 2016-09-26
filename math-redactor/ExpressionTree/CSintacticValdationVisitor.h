#pragma once

#include "IVisitor.h"
#include "IVisitorResult.h"
#include "Expression.h"
#include <string>
#include <set>

class CSintacticValidationVisitor : public IVisitor {
public:

	CSintacticValidationVisitor();
	void ClearVisitor();

	IVisitorResult* Visit(COpExp *exp) override;
	IVisitorResult* Visit(CNumExp *exp) override;
	IVisitorResult* Visit(CIdExp *exp) override;

private:
	std::set<std::string> visibleIds;
};