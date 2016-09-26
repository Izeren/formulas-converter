#pragma once

#include "IVisitor.h"
#include "../visitor_results/CSintacticVisitorResults.h"
#include "../expression_tree/Expressions.h"
#include "../utils/LSVUtils.h"

#include <iostream>
#include <algorithm>
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