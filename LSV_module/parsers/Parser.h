#pragma once
#include "./../expression_tree/Expression.h"

class IParser {

public:

	virtual std::string buildFromTree(std::shared_ptr<IExpression> expr) = 0;

	virtual std::shared_ptr<IExpression> parseFromFile(const char *path) = 0;
	
	virtual std::shared_ptr<IExpression> parse(const std::string &str) = 0;

};