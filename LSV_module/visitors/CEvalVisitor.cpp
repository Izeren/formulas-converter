#include "CEvalVisitor.h"

CEvalVisitor::CEvalVisitor( std::unordered_map<std::string, double> _idExpValues )
    : evalFailed(false), idExpValues(_idExpValues)
{}

void CEvalVisitor::Visit( COpExp &exp )
{
    exp.getFirstOperand()->Accept( *this );
    exp.getSecondOperand()->Accept( *this );
    if( evalFailed ) {
        return;
    }
    double secondOperand = valueStack.back();
    valueStack.pop_back();
    double firstOperand = valueStack.back();
    valueStack.pop_back();
    LSVUtils::TOperation operation = exp.getOperation();
    switch( operation ) {
        case LSVUtils::TOperation::PLUS:
        {
            valueStack.push_back(firstOperand + secondOperand);
            break;
        }
        case LSVUtils::TOperation::MINUS:
        {
            valueStack.push_back( firstOperand - secondOperand );
            break;
        }
        case LSVUtils::TOperation::MULTIPLY:
        {
            valueStack.push_back( firstOperand * secondOperand );
            break;
        }
        case LSVUtils::TOperation::FRAC:
        case LSVUtils::TOperation::DIVIDE:
        {
            if( secondOperand != 0 ) {
                valueStack.push_back( firstOperand / secondOperand );
                break;
            } else {
                evalFailed = true;
            }
        }
        case LSVUtils::TOperation::POWER:
        {
            valueStack.push_back( std::pow( firstOperand, secondOperand ) );
            break;
        }
        default:
        {
            evalFailed = true;
        }
    }
}

void CEvalVisitor::Visit( CNumExp &exp )
{
    valueStack.push_back( exp.getValue() );
}

void CEvalVisitor::Visit( CIdExp &exp )
{
    std::string name = exp.getName();
    valueStack.push_back(idExpValues.at(name));
}

void CEvalVisitor::Visit( CSumExp &exp )
{
    auto internalExp = exp.getExpression();
    double sum = 0;
    for( int sumIndex = exp.getStartId(); sumIndex < exp.getFinishId(); ++sumIndex ) {
        idExpValues[exp.getIndexName()] = sumIndex;
        internalExp->Accept( *this );
        if( evalFailed ) {
            break;
        } else {
            sum += valueStack.back();
            valueStack.pop_back();
        }
    }
    if( !evalFailed ) {
        valueStack.push_back(sum);
    }
}

bool CEvalVisitor::isEvalFailed()
{
    evalFailed = evalFailed || valueStack.size() != 1;
    return evalFailed;
}

double CEvalVisitor::getValue() const
{
    return valueStack.front();
}
