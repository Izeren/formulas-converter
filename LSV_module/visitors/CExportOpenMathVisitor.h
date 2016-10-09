#pragma once

#include "IVisitor.h"
#include "../expression_tree/Expression.h"
#include "../utils/LSVUtils.h"

class CExportOpenMathVisitor : public IVisitor
{
public:

	CExportOpenMathVisitor();
	void ClearVisitor();
	void Visit(COpExp &exp) override;
	void Visit(CNumExp &exp) override;
	void Visit(CIdExp &exp) override;
	void Visit(CSumExp &exp) override;

	std::string getOpenMathFile() const;

private:
	std::string description;
	int lastVisited;
	void addAriphmeticOp(TOperation operation, COpExp &exp);
	void addFracOperation(COpExp &exp);
	void addPowerOperation(COpExp &exp);

};