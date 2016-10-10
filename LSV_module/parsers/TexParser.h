#pragma once

#include "./../expression_tree/Expression.h"
#include "./../pugixml/pugixml.hpp"
#include "./../utils/LSVUtils.h"
#include "./../visitors/CExportTexVisitor.h"
#include "Parser.h"
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <stack>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <fstream>
#include <streambuf>
#include <regex>

class CTexParser: public IParser {

	std::unordered_map<char, LSVUtils::TOperation> get_op;

	bool is_op(char c) {
		return get_op.find(c) != get_op.end();
	}

	bool left_assoc(char c) {
		return c == '+' || c == '-' || c == '*' || c == '/';
	}

	int priority(char op) {
		if (op < 0)
			return 4; // op == -'+' || op == -'-'
		return
			op == 's' ? 0 :
			op == '+' || op == '-' ? 1 :
			op == '*' || op == '/' || op == '%' ? 2 :
			-1;
	}

	void cleanDelims(std::string &line) {
		std::string line_no_delims;
		for (char ch : line) {
			if (!isspace(ch)) {
				line_no_delims += ch;
			}
		}
		line = line_no_delims;
	}

	void process_op(std::stack<std::shared_ptr<IExpression>> &expr_stack, char op) {
		if (op < 0) {
			std::shared_ptr<IExpression> l = expr_stack.top();  expr_stack.pop();
			switch (-op) {
			case '+':  {
				expr_stack.push(l);
				break;
			}
			case '-':  {
				std::shared_ptr<IExpression> zero = std::static_pointer_cast<IExpression>(std::make_shared<CNumExp>(0));
				expr_stack.push(std::static_pointer_cast<IExpression>(std::make_shared<COpExp>(zero, l, LSVUtils::TOperation::MINUS)));
				break;
			}
			}
		}
		else {
			std::shared_ptr<IExpression> r = expr_stack.top();  expr_stack.pop();
			std::shared_ptr<IExpression> l = expr_stack.top();  expr_stack.pop();
			LSVUtils::TOperation t_op = get_op[op];
			if (t_op == LSVUtils::TOperation::SUM) {
				std::shared_ptr<CSumExp> sum_exp = std::static_pointer_cast<CSumExp>(l);
				sum_exp->setExpression(r);
				expr_stack.push(std::static_pointer_cast<IExpression>(sum_exp));
			}
			else {
				expr_stack.push(std::static_pointer_cast<IExpression>(std::make_shared<COpExp>(l, r, t_op)));
			}
		}
	}
	
	std::shared_ptr<IExpression> parseExpr(std::string &str, size_t &start) {

		std::stack<std::shared_ptr<IExpression>> expr_stack;
		std::stack<char> op_stack;
		bool may_unary = true;

		for (size_t i = start; i < str.length(); ++i) {

			if (str[i] == '(') {
				op_stack.push('(');
			}
			else if (str[i] == ')') {
				while (op_stack.top() != '(') {
					process_op(expr_stack, op_stack.top());
					op_stack.pop();
				}
				op_stack.pop();
			}
			else if (is_op(str[i])) {
				char cur_op = str[i];
				while (!op_stack.empty() && (
					left_assoc(cur_op) && priority(op_stack.top()) >= priority(cur_op)
					|| !left_assoc(cur_op) && priority(op_stack.top()) > priority(cur_op)))
				{
					process_op(expr_stack, op_stack.top());
					op_stack.pop();
				}
				op_stack.push(cur_op);
				may_unary = false;
			}
			else {

				std::string operand;
				while (i < str.length() && (isalnum(str[i]) || str[i] == '\\')) {
					operand += str[i++];
				}
				--i;
				if (isdigit(operand[0])) {
					expr_stack.push(std::static_pointer_cast<IExpression>(std::make_shared<CNumExp>(std::stoi(operand))));
				}
				else if (operand[0] == '\\') {
					if (operand == "\\sum") {
						parseSum(expr_stack, str, ++i);
						op_stack.push('s');
					}
					else if (operand == "\\frac") {
						expr_stack.push(parseFrac(expr_stack, str, ++i));
					}
				}
				else {
					expr_stack.push(std::static_pointer_cast<IExpression>(std::make_shared<CIdExp>(operand)));
				}

				may_unary = false;
			}
		}

		while (!op_stack.empty()) {
			process_op(expr_stack, op_stack.top());
			op_stack.pop();
		}

		if (expr_stack.size() != 1) {
			errorMessage("Too few operands");
		}
		return expr_stack.top();
	}


	void parseSum(std::stack<std::shared_ptr<IExpression>> &expr_stack, std::string &str, size_t &ind) {

		std::regex sum_exp("_\\{[_[:alnum:]][_[:alnum:][:digit:]]*=[[:digit:]]+\\}\\^\\{[[:digit:]]+\\}.*");
		//std::regex sum_exp("\\sum_.*");
		std::string substr = str.substr(ind);
		if (!std::regex_match(substr, sum_exp)) {
			errorMessage("\\sum must be followed by '_{...}^{...}'");
		}

		std::string sum_id;
		ind += 2;
		while (ind < str.length() && isalnum(str[ind])) {
			sum_id += str[ind++];
		}
		
		ind++;

		std::string sum_start_str;
		while (ind < str.length() && isalnum(str[ind])) {
			sum_start_str += str[ind++];
		}

		ind += 3;

		std::string sum_end_str;
		while (ind < str.length() && isalnum(str[ind])) {
			sum_end_str += str[ind++];
		}

		int sum_start = std::stoi(sum_start_str);
		int sum_end = std::stoi(sum_end_str);

		expr_stack.push(std::static_pointer_cast<IExpression>(std::make_shared<CSumExp>(sum_id, sum_start, sum_end, nullptr)));
	}

	std::shared_ptr<IExpression> parseFrac(std::stack<std::shared_ptr<IExpression>> &expr_stack, std::string &str, size_t &ind) {

		std::regex sum_exp("\\{.*\\}\\{.*\\}.*");
		std::string substr = str.substr(ind);
		if (!std::regex_match(substr, sum_exp)) {
			errorMessage("\\frac must be followed by '{...}{...}'");
		}

		ind++;
		std::string first;
		while (ind < str.length() && str[ind] != '}') {
			first += str[ind++];
		}

		ind += 2;
		std::string second;
		while (ind < str.length() && str[ind] != '}') {
			second += str[ind++];
		}
		
		size_t first_start = 0;
		std::shared_ptr<IExpression> first_expr = parseExpr(first, first_start);

		size_t second_start = 0;
		std::shared_ptr<IExpression> second_expr = parseExpr(second, second_start);

		return std::static_pointer_cast<IExpression>(std::make_shared<COpExp>(first_expr, second_expr, LSVUtils::TOperation::FRAC));
	}


	void errorMessage(std::string &message);
	void errorMessage(const char *message);

public:

	CTexParser() {
		get_op['+'] = LSVUtils::TOperation::PLUS;
		get_op['-'] = LSVUtils::TOperation::MINUS;
		get_op['*'] = LSVUtils::TOperation::MULTIPLY;
		get_op['/'] = LSVUtils::TOperation::DIVIDE;
		get_op['^'] = LSVUtils::TOperation::POWER;
		get_op['s'] = LSVUtils::TOperation::SUM;
	}

	std::string buildFromTree(std::shared_ptr<IExpression> expr) override;

	std::shared_ptr<IExpression> parseFromFile(const char *path) override;

	std::shared_ptr<IExpression> parse(const std::string &str) override;

};

