#pragma once

#include "IExportVisitor.h"
#include "../expression_tree/Expression.h"
#include "../utils/LSVUtils.h"
#include <stack>

class CExportMathMLVisitor : public IExportVisitor
{
public:

	CExportMathMLVisitor();
	void Visit(COpExp &exp) override;
	void Visit(CNumExp &exp) override;
	void Visit(CIdExp &exp) override;
	void Visit(CSumExp &exp) override;

	std::string getFile() const override;

private:

	void addAriphmeticOp(LSVUtils::TOperation operation, COpExp &exp) override;
	void addFracOperation(COpExp &exp) override;
	void addPowerOperation(COpExp &exp) override;
	void openTag(const std::string &tag);
	void closeTag(const std::string &tag);
	void CExportMathMLVisitor::addOperatorTag(const std::string &oper);
	void CExportMathMLVisitor::addIdTag(const std::string &name);
	void CExportMathMLVisitor::addNumberTag(double value);

};