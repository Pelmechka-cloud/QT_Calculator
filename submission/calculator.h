#pragma once

using Number = double;

class Calculator {
public:
    void Set(Number number);
    Number GetNumber() const;

    void Add(Number operand);
    void Sub(Number operand);
    void Mul(Number operand);
    void Div(Number operand);
    void Pow(Number operand);

private:
    Number number_ = 0.0;
};