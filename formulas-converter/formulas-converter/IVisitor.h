#pragma once

#include "IVisitorResult.h"

class COpExp;
class CNumExp;
class CIdExp;

class IVisitor 
{

public:
	virtual IVisitorResult* Visit(COpExp *exp) = 0;
	virtual IVisitorResult* Visit(CNumExp *exp) = 0;
	virtual IVisitorResult* Visit(CIdExp *exp) = 0;
};