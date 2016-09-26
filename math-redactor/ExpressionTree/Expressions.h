#pragma once

#include "IVisitor.h"
#include "IVisitorResult.h"
#include <string>

/*
	ƒл€ хранени€ текущей формулы внутри редактора
	используетс€ древовидна€ структура. ¬с€ работа с
	ней осуществл€етс€ при помощи паттерна Visitor
	(подробнее см. ¬икипеди€ ѕосетитель (шаблон проектировани€))
	«десь есть описание отдельных классов, которые составл€ют
	вершины дерева, они все реализуют общий интерфейс Expression.
	“ак как часто хочетс€, чтобы на руках был результат обхода
	дерева визитором, объ€влен некоторый абстрактный класс
	IVisitorResult (фактически, это аналог void *)
*/

class IExpression
{
public:
	virtual ~IExpression() {}
	virtual IVisitorResult* Accept(IVisitor *) = 0;
};

/*
	CIdExp отвечает за переменные, он хранит только им€
	переменной, которое используетс€ дл€ подстановки 
	соответствующего значени€ при вычислении, сохранени€,
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
	const double *valueAddress; // ƒл€ быстрых вычислений
};

/*
	CNumExp отвечает за числовые константы, с целью 
	упрощени€ будем полагать, что они все вещественные
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
	COpExp отвечает за арифметические действи€ над
	над вычислимыми выражени€ми (IExpression), хранит 
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
		ƒл€ операции FRAC, операнды нумеруютс€ сверху вниз
		ƒл€ операции POWER, firstOperand - base, secondOperand - power
		ƒл€ остальных операций, просто слева направо.
	*/
private:
	IExpression *firstOperand;
	IExpression *secondOperand;
	TOperation operation;
};


/*
	CSumExp отвечает за индексированную конечную сумму
	вычислимых выражений (IExpression), хранит в себе
	им€ переменной счетчика, начальное значение суммировани€
	конечное значение суммировани€, указатель на вычисл€емое
	выражение. Ќачальное и конечное значени€ индекса суммировани€
	должны быть целыми, допускаетс€ start > finish, в этом случае,
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