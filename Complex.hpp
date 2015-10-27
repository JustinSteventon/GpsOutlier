
#ifndef COMPLEX_HPP
#define COMPLEX_HPP

class Complex
{
public:
  Complex();
  Complex(double x, double y);
  Complex(const Complex& obj);
  virtual ~Complex(); 
  const double delta(const Complex &c) const;
  double x() const;
  double y() const;
private:
  double _x;
  double _y;
};

#endif
