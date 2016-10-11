#include "OpenMathParser.h"

//private:
void COpenMathParser::errorMessage(std::string &message) {

	throw std::logic_error("COpenMathParser: Formula contains the error:\n" + message);

}

void COpenMathParser::errorMessage(const char *message) {

	errorMessage(std::string(message));

}

//public
std::string COpenMathParser::buildFromTree(std::shared_ptr<IExpression> expr) {
	CExportOpenMathVisitor exportVisitor = CExportOpenMathVisitor();
	expr->Accept(exportVisitor);
	return exportVisitor.getFile();
}

std::shared_ptr<IExpression> COpenMathParser::parseFromFile(const char *path) {

	std::ifstream t(path);
	std::string str;
	std::string temp;
	while (std::getline(t, temp)) {
		str += temp;
	}
	t.close();
	
	size_t start = 0;
	cleanDelims(str);
	return parseExpr(str, start);
}

std::shared_ptr<IExpression> COpenMathParser::parse(const std::string &str) {

	size_t start = 0;
	std::string str_cpy(str);
	cleanDelims(str_cpy);
	return parseExpr(str_cpy, start);
}