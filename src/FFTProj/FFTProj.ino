//Joey Atkinson, 3/28/2024
//Goal: perform a Radix-2 DIF FFT.
#define SAMPLING_FREQ 4000  //max frequency: 10Khz
#define NUM_OF_BINS 512     //max num of bins (theoretically) 4,096, should be a power of two
#define INPUT_PIN A0        //must be an analog pin, check ATMEGA2560 datasheet for best pin
#define AGS true            //auto generate sinewave, for testing
#if AGS                     //checks if AGS is true
#define AMP 10              //amplitude, is a percentage of 100
#define MIDLINE 512         //middle of the sinewave, normally 512
#define FREQ 10             //frequency of the sinewave
#endif
struct complex {  //used for complex numbers
  float r;
  float i;
};
//look up table should be N/2 for twiddle factors
struct complex wLookup[(NUM_OF_BINS / 2)];

uint8_t delayUs;  //sampling frequency delay, in microseconds
struct complex samples[NUM_OF_BINS];

//calculates the sum of two complex numbers
struct complex cSum(struct complex c1, struct complex c2) {
  struct complex res;
  res.r = c1.r + c2.r;
  res.i = c1.i + c2.i;
  return res;
}

//calculates the difference of two complex numbers
struct complex cDiff(struct complex c1, struct complex c2) {
  struct complex res;
  res.r = c1.r - c2.r;
  res.i = c1.i - c2.i;
  return res;
}

//calculates the product of two complex numbers
struct complex cProd(struct complex c1, struct complex c2) {
  struct complex res;
  res.r = (c1.r * c2.r) - (c1.i * c2.i);
  res.i = (c1.r * c2.i) + (c1.i * c2.r);
  return res;
}

//rearranges the factors to properly lineup with the output fft
//could use bit manipulation to do this, but i decided to go with
//an alternative algorithm
void shuffle(uint8_t n, int offset) {
  if (n > 2) {
    int i, m = n / 2;
    struct complex tempArray[m];
    for (i = 0; i < m; i++)
      tempArray[i] = samples[i * 2 + offset + 1];
    for (i = 0; i < m; i++)
      samples[i + offset] = samples[i * 2 + offset];
    for (i = 0; i < m; i++)
      samples[i + offset + m] = tempArray[i];
  }
}

void radix2fft() {
  fftSub(NUM_OF_BINS, 0);
  for (int i = 0; i < NUM_OF_BINS; i++) {
    samples[i].r = samples[i].r / ((double)NUM_OF_BINS * 2.0);
    samples[i].i = samples[i].i / ((double)NUM_OF_BINS * 2.0);
  }
  samples[0].r = samples[0].r / 2.0;
  samples[0].i = samples[0].i / 2.0;
  //should be able to have the proper samples in the array
}
void fftSub(int n, int offset) {
  uint8_t m;
  struct complex omega;
  struct complex tempComplexX;
  struct complex tempComplexY;
  if (n > 1) {
    m = n / 2;
    shuffle(n, offset);
    fftSub(m, offset);
    fftSub(m, offset + m);
    int lookupPos;
    for (uint8_t i = offset; i < offset + m; i++) {
      //access W values from lookup table
      lookupPos = (i - offset) * NUM_OF_BINS / n;
      omega.i = wLookup[lookupPos].i;
      omega.r = wLookup[lookupPos].r;

      tempComplexX = cProd(samples[i + m], omega);
      tempComplexY = samples[i];
      samples[i] = cSum(samples[i], tempComplexX);
      samples[i + m] = cDiff(tempComplexY, tempComplexX);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  //needed to output sample
  Serial.println("Hello, world!");
  pinMode(LED_BUILTIN, OUTPUT);
  double delayTemp = (1 / SAMPLING_FREQ) * 1000000;  //calculates number here to avoid truncation
  delayUs = (int)delayTemp;                          //the needed delay in the sampling frequency
  delayUs -= 100;                                    //subtract 100 microseconds to account for AnalogIn() delay
  //precompute twiddle factors
  for (int i = 0; i < (NUM_OF_BINS / 2); i++) {
    wLookup[i].r = cos(2.0 * PI * (float)(i) / (float)(NUM_OF_BINS));
    wLookup[i].i = sin(2.0 * PI * (float)(i) / (float)(NUM_OF_BINS));
  }
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
    double incCount = 0;
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
    if (isClip)
      Serial.println("Clipping detected! results may be skewed.");
  }
  radix2fft();
  //ok now we have the FFT in the samples array, now what?
  //TODO: test the fft.
  for (int i = 0; i < NUM_OF_BINS; i++) {
    delay(10);

    Serial.println(samples[i].r);
  }

  delay(1);
}
