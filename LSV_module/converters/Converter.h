#pragma once

#include "./../utils/LSVUtils.h"
#include "./../parsers/Parser.h"
#include "./../parsers/MathMLParser.h"
#include "./../parsers/OpenMathParser.h"
#include "./../parsers/TexParser.h"
#include <iostream>

class Converter {

	std::unordered_map<LSVUtils::TFormat, std::shared_ptr<IParser>> get_parser;

public:

	std::string convert(const std::string &expr, LSVUtils::TFormat from, LSVUtils::TFormat to);

	Converter() {
		get_parser[LSVUtils::TEX] = std::static_pointer_cast<IParser>(std::make_shared<CTexParser>());
		get_parser[LSVUtils::MATHML] = std::static_pointer_cast<IParser>(std::make_shared<CMathMLParser>());
		get_parser[LSVUtils::OPENMATH] = std::static_pointer_cast<IParser>(std::make_shared<COpenMathParser>());
	}

	std::string convert(const char *path, LSVUtils::TFormat from, LSVUtils::TFormat to);

};