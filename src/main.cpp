// test code for mutual communication between Pi and Arduino:
// Arduino should read (and send) turbidity readings only when the Pi
// commands it to:
// the Pi will then send a second command based on the reading
// which the Arduino will interpret to turn on one of the four channels
// of the test relay (or, if the reading is wrong, turn off all channels):
// I'm also going to keep using the sensor smoothing here because if ever
// there was a time where less noisy readings was important the Turbidity
// logging is definitly it

// needed when dealing with Arduino code outside of the Arduino IDE:
# include <Arduino.h>

// specifying the analog pin for the turbidity sensor:
# define TURBID1 A1
// specifying the number of samples to be averaged to smooth sensor readings:
# define NUMSAMPLES 5

// handing the sample number to the sample variables for smoothing:
int samples_t1[NUMSAMPLES];
// defining the pins for the different relay channels:
int relay1 = 2;
int relay2 = 3;
int relay3 = 4;
int relay4 = 5;

void setup() {
  // opens a port that samples at the standard 9600 bits per second:
  Serial.begin(9600);
  // AREF hooked into the 5V power
  analogReference(EXTERNAL);

  // telling the Arduino that the relay channels will only be receiving
  // commands, not giving them:
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  // so all of the channels start in the off position:
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
}

void loop() {
  // creates an unsinged integer for use as a counter:
  uint8_t i;
  // for use in smoothing noisy sensor readings:
  float ave_turb1;

  // checking if there's anything in Serial that's been sent by the Pi:
  // for this setup the Arduino will only act if told to by the Pi:
  if (Serial.available() > 0) {
    // saving the command the Pi sends
    String command = Serial.readStringUntil('\n');
    // removing any excess whitespace the Pi might have sent with the command
    command.trim();

    if (command.equals("turbidread")) {
      samples_t1[NUMSAMPLES] = 0;
      for (i=0; i< NUMSAMPLES; i++) {
        samples_t1[i] = analogRead(TURBID1);
        delay(50);
      }

      // reseting the average variable between iterations:
      ave_turb1 = 0;
      // averaging the above samples:
      for (i=0; i< NUMSAMPLES; i++) {
        ave_turb1 += samples_t1[i];
      }
      ave_turb1 /= NUMSAMPLES;

      // converting the analog readings (which go from 0-1023) to
      // voltages (0-5v):
      float voltage_t1 = ave_turb1*(5.0/1023.0);


      // sending voltage readings to Serial and the Pi:
      Serial.println(voltage_t1);
    }

    // opening the appropriate relay channel when the Pi says so:
      //I believe these realys open when power is LOW, but need to check:
    // opening channel 1 (should occur for plain water):
    if (command.equals("channel1")) {
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, HIGH);
    }
    // opening channel 2 (should occur for the weak coffee):
    else if (command.equals("channel2")) {
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, HIGH);
    }
    // opening channel 3 (should occur for the strong coffee):
    else if (command.equals("channel3")) {
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, HIGH);
    }
    // opening channel 4 (should occur if the sensor is suspended in air):
    else if (command.equals("channel4")) {
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, LOW);
    }
    // all channels off:
    // should happen when the Python code starts running or
    // when there is an invalid turbidity reading:
    else if (command.equals("off")) {
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, HIGH);
    }
  }
}
