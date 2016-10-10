#pragma once
#include "./../expression_tree/Expression.h"

class IParser {

public:

	virtual std::string buildFromTree(std::shared_ptr<IExpression> expr);

	virtual std::shared_ptr<IExpression> parseFromFile(const char *path);
	
	virtual std::shared_ptr<IExpression> parse(const std::string &str);

};