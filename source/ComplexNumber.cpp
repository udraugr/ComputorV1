#include "Equation.hpp"


ComplexNumber operator + (ComplexNumber c1, ComplexNumber c2){
    return ComplexNumber(c1.a_ + c2.a_, c1.b_ + c2.b_);
}


ComplexNumber operator - (ComplexNumber c1, ComplexNumber c2){
    return ComplexNumber(c1.a_ + c2.a_, c1.b_ + c2.b_);
}


ComplexNumber operator + (ComplexNumber c1, double d2){
    return ComplexNumber(c1.a_ + d2, c1.b_);
}


ComplexNumber operator - (ComplexNumber c1, double d2){
    return ComplexNumber(c1.a_ - d2, c1.b_);
}

ComplexNumber operator * (ComplexNumber c1, double d2){
    return ComplexNumber(c1.a_ * d2, c1.b_ * d2);
}

ComplexNumber operator / (ComplexNumber c1, double d2){
    return ComplexNumber(c1.a_ / d2, c1.b_ / d2);
}

std::pair<ComplexNumber, ComplexNumber> doubleSqrt(double d){
    return complexSqrt(ComplexNumber(d, 0.));
}

std::pair<ComplexNumber, ComplexNumber>     complexSqrt(ComplexNumber c1){
    double length = std::sqrt(c1.a_ * c1.a_ + c1.b_ * c1.b_);
    double angle = 0.;
    if (c1.a_ > 0. && c1.b_ > 0.){
        angle = std::atan(c1.b_ / c1.a_);
    } else if (c1.a_ > 0. && c1.b_ < 0.) {
        angle = std::atan(-c1.b_ / c1.a_);
    } else if (c1.a_ < 0. && c1.b_ > 0.) {
        angle = M_PI - std::atan(-c1.b_ / c1.a_);
    } else if (c1.a_ < 0. && c1.b_ < 0.) {
        angle = -M_PI + std::atan(-c1.b_ / c1.a_);
    } else if (isEqual(c1.a_, 0) && c1.b_ > 0.) {
        angle = M_PI / 2.;
    } else if (isEqual(c1.a_, 0) && c1.b_ < 0.) {
        angle = -M_PI / 2.;
    } else if (c1.a_ > 0. && isEqual(c1.b_, 0)) {
        angle = 0.;
    } else if (c1.a_ < 0. && isEqual(c1.b_, 0)) {
        angle = M_PI;
    }
    ComplexNumber sqrt1 = ComplexNumber(length * cos(angle / 2.), length * sin(angle / 2.));
    ComplexNumber sqrt2 = ComplexNumber(length * cos((angle + 2. * M_PI) / 2.), length * sin((angle + 2. * M_PI) / 2.));
    return std::make_pair(sqrt1, sqrt2);
}

std::ostream& operator << (std::ostream &os, const ComplexNumber &c) {
    os << c.a_;
    if (!isEqual(c.b_, 0.)) {
        os << (c.b_ > 0. ?  " + " : " - ") << std::abs(c.b_) << " * i";
    }
    return os;
}

bool operator == (const ComplexNumber &c1, const ComplexNumber &c2) {
    return isEqual(c1.a_, c2.a_) && isEqual(c1.b_, c2.b_);
}