#ifndef KwikComplex_h
#define KwikComplex_h
#include "Arduino.h"

class KwikComplex {
public:
  KwikComplex(const double r = 0, const double i = 0)
    : real(r), imag(i){};
  KwikComplex(const KwikComplex& c)
    : real(c.real), imag(c.imag){};


  void set(const double r, const double i) {
    real = r;
    imag = i;
  };
  void setReal(const double r) {
    real = r;
  };
  void setImag(const double i) {
    imag = i;
  };
  float real() const {
    return real;
  };
  float imag() const {
    return imag;
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
  double real;
  double imag;
};
#endif
