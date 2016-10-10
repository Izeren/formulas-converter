#include "LSVUtils.h"
#include <iostream>

/*
	Эта функция проверяет валидность имени переменной
*/

bool LSVUtils::checkIdName(std::string name)
{
	std::regex idExpression("[_a-zA-Z][_0-9a-zA-Z]*");
	return std::regex_match(name, idExpression);
}

/*
	Эта функция проверяет, валидность написания вещественного числа
*/

bool LSVUtils::checkDouble(std::string value)
{
	std::regex floatNumber("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?");
	return std::regex_match(value, floatNumber);
}


std::unordered_map<LSVUtils::TOperation, LSVUtils::TPriority> LSVUtils::utilsSettings::operationPriorities = {
	{ LSVUtils::TOperation::PLUS, LSVUtils::TPriority::ADDITION_SUBSTRACTION },
	{ LSVUtils::TOperation::MINUS, LSVUtils::TPriority::ADDITION_SUBSTRACTION },
	{ LSVUtils::TOperation::MULTIPLY, LSVUtils::TPriority::MULTIPLICATION_DIVISION },
	{ LSVUtils::TOperation::DIVIDE, LSVUtils::TPriority::MULTIPLICATION_DIVISION },
	{ LSVUtils::TOperation::FRAC, LSVUtils::TPriority::MULTIPLICATION_DIVISION },
	{ LSVUtils::TOperation::SUM, LSVUtils::TPriority::SUMMATION },
	{ LSVUtils::TOperation::POWER, LSVUtils::TPriority::POWERING }
};
