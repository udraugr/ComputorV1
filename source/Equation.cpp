#include "Equation.hpp"

bool isEqual(double a, double b) { return std::abs(a - b) < POSS_EPS; }

static void add(double &a, double b) { a += b; }

static void subt(double &a, double b) { a -= b; }

static void checkDegree(const std::pair<double, double> &elem, Equation  &result, void (*func)(double &, double))
{
    std::vector<std::pair<double, double>> &vec = result.coefs_;
    auto it = std::find_if(begin(vec), end(vec),
        [&elem](const std::pair<double, double>& x) { return isEqual(x.first, elem.first); });
    if (it != end(vec))
        func(it->second, elem.second);
    else {
        double newValue = 0.;
        func(newValue, elem.second);
        vec.push_back(std::make_pair(elem.first, newValue));
    }
}


static void shrinkEquation(Equation &result){
    std::list<std::size_t> forRemove{};

    for (std::size_t i = 0; i < result.coefs_.size(); ++i){
        if (isEqual(result.coefs_[i].second, 0.))
            forRemove.push_front(i);
    }

    for (auto it = begin(forRemove); it != end(forRemove); ++it){
        result.coefs_.erase(begin(result.coefs_) + *it);
    }
}

Equation operator + (Equation q1, Equation q2)
{
    if (q1.varName_ != q2.varName_ && q1.varName_ != 0 && q2.varName_ != 0){
        std::stringstream ss;
        ss << std::string(ERR_DIFF_VAR) << q1.varName_ << std::string(" + ") << q2.varName_;
        throw std::logic_error(ss.str());
    }

    Equation    result;
    for (const auto &elem : q1.coefs_)
        checkDegree(elem, result, add);
    for (const auto &elem : q2.coefs_)
        checkDegree(elem, result, add);

    shrinkEquation(result);

    result.varName_ = std::max(q1.varName_, q2.varName_);
    return result;
}


Equation operator - (Equation q1, Equation q2)
{
    if (q1.varName_ != q2.varName_ && q1.varName_ != 0 && q2.varName_ != 0){
        std::stringstream ss;
        ss << std::string(ERR_DIFF_VAR) << q1.varName_ << std::string(" - ") << q2.varName_;
        throw std::logic_error(ss.str());
    }

    Equation    result;
    for (const auto &elem : q1.coefs_)
        checkDegree(elem, result, add);
    for (const auto &elem : q2.coefs_)
        checkDegree(elem, result, subt);

    shrinkEquation(result);
    
    result.varName_ = std::max(q1.varName_, q2.varName_);
    return result;
}


Equation operator * (Equation q1, Equation q2)
{
    if (q1.varName_ != q2.varName_ && q1.varName_ != 0 && q2.varName_ != 0){
        std::stringstream ss;
        ss << std::string(ERR_DIFF_VAR) << q1.varName_ << std::string(" * ") << q2.varName_;
        throw std::logic_error(ss.str());
    }

    Equation    result;
    for (const auto &elem_l : q1.coefs_){
        for (const auto &elem_r : q2.coefs_){
            std::pair<double, double> new_pair = std::make_pair(elem_l.first + elem_r.first, elem_l.second * elem_r.second);
            checkDegree(new_pair, result, add);
        }
    }

    shrinkEquation(result);

    result.varName_ = std::max(q1.varName_, q2.varName_);
    return result;
}


Equation operator / (Equation q1, Equation q2)
{
    if (q1.varName_ != q2.varName_ && q1.varName_ != 0 && q2.varName_ != 0){
        std::stringstream ss;
        ss << std::string(ERR_DIFF_VAR) << q1.varName_ << std::string(" / ") << q2.varName_;
        throw std::logic_error(ss.str());
    }

    bool isFirst = true;
    double props = 0.; 
    for (const auto& elem : q1.coefs_)
    {
        auto it = std::find_if(begin(q2.coefs_), end(q2.coefs_),
            [&elem](const std::pair<double, double>& x){ return isEqual(elem.first, x.first); });
        if (it == end(q2.coefs_) || std::fpclassify(it->second) == FP_ZERO)
            throw std::fpclassify(it->second) == FP_ZERO ? std::logic_error(ERR_DIV_BY_ZERO) : std::logic_error(ERR_COMPLEX_COMPUTE);
        if (isFirst){
            props = elem.second / it->second;
            isFirst = false;
        } else if (!isEqual(elem.second / it->second, props)){
            throw std::logic_error(ERR_COMPLEX_COMPUTE);
        }
    }

    isFirst = true;
    for (const auto& elem : q2.coefs_)
    {
        auto it = std::find_if(begin(q1.coefs_), end(q1.coefs_),
            [&elem](const std::pair<double, double> &x){ return isEqual(elem.first, x.first); });
        if (it == end(q1.coefs_) || std::fpclassify(elem.second) == FP_ZERO)
            throw std::fpclassify(elem.second) == FP_ZERO ? std::logic_error(ERR_DIV_BY_ZERO) : std::logic_error(ERR_COMPLEX_COMPUTE);
        if (isFirst){
            props = it->second / elem.second;
            isFirst = false;
        } else if (!isEqual(it->second  / elem.second, props)){
            throw std::logic_error(ERR_COMPLEX_COMPUTE);
        }
    }
    
    std::vector<std::pair<double, double>>  ans;
    ans.push_back(std::make_pair(0., props));
    Equation result(ans, std::max(q1.varName_, q2.varName_));

    shrinkEquation(result);

    return result;
}


std::ostream& operator << (std::ostream &os, const Equation &e)
{
    std::string varName;
    varName.assign(1, !e.varName_ ? 'X' : e.varName_);
    bool isFirst = true;
    os << '[';
    for (const auto &elem : e.coefs_)
    {
        if (isFirst){
            isFirst = false;
        } else {
            os << std::string(" + ");
        }
	    os << elem.second << std::string(" * ") << varName << std::string("^(") << elem.first << std::string(")");
    }
    return os << ']';
}
