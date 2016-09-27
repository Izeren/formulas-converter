#include "LSVUtils.h"
#include <iostream>

/*
	��� ������� ��������� ���������� ����� ����������
*/

bool LSVUtils::checkIdName(std::string name)
{
	std::regex idExpression("[_a-zA-Z][_0-9a-zA-Z]*");
	return std::regex_match(name, idExpression);
}

/*
	��� ������� ���������, ���������� ��������� ������������� �����
*/

bool LSVUtils::checkDouble(std::string value)
{
	std::regex floatNumber("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?");
	return std::regex_match(value, floatNumber);
}