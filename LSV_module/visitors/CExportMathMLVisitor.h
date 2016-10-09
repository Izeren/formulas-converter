#pragma once

#include "IVisitor.h"
#include "../expression_tree/Expression.h"
#include "../utils/LSVUtils.h"

class CExportMathMLVisitor : public IVisitor
{
public:

	CExportMathMLVisitor();
	void ClearVisitor();
	void Visit(COpExp &exp) override;
	void Visit(CNumExp &exp) override;
	void Visit(CIdExp &exp) override;
	void Visit(CSumExp &exp) override;

	std::string getMathMLFile() const;

private:
	std::string description;
	int lastVisited;
	void addAriphmeticOp(LSVUtils::TOperation operation, COpExp &exp);
	void addFracOperation(COpExp &exp);
	void addPowerOperation(COpExp &exp);

};