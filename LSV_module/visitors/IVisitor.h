#pragma once

/*
	¬ажный блок forward declaration, здесь нужно указать
	все возможные типы вершин дерева, которое мы планируем
	обходить
*/
class COpExp;
class CNumExp;
class CIdExp;
class CSumExp;

/*
	ƒл€ каждого типа вершин нужно объ€вить и реализовать
	в соответствующем визиторе метод visit
*/

class IVisitor 
{

public:
	virtual void Visit(COpExp &exp) = 0;
	virtual void Visit(CNumExp &exp) = 0;
	virtual void Visit(CIdExp &exp) = 0;
	virtual void Visit(CSumExp &exp) = 0;
};