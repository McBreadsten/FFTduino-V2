#include "KwikComplex.h"
//ballin
#define NUM_OF_BINS 512
#define INPUT_PIN A0
//kwikComplex has 8 bytes for a size
KwikComplex* array;
int bitReverse(int n, int bits) {
  int count = bits - 1;
  int reversedBits = n;
  n >>= 1;
  while (n > 0) {
    reversedBits = (reversedBits << 1) | (n & 1);
    count--;
    n >>= 1;
  }
  int returnValue = ((reversedBits << count)) & ((1 << bits) - 1);
  return returnValue;  //trust me this works
}
void radix2fft(KwikComplex arr[]) {
  int bits = (int)(log(NUM_OF_BINS) / log(2.0));
  int i;
  int swapPos;
  KwikComplex temp;
  for (i = 1; i < NUM_OF_BINS / 2; i++) {
    swapPos = bitReverse(i, bits);
    temp = arr[i];
    arr[i] = arr[swapPos];
    arr[swapPos] = temp;
  }
  int N, j, k, evenIdx, oddIdx;
  KwikComplex even;
  KwikComplex odd;
  double twiddle;
  for (N = 2; N <= NUM_OF_BINS; N <<= 1) {
    for (j = 0; j < NUM_OF_BINS; j += N) {
      for (k = 0; k < N / 2; k++) {
        evenIdx = j + k;
        oddIdx = evenIdx + (N / 2);
        even = arr[evenIdx];
        odd = arr[oddIdx];
        twiddle = (-2 * PI * k) / (double)N;
        temp.set(0, twiddle);

        temp = temp.complexExp();
        temp *= odd;
        arr[evenIdx] = even + temp;
        arr[oddIdx] = even - temp;
      }
    }
  }
}
void hamming(KwikComplex arr[]){
  //w(n) = 0.54 - 0.46 * cos(2PI * [n/N]), 0 <= n <= N
  int i;
  for(i = 0; i < NUM_OF_BINS; i++)
    arr[i] *= 0.54 - 0.46 * cos(2.0 * PI * (i / NUM_OF_BINS));
}
void setup() {
  Serial.begin(115200);
  array = (KwikComplex*)malloc(NUM_OF_BINS*sizeof(KwikComplex));
  if(array == NULL){
    Serial.println("Out of memory!");
    while(true)
    {
      ;
    }
  }
}
void loop() {
  int i;
  double temp;
  for(i = 0; i < NUM_OF_BINS; i++){
    //ok so we get a number 0 - 1023
    //transform it into one from -1 to 1
    temp = 1.0 * analogRead(INPUT_PIN);
    temp -= 511.5;
    temp /= 511.5;
    array[i] = temp;
  }
  hamming(array);
  radix2fft(array);
  //TODO: map the bins to real world frequencies
}