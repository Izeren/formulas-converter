#pragma once

#include <string>
#include <regex>
#include <unordered_map>

namespace LSVUtils
{

	enum TOperation { PLUS, MINUS, MULTIPLY, DIVIDE, FRAC, POWER, SUM };

	enum TPriority { EMPTY_FORMULA, SUMMATION, ADDITION_SUBSTRACTION, MULTIPLICATION_DIVISION, POWERING };

	bool checkIdName(std::string name);

	bool checkDouble(std::string value);


	class utilsSettings {
	public:
		static std::unordered_map<TOperation, TPriority> operationPriorities;
	};

}