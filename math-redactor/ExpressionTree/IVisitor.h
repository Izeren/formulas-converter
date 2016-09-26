#pragma once

#include "IVisitorResult.h"

/*
	������ ���� forward declaration, ����� ����� �������
	��� ��������� ���� ������ ������, ������� �� ���������
	��������
*/
class COpExp;
class CNumExp;
class CIdExp;

/*
	��� ������� ���� ������ ����� �������� � �����������
	� ��������������� �������� ����� visit
*/

class IVisitor 
{

public:
	virtual IVisitorResult* Visit(COpExp *exp) = 0;
	virtual IVisitorResult* Visit(CNumExp *exp) = 0;
	virtual IVisitorResult* Visit(CIdExp *exp) = 0;
};