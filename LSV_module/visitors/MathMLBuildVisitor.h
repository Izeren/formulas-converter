#pragma once

#include "IVisitor.h"
#include "../expression_tree/Expressions.h"


class MathMLBuildVisitor : public IVisitor {

public:

	void Visit(COpExp &exp) override;
	void Visit(CNumExp &exp) override;
	void Visit(CIdExp &exp) override;
	void Visit(CSumExp &exp) override;

};