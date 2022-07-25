#include "Parser.hpp"

Lexema::Lexema(const std::string &input, std::size_t &index)
{
    std::vector<unsigned char> operators = {'=', '+', '-', '*', '/', '(',  ')', '^'};
    unsigned char c = input[index];
    
    for (size_t i = 0; i < operators.size(); ++i){
        if (c == operators[i]){
            lexemaType_ = oper;
            operType_ = static_cast<OperType>(i);
            ++index;
            return;
        }
    }

    operType_ = no_oper;
    if (std::isdigit(c) || c == static_cast<unsigned char>('.')) {
        lexemaType_ = value;
        size_t i = 0;
        value_ = std::stod(&input.c_str()[index], &i);
        index += i;
        return;
    } else if (std::isalpha(c)){
        lexemaType_ = var;
        varName_ = c;
        ++index;
    } else {
        std::string symbol;
        symbol.assign(static_cast<char>('.'), 1);
        throw std::logic_error(std::string(ERR_UNKNOW_SYMBOL) + symbol);
    }
}

std::ostream& operator << (std::ostream &os, const Lexema &lexema){
    std::vector<unsigned char> operators = {'=', '+', '-', '*', '/', '(',  ')', '^'};

    if (lexema.getLexemaType() == oper)
        os << operators[static_cast<std::size_t>(lexema.getOperType())];
    else if (lexema.getLexemaType() == value)
        os << lexema.getValue();
    else if (lexema.getLexemaType() == var)
        os << lexema.getVarName();
    return os; 
}