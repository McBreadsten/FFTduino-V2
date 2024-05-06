#include "KwikComplex.h"
//ballin
#define NUM_OF_BINS 512
#define INPUT_PIN A0
#define SAMPLING_FREQ 512
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
  return returnValue;
};

void radix2fft(KwikComplex arr[]) {
  int bits = (log(NUM_OF_BINS) / log(2.0));
  unsigned int i;
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
};

void hamming(KwikComplex arr[]) {
  //w(n) = 0.54 - 0.46 * cos(2PI * [n/N]), 0 <= n <= N
  int i;
  for (i = 0; i < NUM_OF_BINS; i++)
    arr[i].setReal(arr[i].real() * (0.54 - 0.46 * cos(2.0 * PI * i / (1.0 * NUM_OF_BINS))));
};
void fftShift(KwikComplex arr[]) {
  KwikComplex temp;
  int shiftAdd = NUM_OF_BINS / 2;
  for (int i = 0; i < shiftAdd; i++) {
    temp = arr[i];
    arr[i] = arr[i + shiftAdd];
    arr[i + shiftAdd] = temp;
  }
};

void setup() {
  Serial.begin(115200);
  delay(100);
  array = (KwikComplex*)malloc(NUM_OF_BINS * sizeof(KwikComplex));
  if (array == NULL) {
    Serial.println("Out of memory!");
    while (true) {
      ;
    }
  } else {
    Serial.println("Memory OK! size: " + (NUM_OF_BINS * sizeof(KwikComplex)));
  }
}

void loop() {
  int i;
  double temp;
  long delayTimeMicros = 1 / SAMPLING_FREQ;
  delayTimeMicros *= 100;
  long long currentTime = 0;
  //time for each sample (uS) = [(1/samplingFreq) * 10^6]
  for (i = 0; i < NUM_OF_BINS; i++) {
    currentTime = micros();
    //ok so we get a number 0 - 1023
    //transform it into one from -1 to 1
    //decided to do outside of sampling loop for SPEED
    array[i].setReal(1.0 * analogRead(INPUT_PIN));
    //cool! now we need to delay until the next sample
    currentTime = micros() - currentTime;
    currentTime = delayTimeMicros - currentTime;
    //finalDelay = delayTime - delta(currentTime)
    if (currentTime <= 0) {
      Serial.println("TOO FAST!");
    } else {
      delayMicroseconds(currentTime);
    }
  }

  for (i = 0; i < NUM_OF_BINS; i++) {
    array[i].setReal(array[i].real() - 511.5);
    array[i].setReal(array[i].real() / 511.5);
    array[i].setImag(0);
    Serial.println(array[i].real());
  }
  hamming(array);
  Serial.println("Radix 2 FFT, Joey Atkinson, May 5th, 2024");
  radix2fft(array);
  fftShift(array);

  //RWF at nth bin = n * Fs / N
  //remember, (N / 2 + 1) -> (N - 1) is the comp cong of first half
  double realWorldFreq = 0;
  double largestNum = 0;
  double freqOfLargest = 0;

  for (i = 0; i < NUM_OF_BINS; i++) {
    Serial.print("FFT_Magnitude:");
    Serial.print(sqrt((array[i].real() * array[i].real()) + (array[i].imag() * array[i].imag())));
    Serial.print("Frequency: ");
    Serial.println((realWorldFreq * (double)SAMPLING_FREQ / (double)NUM_OF_BINS) - (SAMPLING_FREQ / 2));
    //Serial.println((realWorldFreq * (double)SAMPLING_FREQ / (double)NUM_OF_BINS));
    realWorldFreq++;
    //delay(1);
    //if (sqrt((array[i].real() * array[i].real()) + (array[i].imag() * array[i].imag())) > largestNum && i > 261 && i < 500) {
    //largestNum = sqrt((array[i].real() * array[i].real()) + (array[i].imag() * array[i].imag()));
    //freqOfLargest = realWorldFreq * (double)SAMPLING_FREQ / (double)NUM_OF_BINS;
    //}
  }
  //below is for testing:
  //Serial.print("Largest Magnitude: ");
  //Serial.print(largestNum);
  //Serial.print(", at Frequency: ");
  //Serial.println(freqOfLargest);
  while (true) {
    ;  //loops indefinitely
  }
}