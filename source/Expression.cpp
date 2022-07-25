#include "Expression.hpp"


static void             unexpectedLexemaMessage(const Lexema &lexema){
    std::stringstream ss;
    ss << ERR_LEXEMS_NOT_OVER << lexema;
    throw std::logic_error(ss.str());
}


void        Expression::compute(void){
    return exprComp();
}


void        Expression::exprComp(){
    std::size_t         saveIndex = 0;

    Equation e = exprAdditive(saveIndex);
    if (saveIndex){
        allResults_.push_back(e);
        while (saveIndex != exprLexems_.size()){
            if (saveIndex < exprLexems_.size() - 1 && exprLexems_[saveIndex].getLexemaType() == oper && exprLexems_[saveIndex].getOperType() == assigned) {
                ++saveIndex;
                e = exprAdditive(saveIndex);
                if (!saveIndex){
                    unexpectedLexemaMessage(exprLexems_[saveIndex]);
                }
                allResults_.push_back(e);
            } else {
                unexpectedLexemaMessage(exprLexems_[saveIndex]);
            }
        }
    } else{
        unexpectedLexemaMessage(exprLexems_[saveIndex]);
    }
}


Equation                Expression::exprAdditive(std::size_t &index){
    std::size_t         saveIndex = index;

    Equation ans = exprMultiplicative(saveIndex);
    if (saveIndex){
        index = saveIndex;
        while (saveIndex < exprLexems_.size() - 1 && exprLexems_[saveIndex].getLexemaType() == oper &&
            (exprLexems_[saveIndex].getOperType() == plus || exprLexems_[saveIndex].getOperType() == minus)){
                bool isSubtraction = exprLexems_[saveIndex].getOperType() == minus;
                ++saveIndex;

                Equation e = exprMultiplicative(saveIndex);
                if (!saveIndex)
                    unexpectedLexemaMessage(exprLexems_[saveIndex]);
                
                ans = isSubtraction ? ans - e : ans + e; 
                index = saveIndex;
        }
    }
    return ans;
}


Equation                Expression::exprMultiplicative(std::size_t &index){
    std::size_t         saveIndex = index;

    Equation ans = exprDegree(saveIndex);
    if (saveIndex){
        while (saveIndex) {
            index = saveIndex;
            bool isDivision = false;
            bool wasSign = false;
            if (saveIndex < exprLexems_.size() - 1 && exprLexems_[saveIndex].getLexemaType() == oper &&
                        (exprLexems_[saveIndex].getOperType() == multiplication || exprLexems_[saveIndex].getOperType() == division)){
                isDivision = exprLexems_[saveIndex].getOperType() == division;
                ++saveIndex;
                wasSign = true;
            }
            Equation e = exprDegree(saveIndex, wasSign);
            if (!saveIndex && wasSign){
                unexpectedLexemaMessage(exprLexems_[saveIndex]);
            } else if (!saveIndex && !wasSign){
                break;
            }

            if (isDivision)
                ans = ans / e;
            else
                ans = ans * e;
        }
    } else {
        index = 0;
    }
    return ans;
}


Equation                Expression::exprDegree(std::size_t &index, bool wasSign){
    std::size_t         saveIndex = index;

    Equation ans = exprUnar(saveIndex, wasSign);
    if (saveIndex){
        index = saveIndex;
        if (saveIndex < exprLexems_.size() - 1 && exprLexems_[saveIndex].getLexemaType() == oper && exprLexems_[saveIndex].getOperType() == degree){
            ++saveIndex;
            Equation e = exprUnar(saveIndex);
            if (e.coefs_.size() == 1 && e.coefs_[0].second + POSS_EPS > 0. && isEqual(e.coefs_[0].first, 0.)){
                double x = e.coefs_[0].second;
                if (isEqual(e.coefs_[0].second, 0.)){
                    std::vector<std::pair<double, double>>  tmp;
                    tmp.push_back(std::make_pair(0., 1.));
                    ans = Equation(tmp, 0);
                    index = saveIndex;
                }
                else if (isEqual(x, floor(x))){
                    Equation before = ans;
                    for (size_t i = 2; i <= static_cast<size_t>(x); ++i){
                        ans = ans * before;
                    }
                    index = saveIndex;
                } else {
                    std::stringstream ss;
                    ss << ERR_DIFF_DEGR << e;
                    throw std::logic_error(ss.str());
                }
            } else {
                std::stringstream ss;
                ss << ERR_DIFF_DEGR << e;
                throw std::logic_error(ss.str());
            }
        }
    } else {
        index = 0;
    }
    return ans;
}


Equation                Expression::exprUnar(std::size_t &index, bool wasSign){
    bool isNegative = false;

    if (index < exprLexems_.size() - 1 && exprLexems_[index].getLexemaType() == oper
        && (exprLexems_[index].getOperType() == plus || exprLexems_[index].getOperType() == minus)){
            if (!wasSign){
                index = 0;
                return Equation();
            } else if (exprLexems_[index].getOperType() == minus){
                isNegative = true;
            }
            ++index;
    }

    Equation ans = exprBrackets(index);
    if (isNegative) {
        std::vector<std::pair<double, double>> tmp;
        Equation tmpEquation = Equation(tmp, 0);
        ans = tmpEquation - ans;
    }
    return ans;
}


Equation                Expression::exprBrackets(std::size_t &index){
    std::size_t         saveIndex = index;

    Equation ans;
    if (saveIndex < exprLexems_.size() - 1 && exprLexems_[saveIndex].getLexemaType() == oper && exprLexems_[saveIndex].getOperType() == l_bracket){
        ++saveIndex;
        ans = exprAdditive(saveIndex);
        if (saveIndex){
            if (exprLexems_[saveIndex].getOperType() == r_bracket){
                index = saveIndex + 1;
            } else {
                std::stringstream ss;
                std::string s;
                s.assign('}', 1);
                ss << ERR_LEXEM_EXPECTED << s;
                throw std::logic_error(ss.str());
            }
        } else {
            throw std::logic_error(ERR_SYNTAX);
        }
    } else {
        ans = exprValue(saveIndex);

        if (saveIndex)
            index = saveIndex;
        else
            index = 0;
    }
    return ans;
}

Equation                Expression::exprValue(std::size_t &index){
    std::size_t         saveIndex = index;

    Equation ans = exprDigit(saveIndex);
    if (!saveIndex){
        saveIndex = index;
        ans = exprVar(saveIndex);
        index = saveIndex;
    } else {
        index = saveIndex;
    }
    return ans;
}

Equation                Expression::exprDigit(std::size_t &index){
    std::vector<std::pair<double, double>> vec;

    if (exprLexems_[index].getLexemaType() == value){
        vec.push_back(std::make_pair(0., exprLexems_[index].getValue()));
        ++index;
    } else {
        index = 0;
    }
    return Equation(vec, 0);
}

Equation                Expression::exprVar(std::size_t &index){
    std::vector<std::pair<double, double>> vec;
    if (exprLexems_[index].getLexemaType() == var){
        vec.push_back(std::make_pair(1., 1.));
        Equation ans = Equation(vec, exprLexems_[index].getVarName());
        ++index;
        return ans;
    }
    index = 0;
    return Equation(vec, 0);
}