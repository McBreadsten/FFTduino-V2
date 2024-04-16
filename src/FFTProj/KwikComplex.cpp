#include "Arduino.h"
#include "KwikComplex.h"

//boolean ops
bool KwikComplex::operator==(const KwikComplex &c) const {
  return (re == c.re) && (im == c.im);
}
bool KwikComplex::operator!=(const KwikComplex &c) const {
  return (re != c.re) || (im != c.im);  //demorgans law
}
//Math ops
KwikComplex KwikComplex::operator+(const KwikComplex &c) const {
  return KwikComplex(re + c.re, im + c.im);
}
KwikComplex KwikComplex::operator-(const KwikComplex &c) const {
  return KwikComplex(re - c.re, im - c.im);
}
KwikComplex KwikComplex::operator*(const KwikComplex &c) const {
  double r = re * c.re - im * c.im;
  double i = re * c.im + im * c.re;
  return KwikComplex(r, i);
}
KwikComplex KwikComplex::operator/(const KwikComplex &c) const {
  double q = 1.0 / (c.re * c.re + c.im * c.im);
  double r = (re * c.re + im * c.im) * q;
  double i = (im * c.re - re * c.im) * q;
  return KwikComplex(r, i);
}
//Self referencing ops
KwikComplex &KwikComplex::operator+=(const KwikComplex &c) {
  re += c.re;
  im += c.im;
  return *this;
}
KwikComplex &KwikComplex::operator-=(const KwikComplex &c) {
  re -= c.re;
  im -= c.im;
  return *this;
}
KwikComplex &KwikComplex::operator*=(const KwikComplex &c) {
  double r = re * c.re - im * c.im;
  double i = re * c.im + im * c.re;
  re = r;
  im = i;
  return *this;
}
KwikComplex &KwikComplex::operator/=(const KwikComplex &c) {
  double q = 1.0 / (c.re * c.re + c.im * c.im);
  double r = (re * c.re + im * c.im) * q;
  double i = (im * c.re - re * c.im) * q;
  re = r;
  im = i;
  return *this;
}
//special funny
KwikComplex KwikComplex::complexExp() const {
  double a = exp(re);
  return KwikComplex(a * cos(im), a * sin(im));
}
KwikComplex KwikComplex::complexSin() const {
  return KwikComplex(sin(re) * cosh(im), sinh(im) * cos(re));
}
KwikComplex KwikComplex::complexCos() const {
  return KwikComplex(cos(re) * cosh(im), -1.0 * sin(re) * sinh(im));
}