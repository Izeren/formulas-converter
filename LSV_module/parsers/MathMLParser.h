#pragma once

#include "./../expression_tree/Expression.h"

class CMathMLParser {

public:

	std::string buildFromTree(IExpression *expr);

	IExpression *parse(std::string expr);

};

