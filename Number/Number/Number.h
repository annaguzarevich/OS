#pragma once

class Number
{
public:
	explicit Number(double value_);

	void Add(double addend);
	void Substract(double subrtahend);
	void Multiply(double multiplier);
	void Divide(double divisor);

	double GetValue();

private:
	double value;
};

const Number Zero(0);
const Number One(1);

Number CreateNumber(double value);

