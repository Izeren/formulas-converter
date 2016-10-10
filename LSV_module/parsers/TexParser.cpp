#include "TexParser.h"

//private:
void CTexParser::errorMessage(std::string &message) {

	throw std::logic_error("CTexParser: Formula contains the error:\n" + message);

}

void CTexParser::errorMessage(const char *message) {

	errorMessage(std::string(message));

}

std::string CTexParser::buildFromTree(std::shared_ptr<IExpression> expr) {
	CExportTexVisitor exportVisitor = CExportTexVisitor();
	expr->Accept(exportVisitor);
	return exportVisitor.getFile();
}

std::shared_ptr<IExpression> CTexParser::parseFromFile(const char *path) {

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

std::shared_ptr<IExpression> CTexParser::parse(const std::string &str) {

	size_t start = 0;
	std::string str_cpy(str);
	cleanDelims(str_cpy);
	return parseExpr(str_cpy, start);
}