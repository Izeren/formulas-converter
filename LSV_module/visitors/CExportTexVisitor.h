#pragma once

#include "IExportVisitor.h"
#include "../expression_tree/Expression.h"
#include "../utils/LSVUtils.h"

class CExportTexVisitor : public IExportVisitor
{
public:

	CExportTexVisitor();
	void Visit(COpExp &exp) override;
	void Visit(CNumExp &exp) override;
	void Visit(CIdExp &exp) override;
	void Visit(CSumExp &exp) override;

	std::string getFile() const override;

private:
	std::string description;
	std::stack<LSVUtils::TPriority> priorities;
	int lastVisited;

	void addAriphmeticOp(LSVUtils::TOperation operation, COpExp &exp) override;
	void addFracOperation(COpExp &exp) override;
	void addPowerOperation(COpExp &exp) override;

};