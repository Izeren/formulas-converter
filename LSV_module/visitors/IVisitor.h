#pragma once

/*
	������ ���� forward declaration, ����� ����� �������
	��� ��������� ���� ������ ������, ������� �� ���������
	��������
*/
class COpExp;
class CNumExp;
class CIdExp;
class CSumExp;

/*
	��� ������� ���� ������ ����� �������� � �����������
	� ��������������� �������� ����� visit
*/

class IVisitor 
{

public:
	virtual void Visit(COpExp &exp) = 0;
	virtual void Visit(CNumExp &exp) = 0;
	virtual void Visit(CIdExp &exp) = 0;
	virtual void Visit(CSumExp &exp) = 0;
};