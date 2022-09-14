#include <iostream>
#include "Vector.h"

int main()
{
    Vector vect1(Number(3), Number(6));
    Vector vect2(Number(1), Number(2));

    std::cout << vect1.GetX().GetValue()<<std::endl;
    vect1.VectorAddition(vect2);
    std::cout << vect1.GetX().GetValue() << std::endl;
    vect2.VectorAddition(OneOne);
    std::cout << vect2.GetY().GetValue() << std::endl;

    std::cout << vect1.PolarRadius().GetValue() << std::endl;
    std::cout << vect1.PolarAngle().GetValue() << std::endl;
}