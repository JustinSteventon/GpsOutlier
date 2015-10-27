#include "Outlier.hpp"

int main(int argc, char *argv[])
{
  char* filename = argv[1];
  Outlier* outlier = new Outlier(filename, 1000);
  outlier->normalize();
  return 0;
}
