#pragma once

#include "Expression.h"
#include "LSVUtils.h"

class Converter {


public:

	std::string convert(std::string expr, Format from, Format to);

};