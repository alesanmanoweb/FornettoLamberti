#define ZEROCROSS 13
#define TRIACA 14
#define TRIACB 12
#define POTA 36
#define POTB 39

#define ANALOGINPUTBITS 10
#define CONTROLBITS 6
// range = 64
#define RANGE ((1 << CONTROLBITS) - 1)

//volatile static bool cycle = true;
volatile static uint32_t powerA = 48;
volatile static uint32_t stateA = 0;
volatile static uint32_t powerB = 0;
volatile static uint32_t stateB = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(ZEROCROSS, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ZEROCROSS), isr, RISING);

  pinMode(TRIACA, OUTPUT);
  digitalWrite(TRIACA, LOW);
  pinMode(TRIACB, OUTPUT);
  digitalWrite(TRIACB, LOW);

  pinMode(POTA, INPUT);
  pinMode(POTB, INPUT);
  analogReadResolution(ANALOGINPUTBITS);
}

void isr()
{
  // wait for real cross (15Vrms)
  delayMicroseconds(840); // do not trust too much

  stateA += powerA;
  if(stateA >= RANGE)
  {
    stateA -= RANGE;
    digitalWrite(TRIACA, HIGH);
  }
  else
  {
    digitalWrite(TRIACA, LOW);
  }

  stateB += powerB;
  if(stateB >= RANGE)
  {
    stateB -= RANGE;
    digitalWrite(TRIACB, HIGH);
  }
  else
  {
    digitalWrite(TRIACB, LOW);
  }
}

uint32_t conditionPot(uint32_t value)
{
  value >>= (ANALOGINPUTBITS - CONTROLBITS);
  return value;
}

void loop()
{
  powerA = conditionPot((uint32_t)analogRead(POTA));
  powerB = conditionPot((uint32_t)analogRead(POTB));
  delay(100);
}
