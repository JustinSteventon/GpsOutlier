#include "Complex.hpp"
#include <math.h>

Complex::Complex()
  :_x(0.0),
   _y(0.0){}

Complex::Complex(double x, double y)
  :_x(x),
   _y(y){  
}

Complex::~Complex(){}


Complex::Complex(const Complex& obj){
  _x = obj._x;
  _y = obj._y;
}

double Complex::x() const{
    return _x;
}
double Complex::y() const{
    return _y;
}

const double  Complex::delta(const Complex &c) const{
  double d1 = sqrt(_x*_x + _y*_y);
  double d2 = sqrt(c._x*c._x + c._y*c._y);
  return fabs(d1-d2);
}


