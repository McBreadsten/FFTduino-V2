#include "Complex.h"
//#include <stdlib.h>
#define NUM_OF_BINS 256
#define FREQ 10
//I couldnt be fucked to make my own
//complex number library
void subFFT(Complex arr[], Complex out[], int n, int step){
  if(step < n){
    subFFT(out, arr, n, step*2);
    subFFT(out + step, arr + step, n, step * 2);
  }
  for(int i = 0; i < n; i += 2 * step){
    Complex t;
    t.setReal(1);
    t.setImag(-1 * PI * i / n);
    t = t.c_exp();
    t *= out[i + step];
    arr[i / 2] = out[i] + t;
    arr[(i + n)/2] = out[i] - t;
    //r = 1
    //i = 2pikn/N
  }

}
void FFT(Complex arr[], int n){
  int i;
  Complex out[n];
  for(i = 0; i < n; i++)
    out[i] = arr[i];
  subFFT(arr, out, n, 1);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}
void show(const char * s, Complex buf[]) {
	Serial.print(s);
	for (int i = 0; i < 8; i++)
		if (!buf[i].imag()){
			Serial.print((double)buf[i].real());
      Serial.print(" ,");
    }else{
      Serial.print("(");
			Serial.print((double)buf[i].real());
      Serial.print(" , ");
      Serial.print((double)buf[i].imag());
      Serial.print("), ");
    }
}
void loop() {
  // put your main code here, to run repeatedly:
  Complex arr[NUM_OF_BINS];
  //generate sine wave of any frequency relative to the sampling rate
  double count = 0;
  double inc = 2 * PI * ((double)FREQ / (double)NUM_OF_BINS);
  Serial.println(inc);
  //inc /= (double)NUM_OF_BINS;
  for(int i = 0; i < NUM_OF_BINS; i++){

    arr[i].set(sin(count), 0);
    count += inc;
  }
  //show("Data: ", arr);
  for(int i = 0; i < NUM_OF_BINS; i++){
    Serial.print("Marker:");
    Serial.print(i);
    Serial.print(", Data:");
    Serial.println(arr[i].real());
    delay(10);
  }
  FFT(arr, NUM_OF_BINS);
  for(int i = 0; i < NUM_OF_BINS; i++){
    Serial.print("Marker:");
    Serial.print(i);
    Serial.print(", Data:");
    Serial.println(arr[i].real());
    delay(10);
  }
  //show("\nFFT : ", arr);
  while(1){
    ;
  }


}
