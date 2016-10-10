#pragma once

#define BAD_ID "error_name"
#include "../visitors/IVisitor.h"
#include <string>
#include <memory>
#include <iostream>

/**
	Для хранения текущей формулы внутри редактора
	используется древовидная структура. Вся работа с
	ней осуществляется при помощи паттерна Visitor
	(подробнее см. Википедия Посетитель (шаблон проектирования))
	Здесь есть описание отдельных классов, которые составляют
	вершины дерева, они все реализуют общий интерфейс Expression.
*/
class IExpression
{
public:
	virtual ~IExpression() {}
	virtual void Accept(IVisitor &) = 0;
};

/**
	CIdExp отвечает за переменные, он хранит только имя
	переменной, которое используется для подстановки 
	соответствующего значения при вычислении, сохранения,
	а также проверки формулы на валидность
*/
class CIdExp : public IExpression {
public:
	void Accept(IVisitor &visitor) override;
	CIdExp();
	CIdExp(const std::string &name);

	bool setName(const std::string &name);
	std::string getName() const;
	bool setAddress(const double* address);
	const double *getAddress() const;
	double getValue() const;

private:
	std::string name;
	const double *valueAddress; // Для быстрых вычислений
};

/**
	CNumExp отвечает за числовые константы, с целью 
	упрощения будем полагать, что они все вещественные
*/
class CNumExp : public IExpression {
public:
	CNumExp(const std::string &value);
	CNumExp(double value = 0);
	void Accept(IVisitor &visitor) override;

	bool setValue(const std::string &value);
	void setValue(double value);
	double getValue() const;

private:
	double value;
};



enum TOperation { PLUS, MINUS, MULTIPLY, DIVIDE, FRAC, POWER, SUM };


/**
COpExp отвечает за арифметические действия над
над вычислимыми выражениями (IExpression), хранит
левый, правый операнды, а также саму операцию
*/
class COpExp : public IExpression {
public:
	void Accept(IVisitor &visitor) override;
	COpExp(std::shared_ptr<IExpression> leftOperand, std::shared_ptr<IExpression> rightOperand, TOperation operation);
	COpExp();

	bool setFirstOperand(std::shared_ptr<IExpression> pointer);
	std::shared_ptr<IExpression> getFirstOperand() const;
	bool setSecondOperand(std::shared_ptr<IExpression> pointer);
	std::shared_ptr<IExpression> getSecondOperand() const;
	void setOperation(TOperation operation);
	TOperation getOperation() const;

	/*
		Для операции FRAC, операнды нумеруются сверху вниз
		Для операции POWER, firstOperand - base, secondOperand - power
		Для остальных операций, просто слева направо.
	*/
private:
	std::shared_ptr<IExpression> firstOperand;
	std::shared_ptr<IExpression> secondOperand;
	TOperation operation;
};


/**
	CSumExp отвечает за индексированную конечную сумму
	вычислимых выражений (IExpression), хранит в себе
	имя переменной счетчика, начальное значение суммирования
	конечное значение суммирования, указатель на вычисляемое
	выражение. Начальное и конечное значения индекса суммирования
	должны быть целыми, допускается start > finish, в этом случае,
	сумма равна 0
*/

class CSumExp : public IExpression
{
public:
	void Accept(IVisitor &visitor) override;
	CSumExp();
	CSumExp(const std::string &indexName, int startId, int finishId, std::shared_ptr<IExpression> expression);

	bool setIndexName(const std::string &indexName);
	std::string getIndexName() const;
	void setStartId(int startId);
	int getStartId() const;
	void setFinishId(int finishId);
	int getFinishId() const;
	bool setExpression(std::shared_ptr<IExpression> pointer);
	std::shared_ptr<IExpression> getExpression() const;

private:

	std::string indexName;
	int startId;
	int finishId;
	std::shared_ptr<IExpression> expression;
};