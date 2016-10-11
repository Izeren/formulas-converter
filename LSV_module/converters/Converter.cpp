#include "Converter.h"

std::string Converter::convert(const std::string &expr, LSVUtils::TFormat from, LSVUtils::TFormat to) {

	std::shared_ptr<IParser> parser_from = get_parser[from];
	std::shared_ptr<IParser> parser_to = get_parser[to];

	std::shared_ptr<IExpression> root = parser_from->parse(expr);
	return parser_to->buildFromTree(root);

}

std::string Converter::convert(const char *path, LSVUtils::TFormat from, LSVUtils::TFormat to)
{
	std::shared_ptr<IParser> parser_from = get_parser[from];
	std::shared_ptr<IParser> parser_to = get_parser[to];

	std::shared_ptr<IExpression> root = parser_from->parseFromFile(path);
	return parser_to->buildFromTree(root);
}
