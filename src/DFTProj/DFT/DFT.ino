//Goal: see what a DFT looks like
#define SAMPLING_FREQ 4000  //max frequency: 10Khz
#define NUM_OF_BINS 512    //max num of bins (theoretically) 4,096, should be a power of two
#define INPUT_PIN A0        //must be an analog pin, check ATMEGA2560 datasheet for best pin
#define AGS true            //auto generate sinewave, for testing
#if AGS                     //checks if AGS is true
#define AMP 40             //amplitude, is a percentage of 100
#define MIDLINE 512         //middle of the sinewave, normally 512
#define FREQ 300             //frequency of the sinewave
#endif
struct complex samples[(int)BIN_SIZE];

struct complex {  //used for complex numbers
  float r;
  float i;
};
struct complex cSum(struct complex c1, struct complex c2){
  //(a + bi) + (c + di) = (a + c) + (b + d)i
  struct complex res;
  res.r = c1.r + c2.r;
  res.i = c1.i + c2.i;
  return res;
}
struct complex cDiff(struct complex c1, struct complex c2){
  //(a + bi) - (c + di) = (a - c) + (b - d)i
  struct complex res;
  res.r = c1.r - c2.r;
  res.i = c1.i - c2.i;
  return res; 
}
struct complex cProd(struct complex c1, struct complex c2){
  //(a + bi)(c + di) = (ac - bd) + (ad + bc)i
  struct complex res;
  res.r = (c1.r * c2.r) - (c1.i * c2.i);
  res.i = (c1.r * c2.i) + (c1.i + c2.r);
  return res;
}
struct complex cQuot(struct complex c1, struct complex c2){
  //(a + bi) / (c + di) = [(ac + bd) + (bc - ad)i] / [(c*c) + (d * d)]
  struct complex res;
  res.r = (c1.r * c2.r) + (c1.i * c2.i);
  res.r /= (c2.r * c2.r) + (c2.i * c2.i);
  res.i = (c1.i * c2.r) - (c1.r * c2.i);
  res.i /= (c2.r * c2.r) + (c2.i * c2.i);
  return res;
}
void generateSine(struct complex array[]){
  //generate a sine wave for the program to sample

}
void DFT(struct complex arr[]){
  int i, j;
  struct complex X_k[NUM_OF_BINS]; //holds the end result
  for(i = 0; i < (int)NUM_OF_BINS; i++){ //for the dft, iterate through each X[k]
    struct complex xTemp;
    xTemp.r = 0;
    xTemp.i = 0;
    for(j = 0; j < (int)NUM_OF_BINS; j++){ //take the sum nerd
      xTemp = cSum(cProd(arr[j], calcTwiddle(i, NUM_OF_BINS, j)), xTemp);
    }
    X_k[i] = xTemp;
  }

}
struct complex calcTwiddle(int k, int N, int n){
  //e^-i*2pi*n*(k/N) = -[cos(2*pi*n*k/N)) + i * sin(2 * pi * n * k /N)
  double twiddleExp = 2 * PI * n * k;
  twiddleExp /= (double) N;
  struct complex res;
  res.r = -1 * cos(twiddleExp);
  res.i = -1 * sin(twiddleExp);
  return res;
}

void setup() {
  // put your setup code here, to run once:
    // put your setup code here, to run once:
  Serial.begin(115200);  //needed to output sample
  Serial.println("Hello, world!");
  pinMode(LED_BUILTIN, OUTPUT);
  double delayTemp = (1 / SAMPLING_FREQ) * 1000000;  //calculates number here to avoid truncation
  delayUs = (int)delayTemp;                          //the needed delay in the sampling frequency
  delayUs -= 100;                                    //subtract 100 microseconds to account for AnalogIn() delay
}

void loop() {
  // put your main code here, to run repeatedly:
  // sample the input signal
  bool isAGS = AGS;
  if (!isAGS) {
    for (uint8_t i = 0; i < NUM_OF_BINS; i++) {
      samples[i].r = analogRead(INPUT_PIN);
      samples[i].i = 0;
      delayMicroseconds(delayUs);  //needed delay in order to sample at correct sampling frequency
    }
  } else {  //generate a sinewave with phase, frequency, and midline defined
    //we have a sampling frequency, and we have a requested phase
    //if the frequency of the sinewave matched the sampling frequency, we would get only
    //samples at the start of every cycle. if it was half the sampling frequency, it would be half of a cycle.
    //first, we determine the ratio between the artificial frequency and the sampling frequency.
    double sampleRatio = (double)FREQ / (double)SAMPLING_FREQ;
    double sampleFreq = 2 * PI * sampleRatio;  //incrementation amount (in rad)

    double maxAmpPercentage = (double)AMP / 100;
    bool isClip = false;
    for (int i = 0; i < NUM_OF_BINS; i++) {
      if (incCount >= (2 * PI))
        incCount -= (2 * PI);

      samples[i].r = sin(incCount) * 512 * maxAmpPercentage;
      samples[i].r += MIDLINE;
      samples[i].i = 0;
      if (samples[i].r < 0)
        isClip = true;
      incCount += sampleFreq;
    }
    if (isClip);
      //Serial.println("Clipping detected! results may be skewed.");
  }
  DFT(samples);
  //ok now we have the FFT in the samples array, now what?
  //TODO: test the fft.
  for (int i = 0; i < NUM_OF_BINS; i++) {
    Serial.println(samples[i].r);
  //Serial.println(i);
    delay(10);
  }

}
