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
	~CIdExp();

	std::string name;
};

/*
	CNumExp �������� �� �������� ���������, � ����� 
	��������� ����� ��������, ��� ��� ��� ������������
*/

class CNumExp : public IExpression {
public:
	CNumExp(std::string value = "");
	IVisitorResult* Accept(IVisitor *visitor) override;
	~CNumExp();
	std::string value;
};

/*
	COpExp �������� �� �������������� �������� ���
	��� ����������� ����������� (IExpression), ������ 
	�����, ������ ��������, � ����� ���� ��������
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