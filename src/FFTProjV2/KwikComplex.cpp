//#include "Arduino.h"
#include "KwikComplex.h"

//boolean ops
bool KwikComplex::operator==(const KwikComplex &c) const {
  return (real == c.real) && (imag == c.imag);
}
bool KwikComplex::operator!=(const KwikComplex &c) const {
  return (real != c.real) || (imag != c.imag);  //demorgans law
}
//Math ops
KwikComplex KwikComplex::operator+(const KwikComplex &c) const {
  return KwikComplex(real + c.real, imag + c.imag);
}
KwikComplex KwikComplex::operator-(const KwikComplex &c) const {
  return KwikComplex(real - c.real, imag - c.imag);
}
KwikComplex KwikComplex::operator*(const KwikComplex &c) const {
  double r = real * c.real - imag * c.imag;
  double i = real * c.imag + imag * c.real;
  return KwikComplex(r, i);
}
KwikComplex KwikComplex::operator/(const KwikComplex &c) const {
  double q = 1.0 / (c.real * c.real + c.imag * c.imag);
  double r = (real * c.real + imag * c.imag) * q;
  double i = (imag * c.real - real * c.imag) * q;
  return KwikComplex(r, i);
}
//Self referencing ops
KwikComplex &KwikComplex::operator+=(const KwikComplex &c) {
  real += c.real;
  imag += c.imag;
  return *this;
}
KwikComplex &KwikComplex::operator-=(const KwikComplex &c) {
  real -= c.real;
  imag -= c.imag;
  return *this;
}
KwikComplex &KwikComplex::operator*=(const KwikComplex &c) {
  double r = real * c.real - imag * c.imag;
  double i = real * c.imag + imag * c.real;
  real = r;
  imag = i;
  return *this;
}
KwikComplex &KwikComplex::operator/=(const KwikComplex &c) {
  double q = 1.0 / (c.real * c.real + c.imag * c.imag);
  double r = (real * c.real + imag * c.imag) * q;
  double i = (imag * c.real - real * c.imag) * q;
  real = r;
  imag = i;
  return *this;
}
//special funny
KwikComplex KwikComplex::complexExp() const {
  //double a = exp(real);
  return KwikComplex(cos(imag), sin(imag));
}
KwikComplex KwikComplex::complexSin() const {
  return KwikComplex(sin(real) * cosh(imag), sinh(imag) * cos(real));
}
KwikComplex KwikComplex::complexCos() const {
  return KwikComplex(cos(real) * cosh(imag), -1.0 * sin(real) * sinh(imag));
}
