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

int main123123213()
{

	try {

		Converter converter;

		std::ofstream out_mathml("format_files/out_mathml");
		out_mathml << converter.convert("format_files/test1.tex", LSVUtils::TEX, LSVUtils::MATHML);
		out_mathml.close();
		
		std::ofstream out_openmath("format_files/out_openmath");
		out_openmath << converter.convert("format_files/out_mathml", LSVUtils::MATHML, LSVUtils::OPENMATH);
		out_openmath.close();

		std::ofstream out_tex("format_files/out_tex");
		out_tex << converter.convert("format_files/out_openmath", LSVUtils::OPENMATH, LSVUtils::TEX);
		out_tex.close();

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