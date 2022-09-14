#include "Number.h"

Number::Number(double value_) : value(value_) {}

void Number::Add(double addend)
{
	value += addend;
}

void Number::Substract(double subrtahend)
{
	value -= subrtahend;
}

void Number::Multiply(double multiplier)
{
	value *= multiplier;
}

void Number::Divide(double divisor)
{
	value /= divisor;
}

double Number::GetValue()
{
	return value;
}

Number CreateNumber(double value)
{
	Number number(value);
	return number;
}