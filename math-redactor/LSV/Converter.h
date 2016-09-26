#pragma once

#include "./../ExpressionTree/Expression.h"
#include "./../ExpressionTree/LSVUtils.h"

class Converter {


public:

	std::string convert(std::string expr, Format from, Format to);

};