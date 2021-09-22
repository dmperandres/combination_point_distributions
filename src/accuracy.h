//LIC

#ifndef ACCURACY_H
#define ACCURACY_H

#include <math.h>

//HEA

namespace _accuracy_ns {

  const int ACCURACY=6;
  const int ACCURACY_INT=int(powf(10,ACCURACY));

  float accuracy(float Value);
}
#endif
