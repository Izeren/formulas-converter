#include "Converter.h"

std::string Converter::convert(std::string &expr, LSVUtils::TFormat from, LSVUtils::TFormat to) {



	std::shared_ptr<IParser> parser_from = get_parser[from];
	std::shared_ptr<IParser> parser_to = get_parser[to];


}