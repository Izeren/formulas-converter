#pragma once

#include "IVisitor.h"
#include "../expression_tree/Expression.h"
#include "../utils/LSVUtils.h"
#include <stack>

class IExportVisitor : public IVisitor
{
public:

	void ClearVisitor();

	virtual std::string getFile() const = 0;

protected:
	std::string description;
	std::stack<LSVUtils::TPriority> priorities;
	int lastVisited;

private:
	virtual void addAriphmeticOp(LSVUtils::TOperation operation, COpExp &exp) = 0;
	virtual void addFracOperation(COpExp &exp) = 0;
	virtual void addPowerOperation(COpExp &exp) = 0;

};