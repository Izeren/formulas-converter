#include "TexParser.h"

//private:
void CTexParser::errorMessage(std::string &message) {

	throw std::logic_error("CTexParser: Formula contains the error:\n" + message);

}

void CTexParser::errorMessage(const char *message) {

	errorMessage(std::string(message));

}
