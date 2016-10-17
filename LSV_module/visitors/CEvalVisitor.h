#pragma once
#include "IVisitor.h"
#include "../expression_tree/Expression.h"
#include "../utils/LSVUtils.h"

#include <string>
#include <unordered_map>
#include <vector>

class CEvalVisitor : public IVisitor
{
public:
    CEvalVisitor( std::unordered_map<std::string, double> _idExpValues );

    void Visit( COpExp &exp ) override;
    void Visit( CNumExp &exp ) override;
    void Visit( CIdExp &exp ) override;
    void Visit( CSumExp &exp ) override;

    bool isEvalFailed();
    double getValue() const;
private:
    bool evalFailed;
    std::vector<double> valueStack;
    std::unordered_map<std::string, double> idExpValues;
};