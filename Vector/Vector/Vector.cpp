#include "pch.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector.h"


Vector::Vector(Number x_, Number y_) : x(x_), y(y_) {}

Number Vector::PolarRadius()
{
	return Number(sqrt(pow(x.GetValue(), 2) + pow(y.GetValue(), 2)));
}

Number Vector::PolarAngle()
{
	if (x.GetValue() == 0) {
		if (y.GetValue() >= 0) {
			return Number(M_PI / 2);
		}
		else {
			return Number(-M_PI / 2);
		}
	}

	else if (x.GetValue() < 0) {
		return Number(M_PI + atan(y.GetValue() / x.GetValue()));
	}

	else {
		return Number(atan(y.GetValue() / x.GetValue()));
	}
}

void Vector::VectorAddition(Vector addend) {
	x.Add(addend.x.GetValue());
	y.Add(addend.y.GetValue());
}

Number Vector::GetX()
{
	return x;
}

Number Vector::GetY()
{
	return y;
}