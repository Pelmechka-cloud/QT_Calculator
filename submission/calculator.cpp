#include "calculator.h"
#include <cmath>

void Calculator::Set(Number number) {
    number_ = number;
}

Number Calculator::GetNumber() const {
    return number_;
}

void Calculator::Add(Number operand) {
    number_ += operand;
}

void Calculator::Sub(Number operand) {
    number_ -= operand;
}

void Calculator::Mul(Number operand) {
    number_ *= operand;
}

void Calculator::Div(Number operand) {
    number_ /= operand;
}

void Calculator::Pow(Number operand) {
    number_ = std::pow(number_, operand);
}