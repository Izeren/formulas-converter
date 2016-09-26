#pragma once

#include "Expression.h"
#include "LSVUtils.h"

class CMathMLParser {

public:

	std::string buildFromTree(IExpression *expr);

	IExpression *parse(std::string expr);

};

