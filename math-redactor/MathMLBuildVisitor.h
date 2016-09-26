#pragma once

#include "Expression.h"


class MathMLBuildVisitor : public IVisitor {

public:

	IVisitorResult* Visit(COpExp *exp) override;
	IVisitorResult* Visit(CNumExp *exp) override;
	IVisitorResult* Visit(CIdExp *exp) override;

};