#include "MathMLParser.h"

//private:
bool CMathMLParser::nameEqual(pugi::xml_node &node, const char *name) {
	return !strcmp(node.name(), name);
}

void CMathMLParser::errorMessage(std::string &message) {

	throw std::logic_error("CMathMLParser: Formula contains the error:\n" + message);

}

void CMathMLParser::errorMessage(const char *message) {

	errorMessage(std::string(message));

}


//public:
std::string CMathMLParser::buildFromTree(std::shared_ptr<IExpression> expr) {
	CExportMathMLVisitor exportVisitor = CExportMathMLVisitor();
	expr->Accept(exportVisitor);
	return exportVisitor.getFile();
}

std::shared_ptr<IExpression> CMathMLParser::parseFromFile(const char *path) {

	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(path);

	if (result) {

		pugi::xml_node root = doc.first_child();
		if (std::string(root.name()) != "math") {
			errorMessage("Root element must have name \"math\"");
		}

		return parseExpr(root);
	} else {

		std::ostringstream sstream;

		sstream << "CMathMLParser: parsed with errors.\n";
		sstream << "Error description: " << result.description() << "\n";
		
		std::ifstream strm;
		strm.open(path);
		strm.seekg(result.offset);

		std::string error_line;
		strm >> error_line;
		strm.close();

		sstream << "Error offset: " << result.offset << " (error at [..." << error_line << "]\n\n";

		throw std::runtime_error(sstream.str());
	}
	


	return nullptr;
}