#pragma once

#include "./../ExpressionTree/Expression.h"
#include "./../ExpressionTree/LSVUtils.h"

class CMathMLParser {

public:

	std::string buildFromTree(IExpression *expr);

	IExpression *parse(std::string expr);

};

