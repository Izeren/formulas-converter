#pragma once

#include "IVisitor.h"
#include "../expression_tree/Expressions.h"


class MathMLBuildVisitor : public IVisitor {

public:

	IVisitorResult* Visit(COpExp *exp) override;
	IVisitorResult* Visit(CNumExp *exp) override;
	IVisitorResult* Visit(CIdExp *exp) override;
	IVisitorResult* Visit(CSumExp *exp) override;

};