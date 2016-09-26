#pragma once

#include "IVisitor.h"
#include "IVisitorResult.h"
#include <string>

/*
	��� �������� ������� ������� ������ ���������
	������������ ����������� ���������. ��� ������ �
	��� �������������� ��� ������ �������� Visitor
	(��������� ��. ��������� ���������� (������ ��������������))
	����� ���� �������� ��������� �������, ������� ����������
	������� ������, ��� ��� ��������� ����� ��������� Expression.
	��� ��� ����� �������, ����� �� ����� ��� ��������� ������
	������ ���������, �������� ��������� ����������� �����
	IVisitorResult (����������, ��� ������ void *)
*/

class IExpression
{
public:
	virtual ~IExpression() {}
	virtual IVisitorResult* Accept(IVisitor *) = 0;
};

/*
	CIdExp �������� �� ����������, �� ������ ������ ���
	����������, ������� ������������ ��� ����������� 
	���������������� �������� ��� ����������, ����������,
	� ����� �������� ������� �� ����������
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
	const double *valueAddress; // ��� ������� ����������
};

/*
	CNumExp �������� �� �������� ���������, � ����� 
	��������� ����� ��������, ��� ��� ��� ������������
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
	COpExp �������� �� �������������� �������� ���
	��� ����������� ����������� (IExpression), ������ 
	�����, ������ ��������, � ����� ���� ��������
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
		��� �������� FRAC, �������� ���������� ������ ����
		��� �������� POWER, firstOperand - base, secondOperand - power
		��� ��������� ��������, ������ ����� �������.
	*/
private:
	IExpression *firstOperand;
	IExpression *secondOperand;
	TOperation operation;
};


/*
	CSumExp �������� �� ��������������� �������� �����
	���������� ��������� (IExpression), ������ � ����
	��� ���������� ��������, ��������� �������� ������������
	�������� �������� ������������, ��������� �� �����������
	���������. ��������� � �������� �������� ������� ������������
	������ ���� ������, ����������� start > finish, � ���� ������,
	����� ����� 0
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