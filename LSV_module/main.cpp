#include "expression_tree/Expression.h"
#include "visitors/CSintacticValdationVisitor.h"
#include "visitors/CPrintVisitor.h"
#include "parsers/TexParser.h"
#include "parsers/OpenMathParser.h"
#include "converters/Converter.h"
#include <fstream>
#include <iostream>
#include <set>

void testOpenMath(int numberOfTest);
void testTex(int numberOfTest);
void testMathML(int numberOfTest);

int main()
{

	try {

		Converter converter;
		COpenMathParser parser;
		std::shared_ptr<IExpression> operationTree = parser.parseFromFile("format_files/test1.openmath");
		CPrintVisitor printVisitor;
		operationTree->Accept(printVisitor);
		std::cout << printVisitor.getDigraphDescription() << "\n";

		std::set<std::string> ids = { "x" };
		CSintacticValidationVisitor validator;
		validator.setVisibleIds(ids);
		operationTree->Accept(validator);
		std::cout << validator.getValidationStatus() << "\n";
		/*
		std::ofstream outML("format_files/a_mathml.txt");
		outML << converter.convert("format_files/a.txt", LSVUtils::TEX, LSVUtils::MATHML);
		outML.close();

		std::ofstream outOpenMath("format_files/a_om.txt");
		outOpenMath << converter.convert("format_files/a_mathml.txt", LSVUtils::MATHML, LSVUtils::OPENMATH);
		outOpenMath.close();

		std::ofstream outTex("format_files/a_tex.txt");
		outTex << converter.convert("format_files/a_om.txt", LSVUtils::OPENMATH, LSVUtils::TEX);
		outTex.close();
		*/



		
	} catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}

	system("pause");

	return 0;
}

void testOpenMath(int numberOfTest)
{
	std::string path = "format_files/test" + std::to_string(numberOfTest) + ".openmath";
	std::cout << "Test " << numberOfTest << " from file: " + path << "\n";
	Converter converter;
	std::ofstream out("format_files/open_math_result_test" + std::to_string(numberOfTest) + ".txt");
	switch (numberOfTest)
	{
		case 1:
		{
			out << converter.convert(path.c_str(), LSVUtils::OPENMATH, LSVUtils::TEX);
			break;
		}
		case 2:
		{
			out << converter.convert(path.c_str(), LSVUtils::OPENMATH, LSVUtils::MATHML);
			break;
		}
		default:
		{
			break;
		}
	}
	out.close();
}

void testTex(int numberOfTest)
{
	std::string path = "format_files/test" + std::to_string(numberOfTest) + ".tex";
	std::cout << "Test " << numberOfTest << " from file: " + path << "\n";
	Converter converter;
	std::ofstream out("format_files/tex_result_test" + std::to_string(numberOfTest) + ".txt");
	switch (numberOfTest)
	{
	case 1:
	{
		out << converter.convert(path.c_str(), LSVUtils::TEX, LSVUtils::OPENMATH);
		break;
	}
	case 2:
	{
		out << converter.convert(path.c_str(), LSVUtils::TEX, LSVUtils::MATHML);
		break;
	}
	default:
	{
		break;
	}
	}
	out.close();
}

void testMathML(int numberOfTest)
{
	std::string path = "format_files/test" + std::to_string(numberOfTest) + ".mathml";
	std::cout << "Test " << numberOfTest << " from file: " + path << "\n";
	Converter converter;
	std::ofstream out("format_files/math_ml_result_test" + std::to_string(numberOfTest) + ".txt");
	switch (numberOfTest)
	{
	case 1:
	{
		out << converter.convert(path.c_str(), LSVUtils::MATHML, LSVUtils::OPENMATH);
		break;
	}
	case 2:
	{
		out << converter.convert(path.c_str(), LSVUtils::MATHML, LSVUtils::TEX);
		break;
	}
	default:
	{
		break;
	}
	}
	out.close();
}