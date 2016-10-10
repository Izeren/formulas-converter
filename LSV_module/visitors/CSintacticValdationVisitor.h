#pragma once

#include "IVisitor.h"
#include "../expression_tree/Expression.h"
#include "../utils/LSVUtils.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <set>

class CSintacticValidationVisitor : public IVisitor {
public:

	CSintacticValidationVisitor();
	void ClearVisitor();

	void Visit(COpExp &exp) override;
	void Visit(CNumExp &exp) override;
	void Visit(CIdExp &exp) override;
	void Visit(CSumExp &exp) override;

	std::string getError() const;
	bool getValidationStatus() const;
	void setVisibleIds(const std::set<std::string> &visibleIds);
	void setError(const std::string &errorText);

private:
	std::set<std::string> visibleIds;
	std::string validationError;
	bool isValidated;
};