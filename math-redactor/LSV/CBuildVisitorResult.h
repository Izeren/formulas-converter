#pragma once

#include "./../ExpressionTree/IVisitorResult.h"
#include <string>

class CBuildVisitorResult : public IVisitorResult {

private:

	std::string result;

public:

	CBuildVisitorResult(std::string result) : result(result) {}

	std::string getResult() { return result; }

};