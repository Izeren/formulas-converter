#pragma once

#include "IVisitor.h"
#include "IVisitorResult.h"
#include <string>

/*
	Для хранения текущей формулы внутри редактора
	используется древовидная структура. Вся работа с
	ней осуществляется при помощи паттерна Visitor
	(подробнее см. Википедия Посетитель (шаблон проектирования))
	Здесь есть описание отдельных классов, которые составляют
	вершины дерева, они все реализуют общий интерфейс Expression.
	Так как часто хочется, чтобы на руках был результат обхода
	дерева визитором, объявлен некоторый абстрактный класс
	IVisitorResult (фактически, это аналог void *)
*/

class IExpression
{
public:
	virtual ~IExpression() {}
	virtual IVisitorResult* Accept(IVisitor *) = 0;
};

/*
	CIdExp отвечает за переменные, он хранит только имя
	переменной, которое используется для подстановки 
	соответствующего значения при вычислении, сохранения,
	а также проверки формулы на валидность
*/

class CIdExp : public IExpression {
public:
	IVisitorResult* Accept(IVisitor *visitor) override;
	CIdExp(std::string name = "x");
	~CIdExp();

	std::string name;
};

/*
	CNumExp отвечает за числовые константы, с целью 
	упрощения будем полагать, что они все вещественные
*/

class CNumExp : public IExpression {
public:
	CNumExp(std::string value = "");
	IVisitorResult* Accept(IVisitor *visitor) override;
	~CNumExp();
	std::string value;
};

/*
	COpExp отвечает за арифметические действия над
	над вычислимыми выражениями (IExpression), хранит 
	левый, правый операнды, а также саму операцию
*/

enum TOperation { PLUS, MINUS, MULTIPLY, DIVISE };

class COpExp : public IExpression {
public:
	IVisitorResult* Accept(IVisitor *visitor) override;
	COpExp(IExpression *leftOperand = nullptr, IExpression *rightOperand = nullptr, TOperation operation = PLUS);
	~COpExp();

	IExpression *leftOperand;
	IExpression *rightOperand;
	TOperation operation;
};