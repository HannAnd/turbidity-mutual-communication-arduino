// test code for mutual communication between Raspberry Pi (3B+) and
// Arduino (Nano Every):
// see README for more detailed description:

// needed when dealing with Arduino code outside of the Arduino IDE:
# include <Arduino.h>

// specifying the analog pins for the turbidity sensors:
# define TURBID1 A1
# define TURBID2 A2
# define TURBID3 A3

// specifying the number of samples to be averaged to smooth (average)
// sensor readings:
# define NUMSAMPLES 5

// handing the sample number to the sample variables for smoothing:
int samples_t[NUMSAMPLES];

// defining the pins for the different relay channels:
int relay1 = 2;
int relay2 = 3;
int relay3 = 4;
int relay4 = 5;
int relay5 = 6;
int relay6 = 7;
int relay7 = 8;
int relay8 = 9;

void setup() {
  // opens a port that samples at the standard 9600 bits per second:
  Serial.begin(9600);
  // AREF hooked into the 5V power
  analogReference(EXTERNAL);

  // sets up relay channels to receive commands
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


// function for setting channel states:
void serialparse(int command) {
  // creates an unsigned integer for use as a counter:
  uint8_t i;
  // for use in smoothing noisy sensor readings:
  float ave_turb;
  float voltage_t;

  switch (command) {

    // read turbidity sensor 1:
    case 1:
    samples_t[NUMSAMPLES] = 0;
    analogRead(TURBID1);  // read to align multiplexer to A1:
    delay(10);
    analogRead(TURBID1);  // unused reading to deal with ADC problems:
    delay(50);

    for (i=0; i< NUMSAMPLES; i++)  {
      samples_t[i] = analogRead(TURBID1);
      delay(50);
    }

    // resetting the average variable between iterations:
    ave_turb = 0;
    // averaging the above samples:
    for (i=0; i< NUMSAMPLES; i++)  {
      ave_turb += samples_t[i];
    }
    ave_turb /= NUMSAMPLES;

    // converting analog readings (which go from 0-1023) to voltages (0-5v):
    voltage_t = ave_turb*(5.0/1023.0);  //might leave analog readings as-is and learn to just convert NTU from that

    // sending voltage readings through Serial to the Pi:
    Serial.println(voltage_t);
    break;

    // read turbidity sensor 2:
    case 2:
    samples_t[NUMSAMPLES] = 0;
    analogRead(TURBID2);  // read to align multiplexer to A2:
    delay(10);
    analogRead(TURBID2);  // unused reading to deal with ADC problems:
    delay(50);

    for (i=0; i< NUMSAMPLES; i++)  {
      samples_t[i] = analogRead(TURBID1);
      delay(50);
    }

    // resetting the average variable between iterations:
    ave_turb = 0;
    // averaging the above samples:
    for (i=0; i< NUMSAMPLES; i++)  {
      ave_turb += samples_t[i];
    }
    ave_turb /= NUMSAMPLES;

    // converting analog readings (which go from 0-1023) to voltages (0-5v):
    voltage_t = ave_turb*(5.0/1023.0);  //might leave analog readings as-is and learn to just convert NTU from that

    // sending voltage readings through Serial to the Pi:
    Serial.println(voltage_t);
    break;

    // read turbidity sensor 3:
    case 3:
    samples_t[NUMSAMPLES] = 0;
    analogRead(TURBID3);  // read to align multiplexer to A3:
    delay(10);
    analogRead(TURBID3);  // unused reading to deal with ADC problems:
    delay(50);

    for (i=0; i< NUMSAMPLES; i++)  {
      samples_t[i] = analogRead(TURBID1);
      delay(50);
    }

    // resetting the average variable between iterations:
    ave_turb = 0;
    // averaging the above samples:
    for (i=0; i< NUMSAMPLES; i++)  {
      ave_turb += samples_t[i];
    }
    ave_turb /= NUMSAMPLES;

    // converting analog readings (which go from 0-1023) to voltages (0-5v):
    voltage_t = ave_turb*(5.0/1023.0);  //might leave analog readings as-is and learn to just convert NTU from that

    // sending voltage readings through Serial to the Pi:
    Serial.println(voltage_t);
    break;

    // all cases below are for opening the appropriate channel at the
    // command of the Pi:

    // opening channel 1 (should occur for plain water):
    case 4:
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    break;

    // opening channel 2 (should occur for the weak coffee):
    case 5:
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    break;

    // opening channel 3 (should occur for the strong coffee):
    case 6:
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, HIGH);
    break;

    // opening channel 4 (should occur if the sensor is suspended in air):
    case 7:
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, LOW);
    break;

    // all channels off:
    // should happen when the Python code starts running or
    // when there is an invalid turbidity reading:
    case 8:
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    break;

    default:
    // no action:
    break;
  }
}

void loop() {
  // checking if there's anything in Serial that's been sent by the Pi:
  // for this setup the Arduino will only act if told to by the Pi:
  if (Serial.available() > 0) {
    // saving the command the Pi sends
    String pi_out = Serial.readStringUntil('\n');
    // removing any excess whitespace the Pi might have sent with the command
    pi_out.trim();
    // converting the Serial string to integer for the switch case:
    int command = pi_out.toInt();

    serialparse(command);

  }
}
