//Joey Atkinson, 3/28/2024
#include "Arduino.h"
#include "Math.h"
#include "Complex.h"
//Goal: perform a 1024 sample Radix-2 DIF FFT.
#define SAMPLING_FREQ 5000 //max frequency: 10Khz
#define NUM_OF_BINS 1024 //max num of bins (theoretically) 4,096, should be a power of two
#define INPUT_PIN A0 //must be an analog pin, check ATMEGA2560 datasheet for best pin

uint8_t delayUs; //sampling frequency delay, in microseconds
float samples[NUM_OF_BINS]; //stores a number 0 - 1023
void fft(float x[], const int size);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //needed to output sample
  while(!Serial){
    ; //wait for serial monitor to initialize
  }
  double delayTemp = (1 / SAMPLING_FREQ) * 1000000; //calculates number here to avoid truncation
  delayUs = (int) delayTemp; //the needed delay in the sampling frequency 
  delayUs -= 100; //subtract 100 microseconds to account for AnalogIn() delay

}

void loop() {
  // put your main code here, to run repeatedly:
  // sample the input signal
  for(uint8_t i = 0; i < NUM_OF_BINS; i++){
    samples[i] = analogRead(INPUT_PIN);
    delayMicroseconds(delayUs); //needed delay in order to sample at correct sampling frequency
  }
  fft(samples, NUM_OF_BINS);
  //TODO: calculate the radix-2 fft
  //ok so we have a list of samples... hmmmmm
}

void fft(float x[], const int size){
  //cooley tukey my hated
  
  if(size == 1)
    return x;
}


