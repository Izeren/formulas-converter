#pragma once

#include "../visitors/IVisitor.h"
#include "../visitor_results/IVisitorResult.h"
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
	virtual ~CIdExp();

	bool setName(std::string name);
	std::string getName() const;
	bool setAddress(const double* address);
	const double *getAddress() const;
	double getValue() const;

private:
	std::string name;
	const double *valueAddress; // Для быстрых вычислений
};

/*
	CNumExp отвечает за числовые константы, с целью 
	упрощения будем полагать, что они все вещественные
*/

class CNumExp : public IExpression {
public:
	CNumExp(std::string value = "");
	CNumExp(double value = 0);
	IVisitorResult* Accept(IVisitor *visitor) override;
	virtual ~CNumExp();

	bool setValue(std::string value);
	void setValue(double value);
	double getValue() const;

private:
	double value;
};

/*
	COpExp отвечает за арифметические действия над
	над вычислимыми выражениями (IExpression), хранит 
	левый, правый операнды, а также саму операцию
*/

enum TOperation { PLUS, MINUS, MULTIPLY, DIVISE, FRAC, POWER};

class COpExp : public IExpression {
public:
	IVisitorResult* Accept(IVisitor *visitor) override;
	COpExp(IExpression *leftOperand = nullptr, IExpression *rightOperand = nullptr, TOperation operation = PLUS);
	virtual ~COpExp();

	bool setFirstOperand(IExpression *pointer);
	IExpression *getFirstOperand() const;
	bool setSecondOperand(IExpression *pointer);
	IExpression *getSecondOperand() const;
	void setOperation(TOperation operation);
	TOperation getOperation() const;

	/*
		Для операции FRAC, операнды нумеруются сверху вниз
		Для операции POWER, firstOperand - base, secondOperand - power
		Для остальных операций, просто слева направо.
	*/
private:
	IExpression *firstOperand;
	IExpression *secondOperand;
	TOperation operation;
};


/*
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
	IVisitorResult* Accept(IVisitor *visitor) override;
	CSumExp(std::string indexName = "k", int startId = 1, int finishId = 5, IExpression *expression = 0);
	virtual ~CSumExp();

	bool setIndexName(std::string indexName);
	std::string getIndexName() const;
	void setStartId(int startId);
	int getStartId() const;
	void setFinishId(int finishId);
	int getFinishId() const;
	bool setExpression(IExpression *pointer);
	IExpression *getExpression() const;

private:

	std::string indexName;
	int startId;
	int finishId;
	IExpression *expression;
};