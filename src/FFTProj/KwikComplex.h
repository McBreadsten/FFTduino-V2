#ifndef KwikComplex_h
#define KwikComplex_h

#include "Arduino.h"

class KwikComplex {
public:
  KwikComplex(const double r = 0, const double i = 0)
    : re(r), im(i){};
  KwikComplex(const KwikComplex& c)
    : re(c.re), im(c.im){};


  void set(const double r, const double i) {
    re = r;
    im = i;
  };
  void setReal(const double r) {
    re = r;
  };
  void setImag(const double i) {
    im = i;
  };
  float real() const {
    return re;
  };
  float imag() const {
    return im;
  };
  //boolean operators
  bool operator==(const KwikComplex&) const;
  bool operator!=(const KwikComplex&) const;
  //Standard operators on math operators
  KwikComplex operator+(const KwikComplex&) const;
  KwikComplex operator-(const KwikComplex&) const;
  KwikComplex operator*(const KwikComplex&) const;
  KwikComplex operator/(const KwikComplex&) const;

  KwikComplex& operator+=(const KwikComplex&);
  KwikComplex& operator-=(const KwikComplex&);
  KwikComplex& operator*=(const KwikComplex&);
  KwikComplex& operator/=(const KwikComplex&);

  KwikComplex complexExp() const;
  KwikComplex complexSin() const;
  KwikComplex complexCos() const;

private:
  double re;
  double im;
};
#endif