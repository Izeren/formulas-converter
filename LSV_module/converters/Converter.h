#pragma once

#include "./../utils/LSVUtils.h"
#include "./../parsers/Parser.h"
#include "./../parsers/MathMLParser.h"
#include "./../parsers/OpenMathParser.h"
#include "./../parsers/TexParser.h"
#include <iostream>
#include <unordered_map>

class Converter {

	std::unordered_map<LSVUtils::TFormat, std::shared_ptr<IParser>> get_parser;

public:

	std::string convert(std::string &expr, LSVUtils::TFormat from, LSVUtils::TFormat to);

	Converter() :
		get_parser{
			{ LSVUtils::TEX, std::static_pointer_cast<IParser>(std::make_shared<CTexParser>()) },
			{ LSVUtils::MATHML, std::static_pointer_cast<IParser>(std::make_shared<CMathMLParser>()) },
			{ LSVUtils::OPENMATH, std::static_pointer_cast<IParser>(std::make_shared<COpenMathParser>()) }
	}{}

};