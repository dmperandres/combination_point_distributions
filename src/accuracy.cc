//LIC

#include "accuracy.h"

//HEA

float _accuracy_ns::accuracy(float Value)
{
  Value*=_accuracy_ns::ACCURACY_INT;
  Value=round(Value);
  Value/=_accuracy_ns::ACCURACY_INT;

  return Value;
}
