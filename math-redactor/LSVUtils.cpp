#include "LSVUtils.h"
#include <iostream>

bool LSVUtils::checkIdName(std::string name)
{
	std::regex idExpression("[_a-zA-Z][_0-9a-zA-Z]*");
	std::cout << "Checking the Id: " << name << "\n";
	bool validationResult = std::regex_match(name, idExpression);
	std::cout << validationResult << "\n";
	return validationResult;
}

bool LSVUtils::checkDouble(std::string value)
{
	std::regex floatNumber("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?");
	std::cout << "Checking the float value: " << value << "\n";
	bool validationResult = std::regex_match(value, floatNumber);
	std::cout << validationResult << "\n";
	return validationResult;
}