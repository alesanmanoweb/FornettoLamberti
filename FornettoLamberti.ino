#define ZEROCROSS 13
#define TRIACA 14
#define TRIACB 12
#define POTA 36
#define POTB 39

#define ANALOGINPUTBITS 10
#define CONTROLBITS 6
// range = 64
#define RANGE ((1 << CONTROLBITS) - 1)

// number of microsends before the actual zero crossing we get the interrupt
// tested at 240VAC with 2 x 47k resistors on H11L1
#define TURN_ON_WAIT 840

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
  // turn off everything before the zero-crossing
  digitalWrite(TRIACA, LOW);
  digitalWrite(TRIACB, LOW);

  // wait for the actual zero-crossing
  delayMicroseconds(TURN_ON_WAIT);

  // let's check if we have to turn on
  stateA += powerA;
  if(stateA >= RANGE)
  {
    stateA -= RANGE;
    digitalWrite(TRIACA, HIGH);
  }

  stateB += powerB;
  if(stateB >= RANGE)
  {
    stateB -= RANGE;
    digitalWrite(TRIACB, HIGH);
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
