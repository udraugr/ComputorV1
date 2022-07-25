#include "Equation.hpp"

std::pair<ComplexNumber, ComplexNumber>        Equation::Solve(){
    double coefs[3] = {0.l, 0.l, 0.l};
    long long int maxDegree = -1;
    for (std::size_t i = 0; i < coefs_.size(); ++i) {
        if (static_cast<long long int>(coefs_[i].first) > maxDegree){
            maxDegree = static_cast<long long int>(coefs_[i].first);
        }
        coefs[static_cast<std::size_t>(coefs_[i].first)] = coefs_[i].second;
    }

    if (maxDegree > 2){
        std::stringstream ss;
        ss << ERR_BIG_DEGREE << maxDegree;
        throw std::logic_error(ss.str());
    }

    std::pair<ComplexNumber, ComplexNumber>     ans;

    if (maxDegree == -1){
        throw std::logic_error(ERR_DEGENERATED);
    } else if (maxDegree == 0){
        throw std::logic_error(ERR_NO_SOLUTION);
    } else if (maxDegree == 1){
        ans = std::make_pair(ComplexNumber(-coefs[0] / coefs[1], 0.), ComplexNumber(-coefs[0] / coefs[1], 0.));
    } else {
        ComplexNumber discriminant = ComplexNumber(coefs[1] * coefs[1] - 4. * coefs[2] * coefs[0], 0.);
        auto sqrts = complexSqrt(discriminant);
        ComplexNumber ans1 = (ComplexNumber(-coefs[1], 0.) + sqrts.first) / (coefs[2] * 2.);
        ComplexNumber ans2 = (ComplexNumber(-coefs[1], 0.) + sqrts.second) / (coefs[2] * 2.);
        ans = std::make_pair(ans1, ans2);
    }
    return ans;

}