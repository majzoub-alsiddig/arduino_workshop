const int LEDS = {1, 3, 5}; // Red Yellow Green


void setup()
{
	for (int i = 0; i < 3; i++) pinMode(LEDS[i], OUTPUT);
}

void loop()
{
	// loop over all of them
	for(int i = 0; i < 3; i++)
		digitalWrite(LEDS[i], HIGH);
		delay(700);
		digitalWrite(LEDS[i], LOW);
	}
}
