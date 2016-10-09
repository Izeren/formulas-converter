#pragma once

#include "./../expression_tree/Expression.h"
#include "./../pugixml/pugixml.hpp"
#include "./../utils/LSVUtils.h"
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <stack>
#include <set>
#include <map>

namespace StrOp {
	const std::string PLUS = "+";
	const std::string MINUS = "-";
	const std::string MULT = "*";
	const std::string DIVIDE = "/";
	const std::string POWER = "^";
	const std::string SUM = "&sum;";
	const std::string LBRACE = "(";
	const std::string RBRACE = ")";
};



class CMathMLParser {

	bool is_op(char c) {
		return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
	}

	bool left_assoc(char c) {
		return c == '+' || c == '-' || c == '*' || c == '/';
	}

	int priority(char op) {
		if (op < 0)
			return 4; // op == -'+' || op == -'-'
		return
			op == '+' || op == '-' ? 1 :
			op == '*' || op == '/' || op == '%' ? 2 :
			-1;
	}

	LSVUtils::TOperation getTOp(char op) {
		switch (op) {
		case '+':  {
			return LSVUtils::TOperation::PLUS;
			break;
		}
		case '-':  {
			return LSVUtils::TOperation::MINUS;
			break;
		}
		case '*':  {
			return LSVUtils::TOperation::MULTIPLY;
			break;
		}
		case '/':  {
			return LSVUtils::TOperation::DIVIDE;
			break;
		}
		case '^':  {
			return LSVUtils::TOperation::POWER;
			break;
		}
		default:
		{
			return LSVUtils::TOperation::PLUS;
		}
		}
	}

