#include "OpenMathParser.h"

//private:
void COpenMathParser::errorMessage(std::string &message) {

	throw std::logic_error("CTexParser: Formula contains the error:\n" + message);

}

void COpenMathParser::errorMessage(const char *message) {

	errorMessage(std::string(message));

}

//public
std::string COpenMathParser::buildFromTree(std::shared_ptr<IExpression> expr) {
	CExportTexVisitor exportVisitor = CExportTexVisitor();
	expr->Accept(exportVisitor);
	return exportVisitor.getFile();
}

std::shared_ptr<IExpression> COpenMathParser::parseFromFile(const char *path) {

	std::ifstream t(path);
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	size_t start = 0;
	cleanDelims(str);
	return parseExpr(str, start);
}