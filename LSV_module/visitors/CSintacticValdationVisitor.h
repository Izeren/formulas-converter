#pragma once

#include "IVisitor.h"
#include "../expression_tree/Expressions.h"
#include "../utils/LSVUtils.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <set>

class CSintacticValidationVisitor : public IVisitor {
public:

	void ClearVisitor();

	void Visit(COpExp &exp) override;
	void Visit(CNumExp &exp) override;
	void Visit(CIdExp &exp) override;
	void Visit(CSumExp &exp) override;

	bool getValidationStatus() const;

private:
	std::set<std::string> visibleIds;
	bool isValidated;
};