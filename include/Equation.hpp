#pragma once

#include <utility>
#include <algorithm>
#include <vector>
#include <list>
#include <cmath>
#include <cfloat>
#include <stdexcept>
#include <exception>
#include <iostream>
#include <sstream>
#include <math.h>

#define POSS_EPS 0.01l

#define ERR_COMPLEX_COMPUTE "Too complex manipulation!"
#define ERR_DIV_BY_ZERO "Division by zero!"
#define ERR_DIFF_VAR "Operation with different variables! "
#define ERR_BIG_DEGREE "Degree cannot be greater than 2! Now degree : "
#define ERR_DEGENERATED "The equation has degenerated!"
#define ERR_NO_SOLUTION "The equation has no solution!"


struct ComplexNumber{
    ComplexNumber(double a, double b) : a_(a) , b_(b) {};
    ComplexNumber() = default;
    double          a_;
    double          b_;
};

ComplexNumber operator + (ComplexNumber c1, ComplexNumber c2);
ComplexNumber operator - (ComplexNumber c1, ComplexNumber c2);
ComplexNumber operator + (ComplexNumber c1, double d2);
ComplexNumber operator - (ComplexNumber c1, double d2);
ComplexNumber operator / (ComplexNumber c1, double d2);
ComplexNumber operator * (ComplexNumber c1, double d2);
std::ostream& operator << (std::ostream &os, const ComplexNumber &c);
bool operator == (const ComplexNumber &c1, const ComplexNumber &c2);


std::pair<ComplexNumber, ComplexNumber> doubleSqrt(double d);
std::pair<ComplexNumber, ComplexNumber> complexSqrt(ComplexNumber c);


struct Equation{
    Equation(std::vector<std::pair<double,double>> &coefs, char varName) : coefs_(coefs) , varName_(varName) {};
    Equation() = default;
    
    std::pair<ComplexNumber, ComplexNumber>         Solve(void);

    std::vector<std::pair<double,double>>           coefs_;
    char                                            varName_ = 0;
};

std::ostream& operator << (std::ostream &os, const Equation &e);
Equation operator + (Equation e1, Equation e2);
Equation operator - (Equation e1, Equation e2);
Equation operator * (Equation e1, Equation e2);
Equation operator / (Equation e1, Equation e2);


bool            isEqual(double a, double b);

