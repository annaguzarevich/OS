#pragma once

#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif  // VECTOR_EXPORTS

#include "pch.h"
#include "Number.h"



class VECTOR_API Vector
{
public:
    Vector() = default;
    Vector(Number x_, Number y_);

    Number PolarRadius();
    Number PolarAngle();

    void VectorAddition(Vector addend);

    Number GetX();
    Number GetY();

private:
    Number x;
    Number y;
};

const Vector ZeroZero(Number(0), Number(0));
const Vector OneOne(Number(1), Number(1));


