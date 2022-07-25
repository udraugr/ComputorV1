#pragma once

#include "Equation.hpp"
#include "Lexema.hpp"
#include <sstream>

#define ERR_LEXEMS_NOT_OVER "Unexpected tokens! "
#define ERR_LEXEM_EXPECTED "Expected tokens! "
#define ERR_DIFF_DEGR "Difficult degree! "
#define ERR_SYNTAX "The syntax in the example is incorrect!"
#define ERR_NO_BRACKETS "The syntax in the example is incorrect!"

class Expression{
    public:
        Expression(const std::vector<Lexema> &lexems) : exprLexems_(lexems) {};
        ~Expression() = default;
        void                    compute();
        std::vector<Equation>   getAllResults() { return allResults_; };
    private:
        void                    exprComp();
        Equation                exprAdditive(std::size_t &index);
        Equation                exprMultiplicative(std::size_t &index);
        Equation                exprDegree(std::size_t &index, bool wasSign = true);
        Equation                exprUnar(std::size_t &index, bool wasSign = true);
        Equation                exprBrackets(std::size_t &index);
        Equation                exprValue(std::size_t &index);
        Equation                exprDigit(std::size_t &index);
        Equation                exprVar(std::size_t &index);
    private:
        std::vector<Lexema>     exprLexems_;
        std::vector<Equation>   allResults_;
};