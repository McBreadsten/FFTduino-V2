#include "KwikComplex.h"
#define NUM_OF_BINS 1024
#define AGS false
#define FREQ 0
#if AGS
#define FREQ 10
#endif
//I got bothered, so i made my own
//complex number library
KwikComplex arr[NUM_OF_BINS];
void subFFT(KwikComplex arr[], KwikComplex out[], unsigned short int n, unsigned short int step) {
  unsigned short int i;
  if (step < n) {
    subFFT(out, arr, n, step * 2);
    subFFT(out + step, arr + step, n, step * 2);
  }
  for (i = 0; i < n; i += 2 * step) {
    KwikComplex t;
    t.set(1, -1 * PI * i / n);
    t = t.complexExp();
    t *= out[i + step];
    arr[i / 2] = out[i] + t;
    arr[(i + n) / 2] = out[i] - t;
    //r = 1
    //i = 2pikn/N
  }
}
void FFT(KwikComplex arr[], unsigned short int n) {
  uint8_t i;
  KwikComplex out[n];
  for (i = 0; i < n; i++)
    out[i] = arr[i];
  subFFT(arr, out, n, 1);
}
void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
}
/*void show(const char* s, KwikComplex buf[]) {
  Serial.print(s);
  for (int i = 0; i < 8; i++)
    if (!buf[i].imag()) {
      Serial.print((double)buf[i].real());
      Serial.print(" ,");
    } else {
      Serial.print("(");
      Serial.print((double)buf[i].real());
      Serial.print(" , ");
      Serial.print((double)buf[i].imag());
      Serial.print("), ");
    }
}
*/
void loop() {
  // put your main code here, to run repeatedly:
  //generate sine wave of any frequency relative to the sampling rate
  uint8_t i;
  /*if (AGS) {
    double count = 0;
    double inc = 2 * PI * ((float)FREQ / (double)NUM_OF_BINS);
    //Serial.println(inc);
    //inc /= (double)NUM_OF_BINS;
    for (i = 0; i < NUM_OF_BINS; i++) {

      arr[i].set(sin(count), 0);
      count += inc;
    }
  } 
  else {  //no auto generated sinewave :(
  }*/
  FFT(arr, NUM_OF_BINS);
  //show("Data: ", arr);
  /*for (int i = 0; i < NUM_OF_BINS; i++) {
    Serial.print("Marker:");
    Serial.print(i);
    Serial.print(", Data:");
    Serial.println(arr[i].real());
    delay(10);
  }
  FFT(arr, NUM_OF_BINS);
  for (int i = 0; i < NUM_OF_BINS; i++) {
    Serial.print("Marker:");
    Serial.print(i);
    Serial.print(", Data:");
    Serial.println(arr[i].real());
    delay(10);
  }*/
  //show("\nFFT : ", arr);
  //while (1) {
  //  ;
  //}
}
//FUCKING NINE BYTES ARE YOU KIDDING ME
