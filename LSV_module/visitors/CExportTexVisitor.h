#pragma once

#include "IVisitor.h"
#include "../expression_tree/Expression.h"
#include "../utils/LSVUtils.h"

class CExportTexVisitor : public IVisitor
{
public:

	CExportTexVisitor();
	void ClearVisitor();
	void Visit(COpExp &exp) override;
	void Visit(CNumExp &exp) override;
	void Visit(CIdExp &exp) override;
	void Visit(CSumExp &exp) override;

	std::string getTexFile() const;

private:
	std::string description;
	int lastVisited;
	void addAriphmeticOp(TOperation operation, COpExp &exp);
	void addFracOperation(COpExp &exp);
	void addPowerOperation(COpExp &exp);

};