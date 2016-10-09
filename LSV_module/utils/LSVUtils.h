#pragma once

#include <string>
#include <regex>
#include <unordered_map>

namespace LSVUtils
{

	enum TOperation { PLUS, MINUS, MULTIPLY, DIVIDE, FRAC, POWER, SUMMATION };

	enum TPriority { SUMMATION, ADDITION_SUBSTRACTION, MULTIPLICATION_DIVISION, POWER };

	bool checkIdName(std::string name);

	bool checkDouble(std::string value);


	static class utilsSettings {
	public:
		static std::unordered_map<TOperation, TPriority> operationPriorities;
	};

}