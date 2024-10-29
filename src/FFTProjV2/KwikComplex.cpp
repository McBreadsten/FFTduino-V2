//#include "Arduino.h"
#include "KwikComplex.h"

//boolean ops
bool KwikComplex::operator==(const KwikComplex &num) const {
  return (real == num.real) && (imag == num.imag);
}
bool KwikComplex::operator!=(const KwikComplex &num) const {
  return (real != num.real) || (imag != num.imag);  //demorgans law
}
//Math ops
KwikComplex KwikComplex::operator+(const KwikComplex &num) const {
  return KwikComplex(real + num.real, imag + num.imag);
}
KwikComplex KwikComplex::operator-(const KwikComplex &num) const {
  return KwikComplex(real - num.real, imag - num.imag);
}
KwikComplex KwikComplex::operator*(const KwikComplex &num) const {
  double r = real * num.real - imag * num.imag;
  double i = real * num.imag + imag * num.real;
  return KwikComplex(r, i);
}
KwikComplex KwikComplex::operator/(const KwikComplex &num) const {
  double q = 1.0 / (num.real * num.real + num.imag * num.imag);
  double r = (real * num.real + imag * num.imag) * q;
  double i = (imag * num.real - real * num.imag) * q;
  return KwikComplex(r, i);
}
//Self referencing ops
KwikComplex &KwikComplex::operator+=(const KwikComplex &num) {
  real += num.real;
  imag += num.imag;
  return *this;
}
KwikComplex &KwikComplex::operator-=(const KwikComplex &num) {
  real -= num.real;
  imag -= num.imag;
  return *this;
}
KwikComplex &KwikComplex::operator*=(const KwikComplex &num) {
  double r = real * num.real - imag * num.imag;
  double i = real * num.imag + imag * num.real;
  real = r;
  imag = i;
  return *this;
}
KwikComplex &KwikComplex::operator/=(const KwikComplex &num) {
  double inv = 1.0 / (num.real * num.real + num.imag * num.imag);
  double r = (real * num.real + imag * num.imag) * inv;
  double i = (imag * num.real - real * num.imag) * inv;
  real = r;
  imag = i;
  return *this;
}
//special funny
KwikComplex KwikComplex::complexExp() const {
  //double a = exp(real); outdated idea, used for scaling sin and cos
  return KwikComplex(cos(imag), sin(imag));
}
KwikComplex KwikComplex::complexSin() const {
  return KwikComplex(sin(real) * cosh(imag), sinh(imag) * cos(real));
}
KwikComplex KwikComplex::complexCos() const {
  return KwikComplex(cos(real) * cosh(imag), -1.0 * sin(real) * sinh(imag));
}
