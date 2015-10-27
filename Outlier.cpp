#include "Outlier.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_VALUES 1024

Outlier::Outlier():
   inputFile(nullptr),
   count(0),
   _anomalyThreshold(0)
{
  values = new Complex[MAX_VALUES];
  currentValue = values;
}

Outlier::Outlier(const char* inputFileName, double anomalyThreshold):
  count(0){
  values = new Complex[MAX_VALUES];
  *currentValue = values[0];
  inputFile = fopen(inputFileName, "r");
  _anomalyThreshold = anomalyThreshold;
}

Outlier::~Outlier(){
  fclose(inputFile);
  delete [] values;
  inputFile = nullptr;
  values = nullptr;
  currentValue = nullptr;
}

const char* Outlier::readLine() const{
  const char* buf = new char[128];
  size_t lineSize = fread( (void*)buf, 128, 1, inputFile );
  return (lineSize == 0) ? 0 : buf;
}

double Outlier::extractX(const char* line) const{
  char buf[64];
  strncpy(buf, line, 64);
  char* pos = strchr(buf, ',');
  pos[0] = '\0';
  double d = atof(buf);
  return d;
}

double Outlier::extractY(const char* line) const{
  char* pos = strchr(line, ',');
  pos++;
  char buf[64];
  strncpy(buf, pos, 64);
  double d = atof(buf);
  return d;
}

Complex* Outlier::makeGeoposition(const char* line) const{
  double x = extractX(line);
  double y = extractY(line);
  delete [] line;
  return new Complex(x,y);
}

bool Outlier::threshold(Complex* coord) const{
  double delta = coord->delta(values[count - 1]);
  return delta == _anomalyThreshold;
}

void Outlier::normalize(){
    const char* line = nullptr;
  while((line = readLine()) != nullptr && count < _anomalyThreshold){
    Complex* geoCoord = makeGeoposition(line);
    *currentValue = *geoCoord;
    if(threshold(currentValue)){
        count++;
        printf("x:%d, y:%d", currentValue->x(), currentValue->y());
    }
    delete geoCoord;
  }
}

