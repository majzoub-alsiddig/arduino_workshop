#include <Servo.h>

const int TRIG_PIN = 7;
const int ECHO_PIN = 8;
const int SERVO_PIN = 9;

Servo doorServo;

void setup() 
{
	Serial.begin(9600);

	pinMode(TRIG_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);

	doorServo.attach(SERVO_PIN);
	doorServo.write(0);

	Serial.println("Auto mini door ready: ");
}


void loop()
{
	long cm = readDistanceCm();

	if (cm < 0) 
	{
		Serial.println("out of range");
	} 
	else 
	{
		Serial.print("Distance: ");
		Serial.print(cm);
		Serial.println(" cm");

		if (cm <= 20) 
		{
			doorServo.write(90);
			Serial.println("Found target");
		} 
		else 
		{
			doorServo.write(0);
			Serial.println("No taget");
		}
	}
	
	delay(150);
}

long readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  long cm = duration * 0.0343 / 2;
  return cm;
}
