#ifndef OUTLIER_H
#define OUTLIER_H

#include "Complex.hpp"
#include <stdio.h>

class Outlier
{
public:
  Outlier();
  Outlier(const char* inputFile, double anomalyThreshold);
  ~Outlier();
  void normalize();
  
protected:
  Complex* makeGeoposition(const char* line) const;
  double extractY(const char* line) const;
  double extractX(const char* line) const;
  bool threshold(Complex* coord) const;
  const char* readLine() const;
private:
  double _anomalyThreshold;
  FILE* inputFile;
  Complex* values;
  Complex* currentValue;
  size_t count;
};

#endif // OUTLIER_H
