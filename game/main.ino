/*
  Memory game — 5 LEDs on pins 2-6, potentiometer on A5, push button on pin 8.

  Round flow:
    1. The game flashes a random sequence of 5 LEDs, one at a time.
    2. A longer dark pause, then the player's LED comes on and follows the pot.
    3. The player dials to an LED and presses; the LED blinks out to confirm.
    4. One wrong entry ends the round.

  Visual language matches the starter sketch: the pot's LED is on, and
  pressing the button turns it off.

  Wiring: button between pin 8 and GND (uses the internal pull-up).
          Leave A0 unconnected — its floating noise seeds the RNG.
*/

const uint8_t LED_PINS[] = {2, 3, 4, 5, 6};
const uint8_t LED_COUNT  = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

const uint8_t POT_PIN    = A5;
const uint8_t BUTTON_PIN = 8;
const uint8_t SEED_PIN   = A0;   // must be left floating

const uint8_t SEQUENCE_LENGTH = 5;

const unsigned long FLASH_ON    = 400;  // ms an LED stays lit during playback
const unsigned long FLASH_OFF   = 200;  // ms gap between flashes
const unsigned long PHASE_PAUSE = 800;  // ms of darkness before the player's turn
const unsigned long DEBOUNCE_MS = 20;

uint8_t sequence[SEQUENCE_LENGTH];

void setup() {
  Serial.begin(9600);
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  randomSeed(analogRead(SEED_PIN));
}

void loop() {
  newSequence();

  Serial.println(F("\n--- New round --- watch the sequence"));
  showSequence();

  Serial.println(F("Your turn: dial to an LED, press to commit."));
  if (playRound()) {
    Serial.println(F("* Correct! You win. *"));
    winAnimation();
  } else {
    Serial.println(F("* Wrong. Round over. *"));
    loseAnimation();
  }

  delay(1500);
}

/* ---------- LED helpers ---------- */

void allOff() {
  for (uint8_t i = 0; i < LED_COUNT; i++) digitalWrite(LED_PINS[i], LOW);
}

void allOn() {
  for (uint8_t i = 0; i < LED_COUNT; i++) digitalWrite(LED_PINS[i], HIGH);
}

void lightOnly(uint8_t index) {
  allOff();
  digitalWrite(LED_PINS[index], HIGH);
}

/* ---------- Pot reading ---------- */

// 0-99 -> 0, 100-199 -> 1, ... 400-499 -> 4, 500-599 -> 0, ... 900-1023 -> 4
uint8_t selectedLed(int potValue) {
  potValue = constrain(potValue, 0, 999);
  return (potValue / 100) % LED_COUNT;
}

/* ---------- Round logic ---------- */

void newSequence() {
  for (uint8_t i = 0; i < SEQUENCE_LENGTH; i++) {
    sequence[i] = random(LED_COUNT);
  }
}

void showSequence() {
  allOff();
  delay(800);
  for (uint8_t i = 0; i < SEQUENCE_LENGTH; i++) {
    lightOnly(sequence[i]);
    delay(FLASH_ON);
    allOff();
    delay(FLASH_OFF);   // the gap is what makes a repeated LED readable
  }
  delay(PHASE_PAUSE);   // longer beat: the sequence is over, your turn next
}

// Returns true only if all SEQUENCE_LENGTH entries matched.
bool playRound() {
  for (uint8_t step = 0; step < SEQUENCE_LENGTH; step++) {
    uint8_t entry = waitForPress();
    Serial.print(F("  step "));
    Serial.print(step + 1);
    Serial.print(F(": LED on pin "));
    Serial.println(LED_PINS[entry]);

    if (entry != sequence[step]) return false;
  }
  return true;
}

// Keeps the pot's LED lit until the button is pressed, then returns whichever
// LED was lit at the moment of the press.
uint8_t waitForPress() {
  waitForRelease();

  uint8_t selected = 0;
  while (true) {
    selected = selectedLed(analogRead(POT_PIN));

    if (digitalRead(BUTTON_PIN) == LOW) {
      delay(DEBOUNCE_MS);
      if (digitalRead(BUTTON_PIN) == LOW) break;   // settled, it's a real press
    }
    lightOnly(selected);
  }

  allOff();          // the LED blinking out is the confirmation
  waitForRelease();
  return selected;
}

void waitForRelease() {
  while (digitalRead(BUTTON_PIN) == LOW) { }
  delay(DEBOUNCE_MS);
}

/* ---------- Feedback animations ---------- */
void winAnimation() {
  allOff();
  delay(200);
  for (int8_t i = LED_COUNT - 1; i >= 0; i--) {   // fill in, one end to the other
    digitalWrite(LED_PINS[i], HIGH);
    delay(120);
  }
  delay(400);
  for (uint8_t i = 0; i < LED_COUNT; i++) {       // empty out, same direction
    digitalWrite(LED_PINS[i], LOW);
    delay(120);
  }
}

void loseAnimation() {
  for (uint8_t blink = 0; blink < 2; blink++) {
    allOn();
    delay(250);
    allOff();
    delay(250);
  }
}
