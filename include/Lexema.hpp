#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <exception>

#define ERR_UNKNOW_SYMBOL "Invalid characters entered! "

enum LexemaType{
    oper,
    value,
    var
};

enum OperType{
    assigned = 0,
    plus,
    minus,
    multiplication,
    division,
    l_bracket,
    r_bracket,
    degree,
    no_oper
};

class Lexema{
    public:
        Lexema(const std::string &input, std::size_t &index);
        ~Lexema() = default;

        LexemaType      getLexemaType() const{ return lexemaType_; }
        OperType        getOperType() const { return operType_; }
        double          getValue() const { return value_; }
        char            getVarName() const { return varName_; }
    private:
        LexemaType      lexemaType_;
        OperType        operType_;
        double          value_;
        char            varName_;
};

std::ostream& operator << (std::ostream &os, const Lexema &lexema);