	std::string cleanDelims(std::string &line) {
		std::string line_no_delims;
		for (char ch : line) {
			if (!isspace(ch)) {
				line_no_delims += ch;
			}
		}
		return line_no_delims;
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
			LSVUtils::TOperation t_op = getTOp(op);
			expr_stack.push(std::static_pointer_cast<IExpression>(std::make_shared<COpExp>(l, r, t_op)));
		}
	}

	//obtains next Operator token while parsing an expression node
	void parseOperator(pugi::xml_node &node,
		std::stack<std::shared_ptr<IExpression>> &expr_stack,
		std::stack<char> &op_stack,
		bool &may_unary)
	{

		pugi::xml_node op_node = node.first_child();
		if (op_node.type() != pugi::node_pcdata) {
			errorMessage("Operation node must contain operator");
		}

		std::string op_str(cleanDelims(std::string(op_node.value())));
		if (op_str.length() > 1) {
			errorMessage("Wrong operator: " + op_str);
		} else if (op_str.empty()) {
			errorMessage("Empty operator");
		}

		char cur_op = op_str[0];

		if (cur_op == '(') {
			op_stack.push(cur_op);
			may_unary = true;
		} else if (cur_op == ')') {
			while (op_stack.top() != '(') {
				process_op(expr_stack, op_stack.top());
				op_stack.pop();
			}
			op_stack.pop();
			may_unary = false;
		} else if (is_op(cur_op)) {
			if (may_unary && (cur_op == '+' || cur_op == '-'))  cur_op = -cur_op;
			while (!op_stack.empty() && (
				left_assoc(cur_op) && priority(op_stack.top()) >= priority(cur_op) ||
				!left_assoc(cur_op) && priority(op_stack.top()) > priority(cur_op)))
			{
				process_op(expr_stack, op_stack.top());

			}
			op_stack.push(cur_op);
			may_unary = true;
		}
	}

	//obtains next Number token while parsing an expression node
	void parseNum(pugi::xml_node &node,
		std::stack<std::shared_ptr<IExpression>> &expr_stack) {

		pugi::xml_node num_node = node.first_child();
		if (num_node.type() != pugi::node_pcdata) {
			errorMessage("Number node must contain value");
		}

		std::string num_str(cleanDelims(std::string(num_node.value())));
		if (!LSVUtils::checkDouble(num_str)) {
			errorMessage("Incorrect number :" + num_str);
		} 
		else if (num_str.empty()) {
			errorMessage("Empty number");
		}


		std::shared_ptr<IExpression> num = std::static_pointer_cast<IExpression>(std::make_shared<CNumExp>(num_str));
		expr_stack.push(num);
	}

	//obtains next Identifier token while parsing an expression node
	void parseId(pugi::xml_node &node,
		std::stack<std::shared_ptr<IExpression>> &expr_stack) {

		pugi::xml_node id_node = node.first_child();
		if (id_node.type() != pugi::node_pcdata) {
			errorMessage("Identifier node must contain value");
		}

		std::string id_str(cleanDelims(std::string(id_node.value())));
		if (!LSVUtils::checkIdName(id_str)) {
			errorMessage("Incorrect identifier :" + id_str);
		}
		else if (id_str.empty()) {
			errorMessage("Empty identifier");
		}

		std::shared_ptr<IExpression> num = std::static_pointer_cast<IExpression>(std::make_shared<CIdExp>(id_str));
		expr_stack.push(num);

	}

	std::shared_ptr<IExpression> parseExpr(pugi::xml_node &node) {

		int node_size = std::distance(node.children().begin(), node.children().end());

		if (node_size == 0) {
			return nullptr;
		}

		std::stack<std::shared_ptr<IExpression>> expr_stack;
		std::stack<char> op_stack;
		bool may_unary = true;

		if (node_size == 1 && node.first_child().type() == pugi::node_pcdata) {
			std::string value = std::string(node.first_child().value());
			if (nameEqual(node, "mo")) {
				errorMessage("Expression node consists only from operator: " + value);
			}
			else if (nameEqual(node, "mn")) {
				parseNum(node, expr_stack);
			}
			else if (nameEqual(node, "mi")) {
				parseId(node, expr_stack);
			}
			else if (nameEqual(node, "msup")) {
				expr_stack.push(parsePower(node));
			}
			else if (nameEqual(node, "mfrac")) {
				expr_stack.push(parseFrac(node));
			}
			else if (nameEqual(node, "munderover")) {
				if (nameEqual(node, "mo")) {
					errorMessage("Expression node consists only from operator sum");
				}
			}
		} 
		else {
			
			for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it) {

				if (nameEqual(*it, "mo")) {
					parseOperator(*it, expr_stack, op_stack, may_unary);
				}
				else {
					if (nameEqual(*it, "mn")) {
						parseNum(*it, expr_stack);
					}
					else if (nameEqual(*it, "mi")) {
						parseId(*it, expr_stack);
					}
					else if (nameEqual(*it, "msup")) {
						expr_stack.push(parsePower(*it));
					}
					else if (nameEqual(*it, "mfrac")) {
						expr_stack.push(parseFrac(*it));
					}
					else if (nameEqual(*it, "munderover")) {
						pugi::xml_node sum = *it;
						++it;
						pugi::xml_node expr = *it;
						expr_stack.push(parseSum(sum, expr));
					}
					else if (nameEqual(*it, "mrow")) {
						expr_stack.push(parseExpr(*it));
					}
					may_unary = false;
				}
			}
		}

		while (!op_stack.empty()) {
			process_op(expr_stack, op_stack.top());
			op_stack.pop();
		}

		if (expr_stack.size() != 1) {
			errorMessage("Incorrect expression at node " + std::string(node.name()));
		}
		return expr_stack.top();
	}


	std::shared_ptr<IExpression> parseSum(pugi::xml_node &sum_node, pugi::xml_node &arg_node) {
		
		if (std::distance(sum_node.children().begin(), sum_node.children().end()) != 3) {
			errorMessage("Sum node must contain exactly three children");
		}

		if (!nameEqual(sum_node, "munderover")) {
			errorMessage("Sum node must have name \"munderover\"");
		}


		//operator
		pugi::xml_node sum_op = sum_node.first_child();
		pugi::xml_node sum_op_val = sum_op.first_child();

		if (strcmp(sum_op_val.value(), "&sum;")) {
			errorMessage("Sum operator node must have value \"&sum;\"");
		}


		//range start
		pugi::xml_node start_range = sum_op.next_sibling();
		
		if (std::distance(start_range.children().begin(), start_range.children().end()) != 3) {
			errorMessage("Sum expression node must contain exactly three children");
		}

		pugi::xml_node range_id = start_range.first_child();
		pugi::xml_node range_op = range_id.next_sibling();
		pugi::xml_node range_start_val = range_op.next_sibling();

		if (!nameEqual(range_id, "mi")) {
			errorMessage("Sum expression node must contain identifier");
		}
		if (!nameEqual(range_op, "mo") || strcmp(range_op.first_child().value(), "=")) {
			errorMessage("Sum expression node must contain operator =");
		}
		if (!nameEqual(range_start_val, "mn")) {
			errorMessage("Sum expression node must contain start value");
		}

		std::string range_id_str = std::string(range_id.first_child().value());
		std::string range_start_val_str = std::string(range_start_val.first_child().value());

		if (!LSVUtils::checkIdName(range_id_str)) {
			errorMessage("Sum expression node contains illegal identifier");
		}

		if (!LSVUtils::checkDouble(range_start_val_str)) {
			errorMessage("Sum expression node contains illegal start value");
		}


		//range end
		pugi::xml_node end_range = start_range.next_sibling();
		pugi::xml_node range_end_val = end_range.first_child();

		if (!nameEqual(range_end_val, "mn")) {
			errorMessage("Sum expression node must contain end value");
		}

		std::string range_end_val_str = std::string(range_end_val.first_child().value());
		if (!LSVUtils::checkDouble(range_end_val_str)) {
			errorMessage("Sum expression node contains illegal end value");
		}

		int range_start_val_int = std::stoi(range_start_val_str);
		int range_end_val_int = std::stoi(range_end_val_str);
		std::shared_ptr<IExpression> expr = parseExpr(arg_node);

		//build sum and return
		return std::static_pointer_cast<IExpression>(
			std::make_shared<CSumExp>(range_id_str, range_start_val_int, range_end_val_int, expr));
	}

	std::shared_ptr<IExpression> parsePower(pugi::xml_node &node) {

		if (std::distance(node.children().begin(), node.children().end()) != 2) {
			errorMessage("Power node must contain exactly two children");
		}

		if (!nameEqual(node, "msup")) {
			errorMessage("Power node must have name \"msup\"");
		}

		pugi::xml_node first = node.first_child();
		pugi::xml_node second = first.next_sibling();

		std::shared_ptr<IExpression> first_expr = parseExpr(first);
		std::shared_ptr<IExpression> second_expr = parseExpr(second);

		return std::static_pointer_cast<IExpression>(std::make_shared<COpExp>(first_expr, second_expr, LSVUtils::TOperation::POWER));
	}

	std::shared_ptr<IExpression> parseFrac(pugi::xml_node &node) {

		if (std::distance(node.children().begin(), node.children().end()) != 2) {
			errorMessage("Power node must contain exactly two children");
		}

		if (!nameEqual(node, "mfrac")) {
			errorMessage("Frac node must have name \"mfrac\"");
		}

		pugi::xml_node first = node.first_child();
		pugi::xml_node second = first.next_sibling();

		std::shared_ptr<IExpression> first_expr = parseExpr(first);
		std::shared_ptr<IExpression> second_expr = parseExpr(second);

		return std::static_pointer_cast<IExpression>(std::make_shared<COpExp>(first_expr, second_expr, LSVUtils::TOperation::FRAC));
	}


	bool nameEqual(pugi::xml_node &node, const char *name);
	void errorMessage(std::string &message);
	void errorMessage(const char *message);

public:

	std::string buildFromTree(std::shared_ptr<IExpression> expr);

	std::shared_ptr<IExpression> parseFromFile(const char *path);

};

